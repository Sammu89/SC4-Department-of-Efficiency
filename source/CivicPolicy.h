// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure (SC4-free) civic funding policy: generic demand/capacity funding decision, structural
// classification into writer shapes and policy signals, and the strict config parser.
// Nothing here knows about GZCOM, offsets or pointers.

#include "CivicFundingPlan.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sc4af {

// Structural shape of the local funding control (how the writer must call the setter).
enum class WriterShape {
    EducationWithSecondary,
    EducationStaffOnly,
    HealthWithSecondary,
    HealthStaffOnly,
    NoLocalFunding,
    Unknown
};

// What (if anything) the policy may use as a demand signal.
enum class PolicyKind {
    DemandCapacity,         // people served vs people capacity (schools, colleges, clinics, ...)
    SpecialDemandCapacity,  // same maths, non-person units (books, visitors); needs verification
    NoDemandSignal,
    UnknownSignal
};

inline const char* ShapeName(WriterShape s) {
    switch (s) {
    case WriterShape::EducationWithSecondary: return "EDUCATION_WITH_SECONDARY";
    case WriterShape::EducationStaffOnly: return "EDUCATION_STAFF_ONLY";
    case WriterShape::HealthWithSecondary: return "HEALTH_WITH_SECONDARY";
    case WriterShape::HealthStaffOnly: return "HEALTH_STAFF_ONLY";
    case WriterShape::NoLocalFunding: return "NO_LOCAL_FUNDING";
    case WriterShape::Unknown: return "UNKNOWN";
    }
    return "UNKNOWN";
}

inline const char* PolicyKindName(PolicyKind k) {
    switch (k) {
    case PolicyKind::DemandCapacity: return "DEMAND_CAPACITY";
    case PolicyKind::SpecialDemandCapacity: return "SPECIAL_DEMAND_CAPACITY";
    case PolicyKind::NoDemandSignal: return "NO_DEMAND_SIGNAL";
    case PolicyKind::UnknownSignal: return "UNKNOWN_SIGNAL";
    }
    return "UNKNOWN_SIGNAL";
}

// Shape from structure only: which list returned the facility and which budget lines exist.
inline WriterShape ClassifyShape(FundingFamily family, bool hasStaffLine, bool hasSecondaryLine) {
    if (!hasStaffLine) return hasSecondaryLine ? WriterShape::Unknown : WriterShape::NoLocalFunding;
    if (family == FundingFamily::School)
        return hasSecondaryLine ? WriterShape::EducationWithSecondary : WriterShape::EducationStaffOnly;
    return hasSecondaryLine ? WriterShape::HealthWithSecondary : WriterShape::HealthStaffOnly;
}

// How the capacity of ONE facility relates to the theoretical model base x CapacityFactor(funding).
// A city (or a building) can scale the capacity by a constant; the planner must use the REAL
// capacity, not the theoretical one, without trusting a single unexplained observation:
//   UnitModel   the observed capacity equals the model within the estimate's jitter.
//   Provisional the capacity deviates by a factor m (observation / model at the current funding) that has
//               not been seen at two different fundings yet. Planning is conservative: min(m, 1) (never
//               relies on an unconfirmed bonus), and a facility is never RAISED just because the
//               conservative factor is below what it really has.
//   Confirmed   the same factor was measured at two different fundings (before and after one of our
//               verified writes); planning uses it, so the minimum sufficient funding is the real one.
//   Unsupported the deviation cannot be read as a constant factor (too little resolution, impossible
//               factor): the facility is observed, not written.
enum class CapacityEvidence { UnitModel, Provisional, Confirmed, Unsupported, Incompatible };
inline const char* CapacityEvidenceName(CapacityEvidence e) {
    switch (e) {
    case CapacityEvidence::UnitModel: return "UNIT";
    case CapacityEvidence::Provisional: return "PROVISIONAL";
    case CapacityEvidence::Confirmed: return "CONFIRMED";
    case CapacityEvidence::Unsupported: return "UNSUPPORTED";
    case CapacityEvidence::Incompatible: return "INCOMPATIBLE";
    }
    return "UNKNOWN";
}
//   Incompatible a calibration write moved the funding correctly but the capacity did NOT follow a constant factor (a non-multiplicative curve): no curve is invented, the facility is never
//               REDUCED automatically and is only raised when its observed capacity no longer covers the demand; it is not probed again.
struct CapacityPrior {                     // remembered per facility for the city lifecycle (the session owns it)
    double factor = 1.0;
    bool confirmed = false;
    bool probed = false;                   // one controlled calibration write has already been spent on this facility (never repeated monthly)
    bool incompatible = false;
};
constexpr double kCalibrationMinCapacityStep = 20.0;  // units: the calibration step must move the capacity by at least this much, so the +-1 jitter is a small part of the second measurement
constexpr std::int64_t kMinModelForFactor = 200;      // below this the +-1 jitter is more than 0.5% of the model: a factor cannot be read from it
constexpr double kMinCapacityFactor = 0.5, kMaxCapacityFactor = 2.0;
constexpr double kMinDistinctFundingStep = 0.02;      // two fundings count as different points of the curve when their capacity factors differ by at least this

struct FacilityState {
    FundingFamily family = FundingFamily::School;
    std::string subtype;
    int cellX = 0, cellZ = 0;
    float posX = 0.0f, posZ = 0.0f;      // the occupant's position in world coordinates (the unit the game's camera and advice subjects use)
    bool hasPos = false;
    WriterShape shape = WriterShape::Unknown;
    PolicyKind kind = PolicyKind::UnknownSignal;
    std::uint32_t queryLayout = 0;       // occupant property 0x2A499F85 (0 = unreadable)
    bool vanillaLayout = false;          // layout is one of the compiled vanilla query layouts
    bool layoutMatchesFamily = false;    // and its unit pair belongs to this family's vanilla mechanism
    bool fundingCoherent = false;        // getter / query buffer / budget item agree (AssessFundingCoherence); fail closed
    std::string coherenceReason;         // why they do not (diagnostics only)
    std::optional<std::int64_t> usage;   // nullopt = not sampled (NEVER zero)
    std::int64_t capacity = 0;           // EstimateCurrentOccupantCapacity out1
    std::int64_t baseCapacity = 0;       // query buffer +0x14
    float buildingFunding = 0;
    bool hasSecondary = false;
    float secondaryFunding = 0;          // only meaningful when hasSecondary
    std::int64_t baseCost = 0;
    std::int64_t monthlyCost = 0;
    std::optional<float> strikeCoverage;          // the game's own coverage ratio of THIS building (0..1; < 0.8 rolls for a strike); nullopt = unreadable
    bool onStrike = false;                         // a strike is in progress in this building
    double usageVolatility = 0.0;                  // the largest month-to-month RISE of this building's usage seen in this city lifecycle (0 = no history)
    std::optional<CapacityPrior> capacityPrior;   // what this city lifecycle has learned about THIS facility's capacity factor (nullopt = nothing)
};

// Policy: the margins follow the game's STRIKE rule. The game rolls a strike only below coverage 0.8; its
// "strike rumour" and underfunding advice are silenced unless a strike is in progress
// (DepartmentEfficiencyCivicAdvice.dat), because they fired during small transient budget adjustments.
constexpr double kStrikeTargetCoverage = 0.85;  // coverage kept even after the largest rise seen: 0.05 above the strike knee (0.8) for the jitter and the monthly-stale coverage
constexpr float kStrikeSafeCoverage = 0.85f;  // a building the game shows below this coverage is never lowered (the knee is 0.8)

// Policy (no pre-strike advice): a SMALL facility swings by tens of percent in one month (e.g. usage 34 -> 60,
// +76%, with capacity 36: coverage 0.60, so the game's pre-strike advice fires before the risk watch can raise
// the funding) while its whole funding costs a few dollars. Below these usages the capacity kept is a multiple
// of the usage, at least the learned rise. Larger facilities keep the learned/configured reserve (their money
// matters and their swings are learned).
constexpr int kUnsampledSafePercent = 100;           // a facility with no usage sample yet is raised to this funding (capped by the slider maximum), never lowered
constexpr std::int64_t kSmallFacilityUsage = 100;   // usage below this: factor kSmallFacilityFactor
constexpr double kSmallFacilityFactor = 1.5;        // a +76% month (34 -> 60) keeps coverage 1.5 / 1.76 = 0.85 >= the strike knee
constexpr std::int64_t kMidFacilityUsage = 200;     // usage below this (and not small): factor kMidFacilityFactor
constexpr double kMidFacilityFactor = 1.25;         // a +45% month keeps coverage 1.25 / 1.45 = 0.86
inline double SmallFacilityReserveFactor(std::int64_t usage) {
    return usage < kSmallFacilityUsage ? kSmallFacilityFactor : usage < kMidFacilityUsage ? kMidFacilityFactor : 0.0;
}

struct PolicyConfig {
    double reservePercent = 5.0;
    // The safety margin of each family (education and health have their own); negative = reservePercent.
    // A MINIMUM: the strike guard and the small-building factor below can ask for more (DecideFunding takes the largest).
    double educationReservePercent = -1.0, healthReservePercent = -1.0;
    int minFundingPercent = 1;    // Derived from the executable: the smallest non-zero whole percent (also used for genuine zero usage). 0 would make supply 0 and the coverage ratio 0 as soon as any demand appears (strike roll 100%). The exe has NO fixed-percent strike threshold
    int maxFundingPercent = 120;  // vanilla funding slider range is 0..120 whole percent (query layout minmaxvalue)
    std::int64_t capacityJitter = 1;  // capacity estimate jitters by +-1 between calls
};

enum class Action {
    Write,
    NoChange,
    WaitForSample,
    InsufficientCapacity,   // best available setting chosen; Write or NoChange reported via `write`
    NoPolicy,               // UNSAFE_OR_UNMAPPED: no comparable demand/capacity pair
    NotWritable,            // NOT_LOCALLY_FUNDED: no standard staff line / no meaningful local cost
    NonVanillaMechanism     // custom/unreadable query layout or family mismatch: never automated
};

inline const char* ActionName(Action a) {
    switch (a) {
    case Action::Write: return "WRITE";
    case Action::NoChange: return "NO_CHANGE";
    case Action::WaitForSample: return "WAIT_FOR_SAMPLE";
    case Action::InsufficientCapacity: return "INSUFFICIENT_CAPACITY";
    case Action::NoPolicy: return "NO_POLICY";
    case Action::NotWritable: return "NOT_WRITABLE";
    case Action::NonVanillaMechanism: return "NON_VANILLA_MECHANISM";
    }
    return "UNKNOWN";
}

struct Decision {
    Action action = Action::NoPolicy;
    bool write = false;            // true when a setter call is required
    int targetPercent = 0;
    float targetFraction = 0;
    std::int64_t required = 0;
    std::int64_t predictedCapacity = 0;
    std::int64_t currentCost = 0;
    std::int64_t targetCost = 0;
    std::int64_t saving = 0;       // currentCost - targetCost (negative when spending more)
    bool calibration = false;      // a controlled calibration write (a small RAISE to obtain the second point of the capacity curve): not a normal write, not a saving
    CapacityEvidence capacityEvidence = CapacityEvidence::UnitModel;
    double capacityFactor = 1.0;   // the factor observed / learned for this facility (1 for UnitModel)
    double planFactor = 1.0;       // the factor the plan relied on (Provisional: min(factor, 1))
    std::string reason;
};

// The vanilla slider commits `int percent * 0.01f`; reproduce the exact float it produces.
inline float FractionForPercent(int percent) {
    return static_cast<float>(static_cast<double>(percent) * static_cast<double>(0.01f));
}

// Vanilla populates the slider with round-to-nearest-even(fraction * 100).
inline int PercentForFraction(float fraction) {
    return static_cast<int>(std::nearbyint(static_cast<double>(fraction) * 100.0));
}

inline std::int64_t ModelCapacity(std::int64_t baseCapacity, float fraction) {
    return EstimateCapacityFromBase(baseCapacity, fraction);
}

// Was the service left below the requirement by a verified normal write? Only a write whose decision was SUFFICIENT can breach it: when no legal setting covers usage x reserve
// (Action::InsufficientCapacity) the policy raises the facility to the highest legal setting, the shortfall is the physical limit of the building (reported as `insufficient`, an alert for
// the player), not a failure of the plugin; the write itself is still verified against the capacity model.
inline bool ServiceBreached(const Decision& d, std::int64_t capacityAfter, std::int64_t jitter) {
    if (d.action == Action::InsufficientCapacity) return false;
    return capacityAfter + jitter < d.required;
}

struct CapacityCalibration {
    CapacityEvidence evidence = CapacityEvidence::UnitModel;
    double factor = 1.0;
    std::string note;
};

inline CapacityCalibration CalibrateCapacity(const FacilityState& f, const PolicyConfig& config) {
    CapacityCalibration c;
    const double model0 = static_cast<double>(f.baseCapacity) * CapacityFactor(f.buildingFunding);
    if (f.baseCapacity <= 0 || !std::isfinite(model0)) { c.evidence = CapacityEvidence::Unsupported; c.note = "base capacity unknown"; return c; }
    if (std::llabs(f.capacity - static_cast<std::int64_t>(model0)) <= config.capacityJitter) return c;     // UnitModel (also clears any earlier doubt: the observation is back on the model)
    if (f.capacity <= 0) { c.evidence = CapacityEvidence::Unsupported; c.note = "capacity unreadable or zero"; return c; }
    const double m = static_cast<double>(f.capacity) / model0;
    if (model0 < static_cast<double>(kMinModelForFactor) || !(m >= kMinCapacityFactor && m <= kMaxCapacityFactor)) {
        c.evidence = CapacityEvidence::Unsupported;
        c.note = "capacity " + std::to_string(f.capacity) + " vs model " + std::to_string(static_cast<std::int64_t>(model0)) + " cannot be read as a constant factor";
        return c;
    }
    c.factor = m;
    if (f.capacityPrior && f.capacityPrior->incompatible) { c.evidence = CapacityEvidence::Incompatible; return c; }
    const bool compatible = f.capacityPrior && f.capacityPrior->confirmed && std::fabs(static_cast<double>(f.capacity) - f.capacityPrior->factor * model0) <= static_cast<double>(config.capacityJitter) + 1.0;
    if (compatible) { c.evidence = CapacityEvidence::Confirmed; c.factor = f.capacityPrior->factor; }
    else c.evidence = CapacityEvidence::Provisional;     // first sight, a changed factor, or a confirmed factor that the new observation contradicts: re-evaluated from scratch
    return c;
}

// What one verified write teaches about the factor: the capacity measured AFTER it, at a different funding, agreeing with the factor read BEFORE it. nullopt when there is nothing to remember.
inline std::optional<CapacityPrior> LearnFromWrite(const FacilityState& before, float fundingAfter, std::int64_t capacityAfter, const CapacityCalibration& used, const PolicyConfig& config) {
    if (used.evidence == CapacityEvidence::UnitModel || used.evidence == CapacityEvidence::Unsupported) return std::nullopt;
    const double modelAfter = static_cast<double>(before.baseCapacity) * CapacityFactor(fundingAfter);
    if (modelAfter < static_cast<double>(kMinModelForFactor) || capacityAfter <= 0) return std::nullopt;
    const double m1 = static_cast<double>(capacityAfter) / modelAfter;
    const bool distinct = std::fabs(CapacityFactor(fundingAfter) - CapacityFactor(before.buildingFunding)) >= kMinDistinctFundingStep;
    const double ratio = CapacityFactor(fundingAfter) / std::max(1e-9, CapacityFactor(before.buildingFunding));
    const bool agrees = std::fabs(static_cast<double>(capacityAfter) - used.factor * modelAfter) <= static_cast<double>(config.capacityJitter) + ratio + 1.0;
    CapacityPrior p;
    p.factor = m1;
    p.confirmed = distinct && agrees && m1 >= kMinCapacityFactor && m1 <= kMaxCapacityFactor;
    p.probed = before.capacityPrior && before.capacityPrior->probed;
    return p;
}

// What a CALIBRATION write turned out to be. The calibration raise is only ever made while the demand is covered, so after it the capacity must still cover usage x reserve; that is checked
// explicitly, for every outcome, so no calibration write escapes the service control:
//   Verified                      every channel verified AND the capacity covers the requirement.
//   IncompatibleServicePreserved  every funding channel, the secondary line and the cost verified; only the capacity did not follow a constant factor (the capacity check is the LAST one of
//                                 the verification) AND it still covers the requirement: record it, never calibrate this facility again, carry on.
//   ServiceViolation              the capacity does not cover the requirement (whatever the verification said): not safe, the existing protection applies.
//   Unverified                    anything else (another channel failed, an unreadable / non-finite / implausible observation): the generic unverified-write protection applies.
enum class CalibrationOutcome { Verified, IncompatibleServicePreserved, ServiceViolation, Unverified };
inline CalibrationOutcome ClassifyCalibration(bool verified, const std::string& failReason, const Readback& after, std::int64_t required, std::int64_t jitter) {
    constexpr const char* kCapacityOnly = "capacity outside the +-1 model";
    if (!ReadbackFinite(after) || after.capacity <= 0 || after.baseCapacity <= 0) return CalibrationOutcome::Unverified;       // a doubtful reading is never "service preserved"
    const bool covers = after.capacity + jitter >= required;
    if (verified) return covers ? CalibrationOutcome::Verified : CalibrationOutcome::ServiceViolation;
    if (failReason != kCapacityOnly) return CalibrationOutcome::Unverified;
    return covers ? CalibrationOutcome::IncompatibleServicePreserved : CalibrationOutcome::ServiceViolation;
}

inline double ReservePercentFor(const PolicyConfig& c, FundingFamily family) {
    const double own = family == FundingFamily::Health ? c.healthReservePercent : c.educationReservePercent;
    return own >= 0.0 ? own : c.reservePercent;
}

inline Decision DecideFunding(const FacilityState& f, const PolicyConfig& config) {
    Decision d;
    d.currentCost = f.monthlyCost;
    // Structural eligibility (no names, no origin): a facility is only managed when it is a locally
    // funded civic expense on a standard mechanism. A missing staff line or a zero base cost means
    // there is no player-funded local control to optimise (e.g. a vanilla Private School).
    if (f.shape == WriterShape::NoLocalFunding || f.shape == WriterShape::Unknown || f.baseCost <= 0) {
        d.action = Action::NotWritable;
        d.reason = f.baseCost <= 0 && f.shape != WriterShape::NoLocalFunding && f.shape != WriterShape::Unknown
                       ? "local funding line has no cost" : std::string("shape ") + ShapeName(f.shape);
        return d;
    }
    // Only validated vanilla mechanisms. A custom query layout, or a layout of the
    // wrong family, is a different mechanism and is never automated.
    if (!f.vanillaLayout || !f.layoutMatchesFamily) {
        d.action = Action::NonVanillaMechanism;
        d.reason = f.queryLayout == 0 ? "query layout unreadable" : "query layout is not a vanilla mechanism of this family";
        return d;
    }
    // Writer/readback coherence: a staff-looking budget line on a vanilla layout is still a different
    // mechanism when the channels disagree at rest, because the vanilla setter cannot converge on it.
    if (!f.fundingCoherent) {
        d.action = Action::NonVanillaMechanism;
        d.reason = "funding channels incoherent (" + f.coherenceReason + "): the vanilla writer does not control this line";
        return d;
    }
    if (f.kind != PolicyKind::DemandCapacity && f.kind != PolicyKind::SpecialDemandCapacity) {
        d.action = Action::NoPolicy;
        d.reason = std::string("signal ") + PolicyKindName(f.kind);
        return d;
    }
    // A strike in progress: the service comes first, and it is decided BEFORE the sample and capacity-model checks. A strike cuts the staff term of the capacity, so the
    // capacity measured during it may fall outside any constant-factor reading of the model (CapacityEvidence::Unsupported) and usage may be unsampled: neither may keep the funding from going to the
    // maximum the slider allows (the strike ends at coverage 0.9), whatever it costs.
    if (f.onStrike) {
        const CapacityCalibration scal = CalibrateCapacity(f, config);
        const double sfactor = scal.evidence == CapacityEvidence::Unsupported ? 1.0 : std::min(scal.factor, 1.0);
        const int top = config.maxFundingPercent;
        d.targetPercent = top;
        d.targetFraction = FractionForPercent(top);
        d.capacityEvidence = scal.evidence;
        d.capacityFactor = scal.evidence == CapacityEvidence::Unsupported ? 1.0 : scal.factor;
        d.planFactor = sfactor;
        d.predictedCapacity = f.baseCapacity > 0 ? static_cast<std::int64_t>(sfactor * static_cast<double>(f.baseCapacity) * CapacityFactor(d.targetFraction)) : 0;
        if (f.usage) d.required = static_cast<std::int64_t>(std::ceil(static_cast<double>(*f.usage) * (1.0 + ReservePercentFor(config, f.family) / 100.0) - 1e-9));
        d.targetCost = RoundedFundingCost(f.baseCost, d.targetFraction);
        d.saving = d.currentCost - d.targetCost;
        const bool already = std::fabs(f.buildingFunding - d.targetFraction) <= 0.005f + 1e-6f;
        d.write = !already;
        d.action = already ? Action::InsufficientCapacity : Action::Write;
        d.reason = "a strike is in progress: funding to the maximum to recover the coverage (a strike ends at coverage 0.9)";
        return d;
    }
    if (!f.usage) {
        // A facility with no usage sample yet is typically one that has just been built: the game opens it with a low funding (e.g. 6%, capacity 71 for a demand of 99: coverage 0.717, which
        // triggers the pre-strike advice) and the first sample takes days. Unsampled is never zero demand, so there is nothing to optimise yet; but the service comes first: it is RAISED to a safe level
        // (never lowered; the reduction to the real need waits for the sample and the monthly pass, like any other saving).
        const int safe = std::min(config.maxFundingPercent, kUnsampledSafePercent);
        if (PercentForFraction(f.buildingFunding) < safe) {
            d.targetPercent = safe;
            d.targetFraction = FractionForPercent(safe);
            d.targetCost = RoundedFundingCost(f.baseCost, d.targetFraction);
            d.saving = d.currentCost - d.targetCost;
            d.write = true;
            d.action = Action::Write;
            d.reason = "usage not sampled yet (never treated as zero demand): a facility without a sample is raised to a safe funding until it has one (no pre-strike advice while it opens)";
            return d;
        }
        d.action = Action::WaitForSample;
        d.reason = "usage not sampled yet (never treated as zero demand)";
        return d;
    }
    if (f.baseCapacity <= 0) {
        d.action = Action::NoPolicy;
        d.reason = "base capacity unknown";
        return d;
    }
    // The reserve is the larger of the configured percentage and what keeps the game's coverage ratio (capacity / usage) at the target after the largest rise this building has shown.
    const double reserveFactor = std::max({1.0 + ReservePercentFor(config, f.family) / 100.0, kStrikeTargetCoverage * (1.0 + f.usageVolatility), SmallFacilityReserveFactor(*f.usage)});
    const double reserved = static_cast<double>(*f.usage) * reserveFactor;
    d.required = static_cast<std::int64_t>(std::ceil(reserved - 1e-9));

    // The REAL capacity curve of this facility (not the theoretical one): see CapacityEvidence.
    const CapacityCalibration cal = CalibrateCapacity(f, config);
    d.capacityEvidence = cal.evidence;
    d.capacityFactor = cal.factor;
    if (cal.evidence == CapacityEvidence::Unsupported) {
        d.action = Action::NoPolicy;
        d.reason = "capacity model not supported (" + cal.note + "): observed only, no write";
        return d;
    }
    const bool unconfirmed = cal.evidence == CapacityEvidence::Provisional || cal.evidence == CapacityEvidence::Incompatible;
    const double planFactor = unconfirmed ? std::min(cal.factor, 1.0) : cal.factor;
    d.planFactor = planFactor;
    const double factorNow = std::max(1e-9, CapacityFactor(f.buildingFunding));
    // Capacity the plan may rely on at a setting: the factor-scaled model minus the estimate's jitter and, when a factor is in play, the uncertainty of that factor at the new setting.
    const auto reliableCapacity = [&](int p) {
        const float frac = FractionForPercent(p);
        if (cal.evidence == CapacityEvidence::UnitModel) return ModelCapacity(f.baseCapacity, frac) - config.capacityJitter;
        const double predicted = planFactor * static_cast<double>(f.baseCapacity) * CapacityFactor(frac);
        const std::int64_t uncertainty = static_cast<std::int64_t>(std::ceil(CapacityFactor(frac) / factorNow));
        return static_cast<std::int64_t>(predicted) - config.capacityJitter - uncertainty;
    };

    // Smallest legal whole-percent setting whose reliable capacity still covers the requirement.
    int chosen = -1;
    for (int p = config.minFundingPercent; p <= config.maxFundingPercent; ++p) {
        if (reliableCapacity(p) >= d.required) {
            chosen = p;
            break;
        }
    }
    bool insufficient = false;
    if (chosen < 0) {
        chosen = config.maxFundingPercent;
        insufficient = true;
    }
    // A factor that has not been confirmed never justifies RAISING the funding: if the capacity really observed already covers the requirement, keep the current setting
    // (a reduction below it is only planned with the conservative factor, and the write itself then confirms the factor).
    const bool wouldRaise = FractionForPercent(chosen) > f.buildingFunding + 0.005f;
    const bool wouldCut = FractionForPercent(chosen) < f.buildingFunding - 0.005f;
    const bool covered = f.capacity - config.capacityJitter >= d.required;
    // STRIKE MARGIN: the game rolls for a strike on the building's own coverage ratio (capacity against the demand of its catchment, which can exceed the usage this building serves), with chance 0 only
    // from 0.8. A building the game already shows below the safe margin (or on strike) never has its funding LOWERED: the usage-based requirement cannot see that demand.
    if (wouldCut && !insufficient && (f.onStrike || (f.strikeCoverage && *f.strikeCoverage < kStrikeSafeCoverage))) {
        d.targetPercent = PercentForFraction(f.buildingFunding);
        d.targetFraction = f.buildingFunding;
        d.predictedCapacity = f.capacity;
        d.targetCost = d.currentCost;
        d.saving = 0;
        d.write = false;
        d.action = Action::NoChange;
        d.reason = f.onStrike ? std::string("a strike is in progress in this building: the funding is not lowered")
                              : "the game's coverage ratio of this building is " + std::to_string(*f.strikeCoverage) + " (< " + std::to_string(kStrikeSafeCoverage) + ", strike margin): the funding is not lowered";
        return d;
    }
    // A small controlled RAISE gives the second point of the curve when nothing else will: only a factor above 1 (a possible saving) that no write has tested yet, and only when the demand is covered
    // anyway (service is never reduced, School Bus / Ambulance are not part of it). The step moves the capacity by at least kCalibrationMinCapacityStep; no room above = no calibration.
    const bool probeWanted = cal.evidence == CapacityEvidence::Provisional && cal.factor > 1.0 + 1e-9 && !(f.capacityPrior && f.capacityPrior->probed) && !insufficient && !wouldCut && covered;
    if (probeWanted) {
        const double minStep = std::max(static_cast<double>(kMinDistinctFundingStep), kCalibrationMinCapacityStep / (cal.factor * static_cast<double>(f.baseCapacity)));
        for (int p = PercentForFraction(f.buildingFunding) + 1; p <= config.maxFundingPercent; ++p) {
            if (CapacityFactor(FractionForPercent(p)) - CapacityFactor(f.buildingFunding) >= minStep) {
                d.calibration = true;
                d.targetPercent = p;
                d.targetFraction = FractionForPercent(p);
                d.predictedCapacity = static_cast<std::int64_t>(cal.factor * static_cast<double>(f.baseCapacity) * CapacityFactor(d.targetFraction));
                d.targetCost = RoundedFundingCost(f.baseCost, d.targetFraction);
                d.saving = d.currentCost - d.targetCost;
                d.write = true;
                d.action = Action::Write;
                d.reason = "calibration: one small controlled raise to measure the capacity at a second funding (factor " + std::to_string(cal.factor) + " seen once); the demand is covered, the next evaluation plans the minimum";
                return d;
            }
        }
    }
    if (unconfirmed && !insufficient && ((wouldRaise && covered) || (cal.evidence == CapacityEvidence::Incompatible && wouldCut))) {
        d.targetPercent = PercentForFraction(f.buildingFunding);
        d.targetFraction = f.buildingFunding;
        d.predictedCapacity = f.capacity;
        d.targetCost = d.currentCost;
        d.saving = 0;
        d.write = false;
        d.action = Action::NoChange;
        d.reason = cal.evidence == CapacityEvidence::Incompatible
                       ? std::string("the capacity did not follow a constant factor when the funding was moved: no automatic reduction until there is evidence")
                       : "capacity factor " + std::to_string(cal.factor) + " not yet confirmed at two fundings: no reduction is predictable with certainty and the observed capacity already covers usage*reserve";
        return d;
    }
    d.targetPercent = chosen;
    d.targetFraction = FractionForPercent(chosen);
    d.predictedCapacity = cal.evidence == CapacityEvidence::UnitModel ? ModelCapacity(f.baseCapacity, d.targetFraction)
                                                                      : static_cast<std::int64_t>(planFactor * static_cast<double>(f.baseCapacity) * CapacityFactor(d.targetFraction));
    d.targetCost = RoundedFundingCost(f.baseCost, d.targetFraction);
    d.saving = d.currentCost - d.targetCost;

    // Same grid cell => already the answer: never rewrite an effectively equal float.
    const bool sameCell = std::fabs(f.buildingFunding - d.targetFraction) <= 0.005f + 1e-6f;
    d.write = !sameCell;
    if (insufficient) {
        d.action = Action::InsufficientCapacity;
        d.reason = "no legal setting covers usage*reserve; highest legal capacity chosen";
    } else if (sameCell) {
        d.action = Action::NoChange;
        d.reason = "current funding is already the minimum sufficient setting";
    } else {
        d.action = Action::Write;
        d.reason = "minimum legal setting that covers usage*reserve";
    }
    return d;
}

// --- Civic funding configuration (separate from the writer-test INI) -------------------

enum class AutoMode { Off, Shadow, Auto };

inline const char* AutoModeName(AutoMode m) {
    return m == AutoMode::Off ? "OFF" : m == AutoMode::Shadow ? "SHADOW" : "AUTO";
}

struct AutoFinanceConfig {
    AutoMode mode = AutoMode::Off;
    double reservePercent = 5.0;
    double educationReservePercent = -1.0;   // EducationReservePercent / HealthReservePercent: the margin of each family; absent (negative) = ReservePercent
    double healthReservePercent = -1.0;
    double EducationReserve() const { return educationReservePercent >= 0.0 ? educationReservePercent : reservePercent; }
    double HealthReserve() const { return healthReservePercent >= 0.0 ? healthReservePercent : reservePercent; }
    bool education = true;
    bool health = true;
    int minFundingPercent = 1;    // see PolicyConfig
    int maxFundingPercent = 120;
};

// Strict parser: any unknown key, duplicate, malformed value or out-of-range number yields Off.
inline bool ParseAutoFinanceConfig(const std::string& text, AutoFinanceConfig& config, std::string& error) {
    AutoFinanceConfig parsed;
    unsigned seen = 0;
    std::size_t pos = 0;
    while (pos <= text.size()) {
        auto eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size();
        const std::string line = Trim(text.substr(pos, eol - pos));
        pos = eol + 1;
        if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') continue;
        const auto eq = line.find('=');
        if (eq == std::string::npos) { error = "malformed line: " + line; config = AutoFinanceConfig{}; return false; }
        const std::string key = Trim(line.substr(0, eq)), value = Trim(line.substr(eq + 1));
        unsigned bit = 0;
        bool ok = true;
        const auto boolValue = [&](bool& out) {
            if (value == "true") out = true; else if (value == "false") out = false; else ok = false;
        };
        if (key == "Mode") {
            bit = 1;
            if (value == "Off") parsed.mode = AutoMode::Off;
            else if (value == "Shadow") parsed.mode = AutoMode::Shadow;
            else if (value == "Auto") parsed.mode = AutoMode::Auto;
            else ok = false;
        } else if (key == "ReservePercent") {
            bit = 2;
            float v = 0;
            ok = ParseFloatStrict(value, v) && v >= 0.0f && v <= 50.0f;
            parsed.reservePercent = v;
        } else if (key == "EducationReservePercent" || key == "HealthReservePercent") {
            bit = key[0] == 'E' ? 64 : 128;
            float v = 0;
            ok = ParseFloatStrict(value, v) && v >= 0.0f && v <= 50.0f;
            (key[0] == 'E' ? parsed.educationReservePercent : parsed.healthReservePercent) = v;
        } else if (key == "Education") { bit = 4; boolValue(parsed.education); }
        else if (key == "Health") { bit = 8; boolValue(parsed.health); }
        else if (key == "MinFundingPercent") {
            bit = 16; ok = ParseIntStrict(value, parsed.minFundingPercent) && parsed.minFundingPercent >= 0 &&
                           parsed.minFundingPercent <= 100;
        } else if (key == "MaxFundingPercent") {
            bit = 32; ok = ParseIntStrict(value, parsed.maxFundingPercent) && parsed.maxFundingPercent >= 1 &&
                           parsed.maxFundingPercent <= 120;
        } else { error = "unknown key: " + key; config = AutoFinanceConfig{}; return false; }
        if (!ok) { error = "bad value for " + key; config = AutoFinanceConfig{}; return false; }
        if (seen & bit) { error = "duplicate key: " + key; config = AutoFinanceConfig{}; return false; }
        seen |= bit;
    }
    if (parsed.minFundingPercent > parsed.maxFundingPercent) {
        error = "MinFundingPercent > MaxFundingPercent"; config = AutoFinanceConfig{}; return false;
    }
    // Every key is optional; defaults are safe (Mode=Off).
    config = parsed;
    error.clear();
    return true;
}

} // namespace sc4af
