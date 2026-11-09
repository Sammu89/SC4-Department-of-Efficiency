// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Unified Auto controller: Power, Water, Sanitation and Neighbor Deals as ONE control loop.
//
//   READ -> PLAN WHOLE SYSTEM -> ordered per-commodity actions -> FRESH REVALIDATION -> EXECUTE ONE SAFE PHASE -> OBSERVE / VERIFY
//        -> REPLAN FROM AUTHORITATIVE STATE (next cycle)
//
// * One planning pass (UnifiedPlan.h) feeds Shadow and this controller; the controller never executes an old plan. Every cycle plans again
//   from a fresh snapshot and executes at most the LOWEST pending phase of each commodity (DealPortfolio.h phase order, which keeps every
//   intermediate state safe). A deal transaction is a state machine pumped by later snapshots: while one is in flight nothing
//   else touches its commodity and no second deal starts. A Sanitation transaction likewise blocks Garbage funding and Garbage deals until
//   CAPACITY_READBACK_CONFIRMED. No wall clock anywhere.
// * Constructing it needs UnifiedAutoValidationCapability, which can only be created when the unified controller is compiled in (UnifiedAutoCapability.h).
// * Failure isolation: a Deal technical failure disables Deal Auto for the city session; a Sanitation technical failure disables Garbage
//   Auto (funding and Garbage deals); Power / Water failures use the per-subsystem fail-closed handling. Local utilities then replan with
//   the active-contract accounting (no speculative deals).
// * Observability: every plan has an ID, every execution names its plan and phase, every deal write logs the quote chain,
//   Sanitation logs its stages, and FinalValidation.h turns these events into STATUS / milestone lines.

#include "SanitationPacing.h"
#include "FinalValidation.h"
#include "LandfillAdvisor.h"
#include "LandfillCollector.h"
#include "UnifiedDiagnostics.h"
#include "SafeMoney.h"
#include "ServiceMonitor.h"
#include "ServiceNotices.h"
#include "ServiceReadiness.h"
#include "SanitationWriterPlan.h"
#include "UnifiedAutoCapability.h"
#include "UnifiedPlan.h"
#include "UtilityAuto.h"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <optional>
#include <iomanip>
#include <sstream>
#include <set>
#include <map>
#include <string>
#include <vector>

namespace sc4af {

inline bool UnifiedGateOpen(const UtilityResearchConfig& c) {
    return UnifiedAutoValidationCapability::kBuildEnabled && UnifiedGateOpenRuntime(c);
}

// Phase numbers in logs are 1-based: 1 CAPACITY_RAISE, 2 SELL_DOWN, 3 BUY_UP, 4 SELL_UP, 5 CAPACITY_CUT, 6 BUY_DOWN.
inline const char* UnifiedPhaseName(int phase) {
    static const char* const names[] = {"CAPACITY_RAISE", "SELL_DOWN", "BUY_UP", "SELL_UP", "CAPACITY_CUT", "BUY_DOWN"};
    return phase >= 0 && phase < 6 ? names[phase] : "?";
}

// ---- ledger: planned, pending and realized effects are never mixed -------------------------------------------------------------------
struct UnifiedLedger {
    std::int64_t realizedPower = 0, realizedWater = 0, realizedSanitation = 0;   // VERIFIED monthly run-rate saving (positive = cheaper)
    std::int64_t realizedDealNet = 0;       // VERIFIED change of the simulator's monthly (income - expense) after completed deal transactions
    std::int64_t pending = 0;               // planned effect of transactions in flight, not verified yet
    std::int64_t shadowPotential = 0;       // value of the current plan that has not been attempted
    int fundingWrites = 0, dealTransactions = 0, sanitationTransactions = 0;
    std::int64_t realizedLocalFunding() const { return SatAdd(SatAdd(realizedPower, realizedWater), realizedSanitation); }
    std::int64_t realizedNet() const { return SatAdd(realizedLocalFunding(), realizedDealNet); }
    std::string Format() const {
        std::ostringstream o;
        o << "[UnifiedAuto][Ledger] REALIZED localFunding=" << realizedLocalFunding() << " (power=" << realizedPower << " water=" << realizedWater << " sanitation=" << realizedSanitation
          << ") REALIZED dealIncomeMinusExpense=" << realizedDealNet << " REALIZED net=" << realizedNet() << " | PENDING(unverified)=" << pending
          << " | SHADOW/POTENTIAL(not attempted)=" << shadowPotential << " | funding writes=" << fundingWrites << " deal transactions=" << dealTransactions
          << " sanitation transactions=" << sanitationTransactions;
        return o.str();
    }
};

// ---- game-facing seam ---------------------------------------------------------------------------------------------------------------
class IUnifiedActuator : public IUtilityActuator {
public:
    // Fresh authoritative snapshot WITH the deal market (ProposeDeals refreshed the commodity cache).
    virtual bool ReadForPlan(UtilitySnapshot& out) = 0;
    // The light read of the fast service check (a handful of authoritative counters; no enumeration, no deal market). false = not available.
    virtual bool ReadServiceCounters(ServiceCounters& out) { (void)out; return false; }
    // Vanilla UpdateCommodityStats on the local city (read-side refresh of the cache GetDealCost / GetDealMaxAmount read).
    virtual bool RefreshCommodityStats() = 0;
    // GetDealCost(type, amount, neighbor) right now, truncated.
    virtual Opt<std::int64_t> Quote(const DealKey& key, std::int64_t amount) = 0;
    // Observe -> machine.Next -> execute AT MOST ONE primitive. Returns the step (primitive None = nothing executed).
    virtual DealStep PumpDeal(DealWriteMachine& machine, std::uint32_t lifecycleEpoch) = 0;
    virtual SanitationStep PumpSanitation(SanitationWriteMachine& machine, std::uint32_t lifecycleEpoch) = 0;
};

class UnifiedAutoController {
public:
    explicit UnifiedAutoController(const UnifiedAutoValidationCapability&) {}

    // City (re)loaded or unloaded: nothing survives into another lifecycle epoch (the validation tracker is the exception: it must see the reload).
    void ResetCity() {
        legacy_.ResetCity();
        days_ = months_ = 0;
        garbageOff_ = deals_off_ = Off{};
        for (auto& d : pendingDeals_) d.reset();
        pendingSan_.reset();
        monitor_.Reset();
        episodes_.Reset();
        contractTable_ = "none";
        contractTableLogged_ = false;
        dormantGiveUp_.clear();
        refusedNew_.clear();
        haveSanWrite_ = pacedLogged_ = false;
        lastSanWriteDay_ = lastPacedLogDay_ = 0;
        createdHere_.clear();
        auditSig_.clear();
        servedOk_[0] = servedOk_[1] = false;
        internalLogged_[0] = internalLogged_[1] = false;
        lastCounters_ = ServiceCounters{};
        ledger_ = UnifiedLedger{};
        lastConfig_.clear();
        lastPlanText_.clear();
        lines_.clear();
        envLogged_ = false;
        lastPowerOff_ = lastWaterOff_ = false;
        powerEvaluated_ = waterEvaluated_ = false;               // nothing of the previous city says this one was already evaluated
        lastWaterDay_ = 0;
        for (auto& r : ready_) r = ReadinessState{};
        landfill_.Reset();                                   // the geometry is calibrated per city lifecycle and never carried over
        tracker_.OnReset();
    }

    // Normal snapshot fields only (capacity, usage, published Landfilled): the landfill geometry is learnt from the month boundaries the game shows.
    static LandfillReading LandfillReadingOf(const UtilitySnapshot& s) {
        LandfillReading r;
        const auto& a = s.garbage.aggregate;
        if (a.landfillCapacity) r.capacity = *a.landfillCapacity;
        if (a.landfillUsage) r.usage = *a.landfillUsage;
        if (a.toLandfill) r.published = *a.toLandfill;
        return r;
    }
    void ObserveLandfillBaseline(const UtilitySnapshot& s) { landfill_.OnBaseline(LandfillReadingOf(s)); }
    void ObserveLandfillMonth(const UtilitySnapshot& s) { landfill_.OnMonthEvent(LandfillReadingOf(s)); }
    const LandfillGeometryTracker& landfill() const { return landfill_; }
    // Landfill Size Savings Advisor (LandfillAdvisor.h). Its per-city record is NOT part of ResetCity (leaving the gate must not reset any period): the plugin clears / restores it per city.
    LandfillAdvisorEngine& landfillAdvisor() { return landfillAdvisor_; }
    LandfillGhostEngine& landfillGhosts() { return landfillGhosts_; }
    // Set by the plugin (game access): the spatial read of the landfill zones and the native dry run of a placement. Empty = no spatial evidence (no landfill advice, no repair).
    LandfillSnapshotCache::ReadFn landfillSpatialRead;
    LandfillNativeCheckFn landfillNativeCheck;

    void OnDay(const UtilityResearchConfig& cfg, IUnifiedActuator& act) {
        lines_.clear();
        ++days_;
        if (!Gate(cfg)) return;
        tracker_.OnDay();
        const bool powerDay = cfg.powerCadenceDays > 0 && days_ % static_cast<std::uint32_t>(cfg.powerCadenceDays) == 0;      // the full review of the electricity, every PowerCadenceDays days
        if (tracker_.sessionFailed()) return;                         // terminal failure: no further validation writes (nor new plans)
        // FAST SERVICE CHECK: a light read of the authoritative counters. A confirmed regional deficit or a material change of capacity / contracts / demand asks for an immediate replan of that
        // commodity (the first safe phase of the next cycle) instead of waiting for the cadence; a counter identical to the previous sample is not a new observation and decides nothing.
        bool urgentPower = false, urgentWater = false;
        bool powerFresh = true, waterFresh = true;                      // the light sample is a NEW observation (an identical one is stale and decides nothing)
        ServiceCounters sc;
        bool haveCounters = false;
        // A commodity is evaluated when ITS funding manager OR its contract permission (AutoDeals master + AutoTrade*) wants it: the two are independent. What may be WRITTEN
        // is decided separately, per kind of write (Allow::manage for the funding, Allow::trade for the contracts).
        const bool wantPower = cfg.autoPower || cfg.tradeActive(0), wantWater = cfg.autoWater || cfg.tradeActive(1), wantGarbage = cfg.autoGarbage || cfg.tradeActive(2);
        bool garbageDue = wantGarbage;                                  // without counters Garbage is evaluated every day
        if (wantPower || wantWater || wantGarbage) {
            haveCounters = act.ReadServiceCounters(sc);
            if (haveCounters) {
                ++serviceChecks_;
                lastCounters_ = sc;
                ServiceProof proof;
                for (const bool power : {true, false})
                    if (monitor_.CheckProof(sc, power, proof)) Info(std::string("[ServiceCheck][Proof] ") + proof.text);
                if (wantPower) {
                    const auto v = monitor_.ObservePower(sc);
                    urgentPower = v.trigger;
                    powerFresh = v.cause != ServiceCause::Stale;
                    NoteServiceSample(0, v, sc.pDemand, sc.pConsumption);
                }
                if (wantWater) {
                    const auto v = monitor_.ObserveWater(sc);
                    urgentWater = v.trigger;
                    waterFresh = v.cause != ServiceCause::Stale;
                    NoteServiceSample(1, v, sc.wDemand, sc.wConsumed);
                }
                if (wantGarbage && sc.garbage) {
                    // Garbage is planned when the counters behind its plan moved (or something is in flight); an unchanged sample does not re-run the optimiser every day
                    const auto g = monitor_.ObserveGarbage(sc);
                    garbageDue = g.trigger;
                    if (g.trigger) Info(std::string("[ServiceCheck] ") + g.text + " -> garbage replan");
                }
            }
        }
        Allow allow = BaseAllow(cfg);
        // The readings decide, not a timer. Until a commodity has been evaluated once, it is tried on every NEW sample of its counters; the light counters only SPARE a full plan when they already prove
        // the commodity is not ready (demand reads 0, counts not populated, contradictory), the full snapshot gives the verdict (ServiceReadiness.h).
        const bool powerCountersOk = !(haveCounters && PowerCountersShowNotReady(sc));
        const bool waterCountersOk = !(haveCounters && WaterCountersShowNotReady(sc));
        const bool powerFirstTry = !powerEvaluated_ && powerFresh;
        // The full periodic review of the water, every WaterCadenceDays simulated days since its LAST evaluation (periodic, urgent or monthly: one evaluation serves them all, so a periodic
        // review never repeats a full plan that an urgent one has just made); independent of the power cadence.
        const bool waterPeriodic = cfg.waterCadenceDays > 0 && days_ - lastWaterDay_ >= static_cast<std::uint32_t>(cfg.waterCadenceDays);
        const bool waterFirstTry = !waterEvaluated_ && waterFresh;
        allow.power = wantPower && powerCountersOk && (powerDay || powerFirstTry || urgentPower);
        allow.water = wantWater && waterCountersOk && (waterPeriodic || waterFirstTry || urgentWater);
        allow.garbage = wantGarbage && garbageDue;
        const bool inFlight = AnyDealPending() || pendingSan_;
        if (!inFlight && !allow.power && !allow.water && !allow.garbage) return;       // nothing is due today
        if (!allow.power && !allow.water && !allow.garbage) {
            // Only transactions in flight, nothing due: they are advanced with fresh observations and that is all. The whole-system read and plan cost ~230 ms in a big city and a Sanitation write waits
            // days for the game to show its efficiency: a full cycle every day just to wait would slow the game down.
            PumpInFlight(act, allow);
            EmitMilestones(false);
            return;
        }
        RunCycle(cfg, act, allow);
        if (allow.power && ready_[0].ok) powerEvaluated_ = true;
        if (allow.water && ready_[1].ok) { waterEvaluated_ = true; lastWaterDay_ = days_; }
        if (haveCounters) {
            if (allow.power) monitor_.NoteEvaluated(sc, true);
            if (allow.water) monitor_.NoteEvaluated(sc, false);
            if (allow.garbage && sc.garbage) monitor_.NoteEvaluatedGarbage(sc);
        }
        EmitMilestones(false);
    }

    void OnMonth(const UtilityResearchConfig& cfg, IUnifiedActuator& act) {
        lines_.clear();
        for (auto& l : landfill_.TakeLogs()) Info(std::move(l));
        { const auto diag = tracker_.TakeDiagnostics(); if (validation_) for (const auto& l : diag) Info(l); }     // Civic per-facility reversals of the previous evidence
        ++months_;
        legacy_.CountMonth();
        if (!Gate(cfg)) return;
        tracker_.OnMonth();
        if (tracker_.sessionFailed()) return;                         // terminal failure: no further validation writes (nor new plans)
        Allow allow = BaseAllow(cfg);
        allow.water = cfg.autoWater || cfg.tradeActive(1);
        allow.garbage = cfg.autoGarbage || cfg.tradeActive(2);
        RunCycle(cfg, act, allow);
        if (allow.water && ready_[1].ok) { waterEvaluated_ = true; lastWaterDay_ = days_; }
        {
            ServiceCounters sc;
            if (act.ReadServiceCounters(sc)) { lastCounters_ = sc; if (allow.water) monitor_.NoteEvaluated(sc, false); if (allow.garbage && sc.garbage) monitor_.NoteEvaluatedGarbage(sc); }
        }
        EmitMilestones(true);
    }

    const std::vector<AutoLine>& lines() const { return lines_; }
    const UnifiedLedger& ledger() const { return ledger_; }
    const FinalValidationTracker& tracker() const { return tracker_; }
    // The separate civic controller (Education, Health): its monthly evidence feeds the tracker.
    void SetCivicExpected(bool education, bool health) { tracker_.SetCivicExpected(education, health); }
    void OnCivicMonth(const CivicMonthEvidence& e) { tracker_.OnCivicMonth(e); }     // its stability diagnostics are logged with the next OnMonth
    bool sessionFailed() const { return tracker_.sessionFailed(); }
    // The service-insufficiency episode of commodity `i` (0 power, 1 water, 2 garbage) has been proven and not yet closed by a proven recovery (the player notice is deferred on it).
    // The readings of commodity i (0 power, 1 water, 2 garbage) and of the contracts are present and consistent in the latest snapshot: otherwise the controller neither writes nor trades for it.
    bool commodityReady(int i) const { return ready_[i].ok; }
    bool serviceEpisodeActive(int i) const { return episodes_.active(i); }
    bool powerDisabled() const { return legacy_.powerDisabled(); }
    bool waterDisabled() const { return legacy_.waterDisabled(); }
    bool garbageDisabled() const { return garbageOff_.disabled; }
    bool dealsDisabled() const { return deals_off_.disabled; }
    const std::string& garbageReason() const { return garbageOff_.reason; }
    const std::string& dealsReason() const { return deals_off_.reason; }
    bool dealPending() const { return AnyDealPending(); }
    const std::string& contractTable() const { return contractTable_; }       // the committed contracts at the last plan (logged at shutdown: save / reload proof)
    std::size_t serviceChecks() const { return serviceChecks_; }      // light counter reads done by the fast service check
    bool sanitationPending() const { return static_cast<bool>(pendingSan_); }
    const std::string& lastPlanText() const { return lastPlanText_; }
    std::uint32_t planId() const { return planId_; }

private:
    struct Off {
        bool disabled = false;
        std::string reason;
    };
    struct Allow {
        bool power = false, water = false, garbage = false, deals = false;   // what may be EVALUATED in this cycle (power / water / garbage are due-ness, wanted by the manager OR the contract permission; deals is AutoDeals)
        bool manage[3] = {false, false, false};                                // the FUNDING manager of each commodity is on (AutoPower / AutoWater / AutoGarbage): may write the funding
        bool trade[3] = {false, false, false};                                 // the CONTRACTS of each commodity may be managed, in both directions (AutoDeals master AND AutoTradePower / AutoTradeWater / AutoTradeGarbage)
    };
    // The permissions of this moment, from the configuration as it is NOW (a transaction in flight is judged against these, never against the ones it was started under).
    static Allow BaseAllow(const UtilityResearchConfig& cfg) {
        Allow a;
        a.deals = cfg.autoDeals;
        a.manage[0] = cfg.autoPower; a.manage[1] = cfg.autoWater; a.manage[2] = cfg.autoGarbage;
        for (int i = 0; i < 3; ++i) a.trade[i] = cfg.tradeActive(i);
        return a;
    }
    // May the transaction in flight still issue a primitive? Only while the master (AutoDeals) AND this commodity's contract permission are on, judged now, whatever direction or operation the transaction
    // has (create, raise, lower, cancel: all of them are interventions in the contracts of this commodity). The commodity's FUNDING manager has no say in it.
    bool DealMayWrite(int ci, const Allow& allow) const { return allow.deals && allow.trade[ci]; }
    struct PendingDeal {
        std::unique_ptr<DealWriteMachine> machine;
        PortfolioAction action;
        Commodity commodity = Commodity::None;
        std::uint32_t planId = 0;
        std::int64_t netBefore = 0;
        bool netKnown = false;
        DealCausal causal;              // the exogenous state this write was planned from (deal stability gate)
        std::string preRow;             // [DealTx] fields of the row before the write
        std::string preRegional;        // regional counters + simulator money before the write
    };
    struct PendingSan {
        std::unique_ptr<SanitationWriteMachine> machine;
        GarbageFundingWritePlan plan;
        std::uint32_t planId = 0;
        std::int64_t expenseBefore = 0;
        bool expenseKnown = false;
        bool issuedLogged = false, readbackLogged = false, pendingLogged = false;      // each of the three is logged ONCE per transaction (the 1 s poll must not repeat a line while it waits)
        int direction = 0;
        std::string causal;     // fingerprint of the exogenous inputs this target was planned from (stability gate)
    };

    void Info(std::string t) { lines_.push_back({0, std::move(t)}); }
    void Warn(std::string t) { lines_.push_back({1, std::move(t)}); }
    void Err(std::string t) { lines_.push_back({2, std::move(t)}); }
    void Drain() {
        for (auto& l : legacy_.TakeLines()) lines_.push_back(std::move(l));
    }

    bool Gate(const UtilityResearchConfig& cfg) {
        validation_ = cfg.unifiedAutoValidation;                       // the grading overlay: milestones, environment / scout lines
        verbose_ = cfg.detailedLog || cfg.unifiedAutoValidation;      // the full per-plan decision dump
        if (!UnifiedGateOpen(cfg)) {
            if (!lastConfig_.empty() || garbageOff_.disabled || deals_off_.disabled || AnyDealPending() || pendingSan_) ResetCity();   // leaving the gate is an explicit reset
            return false;
        }
        std::ostringstream o;
        o << "config UnifiedAuto Mode=Auto AutoPower=" << cfg.autoPower << " AutoWater=" << cfg.autoWater << " AutoGarbage=" << cfg.autoGarbage << " AutoDeals=" << cfg.autoDeals
          << " DealDiscovery=" << (DealDiscoveryAllowed(cfg) ? "on" : "off(existing contracts only; ProposeDeals not called)")
          << ' ' << PolicyText(cfg) << " PowerCadenceDays=" << cfg.powerCadenceDays << " WaterCadenceDays=" << cfg.waterCadenceDays;
        if (o.str() != lastConfig_) { Info("[UnifiedAuto] " + o.str()); lastConfig_ = o.str(); }
        return true;                                                   // no fixed wait; each commodity acts when ITS readings are ready (ServiceReadiness.h, checked in every cycle)
    }

    // The phase of a commodity that may run now: the lowest one, EXCEPT a Sanitation funding CUT that the pacing (SanitationPacing.h) defers: keeping the funding above its
    // target only adds capacity, so the later Garbage phase (buy-down: lower / cancel an EXPORT) is safe meanwhile. Otherwise a small cut waiting out its delay would block
    // the cancellation of an export plan after plan, and a raise at the start of the next month would restart the wait. A deferred RAISE still
    // blocks everything (service first). Every caller (the cycle, the deal re-check after the quote refresh, the phase-order tracker) uses this same definition.
    int ExecutablePhase(const UnifiedPlan& p, Commodity c) const {
        const int lowest = LowestPhase(p, c);
        if (c != Commodity::Garbage || lowest != kPhaseCapacityCut) return lowest;
        for (const auto& a : p.actions) {
            if (a.kind != UnifiedActionKind::SanitationFunding || a.phase != kPhaseCapacityCut) continue;
            if (PaceSanitationWrite(a.sanitation.expectedPercent, a.sanitation.targetPercent, haveSanWrite_, days_ - lastSanWriteDay_).allowed) return lowest;
        }
        int next = -1;
        for (const auto& a : p.actions)
            if (a.commodity == c && a.phase > kPhaseCapacityCut && (next < 0 || a.phase < next)) next = a.phase;
        return next < 0 ? lowest : next;
    }

    // A commodity whose funding writer failed its verification keeps its contracts: its funding is frozen in the plans exactly as when its manager is off, so no plan relies on a funding change that will not run.
    bool FundingDisabled(Commodity c) const {
        return (c == Commodity::Power && legacy_.powerDisabled()) || (c == Commodity::Water && legacy_.waterDisabled()) || (c == Commodity::Garbage && garbageOff_.disabled);
    }
    OptimizerConfig UnifiedOptimizerConfig(const UtilityResearchConfig& cfg) const {
        return MakeUnifiedOptimizerConfig(cfg, legacy_.powerDisabled(), legacy_.waterDisabled(), garbageOff_.disabled);
    }

    bool CommodityBlocked(Commodity c) const {
        if (pendingDeals_[CommodityIndex(c)]) return true;       // this commodity's contract is untouched until its accounting reconciles (the other commodities are independent)
        if (c == Commodity::Garbage && pendingSan_) return true;
        return false;
    }

    static std::int64_t NetOf(const UtilitySnapshot& s, bool& known) {
        known = s.deals.monthlyIncome && s.deals.monthlyExpense;
        return known ? *s.deals.monthlyIncome - *s.deals.monthlyExpense : 0;
    }

    static std::string MarginalText(const PortfolioAction& a) {
        return a.marginalKnown ? (a.cashMarginal == a.marginal ? std::to_string(a.marginal) : "cash " + std::to_string(a.cashMarginal) + " economic " + std::to_string(a.marginal)) : std::string("N/A(") + (a.kind == PortfolioActionKind::Cancel || a.kind == PortfolioActionKind::Decrease ? "RESTORES_FEASIBILITY" : "REQUIRED_FOR_FEASIBILITY") + ")";
    }

    static std::string DealText(const DealKey& k) { return std::string(DealTypeName(k.type)) + " neighbor " + std::to_string(k.neighbor); }

    void EmitMilestones(bool month) {
        if (validation_) {
            std::vector<std::string> ms;
            tracker_.Milestones(ms);
            if (month || !ms.empty()) for (const auto& l : tracker_.StatusLines()) Info(l);
            for (const auto& l : ms) Info(l);
        }
        if (tracker_.sessionFailed()) for (const auto& l : tracker_.FailureDump(context_)) Info(l);   // from the state already read: no write
    }

    // ---- environment: what this city can exercise, logged once per loaded city -------------------------------------------------------
    void LogEnvironment(const UtilitySnapshot& snap, const UnifiedPlan& plan) {
        envLogged_ = true;
        const bool power = !snap.power.plants.empty() && (snap.power.validity == SnapshotValidity::Valid || snap.power.validity == SnapshotValidity::Partial);
        const bool water = snap.water.validity == SnapshotValidity::Valid || snap.water.validity == SnapshotValidity::Partial;
        const bool sanitation = snap.garbage.department.present;
        const bool load = snap.garbage.aggregate.produced && *snap.garbage.aggregate.produced > 0;
        const bool landfill = snap.garbage.aggregate.landfillTiles && *snap.garbage.aggregate.landfillTiles > 0;
        const bool cP = snap.deals.connectionPower && *snap.deals.connectionPower;
        const bool cW = snap.deals.connectionWater && *snap.deals.connectionWater;
        const bool cG = snap.deals.connectionGarbage && *snap.deals.connectionGarbage;
        int latent = 0;
        for (const auto& c : plan.portfolio.commodities) if (c.hasLatent()) ++latent;
        std::vector<std::string> missing;
        if (!power) missing.push_back("Power");
        if (!water) missing.push_back("Water");
        if (!sanitation) missing.push_back("Sanitation");
        if (!load) missing.push_back("GarbageLoad");
        if (!landfill) missing.push_back("Landfill");
        if (!cP && !cW && !cG) missing.push_back("NeighborConnection");
        const auto yn = [](bool b) { return b ? "YES" : "NO"; };
        std::ostringstream o;
        o << "[FinalValidation][Environment] Power=" << yn(power) << " Water=" << yn(water) << " Sanitation=" << yn(sanitation) << " GarbageLoad=" << yn(load) << " Landfill=" << yn(landfill)
          << " PowerNeighborConnection=" << yn(cP) << " WaterNeighborConnection=" << yn(cW) << " GarbageNeighborConnection=" << yn(cG) << " VisibleDeals=" << snap.deals.opportunities.size()
          << " EligibleLatentDeals=" << latent << " UnifiedAuto=ARMED SESSION_SUITABLE=" << (missing.empty() ? "FULL" : "PARTIAL");
        Info(o.str());
        {   // the city scout: what this city can exercise, shown to the player as a native message (Plugin: CueForLogLine)
            bool offBuyPower = false, offSellPower = false, offWaterBuy = false, offGarbageExport = false;
            for (const auto& op : snap.deals.opportunities) {
                offBuyPower = offBuyPower || op.type == utility_id::kDealBuyPower;
                offSellPower = offSellPower || op.type == utility_id::kDealSellPower;
                offWaterBuy = offWaterBuy || op.type == utility_id::kDealBuyWater;
                offGarbageExport = offGarbageExport || op.type == utility_id::kDealExportGarbage;
            }
            const auto& pa = snap.power.aggregate;
            const bool powerShort = pa.production && pa.requirement && *pa.production < *pa.requirement;     // the city produces less than it needs: the case in which BUY_POWER is useful
            Info(std::string("[FinalValidation][Scout] buyPower=") + yn(offBuyPower) + " powerDeficit=" + yn(powerShort) + " sellPower=" + yn(offSellPower) + " waterBuy=" + yn(offWaterBuy) +
                 " garbageExport=" + yn(offGarbageExport) + " missingServices=" + yn(!power || !water || !landfill));
        }
        if (!missing.empty()) {
            std::string m;
            for (std::size_t i = 0; i < missing.size(); ++i) m += (i ? "," : "") + missing[i];
            const bool onlyWater = missing.size() == 1 && missing[0] == "Water";
            Info("[FinalValidation] CITY_PARTIAL Missing=" + m + (onlyWater ? "  -> CONTINUE with this city: the Water gates read NOT_REQUIRED here; every other gate can still be validated"
                                                                          : "  -> this city cannot exercise the missing parts: return to Region View WITHOUT closing SimCity 4 and load a disposable city that has them"));
        }
        tracker_.SetEnvironment(power, water, sanitation);
    }

    // The transactions in flight (a Sanitation write, a deal) advanced by fresh observations. No plan is made here.
    void PumpInFlight(IUnifiedActuator& act, const Allow& allow) {
        if (pendingSan_) PumpSanitationTx(act, allow.manage[2]);                                    // Garbage switched off mid-transaction: observations only, no new write (see PumpSanitationTx)
        for (int ci = 0; ci < 3; ++ci) if (pendingDeals_[ci]) PumpDealTx(ci, act, DealMayWrite(ci, allow));   // AutoDeals or the commodity's contract permission switched off mid-session: observations only, no primitive (see PumpDealTx)
        NoteDisabledTransitions();
    }

    // ---- one cycle ------------------------------------------------------------------------------------------------------------------
    void RunCycle(const UtilityResearchConfig& cfg, IUnifiedActuator& act, const Allow& allow) {
        const bool carried = AnyDealPending() || pendingSan_;
        // 1. transactions already in flight are advanced by FRESH observations first
        PumpInFlight(act, allow);
        // a clean instant: the in-flight transactions have just been concluded, nothing new has been started. A city that renegotiates a contract in nearly every plan never has one at the END of
        // a cycle, so the session milestones are also evaluated here.
        if (!AnyDealPending() && !pendingSan_) EmitMilestones(false);

        // 2. fresh authoritative snapshot, whole-system plan
        UtilitySnapshot snap;
        const auto tRead = std::chrono::steady_clock::now();
        if (!act.ReadForPlan(snap) || snap.lifecycleEpoch == 0) { Warn("[UnifiedAuto] fresh snapshot unavailable: zero writes"); return; }
        const auto tPlan = std::chrono::steady_clock::now();
        const auto plans0 = gGarbagePlanCalls, evals0 = gGarbageEvalCalls;
        gPlanWork = PlanWork{};
        gPortfolioWork = PortfolioWork{};
        snap.garbage.geometry = landfill_.View();
        epoch_ = snap.lifecycleEpoch;
        const OptimizerConfig opt = UnifiedOptimizerConfig(cfg);
        const UtilityTuning tuning = VanillaUtilityTuning();
        for (int i = 0; i < 3; ++i) buyAllowed_[i] = cfg.tradeActive(i);     // for the wording of a shortage notice
        const UnifiedPlan plan = PlanFor(snap, cfg, opt, tuning);
        const auto tDone = std::chrono::steady_clock::now();
        const auto ms = [](auto a, auto b) { return std::chrono::duration_cast<std::chrono::microseconds>(b - a).count() / 1000.0; };
        workText_ = " readMs=" + FormatMs(ms(tRead, tPlan)) + " planMs=" + FormatMs(ms(tPlan, tDone)) + " garbagePlans=" + std::to_string(gGarbagePlanCalls - plans0) +
                    " garbageEvals=" + std::to_string(gGarbageEvalCalls - evals0) + " passes=" + std::to_string(gPlanWork.passes) + " portfolioMs=" + FormatMs(gPlanWork.portfolioUs / 1000) +
                    " powerMs=" + FormatMs(gPlanWork.powerUs / 1000) + " waterMs=" + FormatMs(gPlanWork.waterUs / 1000) + " garbageMs=" + FormatMs(gPlanWork.garbageUs / 1000);
        for (int i = 0; i < 3; ++i)
            workText_ += std::string(" pf") + "PWG"[i] + "{build=" + FormatMs(gPortfolioWork.buildUs[i] / 1000) + " solve=" + FormatMs(gPortfolioWork.solveUs[i] / 1000) + " local=" +
                         std::to_string(gPortfolioWork.localEvals[i]) + " picks=" + std::to_string(gPortfolioWork.compositionEvals[i]) + "}";
        const std::uint32_t id = NewPlan(snap, plan, carried, "cycle");
        workText_.clear();
        LogDecision(snap, opt, tuning, plan, id);
        NoteReadiness(snap);
        lastPlanText_.clear();
        { const bool show = verbose_ || !plan.actions.empty(); for (const auto& l : FormatUnifiedPlan(plan)) { lastPlanText_ += l + "\n"; if (show) Info(l); } }
        ledger_.shadowPotential = plan.expectedMonthlyResult();
        if (!envLogged_ && validation_) LogEnvironment(snap, plan);
        RunLandfillAdvisor(snap, plan, opt, tuning, cfg, allow);

        // 3. per commodity: the lowest pending phase, only when its owner may act now and nothing in flight blocks it
        // Independent commodities are handled in the SAME cycle: each one gets its own action (a funding phase, or one deal), verified before the next. A deal decision refreshes the
        // commodity cache and replans from fresh state; the following commodities use THAT plan and snapshot (never a stale one), so nothing is executed from a state a write has changed.
        // The cycle stops only when a transaction is still in flight (it needs more observations before anything else may build on it).
        const Commodity order[3] = {Commodity::Power, Commodity::Water, Commodity::Garbage};
        UtilitySnapshot liveSnap = snap;
        UnifiedPlan livePlan = plan;
        std::uint32_t liveId = id;
        for (const Commodity c : order) {
            const int ci = CommodityIndex(c);
            const int phase = ExecutablePhase(livePlan, c);
            if (phase < 0 || CommodityBlocked(c)) continue;
            const bool fundingPhase = phase == kPhaseCapacityRaise || phase == kPhaseCapacityCut;
            if (c == Commodity::Power && !allow.power) continue;
            if (c == Commodity::Water && !allow.water) continue;
            if (c == Commodity::Garbage && !allow.garbage) continue;
            if (!ready_[ci].ok) continue;                 // the readings of THIS commodity are not ready: no write, no deal; the others carry on
            if (fundingPhase && FundingDisabled(c)) continue;      // a failed funding write stops this commodity's FUNDING for the session; its contracts are a separate permission and carry on
            if (fundingPhase) {
                if (allow.manage[ci]) ExecuteFundingPhase(c, phase, liveId, livePlan, liveSnap, cfg, act);   // the funding writes belong to the funding manager only (a frozen plan has none for an unmanaged commodity)
            } else if (allow.deals && allow.trade[ci] && !deals_off_.disabled) {          // the contracts belong to the master AND this commodity's contract permission only
                DealReplan replan;
                StartDealPhase(c, phase, liveId, livePlan, cfg, opt, tuning, act, &replan);
                if (replan.valid) { liveSnap = std::move(replan.snap); livePlan = std::move(replan.plan); liveId = replan.id; }
                // a deal still waiting for its readback only holds ITS commodity (CommodityBlocked); the other commodities carry on from the fresh plan
                if (curPlanId_ != liveId) break;            // defensive: the plan in use is not the current one
            }
        }
        NoteDisabledTransitions();
        Info(ledger_.Format());
    }

    // Landfill Size Savings Advisor hook: reads the snapshot and the plan of THIS cycle (no extra read, no second evaluation) and never writes. The tile count is observed on every cycle (the temporal
    // rules); the analysis needs the option AND a sanitation manager that really optimises (AutoGarbage on, funding neither frozen nor fail-closed, garbage readings ready, due in this cycle): with the
    // manager off the funding is frozen, a baseline would not be the plan's optimum. Check order (cheapest first): option, landfill, optimising, plan computable, 24-month protection, 12-month interval,
    // once per simulated month, budget (LandfillAdvisor.h).
    void RunLandfillAdvisor(const UtilitySnapshot& snap, const UnifiedPlan& plan, const OptimizerConfig& opt, const UtilityTuning& tuning, const UtilityResearchConfig& cfg, const Allow& allow) {
        const auto& a = snap.garbage.aggregate;
        LandfillCycle lc;
        lc.option = cfg.warnLandfillSizeSavings;
        lc.thresholdPercent = cfg.landfillSavingsAlertThreshold;
        lc.tilesKnown = a.landfillTiles.has_value() && MayReadForDecisions(snap.garbage.validity);
        if (lc.tilesKnown) lc.tiles = *a.landfillTiles;
        lc.now = LandfillSimMonth(snap.simYear, snap.simDayOfYear);
        lc.optimising = allow.garbage && allow.manage[2] && !opt.freezeGarbage && !garbageOff_.disabled && ready_[2].ok;
        lc.planComputable = plan.garbage.status == GarbagePlanStatus::Optimal || plan.garbage.status == GarbagePlanStatus::NoChange;
        lc.funding = snap.garbage.department.funding;
        lc.maxAllowed = snap.garbage.department.maxAllowed;
        lc.idealMonthly = snap.garbage.department.idealMonthly;
        const auto base = [&]() { auto b = GarbageBaseInput(snap, opt, tuning); ApplyGarbageTargets(b.input, plan.garbageTargets); return b.input; };
        // the spatial view of the landfill (LandfillSpatial.h), read at most once in this cycle and shared by the ghost repair and the size advisor
        LandfillSnapshotCache spatial(landfillSpatialRead);
        // ghost squares first (a repair changes the zone the size advisor would analyse; the advisor then sees the repaired zone and its protection restarts)
        // (ghost squares are a game bug, not an economic choice: repaired whenever the Department manages sanitation; no option, no threshold, no notice, log only)
        LandfillGhostCycle gc;
        gc.optimising = lc.optimising && lc.planComputable && lc.tilesKnown && lc.tiles > 0;
        gc.now = lc.now;
        for (auto& l : landfillGhosts_.Evaluate(gc, spatial, landfillNativeCheck)) Info(std::move(l));
        // a repair does NOT restart the advisor's 24-month protection: the game restores the ghosts at every load, so the repair recurs every session (a restart would block the advisor for
        // good), and it never lowers the capacity; the advisor only waits for the verification and then analyses the repaired zone
        if (landfillGhosts_.verifying()) return;          // the zone is about to change (the game has not handled the re-zoning yet): no size analysis on a view of before
        for (auto& l : landfillAdvisor_.Evaluate(lc, base, &spatial, landfillNativeCheck)) Info(std::move(l));
    }

    // The verdict of the fresh snapshot per commodity, logged only when it changes. A commodity that is not ready is not acted on in this cycle (the loop above); nothing is inferred from a
    // reading that is missing, contradictory or not populated.
    void NoteReadiness(const UtilitySnapshot& snap) {
        static const char* names[3] = {"POWER", "WATER", "GARBAGE"};
        const Readiness v[3] = {PowerReadiness(snap), WaterReadiness(snap), GarbageReadiness(snap)};
        const Readiness d = DealsReadiness(snap);
        for (int i = 0; i < 3; ++i) {
            // the deals of a commodity need the contracts read as well
            const bool ok = v[i].ready && d.ready;
            const std::string why = !v[i].ready ? v[i].reason : d.reason;
            if (!ready_[i].known || ready_[i].ok != ok || ready_[i].reason != why)
                Info(std::string("[Readiness] ") + names[i] + (ok ? " READY: the readings needed for a safe decision are present, current and consistent" : " NOT_READY (no write for it yet): " + why));
            ready_[i] = {ok, true, why};
        }
    }

    // ---- diagnostics (never read by a decision) -----------------------------------------------------------------------------------------
    static std::string RowText(const UtilitySnapshot& s, const DealKey& k) {
        for (const auto& r : s.deals.deals) {
            if (r.type != k.type || !r.otherCitySerial || *r.otherCitySerial != k.neighbor) continue;
            std::ostringstream o;
            o << "flags=0x" << std::hex << (r.flags ? *r.flags : 0) << std::dec << " amount=" << (r.amount ? std::to_string(*r.amount) : std::string("?")) << " AmountUsed=" << (r.amountUsed ? std::to_string(*r.amountUsed) : std::string("UNKNOWN"))
              << " storedCost=" << (r.cost ? std::to_string(*r.cost) : std::string("?"));
            return o.str();
        }
        return "row=ABSENT";
    }
    static std::string RegionalText(const UtilitySnapshot& s, Commodity c) {
        const int ci = c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : 2;
        const auto& b = s.deals.localMarket.book[ci];
        std::ostringstream o;
        o << "regionalImported=" << (b.imported ? std::to_string(static_cast<long long>(*b.imported)) : std::string("?")) << " regionalExported=" << (b.exported ? std::to_string(static_cast<long long>(*b.exported)) : std::string("?"))
          << " monthlyIncome=" << (s.deals.monthlyIncome ? std::to_string(static_cast<long long>(*s.deals.monthlyIncome)) : std::string("?")) << " monthlyExpense=" << (s.deals.monthlyExpense ? std::to_string(static_cast<long long>(*s.deals.monthlyExpense)) : std::string("?"));
        return o.str();
    }

    // The decision of one plan: snapshot, candidates, legal modes, optimum, counterfactual marginals, search, day projection; plus what changed since the previous plan.
    void LogDecision(const UtilitySnapshot& snap, const OptimizerConfig& opt, const UtilityTuning& tuning, const UnifiedPlan& plan, std::uint32_t id) {
        const std::string dayKey = std::to_string(snap.simYear) + '/' + std::to_string(snap.simDayOfYear);
        const bool acting = !plan.actions.empty();
        const bool full = acting || dayKey != lastLoggedDay_ || !decisionLoggedOnce_;
        decisionLoggedOnce_ = true;
        lastLoggedDay_ = dayKey;
        if (verbose_) for (const auto& l : DescribeDealOptimizer(snap, opt, tuning, plan, id, full)) Info(l);
        // ReplanDiff: which causal input moved since the previous decision
        const DealCausal now = verbose_ ? DescribeGarbageCausal(snap, opt, tuning) : prevCausal_;
        std::string targets;
        for (const auto& t : plan.garbageTargets) targets += (targets.empty() ? "" : ",") + std::to_string(t.key.type) + "/" + std::to_string(t.key.neighbor) + "=" + std::to_string(t.amount);
        std::string modes;
        for (const auto& c : plan.portfolio.commodities) modes += (modes.empty() ? "" : ",") + c.mode;
        if (verbose_ && prevPlanId_ != 0) {
            Info("[UnifiedAuto][ReplanDiff] previousPlan=" + std::to_string(prevPlanId_) + " newPlan=" + std::to_string(id) + " changedInputs=" +
                 (DealCausal::Changed(prevCausal_, now).empty() ? std::string("NONE") : DealCausal::Changed(prevCausal_, now)) + " previousMode=" + prevModes_ + " newMode=" + modes + " previousTarget=" + prevGarbageTarget_ +
                 " newTarget=" + std::to_string(plan.garbage.targetPercent) + "%/" + targets);
        }
        prevPlanId_ = id;
        prevCausal_ = now;
        prevModes_ = modes;
        prevGarbageTarget_ = std::to_string(plan.garbage.targetPercent) + "%/" + targets;
        context_ = "planId=" + std::to_string(id) + " simulationDate=" + dayKey + " currentPortfolioMode=" + modes + " currentGarbageTargets=" + targets + " currentSanitationFunding=" +
                   std::to_string(plan.garbage.currentPercent) + "%->" + std::to_string(plan.garbage.targetPercent) + "% lastSuccessfulWrite=" + (lastWrite_.empty() ? std::string("NONE") : lastWrite_);
    }

    // A plan generation: ID, epoch, what it contains. A verified write is always followed by a NEW id (fresh snapshot, replan).
    std::uint32_t NewPlan(const UtilitySnapshot& snap, const UnifiedPlan& plan, bool carried, const char* why) {
        const std::uint32_t id = ++planId_;
        curPlanId_ = id;
        FvPlanEvent ev;
        ev.id = id;
        ev.epoch = snap.lifecycleEpoch;
        ev.powerValid = (snap.power.validity == SnapshotValidity::Valid || snap.power.validity == SnapshotValidity::Partial) && plan.power.status != PlanStatus::NotAvailable;
        ev.waterValid = (snap.water.validity == SnapshotValidity::Valid || snap.water.validity == SnapshotValidity::Partial) && plan.water.status != PlanStatus::NotAvailable;
        ev.sanitationValid = plan.garbage.status != GarbagePlanStatus::NotAvailable;
        ev.portfolioUsed = plan.portfolioUsed[0] || plan.portfolioUsed[1] || plan.portfolioUsed[2];
        for (const auto& a : plan.actions) {
            if (a.kind == UnifiedActionKind::PlantFunding) ++ev.powerActions;
            else if (a.kind == UnifiedActionKind::WaterFunding) ++ev.waterActions;
            else if (a.kind == UnifiedActionKind::SanitationFunding) ++ev.sanitationActions;
            else ++ev.dealActions;
        }
        ev.pendingCarried = carried;
        tracker_.OnPlan(ev);
        std::ostringstream o;
        o << "[UnifiedAuto][Plan] id=" << id << " (" << why << ") epoch=" << snap.lifecycleEpoch << " day=" << snap.simYear << '/' << snap.simDayOfYear << " actions power=" << ev.powerActions
          << " water=" << ev.waterActions << " sanitation=" << ev.sanitationActions << " deals=" << ev.dealActions << " expectedMonthlyResult=" << plan.expectedMonthlyResult() << workText_;
        Info(o.str());
        UpdateServiceEpisodes(snap, plan);
        AuditContracts(plan);
        {   // the committed contracts as the game shows them: logged once when the city's first plan is made (BEFORE any write of this session) and kept for the shutdown line (save / reload proof)
            contractTable_ = ContractTableOf(snap);
            if (!contractTableLogged_) { contractTableLogged_ = true; Info("[Contracts] LOADED " + contractTable_); }
        }
        return id;
    }

    static std::string ContractTableOf(const UtilitySnapshot& s) {
        std::vector<std::string> e;
        for (const auto& r : s.deals.deals) {
            if (!DealIsCommitted(r) || !r.otherCitySerial) continue;
            e.push_back(std::string(DealTypeName(r.type)) + ":" + std::to_string(*r.otherCitySerial) + "=" + (r.amount ? std::to_string(*r.amount) : std::string("?")) + "@" + (r.cost ? std::to_string(*r.cost) : std::string("?")));
        }
        std::sort(e.begin(), e.end());
        std::string out;
        for (const auto& x : e) out += (out.empty() ? "" : ",") + x;
        return out.empty() ? std::string("none") : out;
    }
    // READ-ONLY diagnosis of every existing contract, one line when its state changes (first plan after the city loads, or an amount / cost / marginal / origin change):
    // what it is, what it brings, whether the service needs it and where it came from. It never influences a decision.
    //  origin: PLUGIN_THIS_SESSION = created by this plugin since the city loaded; SAVE_OR_EARLIER_SESSION = present when the city loaded (a save, or a previous session: not distinguishable);
    //  SUSPENDED = an obligation of the game, not changeable.
    void AuditContracts(const UnifiedPlan& plan) {
        for (const auto& pc : plan.portfolio.commodities) {
            for (const auto& d : pc.decisions) {
                if (d.from <= 0 && !d.dormant) continue;
                const std::string key = std::to_string(d.key.type) + '/' + std::to_string(d.key.neighbor);
                std::ostringstream o;
                o << "[ContractAudit] " << CommodityName(pc.commodity) << ' ' << DealTypeName(d.key.type) << " neighbor=" << d.key.neighbor << " physicalAmount=" << d.from
                  << " storedMonthly=" << d.fromMoney << " quoteNow=" << d.toQuoteNow << " economicMarginal=" << (d.marginalKnown ? std::to_string(d.marginal) : std::string("N/A")) << " cashMarginal="
                  << (d.marginalKnown ? std::to_string(d.cashMarginal) : std::string("N/A")) << " cancelEffectOnService=" << (d.requiredForFeasibility ? "SERVICE_NEEDS_IT" : "NONE")
                  << " origin=" << (createdHere_.count(key) ? "PLUGIN_THIS_SESSION" : "SAVE_OR_EARLIER_SESSION") << " plannedTo=" << d.to << (d.dormant ? " dormant=YES(active row at amount 0: the game charges its stored value for nothing; cancelled)" : "");
                const std::string sig = o.str();
                auto it = auditSig_.find(key);
                if (it == auditSig_.end() || it->second != sig) { auditSig_[key] = sig; Info(sig); }
            }
            for (const auto& cd : pc.candidateDiag) {
                if (!cd.fixed || cd.current <= 0) continue;
                const std::string key = std::to_string(cd.key.type) + '/' + std::to_string(cd.key.neighbor);
                const std::string sig = std::string("[ContractAudit] ") + CommodityName(pc.commodity) + ' ' + DealTypeName(cd.key.type) + " neighbor=" + std::to_string(cd.key.neighbor) +
                                        " physicalAmount=" + std::to_string(cd.current) + " origin=SUSPENDED (an obligation of the game, not changeable; it counts in the local need)";
                auto it = auditSig_.find(key);
                if (it == auditSig_.end() || it->second != sig) { auditSig_[key] = sig; Info(sig); }
            }
        }
    }

    // The one intrusive service warning (see ServiceNotices.h). The episode state lives apart from the economic optimiser: it only reads the plan, never steers it.
    void UpdateServiceEpisodes(const UtilitySnapshot& snap, const UnifiedPlan& plan) {
        static const char* names[3] = {"POWER", "WATER", "GARBAGE"};
        for (int i = 0; i < 3 && static_cast<std::size_t>(i) < plan.portfolio.commodities.size(); ++i) {
            const auto& pc = plan.portfolio.commodities[static_cast<std::size_t>(i)];
            const bool pending = pendingDeals_[i] || (i == 2 && pendingSan_);
            const bool data = NoticeEpisodes::DataComplete(snap, i);
            bool proven = false, recovered = false;
            std::string basis;
            if (i < 2) {
                // authoritative counters: the need above the service delivered AND the supply short (an internal connection problem never gets here)
                // the warning waits for the recovery: while valid purchases / cancellations of this plan are still to be executed the insufficiency is not final (PlanServiceEmergency)
                proven = pc.serviceDeficit && data && !pending && pc.recoveryPending == 0 && monitor_.InDeficit(i == 0);
                recovered = servedOk_[i];
                basis = "game counters: demand " + std::to_string(i == 0 ? lastCounters_.pDemand.value_or(-1) : lastCounters_.wDemand.value_or(-1)) + ", service delivered " +
                        std::to_string(i == 0 ? lastCounters_.pConsumption.value_or(-1) : lastCounters_.wConsumed.value_or(-1)) +
                        (buyAllowed_[i] ? "; maximum usable local capacity and every regional purchase on offer cannot cover it"
                                        : "; maximum usable local capacity and the existing contracts cannot cover it (the player has switched off the automatic management of this resource's regional contracts)");
            } else {
                // Garbage has no authoritative need counter: this is the physical MODEL, not a measurement by the game
                proven = pc.serviceDeficit && data && !pending && pc.recoveryPending == 0 && snap.garbage.validity == SnapshotValidity::Valid;
                recovered = pc.evaluated && !pc.serviceDeficit && snap.garbage.aggregate.uncollected && *snap.garbage.aggregate.uncollected == 0;
                basis = buyAllowed_[2] ? "physical model (not a game measurement): the facilities and every export on offer cannot process the garbage produced"
                                       : "physical model (not a game measurement): the facilities and the existing export contracts cannot process the garbage produced (the player has switched off the automatic management of the garbage contracts)";
            }
            if (pc.serviceDeficit && !pc.recovery.empty()) {
                basis += "; recovery=" + pc.recovery;
                if (pc.emergencyDemand >= 0) basis += " (the best plan serves " + std::to_string(pc.emergencyServedPlan) + " of " + std::to_string(pc.emergencyDemand) + " with local capacity " + std::to_string(pc.emergencyCapacity) + " and " + std::to_string(pc.emergencyBought) + " bought)";
                else basis += " (exports in force after the plan: " + std::to_string(pc.emergencyBought) + ")";
            }
            const auto r = episodes_.Update(i, proven, recovered);
            if (r.emit) Info(std::string("[Service][NoSupply] ") + names[i] + ": " + basis);
            else if (r.rearmed) Info(std::string("[Service][EpisodeEnded] ") + names[i] + ": recovery proven, a new insufficiency would be a new episode");
        }
    }

    // A fresh authoritative sample of Power (0) or Water (1): recovery evidence, and the single log line of a state change (never a line per day).
    void NoteServiceSample(int i, const ServiceVerdict& v, const Opt<std::int64_t>& demand, const Opt<std::int64_t>& served) {
        if (v.cause == ServiceCause::Stale) return;                     // an old counter proves nothing
        if (demand && served) servedOk_[i] = *served >= *demand;
        const bool internal = v.cause == ServiceCause::InternalShortfall;
        if (v.trigger) Info(std::string("[ServiceCheck] ") + ServiceCauseName(v.cause) + ": " + v.text + " -> immediate replan");
        else if (internal && !internalLogged_[i]) Info(std::string("[ServiceCheck] ") + ServiceCauseName(v.cause) + ": " + v.text);
        internalLogged_[i] = internal;
    }

    void NoteDisabledTransitions() {
        if (legacy_.powerDisabled() && !lastPowerOff_) { tracker_.OnFailedWrite("power fail-closed"); lastPowerOff_ = true; }
        if (legacy_.waterDisabled() && !lastWaterOff_) { tracker_.OnFailedWrite("water fail-closed"); lastWaterOff_ = true; }
    }

    UnifiedPlan PlanFor(const UtilitySnapshot& snap, const UtilityResearchConfig& cfg, const OptimizerConfig& opt, const UtilityTuning& tuning) const {
        UnifiedOptions uo;
        // The contracts of a commodity are planned only with the master (AutoDeals) AND its AutoTrade* permission; its funding manager is not part of it. A commodity left out keeps its real contracts untouched
        // and counted (fixed commitments of the local plans, in both directions). A failed funding write / Sanitation verification (fail-closed) freezes only that commodity's FUNDING (UnifiedOptimizerConfig); its contracts go on.
        uo.includeDeals = !deals_off_.disabled && cfg.autoDeals;
        uo.dealScope.commodity[0] = cfg.tradeActive(0);
        uo.dealScope.commodity[1] = cfg.tradeActive(1);
        uo.dealScope.commodity[2] = cfg.tradeActive(2);
        // A commodity whose contracts are the player's but whose service the Department still watches (its manager is on) is evaluated for the DIAGNOSIS only: its real contracts pinned as they are, no action,
        // never the accounting of the local plans. It is how the shortage notice still reaches a player who manages the contracts by hand.
        uo.dealScope.observe[0] = cfg.autoPower && !cfg.tradeActive(0);
        uo.dealScope.observe[1] = cfg.autoWater && !cfg.tradeActive(1);
        uo.dealScope.observe[2] = cfg.autoGarbage && !cfg.tradeActive(2);
        uo.dealScope.modeTable = true;                     // diagnostics only: LOCAL / BUY / SELL solved separately (log only)
        // a deficit confirmed by the game's own counters: no sale of that commodity may stay in force
        uo.dealScope.noSell[0] = monitor_.InDeficit(true);
        uo.dealScope.noSell[1] = monitor_.InDeficit(false);
        for (const auto& k : dormantGiveUp_) uo.dealScope.skipDormant.push_back(k);
        for (const auto& r : refusedNew_)
            if (months_ - r.month < kRefusalMonths) uo.dealScope.refusedNew.push_back(r.key);
        return BuildUnifiedPlan(snap, opt, tuning, uo);
    }

    // ---- funding phases --------------------------------------------------------------------------------------------------------------
    void ExecuteFundingPhase(Commodity c, int phase, std::uint32_t id, const UnifiedPlan& plan, const UtilitySnapshot& snap, const UtilityResearchConfig& cfg, IUnifiedActuator& act) {
        const bool raise = phase == kPhaseCapacityRaise;
        if (c == Commodity::Power || c == Commodity::Water) {
            SubsystemPlan only = c == Commodity::Power ? plan.power : plan.water;
            only.actions.clear();
            for (const auto& a : (c == Commodity::Power ? plan.power : plan.water).actions)
                if (a.capability == ActionCapability::Executable && ((a.toPercent > a.fromPercent) == raise)) only.actions.push_back(a);
            if (only.actions.empty()) return;
            const bool power = c == Commodity::Power;
            std::ostringstream pre;
            for (const auto& a : only.actions) pre << (power ? "plant@" + std::to_string(a.cellX) + "," + std::to_string(a.cellZ) : std::string("department")) << ' ' << a.fromPercent << "%->" << a.toPercent << "% ";
            Info("[UnifiedAuto][Execute] plan=" + std::to_string(id) + " phase=" + std::to_string(phase + 1) + " " + UnifiedPhaseName(phase) + " " + CommodityLabel(c) + " preState " + pre.str());
            if (power) for (const auto& l : DescribePowerEconomics(snap, UnifiedOptimizerConfig(cfg), VanillaUtilityTuning(), plan)) Info(l);
            tracker_.OnExecute(id, curPlanId_, phase, LowestPhase(plan, c));
            const auto before = legacy_.ledger();
            if (power) legacy_.ExecutePowerPlan(snap, only, cfg, act); else legacy_.ExecuteWaterPlan(snap, only, cfg, act);
            Drain();
            const auto after = legacy_.ledger();
            const int written = after.writes - before.writes;
            if (written > 0) {
                ++ledger_.fundingWrites;
                tracker_.OnFundingWrite(power, true, 0);          // counted here; the keyed reversal check below
                for (const auto& a : only.actions)
                    tracker_.OnFundingAction(power, power ? "plant@" + std::to_string(a.cellX) + "," + std::to_string(a.cellZ) : std::string("department"), raise ? 1 : -1,
                                             power ? DescribePowerCausal(snap, a.cellX, a.cellZ) : DescribeWaterCausal(snap));
                Info("[UnifiedAuto][Verified] plan=" + std::to_string(id) + " " + CommodityLabel(c) + " funding write readback OK (writer compare-and-set + getter); the next cycle replans from a fresh snapshot");
            }
            ledger_.realizedPower = SatAdd(ledger_.realizedPower, SatSub(after.realizedPower, before.realizedPower));
            ledger_.realizedWater = SatAdd(ledger_.realizedWater, SatSub(after.realizedWater, before.realizedWater));
            return;
        }
        // Sanitation: one asynchronous transaction
        for (const auto& a : plan.actions) {
            if (a.kind != UnifiedActionKind::SanitationFunding || a.phase != phase) continue;
            // PACED (SanitationPacing.h): the funding of the garbage department moves in steps and not on every cycle; the plan is the optimizer's, the execution is paced.
            const PacedSanitation paced = PaceSanitationWrite(a.sanitation.expectedPercent, a.sanitation.targetPercent, haveSanWrite_, days_ - lastSanWriteDay_);
            if (!paced.allowed) {
                if (!pacedLogged_ || days_ - lastPacedLogDay_ >= 10) {
                    pacedLogged_ = true;
                    lastPacedLogDay_ = days_;
                    Info("[UnifiedAuto][Sanitation] PACED plan=" + std::to_string(id) + " " + paced.note + " (" + std::to_string(a.sanitation.expectedPercent) + "% -> plan target " + std::to_string(a.sanitation.targetPercent) + "%)");
                }
                return;
            }
            auto p = std::make_unique<PendingSan>();
            p->plan = a.sanitation;
            if (paced.target != a.sanitation.targetPercent) {
                p->plan.targetPercent = paced.target;
                p->plan.reason += " [paced: " + paced.note + "]";
                Info("[UnifiedAuto][Sanitation] PACED plan=" + std::to_string(id) + " " + paced.note);
            }
            haveSanWrite_ = true;
            lastSanWriteDay_ = days_;
            p->planId = id;
            p->direction = raise ? 1 : -1;
            {
                const auto di = DescribeGarbageDecision(snap, UnifiedOptimizerConfig(cfg), VanillaUtilityTuning(), plan, id);
                p->causal = di.causal;
                if (!di.line.empty()) Info(di.line);
                for (const auto& l : di.extra) Info(l);
            }
            p->machine = std::make_unique<SanitationWriteMachine>(MakeRequest(p->plan, snap.lifecycleEpoch), VanillaUtilityTuning().powerEfficiency);
            if (snap.garbage.department.totalExpenses) { p->expenseBefore = *snap.garbage.department.totalExpenses; p->expenseKnown = true; }
            ledger_.pending = SatSub(ledger_.pending, a.sanitation.expectedMonthlyDelta);
            Info("[UnifiedAuto][Execute] plan=" + std::to_string(id) + " phase=" + std::to_string(phase + 1) + " " + UnifiedPhaseName(phase) + " GARBAGE preState funding=" +
                 std::to_string(a.sanitation.expectedPercent) + "% efficiency=" + (snap.garbage.aggregate.fundingEfficiency ? std::to_string(*snap.garbage.aggregate.fundingEfficiency) : std::string("?")) +
                 " target=" + std::to_string(p->plan.targetPercent) + "% (" + p->plan.reason + ")");
            pendingSan_ = std::move(p);
            PumpSanitationTx(act, true);
            return;   // one Sanitation transaction at a time
        }
    }

    // `mayWrite` = the Garbage manager is on. A Sanitation transaction is a single funding write followed by read-only verification: with the manager off, a transaction whose write has not been issued is
    // dropped WITHOUT issuing it; one whose write was issued is only observed until it concludes (no rollback: the funding stays as the game has it).
    void PumpSanitationTx(IUnifiedActuator& act, bool mayWrite) {
        auto& p = *pendingSan_;
        if (!mayWrite && !p.machine->writeIssued()) {
            Warn("[UnifiedAuto][Sanitation] SUSPENDED_BY_OPT_OUT plan=" + std::to_string(p.planId) + ": Garbage management is off, the funding write was not issued and is not issued now");
            ledger_.pending = SatAdd(ledger_.pending, p.plan.expectedMonthlyDelta);
            pendingSan_.reset();
            return;
        }
        for (int i = 0; i < 3 && (mayWrite || p.machine->writeIssued()); ++i) {
            const SanitationStep st = act.PumpSanitation(*p.machine, epoch_);
            if (!st.write) break;
        }
        const auto state = p.machine->state();
        const std::string pid = std::to_string(p.planId);
        if (p.machine->writeIssued() && !p.issuedLogged) {
            p.issuedLogged = true;
            tracker_.OnExecute(p.planId, curPlanId_, p.direction > 0 ? kPhaseCapacityRaise : kPhaseCapacityCut, p.direction > 0 ? kPhaseCapacityRaise : kPhaseCapacityCut);
            tracker_.OnSanitation(FvSanitation::Issued, p.direction, p.causal);
            Info("[UnifiedAuto][Sanitation] SANITATION_WRITE_ISSUED plan=" + pid + " " + std::to_string(p.plan.expectedPercent) + "% -> " + std::to_string(p.plan.targetPercent) + "%");
        }
        if (p.machine->fundingReadBack() && !p.readbackLogged) {
            p.readbackLogged = true;
            tracker_.OnSanitation(FvSanitation::FundingReadback);
            Info("[UnifiedAuto][Sanitation] FUNDING_READBACK plan=" + pid + " the department funding reads the target (stage one of two)");
        }
        if (!IsTerminal(state)) {
            tracker_.OnSanitation(FvSanitation::CapacityPending);
            if (!p.pendingLogged) {
                p.pendingLogged = true;
                Info(std::string("[UnifiedAuto][Sanitation] CAPACITY_PENDING plan=") + pid + " " + SanitationWriteStateName(state) + ": " + p.machine->reason() +
                     " (no other Sanitation write and no Garbage deal until CAPACITY_READBACK_CONFIRMED)");
            }
            return;
        }
        const auto plan = p.plan;
        const bool expenseKnown = p.expenseKnown;
        const std::int64_t before = p.expenseBefore;
        const bool issued = p.issuedLogged;
        ledger_.pending = SatAdd(ledger_.pending, plan.expectedMonthlyDelta);
        const std::string why = p.machine->reason();
        pendingSan_.reset();
        if (state == SanitationWriteState::CapacityReadbackConfirmed) {
            ++ledger_.sanitationTransactions;
            ++ledger_.fundingWrites;
            UtilitySnapshot after;
            std::int64_t saving = -plan.expectedMonthlyDelta;
            if (expenseKnown && act.ReadForPlan(after) && after.garbage.department.totalExpenses) saving = before - *after.garbage.department.totalExpenses;
            ledger_.realizedSanitation = SatAdd(ledger_.realizedSanitation, saving);
            tracker_.OnSanitation(FvSanitation::Confirmed);
            Info("[UnifiedAuto][Sanitation] CAPACITY_CONFIRMED plan=" + pid + " saving=" + std::to_string(saving) + " (" + why + ")");
            Info("[UnifiedAuto][Verified] plan=" + pid + " GARBAGE Sanitation funding, efficiency and capacity reflect the new funding; the next cycle replans from a fresh snapshot");
            return;
        }
        if (state == SanitationWriteState::VerifyFailed) {
            tracker_.OnSanitation(FvSanitation::Failed);
            tracker_.OnFailedWrite("sanitation fail-closed: " + why);
            Err("[UnifiedAuto][Sanitation] FAIL CLOSED (technical): " + why + "; Sanitation funding disabled for this city session (garbage contracts, if enabled, continue with the funding frozen); Power and Water continue");
            garbageOff_ = {true, why};
            return;
        }
        if (issued) tracker_.OnSanitation(FvSanitation::Failed);
        Warn(std::string("[UnifiedAuto][Sanitation] ") + SanitationWriteStateName(state) + ": " + why + "; skipped, replanning from fresh state");
    }

    // ---- deal phases -----------------------------------------------------------------------------------------------------------------
    // The state a deal decision replanned from (the quote refresh): the rest of the cycle continues from it instead of waiting for the next cycle.
    struct DealReplan {
        bool valid = false;
        UtilitySnapshot snap;
        UnifiedPlan plan;
        std::uint32_t id = 0;
    };

    void StartDealPhase(Commodity c, int phase, std::uint32_t id, const UnifiedPlan& plan, const UtilityResearchConfig& cfg, const OptimizerConfig& opt, const UtilityTuning& tuning,
                        IUnifiedActuator& act, DealReplan* replan = nullptr) {
        if (pendingDeals_[CommodityIndex(c)]) return;   // one deal transaction in flight per commodity (the commodities have separate books and contracts)
        const UnifiedAction* chosen = nullptr;
        for (const auto& a : plan.actions)
            if (a.kind == UnifiedActionKind::Deal && a.commodity == c && a.phase == phase) { chosen = &a; break; }
        if (!chosen) return;
        const PortfolioAction planned = chosen->deal;
        const std::string phaseText = "phase=" + std::to_string(phase + 1) + " " + UnifiedPhaseName(phase);
        const bool needsQuote = planned.write.op != DealWriteOp::Cancel;

        // The plan was built before this decision: refresh the vanilla commodity cache, read again, and require the SAME action to
        // survive in a plan made from the fresh state. A profitable quote of an earlier moment is never executed.
        if (!act.RefreshCommodityStats()) { Warn("[UnifiedAuto][Deal] commodity stats refresh unavailable: zero deal writes"); return; }
        UtilitySnapshot fresh;
        if (!act.ReadForPlan(fresh) || fresh.lifecycleEpoch != epoch_) { Warn("[UnifiedAuto][Deal] fresh read unavailable or from another lifecycle: zero deal writes"); return; }
        fresh.garbage.geometry = landfill_.View();
        const UnifiedPlan again = PlanFor(fresh, cfg, opt, tuning);
        const std::uint32_t id2 = NewPlan(fresh, again, false, "quote-refresh replan");
        LogDecision(fresh, opt, tuning, again, id2);
        if (replan) { replan->valid = true; replan->snap = fresh; replan->plan = again; replan->id = id2; }
        const UnifiedAction* match = nullptr;
        for (const auto& a : again.actions) {
            if (a.kind != UnifiedActionKind::Deal) continue;
            const auto& w = a.deal.write;
            if (w.key == planned.write.key && w.op == planned.write.op && w.expectedAmount == planned.write.expectedAmount && w.targetAmount == planned.write.targetAmount) match = &a;
        }
        // The books the game priced against (diagnostic only): our side and the neighbor's, as read after UpdateCommodityStats, so a model-versus-game difference can be recomputed offline.
        const auto bookText = [&]() {
            const int ci = c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : 2;
            const auto one = [](const CommodityBookRow& r) {
                if (!r.complete()) return std::string("incomplete");
                const CommodityBook b = r.get();
                std::ostringstream o;
                o << std::setprecision(12) << "produced/imported/demanded/exported/unit=" << b.produced << '/' << b.imported << '/' << b.demanded << '/' << b.exported << '/' << b.costPerUnit;
                return o.str();
            };
            std::string out = " books{local " + one(fresh.deals.localMarket.book[ci]);
            for (const auto& n : fresh.deals.neighbors)
                if (n.serial == planned.write.key.neighbor) out += "; neighbor " + one(n.book[ci]);
            // the basis of the decision (from the plan made on the FRESH state): everything a flip of the amount could depend on, in one place
            const std::size_t ix = static_cast<std::size_t>(ci);
            if (ix < again.portfolio.commodities.size()) {
                const auto& r = again.portfolio.commodities[ix];
                out += "} basis{day=" + std::to_string(fresh.simYear) + "/" + std::to_string(fresh.simDayOfYear) + " demand=" + std::to_string(r.demand) + " localCapacityNow=" + std::to_string(r.localCapacityNow) +
                       " requiredLocalNeed=" + std::to_string(r.requiredNeed) + " keepFeasible=" + (r.keepFeasible ? "YES" : "NO") + " optimumNet=" + std::to_string(r.optimumNet) + " keepNet=" + std::to_string(r.keepNet);
            }
            return out + "} toQuoteNow=" + std::to_string(planned.toQuoteNow);
        };
        const auto quoteLine = [&](const std::string& fresher, const std::string& auth, const std::string& net, const std::string& decision) {
            Info("[UnifiedAuto][Quote] plan=" + std::to_string(id) + "->" + std::to_string(id2) + " " + DealText(planned.write.key) + " " + std::to_string(planned.write.expectedAmount) + "->" +
                 std::to_string(planned.write.targetAmount) + " plannedQuote=" + (needsQuote ? std::to_string(planned.toMoney) : std::string("n/a")) + " UpdateCommodityStats=done freshModelQuote=" + fresher +
                 " authoritativeQuote=" + auth + " netAfterRefresh=" + net + " decision=" + decision + bookText());
        };
        if (!match) {
            quoteLine("-", "-", "-", "NO_WRITE (the action is not in the plan made after the refresh)");
            tracker_.OnQuote(false, false);
            return;
        }
        if (match->phase != ExecutablePhase(again, c)) {
            quoteLine(std::to_string(match->deal.toMoney), "-", MarginalText(match->deal), "NO_WRITE (an earlier phase appeared after the refresh)");
            tracker_.OnQuote(false, false);
            return;
        }
        const PortfolioAction& act2 = match->deal;
        const bool creates = act2.kind == PortfolioActionKind::Create || act2.kind == PortfolioActionKind::Increase;
        // CREATE / INCREASE needs a strictly positive monetary marginal, OR to be required for feasibility (the alternative cannot be served: no
        // monetary comparison exists). The completed target is feasible by construction, and the optimum already respects the protected own reserve and
        // no-re-export, so "required" never bypasses them.
        if (creates && act2.marginalKnown && act2.marginal <= 0) {
            quoteLine(std::to_string(act2.toMoney), "-", MarginalText(act2), "NO_WRITE (net benefit not strictly positive)");
            tracker_.OnQuote(false, false);
            return;
        }
        std::string authText = "n/a";
        if (act2.write.op != DealWriteOp::Cancel) {
            const auto quote = act.Quote(act2.write.key, act2.write.targetAmount);
            if (!quote) {
                quoteLine(std::to_string(act2.toMoney), "unavailable", MarginalText(act2), "NO_WRITE (authoritative quote unavailable)");
                tracker_.OnQuote(false, false);
                return;
            }
            authText = std::to_string(*quote);
            // the game answers against the CURRENT amount (QuoteCurrentState); the plan was valued in the post-deal state, the two coincide by construction
            const std::int64_t expectedQuote = act2.toQuoteNow > 0 ? act2.toQuoteNow : act2.toMoney;
            const std::int64_t tolerance = std::max<std::int64_t>(2, expectedQuote / 100);
            if (std::llabs(*quote - expectedQuote) > tolerance) {
                quoteLine(std::to_string(act2.toMoney), authText, MarginalText(act2), "NO_WRITE (PRICING MODEL DIVERGENCE: Deal Auto fail-closed)");
                tracker_.OnQuote(false, true);
                tracker_.OnFailedWrite("deal fail-closed: pricing model divergence");
                Err("[UnifiedAuto][Deal] PRICING MODEL DIVERGENCE: authoritative quote " + authText + " vs modelled " + std::to_string(expectedQuote) +
                    "; Deal Auto disabled for this city session (local utilities continue with the active-contract accounting)");
                deals_off_ = {true, "pricing model divergence"};
                return;
            }
        }
        quoteLine(std::to_string(act2.toMoney), authText, MarginalText(act2), "WRITE");
        tracker_.OnQuote(true, false);
        tracker_.OnDealStart(c == Commodity::Garbage);
        if (c == Commodity::Garbage)   // the economics and the import schedule the decision rests on (foreign garbage only; empty otherwise)
            for (const auto& l : DescribeGarbageDecision(fresh, opt, tuning, again, id2).extra) Info(l);
        tracker_.OnExecute(id2, curPlanId_, phase, ExecutablePhase(again, c));
        auto p = std::make_unique<PendingDeal>();
        p->action = act2;
        p->commodity = c;
        p->planId = id2;
        p->netBefore = NetOf(fresh, p->netKnown);
        p->causal = DescribeDealCausal(fresh, opt, tuning, act2.write.key).Exogenous();     // exogenous inputs only (stability gate)
        p->preRow = RowText(fresh, act2.write.key);
        p->preRegional = RegionalText(fresh, c);
        Info("[DealTx][PreWrite] plan=" + std::to_string(id2) + " decisionId=" + DecisionId(fresh, id2) + " commodity=" + CommodityName(c) + " type=" + DealTypeName(act2.write.key.type) + " neighbor=" +
             std::to_string(act2.write.key.neighbor) + " operation=" + std::to_string(static_cast<int>(act2.write.op)) + " expectedAmount=" + std::to_string(act2.write.expectedAmount) + " targetAmount=" +
             std::to_string(act2.write.targetAmount) + ' ' + p->preRow + " modelQuote=" + std::to_string(act2.toMoney) + " authoritativeQuote=" + authText + " economicMarginal=" +
             (act2.marginalKnown ? std::to_string(act2.marginal) : std::string("N/A")) + " cashMarginal=" + (act2.marginalKnown ? std::to_string(act2.cashMarginal) : std::string("N/A")) + " requiredReason=" +
             (act2.requiredReason.empty() ? std::string("none") : act2.requiredReason) + " reason=\"" + act2.write.reason + "\" " + p->preRegional);
        p->machine = std::make_unique<DealWriteMachine>(MakeRequest(act2.write, fresh.lifecycleEpoch));
        ledger_.pending = SatAdd(ledger_.pending, act2.cashMarginal);   // CASH (never the economic wear); 0 when there is no monetary marginal
        Info("[UnifiedAuto][Execute] plan=" + std::to_string(id2) + " " + phaseText + " " + CommodityLabel(c) + " " + PortfolioActionKindName(act2.kind) + " " + act2.write.reason +
             " preState: amount=" + std::to_string(act2.write.expectedAmount) + " semantic key (type, neighbor)=(" + std::to_string(act2.write.key.type) + "," + std::to_string(act2.write.key.neighbor) + ")");
        pendingDeals_[CommodityIndex(c)] = std::move(p);
        PumpDealTx(CommodityIndex(c), act, true);      // a deal phase only starts with the master and the commodity's contract permission on
    }

    static bool SoftDealState(DealWriteState s) {
        return s == DealWriteState::PreconditionChanged || s == DealWriteState::TargetMissing || s == DealWriteState::TargetAmbiguous ||
               s == DealWriteState::TargetSuspended || s == DealWriteState::QuantityNotLegal || s == DealWriteState::VanillaRejected ||
               s == DealWriteState::InvalidSnapshot || s == DealWriteState::AlreadyInState;
    }

    // `mayWrite` = the master (AutoDeals) and this commodity's contract permission (AutoTrade*) are on NOW. With either off, a transaction that is only waiting for the accounting to reconcile (readback) is still observed
    // until it concludes (reads only: no primitive can come out of it); one that still has a primitive to issue is closed WITHOUT issuing it: nothing creates, changes or
    // cancels a contract after the opt-out. The contract stays as the game has it; the plans account it from fresh reads.
    void PumpDealTx(int ci, IUnifiedActuator& act, bool mayWrite) {
        auto& p = *pendingDeals_[ci];
        if (!mayWrite && !p.machine->readbackOnly()) {
            const auto& k = p.action.write.key;
            Warn("[UnifiedAuto][Deal] SUSPENDED_BY_OPT_OUT plan=" + std::to_string(p.planId) + " " + DealText(k) + " operation=" + DealWriteOpName(p.action.write.op) + " state=" +
                 DealWriteStateName(p.machine->state()) + " primitivesIssued=" + std::to_string(p.machine->primitivesIssued()) + ": AutoDeals or this commodity's contract permission is off, no further primitive is issued; " +
                 "the contract is left as the game has it and is accounted from fresh reads (" + p.machine->reason() + ")");
            ledger_.pending = SatSub(ledger_.pending, p.action.cashMarginal);
            pendingDeals_[ci].reset();
            return;
        }
        for (int i = 0; i < 6; ++i) {
            const DealStep st = act.PumpDeal(*p.machine, epoch_);
            if (st.primitive == DealPrimitive::None) break;
        }
        const auto state = p.machine->state();
        const std::string pid = std::to_string(p.planId);
        if (!IsTerminal(state)) {
            Info(std::string("[UnifiedAuto][Deal] DEAL_PENDING_READBACK plan=") + pid + " " + DealWriteStateName(state) + ": " + p.machine->reason() + " (this contract and its commodity are untouched until the accounting reconciles)");
            return;
        }
        const PortfolioAction done = p.action;
        const DealCausal causal = p.causal;
        const std::string preRow = p.preRow, preRegional = p.preRegional;
        const Commodity doneCommodity = p.commodity;
        const bool netKnown = p.netKnown;
        const std::int64_t netBefore = p.netBefore;
        const std::string why = p.machine->reason();
        const std::int32_t effectiveTarget = p.machine->snappedFrom() > 0 ? p.machine->effectiveTarget() : done.write.targetAmount;   // a new garbage import may have been written below the plan (largest legal quantity)
        ledger_.pending = SatSub(ledger_.pending, done.cashMarginal);
        pendingDeals_[ci].reset();
        if (state == DealWriteState::Applied) {
            ++ledger_.dealTransactions;
            UtilitySnapshot after;
            std::int64_t delta = done.marginalKnown ? done.cashMarginal : 0;   // no monetary marginal: the model fallback claims nothing
            bool known = false;
            if (act.ReadForPlan(after)) {
                const std::int64_t net = NetOf(after, known);
                if (known && netKnown) delta = net - netBefore;
            }
            ledger_.realizedDealNet = SatAdd(ledger_.realizedDealNet, delta);
            if ((doneCommodity == Commodity::Power || doneCommodity == Commodity::Water) && LocalIsBuyer(done.write.key.type) && done.write.op != DealWriteOp::Cancel) {
                // a purchase was written: the service it was written for is judged on the first authoritative update that follows (never on the write itself)
                ServiceCounters now;
                if (!act.ReadServiceCounters(now)) now = lastCounters_;
                std::int64_t contracted = 0;
                if (!after.deals.deals.empty()) contracted = CommittedAmount(after.deals.deals, done.write.key.type);
                monitor_.ExpectProof(doneCommodity == Commodity::Power, contracted, now);
            }
            {
                const std::string akey = std::to_string(done.write.key.type) + '/' + std::to_string(done.write.key.neighbor);
                if (done.write.op == DealWriteOp::Create) createdHere_.insert(akey);
                else if (done.write.op == DealWriteOp::Cancel) { createdHere_.erase(akey); auditSig_.erase(akey); }
            }
            if (known) contractTable_ = ContractTableOf(after);       // the table of the shutdown line follows every verified deal write, not only the plans
            tracker_.OnDealApplied(done.write.op == DealWriteOp::Cancel, done.write.key.type, done.write.key.neighbor, done.write.expectedAmount,
                                   done.write.op == DealWriteOp::Cancel ? 0 : effectiveTarget, causal);
            { const auto diag = tracker_.TakeDiagnostics(); if (validation_) for (const auto& l : diag) Info(l); }
            Info("[DealTx][PostWrite] plan=" + pid + " commodity=" + CommodityName(doneCommodity) + " type=" + DealTypeName(done.write.key.type) + " neighbor=" + std::to_string(done.write.key.neighbor) + " result=APPLIED before{" +
                 preRow + ' ' + preRegional + "} after{" + (known ? RowText(after, done.write.key) + ' ' + RegionalText(after, doneCommodity) : std::string("simulator totals unreadable")) + "} measuredMonthlyDelta=" + std::to_string(delta));
            lastWrite_ = "plan " + pid + " " + DealText(done.write.key) + " " + std::to_string(done.write.expectedAmount) + "->" + std::to_string(done.write.op == DealWriteOp::Cancel ? 0 : done.write.targetAmount);
            Info("[UnifiedAuto][Deal] APPLIED plan=" + pid + " " + done.write.reason + " measuredMonthlyDelta=" + std::to_string(delta) + (known ? " (simulator income - expense)" : " (model: simulator totals unreadable)"));
            Info("[UnifiedAuto][Verified] plan=" + pid + " " + DealText(done.write.key) + " flags, amount, stored cost, RegionalCity import/export and simulator money reconcile with the deal list (" + why + "); the next cycle replans from a fresh snapshot");
            return;
        }
        if (done.write.op == DealWriteOp::Cancel && done.write.expectedAmount == 0) {
            // the cleanup of a dormant 1 $/month row is an optional saving: if the game does not accept it, give up on THAT row for this city session (never retried, never a fail-closed of the deals)
            dormantGiveUp_.push_back(done.write.key);
            Warn(std::string("[UnifiedAuto][Deal] dormant contract cleanup refused (") + DealWriteStateName(state) + ") plan=" + pid + ": " + why + "; this row is left as it is for this city session");
            return;
        }
        if (SoftDealState(state)) {
            tracker_.OnDealRefused(done.write.key.type, done.write.key.neighbor, static_cast<int>(done.write.op), done.write.targetAmount);
            if (done.write.op == DealWriteOp::Create) {
                // the game would not create it: leave it out of the next plans for a while, otherwise the same plan is rebuilt every cycle and its funding phases never run
                refusedNew_.erase(std::remove_if(refusedNew_.begin(), refusedNew_.end(), [&](const RefusedNew& r) { return r.key == done.write.key; }), refusedNew_.end());
                refusedNew_.push_back({done.write.key, months_});
            }
            Warn(std::string("[UnifiedAuto][Deal] ") + DealWriteStateName(state) + " plan=" + pid + ": " + why + "; skipped, replanning from fresh state");
            return;
        }
        tracker_.OnFailedWrite("deal fail-closed: " + why);
        Err(std::string("[UnifiedAuto][Deal] FAIL CLOSED (technical): ") + DealWriteStateName(state) + ": " + why + "; Deal Auto disabled for this city session (local utilities replan with the active-contract accounting)");
        deals_off_ = {true, why};
    }

    UtilityAutoController legacy_;
    FinalValidationTracker tracker_;
    LandfillGeometryTracker landfill_;
    LandfillAdvisorEngine landfillAdvisor_;
    LandfillGhostEngine landfillGhosts_;
    std::string lastLoggedDay_, prevModes_, prevGarbageTarget_, context_, lastWrite_;
    bool decisionLoggedOnce_ = false;
    std::uint32_t prevPlanId_ = 0;
    DealCausal prevCausal_;
    std::uint32_t days_ = 0, months_ = 0, epoch_ = 0, planId_ = 0, curPlanId_ = 0;
    Off garbageOff_, deals_off_;
    std::vector<DealKey> dormantGiveUp_;                // dormant rows the game refused to cancel (this city session)
    struct RefusedNew { DealKey key; std::uint32_t month; };
    static constexpr std::uint32_t kRefusalMonths = 3;  // a refused new contract is not planned again for this many months (the simulator's gates move with treasury, demand and connections)
    std::vector<RefusedNew> refusedNew_;                // new contracts the simulator refused (this city session)
    bool haveSanWrite_ = false, pacedLogged_ = false;   // Sanitation funding pacing (SanitationPacing.h): the day of the previous write, and the throttle of the "deferred" line
    std::uint32_t lastSanWriteDay_ = 0, lastPacedLogDay_ = 0;
    bool envLogged_ = false, lastPowerOff_ = false, lastWaterOff_ = false;
    bool buyAllowed_[3] = {true, true, true};                 // the Department manages the regional contracts of each commodity (power, water, garbage): only for the wording of a shortage notice
    bool powerEvaluated_ = false, waterEvaluated_ = false;   // a full evaluation of this commodity with READY readings has happened in this city
    std::uint32_t lastWaterDay_ = 0;                          // the day (of this city session) of the last full evaluation of the water
    struct ReadinessState { bool ok = false; bool known = false; std::string reason; };
    ReadinessState ready_[3];                                 // the verdict of the latest full snapshot per commodity (power, water, garbage)
    bool validation_ = false, verbose_ = false;
    ServiceMonitor monitor_;
    NoticeEpisodes episodes_;
    std::string contractTable_ = "none";
    bool contractTableLogged_ = false;
    std::set<std::string> createdHere_;                 // contracts created by this plugin since the city loaded (diagnosis only)
    std::map<std::string, std::string> auditSig_;        // last [ContractAudit] line per contract
    bool servedOk_[2] = {false, false};          // the latest fresh authoritative sample shows the service delivered
    bool internalLogged_[2] = {false, false};
    ServiceCounters lastCounters_;
    std::size_t serviceChecks_ = 0;
    std::unique_ptr<PendingDeal> pendingDeals_[3];     // power, water, garbage: one in-flight deal transaction per commodity
    static int CommodityIndex(Commodity c) { return c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : 2; }
    bool AnyDealPending() const { return pendingDeals_[0] || pendingDeals_[1] || pendingDeals_[2]; }
    std::unique_ptr<PendingSan> pendingSan_;
    UnifiedLedger ledger_;
    std::string lastConfig_, lastPlanText_;
    std::string workText_;                      // diagnostics of the current cycle's cost (read / plan time, garbage plans and evaluations), appended to its [Plan] line
    static std::string FormatMs(double v) { std::ostringstream o; o << std::fixed << std::setprecision(1) << v; return o.str(); }
    std::vector<AutoLine> lines_;
};

} // namespace sc4af
