// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Bounds and saturating arithmetic for the money of the planning layer (portfolio, unified plan, ledger). Pure.
//
// Where the money comes from: stored deal costs are trunc(GetDealCost) of a double (DealQuote.h StoredCostFromQuote), deal amounts are int32,
// department costs are int32 budget lines times a percentage. In the game they stay far below 2^40. The pure layer must still never meet
// signed-overflow UB if it is fed extreme but valid int64 / double values, and it must NEVER use an extreme integer to mean "infeasible"
// (infeasibility is explicit state, see PortfolioCommodityResult::keepFeasible). So every figure entering a sum is clamped to
// +-kMoneyBound (2^50 ~ 1.1e15), which leaves 2^13 terms of head-room before a 64-bit sum could overflow, far more than the number of
// candidates, plants and ledger entries; the few sums that can grow without a fixed term count (the ledger) saturate.

#include <cmath>
#include <cstdint>

namespace sc4af {

constexpr std::int64_t kMoneyBound = std::int64_t{1} << 50;

inline std::int64_t ClampMoney(std::int64_t v) { return v > kMoneyBound ? kMoneyBound : v < -kMoneyBound ? -kMoneyBound : v; }

// double -> money, NaN and out-of-range safe (a plain cast of an out-of-range double is undefined behaviour)
inline std::int64_t ClampMoney(double v) {
    if (!(v == v)) return 0;
    if (v >= static_cast<double>(kMoneyBound)) return kMoneyBound;
    if (v <= -static_cast<double>(kMoneyBound)) return -kMoneyBound;
    return static_cast<std::int64_t>(v);
}

// ceil for a reserve target (demand or load times the reserve factor). The product of an integer and a decimal factor can land a hair above the exact
// result (100000 x 1.10 = 110000.00000000001): that must stay 110000, not become 110001.
inline std::int64_t CeilTarget(double v) { return static_cast<std::int64_t>(std::ceil(v - 1e-7)); }

inline std::int64_t SatAdd(std::int64_t a, std::int64_t b) {
    if (b > 0 && a > INT64_MAX - b) return INT64_MAX;
    if (b < 0 && a < INT64_MIN - b) return INT64_MIN;
    return a + b;
}

inline std::int64_t SatSub(std::int64_t a, std::int64_t b) {
    if (b < 0 && a > INT64_MAX + b) return INT64_MAX;
    if (b > 0 && a < INT64_MIN + b) return INT64_MIN;
    return a - b;
}

} // namespace sc4af
