// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure (SC4-free) planning, config parsing and verification rules for the civic
// funding writer harness. Everything here is deterministic.
// The runtime layer (CivicFundingWriter.cpp) only gathers state, executes the plan
// through the vanilla setter and feeds the read-back into VerifyReadback().

#include "CivicClassifier.h"

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>
#include <vector>

namespace sc4af {

enum class FundingFamily { School, Health };

// The last bool of Get/SetOccupantFundingPercentages selects the purpose pair:
// true = school staff/bus, false = hospital staff/ambulance.
constexpr bool FundingSelector(FundingFamily family) { return family == FundingFamily::School; }

enum class WriteMode { Off, DryRun, Apply };

enum class WriteResult {
    NotArmed,
    Applied,
    AlreadyApplied,
    TargetNotFound,
    TargetAmbiguous,
    PreconditionMismatch,
    UnsupportedClass,
    TargetOutOfRange,
    UnsafeLowering,
    SetterFailed,
    VerificationFailed,
    ReadyToApply, // planner-only: precondition satisfied, setter not called yet
    DryRunOk      // planner-only: would apply, dry-run mode
};

inline const char* ResultName(WriteResult r) {
    switch (r) {
    case WriteResult::NotArmed: return "NotArmed";
    case WriteResult::Applied: return "Applied";
    case WriteResult::AlreadyApplied: return "AlreadyApplied";
    case WriteResult::TargetNotFound: return "TargetNotFound";
    case WriteResult::TargetAmbiguous: return "TargetAmbiguous";
    case WriteResult::PreconditionMismatch: return "PreconditionMismatch";
    case WriteResult::UnsupportedClass: return "UnsupportedClass";
    case WriteResult::TargetOutOfRange: return "TargetOutOfRange";
    case WriteResult::UnsafeLowering: return "UnsafeLowering";
    case WriteResult::SetterFailed: return "SetterFailed";
    case WriteResult::VerificationFailed: return "VerificationFailed";
    case WriteResult::ReadyToApply: return "ReadyToApply";
    case WriteResult::DryRunOk: return "DryRunOk";
    }
    return "Unknown";
}

struct WriterConfig {
    bool enabled = false;
    WriteMode mode = WriteMode::Off;
    std::string nonce;                 // one-shot token: a nonce is acted on at most once per process
    FundingFamily family = FundingFamily::School;
    std::string subtype;               // e.g. "elementary", "clinic_or_medical"
    int cellX = 0, cellZ = 0;          // floor(position / 16) in the loaded city
    float expectedBuilding = 0, expectedSecondary = 0;
    bool expectedHasSecondary = true;  // false for staff-only buildings (no bus/ambulance line)
    float targetBuilding = 0;
};

// Eligibility is structural (list membership + budget items), not name based; only a target
// without a class label is refused here. Facilities without a local funding line never
// become candidates.
inline bool IsSupportedWriterClass(FundingFamily, const std::string& subtype) { return !subtype.empty(); }

constexpr float kFundingEpsilon = 1e-4f;      // comparison of a stored fraction with an expected one
constexpr float kSecondaryExactTolerance = 1e-6f;
constexpr float kMaxTestDelta = 0.05f;        // the harness permits only tiny changes
constexpr float kMinFunding = 0.05f, kMaxFunding = 1.2f;
constexpr double kCapacitySafetyFactor = 1.25; // never lower below usage * 1.25

struct FundingState {
    float building = 0;
    float secondary = 0;
    bool hasSecondary = true;  // false: the facility has no secondary budget line at all
};

struct TargetCandidate {
    FundingFamily family = FundingFamily::School;
    std::string subtype;
    int cellX = 0, cellZ = 0;
    FundingState state;
    std::int64_t baseCost = 0;      // budget item cost of the building line (runtime authority)
    std::int64_t baseCapacity = 0;  // query buffer +0x14
    std::optional<std::int64_t> usage;
};

struct WritePlan {
    WriteResult result = WriteResult::NotArmed;
    std::size_t targetIndex = 0;
    bool selector = true;
    float buildingToWrite = 0;
    float secondaryToWrite = 0;  // exact live value, passed straight back to vanilla
    bool secondaryPresent = true;
    std::string reason;
};

inline bool NearlyEqual(float a, float b, float tolerance = kFundingEpsilon) {
    return std::fabs(a - b) <= tolerance;
}

inline WritePlan PlanBuildingFundingWrite(const WriterConfig& config,
                                          const std::vector<TargetCandidate>& candidates) {
    WritePlan plan;
    plan.selector = FundingSelector(config.family);
    if (!config.enabled || config.mode == WriteMode::Off) {
        plan.reason = "writer not armed";
        return plan;
    }
    if (!IsSupportedWriterClass(config.family, config.subtype)) {
        plan.result = WriteResult::UnsupportedClass;
        plan.reason = "class '" + config.subtype + "' is not permitted in Phase 3";
        return plan;
    }
    if (config.targetBuilding < kMinFunding || config.targetBuilding > kMaxFunding ||
        std::fabs(config.targetBuilding - config.expectedBuilding) > kMaxTestDelta + kFundingEpsilon) {
        plan.result = WriteResult::TargetOutOfRange;
        plan.reason = "target outside [0.05,1.2] or more than 0.05 away from the expected value";
        return plan;
    }
    std::vector<std::size_t> matches;
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        const auto& c = candidates[i];
        if (c.family == config.family && c.subtype == config.subtype && c.cellX == config.cellX &&
            c.cellZ == config.cellZ)
            matches.push_back(i);
    }
    if (matches.empty()) {
        plan.result = WriteResult::TargetNotFound;
        plan.reason = "no facility matches family/class/cell";
        return plan;
    }
    if (matches.size() > 1) {
        plan.result = WriteResult::TargetAmbiguous;
        plan.reason = std::to_string(matches.size()) + " facilities match family/class/cell";
        return plan;
    }
    plan.targetIndex = matches[0];
    const auto& now = candidates[plan.targetIndex].state;
    plan.secondaryToWrite = now.secondary; // exact live value, never a reconstruction
    plan.secondaryPresent = now.hasSecondary;

    // Secondary funding is a hard invariant: it must be what the operator expected in every case.
    if (config.expectedHasSecondary != now.hasSecondary ||
        (now.hasSecondary && !NearlyEqual(now.secondary, config.expectedSecondary))) {
        plan.result = WriteResult::PreconditionMismatch;
        plan.reason = "secondary funding differs from the expected value";
        return plan;
    }
    if (NearlyEqual(now.building, config.targetBuilding)) {
        plan.result = WriteResult::AlreadyApplied;
        plan.buildingToWrite = now.building;
        plan.reason = "building funding already equals the target; no setter call";
        return plan;
    }
    if (!NearlyEqual(now.building, config.expectedBuilding)) {
        plan.result = WriteResult::PreconditionMismatch;
        plan.reason = "building funding differs from the expected value (user change?)";
        return plan;
    }
    // Do not lower funding below what the facility currently needs.
    const auto& c = candidates[plan.targetIndex];
    if (config.targetBuilding < now.building) {
        if (!c.usage || c.baseCapacity <= 0) {
            plan.result = WriteResult::UnsafeLowering;
            plan.reason = "usage unsampled or base capacity unknown; refusing to lower";
            return plan;
        }
        const double predicted = static_cast<double>(c.baseCapacity) * config.targetBuilding;
        if (predicted < static_cast<double>(*c.usage) * kCapacitySafetyFactor) {
            plan.result = WriteResult::UnsafeLowering;
            plan.reason = "predicted capacity would fall below usage * 1.25";
            return plan;
        }
    }
    plan.buildingToWrite = config.targetBuilding;
    plan.result = config.mode == WriteMode::DryRun ? WriteResult::DryRunOk : WriteResult::ReadyToApply;
    plan.reason = plan.result == WriteResult::DryRunOk ? "dry run: setter NOT called" : "precondition satisfied";
    return plan;
}

// One representation set read back after (or before) a write.
struct Readback {
    bool hasSecondary = true;
    float getterBuilding = 0, getterSecondary = 0;   // GetOccupantFundingPercentages(selector)
    float bufferBuilding = 0, bufferSecondary = 0;   // query buffer +0x2C / +0x34
    float budgetBuilding = 0, budgetSecondary = 0;   // BudgetSimulator items
    std::int64_t baseCost = 0;
    std::int64_t monthlyCost = 0;                    // reported by the caller from RoundedFundingCost
    std::int64_t capacity = 0;                       // EstimateCurrentOccupantCapacity out1
    std::int64_t baseCapacity = 0;                   // query buffer +0x14
};

// Writer/readback coherence, assessed BEFORE any write. VerifyReadback requires, after a write, that the
// getter, the query buffer and the budget item all report the written value; that is only possible when
// the three channels are views of one quantity, which the vanilla writer controls. A facility whose
// channels already disagree at rest (e.g. a budget line that exists on the exemplar while the getter
// reports an unrelated value) is outside the supported mechanism: the setter would not converge. Same
// tolerances as VerifyReadback; the secondary pair is compared only when the shape has one (a staff-only
// facility's getter out2 is a fallback and has no meaning). NaN never compares equal, so it fails closed.
struct FundingCoherence {
    bool coherent = false;
    std::string reason;
};

inline FundingCoherence AssessFundingCoherence(const Readback& r) {
    const auto fail = [](const char* why) { return FundingCoherence{false, why}; };
    if (!NearlyEqual(r.getterBuilding, r.budgetBuilding)) return fail("building getter != budget item");
    if (!NearlyEqual(r.bufferBuilding, r.budgetBuilding)) return fail("building query buffer != budget item");
    if (r.hasSecondary) {
        if (!NearlyEqual(r.getterSecondary, r.budgetSecondary, kSecondaryExactTolerance))
            return fail("secondary getter != budget item");
        if (!NearlyEqual(r.bufferSecondary, r.budgetSecondary, kSecondaryExactTolerance))
            return fail("secondary query buffer != budget item");
    }
    return {true, "getter, query buffer and budget item agree"};
}

struct VerifyOutcome {
    bool pass = false;
    std::string reason;
};

// Capacity after a write. The absolute model (base x CapacityFactor, +-1) holds when the city applies no capacity modifier. A city can scale every facility by a constant
// (e.g. x1.15 on schools and hospitals alike), so the capacity is also accepted when it follows the change of funding from the capacity read BEFORE the write:
// before x CapacityFactor(after) / CapacityFactor(before), within the propagated jitter. A write that did not move the capacity as the funding change implies still fails.
inline bool CapacityFollowsWrite(const Readback& before, const Readback& after, float expectedBuilding) {
    if (before.capacity <= 0 || before.baseCapacity <= 0 || before.baseCapacity != after.baseCapacity) return false;
    if (!std::isfinite(before.getterBuilding) || before.getterBuilding <= 0.0f) return false;
    const double ratio = CapacityFactor(expectedBuilding) / CapacityFactor(before.getterBuilding);
    const double expected = static_cast<double>(before.capacity) * ratio;
    return std::fabs(static_cast<double>(after.capacity) - expected) <= ratio + 2.0;
}

inline VerifyOutcome VerifyReadback(const Readback& r, float expectedBuilding, float expectedSecondary,
                                    bool expectedHasSecondary = true, const Readback* before = nullptr) {
    const auto fail = [](const char* why) { return VerifyOutcome{false, why}; };
    if (!NearlyEqual(r.getterBuilding, expectedBuilding)) return fail("getter building funding mismatch");
    if (!NearlyEqual(r.bufferBuilding, expectedBuilding)) return fail("query-buffer building funding mismatch");
    if (!NearlyEqual(r.budgetBuilding, expectedBuilding)) return fail("budget item building funding mismatch");
    if (r.hasSecondary != expectedHasSecondary) return fail("secondary line appeared or disappeared");
    if (expectedHasSecondary) {
        if (!NearlyEqual(r.getterSecondary, expectedSecondary, kSecondaryExactTolerance))
            return fail("getter secondary funding changed");
        if (!NearlyEqual(r.bufferSecondary, expectedSecondary, kSecondaryExactTolerance))
            return fail("query-buffer secondary funding changed");
        if (!NearlyEqual(r.budgetSecondary, expectedSecondary, kSecondaryExactTolerance))
            return fail("budget item secondary funding changed");
    }
    if (r.monthlyCost != RoundedFundingCost(r.baseCost, expectedBuilding))
        return fail("monthly cost does not follow round(cost * funding)");
    if (r.baseCapacity > 0 && !CapacityWithinModel(r.capacity, r.baseCapacity, expectedBuilding) && !(before && CapacityFollowsWrite(*before, r, expectedBuilding)))
        return fail("capacity outside the +-1 model");
    return {true, "all representations agree"};
}

// A facility whose funding LOOKS adjustable (three coherent channels) but ignores the setter. The distinction from a partially applied write is strict: the setter ran, the target was
// not confirmed, and the complete readback AFTER is bit-for-bit the readback BEFORE (every funding channel, the secondary line when present, the base and monthly cost, the capacity),
// both readbacks are finite and coherent, and the target really differed from the starting value. Anything else (a single changed field, NaN, an incoherent or incomplete readback)
// is NOT proof of "no effect" and stays an unverified write.
inline bool ReadbackFinite(const Readback& r) {
    return std::isfinite(r.getterBuilding) && std::isfinite(r.bufferBuilding) && std::isfinite(r.budgetBuilding) &&
           (!r.hasSecondary || (std::isfinite(r.getterSecondary) && std::isfinite(r.bufferSecondary) && std::isfinite(r.budgetSecondary)));
}
inline bool ReadbackIdentical(const Readback& a, const Readback& b) {
    if (a.hasSecondary != b.hasSecondary) return false;
    if (!(a.getterBuilding == b.getterBuilding && a.bufferBuilding == b.bufferBuilding && a.budgetBuilding == b.budgetBuilding)) return false;
    if (a.hasSecondary && !(a.getterSecondary == b.getterSecondary && a.bufferSecondary == b.bufferSecondary && a.budgetSecondary == b.budgetSecondary)) return false;
    return a.baseCost == b.baseCost && a.monthlyCost == b.monthlyCost && a.capacity == b.capacity && a.baseCapacity == b.baseCapacity;
}
inline bool SetterHadNoEffect(const Readback& before, const Readback& after, float expectedBuilding) {
    if (!ReadbackFinite(before) || !ReadbackFinite(after) || !std::isfinite(expectedBuilding)) return false;
    if (!AssessFundingCoherence(before).coherent || !AssessFundingCoherence(after).coherent) return false;
    if (NearlyEqual(before.getterBuilding, expectedBuilding)) return false;       // not an attempt to change anything
    return ReadbackIdentical(before, after);
}

// --- strict INI parser: unknown keys, duplicates, or bad numbers fail closed ------------------

inline std::string Trim(const std::string& s) {
    const auto b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    return s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
}

inline bool ParseFloatStrict(const std::string& text, float& out) {
    if (text.empty()) return false;
    char* end = nullptr;
    const float v = std::strtof(text.c_str(), &end);
    if (end != text.c_str() + text.size() || !std::isfinite(v)) return false;
    out = v;
    return true;
}

inline bool ParseIntStrict(const std::string& text, int& out) {
    if (text.empty()) return false;
    char* end = nullptr;
    const long v = std::strtol(text.c_str(), &end, 10);
    if (end != text.c_str() + text.size()) return false;
    out = static_cast<int>(v);
    return true;
}

// Returns true only for a complete, well-formed config. On any problem `config` is left
// disabled and `error` explains why (NO WRITE).
inline bool ParseWriterConfig(const std::string& text, WriterConfig& config, std::string& error) {
    WriterConfig parsed;
    unsigned seen = 0;
    enum : unsigned { kEnabled = 1, kMode = 2, kNonce = 4, kFamily = 8, kClass = 16, kCellX = 32,
                      kCellZ = 64, kExpB = 128, kExpS = 256, kTarget = 512, kAll = 1023 };
    std::size_t pos = 0;
    while (pos <= text.size()) {
        auto eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size();
        const std::string line = Trim(text.substr(pos, eol - pos));
        pos = eol + 1;
        if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') continue;
        const auto eq = line.find('=');
        if (eq == std::string::npos) { error = "malformed line: " + line; config = WriterConfig{}; return false; }
        const std::string key = Trim(line.substr(0, eq)), value = Trim(line.substr(eq + 1));
        unsigned bit = 0;
        bool ok = true;
        if (key == "Enabled") {
            bit = kEnabled;
            if (value == "true") parsed.enabled = true;
            else if (value == "false") parsed.enabled = false;
            else ok = false;
        } else if (key == "Mode") {
            bit = kMode;
            if (value == "Off") parsed.mode = WriteMode::Off;
            else if (value == "DryRun") parsed.mode = WriteMode::DryRun;
            else if (value == "Apply") parsed.mode = WriteMode::Apply;
            else ok = false;
        } else if (key == "Nonce") { bit = kNonce; parsed.nonce = value; ok = !value.empty(); }
        else if (key == "Family") {
            bit = kFamily;
            if (value == "School") parsed.family = FundingFamily::School;
            else if (value == "Health") parsed.family = FundingFamily::Health;
            else ok = false;
        } else if (key == "Class") { bit = kClass; parsed.subtype = value; ok = !value.empty(); }
        else if (key == "CellX") { bit = kCellX; ok = ParseIntStrict(value, parsed.cellX); }
        else if (key == "CellZ") { bit = kCellZ; ok = ParseIntStrict(value, parsed.cellZ); }
        else if (key == "ExpectedBuildingFunding") { bit = kExpB; ok = ParseFloatStrict(value, parsed.expectedBuilding); }
        else if (key == "ExpectedSecondaryFunding") {
            bit = kExpS;
            if (value == "none") { parsed.expectedHasSecondary = false; parsed.expectedSecondary = 0; }
            else ok = ParseFloatStrict(value, parsed.expectedSecondary);
        }
        else if (key == "TargetBuildingFunding") { bit = kTarget; ok = ParseFloatStrict(value, parsed.targetBuilding); }
        else { error = "unknown key: " + key; config = WriterConfig{}; return false; }
        if (!ok) { error = "bad value for " + key; config = WriterConfig{}; return false; }
        if (seen & bit) { error = "duplicate key: " + key; config = WriterConfig{}; return false; }
        seen |= bit;
    }
    if (seen != kAll) { error = "missing keys"; config = WriterConfig{}; return false; }
    config = parsed;
    error.clear();
    return true;
}

} // namespace sc4af
