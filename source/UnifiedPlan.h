// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// ONE planning pass for the whole system: Power, Water, Sanitation and the six-direction Neighbor Deal portfolio, from the SAME
// authoritative snapshot. Used by Shadow and by the unified Auto controller, so what Shadow logs is exactly what Auto would do
// before it executes. Pure: no game types, no writes.
//
//   portfolio (per commodity)  ->  FINAL deal totals S (sell-like) and B (buy-like)
//   local plan (per commodity) ->  required capacity = own target + S - B   (the active-contract accounting is REPLACED, never added)
//   actions                    ->  per commodity, ordered by the phases of DealPortfolio.h
//
// The portfolio is the authority for deal decisions; the local solvers (power plants, water department, Sanitation) stay the authority
// for operating expense, health and service constraints.

#include "DealPortfolio.h"
#include "GarbageControl.h"
#include "SafeMoney.h"
#include "UtilityConfig.h"
#include "UtilityOptimizer.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <sstream>
#include <tuple>
#include <string>
#include <vector>

namespace sc4af {

enum class UnifiedActionKind { PlantFunding, WaterFunding, SanitationFunding, Deal };

struct UnifiedAction {
    UnifiedActionKind kind = UnifiedActionKind::PlantFunding;
    Commodity commodity = Commodity::None;
    int phase = 0;
    PlanAction funding;               // plant / water
    GarbageFundingWritePlan sanitation;   // sanitation
    PortfolioAction deal;             // deal
};

struct UnifiedOptions {
    bool includeDeals = true;                 // false: local plans use the active-contract accounting and no deal action exists
    PortfolioScope dealScope;                 // commodities whose deals may be planned (a failed subsystem excludes its own)
};

struct UnifiedPlan {
    SubsystemPlan power, water;
    GarbageFundingPlan garbage;
    PortfolioPlan portfolio;
    bool portfolioUsed[3] = {false, false, false};     // power, water, garbage: the local plan carries the portfolio's final totals
    DealCommitment commitment[3];
    std::vector<PortfolioChoice> garbageTargets;       // the garbage contracts the portfolio ends with, contract by contract (the local plan projects each one)
    std::vector<UnifiedAction> actions;                // per commodity ordered by phase; the controller picks the lowest phase per commodity
    std::vector<std::string> notes;
    // CASH figures (what the simulator will show; the ledger and the measured deltas only ever see these) and their ECONOMIC counterparts (Power with
    // ConsiderPowerAging also pays the capital wear of the plants; Water and Garbage have none, so they coincide).
    std::int64_t localSaving = 0;                      // cash: currentExpense - targetExpense over power, water and sanitation
    std::int64_t localEconomicSaving = 0;              // economic: the same including the wear of the power plants
    // portfolio optimum minus keeping the current contracts, ONLY over the commodities where both are feasible (real money, always finite).
    // A commodity whose current portfolio is infeasible has no monetary comparison: it is listed in keepInfeasible[] and the plan there is a
    // restoration of feasibility, not a gain.
    std::int64_t dealGain = 0;                         // cash
    std::int64_t dealGainEconomic = 0;
    bool keepInfeasible[3] = {false, false, false};    // power, water, garbage
    bool anyKeepInfeasible() const { return keepInfeasible[0] || keepInfeasible[1] || keepInfeasible[2]; }
    std::int64_t expectedMonthlyResult() const { return SatAdd(localSaving, dealGain); }                       // cash
    std::int64_t expectedEconomicResult() const { return SatAdd(localEconomicSaving, dealGainEconomic); }
    GarbageService garbageServiceNow;
};

inline DealCommitment CurrentCommitment(const UtilitySnapshot& s, Commodity c) {
    DealCommitment k;
    for (const auto& d : s.deals.deals) {
        if (!d.amount || CommodityOfDealType(d.type) != c || !DealIsCommitted(d)) continue;
        (LocalIsBuyer(d.type) ? k.buy : k.sell) += *d.amount;
    }
    return k;
}

inline int CommodityIndexOf(Commodity c) { return c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : 2; }

// Where the time of a plan goes (diagnostics only, never read by a decision; game thread): microseconds per part, summed over the passes of BuildUnifiedPlan.
struct PlanWork { double portfolioUs = 0, powerUs = 0, waterUs = 0, garbageUs = 0; int passes = 0; };
inline PlanWork gPlanWork;
inline double PlanNowUs() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0; }

inline UnifiedPlan BuildUnifiedPlanOnce(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const UnifiedOptions& opt) {
    UnifiedPlan u;
    ++gPlanWork.passes;
    double t0 = PlanNowUs();
    if (opt.includeDeals) u.portfolio = PlanDealPortfolio(s, cfg, t, opt.dealScope);
    gPlanWork.portfolioUs += PlanNowUs() - t0;
    for (int i = 0; i < 3; ++i) {
        // observeOnly (a commodity whose contracts are the player's): the diagnosis exists, the accounting is the active-contract accounting of every real row below, never the portfolio's
        const bool used = opt.includeDeals && static_cast<std::size_t>(i) < u.portfolio.commodities.size() && u.portfolio.commodities[static_cast<std::size_t>(i)].evaluated &&
                          !u.portfolio.commodities[static_cast<std::size_t>(i)].observeOnly;
        u.portfolioUsed[i] = used;
        if (used) {
            const auto& r = u.portfolio.commodities[static_cast<std::size_t>(i)];
            u.commitment[i] = DealCommitment{r.sellTotal, r.buyTotal};
            if (i == 2)
                for (const auto& d : r.decisions) u.garbageTargets.push_back(PortfolioChoice{d.key, d.sellLike, d.to});
        } else {
            const Commodity c = i == 0 ? Commodity::Power : i == 1 ? Commodity::Water : Commodity::Garbage;
            u.commitment[i] = CurrentCommitment(s, c);
        }
    }
    t0 = PlanNowUs();
    u.power = PlanPower(s, cfg, t, u.portfolioUsed[0] ? &u.commitment[0] : nullptr);
    gPlanWork.powerUs += PlanNowUs() - t0;
    t0 = PlanNowUs();
    u.water = PlanWater(s, cfg, t, u.portfolioUsed[1] ? &u.commitment[1] : nullptr);
    gPlanWork.waterUs += PlanNowUs() - t0;
    t0 = PlanNowUs();
    {
        const auto base = GarbageBaseInput(s, cfg, t);
        u.garbageServiceNow = EvaluateGarbageService(base.input);
    }
    u.garbage = PlanGarbageForTargets(s, cfg, t, u.garbageTargets);   // no portfolio for garbage: the contracts stay as they are
    gPlanWork.garbageUs += PlanNowUs() - t0;

    // funding actions: raises are phase 0, cuts phase 4 (per commodity)
    for (const auto& a : u.power.actions) {
        if (a.kind != ActionKind::SetPlantFunding || a.capability != ActionCapability::Executable) continue;
        UnifiedAction x;
        x.kind = UnifiedActionKind::PlantFunding;
        x.commodity = Commodity::Power;
        x.phase = a.toPercent > a.fromPercent ? kPhaseCapacityRaise : kPhaseCapacityCut;
        x.funding = a;
        u.actions.push_back(std::move(x));
    }
    for (const auto& a : u.water.actions) {
        if (a.kind != ActionKind::SetWaterFunding || a.capability != ActionCapability::Executable) continue;
        UnifiedAction x;
        x.kind = UnifiedActionKind::WaterFunding;
        x.commodity = Commodity::Water;
        x.phase = a.toPercent > a.fromPercent ? kPhaseCapacityRaise : kPhaseCapacityCut;
        x.funding = a;
        u.actions.push_back(std::move(x));
    }
    if (!GarbageFundingCanBenefit(s.garbage.aggregate)) {
        // PROVEN absence of every garbage structure (ValidEmpty, every count read and 0): the funding buys nothing (measured on the real engine: the exports, the uncollected stock and the abducted figure
        // are the same at 0%, 25% and 100%), so a funding that still costs money is a pure waste and the cheapest funding gives exactly the same service. Nothing changes when the cost is already 0,
        // when the department figures are unread or when the absence is not the proven one (a building was listed): then the funding is left alone, as before.
        const auto& d = s.garbage.department;
        const int current = d.funding ? UtilityPercentRound(*d.funding) : 0;
        if (s.garbage.validity == SnapshotValidity::ValidEmpty && d.present && d.funding && d.idealMonthly && s.garbage.aggregate.fundingEfficiency && current > 0 &&
            ChargedExpense(*d.idealMonthly, current) > ChargedExpense(*d.idealMonthly, 0)) {
            u.garbage = GarbageFundingPlan{};
            u.garbage.status = GarbagePlanStatus::Optimal;
            u.garbage.currentPercent = current;
            u.garbage.targetPercent = 0;
            u.garbage.currentExpense = ChargedExpense(*d.idealMonthly, current);
            u.garbage.targetExpense = ChargedExpense(*d.idealMonthly, 0);
            u.garbage.steadyStatePercent = 0;
            u.garbage.steadyStateExpense = u.garbage.targetExpense;
            u.garbage.note = "no garbage structure: the funding buys nothing, the cheapest funding gives the same service";
            UnifiedAction x;
            x.kind = UnifiedActionKind::SanitationFunding;
            x.commodity = Commodity::Garbage;
            x.phase = kPhaseCapacityCut;
            x.sanitation.expectedPercent = current;
            x.sanitation.targetPercent = 0;
            x.sanitation.expectedMonthlyDelta = u.garbage.targetExpense - u.garbage.currentExpense;
            x.sanitation.reason = "Sanitation NO_STRUCTURE " + std::to_string(current) + "% -> 0%";
            u.actions.push_back(std::move(x));
            u.notes.push_back("SANITATION_FUNDING_CUT_NO_STRUCTURE: the city has no garbage processing structure (proven); the funding cannot help, so it is lowered to the cheapest (paced, verified)");
        } else {
            u.notes.push_back("SANITATION_FUNDING_LEFT_ALONE: the city has no garbage processing structure (no landfill, incinerator, waste-to-energy plant or recycling center); funding cannot help, the garbage can only be exported");
        }
    } else if ((u.garbage.status == GarbagePlanStatus::Optimal || u.garbage.status == GarbagePlanStatus::InsufficientCapacity) && u.garbage.targetPercent != u.garbage.currentPercent &&
               !(u.garbage.targetPercent < u.garbage.currentPercent && GarbageLandfillCapacityPending(s.garbage.aggregate))) {
        UnifiedAction x;
        x.kind = UnifiedActionKind::SanitationFunding;
        x.commodity = Commodity::Garbage;
        x.phase = u.garbage.targetPercent > u.garbage.currentPercent ? kPhaseCapacityRaise : kPhaseCapacityCut;
        x.sanitation.expectedPercent = u.garbage.currentPercent;
        x.sanitation.targetPercent = u.garbage.targetPercent;
        x.sanitation.expectedMonthlyDelta = u.garbage.targetExpense - u.garbage.currentExpense;
        x.sanitation.reason = std::string("Sanitation ") + GarbagePlanStatusName(u.garbage.status) + " " + std::to_string(u.garbage.currentPercent) + "% -> " +
                              std::to_string(u.garbage.targetPercent) + "%";
        u.actions.push_back(std::move(x));
    }
    for (const auto& a : u.portfolio.actions) {
        UnifiedAction x;
        x.kind = UnifiedActionKind::Deal;
        x.commodity = CommodityOfDealType(a.write.key.type);
        x.phase = a.phase;
        x.deal = a;
        u.actions.push_back(std::move(x));
    }
    std::stable_sort(u.actions.begin(), u.actions.end(), [](const UnifiedAction& a, const UnifiedAction& b) {
        return std::tie(a.commodity, a.phase) < std::tie(b.commodity, b.phase);
    });

    u.localSaving = SatAdd(SatAdd(ClampMoney(u.power.saving()), ClampMoney(u.water.saving())), ClampMoney(u.garbage.saving()));
    u.localEconomicSaving = SatAdd(SatAdd(ClampMoney(u.power.economicSaving()), ClampMoney(u.water.saving())), ClampMoney(u.garbage.saving()));
    for (std::size_t i = 0; i < u.portfolio.commodities.size() && i < 3; ++i) {
        const auto& c = u.portfolio.commodities[i];
        if (!c.evaluated || c.observeOnly) continue;
        if (const auto g = c.cashGain()) u.dealGain = SatAdd(u.dealGain, *g);
        else u.keepInfeasible[i] = true;
        if (const auto g = c.gain()) u.dealGainEconomic = SatAdd(u.dealGainEconomic, *g);
    }
    return u;
}

// Latent options (a sale that needs capacity the city does not have yet) are only worth keeping while the local plan really raises that
// capacity. If a commodity plans a latent sale but no capacity raise, nothing would ever make the sale legal (livelock): plan again for that
// commodity with real options only. At most one extra pass per commodity.
inline UnifiedPlan BuildUnifiedPlan(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const UnifiedOptions& opt = UnifiedOptions{}) {
    UnifiedOptions o = opt;
    UnifiedPlan u = BuildUnifiedPlanOnce(s, cfg, t, o);
    for (int pass = 0; pass < 3; ++pass) {
        bool changed = false;
        for (int i = 0; i < 3 && static_cast<std::size_t>(i) < u.portfolio.commodities.size(); ++i) {
            if (!o.dealScope.allowLatent[i] || !u.portfolio.commodities[static_cast<std::size_t>(i)].hasLatent()) continue;
            const Commodity c = i == 0 ? Commodity::Power : i == 1 ? Commodity::Water : Commodity::Garbage;
            bool raises = false;
            // a latent amount becomes legal after a capacity raise OR after a sell-down / cancel of another contract frees the seller surplus
            for (const auto& a : u.actions) raises = raises || (a.commodity == c && (a.phase == kPhaseCapacityRaise || a.phase == kPhaseSellDown));
            if (!raises) { o.dealScope.allowLatent[i] = false; changed = true; }
        }
        if (!changed) break;
        u = BuildUnifiedPlanOnce(s, cfg, t, o);
    }
    return u;
}

// ---- Sanitation decision evidence ----------------------------------------------------------------------------------------------
// What the Sanitation target was computed from, for the log and for the stability check. `causal` is the fingerprint of the EXOGENOUS inputs the
// planner read (load counters, planned export relief, contracted exports, buildings normalised to efficiency 1, the valve, the calendar, the
// department ideal / maximum). It deliberately excludes the control's own state (current funding and efficiency): those change because the
// control itself was written, so they can never excuse a reversal. Two writes with the same `causal` came from the same causal state.
struct GarbageDecisionInputs {
    std::string line;                  // [Garbage][DecisionInputs] ...
    std::vector<std::string> extra;    // [Garbage][Economics] and [Garbage][ImportSchedule] when foreign garbage is involved
    std::string causal;                // fingerprint of the exogenous inputs (empty: not computable)
};

// "I<neighbor>:<Amount>/<AmountUsed>-><target>[F]" per contract (I import, E export, F suspended).
inline std::string DescribeGarbageFlows(const GarbageServiceInput& in) {
    std::ostringstream o;
    for (const auto& f : in.deals)
        o << (f.isImport ? 'I' : 'E') << f.neighbor << ':' << f.amount << '/' << (f.usedKnown ? std::to_string(static_cast<long long>(f.used)) : std::string("?")) << "->" << f.target
          << (f.fixed ? "F" : "") << (f.usedRestarted ? "R" : "") << '|';     // R: last month's AmountUsed, taken as 0
    return o.str();
}

inline GarbageDecisionInputs DescribeGarbageDecision(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const UnifiedPlan& u, std::uint32_t planId) {
    GarbageDecisionInputs out;
    const auto& g = s.garbage;
    if (!g.aggregate.fundingEfficiency || !g.aggregate.produced || !g.aggregate.imported || !g.aggregate.recycled) return out;
    auto base = GarbageBaseInput(s, cfg, t);
    ApplyGarbageTargets(base.input, u.garbageTargets);
    const double effTarget = base.input.fundingCurve.Eval(u.garbage.targetPercent) / 100.0;
    const auto svc = EvaluateGarbageServiceAt(base.input, effTarget);
    const auto valve = ComputeLandfillValve(base.input.aggregate, base.input.transportCurve);
    const auto& a = g.aggregate;
    const double eff = *a.fundingEfficiency;
    const std::int64_t buildingsNormalised = eff > 0.0 ? RoundNearestEven((static_cast<double>(a.incineratorCapacityMonthly ? *a.incineratorCapacityMonthly : 0) +
                                                                          static_cast<double>(a.w2eCapacityMonthly ? *a.w2eCapacityMonthly : 0)) / eff) : -1;
    const auto cal = base.input.calendar;
    const auto num = [](const auto& o) { return o ? std::to_string(static_cast<long long>(*o)) : std::string("?"); };
    std::ostringstream c;
    // Exogenous inputs only: the contracts as they stand (amount, AmountUsed), never the targets this plan chose for them, and no calendar field by itself.
    std::ostringstream flows;
    for (const auto& f : base.input.deals) flows << (f.isImport ? 'I' : 'E') << f.neighbor << ':' << f.amount << '/' << (f.usedKnown ? std::to_string(static_cast<long long>(f.used)) : std::string("?")) << '|';
    c << "produced=" << *a.produced << ";recycled=" << *a.recycled << ";backlog=" << base.input.backlog << ";deals=" << flows.str() << ";buildingsAtEff1=" << buildingsNormalised << ";tiles=" << num(valve.tiles)
      << ";supply=" << num(valve.supply) << ";freeStorage=" << num(valve.freeStorage) << ";ideal="
      << (g.department.idealMonthly ? std::to_string(static_cast<long long>(*g.department.idealMonthly)) : std::string("?")) << ";max="
      << (g.department.maxAllowed ? std::to_string(UtilityPercentRound(*g.department.maxAllowed)) : std::string("?")) << ";reserve=" << base.input.reserve;
    out.causal = c.str();
    std::ostringstream o;
    o << "[Garbage][DecisionInputs] plan=" << planId << " funding=" << u.garbage.currentPercent << "% efficiency=" << eff << " produced=" << *a.produced << " ownLoad=" << base.noDealLoad
      << " importedCounterSetAside=" << *a.imported << " (last month's deliveries of neighbor deals: never own load) recycled=" << *a.recycled << " reservePercent="
      << ReservePercentFromFactor(base.input.reserve) << " backlog=" << base.input.backlog << " exportRelief=" << u.commitment[2].buy << " importTarget="
      << u.commitment[2].sell << " required=" << (svc.requiredHandling ? std::to_string(static_cast<long long>(*svc.requiredHandling)) : std::string("?")) << " fixedHandling=" << svc.fixedHandling
      << " landfillNeedMonthly=" << svc.landfillNeedMonthly << " landfillNeedRemaining=" << svc.landfillNeedRemaining << " landfillHeadroom=" << num(svc.landfillHeadroomNow)
      << " freeStorage=" << num(valve.freeStorage) << " garbageSupply=" << num(valve.supply) << " dayOfYear=" << s.simDayOfYear << " dayOfMonth=" << cal.dayOfMonth << '/' << cal.daysInMonth
      << " deals=" << DescribeGarbageFlows(base.input) << " remainingFraction=" << cal.remainingFraction() << " target=" << u.garbage.targetPercent << "% causalFingerprint=" << std::hash<std::string>{}(out.causal);
    out.line = o.str();

    // Economics and the import schedule: only when foreign garbage is in play (an active contract or a planned one).
    std::int64_t importsActive = 0, importsUsed = 0;
    for (const auto& f : base.input.deals) if (f.isImport) { importsActive += f.amount; importsUsed += f.used; }
    if (u.commitment[2].sell > 0 || importsActive > 0) {
        const std::int64_t protectedOwn = CeilTarget(static_cast<double>(base.noDealLoad) * base.input.reserve);
        const std::int64_t capacity = svc.fixedHandling + (svc.landfillHeadroomNow ? *svc.landfillHeadroomNow : 0);
        const std::int64_t commercialHeadroom = std::max<std::int64_t>(0, capacity - protectedOwn - base.input.backlog);
        std::int64_t revenue = 0;
        for (const auto& d : u.portfolio.commodities.size() > 2 ? u.portfolio.commodities[2].decisions : std::vector<PortfolioDecision>{})
            if (d.sellLike && d.to > 0) revenue = SatAdd(revenue, d.toMoney);
        auto noImports = base.input;
        noImports.extraImportLoad = 0;
        for (auto& f : noImports.deals) if (f.isImport && !f.fixed) f.target = 0;
        const auto without = PlanGarbageFunding(noImports, g.department.funding, g.department.maxAllowed, g.department.idealMonthly);
        const std::int64_t incremental = without.status == GarbagePlanStatus::Optimal || without.status == GarbagePlanStatus::NoChange ? SatSub(u.garbage.targetExpense, without.targetExpense) : 0;
        std::ostringstream e;
        e << "[Garbage][Economics] plan=" << planId << " ownLoad=" << base.noDealLoad << " reservePercent=" << ReservePercentFromFactor(base.input.reserve) << " protectedOwn=" << protectedOwn
          << " funding=" << u.garbage.targetPercent << "% capacity=" << capacity << " commercialHeadroom=" << commercialHeadroom << " importTarget=" << u.commitment[2].sell
          << " dealRevenue=" << revenue << " incrementalSanitationCost=" << incremental << " netCommercial=" << SatSub(revenue, incremental)
          << " (cash; ownFundingWithoutImports=" << without.targetPercent << "%)";
        out.extra.push_back(e.str());

        const std::int64_t remaining = svc.importRemaining;   // the sum of what each import contract still delivers from its own AmountUsed
        const int hi = g.department.maxAllowed ? UtilityPercentRound(*g.department.maxAllowed) : u.garbage.targetPercent;
        const std::int64_t recoverableBoundaryProbe = RecoverableBoundaryProbe(base.input, hi, std::max<std::int64_t>(u.commitment[2].sell, 1) * 8 + 1000000);
        std::ostringstream f;
        f << "[Garbage][ImportSchedule] plan=" << planId << " amount=" << importsActive << " planned=" << u.commitment[2].sell << " dayOfMonth=" << cal.dayOfMonth << '/' << cal.daysInMonth
          << " amountUsed=" << importsUsed << " remainingDeliveries=" << remaining << " projectedPeakBacklog=" << svc.projectedPeakBacklog
          << " projectedEndMonthBacklog=" << svc.projectedEndBacklog << " recoverable=" << (svc.capacitySufficient ? "YES" : "NO") << " recoverableBoundaryProbe(DIAGNOSTIC_ONLY,not_a_global_maximum)=" << recoverableBoundaryProbe
          << "@" << hi << "% economicMax=" << u.commitment[2].sell << " chosen=" << u.commitment[2].sell;
        out.extra.push_back(f.str());
    }
    return out;
}

// [Power][Economics]: one line per plant the plan changes, with the cash expense, the wear and how each was obtained.
inline std::vector<std::string> DescribePowerEconomics(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const UnifiedPlan& u) {
    std::vector<std::string> lines;
    const auto inputs = BuildPowerInputs(s);
    for (const auto& a : u.power.actions) {
        if (a.kind != ActionKind::SetPlantFunding) continue;
        for (const auto& in : inputs) {
            if (in.ref.cellX != a.cellX || in.ref.cellZ != a.cellZ) continue;
            const auto w = ComputePlantWear(in, t, a.toPercent);
            const std::int64_t cash = ChargedExpense(in.nominalCost, a.toPercent);
            const std::int64_t wear = cfg.considerPowerAging ? w.cost : 0;
            std::ostringstream o;
            o << "[Power][Economics] plant@" << a.cellX << ',' << a.cellZ << " funding=" << a.fromPercent << "%->" << a.toPercent << "% cashCost=" << cash << " observedUsage=" << in.observedUsage
              << " condition=" << in.condition << " agedThreshold=" << (in.agedThreshold ? std::to_string(*in.agedThreshold) : std::string("?")) << " baseAging="
              << (in.baseAging ? std::to_string(*in.baseAging) : std::string("?")) << " agingPerMonth=" << w.agingPerMonth << " replacementCost="
              << (in.replacementCost ? std::to_string(static_cast<long long>(*in.replacementCost)) : std::string("?")) << " wearCost=" << wear << " economicCost=" << SatAdd(cash, wear)
              << " wearModel=" << (cfg.considerPowerAging ? WearModelName(w.model) : "IGNORED(ConsiderPowerAging off: cash only)") << " reservePercent="
              << ReservePercentFromFactor(cfg.powerReserve) << " protectedTarget=" << u.power.ownTarget;
            lines.push_back(o.str());
        }
    }
    return lines;
}

// The lowest phase that still has an action for a commodity (the only phase the controller may execute for it), or -1.
inline int LowestPhase(const UnifiedPlan& u, Commodity c) {
    int best = -1;
    for (const auto& a : u.actions)
        if (a.commodity == c && (best < 0 || a.phase < best)) best = a.phase;
    return best;
}

inline const char* UnifiedKindName(UnifiedActionKind k) {
    switch (k) {
    case UnifiedActionKind::PlantFunding: return "PLANT_FUNDING";
    case UnifiedActionKind::WaterFunding: return "WATER_FUNDING";
    case UnifiedActionKind::SanitationFunding: return "SANITATION_FUNDING";
    case UnifiedActionKind::Deal: return "DEAL";
    }
    return "?";
}

inline const char* CommodityLabel(Commodity c) { return c == Commodity::Power ? "POWER" : c == Commodity::Water ? "WATER" : c == Commodity::Garbage ? "GARBAGE" : "?"; }

// One-line summary (Info) and per-action lines (Info) shared by Shadow and Auto. Candidate rejection detail belongs to Debug logging.
inline std::vector<std::string> FormatUnifiedPlan(const UnifiedPlan& u) {
    std::vector<std::string> out;
    std::ostringstream o;
    o << "[Unified][Plan] expectedMonthlyResult=" << u.expectedMonthlyResult() << " localSaving=" << u.localSaving << " dealGain=" << u.dealGain
      << " power required=" << u.power.required << " (own " << u.power.ownTarget << " sells " << u.power.contractedExports << " buys " << u.power.dependableImports << ")"
      << " water required=" << u.water.required << " garbageFunding=" << u.garbage.currentPercent << "%->" << u.garbage.targetPercent << "% ("
      << GarbagePlanStatusName(u.garbage.status) << ") portfolio=" << (u.portfolioUsed[0] || u.portfolioUsed[1] || u.portfolioUsed[2] ? "used" : "off")
      << " actions=" << u.actions.size();
    if (u.expectedEconomicResult() != u.expectedMonthlyResult())
        o << " economicResult=" << u.expectedEconomicResult() << " (cash excludes plant wear)";
    {
        const char* names[3] = {"power", "water", "garbage"};
        o << " portfolioMode=";
        for (int i = 0; i < 3; ++i)
            o << (i ? "," : "") << names[i] << ':' << (static_cast<std::size_t>(i) < u.portfolio.commodities.size() && u.portfolio.commodities[static_cast<std::size_t>(i)].evaluated &&
                                                           !u.portfolio.commodities[static_cast<std::size_t>(i)].observeOnly
                                                         ? u.portfolio.commodities[static_cast<std::size_t>(i)].mode : std::string("n/a"));
        for (const auto& c : u.portfolio.commodities)
            if (!c.policyConflict.empty()) o << ' ' << c.policyConflict;
    }
    if (u.anyKeepInfeasible()) {
        // the current contracts cannot be kept: the plan restores feasibility there and claims no money for it (dealGain covers the comparable commodities only)
        o << " currentPortfolioInfeasible=";
        const char* names[3] = {"power", "water", "garbage"};
        bool first = true;
        for (int i = 0; i < 3; ++i) if (u.keepInfeasible[i]) { o << (first ? "" : ",") << names[i]; first = false; }
    }
    out.push_back(o.str());
    for (const auto& c : u.portfolio.commodities) if (!c.policy.empty()) out.push_back(c.policy);
    for (const auto& a : u.actions) {
        std::ostringstream l;
        l << "[Unified][Action] " << CommodityLabel(a.commodity) << " phase=" << a.phase << ' ' << UnifiedKindName(a.kind) << ' ';
        if (a.kind == UnifiedActionKind::Deal) l << a.deal.write.reason;
        else if (a.kind == UnifiedActionKind::SanitationFunding) l << a.sanitation.reason;
        else l << "plant@" << a.funding.cellX << ',' << a.funding.cellZ << ' ' << a.funding.fromPercent << "% -> " << a.funding.toPercent << '%';
        out.push_back(l.str());
    }
    return out;
}

} // namespace sc4af
