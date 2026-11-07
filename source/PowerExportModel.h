// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure economic model for the export side of power. No game types.
//
// A sell opportunity is worth reserving capacity for only when its quoted revenue exceeds the extra plant expense needed to
// keep that capacity. There is no fixed percentage and no fixed amount: the reserve is whatever the economics justify and may
// legitimately be 0. Quotes come from the game (GetDealCost at legal amounts); amounts without a quote are never evaluated.
//
// Two kinds of opportunity are kept apart:
//   VISIBLE - ProposeDeals currently offers it (vanilla-eligible right now)
//   LATENT  - not offered now, but the game's own GetDealMaxAmount/GetDealCost answer for that neighbor;
//             it becomes eligible only if seller surplus grows. Never fabricated into a real deal.
// Revenue of either kind is a projection (HYPOTHETICAL until the player accepts a deal) and is never booked as realized income.

#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace sc4af {

struct SellOption {
    std::int64_t amount = 0;    // a legal amount the game quoted
    std::int64_t revenue = 0;   // GetDealCost total for that amount (monthly, as the game reports it)
};

struct NeighborSell {
    std::uint32_t serial = 0;
    bool visible = false;                 // false: latent
    std::int64_t minAmount = 1000;        // from the loaded tuning, never hardcoded by the caller
    std::int64_t maxAmount = 0;           // GetDealMaxAmount: limited by buyer need AND seller surplus
    std::vector<SellOption> options;      // quotes at legal amounts, ascending
};

struct ChosenSell {
    std::uint32_t serial = 0;
    bool visible = false;
    std::int64_t amount = 0;
    std::int64_t revenue = 0;
};

struct FrontierPoint {
    std::uint32_t serial = 0;
    std::int64_t amount = 0, revenue = 0, extraExpense = 0, net = 0;
    bool feasible = false;
};

struct ExportPlan {
    std::vector<ChosenSell> chosen;
    std::int64_t reserve = 0;        // total extra local capacity worth keeping (0 when nothing pays)
    std::int64_t revenue = 0;        // hypothetical monthly revenue
    std::int64_t extraExpense = 0;   // extra plant expense to keep the reserve
    std::int64_t net() const { return revenue - extraExpense; }
    std::vector<FrontierPoint> frontier;   // per neighbor and amount, evaluated alone (diagnostic output)
};

// expenseFor(requiredCapacity) = cheapest legal plant state expense that supplies that capacity, or nullopt if impossible.
using ExpenseFn = std::function<std::optional<std::int64_t>(std::int64_t requiredCapacity)>;

inline ExportPlan PlanExports(std::int64_t baseRequired, const std::vector<NeighborSell>& neighbors, const ExpenseFn& expenseFor,
                              bool wantFrontier = false) {
    ExportPlan plan;
    const auto base = expenseFor(baseRequired);
    if (!base) return plan;   // even the own requirement is infeasible: no room to think about exports

    std::map<std::int64_t, std::optional<std::int64_t>> memo;
    const auto cost = [&](std::int64_t extra) -> std::optional<std::int64_t> {
        const auto it = memo.find(extra);
        if (it != memo.end()) return it->second;
        return memo[extra] = expenseFor(baseRequired + extra);
    };

    // Legal options per neighbor: the quoted amounts inside [min, max]. One deal per neighbor (vanilla rule).
    std::vector<std::vector<SellOption>> legal(neighbors.size());
    for (std::size_t i = 0; i < neighbors.size(); ++i) {
        const auto& n = neighbors[i];
        for (const auto& o : n.options)
            if (o.amount >= n.minAmount && o.amount <= n.maxAmount && o.revenue > 0) legal[i].push_back(o);
    }

    if (wantFrontier) {
        for (std::size_t i = 0; i < neighbors.size(); ++i)
            for (const auto& o : legal[i]) {
                FrontierPoint f;
                f.serial = neighbors[i].serial;
                f.amount = o.amount;
                f.revenue = o.revenue;
                const auto c = cost(o.amount);
                f.feasible = c.has_value();
                if (c) { f.extraExpense = *c - *base; f.net = f.revenue - f.extraExpense; }
                plan.frontier.push_back(f);
            }
    }

    // Combine neighbors: best revenue per total extra capacity (seller surplus is shared, so amounts add up).
    struct Combo { std::int64_t revenue = 0; std::vector<ChosenSell> picks; };
    std::map<std::int64_t, Combo> bySum;
    bySum[0] = Combo{};
    for (std::size_t i = 0; i < neighbors.size(); ++i) {
        std::map<std::int64_t, Combo> next = bySum;   // option "no deal with this neighbor"
        for (const auto& [sum, combo] : bySum)
            for (const auto& o : legal[i]) {
                const std::int64_t ns = sum + o.amount;
                Combo c = combo;
                c.revenue += o.revenue;
                c.picks.push_back({neighbors[i].serial, neighbors[i].visible, o.amount, o.revenue});
                const auto it = next.find(ns);
                if (it == next.end() || c.revenue > it->second.revenue) next[ns] = std::move(c);
            }
        bySum = std::move(next);
    }

    // Evaluate sums by descending revenue; revenue is an upper bound on net (extra expense >= 0), so stop when it cannot win.
    std::vector<std::pair<std::int64_t, const Combo*>> order;
    for (const auto& [sum, combo] : bySum) order.emplace_back(sum, &combo);
    std::sort(order.begin(), order.end(), [](const auto& a, const auto& b) {
        return a.second->revenue != b.second->revenue ? a.second->revenue > b.second->revenue : a.first < b.first;
    });
    std::int64_t bestNet = 0;   // "no export" is always available and has net 0
    for (const auto& [sum, combo] : order) {
        if (sum == 0) continue;
        if (combo->revenue <= bestNet) break;
        const auto c = cost(sum);
        if (!c) continue;
        const std::int64_t extra = std::max<std::int64_t>(0, *c - *base);
        const std::int64_t net = combo->revenue - extra;
        if (net > bestNet || (net == bestNet && net > 0 && sum < plan.reserve)) {
            bestNet = net;
            plan.chosen = combo->picks;
            plan.reserve = sum;
            plan.revenue = combo->revenue;
            plan.extraExpense = extra;
        }
    }
    return plan;
}

} // namespace sc4af
