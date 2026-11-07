// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Validation tracker for the unified controller (pure, no game types). It watches the controller's EVENTS (never log text) and decides automatically
// which behaviours have been observed, so nobody has to read a value or guess when to stop.
//
// Statuses:  PASS          the behaviour was observed and was correct
//            NOT_REQUIRED  the controller evaluated the subsystem for long enough and the plan never called for that write (no gratuitous writes)
//            NOT_CAPTURED  not observed yet
//            FAIL_CLOSED   the safe fail-closed path was observed instead (a valid observation; the subsystem is not releasable)
//            FAIL          an invariant was violated
//
// Milestones (each emitted once): SESSION_PRE_RELOAD_COMPLETE, then after a save -> Region View -> reload of the same process, SESSION_COMPLETE.
// Completion is EVENT-driven (every behaviour observed and the system quiet: consecutive plans with nothing to do, nothing pending), never a count of
// arbitrary extra months, and it is impossible while anything is unresolved: a failure, a pending transaction, a stale transaction carried over a
// reload, a fail-closed subsystem, a repeatedly refused action or a deal that went back to an amount it already left.
// GRAPHICS_STABLE is the only criterion the controller cannot observe: the tester answers one question at the end of the session.

#include "CivicEvidence.h"
#include "DealCausal.h"
#include "UtilityConfig.h"

#include <cstdint>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace sc4af {

enum class VStatus { NotCaptured, Pass, NotRequired, FailClosed, Fail };

inline const char* VStatusName(VStatus s) {
    switch (s) {
    case VStatus::NotCaptured: return "NOT_CAPTURED";
    case VStatus::Pass: return "PASS";
    case VStatus::NotRequired: return "NOT_REQUIRED";
    case VStatus::FailClosed: return "FAIL_CLOSED";
    case VStatus::Fail: return "FAIL";
    }
    return "?";
}

inline bool Acceptable(VStatus s) { return s == VStatus::Pass || s == VStatus::NotRequired || s == VStatus::FailClosed; }

// READY only when every precondition of the unified run holds; the reason names the first one that does not.
inline std::string FinalValidationReadiness(const UtilityResearchConfig& c, bool versionSupported, bool capabilityPresent, bool controllerCreated, bool civicConfigured = true) {
    const char* reason = nullptr;
    if (!versionSupported) reason = "unsupported SimCity 4 version";
    else if (!capabilityPresent) reason = "binary is not a final-validation build (compile-time capability missing)";
    else if (!controllerCreated) reason = "unified controller not created";
    else if (c.phase8Runtime) reason = "Phase8Runtime=true";
    else if (c.landfillResearch) reason = "LandfillResearch=true";
    else if (c.mode != UtilityMode::Auto) reason = "Mode is not Auto";
    else if (!c.enabled) reason = "Enabled=false";
    else if (!c.unifiedAutoValidation) reason = "EnableUnifiedAutoValidation is not true";
    else if (!c.autoPower) reason = "AutoPower is not true";
    else if (!c.autoWater) reason = "AutoWater is not true";
    else if (!c.autoGarbage) reason = "AutoGarbage is not true";
    else if (!civicConfigured) reason = "DepartmentEfficiency.ini is not the Civic final-validation config (Mode=Auto, Education=true, Health=true)";
    if (reason) return std::string("[FinalValidation] NOT_READY reason=") + reason;
    // AutoDeals=false is a deliberate budgets-only round (Education, Health, Power, Water, Garbage): not an error, no failure cue.
    if (!c.autoDeals) return "[FinalValidation] READY_BUDGETS_ONLY (AutoDeals=false: neighbor deals are not validated in this configuration)";
    return "[FinalValidation] READY";
}

struct FvPlanEvent {
    std::uint32_t id = 0;
    std::uint32_t epoch = 0;
    bool powerValid = false, waterValid = false, sanitationValid = false, portfolioUsed = false;
    int powerActions = 0, waterActions = 0, sanitationActions = 0, dealActions = 0;
    bool pendingCarried = false;     // a transaction from before this plan was still in flight when the cycle began
};

enum class FvSanitation { Issued, FundingReadback, CapacityPending, Confirmed, Failed };

class FinalValidationTracker {
public:
    static constexpr int kNotRequiredPlans = 20;     // plans evaluated before "never needed" counts as NOT_REQUIRED
    static constexpr int kStablePlans = 8;           // plans evaluated after the first verified write (or after a reload) whatever they did: the evidence window of STABILITY; no inactivity is required
    static constexpr int kRepeatedRefusals = 3;      // the same action refused in this many consecutive replans is a failure of the model, not bad luck

    // ---- environment (what the loaded city can exercise) ----
    void SetEnvironment(bool power, bool water, bool sanitation) { power_ = power; water_ = water; sanitation_ = sanitation; envKnown_ = true; }

    // ---- events ----
    void OnPlan(const FvPlanEvent& e) {
        ++plans_;
        if (e.powerValid) ++plansPower_;
        if (e.waterValid) ++plansWater_;
        if (e.sanitationValid) ++plansSan_;
        if (e.portfolioUsed) ++plansPortfolio_;
        if (e.powerActions > 0) ++actPower_;
        if (e.waterActions > 0) ++actWater_;
        if (e.sanitationActions > 0) ++actSan_;
        if (e.dealActions > 0) ++actDeal_;
        if (anyWrite_) ++plansAfterWrite_;
        ++plansSinceWrite_;
        const bool quiet = e.powerActions + e.waterActions + e.sanitationActions + e.dealActions == 0 && !e.pendingCarried && !sanPending_ && dealsPending_ == 0;
        quiet_ = quiet ? quiet_ + 1 : 0;
        if (resets_ > 0) {
            ++postResetPlans_;
            ++postResetPlansSinceWrite_;
            postResetQuiet_ = quiet ? postResetQuiet_ + 1 : 0;
            if (postResetPlans_ == 1 && e.pendingCarried) staleCarried_ = true;
            if (postResetPlans_ == 1) postResetEpoch_ = e.epoch;
        } else {
            lastEpoch_ = e.epoch;
        }
    }
    void OnExecute(std::uint32_t planId, std::uint32_t currentPlanId, int phase, int lowestPhase) {
        ++executes_;
        if (planId != currentPlanId || phase != lowestPhase) ++phaseViolations_;
    }
    // `causal` is the fingerprint of the exogenous inputs the planner read for this write (empty = unknown). See Flip().
    void OnFundingWrite(bool power, bool verified, int direction, const std::string& causal = std::string()) {
        if (!verified) { ++unverified_; return; }
        Wrote();
        (power ? powerWrites_ : waterWrites_)++;
        Flip(power ? 0 : 1, direction, causal);
    }
    // One verified funding change of one control (`key`: a power plant cell, "department"): the keyed reversal check. `causal` = fingerprint of the exogenous inputs the plan read
    // (never the funding itself); empty = unknown (fail closed). OnFundingWrite(.., direction = 0) counts the write without the unkeyed check.
    void OnFundingAction(bool power, const std::string& key, int direction, const std::string& causal) { Flip(power ? 0 : 1, direction, causal, key); }
    void OnSanitation(FvSanitation e, int direction = 0, const std::string& causal = std::string()) {
        switch (e) {
        case FvSanitation::Issued:
            if (sanPending_) ++pendingViolations_;
            sanPending_ = true;
            ++sanIssued_;
            Wrote();
            Flip(2, direction, causal);
            break;
        case FvSanitation::FundingReadback: ++sanReadback_; break;
        case FvSanitation::CapacityPending: break;
        case FvSanitation::Confirmed: ++sanConfirmed_; sanPending_ = false; break;
        case FvSanitation::Failed: sanPending_ = false; break;
        }
    }
    void OnDealStart(bool garbageCommodity) {
        if (garbageCommodity && sanPending_) ++pendingViolations_;      // a Garbage deal must wait for CAPACITY_READBACK_CONFIRMED
        ++dealsPending_;
    }
    void OnQuote(bool wrote, bool divergence) {
        ++quotes_;
        if (!wrote) ++quotesNoWrite_;
        if (divergence) ++divergences_;
    }
    void OnDealApplied(bool cancel) {
        Wrote();
        ++dealApplied_;
        if (cancel) ++dealCancels_;
        if (dealsPending_ > 0) --dealsPending_;
    }
    // Semantic state of one contract slot (type, neighbor): the amount it holds after a verified write (0 = cancelled). The same slot
    // returning to an amount it left (A -> B -> A) is a ping-pong, whatever the market looked like.
    // Without a causal fingerprint an A -> B -> A reversal cannot be explained and fails closed.
    void OnDealApplied(bool cancel, std::int32_t type, std::uint32_t neighbor, std::int32_t amountBefore, std::int32_t amountAfter) {
        OnDealApplied(cancel, type, neighbor, amountBefore, amountAfter, DealCausal{});
    }
    // A reversal (the slot returns to an amount it left) is a ping-pong only when NOTHING the planner read changed between the two writes (equal, non-empty
    // fingerprints) or when that cannot be told (a fingerprint is unknown). A reversal whose exogenous inputs differ (the day, an AmountUsed, the load, the
    // market ...) is the optimiser adapting. No cooldown, no hysteresis, no tolerance.
    void OnDealApplied(bool cancel, std::int32_t type, std::uint32_t neighbor, std::int32_t amountBefore, std::int32_t amountAfter, const DealCausal& causal) {
        OnDealApplied(cancel);
        refusals_.erase(std::make_tuple(type, neighbor));
        const auto slot = std::make_tuple(type, neighbor);
        auto& h = history_[slot];
        lastSlot_ = slot;
        if (h.empty()) h.push_back(DealHist{amountBefore, DealCausal{}});
        if (h.back().amount != amountAfter) {
            const std::string name = "slot=" + std::to_string(type) + "/" + std::to_string(neighbor);
            std::ostringstream o;
            o << "[FinalValidation][DealStability] " << name << " previousAmount=" << h.back().amount << " newAmount=" << amountAfter << " direction=" << (amountAfter > h.back().amount ? "UP" : "DOWN")
              << " fingerprintHash=" << (causal.empty() ? std::string("UNKNOWN") : causal.Hash()) << " cause={" << causal.Text() << '}';
            if (h.size() >= 2 && h[h.size() - 2].amount == amountAfter) {
                const auto& prev = h.back().causal;
                const bool explained = !causal.empty() && !prev.empty() && causal.Hash() != prev.Hash();
                o << " previousFingerprintHash=" << (prev.empty() ? std::string("UNKNOWN") : prev.Hash()) << " currentFingerprintHash=" << (causal.empty() ? std::string("UNKNOWN") : causal.Hash())
                  << " causalEqual=" << (!causal.empty() && !prev.empty() && causal.Hash() == prev.Hash() ? "YES" : causal.empty() || prev.empty() ? "UNKNOWN" : "NO");
                if (explained) o << " changedFields=" << DealCausal::Changed(prev, causal) << " REVERSAL_EXPLAINED";
                else { o << " PING_PONG_UNEXPLAINED"; pingPong_ = true; }
            }
            diag_.push_back(o.str());
            h.push_back(DealHist{amountAfter, causal});
        }
    }
    // Structured diagnostic lines produced since the last call (the controller logs them).
    std::vector<std::string> TakeDiagnostics() { auto out = std::move(diag_); diag_.clear(); return out; }
    // FAILURE DUMP: built from the state already read, no write. `context` carries what the controller knows (plan, date, mode, targets, funding).
    std::vector<std::string> FailureDump(const std::string& context) {
        std::vector<std::string> out;
        if (dumped_) return out;
        dumped_ = true;
        std::ostringstream o;
        o << "[FinalValidation][FailureDump] reason=" << FailureReasons() << ' ' << context << " writesAfterFailure=" << writesAfterFail_;
        const auto it = history_.find(lastSlot_);
        if (it != history_.end()) {
            const auto& h = it->second;
            o << " lastDealSlot=" << std::get<0>(lastSlot_) << '/' << std::get<1>(lastSlot_) << " lastStatesOfSlot=";
            std::string changed;
            for (std::size_t i = h.size() > 3 ? h.size() - 3 : 0; i < h.size(); ++i) {
                o << (i + 1 == h.size() || i + 3 == h.size() ? "" : "") << h[i].amount << '(' << (h[i].causal.empty() ? std::string("UNKNOWN") : h[i].causal.Hash()) << ")";
                if (i + 1 < h.size()) o << "->";
            }
            if (h.size() >= 2) changed = DealCausal::Changed(h[h.size() - 2].causal, h.back().causal);
            o << " changedFieldsBetweenLastTwo=" << (changed.empty() ? std::string("NONE") : changed);
            if (!h.back().causal.empty()) o << " lastCause={" << h.back().causal.Text() << '}';
        } else {
            o << " lastDealSlot=NONE";
        }
        out.push_back(o.str());
        return out;
    }
    // A planned deal action the transaction refused (precondition changed, quantity not legal, vanilla gate...). The same slot refused in
    // consecutive replans is the signature of a model that disagrees with the game.
    void OnDealRefused(std::int32_t type, std::uint32_t neighbor, int op, std::int32_t target) {
        if (dealsPending_ > 0) --dealsPending_;
        auto& r = refusals_[std::make_tuple(type, neighbor)];
        if (r.op == op && r.target == target) ++r.count; else { r.op = op; r.target = target; r.count = 1; }
        if (r.count >= kRepeatedRefusals) repeatedRefusal_ = true;
    }
    void OnFailClosed(const std::string& what) { failClosed_.push_back(what); }
    void OnDay() { ++days_; }
    void OnMonth() {
        ++months_;
        ++monthsSinceWrite_;
        if (resets_ > 0) { ++postResetMonths_; ++postResetMonthsSinceWrite_; }
    }
    // ---- Civic funding (Education and Health): the separate controller is part of the same validation ----
    void SetCivicExpected(bool education, bool health) { civicExpected_[0] = education; civicExpected_[1] = health; civicKnown_ = true; }
    void OnCivicMonth(const CivicMonthEvidence& e) {
        if (!e.evaluated) return;
        const CivicFamilyEvidence* f[2] = {&e.education, &e.health};
        for (int i = 0; i < 2; ++i) {
            Civic& c = civic_[i];
            ++c.months;
            c.facilityMonths += f[i]->facilities > 0 ? 1 : 0;
            c.emptyMonths = f[i]->facilities > 0 ? 0 : c.emptyMonths + 1;
            c.writeDecisions += f[i]->writeDecisions;
            if (c.applied > 0 && f[i]->facilities > 0) ++c.postWriteMonths;      // evaluated months after the first verified write (before this month's own writes are added)
            c.applied += f[i]->applied;
            c.failed += f[i]->failed;
            c.serviceChecked += f[i]->serviceChecked;
            c.serviceBreach += f[i]->serviceBreach;
            c.secondaryChecked += f[i]->secondaryChecked;
            c.secondaryDrift += f[i]->secondaryDrift;
            c.quiet = f[i]->facilities > 0 && f[i]->writeDecisions == 0 && f[i]->failed == 0 ? c.quiet + 1 : (f[i]->facilities > 0 ? 0 : c.quiet);
            if (f[i]->writes.empty()) {
                // No per-facility evidence: the writer's identity and causes are unknown, so every reversal counts (fail closed).
                for (int k = 0; k < f[i]->raises; ++k) CivicFlip(i, 1);
                for (int k = 0; k < f[i]->cuts; ++k) CivicFlip(i, -1);
            } else {
                for (const auto& w : f[i]->writes) CivicFacilityWrite(i, w);
            }
        }
    }
    VStatus EducationPlan() const { return CivicPlan(0); }
    VStatus EducationWrite() const { return CivicWrite(0); }
    VStatus EducationService() const { return CivicService(0); }
    VStatus EducationStability() const { return CivicStability(0); }
    VStatus HealthPlan() const { return CivicPlan(1); }
    VStatus HealthWrite() const { return CivicWrite(1); }
    VStatus HealthService() const { return CivicService(1); }
    VStatus HealthStability() const { return CivicStability(1); }
    // Bus / ambulance (secondary) funding lines must be exactly preserved by every Civic write.
    VStatus CivicSecondaryFunding() const {
        if (civic_[0].secondaryDrift + civic_[1].secondaryDrift > 0) return VStatus::Fail;
        if (civic_[0].secondaryChecked + civic_[1].secondaryChecked > 0) return VStatus::Pass;
        if (!CivicAnyExpected()) return VStatus::NotRequired;
        return CivicWrite(0) == VStatus::NotRequired && CivicWrite(1) == VStatus::NotRequired ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    bool CivicFail() const {
        return CivicWrite(0) == VStatus::Fail || CivicWrite(1) == VStatus::Fail || CivicService(0) == VStatus::Fail || CivicService(1) == VStatus::Fail ||
               CivicStability(0) == VStatus::Fail || CivicStability(1) == VStatus::Fail || CivicSecondaryFunding() == VStatus::Fail;
    }
    bool CivicAcceptable() const {
        return Acceptable(EducationPlan()) && Acceptable(EducationWrite()) && Acceptable(EducationService()) && Acceptable(EducationStability()) &&
               Acceptable(HealthPlan()) && Acceptable(HealthWrite()) && Acceptable(HealthService()) && Acceptable(HealthStability()) && Acceptable(CivicSecondaryFunding());
    }

    // The tester's end-of-session answer to "were the graphics normal for the whole session? YES/NO".
    void OnGraphicsAnswer(bool normal) { graphics_ = normal ? 1 : -1; }
    void OnReset() {
        if (plans_ > 0) {
            ++resets_;
            postResetPlans_ = postResetMonths_ = 0;
            postResetPlansSinceWrite_ = postResetMonthsSinceWrite_ = 0;
            postResetQuiet_ = 0;
            staleCarried_ = false;
            sanPending_ = false;
            dealsPending_ = 0;
            for (auto& c : civic_) c.quiet = 0;
            refusals_.clear();
        }
    }

    // ---- derived status ----
    VStatus PowerPlan() const { return !power_ && envKnown_ ? VStatus::NotRequired : plansPower_ > 0 ? VStatus::Pass : VStatus::NotCaptured; }
    VStatus WaterPlan() const { return !water_ && envKnown_ ? VStatus::NotRequired : plansWater_ > 0 ? VStatus::Pass : VStatus::NotCaptured; }
    VStatus PowerWrite() const { return Write(power_, plansPower_, actPower_, powerWrites_, failedPower_); }
    VStatus WaterWrite() const { return Write(water_, plansWater_, actWater_, waterWrites_, failedWater_); }
    VStatus SanitationPlan() const { return !sanitation_ && envKnown_ ? VStatus::NotRequired : plansSan_ > 0 ? VStatus::Pass : VStatus::NotCaptured; }
    VStatus SanitationWrite() const {
        if (sanFailed_) return VStatus::FailClosed;
        if (sanIssued_ > 0) return sanReadback_ > 0 ? VStatus::Pass : VStatus::NotCaptured;
        if (!sanitation_ && envKnown_) return VStatus::NotRequired;
        return NeverNeeded(sanitation_, plansSan_, actSan_) ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus SanitationPropagation() const {
        if (sanFailed_) return VStatus::FailClosed;
        if (sanIssued_ > 0) return sanConfirmed_ > 0 ? VStatus::Pass : VStatus::NotCaptured;
        if (!sanitation_ && envKnown_) return VStatus::NotRequired;
        return NeverNeeded(sanitation_, plansSan_, actSan_) ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus DealPortfolio() const { return plansPortfolio_ > 0 ? VStatus::Pass : VStatus::NotCaptured; }
    VStatus DealWrite() const {
        if (dealFailed_) return VStatus::FailClosed;
        if (dealApplied_ > 0) return VStatus::Pass;
        return NeverNeeded(true, plansPortfolio_, actDeal_) ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus DealAccounting() const { return DealWrite(); }   // APPLIED requires the authoritative accounting reconciliation by construction
    VStatus ReplanAfterWrite() const {
        if (!anyWrite_) return plans_ >= kNotRequiredPlans ? VStatus::NotRequired : VStatus::NotCaptured;
        return plansAfterWrite_ > 0 ? VStatus::Pass : VStatus::NotCaptured;
    }
    VStatus PhaseOrdering() const {
        if (phaseViolations_ > 0 || pendingViolations_ > 0) return VStatus::Fail;
        if (executes_ > 0) return VStatus::Pass;
        return plans_ >= kNotRequiredPlans ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus SaveReload() const {
        if (resets_ == 0) return VStatus::NotCaptured;
        if (staleCarried_) return VStatus::Fail;
        return postResetPlans_ > 0 && postResetEpoch_ != lastEpoch_ ? VStatus::Pass : VStatus::NotCaptured;
    }
    VStatus Stability() const {
        // Stability is convergence under equivalent conditions, not inactivity: a city whose demand moves every month legitimately changes funding every month.
        if (flips_ >= 2) return VStatus::Fail;
        // (a transaction in flight does not make the CONVERGENCE evidence disappear; it only keeps the session from being declared complete: Unresolved())
        const bool pre = anyWrite_ ? plansAfterWrite_ >= kStablePlans : plans_ >= kNotRequiredPlans && actPower_ + actWater_ + actSan_ + actDeal_ == 0;     // never needed a write for 20 plans: nothing to oscillate
        const bool post = resets_ == 0 || postResetPlans_ >= kStablePlans;
        return pre && post ? VStatus::Pass : VStatus::NotCaptured;
    }
    VStatus RepeatedActionRefusal() const { return repeatedRefusal_ ? VStatus::Fail : plans_ > 0 ? VStatus::Pass : VStatus::NotCaptured; }
    VStatus DealPingPong() const { return pingPong_ ? VStatus::Fail : plans_ > 0 ? VStatus::Pass : VStatus::NotCaptured; }
    // Never PASS by itself: only a human can see the screen. NOT_CAPTURED until the answer arrives.
    VStatus GraphicsStable() const { return graphics_ > 0 ? VStatus::Pass : graphics_ < 0 ? VStatus::Fail : VStatus::NotCaptured; }
    std::string FailClosed() const {
        if (failClosed_.empty()) return "NOT_TRIGGERED";
        std::string s = "TRIGGERED(";
        for (std::size_t i = 0; i < failClosed_.size(); ++i) s += (i ? "; " : "") + failClosed_[i];
        return s + ")";
    }

    void OnFailedWrite(const std::string& what) {
        failClosed_.push_back(what);
        if (what.find("power") != std::string::npos) failedPower_ = true;
        else if (what.find("water") != std::string::npos) failedWater_ = true;
        else if (what.find("sanitation") != std::string::npos || what.find("garbage") != std::string::npos) sanFailed_ = true;
        else if (what.find("deal") != std::string::npos) { dealFailed_ = true; dealsPending_ = 0; }
    }

    bool AnyFail() const {
        return PhaseOrdering() == VStatus::Fail || SaveReload() == VStatus::Fail || Stability() == VStatus::Fail || unverified_ > 0 || repeatedRefusal_ || pingPong_ || CivicFail();
    }
    // Something that must be resolved before the session may complete.
    bool Unresolved() const { return AnyFail() || !failClosed_.empty() || sanPending_ || dealsPending_ > 0 || staleCarried_; }
    std::string FailureReasons() const {
        std::string r;
        const auto add = [&](const char* w) { r += (r.empty() ? "" : ", "); r += w; };
        if (PhaseOrdering() == VStatus::Fail) add("PHASE_ORDERING=FAIL");
        if (SaveReload() == VStatus::Fail) add("SAVE_RELOAD=FAIL");
        if (Stability() == VStatus::Fail) add("STABILITY=FAIL");
        if (unverified_ > 0) add("UNVERIFIED_WRITE");
        if (repeatedRefusal_) add("REPEATED_ACTION_REFUSAL=FAIL");
        if (pingPong_) add("DEAL_PING_PONG=FAIL");
        if (CivicFail()) add("CIVIC=FAIL");
        if (!failClosed_.empty()) add("FAIL_CLOSED");
        return r;
    }
    // Evidence required BEFORE the save/reload (everything except the reload itself and its stability)
    bool PreReloadEvidenceComplete() const {
        return envKnown_ && Acceptable(PowerPlan()) && Acceptable(PowerWrite()) && Acceptable(WaterPlan()) && Acceptable(WaterWrite()) && Acceptable(SanitationPlan()) &&
               Acceptable(SanitationWrite()) && Acceptable(SanitationPropagation()) && Acceptable(DealPortfolio()) && Acceptable(DealWrite()) && Acceptable(ReplanAfterWrite()) &&
               Acceptable(PhaseOrdering()) && Stability() == VStatus::Pass && CivicAcceptable() && !Unresolved();
    }

    // Emits each milestone once. `lines` receives the text.
    void Milestones(std::vector<std::string>& lines) {
        if (!preEmitted_ && resets_ == 0 && PreReloadEvidenceComplete()) {
            preEmitted_ = true;
            lines.push_back("[FinalValidation] SESSION_PRE_RELOAD_COMPLETE  -> save the city, return to Region View WITHOUT closing SimCity 4, reload the same city, resume the simulation");
        }
        if (!doneEmitted_ && !failEmitted_ && preEmitted_ && resets_ > 0 && SaveReload() == VStatus::Pass && Stability() == VStatus::Pass && !Unresolved()) {
            doneEmitted_ = true;
            lines.push_back("[FinalValidation] SESSION_COMPLETE  -> save if you wish, return to Region View, close SimCity 4 and send the unchanged DepartmentEfficiency.log; "
                            "GRAPHICS_STABLE needs the player's answer: Os graficos ficaram normais durante toda a sessao? SIM/NAO");
        }
        if (!failEmitted_ && !doneEmitted_ && Unresolved() && (AnyFail() || !failClosed_.empty())) {
            failEmitted_ = true;
            lines.push_back("[FinalValidation] SESSION_FAILED (" + FailureReasons() + ")  -> stop here, close SimCity 4 and send the unchanged DepartmentEfficiency.log");
        }
    }

    std::vector<std::string> StatusLines() const {
        std::vector<std::string> out;
        std::ostringstream o;
        o << "[FinalValidation][STATUS] months=" << months_ << " days=" << days_ << " plans=" << plans_ << " resets=" << resets_ << " executes=" << executes_;
        out.push_back(o.str());
        const auto line = [&](const char* name, VStatus s) { out.push_back(std::string("[FinalValidation][STATUS] ") + name + "=" + VStatusName(s)); };
        line("POWER_PLAN", PowerPlan());
        line("POWER_WRITE", PowerWrite());
        line("WATER_PLAN", WaterPlan());
        line("WATER_WRITE", WaterWrite());
        line("SANITATION_PLAN", SanitationPlan());
        line("SANITATION_WRITE", SanitationWrite());
        line("SANITATION_PROPAGATION", SanitationPropagation());
        line("DEAL_PORTFOLIO", DealPortfolio());
        line("DEAL_WRITE", DealWrite());
        line("DEAL_ACCOUNTING", DealAccounting());
        line("REPLAN_AFTER_WRITE", ReplanAfterWrite());
        line("PHASE_ORDERING", PhaseOrdering());
        line("SAVE_RELOAD", SaveReload());
        line("STABILITY", Stability());
        line("EDUCATION_PLAN", EducationPlan());
        line("EDUCATION_WRITE", EducationWrite());
        line("EDUCATION_SERVICE", EducationService());
        line("EDUCATION_STABILITY", EducationStability());
        line("HEALTH_PLAN", HealthPlan());
        line("HEALTH_WRITE", HealthWrite());
        line("HEALTH_SERVICE", HealthService());
        line("HEALTH_STABILITY", HealthStability());
        line("CIVIC_SECONDARY_FUNDING", CivicSecondaryFunding());
        line("REPEATED_ACTION_REFUSAL", RepeatedActionRefusal());
        line("DEAL_PING_PONG", DealPingPong());
        line("GRAPHICS_STABLE", GraphicsStable());
        out.push_back("[FinalValidation][STATUS] FAIL_CLOSED=" + FailClosed());
        std::ostringstream c;
        c << "[FinalValidation][STATUS] counters powerWrites=" << powerWrites_ << " waterWrites=" << waterWrites_ << " sanitationIssued=" << sanIssued_ << " sanitationFundingReadback=" << sanReadback_
          << " sanitationConfirmed=" << sanConfirmed_ << " dealApplied=" << dealApplied_ << " dealCancels=" << dealCancels_ << " quotes=" << quotes_ << " quotesNoWrite=" << quotesNoWrite_
          << " divergences=" << divergences_ << " writesAfterSessionFailed=" << writesAfterFail_ << " quietPlans=" << quiet_ << " phaseViolations=" << phaseViolations_ << " pendingViolations=" << pendingViolations_ << " flips=" << flips_ << " unverifiedWrites=" << unverified_;
        out.push_back(c.str());
        return out;
    }

    int resets() const { return resets_; }
    int plans() const { return plans_; }
    bool preEmitted() const { return preEmitted_; }
    // Terminal failure: every automated write of the validation build must stop. Writes attempted afterwards are counted and fail the tests.
    bool sessionFailed() const { return failEmitted_; }
    int phaseViolations() const { return phaseViolations_; }
    int writesAfterSessionFailed() const { return writesAfterFail_; }
    void OnWriteAttemptedAfterFailure() { ++writesAfterFail_; }
    bool doneEmitted() const { return doneEmitted_; }

private:
    struct Civic {
        int months = 0, facilityMonths = 0, emptyMonths = 0, writeDecisions = 0, applied = 0, failed = 0, serviceChecked = 0, serviceBreach = 0;
        int secondaryChecked = 0, secondaryDrift = 0, quiet = 0, lastDirection = 0, flipRun = 0, flips = 0, postWriteMonths = 0;
    };
    static constexpr int kCivicEmptyMonths = 3;       // evaluated months with no facility of the family: nothing to manage
    static constexpr int kCivicNotRequiredMonths = 4; // evaluated months with facilities and no write decision: no gratuitous writes
    static constexpr int kCivicStableMonths = 3;      // evaluated months after the first verified write (whatever they did): the evidence window of the stability grade
    bool CivicAnyExpected() const { return civicExpected_[0] || civicExpected_[1]; }
    bool CivicOff(int i) const { return !civicKnown_ || !civicExpected_[i]; }
    void CivicFlip(int i, int direction) {
        Civic& c = civic_[i];
        c.quiet = 0;
        if (c.lastDirection != 0 && c.lastDirection != direction) ++c.flipRun; else c.flipRun = 0;
        c.lastDirection = direction;
        if (c.flipRun >= 2 && c.flips < c.flipRun) c.flips = c.flipRun;
    }
    // Stability is judged PER FACILITY. A reversal (raise after cut, or cut after raise) of the same facility is explained when an external input the decision
    // read differs from the one at its previous write (the demand sample or the base capacity); time passing, the decided funding and the capacity that follows
    // from it are not causes. An unexplained or unverifiable reversal counts; two in a row on one facility (up, down, up with unchanged causes) fail.
    void CivicFacilityWrite(int i, const CivicWriteEvent& w) {
        Civic& c = civic_[i];
        c.quiet = 0;
        auto& h = civicFacilities_[std::make_tuple(i, w.cellX, w.cellZ)];
        if (h.direction != 0 && h.direction != w.direction) {
            const bool known = h.usageKnown && w.usageKnown;
            const bool explained = known && (h.usage != w.usage || h.baseCapacity != w.baseCapacity);
            std::ostringstream o;
            o << "[FinalValidation][CivicStability] " << (i == 0 ? "Education" : "Health") << " cell=" << w.cellX << ',' << w.cellZ << " direction=" << (w.direction > 0 ? "UP" : "DOWN")
              << " usage=" << (h.usageKnown ? std::to_string(h.usage) : std::string("UNKNOWN")) << "->" << (w.usageKnown ? std::to_string(w.usage) : std::string("UNKNOWN"))
              << " base=" << h.baseCapacity << "->" << w.baseCapacity << (explained ? " REVERSAL_EXPLAINED" : known ? " REVERSAL_UNEXPLAINED" : " REVERSAL_CAUSE_UNVERIFIABLE");
            diag_.push_back(o.str());
            h.flipRun = explained ? 0 : h.flipRun + 1;
            if (h.flipRun > c.flips) c.flips = h.flipRun;
        } else {
            h.flipRun = 0;
        }
        h.direction = w.direction;
        h.usageKnown = w.usageKnown; h.usage = w.usage; h.baseCapacity = w.baseCapacity;
    }
    struct CivicFacilityHist { int direction = 0, flipRun = 0; bool usageKnown = false; long long usage = 0, baseCapacity = 0; };
    std::map<std::tuple<int, int, int>, CivicFacilityHist> civicFacilities_;
    VStatus CivicPlan(int i) const {
        if (CivicOff(i)) return VStatus::NotRequired;
        const Civic& c = civic_[i];
        if (c.facilityMonths > 0) return VStatus::Pass;
        return c.emptyMonths >= kCivicEmptyMonths ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus CivicWrite(int i) const {
        if (CivicOff(i)) return VStatus::NotRequired;
        const Civic& c = civic_[i];
        if (c.failed > 0) return VStatus::Fail;
        if (c.applied > 0) return VStatus::Pass;
        if (c.facilityMonths == 0) return c.emptyMonths >= kCivicEmptyMonths ? VStatus::NotRequired : VStatus::NotCaptured;
        return c.writeDecisions == 0 && c.facilityMonths >= kCivicNotRequiredMonths ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus CivicService(int i) const {
        if (CivicOff(i)) return VStatus::NotRequired;
        const Civic& c = civic_[i];
        if (c.serviceBreach > 0) return VStatus::Fail;
        if (c.serviceChecked > 0) return VStatus::Pass;
        return CivicWrite(i) == VStatus::NotRequired ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    VStatus CivicStability(int i) const {
        if (CivicOff(i)) return VStatus::NotRequired;
        const Civic& c = civic_[i];
        if (c.flips >= 2) return VStatus::Fail;
        if (c.facilityMonths == 0) return c.emptyMonths >= kCivicEmptyMonths ? VStatus::NotRequired : VStatus::NotCaptured;
        if (c.applied > 0) return c.postWriteMonths >= kCivicStableMonths ? VStatus::Pass : VStatus::NotCaptured;     // changes justified by the inputs are fine every month
        return c.writeDecisions == 0 && c.facilityMonths >= kCivicNotRequiredMonths ? VStatus::Pass : VStatus::NotCaptured;
    }
    bool civicExpected_[2] = {false, false}, civicKnown_ = false;
    Civic civic_[2];

    static bool NeverNeeded(bool present, int plansSeen, int plansWithAction) {
        return present && plansSeen >= kNotRequiredPlans && plansWithAction == 0;
    }
    VStatus Write(bool present, int plansSeen, int plansWithAction, int writes, bool failed) const {
        if (failed) return VStatus::FailClosed;
        if (writes > 0) return VStatus::Pass;
        if (!present && envKnown_) return VStatus::NotRequired;
        return NeverNeeded(present, plansSeen, plansWithAction) ? VStatus::NotRequired : VStatus::NotCaptured;
    }
    void Wrote() {
        anyWrite_ = true;
        plansSinceWrite_ = monthsSinceWrite_ = 0;
        postResetPlansSinceWrite_ = postResetMonthsSinceWrite_ = 0;
    }
    // Ping-pong detector: the same control moved in alternating directions (up, down, up) = flip-flop.
    // The rule is "same causal state -> same optimum": a reversal is a flip only when NOTHING the planner reads changed between the two
    // writes (equal non-empty fingerprints) or when that cannot be told (a fingerprint is unknown: conservative, counted). A reversal whose
    // exogenous inputs differ (the calendar moved, the planned export relief changed, ...) is the optimiser adapting, not oscillating; it
    // resets the run. No cooldown, no hysteresis, no tolerance: an equal-input A -> B -> A still fails.
    // Per control: `key` separates the plants of one commodity (another plant moving the other way is not a reversal of this one).
    void Flip(int which, int direction, const std::string& causal = std::string(), const std::string& key = std::string()) {
        if (direction == 0) return;
        FlipState& f = flip_[std::make_pair(which, key)];
        const bool reversed = f.direction != 0 && f.direction != direction;
        const bool explained = reversed && !causal.empty() && !f.causal.empty() && causal != f.causal;
        if (reversed && !explained) ++f.run; else f.run = 0;
        f.direction = direction;
        f.causal = causal;
        if (f.run >= 2 && flips_ < f.run) flips_ = f.run;
    }
    struct FlipState { int direction = 0, run = 0; std::string causal; };
    std::map<std::pair<int, std::string>, FlipState> flip_;

    bool power_ = true, water_ = true, sanitation_ = true, envKnown_ = false;
    int plans_ = 0, plansPower_ = 0, plansWater_ = 0, plansSan_ = 0, plansPortfolio_ = 0;
    int actPower_ = 0, actWater_ = 0, actSan_ = 0, actDeal_ = 0;
    int powerWrites_ = 0, waterWrites_ = 0, sanIssued_ = 0, sanReadback_ = 0, sanConfirmed_ = 0, dealApplied_ = 0, dealCancels_ = 0;
    int quotes_ = 0, quotesNoWrite_ = 0, divergences_ = 0;
    int executes_ = 0, phaseViolations_ = 0, pendingViolations_ = 0, unverified_ = 0;
    int months_ = 0, days_ = 0, plansSinceWrite_ = 0, monthsSinceWrite_ = 0, plansAfterWrite_ = 0;
    int resets_ = 0, postResetPlans_ = 0, postResetMonths_ = 0, postResetPlansSinceWrite_ = 0, postResetMonthsSinceWrite_ = 0;
    std::uint32_t lastEpoch_ = 0, postResetEpoch_ = 0;
    bool anyWrite_ = false, sanPending_ = false, staleCarried_ = false;
    int dealsPending_ = 0;      // deal transactions in flight (one per commodity at most)
    int quiet_ = 0, postResetQuiet_ = 0, graphics_ = 0;
    bool repeatedRefusal_ = false, pingPong_ = false;
    struct Refusal { int op = -1; std::int32_t target = -1; int count = 0; };
    std::map<std::tuple<std::int32_t, std::uint32_t>, Refusal> refusals_;
    struct DealHist { std::int32_t amount = 0; DealCausal causal; };
    std::map<std::tuple<std::int32_t, std::uint32_t>, std::vector<DealHist>> history_;
    std::tuple<std::int32_t, std::uint32_t> lastSlot_{0, 0};
    std::vector<std::string> diag_;
    bool dumped_ = false;
    bool failedPower_ = false, failedWater_ = false, sanFailed_ = false, dealFailed_ = false;
    int flips_ = 0;
    bool preEmitted_ = false, doneEmitted_ = false, failEmitted_ = false;
    int writesAfterFail_ = 0;
    std::vector<std::string> failClosed_;
};

} // namespace sc4af
