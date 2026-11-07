// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure planning + verification for the utility writers (power plant funding, water department funding).
// No game types here: the read layer supplies the snapshot, the write layer performs the one vanilla call,
// this file decides whether it may happen and whether it worked.

#include "UtilityModel.h"

#include <cmath>
#include <cstdint>
#include <string>

namespace sc4af {

enum class UtilityWriteResult {
    Applied,
    AlreadyInState,
    NotAvailable,
    NotEligible,
    TargetMissing,
    TargetAmbiguous,
    PreconditionChanged,
    VanillaRejected,
    VerificationFailed,
    UnsupportedMechanism,
    InvalidSnapshot,
};

inline const char* UtilityWriteResultName(UtilityWriteResult r) {
    switch (r) {
    case UtilityWriteResult::Applied: return "Applied";
    case UtilityWriteResult::AlreadyInState: return "AlreadyInState";
    case UtilityWriteResult::NotAvailable: return "NotAvailable";
    case UtilityWriteResult::NotEligible: return "NotEligible";
    case UtilityWriteResult::TargetMissing: return "TargetMissing";
    case UtilityWriteResult::TargetAmbiguous: return "TargetAmbiguous";
    case UtilityWriteResult::PreconditionChanged: return "PreconditionChanged";
    case UtilityWriteResult::VanillaRejected: return "VanillaRejected";
    case UtilityWriteResult::VerificationFailed: return "VerificationFailed";
    case UtilityWriteResult::UnsupportedMechanism: return "UnsupportedMechanism";
    case UtilityWriteResult::InvalidSnapshot: return "InvalidSnapshot";
    }
    return "?";
}

// The vanilla slider commits `int * 0.01f` (same grid as the civic sliders).
inline float UtilityFractionForPercent(int percent) { return static_cast<float>(percent) * 0.01f; }
inline int UtilityPercentForFraction(float f) { return static_cast<int>(std::lround(f * 100.0f)); }
inline bool FractionNear(float a, float b) { return std::fabs(a - b) <= 2e-4f; }
// The plans work in whole percents (the snapshot rounds the funding it reads). A funding that is not a whole percent (e.g. 111.96% when another mod or the
// department slider scales the lines) rounds to the percent the plan expects: the compare-and-set compares in that same unit, otherwise the write would be
// refused as 'changed' forever although nothing changed.
inline bool PercentMatches(float fraction, int percent) { return UtilityPercentForFraction(fraction) == percent; }

struct UtilityLimits {
    int minPercent = 60;    // "Min Acceptable Funding" (tuning 0x29121E8B, runtime value, never assumed)
    int maxPercent = 120;   // department max allowed (tuning 0xE911E2D1 / 0xEA4025A9)
    int maxStepPercent = 10; // step limit of the isolated writers; the automatic path sets it to 100 (no throttle)
    double reserveFactor = 1.20;  // reserve floor (UtilityActuator overrides it from config); the part of the demand produced locally keeps factor x that part
    double imports = 0;           // dependable imports B: they cover demand WITHOUT the reserve (LocalRequired = ceil(R x max(0, D - B)) + S)
};

// The lowest water funding the executor may write must not be stricter than the optimizer's own floor: otherwise the optimizer may choose 57 % (floor
// 50 % = the pipe curve) while the executor, whose default legal minimum is 60 %, refuses the same write at every plan.
inline int WaterLegalMinPercent(bool protectWaterPipes, int waterMinPercent, int defaultMin = UtilityLimits{}.minPercent) {
    return protectWaterPipes ? (waterMinPercent < defaultMin ? waterMinPercent : defaultMin) : 0;
}

struct UtilityWritePlan {
    UtilityWriteResult result = UtilityWriteResult::NotEligible;  // Applied means "may write" here; the executor sets the final value
    bool write = false;
    float targetFraction = 0.0f;
    int targetPercent = 0;
    std::string reason;
};

inline UtilityWritePlan Refuse(UtilityWriteResult r, std::string why) {
    UtilityWritePlan p;
    p.result = r;
    p.reason = std::move(why);
    return p;
}

// ---- Power: one plant, addressed by its cell ------------------------------------------------------------
struct PowerWriteRequest {
    int cellX = 0, cellZ = 0;
    int expectedPercent = 100;   // what the operator believes it is now (compare-and-set)
    int targetPercent = 95;
};

inline UtilityWritePlan PlanPowerPlantFunding(const PowerState& state, const PowerWriteRequest& req, const Curve& efficiencyCurve,
                                              const UtilityLimits& limits, std::int64_t contractedExport = 0) {
    if (state.validity != SnapshotValidity::Valid && state.validity != SnapshotValidity::Partial)
        return Refuse(UtilityWriteResult::InvalidSnapshot, std::string("power snapshot is ") + ValidityName(state.validity));
    const PowerPlantRow* target = nullptr;
    int matches = 0;
    for (const auto& p : state.plants)
        if (p.ref.cellX == req.cellX && p.ref.cellZ == req.cellZ) { target = &p; ++matches; }
    if (matches == 0) return Refuse(UtilityWriteResult::TargetMissing, "no power plant at that cell");
    if (matches > 1) return Refuse(UtilityWriteResult::TargetAmbiguous, "several power plants at that cell");
    if (target->mechanism != MechanismClass::Compatible)
        return Refuse(UtilityWriteResult::UnsupportedMechanism, std::string("mechanism ") + MechanismName(target->mechanism));
    if (!target->funding) return Refuse(UtilityWriteResult::NotAvailable, "plant funding is unreadable");
    if (req.targetPercent < limits.minPercent || req.targetPercent > limits.maxPercent)
        return Refuse(UtilityWriteResult::NotEligible, "target outside the legal funding range");
    if (std::abs(req.targetPercent - req.expectedPercent) > limits.maxStepPercent)
        return Refuse(UtilityWriteResult::NotEligible, "step larger than the isolated-test limit");
    const float expected = UtilityFractionForPercent(req.expectedPercent);
    const float target_f = UtilityFractionForPercent(req.targetPercent);
    if (PercentMatches(*target->funding, req.targetPercent) && !PercentMatches(*target->funding, req.expectedPercent))
        return Refuse(UtilityWriteResult::AlreadyInState, "plant is already at the target");
    if (!PercentMatches(*target->funding, req.expectedPercent))
        return Refuse(UtilityWriteResult::PreconditionChanged, "plant funding differs from the expected value");
    if (FractionNear(expected, target_f)) return Refuse(UtilityWriteResult::AlreadyInState, "target equals expected");
    // Supply reserve: never plan a write that could push the city toward a blackout. Imports are contracts that
    // can lapse, so only LOCAL capacity counts, with every plant at its post-change efficiency.
    if (!state.aggregate.demand || !efficiencyCurve.valid()) return Refuse(UtilityWriteResult::NotAvailable, "demand or efficiency curve unknown");
    double capacityAfter = 0.0;
    for (const auto& p : state.plants) {
        if (!p.maxProduced || !p.funding) return Refuse(UtilityWriteResult::NotAvailable, "plant output unreadable");
        const bool isTarget = &p == target;
        const double funding = isTarget ? req.targetPercent : *p.funding * 100.0;
        capacityAfter += static_cast<double>(*p.maxProduced) * efficiencyCurve.Eval(funding) / 100.0 *
                         (p.conditionPercent ? *p.conditionPercent : 1.0);
    }
    // Contracted exports (sell deals) are served from the same local capacity: cutting funding while selling water can leave buildings unwatered.
    // An increase can never reduce capacity (monotonic efficiency curve): restoring a known-safe state is never blocked.
    if (req.targetPercent < req.expectedPercent &&
        capacityAfter < static_cast<double>(LocalRequiredCapacity(limits.reserveFactor, *state.aggregate.demand, static_cast<std::int64_t>(limits.imports),
                                                                    static_cast<std::int64_t>(contractedExport + limits.imports))))
        return Refuse(UtilityWriteResult::NotEligible, "local capacity after the change (minus contracted exports) would fall below the reserve");
    UtilityWritePlan plan;
    plan.result = UtilityWriteResult::Applied;
    plan.write = true;
    plan.targetFraction = target_f;
    plan.targetPercent = req.targetPercent;
    return plan;
}

// ---- Water: the department funding is the only control --------------------------------------------------
struct WaterWriteRequest {
    int expectedPercent = 100;
    int targetPercent = 95;
};

inline UtilityWritePlan PlanWaterFunding(const WaterState& state, const WaterWriteRequest& req, const Curve& efficiencyCurve,
                                         const UtilityLimits& limits, std::int64_t contractedExport = 0) {
    if (state.validity != SnapshotValidity::Valid && state.validity != SnapshotValidity::Partial)
        return Refuse(UtilityWriteResult::InvalidSnapshot, std::string("water snapshot is ") + ValidityName(state.validity));
    if (!state.department.present || !state.department.funding) return Refuse(UtilityWriteResult::NotAvailable, "water department control unreadable");
    if (state.department.fixedFunding && *state.department.fixedFunding)
        return Refuse(UtilityWriteResult::NotEligible, "department has fixed funding");
    for (const auto& p : state.producers)
        if (p.mechanism == MechanismClass::UnsupportedMechanism)
            return Refuse(UtilityWriteResult::UnsupportedMechanism, "a producer uses a non-standard mechanism");
    int maxAllowed = limits.maxPercent;
    if (state.department.maxAllowed) maxAllowed = std::min(maxAllowed, UtilityPercentForFraction(*state.department.maxAllowed));
    if (req.targetPercent < limits.minPercent || req.targetPercent > maxAllowed)
        return Refuse(UtilityWriteResult::NotEligible, "target outside the legal funding range");
    if (std::abs(req.targetPercent - req.expectedPercent) > limits.maxStepPercent)
        return Refuse(UtilityWriteResult::NotEligible, "step larger than the isolated-test limit");
    const float expected = UtilityFractionForPercent(req.expectedPercent);
    const float target_f = UtilityFractionForPercent(req.targetPercent);
    if (PercentMatches(*state.department.funding, req.targetPercent) && !PercentMatches(*state.department.funding, req.expectedPercent))
        return Refuse(UtilityWriteResult::AlreadyInState, "water funding is already at the target");
    if (!PercentMatches(*state.department.funding, req.expectedPercent))
        return Refuse(UtilityWriteResult::PreconditionChanged, "water funding differs from the expected value");
    if (FractionNear(expected, target_f)) return Refuse(UtilityWriteResult::AlreadyInState, "target equals expected");
    if (!state.aggregate.demand || !state.aggregate.productionCapacity || !efficiencyCurve.valid())
        return Refuse(UtilityWriteResult::NotAvailable, "demand, capacity or efficiency curve unknown");
    const double capacityAfter = static_cast<double>(*state.aggregate.productionCapacity) *
                                 efficiencyCurve.Eval(req.targetPercent) / std::max(1.0, efficiencyCurve.Eval(UtilityPercentForFraction(*state.department.funding)));
    const bool reduction = req.targetPercent < req.expectedPercent;   // increases can never reduce capacity
    if (reduction && capacityAfter < static_cast<double>(LocalRequiredCapacity(limits.reserveFactor, *state.aggregate.demand, static_cast<std::int64_t>(limits.imports),
                                                                                static_cast<std::int64_t>(contractedExport + limits.imports))))
        return Refuse(UtilityWriteResult::NotEligible, "local capacity after the change (minus contracted exports) would fall below the reserve");
    // A water snapshot whose service counts are not populated yet (right after load) or that already shows
    // unwatered buildings is never a moment to cut funding.
    if (reduction && !ServiceCountsReady(state.aggregate)) return Refuse(UtilityWriteResult::NotAvailable, "water service counts are not populated yet");
    if (reduction && UnwateredBlocksCut(state.aggregate)) return Refuse(UtilityWriteResult::NotEligible, "buildings are unwatered right now and the production does not cover the demand");
    UtilityWritePlan plan;
    plan.result = UtilityWriteResult::Applied;
    plan.write = true;
    plan.targetFraction = target_f;
    plan.targetPercent = req.targetPercent;
    return plan;
}

// ---- verification ----------------------------------------------------------------------------------------
struct FundingReadback {
    Opt<float> getter;        // the vanilla getter (plant: GetPowerPlantFundingPercentage; water: department)
    Opt<float> budgetLine;    // plant budget line local funding (plants only)
};

struct Verdict {
    bool pass = false;
    std::string reason;
};

inline Verdict VerifyFundingReadback(const FundingReadback& after, float targetFraction, bool requireBudgetLine) {
    Verdict v;
    if (!after.getter) { v.reason = "getter unreadable after write"; return v; }
    if (!FractionNear(*after.getter, targetFraction)) { v.reason = "getter does not show the target"; return v; }
    if (requireBudgetLine) {
        if (!after.budgetLine) { v.reason = "budget line unreadable after write"; return v; }
        if (!FractionNear(*after.budgetLine, targetFraction)) { v.reason = "budget line does not show the target"; return v; }
    }
    v.pass = true;
    return v;
}

} // namespace sc4af
