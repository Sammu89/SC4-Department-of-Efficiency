// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure reproduction of the quantity ladder the vanilla Neighbor Deals window offers for one deal row
// (SimCity 4.exe 1.1.641.0).
//
//   row builder   0x0077DAB0  (quantity combo population; arg6 = row state, arg5 = commodity)
//   combo handler 0x0078BCA0  (selection index n -> SetAmount), control 0xE8711255
//
// The simulator imposes no grid. The grid belongs to the deal's own IncAmount / IncAmount2 fields, which the window
// (re)computes from GetDealMaxAmount every time a row is repainted. This file never writes anything: it computes the
// list the window would show so that an automatic writer only ever requests quantities the player could have picked.
//
// Row builder, in terms of the code (all integer math truncates toward zero):
//   maxQ  = trunc(GetDealMaxAmount(type, other)); if maxQ <= 0 then maxQ = deal.Amount
//   d10   = maxQ / 10
//   step  = max(d10, M)                       M = tuned minimum deal amount of the commodity (0 when the tuning is absent)
//   SetIncAmount2(0)                          always cleared first
//   row not active (proposal):   SetIncAmount(step)
//   row active, not suspended:   if (Inc == Amount && Inc > M) || Inc == 0 : SetIncAmount(d10) (d10 == 0 -> no list)
//                                step = Inc, selected index = Amount / Inc
//   row suspended (flags 0x300): Inc = Inc2 = Amount, one entry
//   Q'    = max(maxQ, step * selectedIndex, step)
//   if Amount != 0 and Q' / Inc > 15 :  Inc2 = Q' / 10
//        entries = { k * Inc : k >= 1, k * Inc <= Amount }  U  { j * Inc2 : j >= 1, j * Inc2 > Amount, j * Inc2 <= Q' }
//   else entries = { k * step : k >= 1, k * step <= Q' }
// Selection handler (n >= 1, row active and not suspended):
//   Inc2 != 0 and Inc * n > Amount : SetAmount(Inc2 * (Amount / Inc2 - Amount / Inc + n))
//   otherwise                      : SetAmount(Inc * n)

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sc4af {

struct DealQuantityInput {
    std::int64_t maxAmountTruncated = 0;  // trunc(GetDealMaxAmount); <= 0 means the game answered "nothing"
    std::int32_t minAmount = 0;           // NDb_MinDealAmount* tuning (0xEA46932C power, 0x6A469337 water, 0x8A46933D garbage)
    std::int32_t amount = 0;              // deal.GetAmount()
    std::int32_t incAmount = 0;           // deal.GetIncAmount()
    std::int32_t incAmount2 = 0;          // deal.GetIncAmount2()
    bool active = false;                  // flags & 0x02
    bool suspended = false;               // flags & 0x300 (SetAmount refuses these)
};

struct DealQuantityGrid {
    bool listed = false;                  // the window would show a list
    std::int32_t step = 0;                // IncAmount after the window repaints the row
    std::int32_t step2 = 0;               // IncAmount2 after the window repaints the row (0 = single ladder)
    std::int64_t ceiling = 0;             // Q'
    std::vector<std::int32_t> amounts;    // entries 1..N of the combo in ascending order (entry 0 is "no deal")
    std::string note;
};

namespace dealqty_detail {
constexpr std::int64_t kMaxEntries = 4096;   // defensive cap; the window produces at most ~25 entries for a vanilla ratio
}

inline DealQuantityGrid EnumerateDealQuantities(const DealQuantityInput& in) {
    using namespace dealqty_detail;
    DealQuantityGrid g;
    const std::int64_t amount = in.amount;
    std::int64_t maxQ = in.maxAmountTruncated > 0 ? in.maxAmountTruncated : amount;
    const std::int64_t d10 = maxQ / 10;
    const std::int64_t minAmount = in.minAmount;
    std::int64_t step = std::max(d10, minAmount);
    std::int64_t selectedIndex = 0;

    if (in.suspended) {
        // The vanilla row can only keep what it has: both increments become the current amount.
        g.step = in.amount;
        g.step2 = in.amount;
        g.listed = in.amount > 0;
        g.ceiling = amount;
        if (g.listed) g.amounts.push_back(in.amount);
        g.note = "suspended: the only entry is the current amount (SetAmount refuses suspended deals)";
        return g;
    }
    if (!in.active) {
        // Proposal row: IncAmount := max(maxQ / 10, M). No active deal, nothing selected.
        selectedIndex = 0;
    } else {
        std::int64_t inc = in.incAmount;
        if ((inc == amount && inc > minAmount) || inc == 0) inc = d10;
        if (inc <= 0) {
            g.note = "active row with a zero increment: the window shows no list";
            return g;
        }
        step = inc;
        selectedIndex = amount / inc;
    }
    if (step <= 0) {
        g.note = "zero step: no list";
        return g;
    }
    std::int64_t ceiling = std::max<std::int64_t>({maxQ, step * selectedIndex, step});
    g.ceiling = ceiling;
    g.step = static_cast<std::int32_t>(step);

    if (in.active && amount != 0 && ceiling / step > 15) {
        const std::int64_t step2 = ceiling / 10;
        if (step2 > 0) {
            g.step2 = static_cast<std::int32_t>(step2);
            for (std::int64_t q = step; q <= amount && static_cast<std::int64_t>(g.amounts.size()) < kMaxEntries; q += step)
                g.amounts.push_back(static_cast<std::int32_t>(q));
            for (std::int64_t q = step2; q <= ceiling && static_cast<std::int64_t>(g.amounts.size()) < kMaxEntries; q += step2)
                if (q > amount) g.amounts.push_back(static_cast<std::int32_t>(q));
            g.listed = !g.amounts.empty();
            g.note = "two-level ladder: fine entries up to the current amount, coarse entries above it";
            return g;
        }
    }
    for (std::int64_t q = step; q <= ceiling && static_cast<std::int64_t>(g.amounts.size()) < kMaxEntries; q += step)
        g.amounts.push_back(static_cast<std::int32_t>(q));
    g.listed = !g.amounts.empty();
    g.note = "single ladder: k * step up to the ceiling";
    return g;
}

inline bool IsLegalDealQuantity(const DealQuantityGrid& grid, std::int64_t quantity) {
    return grid.listed && std::find(grid.amounts.begin(), grid.amounts.end(), quantity) != grid.amounts.end();
}

// What the combo handler 0x0078BCA0 stores for selection index n (1-based). The value is used to prove that the
// enumerated entries and the handler agree; the writer itself requests the amount directly (SetAmount only).
inline std::optional<std::int64_t> AmountForSelection(const DealQuantityInput& in, const DealQuantityGrid& grid, int n) {
    if (n < 1 || !grid.listed) return std::nullopt;
    const std::int64_t inc = grid.step;
    if (in.active && grid.step2 != 0 && inc * n > in.amount) {
        const std::int64_t inc2 = grid.step2;
        return inc2 * (static_cast<std::int64_t>(in.amount) / inc2 - static_cast<std::int64_t>(in.amount) / inc + n);
    }
    return inc * n;
}

// The legal entries nearest to a wished quantity (floor and ceiling in the vanilla list). Used by planners that think in
// continuous units but may only ask for listed ones.
struct DealQuantityNeighbors {
    std::optional<std::int32_t> below, above;   // largest entry <= wish, smallest entry >= wish
};

inline DealQuantityNeighbors NearestLegalQuantities(const DealQuantityGrid& grid, std::int64_t wish) {
    DealQuantityNeighbors r;
    for (const auto q : grid.amounts) {
        if (q <= wish) r.below = q;
        if (q >= wish && !r.above) r.above = q;
    }
    return r;
}

} // namespace sc4af
