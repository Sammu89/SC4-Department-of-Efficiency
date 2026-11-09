// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Landfill Size Savings Advisor: the pure counterfactual search (no game types, no I/O, no mutated globals, no second simulator).
//
// It answers one question: would REDUCING the landfill (removing empty tiles) save strictly more than the player's threshold (default 25%, 20..50) of the monthly sanitation expense?
// The Department never builds or removes anything; this only produces advice. Enlarging the landfill is not advised.
//
// Baseline E0   = ChargedExpense(idealMonthly, chosen funding of PlanGarbageFunding(base, ...)). This is the plan's own optimum, never the raw current
//                 funding (an unfinished reduction must not fake a saving). Not computable / not feasible / E0 <= 0 -> no advice.
// Candidate n   = scenario provided by the caller (LandfillScenario): its counterfactual input and its idealMonthly (the maintenance delta is applied
//                 once, there). E(n) = ChargedExpense(scenario.idealMonthly, chosen funding of PlanGarbageFunding(scenario.input, ...)), same funding /
//                 maxAllowed / frozen rules. Infeasible with the reserve (InsufficientCapacity / NotAvailable) -> not eligible. Contracts are inside the inputs
//                 and stay constant: no hypothetical contracts or revenue. Saving(n) = E0 - E(n); percent = floor(100 * saving / E0).
// Selection     : maximise Saving in simoleons; tie -> fewer tiles; tie -> lower initialCost when both known.
//                 Threshold: saving * 100 > T * E0 (strict, integers; T = the player's LandfillSavingsAlertThreshold, 20..50, default 25). One single
//                 recommendation. Savings per tile are not monotone: the whole domain is scanned, the search never stops at the first candidate over the
//                 threshold. initialCost is reported apart, never in the saving.
// Domain/budget : at most 64 tiles, 64 counterfactual evaluations per run (candidates; the baseline plan is not counted), target <= 10 ms. The whole budget belongs to REDUCE.
//                 N = min(limit, 64). N <= budget: every n in 1..N. N > budget: a deterministic schedule: a coarse grid of budget/4 points ceil(i*N/c) (i=1..c, ends at N), then the
//                 contiguous prefix 1,2,3,... (skipping already evaluated points) until the budget is used.
//                 Optimality: the report gives K = the largest k such that every n in 1..k was evaluated and demonstrable (validated). Only candidates
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

constexpr int kLandfillMaxTiles = 64;
constexpr int kLandfillMaxEvaluations = 64;
constexpr int kLandfillSavingThresholdPercent = 25;   // the DEFAULT threshold: advice only when saving% is STRICTLY above it (the player chooses 20..50, LandfillSearchOptions::thresholdPercent)

enum class LandfillDirection { None, Reduce };
inline const char* LandfillDirectionName(LandfillDirection d) { return d == LandfillDirection::Reduce ? "REDUCE" : "NONE"; }

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
    int maxTiles = kLandfillMaxTiles;                           // clamped to [0, 64]
    int totalBudget = kLandfillMaxEvaluations;                  // counterfactual evaluations per run, clamped to [0, 64]
    std::chrono::microseconds timeBudget{10000};                // CPU/time target of one run
    std::function<std::chrono::steady_clock::time_point()> clock;   // injectable monotonic clock; empty = std::chrono::steady_clock::now
    int thresholdPercent = kLandfillSavingThresholdPercent;     // LandfillSavingsAlertThreshold (UtilityConfig.h): clamped to [20, 50] by the search; it only decides whether to advise
    // The caller may lower both limits for this run from an AdaptiveBudget.
    void ApplyAdaptive(const AdaptiveBudget& a) {
        totalBudget = std::min(totalBudget, a.cap);
        maxTiles = std::min(maxTiles, a.cap);
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
    int domain = 0;                      // N = min(limit, maxTiles)
    int share = 0;                       // evaluations allotted (the whole budget, up to N)
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
    LandfillDirectionReport report;      // the REDUCE search
    int evaluations = 0;                 // counterfactual candidates evaluated (<= 64)
    bool timeBudgetExceeded = false;     // interrupted by, or finished over, the time budget: feed AdaptiveBudget::Record
    bool interrupted = false;            // the schedule was cut short (time or work cap)
    std::vector<LandfillCandidate> candidates;       // diagnostics: every evaluated candidate, in evaluation order
};

namespace landfill_detail {

// The deterministic schedule of tile counts (see the file header).
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

// `provider(int delta)` returns the scenario for delta = -n (n tiles removed): any type with the fields of sc4af::LandfillScenario (demonstrable, reason,
// input, idealMonthly, initialCost). `maxReduce` is LandfillScenarioLimits::maxReduce (0 = disabled).
template <class Provider>
LandfillAdvice AdviseLandfillSize(const GarbageServiceInput& base, Opt<float> funding, Opt<float> maxAllowed, Opt<std::int64_t> idealMonthly, int maxReduce,
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

    const int cap = std::clamp(options.maxTiles, 0, kLandfillMaxTiles);
    const int budget = std::clamp(options.totalBudget, 0, kLandfillMaxEvaluations);
    const int domain = std::clamp(maxReduce, 0, cap);
    adv.report.limit = std::max(0, maxReduce);
    adv.report.domain = domain;
    if (domain == 0) { adv.reason = "NO_DIRECTION"; return adv; }
    adv.report.share = std::min(domain, budget);
    const std::vector<int> sched = landfill_detail::Schedule(domain, adv.report.share);

    std::map<int, LandfillCandidate> byTiles;
    const auto candStart = now();
    for (const int n : sched) {
        if (adv.evaluations >= kLandfillMaxEvaluations || adv.evaluations >= budget) { adv.interrupted = true; break; }
        const auto t = now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(t - start);
        if (elapsed >= options.timeBudget) { adv.interrupted = true; adv.timeBudgetExceeded = true; break; }
        if (adv.evaluations > 0) {
            const auto spent = std::chrono::duration_cast<std::chrono::microseconds>(t - candStart);
            const auto mean = spent / adv.evaluations;
            if (elapsed + mean > options.timeBudget) { adv.interrupted = true; adv.timeBudgetExceeded = true; break; }
        }
        const auto sc = provider(-n);
        ++adv.evaluations;
        ++adv.report.evaluated;
        LandfillCandidate c;
        c.direction = LandfillDirection::Reduce;
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
                ++adv.report.eligible;
            }
        }
        adv.candidates.push_back(c);
        byTiles[n] = c;
    }
    {
        const auto end = now();
        if (std::chrono::duration_cast<std::chrono::microseconds>(end - start) > options.timeBudget) adv.timeBudgetExceeded = true;
    }

    int validPrefix = 0;
    while (validPrefix < domain) {
        const auto it = byTiles.find(validPrefix + 1);
        if (it == byTiles.end() || !it->second.demonstrable) break;
        ++validPrefix;
    }
    adv.report.fullyEvaluatedTo = validPrefix;
    adv.report.complete = domain > 0 && validPrefix == domain;
    if (validPrefix == 0) { adv.reason = "NO_VALIDATED_DOMAIN"; return adv; }

    // Best candidate inside the fully evaluated and validated interval.
    const LandfillCandidate* best = nullptr;
    const auto better = [](const LandfillCandidate& a, const LandfillCandidate& b) {   // is a better than b
        if (a.saving != b.saving) return a.saving > b.saving;
        if (a.tiles != b.tiles) return a.tiles < b.tiles;
        return a.initialCost && b.initialCost && *a.initialCost < *b.initialCost;
    };
    for (int n = 1; n <= validPrefix; ++n) {
        const auto& c = byTiles[n];
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
    return AdviseLandfillSize(base, funding, maxAllowed, idealMonthly, lim.maxReduce,
                              [&](int delta) { return BuildLandfillScenario(base, idealMonthly, delta); }, options);
}
}  // namespace sc4af
