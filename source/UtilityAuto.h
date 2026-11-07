// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Utility Auto: pure control loop (no game types). Policy: aggressive micromanagement.
//
//   READ (authoritative snapshot) -> VALIDATE -> OPTIMIZE -> REVALIDATE (fresh re-read) -> WRITE -> VERIFY
//
// Scope: power plant funding and the global water funding ONLY. Garbage funding and neighbor deals are handled by the unified
// controller. The game-facing part is an IUtilityActuator supplied by the plugin.
//
// Policy
//  * Reserves are configurable floors (defaults: power 20%, water 10%, garbage 2%); funding jumps straight to the optimum: no step limit,
//    no smoothing, no post-write rollback watch. Aging is diagnostic while current physical constraints remain binding.
//  * Every cycle plans from the real state. The controller keeps no history it needs to understand the city, so a save/reload or a
//    manual player change is simply the next input. It never restores an old value because that value "was safe".
//  * Service loss is answered by planning: the required capacity is recomputed from the current state and funding rises directly. The number of
//    unserved buildings never suspends a subsystem and never holds a saving: a building can lack power or water only because the player did not
//    connect it. Whether the city is short is decided by the game's own indicators (demand, service delivered, capacity,
//    usable imports, contracted exports): the plan, and ServiceMonitor for the fast check.
//  * Transactional safety is kept: snapshot validity, lifecycle epoch, fresh re-read, compare-and-set, legal grid, read-back
//    verification. A write that fails verification is a technical failure: restore what this cycle changed and fail closed.
//  * Subsystem-local: a failure disables only that subsystem for the session (clean reload or leaving Auto resets it).

#include "ServiceReadiness.h"
#include "UnifiedPlan.h"
#include "UtilityConfig.h"
#include "UtilityHealth.h"
#include "UtilityOptimizer.h"
#include "UtilityPlanFormat.h"
#include "UtilityWriterPlan.h"

#include <algorithm>
#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace sc4af {

inline OptimizerConfig MakeOptimizerConfig(const UtilityResearchConfig& c) {
    OptimizerConfig cfg;
    cfg.powerReserve = c.powerReserve;
    cfg.waterReserve = c.waterReserve;
    cfg.garbageReserve = c.garbageReserve;
    cfg.considerPowerAging = c.powerAgingActive();
    cfg.protectWaterPipes = c.waterPipeProtectionActive();
    cfg.allowUtilityArbitrage = c.utilityArbitrageActive();
    cfg.countImports = c.countImports;
    cfg.importSafeDays = c.importSafeDays;
    cfg.minContractBenefit = c.minContractBenefit;
    cfg.waterImportSafeDays = c.waterImportSafeDays;
    cfg.healthHorizonMonths = c.healthHorizonMonths;
    cfg.waterMinPercent = c.waterMinPercent;
    // The export reserve keeps spare capacity for power sales the controller could create. It needs the proposals, so it exists only where proposals can be
    // discovered (DealDiscoveryAllowed) and where the Department may create the sale (AutoTradePower): with the deal controller or the electricity contracts off, no sale will ever be created and the unread
    // proposals are not read as "no opportunity".
    cfg.exportReserve = c.exportReserve && DealDiscoveryAllowed(c) && c.tradePower;
    cfg.useRequirement = c.useRequirement;
    return cfg;
}

// The unified controller's view of the two independent permissions of each commodity:
//  * CONTRACTS (AutoDeals master + AutoTradePower / AutoTradeWater / AutoTradeGarbage): which commodities enter the deal portfolio. A commodity left out is neither evaluated nor given deal actions: its real
//    contracts are only READ and counted as fixed commitments by the active-contract accounting of the local plans (UnifiedPlan: portfolioUsed == false), never optimised away, in either direction.
//  * FUNDING (AutoPower / AutoWater / AutoGarbage): a commodity whose manager is off has its funding frozen in the plans (a plan never relies on a funding change that will not be executed).
// Neither permission needs the other.
// A funding writer that failed its verification (fail-closed) freezes only that commodity's FUNDING, as if its manager were off; the contracts of the commodity go on.
inline OptimizerConfig MakeUnifiedOptimizerConfig(const UtilityResearchConfig& c, bool powerFundingFailed = false, bool waterFundingFailed = false, bool garbageFundingFailed = false) {
    OptimizerConfig o = MakeOptimizerConfig(c);
    o.freezePower = !c.autoPower || powerFundingFailed;
    o.freezeWater = !c.autoWater || waterFundingFailed;
    o.freezeGarbage = !c.autoGarbage || garbageFundingFailed;
    return o;
}

// ---- savings accounting: realized changes never mix with read-only opportunities -------------------------------------
struct SavingsLedger {
    std::int64_t realizedPower = 0, realizedWater = 0;   // monthly run-rate saving actually executed by Auto (positive = cheaper)
    int writes = 0;
    std::int64_t readOnlyOpportunity = 0;                // monthly value of READ_ONLY deal recommendations (NOT realized)
    std::int64_t realized() const { return realizedPower + realizedWater; }
    std::string Format() const {
        std::ostringstream o;
        o << "[UtilityAuto][Savings] REALIZED monthlyRunRate=" << realized() << " (power=" << realizedPower << " water=" << realizedWater
          << ") writes=" << writes << " | POTENTIAL (NOT REALIZED; deal writer disabled) opportunity=" << readOnlyOpportunity;
        return o.str();
    }
};

// ---- game-facing seam ---------------------------------------------------------------------------------------------
struct ActuatorResult {
    UtilityWriteResult result = UtilityWriteResult::NotAvailable;
    std::string detail;
};

class IUtilityActuator {
public:
    virtual ~IUtilityActuator() = default;
    // Fresh authoritative re-read (same lifecycle epoch). false = unavailable.
    virtual bool Reread(UtilitySnapshot& out) = 0;
    // Structural re-acquire + compare-and-set + immediate read-back verification. `after` receives the post-write snapshot.
    virtual ActuatorResult WritePlant(int cellX, int cellZ, int fromPercent, int toPercent, UtilitySnapshot& after) = 0;
    virtual ActuatorResult WriteWater(int fromPercent, int toPercent, UtilitySnapshot& after) = 0;
};

struct AutoLine {
    int level = 0;   // 0 info, 1 warning, 2 error
    std::string text;
};

class UtilityAutoController {
public:
    // City (re)loaded or unloaded: forget everything ("clean city reload" re-enables a subsystem that failed closed).
    void ResetCity() {
        monthsInCity_ = 0;
        power_ = Sub{};
        water_ = Sub{};
        ledger_ = SavingsLedger{};
        lastConfigText_.clear();
    }

    // Both subsystems in one call (monthly fallback).
    void OnMonth(const UtilitySnapshot& snap, const UtilityResearchConfig& cfg, IUtilityActuator& act) {
        lines_.clear();
        ++monthsInCity_;
        if (!Gate(cfg)) return;
        RunCycle(snap, cfg, act, true, true, nullptr);
    }

    // Water follows the simulation month: capacity readings answer a funding change one month later, so a new plan is only
    // meaningful once the next monthly reading exists. This is data latency, not smoothing.
    void OnWaterCycle(const UtilitySnapshot& snap, const UtilityResearchConfig& cfg, IUtilityActuator& act) {
        lines_.clear();
        ++monthsInCity_;
        if (!Gate(cfg)) return;
        RunCycle(snap, cfg, act, false, true, nullptr);
    }

    // Power is evaluated every PowerCadenceDays simulation days (SimNewDay). `powerPlan` may carry the plan the caller already
    // computed (with the deal-offer suppression applied); otherwise it is computed here.
    void OnPowerCycle(const UtilitySnapshot& snap, const UtilityResearchConfig& cfg, IUtilityActuator& act, const SubsystemPlan* powerPlan = nullptr) {
        lines_.clear();
        if (!Gate(cfg)) return;
        RunCycle(snap, cfg, act, true, false, powerPlan);
    }

    // Used by the unified controller: execute ONE already-chosen (and already phase-filtered) Power / Water plan with the same
    // revalidation, compare-and-set, readback and fail-closed logic, without the gate or the plan step.
    void ExecutePowerPlan(const UtilitySnapshot& snap, const SubsystemPlan& sp, const UtilityResearchConfig& cfg, IUtilityActuator& act) { RunPower(snap, sp, cfg, act); }
    void ExecuteWaterPlan(const UtilitySnapshot& snap, const SubsystemPlan& sp, const UtilityResearchConfig& cfg, IUtilityActuator& act) { RunWater(snap, sp, cfg, act); }
    void CountMonth() { ++monthsInCity_; }
    std::vector<AutoLine> TakeLines() { std::vector<AutoLine> out; out.swap(lines_); return out; }

    const std::vector<AutoLine>& lines() const { return lines_; }
    bool powerDisabled() const { return power_.disabled; }
    bool waterDisabled() const { return water_.disabled; }
    const SavingsLedger& ledger() const { return ledger_; }
    std::uint32_t monthsInCity() const { return monthsInCity_; }

private:
    struct Restore { int x = 0, z = 0, from = 0, to = 0; };
    struct Sub {
        bool disabled = false;
        std::string reason;
    };

    bool Gate(const UtilityResearchConfig& cfg) {
        // The read-only recorder runtime (phase8Runtime) never writes, even if a stale configuration is armed.
        // This is a controller-level check in addition to the Mode=Off runtime file.
        if (cfg.phase8Runtime || cfg.mode != UtilityMode::Auto || !cfg.enabled) {
            if (power_.disabled || water_.disabled || !lastConfigText_.empty()) {
                power_ = Sub{};   // leaving Auto is an explicit reset
                water_ = Sub{};
                lastConfigText_.clear();
            }
            return false;
        }
        const std::string cfgText = ConfigText(cfg);
        if (cfgText != lastConfigText_) { Info("[UtilityAuto] " + cfgText); lastConfigText_ = cfgText; }
        return true;   // no fixed wait after a city load; RunPower / RunWater act only on ready readings (ServiceReadiness.h)
    }

    void RunCycle(const UtilitySnapshot& snap, const UtilityResearchConfig& cfg, IUtilityActuator& act, bool doPower, bool doWater, const SubsystemPlan* powerPlan) {
        const OptimizerConfig opt = MakeOptimizerConfig(cfg);
        const UtilityTuning tuning = VanillaUtilityTuning();
        UtilityPlan plan = PlanUtilities(snap, opt, tuning);
        if (powerPlan) plan.power = *powerPlan;
        ledger_.readOnlyOpportunity = 0;
        for (const auto& c : plan.power.cards)
            if (c.calculable && c.profit > 0) ledger_.readOnlyOpportunity += c.profit;
        if (doPower && cfg.autoPower) RunPower(snap, plan.power, cfg, act);
        if (doWater && cfg.autoWater) RunWater(snap, plan.water, cfg, act);
        Info(ledger_.Format());
        if (doPower)
            for (const auto& c : plan.power.cards) Info("[DealCard] " + FormatDealCard(c, cfg.debug));
        if (doWater)
            for (const auto& c : plan.water.cards) Info("[DealCard] " + FormatDealCard(c, cfg.debug));
    }

    void Info(std::string t) { lines_.push_back({0, std::move(t)}); }
    void Warn(std::string t) { lines_.push_back({1, std::move(t)}); }
    void Err(std::string t) { lines_.push_back({2, std::move(t)}); }

    static std::string ConfigText(const UtilityResearchConfig& c) {
        std::ostringstream o;
        o << "config Mode=" << UtilityModeName(c.mode) << " PowerAuto=" << (c.autoPower ? "On" : "Off") << " WaterAuto=" << (c.autoWater ? "On" : "Off")
          << " Sanitation/Deals=ReadOnly(normal build) " << PolicyText(c)
          << " PowerCadenceDays=" << c.powerCadenceDays << " (reserves are floors; direct funding, no step limit, no rollback watch)";
        return o.str();
    }

    static const PowerPlantRow* FindPlant(const UtilitySnapshot& s, int x, int z) {
        for (const auto& p : s.power.plants)
            if (p.ref.cellX == x && p.ref.cellZ == z) return &p;
        return nullptr;
    }

    void RestoreAll(bool power, const std::vector<Restore>& restores, IUtilityActuator& act) {
        for (auto it = restores.rbegin(); it != restores.rend(); ++it) {
            UtilitySnapshot after;
            const ActuatorResult r = power ? act.WritePlant(it->x, it->z, it->to, it->from, after) : act.WriteWater(it->to, it->from, after);
            const bool ok = r.result == UtilityWriteResult::Applied || r.result == UtilityWriteResult::AlreadyInState;
            lines_.push_back({ok ? 0 : 2, std::string("[UtilityAuto][") + (power ? "Power" : "Water") + "][Rollback] " +
                                              (power ? "plant@" + std::to_string(it->x) + ',' + std::to_string(it->z) : std::string("department")) + ' ' + std::to_string(it->to) +
                                              "% -> " + std::to_string(it->from) + "% (undoing this cycle's writes after a technical failure) result=" + UtilityWriteResultName(r.result) +
                                              (r.detail.empty() ? "" : " (" + r.detail + ")")});
        }
    }

    void FailClosed(bool power, const std::string& reason, std::vector<Restore> applied, IUtilityActuator& act) {
        Sub& S = power ? power_ : water_;
        Err(std::string("[UtilityAuto][") + (power ? "Power" : "Water") + "] FAIL CLOSED (technical): " + reason + "; undoing what this cycle changed and disabling " +
            (power ? "Power" : "Water") + " Auto for this city session");
        RestoreAll(power, applied, act);
        S.disabled = true;
        S.reason = reason;
    }

    static bool SoftFailure(UtilityWriteResult r) {
        return r == UtilityWriteResult::PreconditionChanged || r == UtilityWriteResult::NotEligible || r == UtilityWriteResult::AlreadyInState ||
               r == UtilityWriteResult::TargetMissing || r == UtilityWriteResult::TargetAmbiguous || r == UtilityWriteResult::NotAvailable ||
               r == UtilityWriteResult::UnsupportedMechanism;
    }

    void RunPower(const UtilitySnapshot& snap, const SubsystemPlan& sp, const UtilityResearchConfig& cfg, IUtilityActuator& act) {
        Sub& S = power_;
        if (S.disabled) return;
        if (snap.power.validity == SnapshotValidity::ValidEmpty) return;   // nothing to control: not an error
        if (snap.power.validity != SnapshotValidity::Valid && snap.power.validity != SnapshotValidity::Partial) {
            Warn("[UtilityAuto][Power] INVALID_SNAPSHOT (" + snap.power.invalidReason + "): zero writes, retry next cycle");
            return;
        }
        if (const Readiness rd = PowerReadiness(snap); !rd) { Info("[UtilityAuto][Power] readings not ready (" + rd.reason + "): zero writes"); return; }
        const UtilityTuning tuning = VanillaUtilityTuning();
        const OptimizerConfig opt = MakeOptimizerConfig(cfg);
        {
            std::ostringstream o;
            const auto t = SummarizePowerTransition(snap, sp);
            o << "[UtilityAuto][Power] month=" << monthsInCity_ << " ownTarget=" << sp.ownTarget << " contractedExports=" << sp.contractedExports << " dependableImports=" << sp.dependableImports
              << " exportReserve=" << sp.exportReserve << " required=" << sp.required << " currentCost=" << sp.currentExpense << " targetCost=" << sp.targetExpense << " saving=" << sp.saving()
              << " actions=" << sp.actions.size() << " status=" << PlanStatusName(sp.status) << " capacityMargin=" << (sp.availableAfter - sp.required)
              << " capacity=" << sp.availableNow << "->" << sp.availableAfter << " (" << t.capacityDeltaPercent << "%)";
            if (t.departmentFundingBefore && t.departmentFundingAfter)
                o << " deptFunding=" << *t.departmentFundingBefore << "->" << *t.departmentFundingAfter << " poleCurve=" << tuning.poleDecay.Eval(*t.departmentFundingAfter)
                  << " networkRepair=" << (tuning.poleDecay.Eval(*t.departmentFundingAfter) >= opt.poleMinCurveValue ? "OK" : "BELOW_FLOOR");
            for (const auto& n : sp.notes) o << " note=\"" << n << '"';
            Info(o.str());
        }
        std::vector<PlanAction> todo;
        for (const auto& a : sp.actions) {
            if (a.kind != ActionKind::SetPlantFunding || a.capability != ActionCapability::Executable) continue;
            const bool reduction = a.toPercent < a.fromPercent;
            if (reduction && sp.status != PlanStatus::Optimal) continue;
            todo.push_back(a);
        }
        if (todo.empty()) return;
        // increases first, then reductions in cell order: every intermediate state has at least the capacity of the final one
        std::stable_sort(todo.begin(), todo.end(), [](const PlanAction& a, const PlanAction& b) {
            const bool ra = a.toPercent < a.fromPercent, rb = b.toPercent < b.fromPercent;
            return ra != rb ? !ra : std::tie(a.cellX, a.cellZ) < std::tie(b.cellX, b.cellZ);
        });
        UtilitySnapshot fresh;
        if (!act.Reread(fresh) || fresh.lifecycleEpoch != snap.lifecycleEpoch ||
            (fresh.power.validity != SnapshotValidity::Valid && fresh.power.validity != SnapshotValidity::Partial)) {
            Warn("[UtilityAuto][Power] revalidation read unavailable or from another lifecycle: zero writes");
            return;
        }
        // The fresh read must still contain each planned plant at the planned input funding (compare-and-set at plan level); the actuator
        // repeats the check structurally right before the setter.
        std::vector<PlanAction> confirmed;
        for (const auto& a : todo) {
            const auto* row = FindPlant(fresh, a.cellX, a.cellZ);
            if (row && row->funding && UtilityPercentRound(*row->funding) == a.fromPercent) confirmed.push_back(a);
            else Warn(std::string("[UtilityAuto][Power] PreconditionChanged: plant@") + std::to_string(a.cellX) + ',' + std::to_string(a.cellZ) + " is no longer at the planned funding; skipped");
        }
        std::vector<Restore> applied;
        std::int64_t expectedDelta = 0;
        UtilitySnapshot last = fresh;
        for (const auto& a : confirmed) {
            UtilitySnapshot after;
            const ActuatorResult r = act.WritePlant(a.cellX, a.cellZ, a.fromPercent, a.toPercent, after);
            std::ostringstream o;
            o << "[UtilityAuto][Write] Power plant@" << a.cellX << ',' << a.cellZ << ' ' << a.fromPercent << " -> " << a.toPercent << " result="
              << UtilityWriteResultName(r.result) << (r.detail.empty() ? "" : " (" + r.detail + ")");
            if (r.result == UtilityWriteResult::Applied) {
                Info(o.str() + " verified=true");
                applied.push_back({a.cellX, a.cellZ, a.fromPercent, a.toPercent});
                expectedDelta += a.monthlyDelta;
                last = std::move(after);
                continue;
            }
            if (SoftFailure(r.result)) { Warn(o.str() + " verified=n/a; skipped"); continue; }
            Err(o.str() + " verified=false");
            FailClosed(true, std::string("write verification failed (") + UtilityWriteResultName(r.result) + ")", applied, act);
            return;
        }
        if (applied.empty()) return;
        ++ledger_.writes;
        // Realized saving from the authoritative charged department expense (nominal cost x funding), not from capacity.
        std::int64_t measured = expectedDelta;
        std::string source = "model";
        if (snap.power.department.totalExpenses && last.power.department.totalExpenses) {
            measured = *last.power.department.totalExpenses - *snap.power.department.totalExpenses;
            source = "department";
        }
        ledger_.realizedPower += -measured;
        Info("[UtilityAuto][Power] applied=" + std::to_string(applied.size()) + " expectedDelta=" + std::to_string(expectedDelta) + " measuredDelta=" + std::to_string(measured) +
             " (" + source + ") deptExpenses=" + (last.power.department.totalExpenses ? std::to_string(*last.power.department.totalExpenses) : "?"));
    }

    void RunWater(const UtilitySnapshot& snap, const SubsystemPlan& sp, const UtilityResearchConfig& cfg, IUtilityActuator& act) {
        Sub& S = water_;
        if (S.disabled) return;
        if (snap.water.validity == SnapshotValidity::ValidEmpty) return;
        if (snap.water.validity != SnapshotValidity::Valid && snap.water.validity != SnapshotValidity::Partial) {
            Warn("[UtilityAuto][Water] INVALID_SNAPSHOT (" + snap.water.invalidReason + "): zero writes, retry next cycle");
            return;
        }
        if (const Readiness rd = WaterReadiness(snap); !rd) { Info("[UtilityAuto][Water] readings not ready (" + rd.reason + "): zero writes"); return; }
        const UtilityTuning tuning = VanillaUtilityTuning();
        {
            std::ostringstream o;
            const auto& w = snap.water;
            const auto ex = ContractedExportsOf(snap.deals.deals, std::nullopt, w.gameExported, std::nullopt).water;
            o << "[UtilityAuto][Water] month=" << monthsInCity_ << " funding=" << (w.department.funding ? UtilityPercentRound(*w.department.funding) : -1) << "% status=" << PlanStatusName(sp.status)
              << " currentCost=" << sp.currentExpense << " targetCost=" << sp.targetExpense << " saving=" << sp.saving() << " capacityRequirement=" << sp.required
              << " capacity=" << sp.availableNow << "->" << sp.availableAfter << " contractedExports=" << ex << " pipeCurve="
              << (w.department.funding ? tuning.pipeDecay.Eval(UtilityPercentRound(*w.department.funding)) : 0.0) << " pipeConstraint=no-net-decay";
            for (const auto& n : sp.notes) o << " note=\"" << n << '"';
            Info(o.str());
        }
        if (sp.status != PlanStatus::Optimal && sp.status != PlanStatus::InsufficientCapacity) return;
        const PlanAction* action = nullptr;
        for (const auto& x : sp.actions)
            if (x.kind == ActionKind::SetWaterFunding && x.capability == ActionCapability::Executable) action = &x;
        if (!action) return;
        if (action->toPercent < action->fromPercent && sp.status != PlanStatus::Optimal) return;
        UtilitySnapshot fresh;
        if (!act.Reread(fresh) || fresh.lifecycleEpoch != snap.lifecycleEpoch ||
            (fresh.water.validity != SnapshotValidity::Valid && fresh.water.validity != SnapshotValidity::Partial)) {
            Warn("[UtilityAuto][Water] revalidation read unavailable or from another lifecycle: zero writes");
            return;
        }
        if (!fresh.water.department.funding || UtilityPercentRound(*fresh.water.department.funding) != action->fromPercent) {
            Warn("[UtilityAuto][Water] PreconditionChanged: department funding is no longer the planned input; skipped");
            return;
        }
        UtilitySnapshot after;
        const ActuatorResult r = act.WriteWater(action->fromPercent, action->toPercent, after);
        std::ostringstream o;
        o << "[UtilityAuto][Write] Water department " << action->fromPercent << " -> " << action->toPercent << " result=" << UtilityWriteResultName(r.result)
          << (r.detail.empty() ? "" : " (" + r.detail + ")");
        if (r.result != UtilityWriteResult::Applied) {
            if (SoftFailure(r.result)) { Warn(o.str() + " verified=n/a; skipped"); return; }
            Err(o.str() + " verified=false");
            FailClosed(false, std::string("write verification failed (") + UtilityWriteResultName(r.result) + ")", {{0, 0, action->fromPercent, action->toPercent}}, act);
            return;
        }
        Info(o.str() + " verified=true");
        ++ledger_.writes;
        std::int64_t measured = action->monthlyDelta;
        std::string source = "model";
        if (snap.water.department.totalExpenses && after.water.department.totalExpenses) {
            measured = *after.water.department.totalExpenses - *snap.water.department.totalExpenses;
            source = "department";
        }
        ledger_.realizedWater += -measured;
        Info("[UtilityAuto][Water] applied expectedDelta=" + std::to_string(action->monthlyDelta) + " measuredDelta=" + std::to_string(measured) + " (" + source +
             ") deptExpenses=" + (after.water.department.totalExpenses ? std::to_string(*after.water.department.totalExpenses) : "?"));
    }

    std::uint32_t monthsInCity_ = 0;
    Sub power_, water_;
    SavingsLedger ledger_;
    std::string lastConfigText_;
    std::vector<AutoLine> lines_;
};

} // namespace sc4af
