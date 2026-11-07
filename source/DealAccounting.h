// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure reproduction of how vanilla ACCOUNTS for neighbor deals, so a writer can verify its effect against authoritative
// totals instead of against its own intent. Derived from static analysis of SimCity 4.exe 1.1.641.0.
//
//  * RegionalCity import/export (UpdateImportExport 0x004AA200 -> per deal 0x004AA0E0): the deal contributes GetAmount()
//    to the buyer side's Imported and the seller side's Exported UNLESS CheckFlagsOr(0xBC) is true, i.e. unless it carries
//    0x04 cancelled, 0x08 no connection, 0x10 buyer cannot pay, 0x20 no capacity, or 0x80 not in the simulator list.
//    PENDING proposals (0x01) and SUSPENDED deals (0x100/0x200) therefore DO count.
//  * NeighborsSimulator::GetTotalMonthlyNeighborDealIncome (0x006CF860) sums GetCost() of the sell-type deals
//    (SELL_POWER, SELL_WATER, IMPORT_GARBAGE) and ...Expense (0x006CF8D0) of the buy-type deals (BUY_POWER, BUY_WATER,
//    EXPORT_GARBAGE) unless CheckFlagsOr(0x3D) (0x01 pending, 0x04 cancelled, 0x08/0x10/0x20 invalid). Active deals and
//    suspended deals count; pending proposals never do.
// The two rules differ on purpose: a pending proposal moves the regional flows but never the money.

#include "UtilityModel.h"

#include <cstdint>
#include <vector>

namespace sc4af {

namespace deal_flag {
constexpr std::int32_t kPending = 0x01;
constexpr std::int32_t kActive = 0x02;
constexpr std::int32_t kCancelled = 0x04;
constexpr std::int32_t kNoConnection = 0x08;
constexpr std::int32_t kBuyerCannotPay = 0x10;
constexpr std::int32_t kNoCapacity = 0x20;
constexpr std::int32_t kInvalidMask = 0x38;
constexpr std::int32_t kNotInList = 0x80;
constexpr std::int32_t kSuspended = 0x100;
constexpr std::int32_t kSuspensionElapsed = 0x200;
constexpr std::int32_t kSuspensionMask = 0x300;
constexpr std::int32_t kRegionalSkipMask = 0xBC;   // 0x04 | 0x08 | 0x10 | 0x20 | 0x80
constexpr std::int32_t kMoneySkipMask = 0x3D;      // 0x01 | 0x04 | 0x08 | 0x10 | 0x20
}

inline bool DealCountsForRegionalFlow(std::int32_t flags) { return (flags & deal_flag::kRegionalSkipMask) == 0; }
inline bool DealCountsForMonthlyMoney(std::int32_t flags) { return (flags & deal_flag::kMoneySkipMask) == 0; }

// Local city is the BUYER for BUY_POWER, BUY_WATER, EXPORT_GARBAGE (we pay): contributes to Imported(commodity) and to
// the monthly EXPENSE. Local city is the SELLER for SELL_POWER, SELL_WATER, IMPORT_GARBAGE (we are paid): Exported and INCOME.
inline bool LocalIsBuyer(std::int32_t dealType) {
    return dealType == utility_id::kDealBuyPower || dealType == utility_id::kDealBuyWater || dealType == utility_id::kDealExportGarbage;
}

// An OBLIGATION the city owes: a real contract (ACTIVE 0x02, suspended or not) that carries none of the cancelled / invalid / not-in-list
// flags. Pending proposals are vanilla bookkeeping (they do move the regional flows, see above) and are NOT obligations.
inline bool DealIsCommitted(const ExistingDealRow& d) {
    if (!d.flags) return false;
    const auto f = *d.flags;
    return (f & deal_flag::kActive) && !(f & (deal_flag::kCancelled | deal_flag::kInvalidMask | deal_flag::kNotInList));
}

inline std::int64_t CommittedAmount(const std::vector<ExistingDealRow>& deals, std::int32_t dealType) {
    std::int64_t total = 0;
    for (const auto& d : deals)
        if (d.type == dealType && d.amount && DealIsCommitted(d)) total += *d.amount;
    return total;
}

// What the regional city and the simulator totals must read for a list of deals (semantic rows, no pointers).
struct ReconstructedAccounting {
    std::int64_t imported[3] = {0, 0, 0};   // by commodity index (power, water, garbage)
    std::int64_t exported[3] = {0, 0, 0};
    std::int64_t income = 0;
    std::int64_t expense = 0;
};

inline ReconstructedAccounting ReconstructAccounting(const std::vector<ExistingDealRow>& deals) {
    ReconstructedAccounting r;
    for (const auto& d : deals) {
        if (!IsKnownDealType(d.type) || !d.flags) continue;
        const auto c = CommodityOfDealType(d.type);
        const int ci = c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : 2;
        const bool buyer = LocalIsBuyer(d.type);
        // 0x004AA0E0 returns at once when the deal has no city on the other side of its connection.
        if (d.otherCitySerial && DealCountsForRegionalFlow(*d.flags) && d.amount) (buyer ? r.imported : r.exported)[ci] += *d.amount;
        if (DealCountsForMonthlyMoney(*d.flags) && d.cost) (buyer ? r.expense : r.income) += *d.cost;
    }
    return r;
}

} // namespace sc4af
