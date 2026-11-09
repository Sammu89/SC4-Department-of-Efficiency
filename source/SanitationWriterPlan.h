// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure, game-free transaction layer for the Sanitation (garbage) department funding: it defines what a correct funding write looks
// like (compare-and-set, readback, confirmation of the delayed effect).
//
// Engine behaviour:
//  * Garbage uses the same cISC4DepartmentBudget class as Water: dept->SetFundingPercentage(target, lineItem = 0) (0x006AB810).
//    It clamps the value to [0, maxAllowed] (the lower bound is the static zero percentage at 0x00B49030, NOT a tuned minimum:
//    the tuned "min acceptable funding" 0x29121E8B is read only by the power simulator), refuses fixed-funding departments,
//    stores the value synchronously and broadcasts 0x08DAC3E7.
//  * The service effect is DELAYED and staged (observed example, funding 0.61 -> 0.50 on day 3 of the year):
//        funding changes at once                       0.61 -> 0.50
//        PollutionSimulator +0x228 (fundingEfficiency) 0.40125 -> 0.25 on day 17 (= powerCurve(funding*100) * 0.01)
//        monthly W2E capacity getter                   34088 -> 21238 on day 18 (rebuilt every SimNewDay from the per-building
//                                                      capacities, which already include +0x228 and the building condition)
//  * Hence: a write is only "applied" when the funding value reads back; the control is only "confirmed" when the efficiency
//    equals the curve value for the new funding AND the capacity getters followed it. Nothing is waited for by clock: the machine
//    reacts to the readback, and a new write is refused while the previous one has not propagated.

#include "UtilityHealth.h"
#include "UtilityModel.h"
#include "UtilityWriterPlan.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>
#include <string>

namespace sc4af {

enum class SanitationWriteState {
    NotStarted,
    WriteIssued,                       // the executor must perform the one vanilla call and report it
    FundingAppliedPendingCapacity,     // the funding value reads back; efficiency / capacity have not followed yet
    CapacityReadbackConfirmed,         // terminal success: efficiency and capacity reflect the new funding
    AlreadyInState,
    NotAvailable,
    NotEligible,
    PreviousReadbackPending,           // refusal: the last funding change has not propagated to efficiency / capacity
    PreconditionChanged,
    VerifyFailed,
    InvalidSnapshot,
};

inline const char* SanitationWriteStateName(SanitationWriteState s) {
    switch (s) {
    case SanitationWriteState::NotStarted: return "NOT_STARTED";
    case SanitationWriteState::WriteIssued: return "WRITE_ISSUED";
    case SanitationWriteState::FundingAppliedPendingCapacity: return "FUNDING_APPLIED_PENDING_CAPACITY";
    case SanitationWriteState::CapacityReadbackConfirmed: return "CAPACITY_READBACK_CONFIRMED";
    case SanitationWriteState::AlreadyInState: return "ALREADY_IN_STATE";
    case SanitationWriteState::NotAvailable: return "NOT_AVAILABLE";
    case SanitationWriteState::NotEligible: return "NOT_ELIGIBLE";
    case SanitationWriteState::PreviousReadbackPending: return "PREVIOUS_READBACK_PENDING";
    case SanitationWriteState::PreconditionChanged: return "PRECONDITION_CHANGED";
    case SanitationWriteState::VerifyFailed: return "VERIFY_FAILED";
    case SanitationWriteState::InvalidSnapshot: return "INVALID_SNAPSHOT";
    }
    return "?";
}

inline bool IsTerminal(SanitationWriteState s) {
    return s != SanitationWriteState::NotStarted && s != SanitationWriteState::WriteIssued &&
           s != SanitationWriteState::FundingAppliedPendingCapacity;
}

// What the garbage controller hands to the writer.
struct GarbageFundingWritePlan {
    int expectedPercent = 100;            // semantic state the plan was made against (compare-and-set)
    int targetPercent = 100;
    std::int64_t expectedMonthlyDelta = 0; // expense change the optimizer attributed to it (negative = saving)
    std::string reason;
    // required post-write evidence (informational; the machine enforces it)
    bool requireFundingReadback = true;
    bool requireEfficiencyAtCurve = true;
    bool requireCapacityFollowed = true;
};

struct SanitationRequest {
    std::uint32_t lifecycleEpoch = 0;
    int expectedPercent = 100;
    int targetPercent = 100;
    std::string reason;
};

inline SanitationRequest MakeRequest(const GarbageFundingWritePlan& plan, std::uint32_t epoch) {
    return SanitationRequest{epoch, plan.expectedPercent, plan.targetPercent, plan.reason};
}

struct SanitationObservation {
    bool valid = false;
    std::uint32_t lifecycleEpoch = 0;
    std::int64_t simDay = 0;                 // year * 365 + dayOfYear, only for ordering observations
    bool departmentPresent = false;
    Opt<float> funding;                      // department funding fraction
    Opt<bool> fixedFunding;
    Opt<float> maxAllowed;
    Opt<float> fundingEfficiency;            // PollutionSimulator +0x228
    Opt<std::int64_t> incineratorCapacity;   // authoritative monthly getters (already include funding and condition)
    Opt<std::int64_t> w2eCapacity;
};

inline SanitationObservation ObserveSanitation(const UtilitySnapshot& s) {
    SanitationObservation o;
    const auto& g = s.garbage;
    // ValidEmpty (a city with no garbage structure, the absence PROVEN) is a valid read of the department too: the budget of such a city may be cut. Invalid is still refused.
    o.valid = g.validity == SnapshotValidity::Valid || g.validity == SnapshotValidity::Partial || g.validity == SnapshotValidity::ValidEmpty;
    o.lifecycleEpoch = s.lifecycleEpoch;
    o.simDay = static_cast<std::int64_t>(s.simYear) * 365 + s.simDayOfYear;
    o.departmentPresent = g.department.present;
    o.funding = g.department.funding;
    o.fixedFunding = g.department.fixedFunding;
    o.maxAllowed = g.department.maxAllowed;
    o.fundingEfficiency = g.aggregate.fundingEfficiency;
    if (g.aggregate.incineratorCapacityMonthly) o.incineratorCapacity = *g.aggregate.incineratorCapacityMonthly;
    if (g.aggregate.w2eCapacityMonthly) o.w2eCapacity = *g.aggregate.w2eCapacityMonthly;
    return o;
}

// PollutionSimulator +0x228 = FundingToEfficiencyCurve(funding * 100) * 0.01 (0x006E44B0..0x006E4516).
inline double ExpectedSanitationEfficiency(const Curve& curve, double fundingFraction) {
    return curve.Eval(fundingFraction * 100.0) / 100.0;
}

constexpr double kSanitationEfficiencyTolerance = 1e-3;

// True when the delayed stage of the LAST funding change is complete: +0x228 equals the curve value of the current funding.
inline bool SanitationEfficiencySettled(const SanitationObservation& o, const Curve& curve) {
    if (!o.funding || !o.fundingEfficiency || !curve.valid()) return false;
    return std::fabs(static_cast<double>(*o.fundingEfficiency) - ExpectedSanitationEfficiency(curve, *o.funding)) <= kSanitationEfficiencyTolerance;
}

// Pure planning: may a write happen now? (department present, not fixed, target inside the vanilla range, fresh funding equals
// the planned one, previous change propagated). It does NOT judge service sufficiency: that is the controller's decision.
inline UtilityWritePlan PlanSanitationFunding(const SanitationObservation& o, const SanitationRequest& req, const Curve& curve) {
    if (!o.valid) return Refuse(UtilityWriteResult::InvalidSnapshot, "garbage snapshot is not valid");
    if (!o.departmentPresent) return Refuse(UtilityWriteResult::NotAvailable, "no garbage department control");
    if (!o.funding) return Refuse(UtilityWriteResult::NotAvailable, "garbage funding is unreadable");
    if (!o.fixedFunding) return Refuse(UtilityWriteResult::NotAvailable, "fixed-funding flag is unreadable");
    if (*o.fixedFunding) return Refuse(UtilityWriteResult::NotEligible, "department has fixed funding");
    if (!o.maxAllowed) return Refuse(UtilityWriteResult::NotAvailable, "maximum allowed funding is unreadable");
    if (!curve.valid()) return Refuse(UtilityWriteResult::NotAvailable, "funding-to-efficiency curve is unknown");
    const int maxPercent = UtilityPercentRound(*o.maxAllowed);
    if (req.targetPercent < 0 || req.targetPercent > maxPercent)
        return Refuse(UtilityWriteResult::NotEligible, "target outside [0, maxAllowed] (the vanilla setter clamps to that range)");
    const float expected = UtilityFractionForPercent(req.expectedPercent);
    const float target = UtilityFractionForPercent(req.targetPercent);
    if (FractionNear(*o.funding, target) && !FractionNear(*o.funding, expected))
        return Refuse(UtilityWriteResult::AlreadyInState, "funding is already at the target");
    if (!FractionNear(*o.funding, expected)) return Refuse(UtilityWriteResult::PreconditionChanged, "funding differs from the planned value");
    if (FractionNear(expected, target)) return Refuse(UtilityWriteResult::AlreadyInState, "target equals the current funding");
    // The previous change must have propagated, otherwise the capacity getters describe the OLD funding and any decision made
    // from them (including this one) would be made against a stale model.
    if (!o.fundingEfficiency) return Refuse(UtilityWriteResult::NotAvailable, "PollutionSimulator +0x228 efficiency is unreadable (version gate)");
    if (!SanitationEfficiencySettled(o, curve))
        return Refuse(UtilityWriteResult::NotEligible, "PREVIOUS_READBACK_PENDING: efficiency does not yet follow the current funding");
    UtilityWritePlan plan;
    plan.result = UtilityWriteResult::Applied;
    plan.write = true;
    plan.targetFraction = target;
    plan.targetPercent = req.targetPercent;
    return plan;
}

struct SanitationStep {
    bool write = false;                 // the executor must call dept->SetFundingPercentage(targetFraction, 0) and then Written()
    float targetFraction = 0.0f;
    SanitationWriteState state = SanitationWriteState::NotStarted;
    std::string reason;
};

class SanitationWriteMachine {
public:
    SanitationWriteMachine(SanitationRequest request, Curve curve) : req_(std::move(request)), curve_(std::move(curve)) {}

    const SanitationRequest& request() const { return req_; }
    SanitationWriteState state() const { return state_; }
    bool terminal() const { return IsTerminal(state_); }
    const std::string& reason() const { return reason_; }
    bool writeIssued() const { return issued_; }
    bool fundingReadBack() const { return fundingSeen_; }   // the funding value read back as the target (stage one of two)

    // The executor performed the vanilla call. The next Next() reads the funding back; success is never inferred from the call.
    void Written() { if (state_ == SanitationWriteState::WriteIssued) state_ = SanitationWriteState::FundingAppliedPendingCapacity; }
    void Abort(std::string why) { state_ = SanitationWriteState::VerifyFailed; reason_ = std::move(why); }

    SanitationStep Next(const SanitationObservation& o) {
        if (terminal()) return Report();
        if (state_ == SanitationWriteState::WriteIssued) return Out(false, state_, "write issued but not reported as executed");
        if (!o.valid) return Out(false, issued_ ? state_ : SanitationWriteState::InvalidSnapshot, "garbage snapshot is not valid (nothing assumed)");
        if (o.lifecycleEpoch != req_.lifecycleEpoch) return Finish(SanitationWriteState::PreconditionChanged, "city lifecycle changed");
        if (!issued_) return Decide(o);
        return Verify(o);
    }

private:
    SanitationStep Report() const { return Out(false, state_, reason_); }
    SanitationStep Out(bool write, SanitationWriteState s, std::string why) const {
        SanitationStep st;
        st.write = write;
        st.targetFraction = UtilityFractionForPercent(req_.targetPercent);
        st.state = s;
        st.reason = std::move(why);
        return st;
    }
    SanitationStep Finish(SanitationWriteState s, std::string why) {
        state_ = s;
        reason_ = std::move(why);
        return Out(false, s, reason_);
    }

    SanitationStep Decide(const SanitationObservation& o) {
        const auto plan = PlanSanitationFunding(o, req_, curve_);
        if (!plan.write) {
            switch (plan.result) {
            case UtilityWriteResult::AlreadyInState: return Finish(SanitationWriteState::AlreadyInState, plan.reason);
            case UtilityWriteResult::NotAvailable: return Finish(SanitationWriteState::NotAvailable, plan.reason);
            case UtilityWriteResult::PreconditionChanged: return Finish(SanitationWriteState::PreconditionChanged, plan.reason);
            case UtilityWriteResult::InvalidSnapshot: return Finish(SanitationWriteState::InvalidSnapshot, plan.reason);
            default:
                if (plan.reason.rfind("PREVIOUS_READBACK_PENDING", 0) == 0) return Finish(SanitationWriteState::PreviousReadbackPending, plan.reason);
                return Finish(SanitationWriteState::NotEligible, plan.reason);
            }
        }
        issued_ = true;
        baseline_.efficiency = o.fundingEfficiency;
        baseline_.capacity = CapacityOf(o);
        state_ = SanitationWriteState::WriteIssued;
        reason_ = "funding compare-and-set passed; perform dept->SetFundingPercentage(target, 0)";
        return Out(true, state_, reason_);
    }

    static std::int64_t CapacityOf(const SanitationObservation& o) {
        return (o.incineratorCapacity ? *o.incineratorCapacity : 0) + (o.w2eCapacity ? *o.w2eCapacity : 0);
    }

    SanitationStep Verify(const SanitationObservation& o) {
        if (!o.funding) return Out(false, state_, "funding unreadable: not a verdict");
        const float target = UtilityFractionForPercent(req_.targetPercent);
        if (!FractionNear(*o.funding, target)) {
            // Immediately after the write the value must read back. Later on a different value means someone else moved the slider.
            if (!fundingSeen_) return Finish(SanitationWriteState::VerifyFailed, "the funding value did not read back as the target after the write");
            return Finish(SanitationWriteState::PreconditionChanged, "funding was changed by someone else while the effect was propagating");
        }
        fundingSeen_ = true;
        if (!o.fundingEfficiency) return Out(false, state_, "efficiency unreadable: capacity confirmation cannot be proven");
        if (!SanitationEfficiencySettled(o, curve_)) return Out(false, state_, "funding applied; efficiency has not followed the curve yet");
        const std::int64_t base = baseline_.capacity;
        if (base > 0 && baseline_.efficiency && *baseline_.efficiency > 0.0f) {
            // Capacity getters are rebuilt every SimNewDay from per-building capacities that already include the efficiency.
            const double expected = static_cast<double>(base) * static_cast<double>(*o.fundingEfficiency) / static_cast<double>(*baseline_.efficiency);
            const double tolerance = std::max(8.0, 0.03 * expected);
            const double now = static_cast<double>(CapacityOf(o));
            if (std::fabs(now - expected) > tolerance)
                return Out(false, state_, "efficiency follows the curve; capacity getters have not been rebuilt yet");
        }
        return Finish(SanitationWriteState::CapacityReadbackConfirmed, "funding, efficiency and capacity reflect the new funding");
    }

    struct Baseline {
        Opt<float> efficiency;
        std::int64_t capacity = 0;
    };

    SanitationRequest req_;
    Curve curve_;
    SanitationWriteState state_ = SanitationWriteState::NotStarted;
    std::string reason_ = "not started";
    bool issued_ = false;
    bool fundingSeen_ = false;
    Baseline baseline_;
};

} // namespace sc4af
