// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "CivicFundingWriter.h"

#include "CivicFacilityIO.h"
#include "CivicFundingPlan.h"
#include "Logging.h"

#include "cISC4City.h"

#include <windows.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>

namespace sc4af {

struct CivicFundingWriter::Impl {
    WriterConfig config;
    bool configValid = false;
    std::string configError;
    std::set<std::string> doneNonces;
    bool haveBaseline = false;
    Readback baseline;
};

CivicFundingWriter::Impl& CivicFundingWriter::State() {
    static Impl impl;
    return impl;
}

namespace {

// The writer test may address any class the policy layer knows, including "special" ones;
// it never depends on whether the automatic policy is enabled for them.
constexpr bool kSpecialSignalsForCandidateReading = true;

std::string Fmt(float v) {
    std::ostringstream s;
    s << std::setprecision(9) << v;
    return s.str();
}

TargetCandidate ToCandidate(const LiveFacility& l) {
    TargetCandidate c;
    c.family = l.state.family;
    c.subtype = l.state.subtype;
    c.cellX = l.state.cellX;
    c.cellZ = l.state.cellZ;
    c.state.building = l.state.buildingFunding;
    c.state.secondary = l.readback.getterSecondary;
    c.state.hasSecondary = l.state.hasSecondary;
    c.baseCost = l.readback.baseCost;
    c.baseCapacity = l.readback.baseCapacity;
    c.usage = l.state.usage;
    return c;
}

// Only facilities that actually own a local funding line can be write targets.
bool IsWritable(const LiveFacility& l) {
    return l.state.shape != WriterShape::NoLocalFunding && l.state.shape != WriterShape::Unknown;
}

std::vector<LiveFacility> ReadFamily(cISC4City* city, FundingFamily family) {
    std::vector<LiveFacility> out;
    for (auto& l : ReadFacilities(city, kSpecialSignalsForCandidateReading))
        if (l.state.family == family && IsWritable(l)) out.push_back(l);
    return out;
}

bool ReadFileText(const std::string& path, std::string& text) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return false;
    std::ostringstream buffer;
    buffer << in.rdbuf();
    text = buffer.str();
    return true;
}

std::uint64_t FileWriteTime(const std::string& path) {
    WIN32_FILE_ATTRIBUTE_DATA data{};
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) return 0;
    return (static_cast<std::uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime;
}

void LogResult(const char* phase, WriteResult result, const std::string& detail) {
    Log(LogLevel::Info, std::string("[WriterTest][") + phase + "] result=" + ResultName(result) + " " + detail);
}

} // namespace

bool CivicFundingWriter::LoadConfigIfChanged() {
    auto& st = State();
    if (configPath_.empty()) return false;
    const auto stamp = FileWriteTime(configPath_);
    if (stamp == lastWriteTime_) return st.configValid;
    lastWriteTime_ = stamp;
    st.configValid = false;
    st.config = WriterConfig{};
    if (stamp == 0) return false; // no file => no write
    std::string text;
    if (!ReadFileText(configPath_, text)) {
        Log(LogLevel::Warning, "[WriterTest] config unreadable; NO WRITE");
        return false;
    }
    std::string error;
    if (!ParseWriterConfig(text, st.config, error)) {
        st.config = WriterConfig{};
        st.configError = error;
        Log(LogLevel::Warning, "[WriterTest] config invalid (" + error + "); NO WRITE");
        return false;
    }
    st.configValid = true;
    Log(LogLevel::Info, std::string("[WriterTest] config loaded enabled=") + (st.config.enabled ? "true" : "false") +
        " mode=" + (st.config.mode == WriteMode::Apply ? "Apply" : st.config.mode == WriteMode::DryRun ? "DryRun" : "Off") +
        " nonce=" + st.config.nonce);
    return true;
}

void CivicFundingWriter::ResetCity() { State().haveBaseline = false; }

void CivicFundingWriter::Poll(cISC4City* city, std::uint64_t msSinceCityInit) {
    if (!city || !LoadConfigIfChanged()) return;
    auto& st = State();
    const auto& cfg = st.config;
    if (!cfg.enabled || cfg.mode == WriteMode::Off) return;
    if (msSinceCityInit < 3000) return;                 // never act while the city is still settling
    if (st.doneNonces.count(cfg.nonce)) return;          // one-shot per nonce
    st.doneNonces.insert(cfg.nonce);                     // recorded BEFORE acting: no retry storm

    auto live = ReadFamily(city, cfg.family);
    std::vector<TargetCandidate> candidates;
    for (const auto& l : live) candidates.push_back(ToCandidate(l));
    auto plan = PlanBuildingFundingWrite(cfg, candidates);

    std::ostringstream head;
    head << "nonce=" << cfg.nonce << " selector=" << (plan.selector ? "true" : "false") << " family="
         << (cfg.family == FundingFamily::School ? "School" : "Health") << " class=" << cfg.subtype << " cell="
         << cfg.cellX << ',' << cfg.cellZ << " expected=" << Fmt(cfg.expectedBuilding) << '/'
         << (cfg.expectedHasSecondary ? Fmt(cfg.expectedSecondary) : std::string("none")) << " target="
         << Fmt(cfg.targetBuilding) << " candidates=" << candidates.size();
    Log(LogLevel::Info, "[WriterTest][ARMED] mode=" +
        std::string(cfg.mode == WriteMode::Apply ? "Apply" : "DryRun") + " " + head.str());

    if (plan.result != WriteResult::ReadyToApply && plan.result != WriteResult::DryRunOk) {
        LogResult("PLAN", plan.result, plan.reason);
        if (plan.result == WriteResult::TargetNotFound || plan.result == WriteResult::TargetAmbiguous ||
            plan.result == WriteResult::PreconditionMismatch) {
            for (const auto& l : live) Log(LogLevel::Info, "[WriterTest][CANDIDATE] " + DescribeFacility(l));
        }
        if (plan.result == WriteResult::AlreadyApplied && plan.targetIndex < live.size()) {
            Log(LogLevel::Info, "[WriterTest][STATE] " + DescribeFacility(live[plan.targetIndex]));
            st.baseline = live[plan.targetIndex].readback;
            st.haveBaseline = true;
        }
        return;
    }

    const LiveFacility& target = live[plan.targetIndex];
    Log(LogLevel::Info, "[WriterTest][BEFORE] " + DescribeFacility(target));
    Log(LogLevel::Info, "[WriterTest][PROPOSE] buildingFunding=" + Fmt(plan.buildingToWrite) + " secondaryPassedBack=" +
        (plan.secondaryPresent ? Fmt(plan.secondaryToWrite) : std::string("none(fallback ") + Fmt(plan.secondaryToWrite) + ")") +
        " selector=" + (plan.selector ? "true" : "false") + " expectedCost=" +
        std::to_string(RoundedFundingCost(target.readback.baseCost, plan.buildingToWrite)) + " expectedCapacity~=" +
        std::to_string(EstimateCapacityFromBase(target.readback.baseCapacity, plan.buildingToWrite)));
    if (plan.result == WriteResult::DryRunOk) {
        LogResult("DRYRUN", WriteResult::DryRunOk, "setter NOT called");
        return;
    }

    // ---- WRITE: the single vanilla call (static trace: slider commit -> slot +0x4C). ----
    Log(LogLevel::Info, "[WriterTest][APPLY] SetOccupantFundingPercentages(building=" + Fmt(plan.buildingToWrite) +
        ", secondary=" + Fmt(plan.secondaryToWrite) + ", selector=" + (plan.selector ? "true" : "false") + ")");
    if (!WriteBuildingFunding(city, target, plan.buildingToWrite))
        Log(LogLevel::Warning, "[WriterTest][APPLY] setter returned false; judging by read-back only");

    // ---- VERIFY: re-read from the authoritative lists and re-locate by the same selector. ----
    auto after = ReadFamily(city, cfg.family);
    std::vector<TargetCandidate> afterCandidates;
    for (const auto& l : after) afterCandidates.push_back(ToCandidate(l));
    auto verifyCfg = cfg;
    verifyCfg.expectedBuilding = plan.buildingToWrite;
    verifyCfg.expectedSecondary = plan.secondaryToWrite;
    verifyCfg.expectedHasSecondary = plan.secondaryPresent;
    auto relocate = PlanBuildingFundingWrite(verifyCfg, afterCandidates);
    if (relocate.result != WriteResult::AlreadyApplied) {
        LogResult("VERIFY", WriteResult::VerificationFailed,
                  std::string("FAIL reason=post-write relocation/state: ") + ResultName(relocate.result) + " " + relocate.reason);
        return;
    }
    const LiveFacility& now = after[relocate.targetIndex];
    Log(LogLevel::Info, "[WriterTest][AFTER] " + DescribeFacility(now));
    const auto verdict = VerifyReadback(now.readback, plan.buildingToWrite, plan.secondaryToWrite, plan.secondaryPresent);
    if (verdict.pass) {
        LogResult("VERIFY", WriteResult::Applied, "PASS " + verdict.reason);
        st.baseline = now.readback;
        st.haveBaseline = true;
    } else {
        LogResult("VERIFY", WriteResult::VerificationFailed, "FAIL reason=" + verdict.reason);
    }
}

void CivicFundingWriter::ObserveMonth(cISC4City* city, std::uint32_t month) {
    auto& st = State();
    if (!city || !LoadConfigIfChanged() || !st.config.enabled) return;
    const auto& cfg = st.config;
    auto live = ReadFamily(city, cfg.family);
    const LiveFacility* found = nullptr;
    int matches = 0;
    for (const auto& l : live) {
        if (l.state.subtype == cfg.subtype && l.state.cellX == cfg.cellX && l.state.cellZ == cfg.cellZ) {
            found = &l;
            ++matches;
        }
    }
    if (matches != 1) {
        Log(LogLevel::Warning, "[WriterTest][OBSERVE] month=" + std::to_string(month) + " target matches=" +
            std::to_string(matches));
        return;
    }
    const auto& r = found->readback;
    std::string drift;
    if (st.haveBaseline) {
        if (!NearlyEqual(r.getterBuilding, st.baseline.getterBuilding)) drift += " building-funding";
        if (r.hasSecondary && !NearlyEqual(r.getterSecondary, st.baseline.getterSecondary, kSecondaryExactTolerance))
            drift += " secondary-funding";
        if (r.monthlyCost != st.baseline.monthlyCost) drift += " cost";
        if (std::llabs(r.capacity - st.baseline.capacity) > 1) drift += " capacity";
    } else {
        st.baseline = r;
        st.haveBaseline = true;
    }
    Log(LogLevel::Info, "[WriterTest][OBSERVE] month=" + std::to_string(month) + " " + DescribeFacility(*found) +
        (drift.empty() ? " drift=none" : " DRIFT:" + drift));
}

} // namespace sc4af
