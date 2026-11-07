// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "AutoFinance.h"

#include "CivicFacilityIO.h"
#include "cISC4City.h"
#include "cISC4ResidentialSimulator.h"
#include "Logging.h"
#include "QuerySemantics.h"

#include "cISC4City.h"

#include <windows.h>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <tuple>

namespace sc4af {

// Writer shapes whose setter behaviour has been verified; extend only with a verified shape.
bool IsValidatedWriterShape(WriterShape shape) {
    switch (shape) {
    case WriterShape::EducationWithSecondary:  // elementary school
    case WriterShape::HealthWithSecondary:     // clinic
    case WriterShape::EducationStaffOnly:      // college (setter skips the absent secondary line)
    case WriterShape::HealthStaffOnly:         // Disease Research Center
        return true;
    default:
        return false;
    }
}

// Library/museum/patron semantics come from the game's own query labels (books checked out vs
// books in collection, visitors vs exhibit tickets, ...).
bool SpecialSignalsVerified() { return true; }

namespace {

std::uint64_t FileWriteTime(const std::string& path) {
    WIN32_FILE_ATTRIBUTE_DATA data{};
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) return 0;
    return (static_cast<std::uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime;
}

std::string Fmt(float v) {
    std::ostringstream s;
    s << std::setprecision(9) << v;
    return s.str();
}

bool FamilyEnabled(const AutoFinanceConfig& c, FundingFamily f) {
    return f == FundingFamily::School ? c.education : c.health;
}

// A facility is writer-eligible when the policy passed every structural check (line, layout, coherence).
bool WriterEligible(Action a) {
    return a == Action::Write || a == Action::NoChange || a == Action::WaitForSample || a == Action::InsufficientCapacity;
}

std::string EligibilityKey(const LiveFacility& f, const Decision& d) {
    return std::string(ActionName(d.action)) + '|' + (f.state.fundingCoherent ? "COHERENT" : f.state.coherenceReason);
}

// Why this facility is (not) writer-eligible, with every channel the decision rests on.
std::string DescribeEligibility(const LiveFacility& f, const Decision& d) {
    const auto& s = f.state;
    const auto& r = f.readback;
    std::ostringstream o;
    o << "[AutoFinance][Eligibility] " << (s.family == FundingFamily::School ? "School/" : "Health/") << s.subtype
      << " cell=" << s.cellX << ',' << s.cellZ << " writerEligibility=" << (WriterEligible(d.action) ? "ELIGIBLE" : ActionName(d.action))
      << " fundingCoherence=" << (s.fundingCoherent ? "COHERENT" : "INCOHERENT") << " getter=" << Fmt(r.getterBuilding)
      << " buffer=" << Fmt(r.bufferBuilding) << " budget=" << Fmt(r.budgetBuilding);
    if (s.hasSecondary)
        o << " secondary(getter/buffer/budget)=" << Fmt(r.getterSecondary) << '/' << Fmt(r.bufferSecondary) << '/' << Fmt(r.budgetSecondary);
    o << " shape=" << ShapeName(s.shape) << " layout=0x" << std::hex << s.queryLayout << std::dec << " baseCost=" << s.baseCost
      << " reason=" << (s.fundingCoherent ? d.reason : s.coherenceReason + "; " + d.reason);
    return o.str();
}

} // namespace

void AutoFinance::LoadConfigIfChanged() {
    if (configPath_.empty()) return;
    const auto stamp = FileWriteTime(configPath_);
    if (stamp == lastWriteTime_) return;
    lastWriteTime_ = stamp;
    config_ = AutoFinanceConfig{};  // Mode=Off unless the file says otherwise and is valid
    if (stamp == 0) {
        Log(LogLevel::Info, "[AutoFinance] no DepartmentEfficiency.ini; Mode=OFF");
        return;
    }
    std::ifstream in(configPath_, std::ios::binary);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string error;
    if (!ParseAutoFinanceConfig(buffer.str(), config_, error)) {
        config_ = AutoFinanceConfig{};
        Log(LogLevel::Warning, "[AutoFinance] config invalid (" + error + "); Mode=OFF");
        return;
    }
    Log(LogLevel::Info, "[AutoFinance] config loaded: " + Banner());
}

std::string AutoFinance::Banner() const {
    std::ostringstream s;
    s << "AutoFinance Mode=" << AutoModeName(config_.mode) << " Reserve=" << config_.reservePercent << "% EducationReserve=" << config_.EducationReserve() << "% HealthReserve=" << config_.HealthReserve()
      << "% Education=" << (config_.education ? "true" : "false") << " Health=" << (config_.health ? "true" : "false")
      << " Range=" << config_.minFundingPercent << "-" << config_.maxFundingPercent
      << "% AutoShapes=EDUCATION_WITH_SECONDARY,EDUCATION_STAFF_ONLY,HEALTH_WITH_SECONDARY,HEALTH_STAFF_ONLY"
      << " Eligible=validated vanilla mechanisms only (vanilla query layouts: school/college/library/museum/health)"
      << " (NOT_LOCALLY_FUNDED / NON_VANILLA_MECHANISM / UNSAFE_OR_UNMAPPED are never written)";
    return s.str();
}

void AutoFinance::TripOnUnverifiedWrite(CivicWriteOutcome outcome, const std::string& reason, const FacilityKey& facility, const CivicSession::FacilityIdentity& identity) {
    if (!CivicWriterGuard::TripsWriter(outcome)) return;
    session_.QuarantineNotWritable(facility, identity);   // THIS facility is out of the writer for the city lifecycle; the others continue
    const bool trippedNow = guard_.Observe(outcome);
    Log(LogLevel::Warning, "[AutoFinance][Safety] POST_WRITE_UNVERIFIED " + std::string(std::get<0>(facility) == 0 ? "School/" : "Health/") + identity.subtype + " cell=" +
                               std::to_string(std::get<1>(facility)) + ',' + std::to_string(std::get<2>(facility)) + " reason=" + reason +
                               "; this facility is excluded for the city lifecycle, the other facilities continue (" + std::to_string(guard_.failedFacilities) + " of " +
                               std::to_string(CivicWriterGuard::kTripAfterFacilities) + " before the writer itself is distrusted)");
    if (trippedNow)  // only on the transition: one line per city lifecycle, never monthly
        Log(LogLevel::Error, "[AutoFinance][Safety] " + std::to_string(guard_.failedFacilities) + " different facilities could not be verified: the writer itself is distrusted; Civic Auto writes DISABLED for this city lifecycle");
}

namespace {
constexpr std::uint32_t kWatchIntervalDays = 2;  // the risk watch enumerates the facilities every second simulated day (every day while the game reports a strike)

std::tuple<int, int, int> IdOf(const LiveFacility& f) { return std::make_tuple(static_cast<int>(f.state.family), f.state.cellX, f.state.cellZ); }
}  // namespace

void AutoFinance::OnDay(cISC4City* city, std::uint32_t month) {
    if (config_.mode != AutoMode::Auto || !city) return;
    ++dayInCity_;
    // The first simulated day after opening is the earliest safe moment: the city is fully loaded by then, which PostCityInit does not guarantee.
    if (open_.pending()) {
        Evaluate(city, month, EvalTrigger::Open);
        open_.Evaluated(lastEvalComplete_);
        return;
    }
    // Use ONLY the two argument-less bool getters (vtable entries 3 and 4: `mov al, [ecx+off]; test; setne al; ret`). The SDK header declares
    // ChanceOfSchoolStrike / ChanceOfHealthStrike as `float f(void)`, but in this executable those entries take a stack argument and write through it (`ret 4`):
    // calling them as declared crashes the game. They are NEVER called.
    auto* residential = city->GetResidentialSimulator();
    const bool strikeNow = residential && (residential->SchoolIsOnStrike() || residential->HealthIsOnStrike());
    if (!strikeNow && dayInCity_ % kWatchIntervalDays != 0) return;
    WatchTick(city, month, strikeNow);
}

// The risk watch: per-building coverage (the game's own strike input), strike state and the demand the policy would now serve. Reads only; an evaluation (raises only) runs when a risk
// appears or changes, and again every kRecheckDays while it persists.
void AutoFinance::WatchTick(cISC4City* city, std::uint32_t month, bool strikeNow) {
    PolicyConfig policy;
    policy.reservePercent = config_.reservePercent;
    policy.educationReservePercent = config_.EducationReserve();
    policy.healthReservePercent = config_.HealthReserve();
    policy.minFundingPercent = config_.minFundingPercent;
    policy.maxFundingPercent = config_.maxFundingPercent;
    auto enumeration = EnumerateFacilities(city, true);
    if (enumeration.validity == SnapshotValidity::Invalid || enumeration.validity == SnapshotValidity::ValidEmpty) return;
    bool trigger = false;
    std::string first;
    for (auto& f : enumeration.facilities) {
        if (!FamilyEnabled(config_, f.state.family)) continue;
        const auto id = IdOf(f);
        const CivicSession::FacilityIdentity identity{f.state.subtype, f.state.baseCost, f.state.baseCapacity};
        if (session_.IsNotWritable(id, identity)) continue;
        const StrikeSignals strike = ReadStrikeSignals(f.occupant, f.state.family);
        f.state.strikeCoverage = strike.coverage;
        f.state.onStrike = strike.strikeSince && *strike.strikeSince != 0;
        if (f.state.usage) f.state.usageVolatility = CivicSession::WithIntakeAssumption(f.state.subtype, session_.UsageSamples(id), session_.UpwardVolatility(id));
        f.state.capacityPrior = session_.CapacityPriorFor(id, identity);
        const Attention a = NeedsAttention(f.state, policy);
        if (!a.needed) { latch_.Clear(id); continue; }
        const std::string signature = a.why + '|' + (f.state.usage ? std::to_string(*f.state.usage) : std::string("?")) + '|' + std::to_string(f.state.capacity) + '|' + std::to_string(PercentForFraction(f.state.buildingFunding));
        if (latch_.Trigger(id, signature, dayInCity_)) {
            trigger = true;
            if (first.empty()) first = std::string(f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype + " cell=" + std::to_string(f.state.cellX) + ',' + std::to_string(f.state.cellZ) + ": " + a.why;
        }
    }
    if (!trigger) return;
    Log(LogLevel::Info, std::string("[AutoFinance][Watch] day=") + std::to_string(dayInCity_) + " gameStrikeFlag=" + (strikeNow ? "1" : "0") + " first: " + first + "; evaluating now instead of waiting for the monthly pass");
    Evaluate(city, month, EvalTrigger::Watch);
}

void AutoFinance::OnMonth(cISC4City* city, std::uint32_t month) {
    LoadConfigIfChanged();
    evidence_ = CivicMonthEvidence{};
    Evaluate(city, month, EvalTrigger::Month);
}

void AutoFinance::Evaluate(cISC4City* city, std::uint32_t month, EvalTrigger trigger) {
    const bool monthly = trigger == EvalTrigger::Month;
    CivicMonthEvidence scratch;                                  // Open / Watch evaluations never touch the monthly evidence
    CivicMonthEvidence& E = monthly ? evidence_ : scratch;
    if (config_.mode == AutoMode::Off || !city) return;
    lastEvalComplete_ = false;                                   // set at the end of a valid evaluation that found a usage sample for every facility

    PolicyConfig policy;
    policy.reservePercent = config_.reservePercent;
    policy.educationReservePercent = config_.EducationReserve();
    policy.healthReservePercent = config_.HealthReserve();
    policy.minFundingPercent = config_.minFundingPercent;
    policy.maxFundingPercent = config_.maxFundingPercent;

    auto enumeration = EnumerateFacilities(city, true);
    auto& facilities = enumeration.facilities;

    std::vector<ObservedFunding> present;
    for (const auto& f : facilities)
        present.push_back({std::make_tuple(static_cast<int>(f.state.family), f.state.cellX, f.state.cellZ),
                           f.state.buildingFunding});
    // Validity, empty/invalid transitions and the phantom-slider watch live in the pure session model.
    // A facility we wrote earlier must still hold what we wrote; a different value is a player edit or
    // drift, logged either way, and the expectation is dropped.
    const auto event = session_.Observe(enumeration.validity, present);
    for (const auto& d : event.drifts) {
        Log(LogLevel::Warning, "[AutoFinance][DRIFT] " + std::string(std::get<0>(d.key) == 0 ? "School" : "Health") +
            " cell=" + std::to_string(std::get<1>(d.key)) + ',' + std::to_string(std::get<2>(d.key)) +
            " expected " + Fmt(d.expected) + " found " + Fmt(d.found));
    }
    if (event.logTransition) {
        std::ostringstream t;
        t << "[AutoFinance][Snapshot] " << event.message << " schoolsList=" << (enumeration.input.schools.available ? "ok" : "FAILED")
          << ':' << enumeration.input.schools.count << " hospitalsList=" << (enumeration.input.hospitals.available ? "ok" : "FAILED")
          << ':' << enumeration.input.hospitals.count << " read=" << enumeration.input.readOk << " unreadable="
          << enumeration.input.readFailed;
        if (!enumeration.skipReasons.empty()) t << " skip=" << enumeration.skipReasons;
        if (enumeration.input.notOfFamily) t << " setAside=" << enumeration.input.notOfFamily << "(" << enumeration.notOfFamily << ": listed by the game, not a school / hospital)";
        Log(event.severity == LogSeverityHint::Warning ? LogLevel::Warning : LogLevel::Info, t.str());
    }
    if (enumeration.input.notOfFamily && enumeration.notOfFamily != notOfFamilyLogged_) {      // once per city (and again only if the set changes)
        notOfFamilyLogged_ = enumeration.notOfFamily;
        Log(LogLevel::Info, "[AutoFinance][Snapshot] set aside " + std::to_string(enumeration.input.notOfFamily) + " occupant(s) the game lists as school / hospital but that are not one, or have no local funding line (" +
                                enumeration.notOfFamily + "): never funded, never counted as unreadable, they never block the others");
    }
    // INVALID: no writes, no cached substitute, no per-month noise beyond the rate-limited line above.
    if (enumeration.validity == SnapshotValidity::Invalid) return;
    // VALID_EMPTY: nothing to manage. Silent in steady state (transitions were logged above).
    if (enumeration.validity == SnapshotValidity::ValidEmpty) return;
    if (enumeration.validity == SnapshotValidity::Partial && !enumeration.skipReasons.empty() && event.logTransition)
        Log(LogLevel::Warning, "[AutoFinance][Snapshot] PARTIAL: some facilities were unreadable and are skipped this month");

    E.evaluated = config_.mode == AutoMode::Auto && !guard_.tripped;
    shortage_ = CivicShortage{};
    shortage_.evaluated = true;
    if (monthly) session_.NextMonth();
    int counts[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    std::int64_t potentialSaving = 0, appliedSaving = 0;
    int applied = 0, failed = 0, unsampled = 0;
    int strikeSeen[2] = {0, 0}, strikeLow[2] = {0, 0}, strikeNear[2] = {0, 0}, strikeOn[2] = {0, 0};

    for (auto& f : facilities) {
        if (!FamilyEnabled(config_, f.state.family)) continue;
        const auto facilityId = std::make_tuple(static_cast<int>(f.state.family), f.state.cellX, f.state.cellZ);
        const CivicSession::FacilityIdentity identity{f.state.subtype, f.state.baseCost, f.state.baseCapacity};
        // A facility whose setter was proven to have no effect stays out of the writer for this city lifecycle (no monthly retry, no evidence, no economics).
        if (session_.IsNotWritable(facilityId, identity)) continue;
        const StrikeSignals strike = ReadStrikeSignals(f.occupant, f.state.family);          // the game's own strike inputs of this building (read only)
        f.state.strikeCoverage = strike.coverage;
        f.state.onStrike = strike.strikeSince && *strike.strikeSince != 0;
        if (f.state.usage) {
            if (monthly) session_.ObserveUsage(facilityId, *f.state.usage);          // the monthly history is sampled once a month, never by the day-level evaluations
            f.state.usageVolatility = CivicSession::WithIntakeAssumption(f.state.subtype, session_.UsageSamples(facilityId), session_.UpwardVolatility(facilityId));
        }
        f.state.capacityPrior = session_.CapacityPriorFor(facilityId, identity);       // what this city has taught about THIS facility's real capacity curve
        Decision d = DecideFunding(f.state, policy);
        // Open / Watch evaluations only RAISE: a reduction (a saving) and a calibration probe wait for the monthly pass. The service first, the economy after.
        if (!monthly && ((d.write && d.targetFraction < f.state.buildingFunding - 0.005f) || d.calibration)) {
            d.write = false;
            d.calibration = false;
            d.action = Action::NoChange;
            d.targetPercent = PercentForFraction(f.state.buildingFunding);
            d.targetFraction = f.state.buildingFunding;
            d.targetCost = d.currentCost;
            d.saving = 0;
            d.reason = "reduction / calibration deferred to the monthly pass";
        }
        {   // strike episodes of this building: start and recovery are logged (the recovery is what the next evaluation reads back from the game, never assumed)
            if (f.state.onStrike) {
                if (!strikeStart_.count(facilityId)) {
                    strikeStart_[facilityId] = dayInCity_;
                    Log(LogLevel::Info, std::string("[AutoFinance][Strike] START ") + (f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype + " cell=" + std::to_string(f.state.cellX) + ',' +
                        std::to_string(f.state.cellZ) + " coverage=" + (strike.coverage ? Fmt(*strike.coverage) : std::string("n/a")) + " funding=" + std::to_string(PercentForFraction(f.state.buildingFunding)) + "%: funding goes to the maximum (" + TriggerName(trigger) + ")");
                }
            } else if (strikeStart_.count(facilityId)) {
                Log(LogLevel::Info, std::string("[AutoFinance][Strike] RECOVERED ") + (f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype + " cell=" + std::to_string(f.state.cellX) + ',' +
                    std::to_string(f.state.cellZ) + " after " + std::to_string(dayInCity_ - strikeStart_[facilityId]) + " simulated day(s); coverage now " + (strike.coverage ? Fmt(*strike.coverage) : std::string("n/a")));
                strikeStart_.erase(facilityId);
            }
        }
        ++counts[static_cast<int>(d.action)];
        if (!f.state.usage && d.action != Action::NotWritable && d.action != Action::NonVanillaMechanism && d.action != Action::NoPolicy) ++unsampled;   // raised to the safe funding, but the evaluation is not complete until it has a sample
        // the factor seen now becomes the prior of the next evaluation (a confirmed factor only survives while the observation keeps agreeing with it)
        {
            const bool spent = f.state.capacityPrior && f.state.capacityPrior->probed;     // the one calibration write of this facility is remembered across evaluations
            const bool keep = d.capacityEvidence == CapacityEvidence::Provisional || d.capacityEvidence == CapacityEvidence::Confirmed || d.capacityEvidence == CapacityEvidence::Incompatible;
            session_.RememberCapacity(facilityId, identity,
                                      keep ? std::optional<CapacityPrior>(CapacityPrior{d.capacityFactor, d.capacityEvidence == CapacityEvidence::Confirmed, spent,
                                                                                         d.capacityEvidence == CapacityEvidence::Incompatible})
                                           : std::nullopt);
        }
        CivicFamilyEvidence& ev = f.state.family == FundingFamily::School ? E.education : E.health;
        ++ev.facilities;
        if (d.write && !d.calibration) ++ev.writeDecisions;      // a calibration raise is not a write decision of the policy
        if (d.action == Action::InsufficientCapacity) ++ev.insufficient;
        std::ostringstream row;
        row << "[AutoFinance][Decision] month=" << month << ' ' << (f.state.family == FundingFamily::School ? "School" : "Health")
            << '/' << f.state.subtype << " cell=" << f.state.cellX << ',' << f.state.cellZ
            << " shape=" << ShapeName(f.state.shape) << " signal=" << PolicyKindName(f.state.kind) << " usage=";
        if (f.state.usage) row << *f.state.usage; else row << "UNSAMPLED";
        row << " capacity=" << f.state.capacity << " base=" << f.state.baseCapacity << " capModel=" << CapacityEvidenceName(d.capacityEvidence) << (d.capacityEvidence == CapacityEvidence::UnitModel ? std::string() : " x" + Fmt(static_cast<float>(d.capacityFactor)) + " plan x" + Fmt(static_cast<float>(d.planFactor))) << " required=" << d.required
            << " funding=" << Fmt(f.state.buildingFunding);
        if (d.action == Action::Write || d.action == Action::NoChange || d.action == Action::InsufficientCapacity)
            row << "->" << d.targetPercent << "% cost=" << d.currentCost << "->" << d.targetCost << " saving=" << d.saving;
        row << " secondary=";
        if (f.state.hasSecondary) row << Fmt(f.readback.getterSecondary); else row << "none";
        row << " action=" << ActionName(d.action) << " (" << d.reason << ")";
        if (f.state.usageVolatility > 0.0) row << " volatility=" << std::fixed << std::setprecision(3) << f.state.usageVolatility << std::defaultfloat;
        // What funding cannot solve, PROVEN: at the maximum funding the capacity attainable (the capacity WITHOUT the strike's staff cut) is still below the safe coverage of the demand.
        {
            const ShortageJudgement sj = JudgeShortage(f.state, policy);
            if (sj.kind == ShortageKind::Proven) {
                const CivicType kind = CivicKindOf(f.state.subtype);          // only the supported vanilla types are ever announced
                // a strike in progress is the game's own native alert: the Department says nothing about a building that is on strike (it is not counted; it counts again once the strike is over)
                if (kind != CivicType::None && !f.state.onStrike && f.state.hasPos) {
                    auto& ks = shortage_.kinds[static_cast<int>(kind)];
                    if (ks.count == 0 || sj.potentialCoverage < ks.coverage) {
                        ks.detail = sj.detail;
                        ks.coverage = sj.potentialCoverage;
                    }
                    ++ks.count;
                    AffectedBuilding b;
                    b.cellX = f.state.cellX;
                    b.cellZ = f.state.cellZ;
                    b.posX = f.state.posX;
                    b.posZ = f.state.posZ;
                    b.hasPos = true;
                    b.coverage = sj.potentialCoverage;
                    ks.buildings.push_back(std::move(b));
                }
            } else if (sj.kind == ShortageKind::TransientStrike) {
                ++shortage_.transientStrikes;
            }
        }
        // the game's own strike inputs of this building (read only): coverage < 0.8 rolls for a strike, strikeSince != 0 is a strike in progress
        {
            row << " coverage=";
            if (strike.coverage) row << std::fixed << std::setprecision(3) << *strike.coverage << std::defaultfloat; else row << "n/a";
            row << " strikeSince=";
            if (strike.strikeSince) row << *strike.strikeSince; else row << "n/a";
            ++strikeSeen[static_cast<int>(f.state.family)];
            if (strike.coverage && *strike.coverage < 0.8f) ++strikeLow[static_cast<int>(f.state.family)];
            else if (strike.coverage && *strike.coverage < 0.9f) ++strikeNear[static_cast<int>(f.state.family)];
            if (strike.strikeSince && *strike.strikeSince != 0) ++strikeOn[static_cast<int>(f.state.family)];
        }
        if (trigger != EvalTrigger::Watch || d.write || d.action == Action::InsufficientCapacity || f.state.onStrike || (strike.coverage && *strike.coverage < kWatchCoverage))
            Log(LogLevel::Info, row.str());

        // Eligibility evidence: first sight of a facility and every change of verdict (never monthly spam),
        // plus once more right before a write when it was not just logged.
        const auto facilityKey = std::make_tuple(static_cast<int>(f.state.family), f.state.cellX, f.state.cellZ);
        const std::string eligibilityKey = EligibilityKey(f, d);
        auto seen = eligibilitySeen_.find(facilityKey);
        bool eligibilityLogged = false;
        if (seen == eligibilitySeen_.end() || seen->second != eligibilityKey) {
            Log(LogLevel::Info, DescribeEligibility(f, d));
            eligibilitySeen_[facilityKey] = eligibilityKey;
            eligibilityLogged = true;
        }

        if (config_.mode != AutoMode::Auto || !d.write || guard_.blocked) continue;
        if (guard_.tripped) continue;
        if (!eligibilityLogged) Log(LogLevel::Info, DescribeEligibility(f, d));
        if (!IsValidatedWriterShape(f.state.shape)) {
            Log(LogLevel::Info, "[AutoFinance][Skip] writer shape not validated yet: " + std::string(ShapeName(f.state.shape)));
            continue;
        }

        // ---- compare-and-set: re-read this facility right before writing ----
        LiveFacility fresh;
        if (!ReadFacility(city, f.occupant, f.state.family, SpecialSignalsVerified(), fresh) ||
            !fresh.state.fundingCoherent ||
            !NearlyEqual(fresh.state.buildingFunding, f.state.buildingFunding) ||
            fresh.state.hasSecondary != f.state.hasSecondary ||
            (fresh.state.hasSecondary &&
             !NearlyEqual(fresh.readback.getterSecondary, f.readback.getterSecondary, kSecondaryExactTolerance))) {
            // CivicWriteOutcome::PreWriteSkip: the setter has not run, nothing changed; this never trips the writer.
            Log(LogLevel::Warning, "[AutoFinance][Write] SKIP state changed under us: " + DescribeFacility(f));
            ++failed;
            continue;
        }
        const float secondaryBefore = fresh.readback.getterSecondary;
        WriteBuildingFunding(city, fresh, d.targetFraction);  // setter boolean is not a validity flag

        LiveFacility after;
        if (!ReadFacility(city, f.occupant, f.state.family, SpecialSignalsVerified(), after)) {
            Log(LogLevel::Error, "[AutoFinance][Write] VERIFY FAIL cannot re-read facility after write: " + DescribeFacility(f));
            ++failed;
            ++ev.failed;
            TripOnUnverifiedWrite(CivicWriteOutcome::PostWriteRereadFailed, "cannot re-read facility after write", facilityId, identity);
            continue;
        }
        const auto verdict = VerifyReadback(after.readback, d.targetFraction, secondaryBefore, fresh.state.hasSecondary, &fresh.readback);
        const bool noEffect = !verdict.pass && SetterHadNoEffect(fresh.readback, after.readback, d.targetFraction);
        if (fresh.state.hasSecondary && !noEffect) {      // a proven no-effect attempt is not readback evidence either way
            ++ev.secondaryChecked;
            if (!verdict.pass && verdict.reason.find("secondary") != std::string::npos) ++ev.secondaryDrift;
        }
        const CalibrationOutcome co = d.calibration ? ClassifyCalibration(verdict.pass, verdict.reason, after.readback, d.required, policy.capacityJitter) : CalibrationOutcome::Unverified;
        if (d.calibration && co == CalibrationOutcome::Verified) {
            // A controlled calibration raise, fully verified: it is NOT a normal write (no applied / direction / saving / service counters), it only teaches the capacity curve.
            ++ev.calibrationWrites;
            session_.RecordWrite(facilityId, after.readback.getterBuilding);
            auto learned = LearnFromWrite(f.state, after.readback.getterBuilding, after.readback.capacity, CapacityCalibration{d.capacityEvidence, d.capacityFactor, std::string()}, policy);
            if (learned) learned->probed = true;
            session_.RememberCapacity(facilityId, identity, learned);
            Log(LogLevel::Info, "[AutoFinance][Calibration] PASS " + std::string(f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype + " cell=" + std::to_string(f.state.cellX) + ',' +
                std::to_string(f.state.cellZ) + " funding " + Fmt(f.state.buildingFunding) + "->" + Fmt(after.readback.getterBuilding) + " capacity " + std::to_string(f.state.capacity) + "->" +
                std::to_string(after.readback.capacity) + " factor read before=" + Fmt(static_cast<float>(d.capacityFactor)) + " measured after=" +
                (learned ? Fmt(static_cast<float>(learned->factor)) + (learned->confirmed ? " CONFIRMED" : " NOT_CONFIRMED") : std::string("n/a")) + " (secondary preserved; not counted as a normal write)");
        } else if (d.calibration && co == CalibrationOutcome::ServiceViolation) {
            // The funding may have verified, but the capacity no longer covers usage x reserve: a service violation, whatever the curve is. No rollback (no transaction is known to be safe): the
            // existing protection applies (the session fails and Civic writes stop for this city lifecycle).
            ++ev.serviceBreach;
            ++ev.failed;
            ++failed;
            Log(LogLevel::Error, "[AutoFinance][Calibration] SERVICE_VIOLATION " + std::string(f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype + " cell=" +
                std::to_string(f.state.cellX) + ',' + std::to_string(f.state.cellZ) + " funding " + Fmt(f.state.buildingFunding) + "->" + Fmt(after.readback.getterBuilding) + " capacity " +
                std::to_string(f.state.capacity) + "->" + std::to_string(after.readback.capacity) + " required=" + std::to_string(d.required) + " verification=" + (verdict.pass ? std::string("PASS") : verdict.reason));
            TripOnUnverifiedWrite(CivicWriteOutcome::PostWriteVerifyFailed, "calibration write left the capacity below usage*reserve", facilityId, identity);
        } else if (verdict.pass) {
            ++ev.applied;
            const bool raise = d.targetFraction > f.state.buildingFunding;
            if (raise) ++ev.raises; else ++ev.cuts;
            CivicWriteEvent we;
            we.cellX = f.state.cellX; we.cellZ = f.state.cellZ; we.direction = raise ? 1 : -1;
            we.usageKnown = f.state.usage.has_value();
            we.usage = f.state.usage ? static_cast<long long>(*f.state.usage) : 0;
            we.baseCapacity = static_cast<long long>(f.state.baseCapacity);
            ev.writes.push_back(we);
            ++ev.serviceChecked;
            if (ServiceBreached(d, after.readback.capacity, policy.capacityJitter)) ++ev.serviceBreach;       // a facility already at the best legal setting that still cannot cover the demand is `insufficient`, not a breach
            ++applied;
            appliedSaving += d.saving;
            session_.RecordWrite(std::make_tuple(static_cast<int>(f.state.family), f.state.cellX, f.state.cellZ),
                                 after.readback.getterBuilding);
            // The capacity measured after the write, at a different funding, either confirms the factor read before it or leaves it provisional.
            session_.RememberCapacity(facilityId, identity,
                                      LearnFromWrite(f.state, after.readback.getterBuilding, after.readback.capacity, CapacityCalibration{d.capacityEvidence, d.capacityFactor, std::string()}, policy));
            Log(LogLevel::Info, "[AutoFinance][Write] PASS " + std::string(f.state.family == FundingFamily::School ? "School/" : "Health/") +
                f.state.subtype + " cell=" + std::to_string(f.state.cellX) + ',' + std::to_string(f.state.cellZ) +
                " funding " + Fmt(f.state.buildingFunding) + "->" + Fmt(after.readback.getterBuilding) + " cost " +
                std::to_string(f.state.monthlyCost) + "->" + std::to_string(after.readback.monthlyCost) + " capacity " +
                std::to_string(f.state.capacity) + "->" + std::to_string(after.readback.capacity) + " required=" +
                std::to_string(d.required) + " secondary preserved");
        } else if (d.calibration && co == CalibrationOutcome::IncompatibleServicePreserved) {
            // Every funding channel, the secondary line and the cost verified (the capacity check is the LAST one); only the capacity did not follow a constant factor. Not a partial write:
            // no curve is invented, the facility is left without automatic reduction and is not probed again.
            ++ev.curveIncompatible;
            session_.RecordWrite(facilityId, after.readback.getterBuilding);
            session_.RememberCapacity(facilityId, identity, CapacityPrior{d.capacityFactor, false, true, true});
            Log(LogLevel::Warning, "[AutoFinance][Calibration] CURVE_INCOMPATIBLE " + std::string(f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype + " cell=" +
                std::to_string(f.state.cellX) + ',' + std::to_string(f.state.cellZ) + " funding " + Fmt(f.state.buildingFunding) + "->" + Fmt(after.readback.getterBuilding) + " capacity " +
                std::to_string(f.state.capacity) + "->" + std::to_string(after.readback.capacity) + ": the capacity did not follow a constant factor but still covers usage*reserve (required=" +
                std::to_string(d.required) + "); no automatic reduction and no further calibration for this facility");
        } else if (noEffect) {
            // Proven: the complete readback after the setter is identical to the one before. Nothing was applied, so there is nothing to distrust: exclude this facility only.
            session_.QuarantineNotWritable(facilityId, CivicSession::FacilityIdentity{f.state.subtype, f.state.baseCost, f.state.baseCapacity});
            ++ev.notWritable;
            if (!d.calibration && ev.writeDecisions > 0) --ev.writeDecisions;     // an attempt that applied nothing is not a write decision either
            Log(LogLevel::Warning, "[AutoFinance][Write] SETTER_NO_EFFECT NOT_WRITABLE " + std::string(f.state.family == FundingFamily::School ? "School/" : "Health/") + f.state.subtype +
                " cell=" + std::to_string(f.state.cellX) + ',' + std::to_string(f.state.cellZ) + " target=" + Fmt(d.targetFraction) + " the readback after the setter is identical to the one before (" +
                verdict.reason + "); this facility is excluded for the city lifecycle, no applied write, no saving counted; other facilities continue");
        } else {
            ++failed;
            ++ev.failed;
            Log(LogLevel::Error, "[AutoFinance][Write] VERIFY FAIL reason=" + verdict.reason + " " + DescribeFacility(after));
            // An unverified post-write state (not only a secondary drift) excludes THIS facility for the city lifecycle; three different ones distrust the writer.
            TripOnUnverifiedWrite(CivicWriteOutcome::PostWriteVerifyFailed, verdict.reason, facilityId, identity);
        }
    }

    lastEvalComplete_ = counts[static_cast<int>(Action::WaitForSample)] == 0 && unsampled == 0;
    std::ostringstream summary;
    summary << "[AutoFinance][Summary] trigger=" << TriggerName(trigger) << " day=" << dayInCity_ << " month=" << month << " mode=" << AutoModeName(config_.mode)
            << " facilities=" << facilities.size() << " write=" << counts[static_cast<int>(Action::Write)]
            << " nochange=" << counts[static_cast<int>(Action::NoChange)]
            << " wait=" << counts[static_cast<int>(Action::WaitForSample)]
            << " insufficient=" << counts[static_cast<int>(Action::InsufficientCapacity)]
            << " unsafe_or_unmapped=" << counts[static_cast<int>(Action::NoPolicy)]
            << " not_locally_funded=" << counts[static_cast<int>(Action::NotWritable)]
            << " nonvanilla=" << counts[static_cast<int>(Action::NonVanillaMechanism)]
            << " potentialSaving=" << potentialSaving;
    if (config_.mode == AutoMode::Auto)
        summary << " applied=" << applied << " failed=" << failed << " appliedSaving=" << appliedSaving;
    // strike inputs (read only): buildings whose coverage is below the 0.8 knee (they roll for a strike), in 0.8..0.9, and strikes in progress; index 0 = schools, 1 = hospitals
    if (auto* residential = city->GetResidentialSimulator())
        summary << " strikeState{schoolOnStrike=" << residential->SchoolIsOnStrike() << " healthOnStrike=" << residential->HealthIsOnStrike() << "}";
    summary << " strike{schools seen=" << strikeSeen[0] << " cov<0.8=" << strikeLow[0] << " cov<0.9=" << strikeNear[0] << " onStrike=" << strikeOn[0]
            << "; hospitals seen=" << strikeSeen[1] << " cov<0.8=" << strikeLow[1] << " cov<0.9=" << strikeNear[1] << " onStrike=" << strikeOn[1] << "}";
    Log(LogLevel::Info, summary.str());
}

} // namespace sc4af
