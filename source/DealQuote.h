// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure, exact reproduction of the Neighbor Deal pricing functions of SimCity 4.exe 1.1.641.0:
//   GetDealMaxAmount 0x006D3620, GetDealCost 0x006D3250, UpdateCommodityStats 0x006D2A20, the ProposeDeals gates 0x006D21A0.
// Given the local and neighbor commodity records, the formulas below reproduce the game's SELL_POWER quotes (floor of
// the double) and GetDealMaxAmount answers exactly.
//
// Three distinct economic quantities, never mixed:
//   StoredActiveContractCost   cISC4NeighborDeal::GetCost(), written by UpdateCost when the amount was last set. Authoritative for an
//                              existing contract; never re-priced from today's quote.
//   CurrentVanillaQuote(Q)     GetDealCost(type, Q, neighbor) read right now = DealCostExact on the cached records. Read-only, valid for
//                              any neighbor without a proposal and without surplus; stale until UpdateCommodityStats runs.
//   HypotheticalFutureQuote    DealCostExact on records predicted for a changed local state (more capacity, different expense). Exact
//                              in its formula, only as good as the predicted records; confirmed by a fresh quote before any create.
//
// Local city = SELLER for SELL_POWER / SELL_WATER / IMPORT_GARBAGE, BUYER for BUY_POWER / BUY_WATER / EXPORT_GARBAGE.

#include "SafeMoney.h"
#include "CommodityBook.h"
#include "UtilityModel.h"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace sc4af {

// NDe_MaxUnitCostForGarbageDealsRCurve (0x4B61FFC9). UpdateCommodityStats reloads the shared curve member with it, so this
// is the curve GetDealCost actually evaluates (not NDe_ProfitResponseCurve 0x0A469353).
inline Curve VanillaDealPriceCurve() {
    Curve c;
    ParseCurve({0.0, 1e-5, 0.0075, 0.0075, 1.0, 0.075}, c);
    return c;
}

struct DealMarketTuning {
    double minAmount[3] = {1000.0, 1000.0, 1000.0};   // NDb_MinDealAmount power 0xEA46932C, water 0x6A469337, garbage 0x8A46933D
    double overDemandFactor = 2.5;                     // NDa_MaxOverDemandFactorForBuyer 0xEA4AD2FF
    double minBudget = 5000.0;                         // NDa_MinBudgetForBuyDeals 0xEA4AE9EF
    double minTerm = 60.0;                             // NDa_MinTermToSustainSellDealCost 0xCA46931F
    double minProfitFactor = 0.1;                      // NDc_MinDealProfitFactor 0x6A469342
    double maxProfitFactor = 0.5;                      // NDc_MaxDealProfitFactor 0xCA469346
    double maxTermYears = 5.0;                         // 0xEA46934F, bounds the IMPORT_GARBAGE capacity rule
    Curve priceCurve = VanillaDealPriceCurve();
};

inline double MinAmountOf(const DealMarketTuning& t, std::int32_t dealType) {
    const int ci = dealType - utility_id::kDealBuyPower;
    return t.minAmount[std::clamp(ci / 2, 0, 2)];
}

// GetDealMaxAmount: max(0, min(SellerSurplus, BuyerNeed)); `cur` is the amount of the existing deal of that type with that city.
inline double DealMaxAmountExact(const CommodityBook& seller, const CommodityBook& buyer, double minAmount, double overDemandFactor, double cur) {
    const double thr = minAmount / overDemandFactor + 1.0;
    const double y = std::max(buyer.demanded + buyer.exported, thr);
    const double buyerNeed = overDemandFactor * y - (buyer.produced + buyer.imported) + cur;
    const double sellerSurplus = (seller.produced + seller.imported) - (seller.demanded + seller.exported) + cur;
    return std::max(0.0, std::min(sellerSurplus, buyerNeed));
}

// GetDealCost: amount * clamp(unit*(1+minPF) + 0.5*(g(V/amount) + g(R/Ssup))*unit*(maxPF-minPF), unit*(1+minPF), unit*(1+maxPF)),
// unit = seller.costPerUnit. Returns the exact double the game returns (the stored cost is its truncation, minimum 1).
inline double DealCostExact(double amount, const CommodityBook& seller, const CommodityBook& buyer, double cur, double minAmount,
                            const DealMarketTuning& t) {
    if (amount < 1e-8) return 0.0;
    const double unit = seller.costPerUnit;
    const double costMin = unit * (1.0 + t.minProfitFactor);
    const double costMax = unit * (1.0 + t.maxProfitFactor);
    const double y = std::max(buyer.demanded + buyer.exported, minAmount / t.overDemandFactor + 1.0);
    const double v = 1.0 + (buyer.produced + buyer.imported - cur) - y;
    const double ssup = std::max(seller.produced + seller.imported, 1e-8);
    const double r = ssup - amount - (seller.demanded + seller.exported - cur);
    const double g1 = t.priceCurve.Eval(v / amount);
    const double g2 = t.priceCurve.Eval(r / ssup);
    const double price = costMin + 0.5 * (g1 + g2) * (costMax - costMin);
    return std::clamp(price, costMin, costMax) * amount;
}

// QuoteCurrentState: what GetDealCost answers right now for `amount` while a deal of `cur` exists (the books contain `cur`; the formula adds it back).
// This is what the game stores immediately after SetAmount.
inline double QuoteCurrentState(double amount, const CommodityBook& seller, const CommodityBook& buyer, double cur, double minAmount, const DealMarketTuning& t) {
    return DealCostExact(amount, seller, buyer, cur, minAmount, t);
}

// QuotePostDealState: the price of `amount` in the state after the deal is at `amount`: the seller's Exported and the buyer's Imported carry
// `amount - cur` more, and the later monthly maintenance prices the contract with cur = amount. The books shift by exactly what `cur`
// removes, so the price of a quantity does not depend on the quantity the deal holds today: one value per quantity, valid before and after
// the write. This keeps quotes for different amounts comparable; pricing the current contract and an alternative against different books
// would compare two different states and could make the choice oscillate between amounts.
inline double QuotePostDealState(double amount, const CommodityBook& seller, const CommodityBook& buyer, double cur, double minAmount, const DealMarketTuning& t) {
    CommodityBook s = seller, b = buyer;
    s.exported += amount - cur;
    b.imported += amount - cur;
    return DealCostExact(amount, s, b, amount, minAmount, t);
}

// What UpdateCost stores: trunc(GetDealCost), minimum 1 (0x006CF9C0).
inline std::int64_t StoredCostFromQuote(double quote) {
    const auto v = ClampMoney(quote);   // NaN / infinity / out-of-range doubles are not cast blindly
    return v < 1 ? 1 : v;
}

// UpdateCommodityStats (0x006D2D83..0x006D2DBA, same pattern per commodity): CostPerUnit = (department expenses + cost of the BUY-type
// deals of that commodity only) / (Imported + Produced + 1). Buy deals of other commodities must not be added. Example: 11661 / (0 + 76152 + 1) = 0.153126.
inline double LocalCostPerUnit(double departmentExpenses, double buyDealCostOfCommodity, double imported, double produced) {
    return (departmentExpenses + buyDealCostOfCommodity) / (imported + produced + 1.0);
}

// ProposeDeals gates (0x006D2445..0x006D25F2): a proposal exists only when GetDealMaxAmount >= M (non-strict) and the relevant treasury
// (the local one for BUY types, the neighbor's for SELL types) satisfies three strict tests against cost(M).
struct ProposalGateResult {
    bool maxAmountOk = false, treasuryOk = false;
    bool offered() const { return maxAmountOk && treasuryOk; }
};

inline ProposalGateResult VanillaProposalGate(double maxAmount, double minAmount, double costAtMin, double treasury, const DealMarketTuning& t) {
    ProposalGateResult r;
    r.maxAmountOk = maxAmount >= minAmount;
    r.treasuryOk = t.minBudget < treasury && treasury > costAtMin && t.minTerm * costAtMin < treasury;
    return r;
}

// Maintenance rule that suspends garbage imports (0x006D0F68..0x006D1018): the total of IMPORT_GARBAGE contracts (the local garbage
// Exported record) may not exceed  free landfill storage * (1 - 1/(termMonths + 0.000107)) + (Produced + Imported garbage).
// When it does, the surplus contracts get flag 0x20 in list order and are suspended by the next maintenance pass.
inline double ImportGarbageContractLimit(double freeLandfillStorage, double maxTermYears, double garbageProduced, double garbageImported) {
    const double termMonths = maxTermYears * 12.0;
    return freeLandfillStorage * (1.0 - 1.0 / (termMonths + 0.000107)) + (garbageProduced + garbageImported);
}

} // namespace sc4af
