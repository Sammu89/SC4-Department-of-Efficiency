// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure reproduction of the INPUTS of vanilla's sell-power eligibility (cISC4NeighborsSimulator, SimCity 4.exe 1.1.641.0):
//   GetDealMaxAmount 0x6D3620, ProposeDeals 0x6D21A0, GetDealCost 0x6D3250.
// The runtime layer feeds it the regional-city commodity stats (float getters) and the loaded
// tuning; the trace logs the game's own answer next to this calculation so any divergence is visible.
//
// Local city = SELLER, neighbor = BUYER, commodity = power.
//   BuyerNeed     = F * max(B.Demanded + B.Exported, M/F + 1) - (B.Produced + B.Imported) + cur
//   SellerSurplus = (S.Produced + S.Imported) - (S.Demanded + S.Exported) + cur
//   GetDealMaxAmount = max(0, min(SellerSurplus, BuyerNeed))
//   ProposeDeals offers SELL iff no deal of that type exists with the neighbor, a suitable connection exists,
//       GetDealMaxAmount >= M   (NON-strict),  and the BUYER'S treasury B.Budget satisfies
//       B.Budget > MinBudget  &&  B.Budget > cost(M)  &&  B.Budget > MinTerm * cost(M),   cost(M) = GetDealCost at amount M.

#include <algorithm>
#include <cmath>
#include <optional>

namespace sc4af {

struct CommodityStats {
    double produced = 0.0, imported = 0.0, demanded = 0.0, exported = 0.0;
};

struct SellTuning {
    double minAmount = 1000.0;      // NDb_MinDealAmountPower 0xEA46932C
    double overDemandFactor = 2.5;  // NDa_MaxOverDemandFactorForBuyer 0xEA4AD2FF
    double minBudget = 5000.0;      // NDa_MinBudgetForBuyDeals 0xEA4AE9EF
    double minTerm = 60.0;          // NDa_MinTermToSustainSellDealCost 0xCA46931F
};

inline double BuyerNeed(const CommodityStats& buyer, const SellTuning& t, double existingDeal = 0.0) {
    const double thr = t.minAmount / t.overDemandFactor + 1.0;
    const double y = std::max(buyer.demanded + buyer.exported, thr);
    return t.overDemandFactor * y - (buyer.produced + buyer.imported) + existingDeal;
}

inline double SellerSurplus(const CommodityStats& seller, double existingDeal = 0.0) {
    return (seller.produced + seller.imported) - (seller.demanded + seller.exported) + existingDeal;
}

inline double DealMaxAmount(const CommodityStats& seller, const CommodityStats& buyer, const SellTuning& t, double existingDeal = 0.0) {
    return std::max(0.0, std::min(SellerSurplus(seller, existingDeal), BuyerNeed(buyer, t, existingDeal)));
}

inline bool BuyerTreasuryOk(double buyerBudget, double costAtMin, const SellTuning& t) {
    return t.minBudget < buyerBudget && buyerBudget > costAtMin && t.minTerm * costAtMin < buyerBudget;
}

struct LatentSell {
    bool eligibleNow = false;        // vanilla would offer it right now (max >= M and the treasury tests pass)
    bool everEligible = false;       // buyer side allows it if the seller had enough surplus (need >= M and treasury ok)
    double buyerNeed = 0.0;          // upper bound of any deal with this neighbor at the current buyer state
    double sellerSurplus = 0.0;
    double gameMax = 0.0;            // what GetDealMaxAmount returns
    double extraSurplusNeeded = 0.0; // capacity to add before vanilla considers the deal (0 when eligible now)
    double maxIfSurplusSufficient = 0.0;   // largest deal the buyer side would allow
};

inline LatentSell EvaluateLatentSell(const CommodityStats& seller, const CommodityStats& buyer, double buyerBudget, double costAtMin,
                                     const SellTuning& t, double existingDeal = 0.0) {
    LatentSell r;
    r.buyerNeed = BuyerNeed(buyer, t, existingDeal);
    r.sellerSurplus = SellerSurplus(seller, existingDeal);
    r.gameMax = DealMaxAmount(seller, buyer, t, existingDeal);
    const bool treasury = BuyerTreasuryOk(buyerBudget, costAtMin, t);
    r.everEligible = treasury && r.buyerNeed >= t.minAmount;
    r.eligibleNow = treasury && r.gameMax >= t.minAmount;
    r.maxIfSurplusSufficient = std::max(0.0, r.buyerNeed);
    r.extraSurplusNeeded = r.everEligible && !r.eligibleNow ? std::max(0.0, t.minAmount - r.sellerSurplus) : 0.0;
    return r;
}

} // namespace sc4af
