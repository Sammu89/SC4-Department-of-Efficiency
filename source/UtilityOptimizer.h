// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure utility optimizer. No game types: it consumes an immutable UtilitySnapshot and returns a plan.
// A plan is NOT permission to write: every action carries a capability.
//
// Objective: lowest defensible monthly operating expense, subject to
//   * service: local capacity after the change >= reserve x demand (see OptimizerConfig) + contracted exports
//     (imports are credited only when configured; an active vanilla contract counts until its
//      own lifecycle changes it -- there is no invented pre-expiry safety window);
//   * vanilla feasibility: only legal funding grid states of controls the game actually has;
//   * current physical service: condition-dependent output and pipe/pole behavior;
//   * coupling: a Waste-to-Energy plant that is disposing garbage is pinned; its cost is counted once.

#include "DealAccounting.h"
#include "DealCard.h"
#include "GarbageControl.h"
#include "PowerExportModel.h"
#include "SafeMoney.h"
#include "UtilityHealth.h"
#include "UtilityModel.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <map>
#include <set>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace sc4af {

enum class ActionCapability { Executable, ReadOnlyRecommendation, Unsupported };

inline const char* CapabilityName(ActionCapability c) {
    switch (c) {
    case ActionCapability::Executable: return "EXECUTABLE";
    case ActionCapability::ReadOnlyRecommendation: return "READ_ONLY_RECOMMENDATION";
    case ActionCapability::Unsupported: return "UNSUPPORTED";
    }
    return "?";
}

enum class PlanStatus { Optimal, NoChange, InsufficientCapacity, Hold, NotAvailable, InvalidSnapshot, ReadOnly };

inline const char* PlanStatusName(PlanStatus s) {
    switch (s) {
    case PlanStatus::Optimal: return "OPTIMAL";
    case PlanStatus::NoChange: return "NO_CHANGE";
    case PlanStatus::InsufficientCapacity: return "INSUFFICIENT_CAPACITY";
    case PlanStatus::Hold: return "HOLD";
    case PlanStatus::NotAvailable: return "NOT_AVAILABLE";
    case PlanStatus::InvalidSnapshot: return "INVALID_SNAPSHOT";
    case PlanStatus::ReadOnly: return "READ_ONLY";
    }
    return "?";
}

struct OptimizerConfig {
    // Reserve FACTORS (1 + percent / 100), configurable inside the safe ranges of UtilityConfig.h (Power 1.10..1.50, Water 1.10..1.50, Garbage 1.02..1.20).
    double powerReserve = 1.20;          // Policy: own power target >= max(requirement, powerReserve x demand); a floor, never a ceiling
    double waterReserve = 1.10;          // Policy: own water capacity >= waterReserve x demand
    double garbageReserve = 1.02;        // Policy: protected own garbage = ceil(garbageReserve x (Produced - Recycled)); a floor, never a ceiling
    bool considerPowerAging = true;      // Power objective = cash operating expense + capital wear (replacement cost x aging); false: cash only (financial emergency)
    bool protectWaterPipes = true;       // Water: no pipe decay, vanilla 50% floor, no cut while pipes are damaged; false: only service + reserve constrain the funding
    // The funding of a commodity is NOT under the Department's hands in this plan (its manager AutoPower / AutoWater / AutoGarbage is off): the local capacity is what it is now and no plan may rely on a
    // funding change that will never be executed. Only the unified controller sets these (a commodity whose contracts are managed while its funding is not); everything else keeps them false.
    // Regional contract permissions (AutoTradePower / Water / Garbage) are NOT here: they decide which commodities enter the portfolio at all (PortfolioScope), never what a plan may assume.
    bool freezePower = false, freezeWater = false, freezeGarbage = false;
    bool allowUtilityArbitrage = false;  // AllowUtilityArbitrage (active only with AutoDeals): the deal portfolio may hold BUY and SELL of one commodity together when it pays more
    int gridStep = 1;                    // funding grid (vanilla slider: whole percents)
    int powerMinPercent = -1;            // -1: use tuning (0x29121E8B)
    int powerMaxPercent = -1;            // -1: use tuning (0xE911E2D1)
    int waterMinPercent = 50;            // the pipe curve 0xCA7138C4 is stable at 50%; the real constraints are service, pipe curve, unwatered buildings
    bool countImports = true;            // plainly active, non-suspended purchase deals lower the own requirement
    // Legacy parsed configuration fields. Kept only for backward-compatible INI parsing; ImportCredit deliberately ignores them.
    int importSafeDays = 10;
    std::int64_t minContractBenefit = 0;   // facultative contracts: minimum net benefit ($/month); 0 = any strictly positive benefit
    int waterImportSafeDays = 31;
    int healthHorizonMonths = 120;       // legacy compatibility/diagnostics only; NO EFFECT on Power funding eligibility
    double pipeMinCurveValue = 0.0;      // water: funding must keep the pipe curve >= this (>= 0: no net decay)
    double poleMinCurveValue = 0.0;      // power: every plant funding must keep the pole curve >= this (a weighted average of
                                         // values >= v is >= v, so this bounds the department funding too)
    bool useRequirement = true;          // own target = max(GetMonthlyRequirement, powerReserve x demand)
    bool exportReserve = true;           // keep capacity for economically justified sell opportunities (visible or latent)
    std::int64_t dpMaxUnitsExport = 2000; // coarser DP resolution while probing export scenarios (conservative flooring)
    std::vector<std::int64_t> sellGrid = {1000, 2000, 3000, 4000, 5000, 7500, 10000, 15000, 20000, 30000, 40000, 50000, 75000, 100000, 150000, 200000};
    std::int64_t dpMaxUnits = 20000;     // capacity resolution of the exact solver
};

// ---- power -------------------------------------------------------------------------------------------
struct PlantInput {
    BuildingRef ref;
    bool controllable = false;   // vanilla compatible mechanism with readable funding, cost, output
    bool pinned = false;         // controllable but must keep its state (e.g. W2E disposing garbage)
    std::string pinReason;
    int currentPercent = 100;
    std::int64_t nominalCost = 0;
    std::uint32_t maxOutput = 0;
    double condition = 1.0;
    Opt<double> baseAging;
    Opt<double> agedThreshold;
    Opt<std::int64_t> replacementCost;   // plop cost of the winning exemplar (0x49CAC341); unknown => no wear economics for THIS plant only
    double observedUsage = 0.0;
    std::int64_t fixedCapacity = 0;  // non-controllable plants: their current effective capacity
};

struct PlantOption {
    int percent = 100;
    std::int64_t expense = 0;    // CASH operating expense (what the simulator charges)
    std::int64_t capacity = 0;
    std::int64_t wear = 0;       // CAPITAL WEAR: an economic cost, never a treasury movement (0 when the aging objective is off or unknown)
    std::int64_t economic() const { return SatAdd(expense, wear); }
};

// ---- capital wear (ConsiderPowerAging) -----------------------------------------------------------------------------------------------
// A plant's condition falls by AgingPerMonth (UtilityHealth.h) and the plant is "aged" below `agedThreshold`. The economically usable life is the
// span between full condition and that threshold, so a month of aging consumes the fraction AgingPerMonth / (1 - agedThreshold) of the replacement
// cost:   CapitalWearCostPerMonth = ReplacementCost x AgingPerMonth / (1 - agedThreshold).
// Never more than what is left of the usable span (a plant is not depreciated below the threshold), and nothing at all for a plant that is already
// aged (no infinity, no automatic replacement). Usage is the plant's observed used / produced of the snapshot: there is no validated model that
// predicts how a different funding redistributes the load across plants, so none is invented.
// Every input must be known for THAT plant (replacement cost, aged threshold, base aging rate); otherwise the plant is priced on cash alone and the
// plant stays fully supported.
enum class WearModel { Available, AlreadyAged, UnknownReplacementCost, UnknownAgedThreshold, UnknownAgingRate };

inline const char* WearModelName(WearModel m) {
    switch (m) {
    case WearModel::Available: return "AVAILABLE";
    case WearModel::AlreadyAged: return "ALREADY_AGED(no wear, no replacement)";
    case WearModel::UnknownReplacementCost: return "UNAVAILABLE(replacement cost unknown: cash only for this plant)";
    case WearModel::UnknownAgedThreshold: return "UNAVAILABLE(aged threshold unknown: cash only for this plant)";
    case WearModel::UnknownAgingRate: return "UNAVAILABLE(base aging rate unknown: cash only for this plant)";
    }
    return "?";
}

struct PlantWear {
    WearModel model = WearModel::UnknownReplacementCost;
    std::int64_t cost = 0;          // per month, 0 unless model == Available
    double agingPerMonth = 0.0;     // condition lost per month at this funding
    double usableSpan = 0.0;
};

inline PlantWear ComputePlantWear(const PlantInput& p, const UtilityTuning& t, int percent) {
    PlantWear w;
    if (!p.replacementCost || *p.replacementCost <= 0) { w.model = WearModel::UnknownReplacementCost; return w; }
    if (!p.agedThreshold || !(*p.agedThreshold > 0.0) || !(*p.agedThreshold < 1.0)) { w.model = WearModel::UnknownAgedThreshold; return w; }
    if (!p.baseAging || !(*p.baseAging >= 0.0)) { w.model = WearModel::UnknownAgingRate; return w; }
    w.usableSpan = 1.0 - *p.agedThreshold;
    w.agingPerMonth = AgingPerMonth(t, *p.baseAging, p.observedUsage, static_cast<double>(percent) / 100.0);
    if (p.condition <= *p.agedThreshold) { w.model = WearModel::AlreadyAged; return w; }
    const double aging = std::min(w.agingPerMonth, p.condition - *p.agedThreshold);   // only the part of the usable span that is still left
    w.model = WearModel::Available;
    const double raw = static_cast<double>(*p.replacementCost) * aging / w.usableSpan;
    w.cost = raw > 0.0 ? ClampMoney(std::nearbyint(raw)) : 0;
    return w;
}

inline std::int64_t PlantCapacityAt(const UtilityTuning& t, const PlantInput& p, int percent) {
    const double eff = t.powerEfficiency.Eval(percent) / 100.0;
    return static_cast<std::int64_t>(std::floor(static_cast<double>(p.maxOutput) * eff * p.condition));
}

inline std::vector<PlantOption> BuildPlantOptions(const PlantInput& p, const UtilityTuning& t, const OptimizerConfig& cfg) {
    const int lo = cfg.powerMinPercent >= 0 ? cfg.powerMinPercent : t.powerMinPercent;
    const int hi = cfg.powerMaxPercent >= 0 ? cfg.powerMaxPercent : t.powerMaxPercent;
    std::vector<int> percents;
    if (!p.pinned) {
        for (int v = lo; v <= hi; v += std::max(1, cfg.gridStep)) percents.push_back(v);
    }
    percents.push_back(p.currentPercent);
    std::sort(percents.begin(), percents.end());
    percents.erase(std::unique(percents.begin(), percents.end()), percents.end());
    std::vector<PlantOption> out;
    for (const int v : percents) {
        if (v != p.currentPercent) {
            if (t.poleDecay.Eval(v) < cfg.poleMinCurveValue) continue;                      // pole repair would fall below the floor
        }
        PlantOption o{v, ChargedExpense(p.nominalCost, v), PlantCapacityAt(t, p, v), 0};
        if (cfg.considerPowerAging) o.wear = ComputePlantWear(p, t, v).cost;
        out.push_back(o);
    }
    return out;
}

struct PowerSolution {
    bool feasible = false;
    std::vector<int> percent;      // per input plant (non-controllable: current)
    std::int64_t expense = 0;      // CASH operating expense of the chosen state
    std::int64_t wear = 0;         // capital wear of the chosen state (0 when the aging objective is off)
    std::int64_t economic() const { return SatAdd(expense, wear); }
    int changes = 0;
    std::int64_t capacity = 0;     // exact local capacity of the chosen state
};

// The funding options of every plant (empty for a plant that is not controllable). They depend on the plants, the tuning and the option grid of the config, NOT on the
// required capacity nor on the DP resolution: the callers that solve the same plants for many needs (the deal portfolio: ~50 needs per plan) build the table once.
using PlantOptionTable = std::vector<std::vector<PlantOption>>;

inline PlantOptionTable BuildPlantOptionTable(const std::vector<PlantInput>& plants, const UtilityTuning& t, const OptimizerConfig& cfg) {
    PlantOptionTable opts(plants.size());
    for (std::size_t i = 0; i < plants.size(); ++i)
        if (plants[i].controllable) opts[i] = BuildPlantOptions(plants[i], t, cfg);
    return opts;
}

// Work counters of the plant DP (diagnostics only, never read by a decision; game thread): options offered to the DP and options it really had to try.
inline std::uint64_t gPowerDpOptions = 0, gPowerDpLive = 0;

// Exact minimiser of (expense, changes) subject to capacity >= required. Capacities are floored to `unit`
// buckets inside the DP (conservative), so a returned state is always truly feasible. `opts` = BuildPlantOptionTable(plants, t, cfg).
//
// DOMINATED OPTIONS are skipped (`prune`, default on; the result is identical with or without). For one plant an option o is
// dominated by an option o' of the same plant when o' reaches at least o's capacity bucket AND is strictly better on (economic cost, change flag) - cheaper, or as cheap
// without a change - or is identical on all three and comes earlier in the list. Any path through o is then strictly beaten (or, for the duplicate, always found later) by
// the same path through o', so o is never in the solution the DP returns, and removing it changes no stored optimum. Typical case: the top of the efficiency curve, where
// a higher funding costs more and adds no capacity bucket.
inline PowerSolution SolvePowerWith(const std::vector<PlantInput>& plants, const PlantOptionTable& opts, std::int64_t required, std::int64_t dpMaxUnits, bool prune = true) {
    PowerSolution best;
    best.percent.resize(plants.size());
    std::int64_t fixedCap = 0;
    std::vector<std::size_t> ctl;
    for (std::size_t i = 0; i < plants.size(); ++i) {
        best.percent[i] = plants[i].currentPercent;
        if (plants[i].controllable) ctl.push_back(i);
        else fixedCap += plants[i].fixedCapacity;
    }
    const std::int64_t need = std::max<std::int64_t>(0, required);
    const std::int64_t unit = std::max<std::int64_t>(1, (need + dpMaxUnits - 1) / std::max<std::int64_t>(1, dpMaxUnits));
    const std::int64_t R = (need + unit - 1) / unit;
    const std::int64_t INF = std::numeric_limits<std::int64_t>::max() / 4;
    struct Cell { std::int64_t cost = 0; int changes = 0; };
    std::vector<Cell> dp(static_cast<std::size_t>(R + 1), Cell{INF, 0});
    const std::int64_t base = std::min<std::int64_t>(R, fixedCap / unit);
    dp[static_cast<std::size_t>(base)] = Cell{0, 0};
    struct Choice { std::int32_t opt = -1; std::int32_t prev = -1; };
    std::vector<std::vector<Choice>> choice(ctl.size(), std::vector<Choice>(static_cast<std::size_t>(R + 1)));
    const auto better = [](const Cell& a, const Cell& b) { return a.cost != b.cost ? a.cost < b.cost : a.changes < b.changes; };
    std::vector<Cell> next(dp.size());
    std::vector<std::int64_t> bucket, economic;
    std::vector<int> changed;
    std::vector<std::size_t> live;
    for (std::size_t k = 0; k < ctl.size(); ++k) {
        const auto& p = plants[ctl[k]];
        const auto& po = opts[ctl[k]];
        // per option, what the inner loop needs (bucket = capacity / unit, the economic cost, the change flag)
        bucket.resize(po.size());
        economic.resize(po.size());
        changed.resize(po.size());
        for (std::size_t o = 0; o < po.size(); ++o) {
            bucket[o] = po[o].capacity / unit;
            economic[o] = po[o].economic();
            changed[o] = po[o].percent != p.currentPercent ? 1 : 0;
        }
        live.clear();
        for (std::size_t o = 0; o < po.size(); ++o) {
            bool dominated = false;
            for (std::size_t q = 0; prune && q < po.size() && !dominated; ++q) {
                if (q == o || bucket[q] < bucket[o] || economic[q] > economic[o] || changed[q] > changed[o]) continue;
                dominated = economic[q] < economic[o] || changed[q] < changed[o] || (bucket[q] == bucket[o] && q < o);
            }
            if (!dominated) live.push_back(o);
        }
        gPowerDpOptions += po.size();
        gPowerDpLive += live.size();
        std::fill(next.begin(), next.end(), Cell{INF, 0});
        auto& ch = choice[k];
        for (std::int64_t c = 0; c <= R; ++c) {
            const Cell here = dp[static_cast<std::size_t>(c)];
            if (here.cost >= INF) continue;
            for (const std::size_t o : live) {
                const std::int64_t nc = std::min<std::int64_t>(R, c + bucket[o]);
                const Cell cand{SatAdd(here.cost, economic[o]), here.changes + changed[o]};
                if (better(cand, next[static_cast<std::size_t>(nc)])) {
                    next[static_cast<std::size_t>(nc)] = cand;
                    ch[static_cast<std::size_t>(nc)] = {static_cast<std::int32_t>(o), static_cast<std::int32_t>(c)};
                }
            }
        }
        dp.swap(next);
    }
    if (dp[static_cast<std::size_t>(R)].cost >= INF) {
        // Infeasible: policy is to maximise available service (highest-capacity healthy option per plant).
        std::int64_t cap = fixedCap;
        std::int64_t expense = 0, wear = 0;
        int changes = 0;
        for (const auto i : ctl) {
            const PlantOption* pick = &opts[i].front();
            for (const auto& o : opts[i])
                if (o.capacity > pick->capacity || (o.capacity == pick->capacity && o.economic() < pick->economic())) pick = &o;
            best.percent[i] = pick->percent;
            cap += pick->capacity;
            expense += pick->expense;
            wear += pick->wear;
            changes += pick->percent != plants[i].currentPercent ? 1 : 0;
        }
        best.feasible = false;
        best.capacity = cap;
        best.expense = expense;
        best.wear = wear;
        best.changes = changes;
        return best;
    }
    std::int64_t c = R;
    std::int64_t cap = fixedCap;
    std::int64_t expense = 0, wear = 0;
    for (std::size_t kk = ctl.size(); kk-- > 0;) {
        const auto ch = choice[kk][static_cast<std::size_t>(c)];
        const auto& opt = opts[ctl[kk]][static_cast<std::size_t>(ch.opt)];
        best.percent[ctl[kk]] = opt.percent;
        cap += opt.capacity;
        expense += opt.expense;
        wear += opt.wear;
        c = ch.prev;
    }
    best.feasible = true;
    best.capacity = cap;
    best.expense = expense;
    best.wear = wear;
    best.changes = dp[static_cast<std::size_t>(R)].changes;
    return best;
}

inline PowerSolution SolvePower(const std::vector<PlantInput>& plants, std::int64_t required, const UtilityTuning& t, const OptimizerConfig& cfg) {
    return SolvePowerWith(plants, BuildPlantOptionTable(plants, t, cfg), required, cfg.dpMaxUnits);
}

// Many needs on the SAME plants (the deal portfolio, the export / buy cards): one option table and one solution per need, shared by every caller that asks for that need
// (the economic AND the cash expense of a need come from the same solution). Exactly SolvePower's results.
class PowerNeedSolver {
public:
    PowerNeedSolver(std::vector<PlantInput> plants, const UtilityTuning& t, const OptimizerConfig& cfg)
        : plants_(std::move(plants)), opts_(BuildPlantOptionTable(plants_, t, cfg)), dpMaxUnits_(cfg.dpMaxUnits) {}
    const PowerSolution& Solve(std::int64_t need) {
        const auto it = memo_.find(need);
        if (it != memo_.end()) return it->second;
        return memo_.emplace(need, SolvePowerWith(plants_, opts_, need, dpMaxUnits_)).first->second;
    }
    std::size_t solved() const { return memo_.size(); }

private:
    std::vector<PlantInput> plants_;
    PlantOptionTable opts_;
    std::int64_t dpMaxUnits_;
    std::map<std::int64_t, PowerSolution> memo_;
};

// ---- plan structures ----------------------------------------------------------------------------------
enum class ActionKind { SetPlantFunding, SetWaterFunding, RecommendDeal };

struct PlanAction {
    ActionKind kind = ActionKind::SetPlantFunding;
    ActionCapability capability = ActionCapability::Executable;
    int cellX = 0, cellZ = 0;
    int fromPercent = 0, toPercent = 0;
    std::int64_t monthlyDelta = 0;      // negative = saving; for deals: revenue (positive)
    std::int32_t dealType = 0;
    Opt<std::uint32_t> neighbor;
    std::int64_t amount = 0;
    std::string note;
};

struct SubsystemPlan {
    PlanStatus status = PlanStatus::NotAvailable;
    std::int64_t currentExpense = 0;    // CASH (what the simulator charges): the ledger and the measured deltas only ever see this
    std::int64_t targetExpense = 0;
    std::int64_t currentWear = 0;       // Power only: capital wear of the current / target state (economic, never cash)
    std::int64_t targetWear = 0;
    std::int64_t required = 0;          // local capacity the plan must keep
    std::int64_t availableNow = 0;
    std::int64_t availableAfter = 0;
    std::vector<PlanAction> actions;
    std::vector<std::string> notes;
    // power only: how `required` was composed (all in local capacity units)
    std::int64_t ownTarget = 0, contractedExports = 0, dependableImports = 0, exportReserve = 0;
    std::int64_t exportRevenue = 0, exportExtraExpense = 0;      // HYPOTHETICAL while deals are read-only
    std::vector<ChosenSell> exportPicks;
    std::vector<FrontierPoint> exportFrontier;
    std::vector<DealCard> cards;                                 // player-facing deal summaries (sell and buy), incremental economics
    std::int64_t saving() const { return SatSub(currentExpense, targetExpense); }                                  // cash
    std::int64_t economicSaving() const { return SatSub(SatAdd(currentExpense, currentWear), SatAdd(targetExpense, targetWear)); }
};

struct GarbageBalance {
    Opt<std::int64_t> inputs;            // Produced + Imported
    Opt<std::int64_t> outputs;           // Recycled + Exported + Incinerated + ToEnergy + Landfilled + Abducted
    Opt<std::int64_t> deltaStock;        // UncollectedEnd - UncollectedStart
    Opt<std::int64_t> unexplainedResidual; // inputs - outputs - deltaStock; counter truncation/rounding remains explicit
    Opt<double> residualFraction;
    bool serviceShortage = false;        // Abducted or uncollected garbage is a failure signal, not useful capacity
};

inline GarbageBalance ComputeGarbageBalance(const GarbageAggregate& a) {
    GarbageBalance b;
    // Abducted garbage is the game's decay, not a failure (see GarbageShortageSignal): only the uncollected stock is
    b.serviceShortage = (a.uncollectedEnd && *a.uncollectedEnd > 0) || (!a.uncollectedEnd && a.uncollected && *a.uncollected > 0);
    if (!a.produced || !a.imported) return b;
    b.inputs = static_cast<std::int64_t>(*a.produced) + *a.imported;
    const auto allOutputs = a.recycled && a.exported && a.incinerated && a.toEnergy && a.toLandfill && a.abducted;
    if (!allOutputs || !a.uncollectedStart || !a.uncollectedEnd) return b;
    b.outputs = static_cast<std::int64_t>(*a.recycled) + *a.exported + *a.incinerated + *a.toEnergy + *a.toLandfill + *a.abducted;
    b.deltaStock = static_cast<std::int64_t>(*a.uncollectedEnd) - *a.uncollectedStart;
    b.unexplainedResidual = *b.inputs - *b.outputs - *b.deltaStock;
    if (*b.inputs != 0) b.residualFraction = static_cast<double>(*b.unexplainedResidual) / static_cast<double>(*b.inputs);
    return b;
}

// Garbage handling reserve (policy default 2%; read-only: garbage funding has no writer).
// Base disposal load is the simulator flow that reaches the disposal pipeline: Produced - Recycled + Imported.
// Landfill storage is deliberately absent: only a reconstructed monthly vanilla intake limit may contribute.
struct GarbageHandling {
    Opt<std::int64_t> baseDisposalLoad;
    Opt<std::int64_t> requiredHandling;
    std::int64_t handling = 0;
    Opt<double> ratio;                 // handling / base disposal load
    bool meetsReserve = false;
    bool landfillIntakeUnknown = false;
    bool serviceShortage = false;
    Opt<std::int64_t> unexplainedResidual;
    std::string basis = "contracted exports + funded incinerator + funded W2E + authoritative landfill monthly intake; storage never amortized";
};

inline GarbageHandling ComputeGarbageHandling(const GarbageAggregate& a, std::int64_t contractedExports, double reserve) {
    GarbageHandling h;
    if (!a.produced || !a.imported || !a.recycled) return h;
    h.baseDisposalLoad = std::max<std::int64_t>(0, static_cast<std::int64_t>(*a.produced) - *a.recycled + *a.imported);
    h.requiredHandling = CeilTarget(static_cast<double>(*h.baseDisposalLoad) * reserve);
    const auto add = [&](const Opt<std::int32_t>& v) { if (v && *v > 0) h.handling += *v; };
    add(a.incineratorCapacityMonthly);
    add(a.w2eCapacityMonthly);
    if (a.landfillIntakeCapacityMonthly) add(a.landfillIntakeCapacityMonthly);
    else if (a.landfillTiles && *a.landfillTiles > 0) h.landfillIntakeUnknown = true;
    h.handling += std::max<std::int64_t>(0, contractedExports);
    h.serviceShortage = ComputeGarbageBalance(a).serviceShortage;
    if (*h.baseDisposalLoad > 0) {
        h.ratio = static_cast<double>(h.handling) / static_cast<double>(*h.baseDisposalLoad);
        h.meetsReserve = !h.serviceShortage && h.handling >= *h.requiredHandling;
    } else {
        h.meetsReserve = !h.serviceShortage;
    }
    h.unexplainedResidual = ComputeGarbageBalance(a).unexplainedResidual;
    return h;
}

struct UtilityPlan {
    SubsystemPlan power, water;
    PlanStatus garbageStatus = PlanStatus::ReadOnly;
    GarbageBalance garbage;
    GarbageHandling garbageHandling;
    std::vector<std::string> garbageNotes;
    GarbageFundingPlan garbageFunding;  // READ_ONLY: lowest-expense Sanitation funding that keeps the modelled service sufficient
    SubsystemPlan deals;                // recommendations only (READ_ONLY_RECOMMENDATION)
    std::int64_t currentExpense() const { return power.currentExpense + water.currentExpense; }
    std::int64_t targetExpense() const { return power.targetExpense + water.targetExpense; }
};

// ---- helpers for snapshot -> inputs ------------------------------------------------------------------------
inline bool DealIsDependable(const ExistingDealRow& d) {
    // flags: 1 pending, 2 active, 4 cancelled, 8/0x10/0x20 invalid/unaffordable/no capacity, 0x100/0x200 suspension states.
    // Only a plain ACTIVE deal (exactly 2) is relied upon: suspended and failing deals are not.
    return d.flags && *d.flags == 2;
}

inline std::int64_t ImportCredit(const DealState& deals, std::int32_t buyType, const DateStamp& now, int safeDays,
                                 std::vector<std::string>* notes) {
    // Vanilla keeps accounting for a plain ACTIVE contract up to its actual lifecycle transition.
    // There is no expiry-gap mechanism, so ImportSafeDays/WaterImportSafeDays are ignored.
    (void)now;
    (void)safeDays;
    std::int64_t credit = 0;
    for (const auto& d : deals.deals) {
        if (d.type != buyType || !d.amount) continue;
        if (!DealIsDependable(d)) { if (notes) notes->push_back("buy deal is not plainly active (suspended/pending/failing), not counted"); continue; }
        credit += *d.amount;
    }
    return credit;
}

// `frozen`: the plants' funding is not under the Department's hands (AutoPower off): every plant is a fixed capacity (its current effective production), nothing is planned to change.
inline std::vector<PlantInput> BuildPowerInputs(const UtilitySnapshot& s, bool frozen = false) {
    std::vector<PlantInput> out;
    for (const auto& p : s.power.plants) {
        PlantInput in;
        in.ref = p.ref;
        const bool ready = p.mechanism == MechanismClass::Compatible && p.funding && p.maxProduced && p.budgetLine &&
                           p.conditionPercent && p.budgetLine->cost > 0;
        in.controllable = ready && !frozen;
        if (ready) {
            in.currentPercent = UtilityPercentRound(*p.funding);
            in.nominalCost = p.budgetLine->cost;
            in.maxOutput = *p.maxProduced;
            in.condition = *p.conditionPercent;
            if (p.baseAgingRate) in.baseAging = *p.baseAgingRate;
            if (p.agedThreshold) in.agedThreshold = *p.agedThreshold;
            if (p.replacementCost) in.replacementCost = *p.replacementCost;
            if (p.produced && *p.produced > 0 && p.used) in.observedUsage = static_cast<double>(*p.used) / *p.produced;
        }
        in.fixedCapacity = p.produced ? static_cast<std::int64_t>(*p.produced) : 0;
        if (p.wasteToEnergy()) {
            for (const auto& g : s.garbage.buildings) {
                if (g.ref.cellX == p.ref.cellX && g.ref.cellZ == p.ref.cellZ && g.w2eUsed && *g.w2eUsed > 0) {
                    in.pinned = true;
                    in.pinReason = "W2E is disposing garbage (coupled occupant)";
                }
            }
        }
        out.push_back(std::move(in));
    }
    return out;
}

inline PlanAction PlantAction(const PlantInput& in, int to, std::int64_t fromExpense, std::int64_t toExpense) {
    PlanAction a;
    a.kind = ActionKind::SetPlantFunding;
    a.capability = ActionCapability::Executable;
    a.cellX = in.ref.cellX;
    a.cellZ = in.ref.cellZ;
    a.fromPercent = in.currentPercent;
    a.toPercent = to;
    a.monthlyDelta = toExpense - fromExpense;
    return a;
}

// ---- export side inputs ------------------------------------------------------------------------------------------
inline Opt<double> TunedValueOf(const DealState& d, std::uint32_t id) {
    for (const auto& t : d.tuning)
        if (t.id == id && t.present && !t.values.empty()) return t.values.front();
    return std::nullopt;
}

inline SellTuning SellTuningOf(const DealState& d) {
    SellTuning t;
    if (const auto v = TunedValueOf(d, 0xEA46932Cu)) t.minAmount = *v;
    if (const auto v = TunedValueOf(d, 0xEA4AD2FFu)) t.overDemandFactor = *v;
    if (const auto v = TunedValueOf(d, 0xEA4AE9EFu)) t.minBudget = *v;
    if (const auto v = TunedValueOf(d, 0xCA46931Fu)) t.minTerm = *v;
    return t;
}

// Neighbors that could buy power from us. Revenue is a CONSERVATIVE lower bound: amount x seller unit cost x (1 + NDc_MinDealProfitFactor).
// The game's price rises with the deal's share of the buyer's need and as seller surplus shrinks (GetDealCost 0x6D3250), so this floor is what
// the deal earns even in the cheapest case; the quoted values stay in the log for reference. Nothing here is a real deal.
inline std::vector<NeighborSell> BuildSellNeighbors(const UtilitySnapshot& s, const OptimizerConfig& cfg, std::vector<std::string>* notes) {
    std::vector<NeighborSell> out;
    const auto& d = s.deals;
    if (!d.proposalsRead || !d.localPowerUnitCost) return out;
    const SellTuning tuning = SellTuningOf(d);
    const double minPF = TunedValueOf(d, 0x6A469342u).value_or(0.1);
    const double unit = *d.localPowerUnitCost;
    if (unit <= 0.0) return out;
    const auto hasPowerDeal = [&](std::uint32_t serial) {
        for (const auto& e : d.deals)
            if (e.type == utility_id::kDealSellPower && e.otherCitySerial && *e.otherCitySerial == serial) return true;   // vanilla: FindDeals(SELL, city) must be empty; a BUY deal with the same city does not block (observed)
        return false;
    };
    const auto quoteAt = [&](std::uint32_t serial, std::int64_t amount) -> std::optional<std::int64_t> {
        for (const auto& c : d.connections)
            if (c.otherCitySerial && *c.otherCitySerial == serial)
                for (const auto& q : c.sellQuotes)
                    if (q.first == amount && q.second > 0) return q.second;
        for (const auto& o : d.opportunities)
            if (o.type == utility_id::kDealSellPower && o.otherCitySerial && *o.otherCitySerial == serial)
                for (const auto& q : o.quotes)
                    if (q.first == amount && q.second > 0) return q.second;
        return std::nullopt;
    };
    const auto build = [&](std::uint32_t serial, bool visible, double maxAmount) {
        NeighborSell n;
        n.serial = serial;
        n.visible = visible;
        n.minAmount = static_cast<std::int64_t>(std::ceil(tuning.minAmount));
        n.maxAmount = static_cast<std::int64_t>(std::floor(maxAmount));
        for (const auto a : cfg.sellGrid)
            if (a >= n.minAmount && a <= n.maxAmount) {
                // authoritative path first: the game's GetDealCost total for this amount. Where the quote is missing, the price floor
                // (local cost/unit x (1 + NDc_MinDealProfitFactor)) is used. Both are conservative: the game's premium only grows with
                // the seller surplus that holding the capacity creates.
                const auto quote = quoteAt(serial, a);
                n.options.push_back({a, quote ? *quote : static_cast<std::int64_t>(std::floor(static_cast<double>(a) * unit * (1.0 + minPF)))});
            }
        if (!n.options.empty()) out.push_back(std::move(n));
    };
    std::set<std::uint32_t> done;
    for (const auto& c : d.connections) {
        if (c.carries != Commodity::Power || !c.active || !c.otherCitySerial || hasPowerDeal(*c.otherCitySerial)) continue;
        if (!done.insert(*c.otherCitySerial).second) continue;                 // one sale per neighbor, however many poles connect to it
        if (!c.nbStats || !c.nbBudget || !c.sellCostAtMin) continue;
        bool visible = false;
        for (const auto& o : d.opportunities)
            if (o.type == utility_id::kDealSellPower && o.otherCitySerial == c.otherCitySerial) visible = true;
        const double costAtMin = static_cast<double>(*c.sellCostAtMin);
        // the buyer side alone decides the ceiling: seller surplus is exactly what the reserve creates
        const double need = BuyerNeed(*c.nbStats, tuning);
        if (!BuyerTreasuryOk(*c.nbBudget, costAtMin, tuning) || need < tuning.minAmount) continue;
        build(*c.otherCitySerial, visible, need);
    }
    if (notes && out.empty()) notes->push_back("no neighbor can currently buy power (no export reserve)");
    return out;
}

// FINAL deal totals chosen by the portfolio for one commodity (sell-like: SELL / IMPORT_GARBAGE; buy-like: BUY / EXPORT_GARBAGE).
// When given, they REPLACE the active-contract accounting of the local plan (no double counting) and the legacy export reserve.
struct DealCommitment {
    std::int64_t sell = 0, buy = 0;
};

inline SubsystemPlan PlanPower(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealCommitment* commit = nullptr) {
    SubsystemPlan plan;
    if (s.power.validity == SnapshotValidity::ValidEmpty) { plan.status = PlanStatus::NoChange; plan.notes.push_back("no power plants"); return plan; }
    if (s.power.validity != SnapshotValidity::Valid && s.power.validity != SnapshotValidity::Partial) { plan.status = PlanStatus::InvalidSnapshot; return plan; }
    if (!s.power.aggregate.demand || !TuningValid(t)) { plan.status = PlanStatus::NotAvailable; plan.notes.push_back("demand or tuning unknown"); return plan; }
    const auto inputs = BuildPowerInputs(s, cfg.freezePower);
    const auto exports = ContractedExportsOf(s.deals.deals, s.power.gameExported, std::nullopt, std::nullopt);
    std::int64_t sellPower = exports.power;
    std::int64_t credit = 0;
    if (cfg.countImports) credit = ImportCredit(s.deals, utility_id::kDealBuyPower, {s.simYear, s.simDayOfYear}, cfg.importSafeDays, &plan.notes);
    else ImportCredit(s.deals, utility_id::kDealBuyPower, {s.simYear, s.simDayOfYear}, cfg.importSafeDays, &plan.notes);
    if (commit) { sellPower = commit->sell; credit = commit->buy; plan.notes.push_back("deal totals from the unified portfolio"); }
    double ownTarget = cfg.powerReserve * static_cast<double>(*s.power.aggregate.demand);
    if (cfg.useRequirement && s.power.aggregate.requirement) ownTarget = std::max(ownTarget, static_cast<double>(*s.power.aggregate.requirement));
    plan.ownTarget = CeilTarget(ownTarget);       // the protected target with NO import (informational)
    plan.contractedExports = sellPower;
    plan.dependableImports = credit;
    // the reserve covers what the city produces itself; the imports are not multiplied by it (LocalRequiredCapacity)
    plan.required = LocalRequiredCapacity(cfg.powerReserve, *s.power.aggregate.demand, credit, sellPower,
                                          cfg.useRequirement && s.power.aggregate.requirement ? static_cast<std::int64_t>(*s.power.aggregate.requirement) : 0);
    std::int64_t availNow = 0;
    for (const auto& in : inputs) availNow += in.controllable ? PlantCapacityAt(t, in, in.currentPercent) : in.fixedCapacity;
    plan.availableNow = availNow;
    for (const auto& in : inputs) {
        if (!in.controllable) continue;
        plan.currentExpense += ChargedExpense(in.nominalCost, in.currentPercent);
        if (cfg.considerPowerAging) plan.currentWear += ComputePlantWear(in, t, in.currentPercent).cost;
    }
    bool anyControllable = false;
    for (const auto& in : inputs) { anyControllable = anyControllable || in.controllable; if (in.pinned) plan.notes.push_back("pinned: " + in.pinReason); }
    if (!anyControllable) { plan.status = PlanStatus::NoChange; plan.targetExpense = plan.currentExpense; plan.targetWear = plan.currentWear; plan.availableAfter = availNow; plan.notes.push_back("no controllable plants"); return plan; }

    if (cfg.exportReserve && !commit) {
        const auto neighbors = BuildSellNeighbors(s, cfg, &plan.notes);
        if (!neighbors.empty()) {
            OptimizerConfig coarse = cfg;
            coarse.dpMaxUnits = cfg.dpMaxUnitsExport;
            PowerNeedSolver solver(inputs, t, coarse);
            const ExpenseFn expenseFor = [&](std::int64_t need) -> std::optional<std::int64_t> {
                const auto& r = solver.Solve(need);
                if (!r.feasible) return std::nullopt;
                return r.economic();   // sales are judged on cash + wear
            };
            const auto ex = PlanExports(plan.required, neighbors, expenseFor, true);
            plan.exportReserve = ex.reserve;
            plan.exportRevenue = ex.revenue;
            plan.exportExtraExpense = ex.extraExpense;
            plan.exportPicks = ex.chosen;
            plan.exportFrontier = ex.frontier;
            plan.required += ex.reserve;
            for (const auto& n : neighbors) {
                // the card shows the best amount for the neighbor (highest incremental profit; the smallest legal amount if all lose)
                const FrontierPoint* best = nullptr;
                for (const auto& f : ex.frontier)
                    if (f.serial == n.serial && (!best || f.net > best->net)) best = &f;
                DealCard c;
                c.sell = true;
                c.neighbor = n.serial;
                c.visible = n.visible;
                if (best && best->feasible) {
                    c.amount = best->amount;
                    c.deal = best->revenue;
                    c.utility = best->extraExpense;
                    c.profit = best->net;
                    c.calculable = true;
                    c.internals = "required " + std::to_string(plan.required - ex.reserve) + " -> " + std::to_string(plan.required - ex.reserve + best->amount);
                } else if (!n.options.empty()) {
                    c.amount = n.options.front().amount;
                }
                plan.cards.push_back(c);
            }
        }
        // BUY cards: what a purchase would save locally versus what the game charges for it (visible opportunities only)
        {
            OptimizerConfig coarse = cfg;
            coarse.dpMaxUnits = cfg.dpMaxUnitsExport;
            PowerNeedSolver solver(inputs, t, coarse);
            const auto expenseAt = [&](std::int64_t need) -> std::optional<std::int64_t> {
                const auto& r = solver.Solve(need);
                if (!r.feasible) return std::nullopt;
                return r.economic();
            };
            const std::int64_t baseNeed = plan.required - plan.exportReserve;
            const auto baseCost = expenseAt(baseNeed);
            for (const auto& o : s.deals.opportunities) {
                if (o.type != utility_id::kDealBuyPower || o.quotes.empty()) continue;
                DealCard c;
                c.sell = false;
                c.visible = true;
                if (o.otherCitySerial) c.neighbor = *o.otherCitySerial;
                bool have = false;
                for (const auto& q : o.quotes) {
                    if (!baseCost) break;
                    const auto after = expenseAt(std::max<std::int64_t>(0, baseNeed - q.first));
                    if (!after) continue;
                    const std::int64_t saving = *baseCost - *after;
                    const std::int64_t profit = saving - q.second;
                    if (!have || profit > c.profit) { c.amount = q.first; c.deal = q.second; c.utility = saving; c.profit = profit; have = true; }
                }
                c.calculable = have;
                if (!have) c.amount = o.quotes.front().first;
                plan.cards.push_back(c);
            }
        }
    }
    const auto sol = SolvePower(inputs, plan.required, t, cfg);
    plan.availableAfter = sol.capacity;
    plan.targetExpense = sol.expense;
    plan.targetWear = sol.wear;
    for (std::size_t i = 0; i < inputs.size(); ++i) {
        if (!inputs[i].controllable || sol.percent[i] == inputs[i].currentPercent) continue;
        plan.actions.push_back(PlantAction(inputs[i], sol.percent[i], ChargedExpense(inputs[i].nominalCost, inputs[i].currentPercent),
                                           ChargedExpense(inputs[i].nominalCost, sol.percent[i])));
    }
    if (!sol.feasible) plan.status = PlanStatus::InsufficientCapacity;
    else plan.status = plan.actions.empty() ? PlanStatus::NoChange : PlanStatus::Optimal;
    return plan;
}

// ---- water --------------------------------------------------------------------------------------------------
struct WaterFundingOption { int pct; std::int64_t expense; std::int64_t capacity; };

// The legal Water funding states (shared by PlanWater and the deal portfolio): reductions are forbidden while buildings are unwatered,
// while pipes are damaged and where the pipe curve would let the network decay.
inline std::vector<WaterFundingOption> BuildWaterFundingOptions(const OptimizerConfig& cfg, const UtilityTuning& t, std::int64_t idealMonthly,
                                                                int cur, int hi, bool unwateredNow, bool pipesDamaged, double effNow,
                                                                std::int64_t availableNow) {
    std::vector<WaterFundingOption> options;
    // ProtectWaterPipes ON: the vanilla 50% floor (below it the pipe network decays), no cut while pipes are damaged, no funding that lets the
    // network decay. OFF: none of those pipe constraints, only service (never cut while buildings are unwatered) and the configured reserve
    // (applied by the caller) bound the funding; no other health floor is invented.
    const int lowest = cfg.protectWaterPipes ? cfg.waterMinPercent : 0;
    for (int v = lowest; v <= hi; v += std::max(1, cfg.gridStep)) {
        if (v < cur) {
            if (unwateredNow) continue;                                   // never cut while buildings are unwatered
            if (cfg.protectWaterPipes) {
                if (pipesDamaged) continue;                               // damaged pipes: never reduce repair funding
                if (t.pipeDecay.Eval(v) < cfg.pipeMinCurveValue) continue;    // funding that lets the network decay
            }
        }
        options.push_back({v, ChargedExpense(idealMonthly, v),
                           static_cast<std::int64_t>(std::floor(static_cast<double>(availableNow) * t.waterEfficiency.Eval(v) / effNow))});
    }
    if (std::none_of(options.begin(), options.end(), [&](const WaterFundingOption& o) { return o.pct == cur; }))
        options.push_back({cur, ChargedExpense(idealMonthly, cur), availableNow});
    if (cfg.freezeWater)   // the funding is not under the Department's hands (AutoWater off): the only state is the current one
        options.erase(std::remove_if(options.begin(), options.end(), [&](const WaterFundingOption& o) { return o.pct != cur; }), options.end());
    return options;
}

inline SubsystemPlan PlanWater(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealCommitment* commit = nullptr) {
    SubsystemPlan plan;
    const auto& w = s.water;
    if (w.validity == SnapshotValidity::ValidEmpty) { plan.status = PlanStatus::NoChange; plan.notes.push_back("no water producers"); return plan; }
    if (w.validity != SnapshotValidity::Valid && w.validity != SnapshotValidity::Partial) { plan.status = PlanStatus::InvalidSnapshot; return plan; }
    if (!w.department.present || !w.department.funding || !w.department.idealMonthly || !w.aggregate.demand ||
        !w.aggregate.productionCapacity || !TuningValid(t)) {
        plan.status = PlanStatus::NotAvailable;
        plan.notes.push_back("water control, expense, demand or capacity unknown");
        return plan;
    }
    for (const auto& p : w.producers)
        if (p.mechanism == MechanismClass::UnsupportedMechanism) { plan.status = PlanStatus::NotAvailable; plan.notes.push_back("a producer uses a non-standard mechanism"); return plan; }
    const int cur = UtilityPercentRound(*w.department.funding);
    plan.currentExpense = ChargedExpense(*w.department.idealMonthly, cur);
    plan.targetExpense = plan.currentExpense;
    const auto exports = ContractedExportsOf(s.deals.deals, std::nullopt, w.gameExported, std::nullopt);
    std::int64_t credit = 0;
    if (cfg.countImports) credit = ImportCredit(s.deals, utility_id::kDealBuyWater, {s.simYear, s.simDayOfYear}, cfg.waterImportSafeDays, &plan.notes);
    else ImportCredit(s.deals, utility_id::kDealBuyWater, {s.simYear, s.simDayOfYear}, cfg.waterImportSafeDays, &plan.notes);
    std::int64_t sellWater = exports.water;
    if (commit) { sellWater = commit->sell; credit = commit->buy; plan.notes.push_back("deal totals from the unified portfolio"); }
    plan.required = LocalRequiredCapacity(cfg.waterReserve, *w.aggregate.demand, credit, sellWater);
    plan.availableNow = *w.aggregate.productionCapacity;
    plan.availableAfter = plan.availableNow;

    // Service state: an existing shortage, or counts that are not populated yet, means HOLD (never cut then).
    if (!ServiceCountsReady(w.aggregate)) { plan.status = PlanStatus::Hold; plan.notes.push_back("SERVICE_COUNTS_NOT_READY (right after load)"); return plan; }
    const bool unwateredNow = UnwateredBlocksCut(w.aggregate);   // reductions are forbidden then (production below demand); raising funding is exactly the answer
    const double effNow = t.waterEfficiency.Eval(static_cast<double>(*w.department.funding) * 100.0);   // the capacity read belongs to the EXACT funding, not to its rounded percent
    if (effNow <= 0.0) { plan.status = PlanStatus::NotAvailable; plan.notes.push_back("efficiency at the current funding is zero"); return plan; }
    const bool pipesDamaged = (w.aggregate.distressedPipes && *w.aggregate.distressedPipes > 0) || (w.aggregate.burstPipes && *w.aggregate.burstPipes > 0);
    int hi = t.waterMaxPercent;
    if (w.department.maxAllowed) hi = std::min(hi, UtilityPercentRound(*w.department.maxAllowed));
    using Opt2 = WaterFundingOption;
    const std::vector<Opt2> options = BuildWaterFundingOptions(cfg, t, *w.department.idealMonthly, cur, hi, unwateredNow, pipesDamaged, effNow,
                                                               plan.availableNow);
    const Opt2* pick = nullptr;
    for (const auto& o : options) {
        if (o.capacity < plan.required) continue;
        if (!pick || o.expense < pick->expense || (o.expense == pick->expense && o.pct == cur)) pick = &o;
    }
    if (!pick) {
        // Insufficient: maximise available service.
        for (const auto& o : options)
            if (!pick || o.capacity > pick->capacity || (o.capacity == pick->capacity && o.expense < pick->expense)) pick = &o;
        plan.status = PlanStatus::InsufficientCapacity;
    } else {
        plan.status = pick->pct == cur ? PlanStatus::NoChange : PlanStatus::Optimal;
    }
    plan.targetExpense = pick->expense;
    plan.availableAfter = pick->capacity;
    if (pick->pct != cur) {
        PlanAction a;
        a.kind = ActionKind::SetWaterFunding;
        a.capability = ActionCapability::Executable;
        a.fromPercent = cur;
        a.toPercent = pick->pct;
        a.monthlyDelta = pick->expense - plan.currentExpense;
        plan.actions.push_back(a);
    }

    // Deal cards for VISIBLE water opportunities (incremental economics on the global funding). Water has no latent probe and no
    // read-only opportunity capacity: a card is diagnostic only.
    const auto cheapestFor = [&](std::int64_t need) -> std::optional<std::int64_t> {
        std::optional<std::int64_t> best;
        for (const auto& o : options)
            if (o.capacity >= need && (!best || o.expense < *best)) best = o.expense;
        return best;
    };
    const std::int64_t baseNeed = plan.required;
    const auto baseCost = cheapestFor(baseNeed);
    for (const auto& o : s.deals.opportunities) {
        if ((o.type != utility_id::kDealSellWater && o.type != utility_id::kDealBuyWater) || o.quotes.empty() || !baseCost) continue;
        const bool sell = o.type == utility_id::kDealSellWater;
        DealCard c;
        c.sell = sell;
        c.resource = "WATER";
        c.visible = true;
        if (o.otherCitySerial) c.neighbor = *o.otherCitySerial;
        bool have = false;
        for (const auto& q : o.quotes) {
            if (o.maxAmount && q.first > *o.maxAmount) continue;
            const auto with = cheapestFor(sell ? baseNeed + q.first : std::max<std::int64_t>(0, baseNeed - q.first));
            if (!with) continue;
            const std::int64_t utility = sell ? *with - *baseCost : *baseCost - *with;   // sell: extra cost; buy: local saving
            const std::int64_t profit = sell ? q.second - utility : utility - q.second;
            if (!have || profit > c.profit) { c.amount = q.first; c.deal = q.second; c.utility = utility; c.profit = profit; have = true; }
        }
        c.calculable = have;
        if (!have) c.amount = o.quotes.front().first;
        plan.cards.push_back(c);
    }
    return plan;
}

// ---- deals (read-only recommendations) -----------------------------------------------------------------------
inline SubsystemPlan PlanDeals(const UtilitySnapshot& s, const OptimizerConfig& cfg, const SubsystemPlan& power, const SubsystemPlan& water,
                               std::int64_t garbageDisposalHeadroom) {
    SubsystemPlan plan;
    plan.status = PlanStatus::ReadOnly;
    const auto& d = s.deals;
    if (d.validity != SnapshotValidity::Valid && d.validity != SnapshotValidity::Partial) {
        plan.status = d.validity == SnapshotValidity::ValidEmpty ? PlanStatus::NoChange : PlanStatus::InvalidSnapshot;
        return plan;
    }
    if (!d.proposalsRead) { plan.notes.push_back("OPPORTUNITIES_NOT_READ_YET"); plan.status = PlanStatus::NotAvailable; return plan; }
    if (d.opportunities.empty()) { plan.notes.push_back("no vanilla opportunity offered (valid empty)"); return plan; }
    // Surplus per commodity after the plan and after the reserve; exports must be backed by local production only.
    const auto exports = ContractedExportsOf(d.deals, std::nullopt, std::nullopt, std::nullopt);
    std::int64_t surplus[3];
    surplus[0] = std::max<std::int64_t>(0, power.availableAfter - power.required);
    surplus[1] = std::max<std::int64_t>(0, water.availableAfter - water.required);
    surplus[2] = std::max<std::int64_t>(0, garbageDisposalHeadroom);
    (void)exports;
    struct Cand { const DealOpportunityRow* row; int idx; };
    std::vector<Cand> cands;
    for (const auto& o : d.opportunities) {
        if (!IsKnownDealType(o.type) || o.quotes.empty()) continue;
        const int idx = CommodityOfDealType(o.type) == Commodity::Power ? 0 : CommodityOfDealType(o.type) == Commodity::Water ? 1 : 2;
        // Only deals that bring in money for us: selling surplus, or receiving garbage (import).
        const bool sells = o.type == utility_id::kDealSellPower || o.type == utility_id::kDealSellWater || o.type == utility_id::kDealImportGarbage;
        if (sells) cands.push_back({&o, idx});
    }
    std::sort(cands.begin(), cands.end(), [](const Cand& a, const Cand& b) {
        return std::tie(a.row->type, a.row->otherCitySerial) < std::tie(b.row->type, b.row->otherCitySerial);
    });
    for (const auto& c : cands) {
        std::int64_t remaining = surplus[c.idx];
        const std::pair<std::int64_t, std::int64_t>* best = nullptr;
        for (const auto& q : c.row->quotes)
            if (q.first <= remaining && q.first <= (c.row->maxAmount ? *c.row->maxAmount : 0) && (!best || q.second > best->second)) best = &q;
        if (!best || best->second <= 0) continue;
        PlanAction a;
        a.kind = ActionKind::RecommendDeal;
        a.capability = ActionCapability::ReadOnlyRecommendation;
        a.dealType = c.row->type;
        a.neighbor = c.row->otherCitySerial;
        a.amount = best->first;
        a.monthlyDelta = best->second;
        a.note = c.row->type == utility_id::kDealImportGarbage
                     ? "READ_ONLY; current quote and max quantity only; price may change with the receiving city's capacity, not a guaranteed saving"
                     : "READ_ONLY; current quote from GetDealCost/GetDealMaxAmount; exports are backed by local surplus only; quote may change";
        plan.actions.push_back(a);
        surplus[c.idx] -= best->first;
        plan.targetExpense -= best->second;  // revenue lowers the net bill
    }
    return plan;
}

inline UtilityPlan PlanUtilities(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t) {
    UtilityPlan plan;
    plan.power = PlanPower(s, cfg, t);
    plan.water = PlanWater(s, cfg, t);
    plan.garbage = ComputeGarbageBalance(s.garbage.aggregate);
    plan.garbageHandling = ComputeGarbageHandling(s.garbage.aggregate, ContractedExportsOf(s.deals.deals, std::nullopt, std::nullopt, s.garbage.gameImported).garbage, cfg.garbageReserve);
    if (s.garbage.validity == SnapshotValidity::Invalid) plan.garbageStatus = PlanStatus::InvalidSnapshot;
    plan.garbageNotes.push_back("garbage control is read-only (no validated writer)");
    {
        // Landfill throughput is the CURRENT valve headroom, re-evaluated on every plan.
        GarbageServiceInput gi;
        gi.aggregate = s.garbage.aggregate;
        gi.deals = GarbageFlowsOf(s.deals.deals);
        gi.reserve = cfg.garbageReserve;
        gi.calendar = CalendarFromDayOfYear(static_cast<int>(s.simDayOfYear));
        for (auto& f : gi.deals)                     // last month's AmountUsed at the start of the month restarts at 0 (GarbageControl.h)
            if (f.usedKnown && StaleMonthStartAmountUsed(f.amount, f.used, gi.calendar.dayOfMonth)) { f.used = 0; f.usedRestarted = true; }
        gi.fundingCurve = t.powerEfficiency;   // 0x6911E2DF drives +0x228
        plan.garbageFunding = PlanGarbageFunding(gi, s.garbage.department.funding, s.garbage.department.maxAllowed, s.garbage.department.idealMonthly);
    }
    // Landfill storage is not monthly disposal throughput. Until the pipeline yields an authoritative surplus, no
    // garbage-deal capacity is claimed from it.
    plan.deals = PlanDeals(s, cfg, plan.power, plan.water, 0);
    return plan;
}

// ---- aggregate transition of one plan ----------------------------------------------------------------------------
// The power department funding is the cost-weighted average of the plant fundings, so it moves by at most the largest per-plant move;
// capacity moves much more because the efficiency curve is steep near 100%. Large moves are allowed.
struct TransitionSummary {
    std::int64_t capacityBefore = 0, capacityAfter = 0;
    double capacityDeltaPercent = 0.0;
    std::int64_t expenseDelta = 0;
    Opt<double> departmentFundingBefore, departmentFundingAfter;   // percent, cost-weighted over the controllable plants
    int changes = 0;
    int maxPointsMoved = 0;
};

inline TransitionSummary SummarizeTransition(const SubsystemPlan& plan) {
    TransitionSummary t;
    t.capacityBefore = plan.availableNow;
    t.capacityAfter = plan.availableAfter;
    if (plan.availableNow > 0)
        t.capacityDeltaPercent = 100.0 * static_cast<double>(plan.availableAfter - plan.availableNow) / static_cast<double>(plan.availableNow);
    t.expenseDelta = plan.targetExpense - plan.currentExpense;
    for (const auto& a : plan.actions) {
        if (a.capability != ActionCapability::Executable) continue;
        ++t.changes;
        t.maxPointsMoved = std::max(t.maxPointsMoved, std::abs(a.toPercent - a.fromPercent));
    }
    return t;
}

inline TransitionSummary SummarizePowerTransition(const UtilitySnapshot& s, const SubsystemPlan& plan) {
    TransitionSummary t = SummarizeTransition(plan);
    double costSum = 0.0, before = 0.0, after = 0.0;
    for (const auto& in : BuildPowerInputs(s)) {
        if (!in.controllable || in.nominalCost <= 0) continue;
        int target = in.currentPercent;
        for (const auto& a : plan.actions)
            if (a.kind == ActionKind::SetPlantFunding && a.cellX == in.ref.cellX && a.cellZ == in.ref.cellZ) target = a.toPercent;
        costSum += static_cast<double>(in.nominalCost);
        before += static_cast<double>(in.nominalCost) * in.currentPercent;
        after += static_cast<double>(in.nominalCost) * target;
    }
    if (costSum > 0.0) { t.departmentFundingBefore = before / costSum; t.departmentFundingAfter = after / costSum; }
    return t;
}

// Current utility expense with every occupant counted once (W2E appears in the power and the garbage list).
inline std::int64_t CurrentUtilityExpense(const UtilitySnapshot& s) {
    std::int64_t total = 0;
    std::vector<std::pair<int, int>> counted;
    const auto once = [&](const BuildingRef& r) {
        const std::pair<int, int> key{r.cellX, r.cellZ};
        if (std::find(counted.begin(), counted.end(), key) != counted.end()) return false;
        counted.push_back(key);
        return true;
    };
    for (const auto& p : s.power.plants)
        if (p.budgetLine && p.funding && once(p.ref)) total += ChargedExpense(p.budgetLine->cost, UtilityPercentRound(*p.funding));
    for (const auto& g : s.garbage.buildings)
        if (g.budgetLine && once(g.ref)) total += ChargedExpense(g.budgetLine->cost, UtilityPercentRound(g.budgetLine->localFunding));
    if (s.water.department.present && s.water.department.idealMonthly && s.water.department.funding)
        total += ChargedExpense(*s.water.department.idealMonthly, UtilityPercentRound(*s.water.department.funding));
    return total;
}

} // namespace sc4af
