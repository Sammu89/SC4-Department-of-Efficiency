// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Landfill Size Savings Advisor: the pure counterfactual search (no game types, no I/O, no mutated globals, no second simulator).
//
// It answers one question: would changing the landfill size (EXPAND = add tiles, REDUCE = remove empty tiles) save strictly more than the player's
// threshold (default 25%, 20..50) of the monthly sanitation expense? The Department never builds or removes anything; this only produces advice.
//
// Baseline E0   = ChargedExpense(idealMonthly, chosen funding of PlanGarbageFunding(base, ...)). This is the plan's own optimum, never the raw current
//                 funding (an unfinished reduction must not fake a saving). Not computable / not feasible / E0 <= 0 -> no advice.
// Candidate d   = scenario provided by the caller (LandfillScenario): its counterfactual input and its idealMonthly (the maintenance delta is applied
//                 once, there). E(d) = ChargedExpense(scenario.idealMonthly, chosen funding of PlanGarbageFunding(scenario.input, ...)), same funding /
//                 maxAllowed / frozen rules. Infeasible with the reserve (InsufficientCapacity / NotAvailable) -> not eligible. Contracts are inside the inputs
//                 and stay constant: no hypothetical contracts or revenue. Saving(d) = E0 - E(d); percent = floor(100 * saving / E0).
// Selection     : maximise Saving in simoleons; tie -> smaller |delta|; tie -> lower initialCost when both known; tie -> EXPAND before REDUCE.
//                 Threshold: saving * 100 > T * E0 (strict, integers; T = the player's LandfillSavingsAlertThreshold, 20..50, default 25). One single
//                 recommendation. Savings per tile are not monotone: the whole domain is scanned, the search never stops at the first candidate over the
//                 threshold. initialCost is reported apart, never in the saving.
// Domain/budget : at most 64 tiles per direction, 64 counterfactual evaluations per run (candidates; the baseline plan is not counted), target <= 10 ms.
//                 Per-direction domain N = min(limit, 64) (limit 0 = direction never evaluated). Budget split: 32/32 by default; a direction that needs
//                 less than its share (N < share) frees the difference for the other one (EXPAND first, then REDUCE, never beyond its own N). With a single
//                 direction it gets the whole budget.
//                 N <= share: every n in 1..N. N > share: a deterministic schedule: a coarse grid of share/4 points ceil(i*N/c) (i=1..c, ends at N), then the
//                 contiguous prefix 1,2,3,... (skipping already evaluated points) until the share is used. The two directions are interleaved
//                 (E, R, E, R, ...) in that schedule so that a time cut starves neither.
//                 Optimality: a direction reports K = the largest k such that every n in 1..k was evaluated and demonstrable (validated). Only candidates
//                 n <= K may be recommended; `complete` = (K == N). A coarse point beyond K is exploratory only (reported in the candidates list, never advised).
// Interruption  : cooperative, before starting each candidate: (1) work cap (evaluation count), (2) the injectable monotonic clock: stop if elapsed >= budget,
//                 or if elapsed + mean cost of the candidates done so far would exceed it. Nothing is ever interrupted in the middle of a candidate.
// AdaptiveBudget: see the struct below (64 -> 48 -> 32 -> 16 -> 8 -> 4 -> 2 when a run exceeded the time budget).

#include "GarbageControl.h"
#include "LandfillScenario.h"
#include "UtilityConfig.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

namespace sc4af {

constexpr int kLandfillMaxTilesPerDirection = 64;
constexpr int kLandfillMaxEvaluations = 64;
constexpr int kLandfillSavingThresholdPercent = 25;   // the DEFAULT threshold: advice only when saving% is STRICTLY above it (the player chooses 20..50, LandfillSearchOptions::thresholdPercent)

enum class LandfillDirection { None, Expand, Reduce };
inline const char* LandfillDirectionName(LandfillDirection d) {
    switch (d) {
    case LandfillDirection::Expand: return "EXPAND";
    case LandfillDirection::Reduce: return "REDUCE";
    default: return "NONE";
    }
}

// Shrinks the per-run budget for future runs after a run that exceeded the time budget. Steps: 64 -> 48 -> 32 -> 16 -> 8 -> 4 -> 2 (floor 2). Once per
// exceeded run, one step; a run inside the budget keeps the current cap (no automatic growth: Reset() is the caller's decision).
struct AdaptiveBudget {
    int cap = kLandfillMaxEvaluations;
    static int NextLower(int c) {
        if (c > 48) return 48;
        if (c > 32) return 32;
        if (c > 16) return 16;
        if (c > 8) return 8;
        if (c > 4) return 4;
        return 2;
    }
    static int NextHigher(int c) {
        if (c < 4) return 4;
        if (c < 8) return 8;
        if (c < 16) return 16;
        if (c < 32) return 32;
        if (c < 48) return 48;
        return kLandfillMaxEvaluations;
    }
    // A run is measured on the wall clock, so one hitch (a context switch, a loading stall) must not shrink the search for good: the cap steps down
    // only after kSlowRunsToStep slow runs in a row, and recovers one step after kFastRunsToRecover runs inside the budget.
    static constexpr int kSlowRunsToStep = 2;
    static constexpr int kFastRunsToRecover = 6;
    int slowRuns = 0, fastRuns = 0;
    void Record(bool exceededTimeBudget) {
        if (exceededTimeBudget) {
            fastRuns = 0;
            if (++slowRuns >= kSlowRunsToStep) { cap = NextLower(cap); slowRuns = 0; }
        } else {
            slowRuns = 0;
            if (cap < kLandfillMaxEvaluations && ++fastRuns >= kFastRunsToRecover) { cap = NextHigher(cap); fastRuns = 0; }
        }
    }
    void Reset() { cap = kLandfillMaxEvaluations; slowRuns = fastRuns = 0; }
};

struct LandfillSearchOptions {
    int maxTilesPerDirection = kLandfillMaxTilesPerDirection;   // clamped to [0, 64]
    int totalBudget = kLandfillMaxEvaluations;                  // counterfactual evaluations per run, clamped to [0, 64]
    std::chrono::microseconds timeBudget{10000};                // CPU/time target of one run
    std::function<std::chrono::steady_clock::time_point()> clock;   // injectable monotonic clock; empty = std::chrono::steady_clock::now
    int thresholdPercent = kLandfillSavingThresholdPercent;     // LandfillSavingsAlertThreshold (UtilityConfig.h): clamped to [20, 50] by the search; it only decides whether to advise
    // The caller may lower both limits for this run from an AdaptiveBudget.
    void ApplyAdaptive(const AdaptiveBudget& a) {
        totalBudget = std::min(totalBudget, a.cap);
        maxTilesPerDirection = std::min(maxTilesPerDirection, a.cap);
    }
};

struct LandfillCandidate {
    LandfillDirection direction = LandfillDirection::None;
    int tiles = 0;                       // |delta|
    bool demonstrable = false;
    std::string reason;                  // why not demonstrable / not eligible
    bool eligible = false;               // demonstrable and feasible with the reserve
    int percent = 0;                     // chosen funding
    std::int64_t expense = 0;            // E(d)
    std::int64_t saving = 0;             // E0 - E(d) (can be <= 0)
    int savingPercent = 0;               // floor(100 * saving / E0) when saving > 0, else 0
    Opt<std::int64_t> initialCost;
};

struct LandfillDirectionReport {
    int limit = 0;                       // LandfillScenarioLimits maximum (0 = disabled)
    int domain = 0;                      // N = min(limit, maxTilesPerDirection)
    int share = 0;                       // evaluations allotted to this direction
    int evaluated = 0;                   // candidates actually evaluated
    int eligible = 0;                    // feasible and demonstrable among them
    int fullyEvaluatedTo = 0;            // K: every n in 1..K evaluated and demonstrable
    bool complete = false;               // K == N and N > 0: the best n in 1..N is the true optimum of the domain
};

struct LandfillAdvice {
    bool hasAdvice = false;
    LandfillDirection direction = LandfillDirection::None;
    int tiles = 0;                       // recommended n (positive)
    std::int64_t saving = 0;             // monthly, conservative (E0 - E)
    int savingPercent = 0;               // floor
    std::int64_t baselineExpense = 0;    // E0
    std::int64_t expenseAfter = 0;
    int baselinePercent = 0;
    int percentAfter = 0;
    Opt<std::int64_t> initialCost;       // one-off, separate from the saving
    Opt<std::int64_t> paybackMonths;     // ceil(initialCost / saving), informational; only when initialCost known and saving > 0
    std::string reason;                  // stable code when !hasAdvice, "ADVICE" otherwise
    std::array<LandfillDirectionReport, 2> report;   // [0] = EXPAND, [1] = REDUCE
    int evaluations = 0;                 // counterfactual candidates evaluated (<= 64)
    bool timeBudgetExceeded = false;     // interrupted by, or finished over, the time budget: feed AdaptiveBudget::Record
    bool interrupted = false;            // the schedule was cut short (time or work cap)
    std::vector<LandfillCandidate> candidates;       // diagnostics: every evaluated candidate, in evaluation order
};

namespace landfill_detail {

inline int ShareOf(int budget, int nExpand, int nReduce, int dir) {
    if (nExpand > 0 && nReduce > 0) {
        int sE = std::min(nExpand, budget / 2), sR = std::min(nReduce, budget - budget / 2);
        int left = budget - sE - sR;
        const int giveE = std::min(left, nExpand - sE); sE += giveE; left -= giveE;
        const int giveR = std::min(left, nReduce - sR); sR += giveR;
        return dir == 0 ? sE : sR;
    }
    const int n = nExpand > 0 ? nExpand : nReduce;
    const bool mine = dir == 0 ? nExpand > 0 : nReduce > 0;
    return mine ? std::min(n, budget) : 0;
}

// The deterministic per-direction schedule of tile counts (see the file header).
inline std::vector<int> Schedule(int domain, int share) {
    std::vector<int> out;
    if (domain <= 0 || share <= 0) return out;
    if (domain <= share) { for (int n = 1; n <= domain; ++n) out.push_back(n); return out; }
    std::vector<char> seen(static_cast<size_t>(domain) + 1, 0);
    const auto add = [&](int n) { if (n >= 1 && n <= domain && !seen[n] && static_cast<int>(out.size()) < share) { seen[n] = 1; out.push_back(n); } };
    const int c = std::max(1, share / 4);
    for (int i = 1; i <= c; ++i) add(static_cast<int>((static_cast<long long>(i) * domain + c - 1) / c));
    for (int n = 1; n <= domain && static_cast<int>(out.size()) < share; ++n) add(n);
    return out;
}

}  // namespace landfill_detail

// `provider(int delta)` returns the scenario for delta (+n expand, -n reduce): any type with the fields of sc4af::LandfillScenario (demonstrable, reason,
// input, idealMonthly, initialCost). `maxExpand` / `maxReduce` are LandfillScenarioLimits (0 = direction disabled).
template <class Provider>
LandfillAdvice AdviseLandfillSize(const GarbageServiceInput& base, Opt<float> funding, Opt<float> maxAllowed, Opt<std::int64_t> idealMonthly, int maxExpand, int maxReduce,
                                  Provider&& provider, const LandfillSearchOptions& options = LandfillSearchOptions{}) {
    LandfillAdvice adv;
    adv.reason = "NO_BASELINE";
    const auto now = [&]() { return options.clock ? options.clock() : std::chrono::steady_clock::now(); };
    const auto start = now();

    const auto basePlan = PlanGarbageFunding(base, funding, maxAllowed, idealMonthly);
    if (basePlan.status != GarbagePlanStatus::Optimal && basePlan.status != GarbagePlanStatus::NoChange) {
        if (basePlan.status == GarbagePlanStatus::InsufficientCapacity) adv.reason = "BASELINE_INFEASIBLE";
        return adv;
    }
    adv.baselinePercent = basePlan.targetPercent;
    adv.baselineExpense = ChargedExpense(*idealMonthly, basePlan.targetPercent);
    if (adv.baselineExpense <= 0) { adv.reason = "BASELINE_NOT_POSITIVE"; return adv; }
    const std::int64_t e0 = adv.baselineExpense;

    const int cap = std::clamp(options.maxTilesPerDirection, 0, kLandfillMaxTilesPerDirection);
    const int budget = std::clamp(options.totalBudget, 0, kLandfillMaxEvaluations);
    const int limits[2] = {maxExpand, maxReduce};
    int domain[2];
    for (int d = 0; d < 2; ++d) { domain[d] = std::clamp(limits[d], 0, cap); adv.report[d].limit = std::max(0, limits[d]); adv.report[d].domain = domain[d]; }
    if (domain[0] == 0 && domain[1] == 0) { adv.reason = "NO_DIRECTION"; return adv; }
    std::vector<int> sched[2];
    for (int d = 0; d < 2; ++d) {
        adv.report[d].share = landfill_detail::ShareOf(budget, domain[0], domain[1], d);
        sched[d] = landfill_detail::Schedule(domain[d], adv.report[d].share);
    }
    // interleave E, R, E, R ...
    std::vector<std::pair<int, int>> order;   // (direction index, tiles)
    for (size_t i = 0; i < std::max(sched[0].size(), sched[1].size()); ++i)
        for (int d = 0; d < 2; ++d) if (i < sched[d].size()) order.emplace_back(d, sched[d][i]);

    std::map<int, LandfillCandidate> byTiles[2];
    const auto candStart = now();
    for (const auto& item : order) {
        if (adv.evaluations >= kLandfillMaxEvaluations || adv.evaluations >= budget) { adv.interrupted = true; break; }
        const auto t = now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t - start);
        if (elapsed >= options.timeBudget) { adv.interrupted = true; adv.timeBudgetExceeded = true; break; }
        if (adv.evaluations > 0) {
            const auto spent = std::chrono::duration_cast<std::chrono::microseconds>(t - candStart);
            const auto mean = spent / adv.evaluations;
            if (elapsed + mean > options.timeBudget) { adv.interrupted = true; adv.timeBudgetExceeded = true; break; }
        }
        const int d = item.first, n = item.second;
        const int delta = d == 0 ? n : -n;
        const auto sc = provider(delta);
        ++adv.evaluations;
        ++adv.report[d].evaluated;
        LandfillCandidate c;
        c.direction = d == 0 ? LandfillDirection::Expand : LandfillDirection::Reduce;
        c.tiles = n;
        c.demonstrable = sc.demonstrable;
        c.initialCost = sc.initialCost;
        if (!sc.demonstrable) {
            c.reason = sc.reason.empty() ? "NOT_DEMONSTRABLE" : sc.reason;
        } else if (!sc.idealMonthly) {
            c.reason = "NO_IDEAL_EXPENSE";
        } else {
            const auto plan = PlanGarbageFunding(sc.input, funding, maxAllowed, sc.idealMonthly);
            if (plan.status != GarbagePlanStatus::Optimal && plan.status != GarbagePlanStatus::NoChange) {
                c.reason = "INFEASIBLE_WITH_RESERVE";
            } else {
                c.eligible = true;
                c.percent = plan.targetPercent;
                c.expense = ChargedExpense(*sc.idealMonthly, plan.targetPercent);
                c.saving = e0 - c.expense;
                if (c.saving > 0) c.savingPercent = static_cast<int>((c.saving * 100) / e0);
                ++adv.report[d].eligible;
            }
        }
        adv.candidates.push_back(c);
        byTiles[d][n] = c;
    }
    {
        const auto end = now();
        if (std::chrono::duration_cast<std::chrono::microseconds>(end - start) > options.timeBudget) adv.timeBudgetExceeded = true;
    }

    int validPrefix[2] = {0, 0};
    for (int d = 0; d < 2; ++d) {
        int k = 0;
        while (k < domain[d]) {
            const auto it = byTiles[d].find(k + 1);
            if (it == byTiles[d].end() || !it->second.demonstrable) break;
            ++k;
        }
        validPrefix[d] = k;
        adv.report[d].fullyEvaluatedTo = k;
        adv.report[d].complete = domain[d] > 0 && k == domain[d];
    }
    if (validPrefix[0] == 0 && validPrefix[1] == 0) { adv.reason = "NO_VALIDATED_DOMAIN"; return adv; }

    // Best candidate inside the fully evaluated and validated intervals.
    const LandfillCandidate* best = nullptr;
    const auto better = [](const LandfillCandidate& a, const LandfillCandidate& b) {   // is a better than b
        if (a.saving != b.saving) return a.saving > b.saving;
        if (a.tiles != b.tiles) return a.tiles < b.tiles;
        if (a.initialCost && b.initialCost && *a.initialCost != *b.initialCost) return *a.initialCost < *b.initialCost;
        return a.direction == LandfillDirection::Expand && b.direction == LandfillDirection::Reduce;
    };
    for (int d = 0; d < 2; ++d)
        for (int n = 1; n <= validPrefix[d]; ++n) {
            const auto& c = byTiles[d][n];
            if (!c.eligible || c.saving <= 0) continue;
            if (!best || better(c, *best)) best = &c;
        }
    if (!best) { adv.reason = "NO_SAVING"; return adv; }
    const int threshold = std::clamp(options.thresholdPercent, kLandfillAlertThresholdMin, kLandfillAlertThresholdMax);
    if (!(best->saving * 100 > static_cast<std::int64_t>(threshold) * e0)) { adv.reason = "BELOW_THRESHOLD"; return adv; }
    adv.hasAdvice = true;
    adv.reason = "ADVICE";
    adv.direction = best->direction;
    adv.tiles = best->tiles;
    adv.saving = best->saving;
    adv.savingPercent = best->savingPercent;
    adv.expenseAfter = best->expense;
    adv.percentAfter = best->percent;
    adv.initialCost = best->initialCost;
    if (best->initialCost && *best->initialCost >= 0 && best->saving > 0)
        adv.paybackMonths = (*best->initialCost + best->saving - 1) / best->saving;
    return adv;
}

}  // namespace sc4af

// The search over the real scenario builder (LandfillScenario.h).
namespace sc4af {
inline LandfillAdvice AdviseLandfillSize(const GarbageServiceInput& base, Opt<float> funding, Opt<float> maxAllowed, Opt<std::int64_t> idealMonthly,
                                         const LandfillSearchOptions& options = LandfillSearchOptions{}) {
    const auto lim = LandfillScenarioLimits(base, idealMonthly);
    return AdviseLandfillSize(base, funding, maxAllowed, idealMonthly, lim.maxExpand, lim.maxReduce,
                              [&](int delta) { return BuildLandfillScenario(base, idealMonthly, delta); }, options);
}
}  // namespace sc4af
