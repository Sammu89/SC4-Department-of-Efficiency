// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Neighbor Deal portfolio optimizer for the six directions:
//   POWER BUY / SELL, WATER BUY / SELL, GARBAGE EXPORT (we pay a neighbor to dispose) / IMPORT (a neighbor pays us to dispose).
// Stateless: no hold, no cooldown, no memory of a rejection, no preference for a contract because it exists. It only produces DealWritePlan
// structures for the transaction layer; nothing is executed here.
//
// Objective per commodity (monthly money, strict):
//     maximise   sum(sell-like revenue) - sum(buy-like cost) - LocalCost(ownNeed + sells - buys)
//   LocalCost is the economic cost of the local side: cash operating expense plus, for Power with ConsiderPowerAging, the capital wear of the plants.
//   Deal money and the simulator ledger are cash only; wear never enters a treasury figure (cash margins are reported next to the economic ones).
//   BUY xor SELL: unless AllowUtilityArbitrage (option, off by default) a commodity ends the plan in exactly one state, LOCAL, BUY or SELL; for
//   Garbage IMPORT is sell-like and EXPORT buy-like, so it never ends with both. With AllowUtilityArbitrage the portfolio may hold both sides (buy from A, sell to B)
//   when that is the best net: the same objective, caps and own-city feasibility apply, so a combination is only taken for its profit. A suspended (fixed) obligation
//   on one side blocks new deals on the other; if the own city cannot be served that way the service wins, no new sell is created and a POLICY_CONFLICT is reported.
//   * existing contracts keep their STORED cost at their current amount (never re-priced); any other amount, and every new
//     candidate, is priced with the exact vanilla function (DealQuote.h) on the cached records;
//   * sell-like = SELL_POWER, SELL_WATER, IMPORT_GARBAGE (they add local load); buy-like = BUY_POWER, BUY_WATER, EXPORT_GARBAGE;
//   * no re-export: the sum of buy-like amounts may not exceed the city's own need (a purchase can never back a sale);
//   * LocalExpense is the existing solver for the commodity (SolvePower / water options / Sanitation funding), which already
//     carries the configured reserve headroom, physical condition, pipe health and the W2E pin, so nothing is double counted;
//   * a deal that does not add strictly more than it costs is not in the optimum: ties are broken by fewer deals, then fewer changes.
// Quotes are first-order (current cache); the executor confirms every create with a fresh quote.

#include "DealAccounting.h"
#include "DealQuantity.h"
#include "DealQuote.h"
#include "GarbageControl.h"
#include "NeighborDealWriterPlan.h"
#include "SafeMoney.h"
#include "SanitationWriterPlan.h"
#include "UtilityOptimizer.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace sc4af {

enum class PortfolioActionKind { Create, Increase, Decrease, Cancel };

inline const char* PortfolioActionKindName(PortfolioActionKind k) {
    switch (k) {
    case PortfolioActionKind::Create: return "CREATE";
    case PortfolioActionKind::Increase: return "INCREASE";
    case PortfolioActionKind::Decrease: return "DECREASE";
    case PortfolioActionKind::Cancel: return "CANCEL";
    }
    return "?";
}

// Execution order, per commodity. A phase starts only when every earlier phase of that
// commodity has nothing left to do in the freshly replanned state:
//   0 local capacity RAISES (funding up, to the capacity of the FINAL state)   1 sell-like DECREASE / CANCEL
//   2 buy-like CREATE / INCREASE                                               3 sell-like CREATE / INCREASE
//   4 local capacity CUTS                                                      5 buy-like DECREASE / CANCEL
// Intermediate-state safety: with S sell-like load and B buy-like relief the local need is own + S - B. Phase 1 only lowers S and phase 2
// only raises B, so the need never exceeds the starting one; phase 3 starts with S <= S_final (decreases already done) and B >= B_final
// (increases done, decreases not yet), so the need never exceeds the final need that phase 0 already covered; phases 4 and 5 start from
// S = S_final and B >= B_final, so a cut to the final capacity and the last buy reductions never leave the need above the final need.
// Sell-up before sell-down would be unsafe: it could exceed both the initial and the final need when sells move in both directions.
constexpr int kPhaseCapacityRaise = 0, kPhaseSellDown = 1, kPhaseBuyUp = 2, kPhaseSellUp = 3, kPhaseCapacityCut = 4, kPhaseBuyDown = 5;

struct PortfolioOption {
    std::int32_t amount = 0;       // 0 = no contract
    std::int64_t money = 0;        // sell-like: revenue (+); buy-like: cost (positive number, subtracted)
    bool latent = false;           // NOT offered by the game right now: it needs local capacity first (the local plan raises it; no deal write yet)
    std::int64_t quoteNow = 0;     // what the game answers for this amount right now (QuoteCurrentState); `money` is the post-deal state price
};

// Books a quantity is priced on: the BASE state (our changeable contracts removed). The DP values every option on them, so the optimum does not
// depend on which contracts happen to be active; the chosen portfolio is then re-priced jointly (JointOptionMoney) and a deal that is no longer
// strictly profitable in the complete state is dropped.
struct PortfolioPriceModel {
    bool valid = false;
    CommodityBook localBase, nbBase;
    double minAmount = 0.0;
};

struct PortfolioCandidate {
    DealKey key;
    PortfolioPriceModel price;
    bool sellLike = false;
    bool existing = false;         // an ACTIVE contract row exists (its amount may be 0: a dormant contract)
    bool rowActive = false;        // the row is ACTIVE (flag 0x02): amount changes are CHANGE_AMOUNT, never CREATE, even from amount 0
    bool fixed = false;            // suspended: counts as an obligation, cannot be changed
    std::int32_t currentAmount = 0;
    std::int64_t storedCost = 0;
    std::int32_t ladderStep = 0;   // canonical (state-independent) ladder of a Power/Water SELL: amounts are multiples of this; 0 = the vanilla ladder of the live row
    std::int32_t ladderMax = 0;
    std::optional<std::int64_t> amountUsed;  // Garbage: AmountUsed of the live row (nullopt: unknown or no row); never replaced by 0
    bool completedThisMonth = false;         // Garbage: a live changeable contract with AmountUsed >= Amount > 0 (its schedule is exhausted until the rollover)
    std::vector<PortfolioOption> options;   // ascending amount; includes 0 for changeable candidates and the current amount when existing
};

using LocalExpenseFn = std::function<std::optional<std::int64_t>(std::int64_t need)>;

// One changeable contract of a complete pick: its key and the amount the portfolio gives it.
struct PortfolioChoice {
    DealKey key;
    bool sellLike = false;
    std::int64_t amount = 0;
};
// The local expense of a COMPLETE pick (every changeable contract with its own amount). Commodities whose local cost depends on WHICH contract carries
// an amount (Garbage: every contract has its own AmountUsed and delivery schedule) provide it; the solver then evaluates complete picks exactly
// instead of an aggregate need.
using CompositionExpenseFn = std::function<std::optional<std::int64_t>(const std::vector<PortfolioChoice>& pick)>;

struct PortfolioCommodity {
    Commodity commodity = Commodity::None;
    bool available = false;
    std::string unavailableReason;
    std::int64_t ownNeed = 0;
    // The local need as a function of the planned purchases and sales when it is NOT ownNeed + sells - buys (Power / Water: the reserve covers only the part produced locally,
    // LocalRequired = ceil(R x max(0, D - B)) + S). `sell` is already the load that counts (sellsLoadLocal applied).
    std::function<std::int64_t(std::int64_t buy, std::int64_t sell)> localNeed;
    std::int64_t demand = -1;             // the effective demand the plan was built on (-1 = unknown): logged with every deal decision so a flip can be explained from one line
    std::int64_t localCapacityNow = -1;   // the local capacity CONFIRMED at the current funding (-1 = unknown): decides which changes may run before the others without touching the service
    std::int64_t buyCap = 0;           // sum of buy-like amounts may not exceed this (no re-export)
    std::int64_t minBenefit = 0;       // a FACULTATIVE deal must improve the net by at least this much ($/month); a deal the service needs (removal infeasible) is exempt
    std::optional<std::int64_t> sellCap;   // extra cap on the sum of sell-like amounts (e.g. the IMPORT_GARBAGE contract limit)
    std::int64_t fixedSell = 0;        // suspended sell-like obligations (always in the need)
    LocalExpenseFn localExpense;       // minimal ECONOMIC cost (cash + wear) for a local need; nullopt = cannot be met. The optimum is chosen on it.
    CompositionExpenseFn compositionExpense;   // when set it REPLACES localExpense for every state (the bucketed need search is not used); same economic meaning
    CompositionExpenseFn compositionCash;      // the cash counterpart (absent = compositionExpense)
    LocalExpenseFn localCash;          // the CASH expense of the same state (optional: absent = the economic cost is cash). Reporting and the ledger only.
    std::int64_t granularity = 100;
    bool signedNeed = false;           // the need axis is a CHANGE from the no-deal state and may be negative (Garbage: exports relieve load)
    bool sellsLoadLocal = true;        // true: a sale raises the local requirement (Power / Water sells, Garbage IMPORT: the foreign load must fit the funding chosen for it)
    bool allowBuyAndSell = false;      // AllowUtilityArbitrage: false = BUY xor SELL
    std::function<std::int64_t(std::int64_t buyTotal)> sellCapByBuy;   // optional: the most sell-like amount allowed for the exports chosen (none used by the current commodities)
    std::function<std::string(std::int64_t buyTotal)> policyNote;      // human-readable budget behind sellCapByBuy (logging only)
    DealMarketTuning pricingTuning;    // for the joint re-pricing of the chosen portfolio
    bool internalSubproblem = false;   // a counterfactual / mode sub-solve: no marginals, no continuity, no mode table
    bool roundingMargin = false;       // the bucketed search keeps (granularity - 1) per purchase below buyCap; only used when the exact first pass overshoots buyCap
    bool continuityLock = false;       // Garbage: a contract whose schedule is exhausted is kept until the rollover (see SolvePortfolio)
    bool wantModeResults = false;      // also solve LOCAL / BUY / SELL separately (diagnostics)
    std::vector<PortfolioCandidate> candidates;
};

struct PortfolioDecision {
    DealKey key;
    bool sellLike = false;
    std::int32_t from = 0, to = 0;
    // Net monthly ECONOMIC benefit of this decision given the others (cash plus wear for Power). It is REAL MONEY only when `marginalKnown`: when
    // reverting this single change leaves a portfolio the local side cannot serve there is no monetary comparison (the change is
    // `requiredForFeasibility`), and `marginal` is 0 and must not be read as a price, a saving or a profit.
    std::int64_t marginal = 0;
    std::int64_t cashMarginal = 0;     // the same comparison in CASH only (what the simulator will show); equals `marginal` for Water and Garbage
    bool marginalKnown = true;
    bool requiredForFeasibility = false;   // the alternative (this change reverted) is infeasible: the change creates / restores feasibility
    std::int64_t fromMoney = 0, toMoney = 0;   // money of the current / target option (stored cost for the current amount, modelled quote otherwise)
    bool rowActive = false;            // an ACTIVE row exists (amount may be 0): a change is CHANGE_AMOUNT
    std::int32_t ladderStep = 0, ladderMax = 0;
    std::int64_t toQuoteNow = 0;       // game answer for the target amount before the write (QuoteCurrentState)
    bool dormant = false;              // an ACTIVE row at amount 0 (what ProposeDeals(list,true) - the game's own deal window - leaves behind) that costs its stored value (1 $/month) and carries nothing: it is cancelled
    bool latent = false;               // the target is a LATENT option: the capacity is planned, the deal itself is written after the capacity exists
    // The counterfactual is the best POLICY-LEGAL portfolio with THIS candidate forced to its current amount (BUY xor SELL, caps, feasibility, fixed
    // obligations and the per-contract Garbage schedule all apply). marginal = optimum - counterfactual, so a known marginal is never negative.
    bool counterfactualLegal = false;      // such a portfolio exists
    std::string counterfactualMode;        // LOCAL / BUY / SELL of that portfolio
    std::int64_t counterfactualNet = 0, counterfactualCashNet = 0;
    std::string requiredReason;            // "" | FEASIBILITY (no legal portfolio even with arbitrage) | POLICY_TRANSITION (only an illegal BUY+SELL would keep it)
    bool counterfactualBeatOptimum = false;   // the sub-solve found more than the main solve (bucketed DP rounding): the marginal is clamped to 0
};

struct PortfolioCommodityResult {
    Commodity commodity = Commodity::None;
    bool evaluated = false;
    // Evaluated for the DIAGNOSIS only (service deficit, notices): the commodity's contracts are managed by the player (AutoTrade* off), so every real contract is pinned as it is, no action is
    // produced and the result is never the accounting of the local plans (those keep the active-contract accounting of every real row).
    bool observeOnly = false;
    std::string note;
    std::string policy;                // human-readable policy line (logging only)
    std::string policyConflict;        // non-empty: a fixed obligation blocked BUY xor SELL and the own-city service won
    std::int64_t searchNodes = 0, searchPruned = 0, searchEvaluated = 0;   // composition search statistics (Garbage)
    std::string mode = "LOCAL";        // LOCAL / BUY / SELL of the final portfolio (sell-like: SELL, IMPORT_GARBAGE; buy-like: BUY, EXPORT_GARBAGE)
    // diagnostics (never read by a decision)
    struct ModeResult {
        std::string mode;
        bool legal = false, feasible = false;
        std::int64_t economicNet = 0, cashNet = 0, localExpense = 0, dealIncome = 0, dealExpense = 0;
        int dealCount = 0, changes = 0;
        std::string reasonIfRejected;
    };
    std::vector<ModeResult> modes;
    struct CandidateDiag {
        DealKey key;
        bool sellLike = false, fixed = false, existing = false, completed = false;
        std::int32_t current = 0, ladderStep = 0, ladderMax = 0;
        std::optional<std::int64_t> amountUsed;
        std::vector<std::int32_t> options;
        std::string lockedBy;          // "" | COMPLETED_UNTIL_ROLLOVER | NEW_SAME_DIRECTION_AS_COMPLETED
    };
    std::vector<CandidateDiag> candidateDiag;
    std::vector<std::string> continuity;       // ROTATION_BLOCKED / ROTATION_ALLOWED / CONTINUITY_RELAXED lines
    std::string arbitrage;                     // "" | why buy + sell together was not searched in this plan (ARBITRAGE_SKIPPED ...)
    long double theoreticalCompositions = 0.0L;
    bool optimumProven = true;
    std::int64_t ownNeed = 0, requiredNeed = 0, sellTotal = 0, buyTotal = 0;
    // for the dynamic order of the writes (ActionsFrom): the confirmed local capacity, the contracts as they stand now, and the local need as a function of the planned flows
    std::int64_t localCapacityNow = -1, currentBuy = 0, currentSell = 0, demand = -1;
    bool sellLoadsLocal = true;
    // The local side cannot serve the city even with no sale and the largest purchase the game offers (supply physically insufficient): every changeable sale is cancelled and the player is warned.
    bool serviceDeficit = false;
    // The emergency plan (PlanServiceEmergency): what the best plan delivers. recovery: "" (no emergency) | TOTAL (the whole demand is served) | PARTIAL (purchases contribute) | NONE (nothing can be bought);
    // recoveryPending = recovery actions still to be executed in this plan (the warning waits for them).
    std::string recovery;
    int recoveryPending = 0;
    std::int64_t emergencyDemand = -1, emergencyServedNow = -1, emergencyServedPlan = -1, emergencyCapacity = -1, emergencyBought = 0;
    std::function<std::int64_t(std::int64_t buy, std::int64_t sell)> localNeedFn;
    std::int64_t baselineNet = 0;      // - LocalCost(ownNeed + fixed): no deals at all (economic)
    std::int64_t baselineCashNet = 0;
    // Keeping every existing contract can be INFEASIBLE (the local side cannot serve it, or a cap is exceeded). That is a qualitative state, not
    // money: `keepFeasible` is false, `keepNet` is 0 and meaningless, and no gain over it is computed.
    bool keepFeasible = true;
    std::int64_t keepNet = 0;          // every existing contract kept, nothing new (valid only when keepFeasible), economic
    std::int64_t keepCashNet = 0;      // the same in cash
    std::int64_t optimumNet = 0;       // economic
    std::int64_t optimumCashNet = 0;   // cash
    // optimum minus keeping the current contracts, only when that comparison is between two feasible portfolios
    std::optional<std::int64_t> gain() const {
        if (!keepFeasible) return std::nullopt;
        return SatSub(optimumNet, keepNet);
    }
    std::optional<std::int64_t> cashGain() const {
        if (!keepFeasible) return std::nullopt;
        return SatSub(optimumCashNet, keepCashNet);
    }
    std::vector<PortfolioDecision> decisions;   // every candidate with a contract before or after
    bool hasLatent() const {
        for (const auto& d : decisions) if (d.latent && d.from != d.to) return true;
        return false;
    }
};

namespace portfolio_detail {

struct Score {
    bool valid = false;
    std::int64_t profit = 0;
    int deals = 0;
    int changes = 0;
};

// higher profit, then fewer deals, then fewer changes
inline bool Better(const Score& a, const Score& b) {
    if (!a.valid) return false;
    if (!b.valid) return true;
    if (a.profit != b.profit) return a.profit > b.profit;
    if (a.deals != b.deals) return a.deals < b.deals;
    return a.changes < b.changes;
}

struct Side {
    std::vector<Score> best;                       // by bucket
    std::vector<std::vector<int>> choice;          // [item][bucket] -> option index
    std::vector<std::vector<int>> weight;          // [item][option] -> buckets
};

inline std::int64_t SignedMoney(const PortfolioCandidate& c, const PortfolioOption& o) { return c.sellLike ? ClampMoney(o.money) : -ClampMoney(o.money); }

inline Side SolveSide(const std::vector<const PortfolioCandidate*>& items, bool sell, std::int64_t g, int maxBuckets) {
    Side s;
    s.best.assign(static_cast<std::size_t>(maxBuckets) + 1, Score{});
    s.best[0] = Score{true, 0, 0, 0};
    for (const auto* item : items) {
        std::vector<int> w;
        for (const auto& o : item->options) {
            const std::int64_t units = o.amount;
            const std::int64_t b = sell ? (units + g - 1) / g : units / g;   // sells round the consumed capacity up, buys round the relief down
            w.push_back(static_cast<int>(std::min<std::int64_t>(b, maxBuckets + 1)));
        }
        std::vector<Score> next(s.best.size(), Score{});
        std::vector<int> pick(s.best.size(), -1);
        for (std::size_t a = 0; a < s.best.size(); ++a) {
            if (!s.best[a].valid) continue;
            for (std::size_t k = 0; k < item->options.size(); ++k) {
                const auto nb = a + static_cast<std::size_t>(w[k]);
                if (nb >= next.size()) continue;
                Score cand = s.best[a];
                const auto& o = item->options[k];
                cand.profit = SatAdd(cand.profit, SignedMoney(*item, o));
                if (o.amount > 0) ++cand.deals;
                if (o.amount != item->currentAmount) ++cand.changes;
                if (Better(cand, next[nb])) { next[nb] = cand; pick[nb] = static_cast<int>(k); }
            }
        }
        s.best = std::move(next);
        s.choice.push_back(std::move(pick));
        s.weight.push_back(std::move(w));
    }
    return s;
}

inline std::vector<int> Backtrack(const Side& s, int bucket) {
    std::vector<int> out(s.choice.size(), 0);
    for (int i = static_cast<int>(s.choice.size()) - 1; i >= 0; --i) {
        const int k = s.choice[static_cast<std::size_t>(i)][static_cast<std::size_t>(bucket)];
        out[static_cast<std::size_t>(i)] = k;
        bucket -= s.weight[static_cast<std::size_t>(i)][static_cast<std::size_t>(k)];
    }
    return out;
}

} // namespace portfolio_detail

// Work of the portfolio (diagnostics only, never read by a decision; game thread): per commodity, the time of its build + solve and how many local-expense / complete-pick
// evaluations missed the memo (each one a real model run).
struct PortfolioWork { double buildUs[3] = {0, 0, 0}, solveUs[3] = {0, 0, 0}; std::uint64_t localEvals[3] = {0, 0, 0}, compositionEvals[3] = {0, 0, 0}; };
inline PortfolioWork gPortfolioWork;
inline int PortfolioWorkIndex(Commodity c) { return c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : 2; }
inline double PortfolioNowUs() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() / 1000.0; }

inline PortfolioCommodityResult SolvePortfolio(const PortfolioCommodity& pc);

// Solves one commodity for a FIXED side per neighbor (see SolvePortfolio). Candidates the caller could not price are simply absent (fail closed).
inline PortfolioCommodityResult SolvePortfolioCore(const PortfolioCommodity& pc) {
    using namespace portfolio_detail;
    PortfolioCommodityResult r;
    r.commodity = pc.commodity;
    r.ownNeed = pc.ownNeed;
    r.localCapacityNow = pc.localCapacityNow;
    r.demand = pc.demand;
    r.sellLoadsLocal = pc.sellsLoadLocal;
    r.localNeedFn = pc.localNeed;
    for (const auto& c : pc.candidates) (c.sellLike ? r.currentSell : r.currentBuy) += c.currentAmount;
    if (!pc.available || (!pc.localExpense && !pc.compositionExpense)) { r.note = pc.available ? "no local expense model" : pc.unavailableReason; return r; }

    for (const auto& c : pc.candidates) {
        PortfolioCommodityResult::CandidateDiag cd;
        cd.key = c.key; cd.sellLike = c.sellLike; cd.fixed = c.fixed; cd.existing = c.existing; cd.completed = c.completedThisMonth;
        cd.current = c.currentAmount; cd.ladderStep = c.ladderStep; cd.ladderMax = c.ladderMax; cd.amountUsed = c.amountUsed;
        for (const auto& o : c.options) cd.options.push_back(o.amount);
        r.candidateDiag.push_back(std::move(cd));
    }
    // CONTINUITY (Garbage). A contract whose AmountUsed reached its Amount has nothing left to move THIS month, but it is a persistent contract: the
    // rollover resets AmountUsed to 0. Its exhaustion alone must not justify cancelling it and opening another neighbor to obtain a fresh 10% schedule
    // (monthly rotation). So the first attempt keeps the exhausted contracts, and opens no new contract in the direction of an exhausted one. Only when
    // that portfolio cannot serve the month (own-city feasibility) are they released.
    if (pc.continuityLock && !pc.internalSubproblem) {
        bool anyCompleted = false;
        bool completedDir[2] = {false, false};
        for (const auto& c : pc.candidates) if (!c.fixed && c.completedThisMonth) { anyCompleted = true; completedDir[c.sellLike ? 1 : 0] = true; }
        if (anyCompleted) {
            auto locked = pc;
            locked.continuityLock = false;
            std::vector<std::string> lines;
            for (std::size_t i = 0; i < locked.candidates.size(); ++i) {
                auto& c = locked.candidates[i];
                if (c.fixed) continue;
                if (c.completedThisMonth) {
                    PortfolioOption cur{c.currentAmount, 0, false};
                    for (const auto& o : c.options) if (o.amount == c.currentAmount) cur = o;
                    c.options = {cur};
                    r.candidateDiag[i].lockedBy = "COMPLETED_UNTIL_ROLLOVER";
                } else if (c.currentAmount == 0 && completedDir[c.sellLike ? 1 : 0]) {
                    c.options = {PortfolioOption{0, 0, false}};
                    r.candidateDiag[i].lockedBy = "NEW_SAME_DIRECTION_AS_COMPLETED";
                }
            }
            auto lr = SolvePortfolio(locked);
            if (lr.evaluated) {
                lr.candidateDiag = r.candidateDiag;
                for (const auto& cd : lr.candidateDiag)
                    if (!cd.lockedBy.empty())
                        lr.continuity.push_back("ROTATION_BLOCKED type=" + std::to_string(cd.key.type) + " neighbor=" + std::to_string(cd.key.neighbor) + " reason=" +
                                                (cd.lockedBy == "COMPLETED_UNTIL_ROLLOVER" ? "AMOUNT_USED_EXHAUSTION_ONLY (kept until the rollover)" : "NEW_CONTRACT_WOULD_RENEW_THE_DAILY_WINDOW"));
                return lr;
            }
            r.continuity.push_back("CONTINUITY_RELAXED reason=KEEPING_THE_COMPLETED_CONTRACTS_CANNOT_SERVE_THE_MONTH (" + lr.note + ")");
            for (auto& cd : r.candidateDiag) cd.lockedBy.clear();
        }
    }

    std::map<std::int64_t, std::optional<std::int64_t>> memo, memoCash;
    const auto Memoised = [&pc](std::map<std::int64_t, std::optional<std::int64_t>>& m, const LocalExpenseFn& fn, std::int64_t need) -> std::optional<std::int64_t> {
        const auto it = m.find(need);
        if (it != m.end()) return it->second;
        ++gPortfolioWork.localEvals[PortfolioWorkIndex(pc.commodity)];
        auto v = fn(need);
        if (v) v = ClampMoney(*v);
        m.emplace(need, v);
        return v;
    };
    const auto E = [&](std::int64_t need) -> std::optional<std::int64_t> { return pc.localExpense ? Memoised(memo, pc.localExpense, need) : std::nullopt; };
    const auto Ecash = [&](std::int64_t need) { return pc.localCash ? Memoised(memoCash, pc.localCash, need) : E(need); };
    const std::int64_t g = std::max<std::int64_t>(1, pc.granularity);
    const auto Axis = [&](std::int64_t need) { return pc.signedNeed ? need : std::max<std::int64_t>(0, need); };
    const auto Load = [&](std::int64_t sell) { return pc.sellsLoadLocal ? sell : std::int64_t{0}; };
    const auto NeedAt = [&](std::int64_t sellTotal, std::int64_t buyTotal) -> std::int64_t {
        return pc.localNeed ? pc.localNeed(buyTotal, Load(sellTotal)) : pc.ownNeed + Load(sellTotal) - buyTotal;
    };
    std::map<std::int64_t, std::int64_t> capMemo;
    const auto CapFor = [&](std::int64_t buyTotal) -> std::int64_t {
        if (!pc.sellCapByBuy) return 0;   // every caller tests pc.sellCapByBuy first: there is no "unlimited" integer
        const auto it = capMemo.find(buyTotal);
        if (it != capMemo.end()) return it->second;
        const auto v = pc.sellCapByBuy(buyTotal);
        capMemo.emplace(buyTotal, v);
        return v;
    };

    std::vector<const PortfolioCandidate*> sells, buys;
    std::int64_t fixedSell = pc.fixedSell, fixedBuy = 0, sellMax = 0, buyMax = 0;
    for (const auto& c : pc.candidates) {
        std::int64_t top = 0;
        for (const auto& o : c.options) top = std::max<std::int64_t>(top, o.amount);
        if (c.fixed) { (c.sellLike ? fixedSell : fixedBuy) += c.currentAmount; continue; }
        (c.sellLike ? sells : buys).push_back(&c);
        (c.sellLike ? sellMax : buyMax) += top;
    }
    // Complete-pick expense (memoised by the exact composition).
    std::map<std::string, std::optional<std::int64_t>> compMemo, compMemoCash;
    const auto CompositionOf = [&](const std::map<const PortfolioCandidate*, std::int32_t>& pick, bool cash) -> std::optional<std::int64_t> {
        std::vector<PortfolioChoice> choices;
        std::string key;
        for (const auto& [c, amount] : pick) {
            choices.push_back(PortfolioChoice{c->key, c->sellLike, amount});
            key += std::to_string(c->key.type) + ':' + std::to_string(c->key.neighbor) + '=' + std::to_string(amount) + ';';
        }
        const bool useCash = cash && pc.compositionCash;
        auto& memoMap = useCash ? compMemoCash : compMemo;
        const auto it = memoMap.find(key);
        if (it != memoMap.end()) return it->second;
        ++gPortfolioWork.compositionEvals[PortfolioWorkIndex(pc.commodity)];
        auto v = (useCash ? pc.compositionCash : pc.compositionExpense)(choices);
        if (v) v = ClampMoney(*v);
        memoMap.emplace(key, v);
        return v;
    };
    const auto moneyOf = [](const PortfolioCandidate& c, std::int32_t amount) -> std::int64_t {
        for (const auto& o : c.options) if (o.amount == amount) return SignedMoney(c, o);
        return 0;
    };
    // The net of a complete pick: deal money (cash) minus the local cost. `cash` selects the cash or the economic local cost; the deal money and the
    // feasibility are the same for both.
    const auto netOf = [&](const std::map<const PortfolioCandidate*, std::int32_t>& pick, bool cash) -> std::optional<std::int64_t> {
        std::int64_t money = 0, sellTot = fixedSell, buyTot = 0;
        for (const auto& [c, amount] : pick) (c->sellLike ? sellTot : buyTot) += amount;
        for (const auto& [c, amount] : pick) {
            if (amount > 0 && c->price.valid) {
                // price in the COMPLETE state: the other contracts of the portfolio move the local book too
                const double otherSell = static_cast<double>(sellTot - fixedSell - (c->sellLike ? amount : 0));
                const double otherBuy = static_cast<double>(buyTot - (c->sellLike ? 0 : amount));
                CommodityBook local = c->price.localBase;
                local.exported += otherSell;
                local.imported += otherBuy;
                const double q = static_cast<double>(amount);
                const double quote = c->sellLike ? QuotePostDealState(q, local, c->price.nbBase, 0.0, c->price.minAmount, pc.pricingTuning)
                                                 : QuotePostDealState(q, c->price.nbBase, local, 0.0, c->price.minAmount, pc.pricingTuning);
                const std::int64_t m = StoredCostFromQuote(quote);
                money = SatAdd(money, c->sellLike ? m : -m);
            } else {
                money = SatAdd(money, moneyOf(*c, amount));
            }
        }
        if (pc.sellCapByBuy && sellTot > CapFor(buyTot)) return std::nullopt;
        const auto e = pc.compositionExpense ? CompositionOf(pick, cash) : cash ? Ecash(Axis(NeedAt(sellTot, buyTot))) : E(Axis(NeedAt(sellTot, buyTot)));
        if (!e) return std::nullopt;
        return SatSub(money, *e);
    };
    const auto net = [&](const std::map<const PortfolioCandidate*, std::int32_t>& pick) { return netOf(pick, false); };
    const std::int64_t base = NeedAt(fixedSell, 0);
    if (pc.compositionExpense) {
        std::map<const PortfolioCandidate*, std::int32_t> zero;                       // no deals at all: every changeable contract at 0
        for (const auto& c : pc.candidates) if (!c.fixed) zero[&c] = 0;
        if (const auto e0 = CompositionOf(zero, false)) r.baselineNet = -*e0;
        if (const auto c0 = CompositionOf(zero, true)) r.baselineCashNet = -*c0;
    } else {
        if (const auto e0 = E(base)) r.baselineNet = -*e0;
        if (const auto c0 = Ecash(base)) r.baselineCashNet = -*c0;
    }

    std::map<const PortfolioCandidate*, std::int32_t> chosen;
    if (pc.compositionExpense) {
        // EXACT search over complete picks. The expense of a pick depends on WHICH contract carries each amount (every contract has its own AmountUsed
        // and delivery schedule), so the bucketed aggregate DP is not used. EVERY legal composition is eligible and every evaluated one is exact: there is
        // no thinning. BUY xor SELL is explored as separate branches (LOCAL, SELL-like only, BUY-like only), never as a SELL x BUY product. Pruning is only
        // by quantities that cannot be legal in any completion (the sums only grow along a branch): sellCap and buyCap. Candidates are sorted by key so the
        // result cannot depend on the neighbor order; ties beyond (net, deals, changes) are broken by the lexicographically smallest amounts.
        // A search too large to be exhausted is NOT approximated: the optimum is not proven and nothing is planned.
        constexpr long double kProofLimit = 5.0e6L;
        const auto byKey = [](const PortfolioCandidate* a, const PortfolioCandidate* b) { return std::tie(a->key.type, a->key.neighbor) < std::tie(b->key.type, b->key.neighbor); };
        std::sort(sells.begin(), sells.end(), byKey);
        std::sort(buys.begin(), buys.end(), byKey);
        const auto spaceOf = [](const std::vector<const PortfolioCandidate*>& items) { long double p = 1; for (const auto* c : items) p *= static_cast<long double>(c->options.size()); return p; };
        r.theoreticalCompositions = spaceOf(sells) + spaceOf(buys) - 1.0L;
        if (spaceOf(sells) > kProofLimit || spaceOf(buys) > kProofLimit) {
            r.note = "OPTIMUM_NOT_PROVEN: the composition space is too large to exhaust (commodity " + std::to_string(static_cast<int>(pc.commodity)) + ", sells " +
                     std::to_string(static_cast<double>(spaceOf(sells))) + " buys " + std::to_string(static_cast<double>(spaceOf(buys))) + "); no speculative change";
            r.optimumProven = false;
            return r;
        }
        const std::int64_t sellLimit = pc.sellCap ? std::max<std::int64_t>(0, *pc.sellCap - fixedSell) : std::numeric_limits<std::int64_t>::max() / 4;
        const std::int64_t buyLimit = std::min<std::int64_t>(pc.buyCap, std::numeric_limits<std::int64_t>::max() / 4);
        // ARBITRAGE (AllowUtilityArbitrage) searches every SELL-like x BUY-like pair: each one is an exact Sanitation plan (~10 us). Above kArbitrageCompositionLimit pairs that
        // would cost the game real time (and the per-decision counterfactuals repeat it), so THIS plan stays BUY xor SELL and says why: never an approximation, never a stall.
        constexpr long double kArbitrageCompositionLimit = 4000.0L;
        bool exclusive = !pc.allowBuyAndSell;
        if (!exclusive && spaceOf(sells) * spaceOf(buys) > kArbitrageCompositionLimit) {
            exclusive = true;
            r.arbitrage = "ARBITRAGE_SKIPPED: " + std::to_string(static_cast<double>(spaceOf(sells) * spaceOf(buys))) + " buy x sell compositions (limit " +
                          std::to_string(static_cast<double>(kArbitrageCompositionLimit)) + "); this plan stays BUY xor SELL";
        }
        const auto anyDeal = [](const std::vector<std::int32_t>& v) { for (const auto a : v) if (a > 0) return true; return false; };
        Score best;
        std::vector<std::int32_t> bestVec;
        std::map<const PortfolioCandidate*, std::int32_t> bestPick;
        const auto consider = [&](const std::vector<std::int32_t>& sa, const std::vector<std::int32_t>& ba) {
            std::map<const PortfolioCandidate*, std::int32_t> pick;
            std::int64_t sellSum = 0, buySum = 0;
            int deals = 0, changes = 0;
            std::vector<std::int32_t> vec;
            for (std::size_t i = 0; i < sells.size(); ++i) { pick[sells[i]] = sa[i]; vec.push_back(sa[i]); sellSum += sa[i]; deals += sa[i] > 0; changes += sa[i] != sells[i]->currentAmount; }
            for (std::size_t i = 0; i < buys.size(); ++i) { pick[buys[i]] = ba[i]; vec.push_back(ba[i]); buySum += ba[i]; deals += ba[i] > 0; changes += ba[i] != buys[i]->currentAmount; }
            if (sellSum > sellLimit || buySum > buyLimit) return;
            ++r.searchEvaluated;
            const auto v = net(pick);
            if (!v) return;
            Score t{true, *v, deals, changes};
            const bool tie = best.valid && !Better(t, best) && !Better(best, t);
            if (Better(t, best) || (tie && vec < bestVec)) { best = t; bestVec = std::move(vec); bestPick = std::move(pick); }
        };
        const auto walk = [&](const std::vector<const PortfolioCandidate*>& items, std::int64_t limit, const auto& leaf) {
            std::vector<std::int32_t> cur(items.size(), 0);
            const auto rec = [&](const auto& self, std::size_t i, std::int64_t sum) -> void {
                ++r.searchNodes;
                if (i == items.size()) { leaf(cur); return; }
                for (const auto& o : items[i]->options) {
                    if (sum + o.amount > limit) { ++r.searchPruned; continue; }
                    cur[i] = o.amount;
                    self(self, i + 1, sum + o.amount);
                }
                cur[i] = 0;
            };
            rec(rec, 0, 0);
        };
        const std::vector<std::int32_t> zeroSell(sells.size(), 0), zeroBuy(buys.size(), 0);
        // a candidate whose option list has no 0 (a counterfactual that forces it to its current amount) cannot be left out of a pick
        const auto zeroOk = [](const std::vector<const PortfolioCandidate*>& items) {
            for (const auto* c : items) if (std::none_of(c->options.begin(), c->options.end(), [](const PortfolioOption& o) { return o.amount == 0; })) return false;
            return true;
        };
        const bool sellsZeroOk = zeroOk(sells), buysZeroOk = zeroOk(buys);
        const auto search = [&](int mode) {
            if (!exclusive) {
                walk(sells, sellLimit, [&](const std::vector<std::int32_t>& sa) { const auto copy = sa; walk(buys, buyLimit, [&](const std::vector<std::int32_t>& ba) { consider(copy, ba); }); });
                return;
            }
            if (mode == 0 && fixedSell > 0 && fixedBuy > 0) return;                // both sides are held by suspended obligations: no strict portfolio
            if (sellsZeroOk && buysZeroOk) consider(zeroSell, zeroBuy);            // LOCAL
            if (mode == 0 && fixedBuy == 0 && buysZeroOk)                          // SELL-like only
                walk(sells, sellLimit, [&](const std::vector<std::int32_t>& sa) { if (anyDeal(sa)) consider(sa, zeroBuy); });
            if ((mode == 1 || fixedSell == 0) && sellsZeroOk)                      // BUY-like only (mode 1: service first, buys as needed, NO new sell)
                walk(buys, buyLimit, [&](const std::vector<std::int32_t>& ba) { if (anyDeal(ba)) consider(zeroSell, ba); });
        };
        search(0);
        if (!best.valid && exclusive && (fixedSell > 0 || fixedBuy > 0)) {
            search(1);
            if (best.valid)
                r.policyConflict = "POLICY_CONFLICT: a suspended contract blocks BUY xor SELL; the own-city service wins (buys as needed, no new sell, no arbitrage)";
        }
        if (!best.valid) { r.note = "no portfolio keeps the local side feasible"; return r; }
        chosen = std::move(bestPick);
    } else {
    // bucket ranges: sells up to the largest extra need the local side can still meet WITH the relief of the largest purchases
    // (purchases lower the need, so they let more be sold); buys up to the own need (no re-export)
    // First pass: the exact cap (the purchases together may reach the effective demand itself). If the bucket rounding lets the real sum overshoot it, the whole solve is repeated with the
    // rounding margin (granularity - 1 per purchase).
    const std::int64_t buyMargin = pc.roundingMargin ? static_cast<std::int64_t>(buys.size()) * (g - 1) : 0;
    const std::int64_t buyLimit = std::min<std::int64_t>(buyMax, std::max<std::int64_t>(0, pc.buyCap - buyMargin));
    std::int64_t sellLimit = sellMax;
    if (pc.sellCap) sellLimit = std::min<std::int64_t>(sellLimit, std::max<std::int64_t>(0, *pc.sellCap - fixedSell));
    if (sellLimit > 0) {
        const auto feasible = [&](std::int64_t extra) { return E(Axis(NeedAt(fixedSell + extra, buyLimit))).has_value(); };
        if (!feasible(sellLimit)) {
            std::int64_t lo = 0, hi = sellLimit;
            if (!feasible(0)) hi = 0;
            while (hi - lo > 1) {
                const std::int64_t mid = lo + (hi - lo) / 2;
                if (feasible(mid)) lo = mid; else hi = mid;
            }
            sellLimit = feasible(hi) ? hi : lo;
        }
    }
    // bucket size is widened when the range is large so the DP stays small; the rounding stays conservative on both sides
    std::int64_t gs = g;
    while (sellLimit / gs > 400) gs *= 2;
    std::int64_t gb = g;
    while (buyLimit / gb > 400) gb *= 2;
    const int A2 = static_cast<int>(sellLimit / gs);
    const int B2 = static_cast<int>(buyLimit / gb);

    const Side sellDp = SolveSide(sells, true, gs, A2);
    const Side buyDp = SolveSide(buys, false, gb, std::max(0, B2));

    // BUY xor SELL. A bucket above 0 always holds a real deal on its side (a sell of any amount rounds up to at least one bucket; a buy that rounds
    // down to bucket 0 has a negative profit, so bucket 0 of the buys is the empty set). Mode 0: strict, the fixed obligations count as deals.
    // Mode 1 (only when mode 0 finds nothing, i.e. a fixed obligation blocks the purchase the own city needs): service first, buys as needed and NO
    // new sell; reported as a policy conflict.
    const bool exclusive = !pc.allowBuyAndSell;
    const auto search = [&](int mode, Score& best, int& bestA, int& bestB) {
        for (int a = 0; a <= A2; ++a) {
            if (!sellDp.best[static_cast<std::size_t>(a)].valid) continue;
            for (int b = 0; b <= B2; ++b) {
                if (!buyDp.best[static_cast<std::size_t>(b)].valid) continue;
                if (exclusive) {
                    if (mode == 0) { if ((fixedSell > 0 || a > 0) && (fixedBuy > 0 || b > 0)) continue; }
                    else if (a > 0) continue;
                }
                if (pc.sellCapByBuy && fixedSell + a * gs > CapFor(b * gb)) continue;     // the sale must fit the handling the own city leaves spare
                const auto e = E(Axis(NeedAt(fixedSell + a * gs, b * gb)));
                if (!e) continue;
                Score t;
                t.valid = true;
                t.profit = SatSub(SatAdd(sellDp.best[static_cast<std::size_t>(a)].profit, buyDp.best[static_cast<std::size_t>(b)].profit), *e);
                t.deals = sellDp.best[static_cast<std::size_t>(a)].deals + buyDp.best[static_cast<std::size_t>(b)].deals;
                t.changes = sellDp.best[static_cast<std::size_t>(a)].changes + buyDp.best[static_cast<std::size_t>(b)].changes;
                if (Better(t, best)) { best = t; bestA = a; bestB = b; }
            }
        }
    };
    Score bestScore;
    int bestA = -1, bestB = -1;
    search(0, bestScore, bestA, bestB);
    if (!bestScore.valid && exclusive && (fixedSell > 0 || fixedBuy > 0)) {
        search(1, bestScore, bestA, bestB);
        if (bestScore.valid)
            r.policyConflict = "POLICY_CONFLICT: a suspended contract blocks BUY xor SELL; the own-city service wins (buys as needed, no new sell, no arbitrage)";
    }
    if (!bestScore.valid) { r.note = "no portfolio keeps the local side feasible"; return r; }

    // chosen option per candidate
    {
        const auto ps = Backtrack(sellDp, bestA);
        const auto pb = Backtrack(buyDp, bestB);
        for (std::size_t i = 0; i < sells.size(); ++i) chosen[sells[i]] = sells[i]->options[static_cast<std::size_t>(ps[i])].amount;
        for (std::size_t i = 0; i < buys.size(); ++i) chosen[buys[i]] = buys[i]->options[static_cast<std::size_t>(pb[i])].amount;
    }
    }

    // The buckets round the relief of the exports down, the exact cap may still differ: when the chosen sales do not fit it, the sales give way
    // (the own city and its exports keep priority) before the exact clean-up.
    if (pc.sellCapByBuy && !net(chosen)) {
        for (auto& [c, amount] : chosen) if (c->sellLike) amount = 0;
    }
    // exact clean-up: drop any deal whose marginal contribution, with the exact (not bucketed) expense, is not strictly positive
    for (bool changed = true; changed;) {
        changed = false;
        const auto full = net(chosen);
        if (!full) break;
        for (auto& [c, amount] : chosen) {
            if (amount == 0) continue;
            if (std::none_of(c->options.begin(), c->options.end(), [](const PortfolioOption& o) { return o.amount == 0; })) continue;   // forced by a constraint: cannot be dropped
            auto without = chosen;
            without[c] = 0;
            const auto alt = net(without);
            if (alt && *alt >= *full) { amount = 0; changed = true; break; }
        }
    }

    // NEVER WORSE THAN KEEPING. The bucketed search rounds amounts and money to its grid; the exact comparison below does not. When keeping every current contract is feasible, legal (BUY xor SELL,
    // caps) and strictly better than the chosen portfolio, the contracts stay as they are: a rewrite that lowers the net is never a decision (otherwise contracts
    // would move by a step and move back a few days later).
    {
        std::map<const PortfolioCandidate*, std::int32_t> keep;
        bool complete = true;
        std::int64_t keepSell = 0, keepBuy = 0;
        for (const auto& [c, amount] : chosen) {
            (void)amount;
            if (std::none_of(c->options.begin(), c->options.end(), [&](const PortfolioOption& o) { return o.amount == c->currentAmount; })) { complete = false; break; }
            keep[c] = c->currentAmount;
            (c->sellLike ? keepSell : keepBuy) += c->currentAmount;
        }
        const bool legal = (pc.allowBuyAndSell || !((fixedSell + keepSell > 0) && (fixedBuy + keepBuy > 0))) && keepBuy <= pc.buyCap && (!pc.sellCap || fixedSell + keepSell <= *pc.sellCap);
        if (complete && legal && keep != chosen) {
            const auto kn = net(keep);
            const auto cn = net(chosen);
            if (kn && cn && *kn > *cn) chosen = std::move(keep);
        }
    }

    // MATERIALITY: a facultative deal that improves the net by less than minBenefit is not worth a contract (new or existing). It is judged by the effective ECONOMIC net of the whole portfolio
    // (the local cost it displaces or adds, wear included), never by the nominal revenue / expense of the contract. A deal without which the portfolio is infeasible (the service, a reserve,
    // a cap) is exempt. Deals that are individually small but together relevant stay: the set of small deals is compared as a whole against the portfolio without all of them.
    if (pc.minBenefit > 0) {
        for (bool changed = true; changed;) {
            changed = false;
            const auto full = net(chosen);
            if (!full) break;
            std::vector<const PortfolioCandidate*> small;
            for (auto& [c, amount] : chosen) {
                if (amount == 0) continue;
                if (std::none_of(c->options.begin(), c->options.end(), [](const PortfolioOption& o) { return o.amount == 0; })) continue;
                auto without = chosen;
                without[c] = 0;
                const auto alt = net(without);
                if (!alt) continue;                                        // needed: never dropped
                if (SatSub(*full, *alt) < pc.minBenefit) small.push_back(c);
            }
            if (small.empty()) break;
            auto none = chosen;
            for (const auto* c : small) none[c] = 0;
            const auto altAll = net(none);
            if (!altAll || SatSub(*full, *altAll) >= pc.minBenefit) break;   // jointly needed or jointly relevant: they stay
            chosen = std::move(none);
            changed = true;
        }
    }

    // The same materiality applies to a CHANGE of the amount of an existing contract: moving it by a ladder step must be worth the minimum, otherwise the contract stays as it is. (The value of such a
    // step moves by tens of $/month from one day to the next with the demand, and each flip is a real contract rewrite.) A change the service needs - keeping the current amount is infeasible, or
    // exceeds a cap - is exempt.
    if (pc.minBenefit > 0) {
        for (bool changed = true; changed;) {
            changed = false;
            const auto full = net(chosen);
            if (!full) break;
            for (auto& [c, amount] : chosen) {
                if (c->currentAmount <= 0 || amount <= 0 || amount == c->currentAmount) continue;     // creations and cancellations are judged above
                if (std::none_of(c->options.begin(), c->options.end(), [&](const PortfolioOption& o) { return o.amount == c->currentAmount; })) continue;
                auto reverted = chosen;
                reverted[c] = c->currentAmount;
                const auto alt = net(reverted);
                if (!alt) continue;                                                                  // keeping the current amount is infeasible: the change is needed
                if (SatSub(*full, *alt) < pc.minBenefit) { chosen = std::move(reverted); changed = true; break; }
            }
        }
    }

    if (!pc.compositionExpense && !pc.roundingMargin) {
        std::int64_t realBuy = 0;
        for (const auto& [c, amount] : chosen) if (!c->sellLike) realBuy += amount;
        if (realBuy > pc.buyCap) { auto again = pc; again.roundingMargin = true; return SolvePortfolio(again); }
    }
    const auto full = net(chosen);
    r.evaluated = full.has_value();
    if (!full) { r.note = "chosen portfolio is infeasible after clean-up"; return r; }
    r.optimumNet = *full;
    const auto fullCash = netOf(chosen, true);
    r.optimumCashNet = fullCash ? *fullCash : *full;
    std::map<const PortfolioCandidate*, std::int32_t> keep;
    for (const auto& [c, amount] : chosen) { (void)amount; keep[c] = c->currentAmount; }
    if (const auto k = net(keep)) {
        r.keepFeasible = true;
        r.keepNet = *k;
        const auto kc = netOf(keep, true);
        r.keepCashNet = kc ? *kc : *k;
    } else {
        r.keepFeasible = false;
        r.keepNet = 0;
        r.keepCashNet = 0;
    }
    for (const auto& [c, amount] : chosen) {
        (c->sellLike ? r.sellTotal : r.buyTotal) += amount;
        if (amount == 0 && c->currentAmount == 0) {
            // A DORMANT contract: an ACTIVE row at amount 0 that the optimum keeps at 0. The game charges its stored value (1 $/month) for nothing, and cancelling it is legal and cannot touch the service:
            // any strictly positive saving justifies it (no transaction cost exists). Suspended rows are not candidates here (fixed).
            if (c->rowActive && c->existing && c->storedCost > 0) {
                PortfolioDecision dd;
                dd.key = c->key;
                dd.sellLike = c->sellLike;
                dd.from = 0;
                dd.to = 0;
                dd.rowActive = true;
                dd.dormant = true;
                dd.marginal = dd.cashMarginal = c->storedCost;
                dd.marginalKnown = true;
                dd.fromMoney = c->storedCost;
                dd.ladderStep = c->ladderStep;
                dd.ladderMax = c->ladderMax;
                r.decisions.push_back(dd);
            }
            continue;
        }
        PortfolioDecision d;
        d.key = c->key;
        d.sellLike = c->sellLike;
        d.from = c->currentAmount;
        d.to = amount;
        d.rowActive = c->rowActive;
        d.ladderStep = c->ladderStep;
        d.ladderMax = c->ladderMax;
        if (!pc.internalSubproblem) {
            auto sub = pc;
            sub.internalSubproblem = true;
            sub.continuityLock = false;
            sub.wantModeResults = false;
            for (auto& cc : sub.candidates) {
                if (!(cc.key == c->key)) continue;
                PortfolioOption cur{c->currentAmount, 0, false};
                for (const auto& o : c->options) if (o.amount == c->currentAmount) cur = o;
                cc.options = {cur};
            }
            const auto fr = SolvePortfolio(sub);
            if (fr.evaluated) {
                d.counterfactualLegal = true;
                d.counterfactualMode = fr.mode;
                d.counterfactualNet = fr.optimumNet;
                d.counterfactualCashNet = fr.optimumCashNet;
                d.marginal = SatSub(*full, fr.optimumNet);
                d.cashMarginal = fullCash ? SatSub(*fullCash, fr.optimumCashNet) : d.marginal;
                if (d.marginal < 0) { d.marginal = 0; d.counterfactualBeatOptimum = true; }
                d.marginalKnown = true;
            } else {
                // No POLICY-LEGAL portfolio keeps this contract at its current amount: there is no monetary comparison (never an extreme integer, never fake money).
                d.marginal = 0;
                d.cashMarginal = 0;
                d.marginalKnown = false;
                d.requiredForFeasibility = true;
                auto relaxed = sub;
                relaxed.allowBuyAndSell = true;
                d.requiredReason = SolvePortfolio(relaxed).evaluated ? "POLICY_TRANSITION" : "FEASIBILITY";
            }
        }
        for (const auto& o : c->options) {
            if (o.amount == d.from) d.fromMoney = o.money;
            if (o.amount == d.to) { d.toMoney = o.money; d.toQuoteNow = o.quoteNow ? o.quoteNow : o.money; d.latent = o.latent; }
        }
        r.decisions.push_back(d);
    }
    for (const auto& c : pc.candidates) if (c.fixed && c.sellLike) r.sellTotal += c.currentAmount;
    r.requiredNeed = Axis(NeedAt(r.sellTotal, r.buyTotal));
    r.mode = r.sellTotal > 0 && r.buyTotal > 0 ? (pc.allowBuyAndSell ? "BUY+SELL(arbitrage)" : "BUY+SELL(conflict)") : r.sellTotal > 0 ? "SELL" : r.buyTotal > 0 ? "BUY" : "LOCAL";
    if (pc.policyNote) r.policy = pc.policyNote(r.buyTotal);
    std::sort(r.decisions.begin(), r.decisions.end(), [](const PortfolioDecision& a, const PortfolioDecision& b) {
        return std::tie(a.key.type, a.key.neighbor) < std::tie(b.key.type, b.key.neighbor);
    });
    if (pc.wantModeResults && !pc.internalSubproblem) {
        for (const char* label : {"LOCAL", "BUY", "SELL"}) {
            auto sub = pc;
            sub.internalSubproblem = true;
            sub.continuityLock = false;
            sub.wantModeResults = false;
            for (auto& cc : sub.candidates) {
                if (cc.fixed) continue;
                const bool keepSide = (label[0] == 'B' && !cc.sellLike) || (label[0] == 'S' && cc.sellLike);
                if (!keepSide) cc.options = {PortfolioOption{0, 0, false}};
            }
            const auto mr = SolvePortfolio(sub);
            PortfolioCommodityResult::ModeResult m;
            m.mode = label;
            m.legal = mr.evaluated;
            m.feasible = mr.evaluated;
            if (mr.evaluated) {
                m.economicNet = mr.optimumNet;
                m.cashNet = mr.optimumCashNet;
                std::int64_t money = 0;
                for (const auto& dd : mr.decisions) {
                    if (dd.to <= 0) continue;
                    ++m.dealCount;
                    (dd.sellLike ? m.dealIncome : m.dealExpense) = SatAdd(dd.sellLike ? m.dealIncome : m.dealExpense, dd.toMoney);
                    if (dd.to != dd.from) ++m.changes;
                }
                money = SatSub(m.dealIncome, m.dealExpense);
                m.localExpense = SatSub(money, m.economicNet);
            } else {
                m.reasonIfRejected = mr.note.empty() ? std::string("LOCAL_SERVICE_INFEASIBLE_OR_XOR_CONFLICT") : mr.note;
            }
            r.modes.push_back(std::move(m));
        }
    }
    return r;
}

// ARBITRAGE between neighbors, not with one neighbor (AllowUtilityArbitrage): for one commodity a neighbor is EITHER a supplier OR a customer of the plan,
// never both (a free search could buy water from a neighbor and sell water to the same city in the same plan, a wash that only exploits the game's
// price asymmetry). Each neighbor that has candidates on both sides gets one side per assignment: 2^k assignments for k such neighbors (at most kMaxSideAssignments; above it
// the plan stays BUY xor SELL and says so), each one solved exactly, the best net kept (ties: the first in the order of the neighbors, deterministic). A suspended contract fixes
// its neighbor's side; a candidate forced to a non-zero amount (a counterfactual) fixes it too. The local-expense models are shared by all assignments (memoised).
constexpr int kMaxSideAssignments = 16;

inline PortfolioCommodityResult SolvePortfolio(const PortfolioCommodity& pc) {
    if (!pc.allowBuyAndSell) return SolvePortfolioCore(pc);
    struct Sides { bool sell = false, buy = false, sellPinned = false, buyPinned = false; };
    std::map<std::uint32_t, Sides> by;
    for (const auto& c : pc.candidates) {
        auto& sd = by[c.key.neighbor];
        const bool forcedNonZero = c.fixed ? c.currentAmount > 0 : std::none_of(c.options.begin(), c.options.end(), [](const PortfolioOption& o) { return o.amount == 0; });
        if (c.sellLike) { sd.sell = true; sd.sellPinned = sd.sellPinned || forcedNonZero; }
        else { sd.buy = true; sd.buyPinned = sd.buyPinned || forcedNonZero; }
    }
    std::vector<std::uint32_t> free;                                    // neighbors whose side the search must choose
    std::map<std::uint32_t, bool> sellSide;                            // neighbors whose side is already decided: true = customer (sell-like)
    for (const auto& [n, sd] : by) {
        if (!(sd.sell && sd.buy)) continue;
        if (sd.sellPinned && sd.buyPinned) continue;                     // two obligations of the game: left as they are
        if (sd.sellPinned || sd.buyPinned) sellSide[n] = sd.sellPinned;
        else free.push_back(n);
    }
    if (free.empty() && sellSide.empty()) return SolvePortfolioCore(pc);
    if (free.size() > 4 || (1 << free.size()) > kMaxSideAssignments) {
        auto strict = pc;
        strict.allowBuyAndSell = false;
        auto r = SolvePortfolioCore(strict);
        r.arbitrage = "ARBITRAGE_SKIPPED: " + std::to_string(free.size()) + " neighbors are offered on both sides (" + std::to_string(1 << std::min<std::size_t>(free.size(), 30)) +
                      " side assignments, limit " + std::to_string(kMaxSideAssignments) + "); this plan stays BUY xor SELL";
        return r;
    }
    std::optional<PortfolioCommodityResult> best;
    for (int mask = 0; mask < (1 << free.size()); ++mask) {
        auto side = sellSide;
        for (std::size_t i = 0; i < free.size(); ++i) side[free[i]] = (mask >> i) & 1;
        auto x = pc;
        for (auto& c : x.candidates) {
            const auto it = side.find(c.key.neighbor);
            if (it == side.end() || c.sellLike == it->second || c.fixed) continue;
            c.options = {PortfolioOption{0, 0, false}};                    // the other side of this neighbor: nothing (an existing contract there is cancelled)
        }
        auto r = SolvePortfolioCore(x);
        if (!r.evaluated) continue;
        if (!best || r.optimumNet > best->optimumNet) best = std::move(r);
    }
    if (best) return *best;
    auto r = SolvePortfolioCore([&] { auto strict = pc; strict.allowBuyAndSell = false; return strict; }());   // no assignment is feasible: the strict plan says why
    return r;
}

// ---- write plans -----------------------------------------------------------------------------------------------------------
struct PortfolioAction {
    DealWritePlan write;
    PortfolioActionKind kind = PortfolioActionKind::Create;
    int phase = 0;
    std::int64_t marginal = 0;         // ECONOMIC, real money only when marginalKnown (see PortfolioDecision); the controller's materiality test reads this one
    std::int64_t cashMarginal = 0;     // CASH: the one the ledger receives
    bool marginalKnown = true;
    bool requiredForFeasibility = false;
    std::string requiredReason;        // FEASIBILITY | POLICY_TRANSITION when the marginal is unknown
    std::int64_t fromMoney = 0, toMoney = 0;
    std::int64_t toQuoteNow = 0;       // the authoritative quote at write time is compared with this (the game prices against the CURRENT amount)
    bool sellLike = false;
    bool rowActive = false;
};

inline int PhaseFor(bool sellLike, PortfolioActionKind kind) {
    const bool up = kind == PortfolioActionKind::Create || kind == PortfolioActionKind::Increase;
    if (sellLike) return up ? kPhaseSellUp : kPhaseSellDown;
    return up ? kPhaseBuyUp : kPhaseBuyDown;
}

inline std::vector<PortfolioAction> ActionsFrom(const PortfolioCommodityResult& r) {
    std::vector<PortfolioAction> out;
    for (const auto& d : r.decisions) {
        if (d.from == d.to && !d.dormant) continue;
        if (d.latent) continue;     // capacity-first: the local plan carries the sale, the deal write waits for a legal quantity
        PortfolioAction a;
        // an ACTIVE row at amount 0 is an existing (dormant) contract: it is operated with CHANGE_AMOUNT, never re-created
        a.kind = d.from == 0 && !d.rowActive ? PortfolioActionKind::Create : d.to == 0 ? PortfolioActionKind::Cancel
                 : d.to > d.from ? PortfolioActionKind::Increase : PortfolioActionKind::Decrease;
        a.write.op = a.kind == PortfolioActionKind::Create ? DealWriteOp::Create : a.kind == PortfolioActionKind::Cancel ? DealWriteOp::Cancel : DealWriteOp::ChangeAmount;
        a.write.key = d.key;
        a.write.expectedAmount = d.from;
        a.write.targetAmount = d.to;
        a.write.ladderStep = d.ladderStep;
        a.write.ladderMax = d.ladderMax;
        a.write.expectedMonthlyDelta = d.cashMarginal;
        a.write.reason = std::string(DealTypeName(d.key.type)) + " neighbor " + std::to_string(d.key.neighbor) + ": " + std::to_string(d.from) + " -> " + std::to_string(d.to) + ", ";
        if (d.marginalKnown) {
            if (d.cashMarginal == d.marginal)
                a.write.reason += "net " + std::to_string(d.marginal) + "/month versus the best portfolio without this change";
            else
                a.write.reason += "net cash " + std::to_string(d.cashMarginal) + " economic " + std::to_string(d.marginal) + "/month (wear priced) versus the best portfolio without this change";
        } else {
            // no monetary comparison exists: the alternative cannot be served. Never an extreme integer, never "profit".
            const bool restores = a.kind == PortfolioActionKind::Cancel || a.kind == PortfolioActionKind::Decrease;
            a.write.reason += std::string("economicMarginal=N/A ") + (restores ? "RESTORES_FEASIBILITY" : "REQUIRED_FOR_FEASIBILITY") +
                              " (keeping the current amount leaves the local side unable to serve the portfolio)";
        }
        if (a.kind == PortfolioActionKind::Cancel) { a.write.evidence.activeFlag = false; a.write.evidence.amountEquals = false; a.write.evidence.storedCostRefreshed = false; }
        a.phase = PhaseFor(d.sellLike, a.kind);
        // DYNAMIC ORDER: the service must hold in every intermediate state, not only in the final portfolio. A change that cannot lower the confirmed service runs with the first (safe) group:
        //  * cancelling / lowering a purchase while the CONFIRMED local capacity already covers the demand, the contracts that stay and the reserve of the local part;
        //  * creating / raising a sale that the game offers right now (not latent) while the confirmed local capacity covers the sales that would then coexist.
        // Everything else keeps the capacity-first order (purchases and capacity raises before cuts; a purchase is never cancelled before its replacement exists).
        if (r.localCapacityNow >= 0 && r.localNeedFn) {
            const auto needAt = [&](std::int64_t buy, std::int64_t sell) { return r.localNeedFn(std::max<std::int64_t>(0, buy), r.sellLoadsLocal ? std::max<std::int64_t>(0, sell) : 0); };
            const std::int64_t delta = static_cast<std::int64_t>(d.to) - d.from;
            if (!d.sellLike && delta < 0 && r.localCapacityNow >= needAt(r.currentBuy + delta, r.currentSell)) {
                a.phase = kPhaseSellDown;
                a.write.reason += " [early: the confirmed local capacity " + std::to_string(r.localCapacityNow) + " already covers the service without this purchase]";
            } else if (d.sellLike && delta > 0 && !d.latent && r.localCapacityNow >= needAt(r.currentBuy, r.currentSell + delta)) {
                a.phase = kPhaseSellDown;
                a.write.reason += " [early: the game offers it now and the confirmed local capacity " + std::to_string(r.localCapacityNow) + " covers both sales]";
            }
        }
        a.marginal = d.marginal;
        a.cashMarginal = d.cashMarginal;
        a.marginalKnown = d.marginalKnown;
        a.requiredForFeasibility = d.requiredForFeasibility;
        a.requiredReason = d.requiredReason;
        a.fromMoney = d.fromMoney;
        a.toMoney = d.toMoney;
        a.toQuoteNow = d.toQuoteNow;
        a.rowActive = d.rowActive;
        a.sellLike = d.sellLike;
        out.push_back(std::move(a));
    }
    // inside one phase a creation / increase goes first: a replacement exists before the contract it replaces is cancelled
    const auto isUp = [](const PortfolioAction& a) { return a.kind == PortfolioActionKind::Create || a.kind == PortfolioActionKind::Increase; };
    std::stable_sort(out.begin(), out.end(), [&](const PortfolioAction& x, const PortfolioAction& y) { return x.phase != y.phase ? x.phase < y.phase : isUp(x) && !isUp(y); });
    return out;
}

// ---- building commodities from a snapshot ----------------------------------------------------------------------------------
inline std::string TuningNote() { return "tuning missing: vanilla defaults"; }

inline DealMarketTuning MarketTuningOf(const DealState& d) {
    DealMarketTuning t;
    const auto v = [&](std::uint32_t id) { return TunedValueOf(d, id); };
    if (const auto x = v(0xEA46932C)) t.minAmount[0] = *x;
    if (const auto x = v(0x6A469337)) t.minAmount[1] = *x;
    if (const auto x = v(0x8A46933D)) t.minAmount[2] = *x;
    if (const auto x = v(0xEA4AD2FF)) t.overDemandFactor = *x;
    if (const auto x = v(0xEA4AE9EF)) t.minBudget = *x;
    if (const auto x = v(0xCA46931F)) t.minTerm = *x;
    if (const auto x = v(0x6A469342)) t.minProfitFactor = *x;
    if (const auto x = v(0xCA469346)) t.maxProfitFactor = *x;
    if (const auto x = v(0xEA46934F)) t.maxTermYears = *x;
    for (const auto& tv : d.tuning) {
        if (tv.id == 0x4B61FFC9 && tv.present) { Curve c; if (ParseCurve(tv.values, c)) t.priceCurve = c; }
    }
    return t;
}

struct CandidateContext {
    const UtilitySnapshot* snapshot = nullptr;
    DealMarketTuning tuning;
};

inline int TypeIndex(std::int32_t type) { return type - utility_id::kDealBuyPower; }

// Builds the candidates of one commodity for every neighbor with a suitable connection. A candidate that cannot be priced from
// complete records is left out (missing is not zero).
inline std::vector<PortfolioCandidate> BuildCandidates(const UtilitySnapshot& s, Commodity commodity, const DealMarketTuning& tuning, std::vector<std::string>* notes,
                                                      bool allowLatent = true) {
    std::vector<PortfolioCandidate> out;
    const auto& d = s.deals;
    if (!d.marketsRead || !d.localMarket.budget) return out;
    const int ci = commodity == Commodity::Power ? 0 : commodity == Commodity::Water ? 1 : 2;
    const auto& lrow = d.localMarket.book[ci];
    if (!lrow.complete()) { if (notes) notes->push_back("local commodity record incomplete"); return out; }
    const CommodityBook local = lrow.get();
    const std::int32_t buyType = utility_id::kDealBuyPower + 2 * ci, sellType = buyType + 1;
    // Contributions of OUR changeable contracts to the books (the game counts every non-invalid row, pending proposals included; suspended rows are fixed
    // obligations and stay). Power/Water SELL candidates are priced and limited on the BASE books (these contributions removed), so the option set and the
    // valuation of a quantity do not depend on which contracts happen to be active: the same target is the optimum before and after it is applied.
    double soldTotal = 0.0, boughtTotal = 0.0;
    for (const auto& r : d.deals) {
        if (!r.flags || !r.amount || !IsKnownDealType(r.type) || CommodityOfDealType(r.type) != commodity || !DealCountsForRegionalFlow(*r.flags)) continue;
        if (*r.flags & deal_flag::kSuspensionMask) continue;
        (LocalIsBuyer(r.type) ? boughtTotal : soldTotal) += *r.amount;
    }
    CommodityBook localBase = local;
    localBase.exported = std::max(0.0, localBase.exported - soldTotal);
    localBase.imported = std::max(0.0, localBase.imported - boughtTotal);

    for (const auto& n : d.neighbors) {
        if (!n.book[ci].complete() || !n.budget) { if (notes) notes->push_back("neighbor " + std::to_string(n.serial) + " record incomplete"); continue; }
        const CommodityBook nb = n.book[ci].get();
        for (const std::int32_t type : {buyType, sellType}) {
            const bool sellLike = type == sellType;
            PortfolioCandidate c;
            c.key = DealKey{type, n.serial};
            c.sellLike = sellLike;
            const auto reject = [&](const char* why) {
                if (notes) notes->push_back(std::string("[DealOptimizer][Rejected] commodity=") + (commodity == Commodity::Power ? "POWER" : commodity == Commodity::Water ? "WATER" : "GARBAGE") + " type=" +
                                            DealTypeName(type) + " neighbor=" + std::to_string(n.serial) + " reason=" + why);
            };
            // existing rows for the key
            const ExistingDealRow* row = nullptr;
            int rows = 0;
            for (const auto& r : d.deals) if (RowMatches(r, c.key)) { row = &r; ++rows; }
            if (rows > 1) { if (notes) notes->push_back("ambiguous (type, neighbor): no write"); continue; }
            double cur = 0.0;
            if (row && row->amount) cur = *row->amount;
            const bool active = row && row->flags && (*row->flags & deal_flag::kActive);
            // (a garbage EXPORT whose AmountUsed is unreadable is an obligation as well: see GarbageFlowsOf)
            const bool suspended = row && row->flags && ((*row->flags & deal_flag::kSuspensionMask) ||
                                                         (active && commodity == Commodity::Garbage && !sellLike && !row->amountUsed && row->amount && *row->amount > 0));   // (an export at amount 0 exports nothing: no obligation)
            const bool pending = row && row->flags && (*row->flags & deal_flag::kPending) && !active;
            if (row && !active && !pending) continue;            // cancelled / invalid, awaiting removal by the simulator
            if (active && !DealIsCommitted(*row)) continue;
            if (active) {
                c.existing = true;
                c.rowActive = true;
                c.currentAmount = row->amount ? *row->amount : 0;
                c.storedCost = row->cost ? *row->cost : 0;
                if (row->amountUsed) c.amountUsed = *row->amountUsed;
                c.completedThisMonth = commodity == Commodity::Garbage && row->amountUsed && c.currentAmount > 0 && *row->amountUsed >= c.currentAmount;
            }
            if (suspended) {                                       // obligation we cannot touch
                c.fixed = true;
                out.push_back(std::move(c));
                continue;
            }
            const double minAmount = MinAmountOf(tuning, type);
            const int ti0 = TypeIndex(type);
            if (sellLike && commodity != Commodity::Garbage) {
                // ---- Power/Water SELL: canonical, state-independent candidate (see the comment on localBase) ----
                CommodityBook nbBase = nb;
                if (row && row->flags && row->amount && DealCountsForRegionalFlow(*row->flags)) nbBase.imported = std::max(0.0, nbBase.imported - *row->amount);
                CommodityBook unlimited = localBase;
                unlimited.produced += 1e12;                              // the seller side is bounded by the LOCAL plan (capacity), never by today's production
                const double canonMax = DealMaxAmountExact(unlimited, nbBase, minAmount, tuning.overDemandFactor, 0.0);
                const double realMaxN = n.maxAmount[ti0] ? static_cast<double>(*n.maxAmount[ti0]) : DealMaxAmountExact(local, nb, minAmount, tuning.overDemandFactor, cur);
                const double costMinBase = DealCostExact(minAmount, localBase, nbBase, 0.0, minAmount, tuning);
                const bool suitableN = n.suitable[ti0];
                const double treasuryN = *n.budget;
                const bool gateCanon = suitableN && canonMax >= minAmount && VanillaProposalGate(canonMax, minAmount, costMinBase, treasuryN, tuning).offered();
                const double costMinNow = DealCostExact(minAmount, local, nb, cur, minAmount, tuning);
                const bool gateReal = suitableN && realMaxN >= minAmount && VanillaProposalGate(realMaxN, minAmount, costMinNow, treasuryN, tuning).offered();
                // A NEW sale is created only through the simulator's OPPORTUNITY list (as for the garbage import below): GetDealMaxAmount can answer while the proposal gates (the neighbor's demand, treasury,
                // connection) say no. Otherwise a sale the game keeps refusing (e.g. SELL_POWER 0 -> 1000 with an empty list) would stay in the plan as a LATENT sale, the plant would be raised for it
                // and cut back at the next plan: surplus power and a flapping funding. Not listed (once the list has been read) = not a candidate, latent or not.
                bool offeredByGame = true;
                if (d.proposalsRead && !active) {              // an ACTIVE row (even dormant, amount 0) is changed, never created: it is not subject to the offer list
                    offeredByGame = false;
                    for (const auto& op : d.opportunities)
                        if (op.type == type && op.otherCitySerial && *op.otherCitySerial == n.serial && (!op.maxAmount || static_cast<double>(*op.maxAmount) >= minAmount)) offeredByGame = true;
                }
                if (!offeredByGame) { reject("NOT_OFFERED_BY_SIMULATOR"); continue; }
                const bool exists = (active && c.currentAmount > 0) || (gateCanon && (allowLatent || gateReal));
                if (!exists) { reject(!suitableN ? "NO_CONNECTION" : canonMax < minAmount ? "SELLER_SURPLUS" : !gateCanon ? "VANILLA_PROPOSAL_GATE" : "QUANTITY_NOT_LEGAL"); continue; }
                const std::int64_t maxQ = static_cast<std::int64_t>(canonMax);
                const std::int64_t step = std::max<std::int64_t>(maxQ / 10, static_cast<std::int64_t>(minAmount));
                c.ladderStep = static_cast<std::int32_t>(step);
                c.ladderMax = static_cast<std::int32_t>(maxQ);
                c.price = PortfolioPriceModel{true, localBase, nbBase, minAmount};
                c.options.push_back({0, 0, false});
                const double reachable = std::max(realMaxN, static_cast<double>(c.currentAmount));
                const auto addCanon = [&](std::int64_t q) {
                    if (q <= 0 || std::any_of(c.options.begin(), c.options.end(), [&](const PortfolioOption& e) { return e.amount == q; })) return;
                    PortfolioOption o;
                    o.amount = static_cast<std::int32_t>(q);
                    o.latent = static_cast<double>(q) > reachable;
                    if (o.latent && !allowLatent) return;
                    o.money = StoredCostFromQuote(QuotePostDealState(static_cast<double>(q), localBase, nbBase, 0.0, minAmount, tuning));
                    o.quoteNow = StoredCostFromQuote(QuoteCurrentState(static_cast<double>(q), local, nb, cur, minAmount, tuning));
                    c.options.push_back(o);
                };
                for (std::int64_t q = step; q <= maxQ && q < step * 64; q += step) addCanon(q);
                if (active && c.currentAmount > 0) {
                    addCanon(c.currentAmount);
                    for (auto& o : c.options) if (o.amount == c.currentAmount) o.quoteNow = c.storedCost;    // what the game stores right now
                }
                std::sort(c.options.begin(), c.options.end(), [](const PortfolioOption& a, const PortfolioOption& b) { return a.amount < b.amount; });
                if (c.options.size() <= 1 && !c.existing) { reject("QUANTITY_NOT_LEGAL"); continue; }
                out.push_back(std::move(c));
                continue;
            }
            if (!sellLike) {
                // ---- BUY (any commodity): canonical ladder on the BASE books; only amounts the game allows right now are offered (no latent buys) ----
                CommodityBook nbBase = nb;
                if (row && row->flags && row->amount && DealCountsForRegionalFlow(*row->flags)) nbBase.exported = std::max(0.0, nbBase.exported - *row->amount);
                // BUY xor SELL: a BUY portfolio runs with every SELL contract of this commodity cancelled, so the neighbor's book must not contain OUR sell contract with it either.
                // (Otherwise an IMPORT_GARBAGE row with a neighbor raises that neighbor's seller surplus and makes EXPORT_GARBAGE with it look legal; the plan cancels the import,
                // the export is no longer legal afterwards, and the next plan creates the import again, day after day.)
                double oppAmount = 0.0;
                for (const auto& r : d.deals)
                    if (RowMatches(r, DealKey{sellType, n.serial}) && r.flags && r.amount && DealCountsForRegionalFlow(*r.flags) && !(*r.flags & deal_flag::kSuspensionMask)) oppAmount += *r.amount;
                nbBase.imported = std::max(0.0, nbBase.imported - oppAmount);
                const double canonMax = DealMaxAmountExact(nbBase, localBase, minAmount, tuning.overDemandFactor, 0.0);
                double realMaxN = n.maxAmount[ti0] ? static_cast<double>(*n.maxAmount[ti0]) : DealMaxAmountExact(nb, local, minAmount, tuning.overDemandFactor, cur);
                if (oppAmount > 0.0) realMaxN = std::min(realMaxN, std::max(canonMax, static_cast<double>(c.currentAmount)));     // what the game answers now includes the sell contract that this portfolio removes
                const double costMinNow = DealCostExact(minAmount, nb, local, cur, minAmount, tuning);
                const bool gateReal = n.suitable[ti0] && realMaxN >= minAmount && VanillaProposalGate(realMaxN, minAmount, costMinNow, *d.localMarket.budget, tuning).offered();
                if (!((active && c.currentAmount > 0) || gateReal)) { reject(!n.suitable[ti0] ? "NO_CONNECTION" : realMaxN < minAmount ? "SELLER_SURPLUS" : "BUYER_BUDGET"); continue; }
                const std::int64_t maxQ = static_cast<std::int64_t>(canonMax);
                const std::int64_t step = std::max<std::int64_t>(maxQ / 10, static_cast<std::int64_t>(minAmount));
                c.ladderStep = static_cast<std::int32_t>(step);
                c.ladderMax = static_cast<std::int32_t>(maxQ);
                c.price = PortfolioPriceModel{true, localBase, nbBase, minAmount};
                c.options.push_back({0, 0, false});
                const double reachable = std::max(realMaxN, static_cast<double>(c.currentAmount));
                const auto addBuy = [&](std::int64_t q) {
                    if (q <= 0 || static_cast<double>(q) > reachable || std::any_of(c.options.begin(), c.options.end(), [&](const PortfolioOption& e) { return e.amount == q; })) return;
                    PortfolioOption o;
                    o.amount = static_cast<std::int32_t>(q);
                    o.money = StoredCostFromQuote(QuotePostDealState(static_cast<double>(q), nbBase, localBase, 0.0, minAmount, tuning));
                    o.quoteNow = StoredCostFromQuote(QuoteCurrentState(static_cast<double>(q), nb, local, cur, minAmount, tuning));
                    if (q != c.currentAmount && static_cast<double>(o.money) >= *d.localMarket.budget) return;   // UpdateState 0x10
                    c.options.push_back(o);
                };
                for (std::int64_t q = step; q <= maxQ && q < step * 64; q += step) addBuy(q);
                if (active && c.currentAmount > 0) {
                    addBuy(c.currentAmount);
                    for (auto& o : c.options) if (o.amount == c.currentAmount) o.quoteNow = c.storedCost;
                }
                std::sort(c.options.begin(), c.options.end(), [](const PortfolioOption& a, const PortfolioOption& b) { return a.amount < b.amount; });
                if (c.options.size() <= 1 && !c.existing) { reject("QUANTITY_NOT_LEGAL"); continue; }
                out.push_back(std::move(c));
                continue;
            }
            const CommodityBook& seller = sellLike ? local : nb;
            const CommodityBook& buyer = sellLike ? nb : local;
            // The REAL legal ladder comes from what the game answers NOW (GetDealMaxAmount on the refreshed cache). A sell that needs capacity the
            // city does not have yet is also considered through the OPTIMISTIC ladder (seller surplus not limiting): those amounts above the real
            // maximum are LATENT options: the local plan raises the capacity first, the next replan then sees the real ladder (never a livelock:
            // a latent amount is only kept while it is above the real maximum).
            const int ti = TypeIndex(type);
            double realMax = 0.0;
            const bool canonGarbageSell = commodity == Commodity::Garbage && sellLike;
            CommodityBook nbBaseG = nb;
            if (canonGarbageSell) {
                // Garbage IMPORT (we dispose of a neighbor's garbage): the maximum of a contract is the local surplus left by the OTHER sell contracts, so judged on the live book it moves with
                // the contracts we hold (e.g. with one neighbor at 4150 the others were QUANTITY_NOT_LEGAL, at 1660 they were legal, and the plan rotated between the two).
                // It is judged on the BASE books instead (our changeable contracts removed on both sides), and the sum of all imports is bounded by the same base surplus (BuildGarbageCommodity).
                if (row && row->flags && row->amount && DealCountsForRegionalFlow(*row->flags)) nbBaseG.imported = std::max(0.0, nbBaseG.imported - *row->amount);
                for (const auto& r : d.deals)
                    if (RowMatches(r, DealKey{buyType, n.serial}) && r.flags && r.amount && DealCountsForRegionalFlow(*r.flags) && !(*r.flags & deal_flag::kSuspensionMask))
                        nbBaseG.exported = std::max(0.0, nbBaseG.exported - *r.amount);
                realMax = DealMaxAmountExact(localBase, nbBaseG, minAmount, tuning.overDemandFactor, 0.0);
                // The base books say how much COULD be sold, but a NEW contract can only be created when the game offers it. If the game's own answer now is below the minimum while
                // the model on the live books says it is legal, the model disagrees with the game for this neighbor (such a contract would be planned 0 -> 1000 plan after plan and refused
                // each time: REPEATED_ACTION_REFUSAL): not a candidate. If the live model is also below the minimum, the shortfall is the surplus our OTHER contracts use,
                // which the sell phases free first, and the base judgement stands.
                if (!(active && c.currentAmount > 0)) {
                    const double modelLive = DealMaxAmountExact(seller, buyer, minAmount, tuning.overDemandFactor, cur);
                    const double gameNow = n.maxAmount[ti] ? static_cast<double>(*n.maxAmount[ti]) : modelLive;
                    if (gameNow < minAmount && modelLive >= minAmount) realMax = 0.0;
                    // The deal simulator proposes a deal only through its OPPORTUNITY list (its own gates: treasury, cost, connection...). A new import with a neighbor that is not in that list cannot
                    // be created, even when GetDealMaxAmount answers: it is not a candidate. Until the list has
                    // been read, missing is not "offered" (no new import).
                    bool listed = false;
                    if (d.proposalsRead)
                        for (const auto& op : d.opportunities)
                            if (op.type == type && op.otherCitySerial && *op.otherCitySerial == n.serial && (!op.maxAmount || static_cast<double>(*op.maxAmount) >= minAmount)) listed = true;
                    if (!listed) realMax = 0.0;
                }
            } else if (n.maxAmount[ti]) realMax = static_cast<double>(*n.maxAmount[ti]);
            else realMax = DealMaxAmountExact(seller, buyer, minAmount, tuning.overDemandFactor, cur);
            double optimisticMax = realMax;
            if (sellLike) {
                CommodityBook unlimited = seller;
                unlimited.produced += 1e12;
                optimisticMax = std::max(realMax, DealMaxAmountExact(unlimited, buyer, minAmount, tuning.overDemandFactor, cur));
            }
            const auto ladder = [&](double maxQ) {
                DealQuantityInput qi;
                qi.maxAmountTruncated = static_cast<std::int64_t>(maxQ);
                qi.minAmount = static_cast<std::int32_t>(minAmount);
                qi.active = active;
                if (row) {
                    qi.amount = row->amount ? *row->amount : 0;
                    qi.incAmount = row->incAmount ? *row->incAmount : 0;
                    qi.incAmount2 = row->incAmount2 ? *row->incAmount2 : 0;
                }
                return EnumerateDealQuantities(qi);
            };
            // The option money of a Garbage IMPORT is the price in the state where this contract alone holds q on the BASE books (like Power / Water SELL): priced against the live book, the value
            // of a quantity moves with every other contract we hold, so the optimum of one plan would not be the optimum of the next and equal splits between neighbors would keep migrating.
            const CommodityBook& moneySeller = canonGarbageSell ? localBase : seller;
            const CommodityBook& moneyBuyer = canonGarbageSell ? nbBaseG : buyer;
            const double moneyCur = canonGarbageSell ? 0.0 : cur;
            const double treasury = sellLike ? *n.budget : *d.localMarket.budget;
            const double costMin = DealCostExact(minAmount, seller, buyer, cur, minAmount, tuning);
            const bool suitable = n.suitable[ti];
            // an ACTIVE contract with a positive amount is always operable; an ACTIVE row at amount 0 is dormant and passes the vanilla gate like a proposal
            const bool realOk = (active && c.currentAmount > 0) || (suitable && realMax >= minAmount && VanillaProposalGate(realMax, minAmount, costMin, treasury, tuning).offered());
            // (Garbage: the vanilla maximum is driven by the garbage flows, not by our handling funding, so nothing is ever latent there. An ACTIVE contract
            // never has latent options: its ladder moves with the market, so a raise planned for an increase that is not legal afterwards would be
            // undone by the next cut and repeated - the increase is considered only on the real ladder.)
            const bool latentOk = allowLatent && !active && sellLike && commodity != Commodity::Garbage && suitable && optimisticMax > realMax && optimisticMax >= minAmount &&
                                  VanillaProposalGate(optimisticMax, minAmount, costMin, treasury, tuning).offered();
            if (!realOk && !latentOk) { reject(!suitable ? "NO_CONNECTION" : realMax < minAmount ? "QUANTITY_NOT_LEGAL" : "VANILLA_PROPOSAL_GATE"); continue; }
            c.options.push_back({0, 0, false});
            const auto addOptions = [&](const DealQuantityGrid& grid, bool latentOnly) {
                for (const auto q : grid.amounts) {
                    if (latentOnly && static_cast<double>(q) <= realMax) continue;
                    PortfolioOption o;
                    o.amount = q;
                    o.latent = latentOnly;
                    // ONE price per quantity, in the state after the deal is at q (the same for the current amount and for every alternative); the
                    // stored cost of the current contract is only a transient of the write (it is re-priced by the next maintenance)
                    o.money = StoredCostFromQuote(QuotePostDealState(q, moneySeller, moneyBuyer, moneyCur, minAmount, tuning));
                    o.quoteNow = StoredCostFromQuote(QuoteCurrentState(q, seller, buyer, cur, minAmount, tuning));
                    if (!sellLike && o.amount != c.currentAmount && static_cast<double>(o.money) >= *d.localMarket.budget) continue;   // UpdateState 0x10
                    if (std::any_of(c.options.begin(), c.options.end(), [&](const PortfolioOption& e) { return e.amount == o.amount; })) continue;
                    c.options.push_back(o);
                }
            };
            if (realOk) addOptions(ladder(realMax), false);
            if (latentOk) addOptions(ladder(optimisticMax), true);
            if (active && std::none_of(c.options.begin(), c.options.end(), [&](const PortfolioOption& o) { return o.amount == c.currentAmount; }))
            {
                PortfolioOption keep{c.currentAmount, StoredCostFromQuote(QuotePostDealState(c.currentAmount, moneySeller, moneyBuyer, moneyCur, minAmount, tuning)), false};
                keep.quoteNow = c.storedCost;
                c.options.push_back(keep);                                                         // the window may not list it; keeping it is always legal
            }
            std::sort(c.options.begin(), c.options.end(), [](const PortfolioOption& a, const PortfolioOption& b) { return a.amount < b.amount; });
            if (c.options.size() <= 1 && !c.existing) { reject("QUANTITY_NOT_LEGAL"); continue; }
            out.push_back(std::move(c));
        }
    }
    return out;
}

// The most ALL purchases together may reach: the local buyer need on the base books (the same bound GetDealMaxAmount applies to each of them, shared).
inline std::int64_t BuyerEnvelope(const UtilitySnapshot& s, Commodity commodity, const DealMarketTuning& tuning) {
    const int ci = commodity == Commodity::Power ? 0 : commodity == Commodity::Water ? 1 : 2;
    const auto& row = s.deals.localMarket.book[ci];
    if (!row.complete()) return 0;
    CommodityBook b = row.get();
    for (const auto& r : s.deals.deals) {
        if (!r.flags || !r.amount || !IsKnownDealType(r.type) || CommodityOfDealType(r.type) != commodity || !DealCountsForRegionalFlow(*r.flags)) continue;
        if ((*r.flags & deal_flag::kSuspensionMask) || !LocalIsBuyer(r.type)) continue;
        b.imported = std::max(0.0, b.imported - *r.amount);
    }
    const double minAmount = tuning.minAmount[ci];
    const double y = std::max(b.demanded + b.exported, minAmount / tuning.overDemandFactor + 1.0);
    return static_cast<std::int64_t>(std::max(0.0, tuning.overDemandFactor * y - (b.produced + b.imported)));
}

// ---- local side per commodity ----------------------------------------------------------------------------------------------
inline PortfolioCommodity BuildPowerCommodity(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealMarketTuning& mt,
                                              std::vector<std::string>* notes, bool allowLatent = true) {
    PortfolioCommodity pc;
    pc.commodity = Commodity::Power;
    // A city with NO plant at all (VALID_EMPTY: a pure importer, or one that just lost its last plant) still has a demand to serve through BUY_POWER: the local capacity is simply 0
    // (otherwise the whole power logic would be switched off and the deficit left alone).
    const bool noPlants = s.power.validity == SnapshotValidity::ValidEmpty && s.power.plants.empty();
    if (!s.power.aggregate.demand || !TuningValid(t) || (s.power.validity != SnapshotValidity::Valid && s.power.validity != SnapshotValidity::Partial && !noPlants)) {
        pc.unavailableReason = "power demand, tuning or snapshot unavailable";
        return pc;
    }
    auto inputs = std::make_shared<std::vector<PlantInput>>(BuildPowerInputs(s, cfg.freezePower));
    bool anyControllable = false;
    for (const auto& in : *inputs) anyControllable = anyControllable || in.controllable;
    double own = cfg.powerReserve * static_cast<double>(*s.power.aggregate.demand);
    if (cfg.useRequirement && s.power.aggregate.requirement) own = std::max(own, static_cast<double>(*s.power.aggregate.requirement));
    {
        const std::int64_t demand = *s.power.aggregate.demand;
        const std::int64_t floor = cfg.useRequirement && s.power.aggregate.requirement ? static_cast<std::int64_t>(*s.power.aggregate.requirement) : 0;
        const double reserve = cfg.powerReserve;
        pc.ownNeed = LocalRequiredCapacity(reserve, demand, 0, 0, floor);
        pc.localNeed = [reserve, demand, floor](std::int64_t buy, std::int64_t sell) { return LocalRequiredCapacity(reserve, demand, buy, sell, floor); };
        pc.buyCap = std::max<std::int64_t>(0, demand);       // the purchases together never exceed the city's own effective demand (no reserve is bought)
        std::int64_t capNow = 0;
        for (const auto& in : *inputs) capNow += in.controllable ? PlantCapacityAt(t, in, in.currentPercent) : in.fixedCapacity;
        pc.localCapacityNow = capNow;
        pc.demand = demand;
    }
    (void)own;
    OptimizerConfig coarse = cfg;
    coarse.dpMaxUnits = cfg.dpMaxUnitsExport;
    std::int64_t fixedCap = 0;
    for (const auto& in : *inputs) if (!in.controllable) fixedCap += in.fixedCapacity;
    // ONE solver for every need of this plan: the economic and the cash expense of a need come from the same solution (one DP per need, not two); the plant options are built once
    auto solver = std::make_shared<PowerNeedSolver>(*inputs, t, coarse);
    pc.localExpense = [solver, anyControllable, fixedCap](std::int64_t need) -> std::optional<std::int64_t> {
        if (!anyControllable) return fixedCap >= need ? std::optional<std::int64_t>(0) : std::nullopt;
        const auto& r = solver->Solve(need);
        if (!r.feasible) return std::nullopt;
        return r.economic();   // cash operating expense + capital wear (wear is 0 when ConsiderPowerAging is off or unknown for the plant)
    };
    pc.localCash = [solver, anyControllable, fixedCap](std::int64_t need) -> std::optional<std::int64_t> {
        if (!anyControllable) return fixedCap >= need ? std::optional<std::int64_t>(0) : std::nullopt;
        const auto& r = solver->Solve(need);
        if (!r.feasible) return std::nullopt;
        return r.expense;      // what the simulator charges for the SAME state
    };
    pc.allowBuyAndSell = cfg.allowUtilityArbitrage;   // option (active only with AutoDeals): buy and sell together when it pays more
    pc.granularity = 100;
    pc.available = true;
    pc.pricingTuning = mt;
    pc.candidates = BuildCandidates(s, Commodity::Power, mt, notes, allowLatent);
    pc.buyCap = std::min(pc.buyCap, BuyerEnvelope(s, Commodity::Power, mt));
    return pc;
}

inline PortfolioCommodity BuildWaterCommodity(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealMarketTuning& mt,
                                              std::vector<std::string>* notes, bool allowLatent = true) {
    PortfolioCommodity pc;
    pc.commodity = Commodity::Water;
    const auto& w = s.water;
    if ((w.validity != SnapshotValidity::Valid && w.validity != SnapshotValidity::Partial) || !w.department.present || !w.department.funding ||
        !w.department.idealMonthly || !w.aggregate.demand || !w.aggregate.productionCapacity || !TuningValid(t) || !ServiceCountsReady(w.aggregate)) {
        pc.unavailableReason = "water control, expense, demand, capacity or service counts unavailable";
        return pc;
    }
    for (const auto& p : w.producers) if (p.mechanism == MechanismClass::UnsupportedMechanism) { pc.unavailableReason = "a producer uses a non-standard mechanism"; return pc; }
    const int cur = UtilityPercentRound(*w.department.funding);
    const double effNow = t.waterEfficiency.Eval(static_cast<double>(*w.department.funding) * 100.0);   // the capacity read belongs to the EXACT funding, not to its rounded percent
    if (effNow <= 0.0) { pc.unavailableReason = "efficiency at the current funding is zero"; return pc; }
    const bool unwatered = UnwateredBlocksCut(w.aggregate);
    const bool pipes = (w.aggregate.distressedPipes && *w.aggregate.distressedPipes > 0) || (w.aggregate.burstPipes && *w.aggregate.burstPipes > 0);
    int hi = t.waterMaxPercent;
    if (w.department.maxAllowed) hi = std::min(hi, UtilityPercentRound(*w.department.maxAllowed));
    auto options = std::make_shared<std::vector<WaterFundingOption>>(
        BuildWaterFundingOptions(cfg, t, *w.department.idealMonthly, cur, hi, unwatered, pipes, effNow, *w.aggregate.productionCapacity));
    {
        const std::int64_t demand = *w.aggregate.demand;
        const double reserve = cfg.waterReserve;
        pc.ownNeed = LocalRequiredCapacity(reserve, demand, 0, 0);
        pc.localNeed = [reserve, demand](std::int64_t buy, std::int64_t sell) { return LocalRequiredCapacity(reserve, demand, buy, sell); };
        pc.buyCap = std::max<std::int64_t>(0, demand);
        pc.localCapacityNow = static_cast<std::int64_t>(*w.aggregate.productionCapacity);
        pc.demand = demand;
    }
    pc.localExpense = [options](std::int64_t need) -> std::optional<std::int64_t> {
        std::optional<std::int64_t> best;
        for (const auto& o : *options) if (o.capacity >= need && (!best || o.expense < *best)) best = o.expense;
        return best;
    };
    pc.granularity = 100;
    pc.available = true;
    pc.allowBuyAndSell = cfg.allowUtilityArbitrage;   // option (active only with AutoDeals)
    pc.pricingTuning = mt;
    pc.candidates = BuildCandidates(s, Commodity::Water, mt, notes, allowLatent);
    pc.buyCap = std::min(pc.buyCap, BuyerEnvelope(s, Commodity::Water, mt));
    return pc;
}

// The Garbage service input for the OWN city, WITHOUT any contract-driven change.
//
// The own disposal load is Produced - Recycled. The Imported counter is NEVER part of it:
//   * `GetGarbageImportedLastMonth` is the last-month field [+0x180]. It is copied from the current-month accumulator [+0x148] at the monthly
//     rollover and is constant for the whole following month;
//   * the accumulator only grows in the daily import step, `cap = round(Amount * 0.1)` per deal while `Amount - AmountUsed > 0`, with
//     AmountUsed reset at the rollover: it holds exactly what NEIGHBOR DEALS delivered, nothing else (Produced is the per-cell own production
//     and does not include it);
//   * so Imported is a HISTORICAL record of deliveries under the contracts of the previous month. It says nothing about the contracts of
//     this month (created, raised, lowered, cancelled) and nothing about the own city. Foreign garbage is judged on its own terms: the
//     imports planned by the portfolio are load on top of the protected own garbage (GarbageServiceInput::extraImportLoad), hosted when the
//     month projects no backlog and when their revenue pays for the funding they need.
// No attribution of the counter to contracts is attempted: "counter minus the active IMPORT contracts" mixes two different periods (last month's
// deliveries and this month's Amounts) and breaks on every in-month change (cancel: phantom own load; create/raise: own load stolen; lower then
// raise: double counted), so the counter is set aside entirely. Suspended exports stay as fixed relief, the standing uncollected stock
// is a backlog that needs handling on top of the monthly load (it is never capacity).
struct GarbageBase {
    GarbageServiceInput input;
    std::int64_t noDealLoad = 0;
};

inline GarbageBase GarbageBaseInput(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t) {
    GarbageBase b;
    const auto& g = s.garbage;
    b.input.aggregate = g.aggregate;
    b.input.reserve = cfg.garbageReserve;
    b.input.fundingFrozen = cfg.freezeGarbage;     // AutoGarbage off: the Sanitation funding is what it is now, no plan may rely on changing it
    b.input.calendar = CalendarFromDayOfYear(static_cast<int>(s.simDayOfYear));
    b.input.fundingCurve = t.powerEfficiency;   // 0x6911E2DF drives +0x228
    b.input.geometry = g.geometry;                  // the calibrated landfill geometry (or UNKNOWN / AMBIGUOUS / MODEL_MISMATCH: then the continuity lock decides)
    b.input.deals = GarbageFlowsOf(s.deals.deals);   // one flow per contract, kept as it is (target = Amount); each one carries its own AmountUsed
    for (auto& f : b.input.deals)                        // the rollover has not reset last month's AmountUsed yet: the contract restarts (GarbageControl.h)
        if (f.usedKnown && StaleMonthStartAmountUsed(f.amount, f.used, b.input.calendar.dayOfMonth)) { f.used = 0; f.usedRestarted = true; }
    // A readable counter is set aside (0 of it is own load); an unreadable one stays unreadable so the service stays "not computable".
    if (g.aggregate.imported) b.input.aggregate.imported = 0;
    if (g.aggregate.uncollected && *g.aggregate.uncollected > 0) b.input.backlog = *g.aggregate.uncollected;
    if (g.aggregate.produced && g.aggregate.recycled && b.input.aggregate.imported)
        b.noDealLoad = std::max<std::int64_t>(0, static_cast<std::int64_t>(*g.aggregate.produced) - *g.aggregate.recycled);
    return b;
}

// Sets the planned Amount of the garbage contracts from a complete pick. A changeable contract that exists keeps its AmountUsed and its flow; a contract
// that does not exist yet starts at AmountUsed = 0. A suspended (fixed) contract is never changed.
inline void ApplyGarbageTargets(GarbageServiceInput& in, const std::vector<PortfolioChoice>& picks) {
    for (const auto& p : picks) {
        if (p.key.type != utility_id::kDealImportGarbage && p.key.type != utility_id::kDealExportGarbage) continue;
        const bool isImport = p.key.type == utility_id::kDealImportGarbage;
        const std::int64_t amount = std::max<std::int64_t>(0, p.amount);
        bool found = false;
        for (auto& f : in.deals) {
            if (f.neighbor != p.key.neighbor || f.isImport != isImport) continue;
            if (!f.fixed) f.target = amount;
            found = true;
            break;
        }
        if (found || amount == 0) continue;
        GarbageDealFlow f;
        f.neighbor = p.key.neighbor;
        f.isImport = isImport;
        f.amount = 0;
        f.used = 0;
        f.target = amount;
        in.deals.push_back(f);
    }
}

// The Garbage funding plan for the FINAL contracts chosen by the portfolio, contract by contract. The own city (reserve included) always comes first;
// a foreign import is hosted only when the funding chosen for it recovers the whole month, and it may raise that funding when its revenue pays for it
// (the portfolio decides); an export is handling capacity.
inline GarbageFundingPlan PlanGarbageForTargets(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const std::vector<PortfolioChoice>& targets) {
    auto base = GarbageBaseInput(s, cfg, t);
    ApplyGarbageTargets(base.input, targets);
    return PlanGarbageFunding(base.input, s.garbage.department.funding, s.garbage.department.maxAllowed, s.garbage.department.idealMonthly);
}

// Totals only (no contract identity): every changeable contract is set aside and the totals are planned as NEW contracts that start today. Tests
// and callers without a portfolio; the real plan uses PlanGarbageForTargets.
inline GarbageFundingPlan PlanGarbageForCommitment(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealCommitment& commit) {
    auto base = GarbageBaseInput(s, cfg, t);
    for (auto& f : base.input.deals) if (!f.fixed) f.target = 0;
    base.input.extraImportLoad = commit.sell;
    base.input.extraExportRelief = commit.buy;
    return PlanGarbageFunding(base.input, s.garbage.department.funding, s.garbage.department.maxAllowed, s.garbage.department.idealMonthly);
}

inline PortfolioCommodity BuildGarbageCommodity(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealMarketTuning& mt,
                                                std::vector<std::string>* notes, bool allowLatent = true) {
    PortfolioCommodity pc;
    pc.commodity = Commodity::Garbage;
    const auto& g = s.garbage;
    if ((g.validity != SnapshotValidity::Valid && g.validity != SnapshotValidity::Partial) || !g.department.present || !g.department.funding ||
        !g.department.maxAllowed || !g.department.idealMonthly || !g.aggregate.produced || !g.aggregate.imported || !g.aggregate.recycled) {
        pc.unavailableReason = "garbage control, expense or flow counters unavailable";
        return pc;
    }
    const auto base = GarbageBaseInput(s, cfg, t);
    auto gi = std::make_shared<GarbageServiceInput>(base.input);
    pc.ownNeed = 0;                      // informational: the expense is evaluated on complete picks (compositionExpense), not on a need axis
    pc.signedNeed = true;
    pc.sellsLoadLocal = true;            // a foreign import is real load: it must fit the funding chosen for it, after the protected own city
    pc.buyCap = base.noDealLoad;         // exports may not exceed the city's own load
    const auto funding = g.department.funding, maxAllowed = g.department.maxAllowed;
    const auto ideal = g.department.idealMonthly;
    // The expense of a COMPLETE pick: every contract is projected with its own Amount / AmountUsed (an aggregate signed delta cannot tell which contract
    // carries an amount, and two contracts with the same total but different AmountUsed deliver on different days).
    pc.compositionExpense = [gi, funding, maxAllowed, ideal](const std::vector<PortfolioChoice>& pick) -> std::optional<std::int64_t> {
        GarbageServiceInput in = *gi;
        ApplyGarbageTargets(in, pick);
        const auto plan = PlanGarbageFunding(in, funding, maxAllowed, ideal);
        // ECONOMIC objective: with a calibrated geometry the monthly run-rate of the NEXT full month (H2), like the deal money; otherwise the cash target expense
        if (plan.status == GarbagePlanStatus::Optimal || plan.status == GarbagePlanStatus::NoChange) return plan.dualHorizon ? plan.steadyStateExpense : plan.targetExpense;
        return std::nullopt;             // no funding keeps the rest of the month (and, when calibrated, the next month) feasible with these contracts
    };
    if (gi->DualHorizonReady())
        pc.compositionCash = [gi, funding, maxAllowed, ideal](const std::vector<PortfolioChoice>& pick) -> std::optional<std::int64_t> {
            GarbageServiceInput in = *gi;
            ApplyGarbageTargets(in, pick);
            const auto plan = PlanGarbageFunding(in, funding, maxAllowed, ideal);
            if (plan.status == GarbagePlanStatus::Optimal || plan.status == GarbagePlanStatus::NoChange) return plan.targetExpense;    // what the simulator will charge now
            return std::nullopt;
        };
    if (s.deals.localMarket.freeLandfillStorage) {
        const double limit = ImportGarbageContractLimit(static_cast<double>(*s.deals.localMarket.freeLandfillStorage), mt.maxTermYears,
                                                        static_cast<double>(*g.aggregate.produced),
                                                        s.deals.localMarket.book[2].imported ? *s.deals.localMarket.book[2].imported : 0.0);
        pc.sellCap = static_cast<std::int64_t>(limit);
    }
    // All imports together are bounded by the local surplus on the BASE book (the same bound the game applies to each of them, shared): (produced + imported) - (demanded + exported) with
    // our changeable contracts removed. Without it every candidate looks legal on its own while the sum is not.
    if (s.deals.localMarket.book[2].complete()) {
        CommodityBook lb = s.deals.localMarket.book[2].get();
        for (const auto& r : s.deals.deals) {
            if (!r.flags || !r.amount || !IsKnownDealType(r.type) || CommodityOfDealType(r.type) != Commodity::Garbage || !DealCountsForRegionalFlow(*r.flags) || (*r.flags & deal_flag::kSuspensionMask)) continue;
            (LocalIsBuyer(r.type) ? lb.imported : lb.exported) = std::max(0.0, (LocalIsBuyer(r.type) ? lb.imported : lb.exported) - *r.amount);
        }
        const auto surplus = static_cast<std::int64_t>(std::max(0.0, (lb.produced + lb.imported) - (lb.demanded + lb.exported)));
        pc.sellCap = pc.sellCap ? std::min(*pc.sellCap, surplus) : surplus;
    }
    pc.granularity = 100;
    pc.available = true;
    pc.continuityLock = !gi->DualHorizonReady();      // calibrated: H1 safety + H2 economics decide; otherwise a completed contract stays until the rollover
    pc.allowBuyAndSell = cfg.allowUtilityArbitrage;   // option (active only with AutoDeals): import foreign garbage for money and export own garbage cheaper, when it pays
    pc.pricingTuning = mt;
    pc.candidates = BuildCandidates(s, Commodity::Garbage, mt, notes, allowLatent);
    pc.buyCap = std::min(pc.buyCap, BuyerEnvelope(s, Commodity::Garbage, mt));
    return pc;
}

struct PortfolioPlan {
    PlanStatus status = PlanStatus::NotAvailable;
    std::vector<PortfolioCommodityResult> commodities;     // power, water, garbage
    std::vector<PortfolioAction> actions;                  // ordered by phase; not executed here
    std::vector<std::string> notes;
    // Money gained over keeping the current contracts, summed over the commodities where that comparison is between two feasible portfolios.
    std::int64_t netGain() const {
        std::int64_t v = 0;
        for (const auto& c : commodities) if (c.evaluated && !c.observeOnly) if (const auto g = c.gain()) v = SatAdd(v, *g);
        return v;
    }
    std::int64_t cashNetGain() const {
        std::int64_t v = 0;
        for (const auto& c : commodities) if (c.evaluated && !c.observeOnly) if (const auto g = c.cashGain()) v = SatAdd(v, *g);
        return v;
    }
    // Commodities whose CURRENT portfolio is infeasible: the plan restores feasibility there, no monetary gain is claimed for it.
    bool keepInfeasible() const {
        for (const auto& c : commodities) if (c.evaluated && !c.observeOnly && !c.keepFeasible) return true;
        return false;
    }
};

// Which commodities take part (index power, water, garbage). A commodity left out is neither evaluated nor given actions.
struct PortfolioScope {
    bool commodity[3] = {true, true, true};
    bool allowLatent[3] = {true, true, true};   // false: only options the game offers right now (used when a latent option would need no capacity change)
    std::vector<DealKey> skipDormant;           // dormant rows whose cancellation was refused by the game in this city session: never retried
    std::vector<DealKey> refusedNew;            // NEW contracts the simulator refused recently (VANILLA_REJECTED and the like): not candidates until the caller lets them expire, so the funding phases behind them can run
    bool noSell[3] = {false, false, false};     // the game's own counters confirm a deficit of this commodity: no sale may stay (sell-like candidates are forced to 0)
    // A commodity that is NOT in `commodity` (its contracts are the player's, AutoTrade* off) may still be evaluated for the DIAGNOSIS: every real contract pinned as a fixed commitment, in both directions,
    // no action, never the accounting of the local plans. It is how a shortage of that resource still reaches the player's notice.
    bool observe[3] = {false, false, false};
    bool modeTable = false;                     // diagnostics: also solve LOCAL / BUY / SELL separately (log only, never read by a decision)
};

// DORMANT rows the solver has no candidate for (typically the game's own deal window left an active row at amount 0 for an opportunity that is no longer offered - "SELLER_SURPLUS" and the like -,
// so no candidate is built and the row would stay for ever). They carry nothing and cost their stored value (1 $/month): cancelling is a strictly positive saving and cannot touch the service.
inline void AddOrphanDormantDecisions(const UtilitySnapshot& s, const PortfolioCommodity& pc, PortfolioCommodityResult& r) {
    for (const auto& row : s.deals.deals) {
        if (!DealIsCommitted(row) || !row.otherCitySerial || !row.amount || *row.amount != 0 || !row.cost || *row.cost <= 0) continue;
        if (CommodityOfDealType(row.type) != pc.commodity) continue;
        if (row.flags && (*row.flags & deal_flag::kSuspensionMask)) continue;
        const DealKey key{row.type, *row.otherCitySerial};
        bool known = false;
        for (const auto& c : pc.candidates) known = known || c.key == key;
        for (const auto& d : r.decisions) known = known || d.key == key;
        if (known) continue;
        PortfolioDecision d;
        d.key = key;
        d.sellLike = !LocalIsBuyer(row.type);
        d.from = d.to = 0;
        d.rowActive = true;
        d.dormant = true;
        d.marginal = d.cashMarginal = *row.cost;
        d.marginalKnown = true;
        d.fromMoney = *row.cost;
        r.decisions.push_back(d);
    }
}

// ---- SERVICE EMERGENCY (no portfolio can serve the city) -----------------------------------------------------------------------------------------------------------------------------------------------
// Principle: when the city cannot be served in full, the Department still maximises the service actually delivered; it does not stop at cancelling the sales
// but also takes the purchases on offer. The order of priorities is lexicographic:
//   1. the own demand that is actually served:  served = D - max(0, max(0, D - B) + S - Cmax)   (Cmax = the largest local capacity the plants / department can legally reach; B purchases; S sales);
//   2. the safety margin where it is reachable, never a block to a partial recovery;
//   3. among equal service, the lowest net cost (purchases are priced with the game quote of the legal quantity).
// Sales that take capacity (or add load) are cancelled; changeable purchases are chosen among the legal quantities the game offers; the purchases never exceed the own demand (buyCap); suspended contracts are
// untouched but counted. Garbage: IMPORT (sell-like, adds local load) is cancelled, EXPORT (buy-like, relieves) is raised as far as the legal quantities and the own load allow, cheapest first.
inline std::int64_t MaxLocalCapacity(const PortfolioCommodity& c) {
    if (!c.localExpense) return 0;
    const auto feasible = [&](std::int64_t need) { return c.localExpense(need).has_value(); };
    std::int64_t lo = 0, hi = std::max<std::int64_t>(1, c.demand > 0 ? c.demand : 1000);
    for (int i = 0; i < 40 && feasible(hi); ++i) { lo = hi; hi *= 2; }
    while (hi - lo > 1) {
        const std::int64_t mid = lo + (hi - lo) / 2;
        (feasible(mid) ? lo : hi) = mid;
    }
    return lo;
}

inline void PlanServiceEmergency(const PortfolioCommodity& c, PortfolioCommodityResult& r) {
    const bool measurable = static_cast<bool>(c.localNeed) && static_cast<bool>(c.localExpense) && c.demand >= 0;     // Power / Water; Garbage has no need axis
    std::int64_t fixedSell = c.fixedSell, fixedBuy = 0, sellNow = 0, buyNow = 0;
    for (const auto& cand : c.candidates) {
        if (cand.fixed) (cand.sellLike ? fixedSell : fixedBuy) += cand.currentAmount;
        else (cand.sellLike ? sellNow : buyNow) += cand.currentAmount;
    }
    const std::int64_t cmax = measurable ? MaxLocalCapacity(c) : 0;
    const std::int64_t D = c.demand;
    const std::int64_t gran = std::max<std::int64_t>(1, c.granularity);
    // the demand served for a total purchase and a total sale
    const auto served = [&](std::int64_t buyTotal, std::int64_t sellTotal) -> std::int64_t {
        if (!measurable) return std::min(buyTotal, c.buyCap);
        const std::int64_t need = std::max<std::int64_t>(0, D - buyTotal) + sellTotal;
        return D - std::max<std::int64_t>(0, need - cmax);
    };
    const auto margin = [&](std::int64_t buyTotal, std::int64_t sellTotal) -> std::int64_t {
        if (!measurable) return 0;
        const std::int64_t need = std::max<std::int64_t>(0, D - buyTotal) + sellTotal;
        const std::int64_t wanted = c.localNeed(buyTotal, sellTotal) - need;      // the reserve on top of the service need
        return std::max<std::int64_t>(0, std::min(cmax - need, wanted));
    };
    // 1. every changeable sale goes (it takes capacity / adds load that the city needs for itself)
    for (const auto& cand : c.candidates) {
        if (!cand.sellLike || cand.fixed || cand.currentAmount <= 0) continue;
        PortfolioDecision d;
        d.key = cand.key; d.sellLike = true; d.from = cand.currentAmount; d.to = 0; d.rowActive = cand.rowActive;
        d.ladderStep = cand.ladderStep; d.ladderMax = cand.ladderMax; d.marginalKnown = false; d.requiredForFeasibility = true; d.requiredReason = "SERVICE_DEFICIT"; d.fromMoney = cand.storedCost;
        r.decisions.push_back(d);
    }
    // 2. the purchases: best (served, margin, -cost) over the legal quantities of every changeable purchase (bucketed dynamic programme over the total bought)
    struct State { std::int64_t total = 0, cost = 0; int changes = 0; std::vector<int> pick; };
    std::vector<const PortfolioCandidate*> buys;
    for (const auto& cand : c.candidates) if (!cand.sellLike && !cand.fixed && !cand.options.empty()) buys.push_back(&cand);
    const auto better = [&](const State& a, const State& b) {
        const auto sa = served(fixedBuy + a.total, fixedSell), sb = served(fixedBuy + b.total, fixedSell);
        if (sa != sb) return sa > sb;
        const auto ma = margin(fixedBuy + a.total, fixedSell), mb = margin(fixedBuy + b.total, fixedSell);
        if (ma != mb) return ma > mb;
        if (a.cost != b.cost) return a.cost < b.cost;
        if (a.changes != b.changes) return a.changes < b.changes;
        return a.total < b.total;
    };
    // The legal quantities are discrete: the smallest entry that serves the whole demand can lie above the demand itself (e.g. 16455 in force, demand 18902, next entry 19746). Such an overshoot of less
    // than ONE ladder step is allowed, but ONLY when it serves more of the demand than anything within the cap (see the choice below); it is never taken for equal service.
    std::int64_t slack = 0;
    for (const auto* b : buys) slack = std::max<std::int64_t>(slack, b->ladderStep);
    const std::int64_t capHard = std::max<std::int64_t>(c.buyCap, fixedBuy);
    std::map<std::int64_t, State> states;
    states[0] = State{};
    for (std::size_t i = 0; i < buys.size(); ++i) {
        std::map<std::int64_t, State> next;
        for (const auto& [bucket, st] : states) {
            (void)bucket;
            for (std::size_t o = 0; o < buys[i]->options.size(); ++o) {
                const auto& opt = buys[i]->options[o];
                if (opt.amount < 0 || fixedBuy + st.total + opt.amount > capHard + slack) continue;     // never more than the own demand / load, plus at most one ladder step (see above)
                State ns = st;
                ns.total += opt.amount;
                ns.cost += opt.amount > 0 ? opt.money : 0;
                ns.changes += opt.amount != buys[i]->currentAmount ? 1 : 0;
                ns.pick.push_back(static_cast<int>(o));
                const std::int64_t key = ns.total / gran;
                auto it = next.find(key);
                if (it == next.end() || better(ns, it->second)) next[key] = std::move(ns);
            }
        }
        if (next.empty()) break;
        states.swap(next);
    }
    State best, bestOver;
    bool have = false, haveOver = false;
    for (const auto& [bucket, st] : states) {
        (void)bucket;
        if (st.pick.size() != buys.size()) continue;
        if (fixedBuy + st.total > capHard) { if (!haveOver || better(st, bestOver)) { bestOver = st; haveOver = true; } continue; }
        if (!have || better(st, best)) { best = st; have = true; }
    }
    // an overshoot above the cap is taken only if it serves strictly more of the demand than the best state within the cap
    // (and by a material amount - more than 1 % of the demand: no purchase above the cap to serve a handful of extra units)
    const std::int64_t material = std::max<std::int64_t>(gran, measurable ? D / 100 : 0);
    if (haveOver && (!have || served(fixedBuy + bestOver.total, fixedSell) > served(fixedBuy + best.total, fixedSell) + material)) { best = bestOver; have = true; }
    std::int64_t bought = 0;
    if (have) {
        for (std::size_t i = 0; i < buys.size(); ++i) {
            const auto& cand = *buys[i];
            const auto& opt = cand.options[static_cast<std::size_t>(best.pick[i])];
            bought += opt.amount;
            if (opt.amount == cand.currentAmount) continue;
            PortfolioDecision d;
            d.key = cand.key; d.sellLike = false; d.from = cand.currentAmount; d.to = opt.amount; d.rowActive = cand.rowActive;
            d.ladderStep = cand.ladderStep; d.ladderMax = cand.ladderMax; d.marginalKnown = false; d.requiredForFeasibility = true; d.requiredReason = "SERVICE_EMERGENCY";
            d.fromMoney = cand.storedCost; d.toMoney = opt.money; d.toQuoteNow = opt.quoteNow ? opt.quoteNow : opt.money; d.latent = opt.latent;
            r.decisions.push_back(d);
        }
    } else {
        bought = buyNow;
    }
    // 3. what the plan delivers, for the log and for the notice (TOTAL / PARTIAL / NONE)
    r.emergencyDemand = measurable ? D : -1;
    r.emergencyServedNow = measurable ? served(fixedBuy + buyNow, fixedSell + sellNow) : -1;
    r.emergencyServedPlan = measurable ? served(fixedBuy + bought, fixedSell) : -1;
    r.emergencyCapacity = measurable ? cmax : -1;
    r.emergencyBought = fixedBuy + bought;
    r.recovery = measurable && r.emergencyServedPlan >= D ? "TOTAL" : (bought + fixedBuy > 0 ? "PARTIAL" : "NONE");
    r.recoveryPending = 0;
    for (const auto& d : r.decisions) if (d.from != d.to && !d.latent) ++r.recoveryPending;
}

inline PortfolioPlan PlanDealPortfolio(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const PortfolioScope& scope = PortfolioScope{}) {
    PortfolioPlan plan;
    const auto& d = s.deals;
    if (d.validity != SnapshotValidity::Valid && d.validity != SnapshotValidity::Partial) {
        plan.status = d.validity == SnapshotValidity::ValidEmpty ? PlanStatus::NoChange : PlanStatus::InvalidSnapshot;
        return plan;
    }
    if (!d.marketsRead) { plan.notes.push_back("MARKETS_NOT_READ_YET"); return plan; }
    const DealMarketTuning mt = MarketTuningOf(d);
    double tb = PortfolioNowUs();
    PortfolioCommodity cs[3] = {BuildPowerCommodity(s, cfg, t, mt, &plan.notes, scope.allowLatent[0]), {}, {}};
    gPortfolioWork.buildUs[0] += PortfolioNowUs() - tb; tb = PortfolioNowUs();
    cs[1] = BuildWaterCommodity(s, cfg, t, mt, &plan.notes, scope.allowLatent[1]);
    gPortfolioWork.buildUs[1] += PortfolioNowUs() - tb; tb = PortfolioNowUs();
    cs[2] = BuildGarbageCommodity(s, cfg, t, mt, &plan.notes, scope.allowLatent[2]);
    gPortfolioWork.buildUs[2] += PortfolioNowUs() - tb;
    bool any = false;
    int index = 0;
    for (auto& c : cs) {
        const bool observing = !scope.commodity[index] && scope.observe[index];
        if (!scope.commodity[index++] && !observing) {
            PortfolioCommodityResult skipped;
            skipped.commodity = c.commodity;
            skipped.note = "excluded from this plan";
            plan.commodities.push_back(std::move(skipped));
            continue;
        }
        c.wantModeResults = scope.modeTable;
        c.minBenefit = std::max<std::int64_t>(0, cfg.minContractBenefit);
        if (observing) {
            // the real contracts, as they stand, are fixed commitments (the solver's own notion of an obligation that cannot be changed); what is not a contract is not a candidate
            std::vector<PortfolioCandidate> real;
            for (auto& cand : c.candidates) {
                if (cand.currentAmount <= 0) continue;
                cand.fixed = true;
                cand.options.clear();
                real.push_back(std::move(cand));
            }
            c.candidates = std::move(real);
            c.allowBuyAndSell = true;       // what the player holds on both sides is not a conflict to resolve
        }
        if (scope.noSell[index - 1] && !observing)
            for (auto& cand : c.candidates)
                if (cand.sellLike && !cand.fixed) cand.options = {PortfolioOption{0, 0, false}};
        // a contract the simulator would not create (its own gates said no) stays at 0 while the refusal is fresh: planning it again only repeats the refusal and blocks every later phase of the commodity
        for (auto& cand : c.candidates)
            if (!cand.rowActive && !cand.fixed && std::any_of(scope.refusedNew.begin(), scope.refusedNew.end(), [&](const DealKey& k) { return k == cand.key; }))
                cand.options = {PortfolioOption{0, 0, false}};
        const double ts = PortfolioNowUs();
        auto r = SolvePortfolio(c);
        gPortfolioWork.solveUs[index - 1] += PortfolioNowUs() - ts;
        if (!c.available) r.note = c.unavailableReason;
        r.commodity = c.commodity;
        if (observing) {
            r.observeOnly = true;
            if (!r.evaluated && c.available && r.optimumProven && (r.note == "no portfolio keeps the local side feasible" || r.note == "chosen portfolio is infeasible after clean-up")) {
                r.serviceDeficit = true;       // the same verdict as the managed commodities; the emergency plan's actions are NOT kept (the contracts are the player's)
                PlanServiceEmergency(c, r);
                r.recoveryPending = 0;
            }
            plan.commodities.push_back(std::move(r));
            continue;
        }
        if (r.evaluated) {
            any = true;
            AddOrphanDormantDecisions(s, c, r);
            for (auto& a : ActionsFrom(r)) plan.actions.push_back(std::move(a));
            if (!scope.skipDormant.empty()) {
                for (auto it = plan.actions.begin(); it != plan.actions.end();) {
                    const bool skip = it->write.op == DealWriteOp::Cancel && it->write.expectedAmount == 0 &&
                                      std::any_of(scope.skipDormant.begin(), scope.skipDormant.end(), [&](const DealKey& k) { return k == it->write.key; });
                    it = skip ? plan.actions.erase(it) : it + 1;
                }
            }
        } else if (c.available && r.optimumProven && (r.note == "no portfolio keeps the local side feasible" || r.note == "chosen portfolio is infeasible after clean-up")) {
            // DEFICIT: no portfolio keeps the local side feasible. The sales only make it worse (they go), but the city is still served as far as it can be: the legal purchases on offer are taken
            // (PlanServiceEmergency).
            r.serviceDeficit = true;
            PlanServiceEmergency(c, r);
            auto cancels = ActionsFrom(r);
            if (!cancels.empty()) any = true;
            for (auto& a : cancels) plan.actions.push_back(std::move(a));
        }
        plan.commodities.push_back(std::move(r));
    }
    std::stable_sort(plan.actions.begin(), plan.actions.end(), [](const PortfolioAction& x, const PortfolioAction& y) { return x.phase < y.phase; });
    plan.status = !any ? PlanStatus::NotAvailable : plan.actions.empty() ? PlanStatus::NoChange : PlanStatus::Optimal;
    return plan;
}

} // namespace sc4af
