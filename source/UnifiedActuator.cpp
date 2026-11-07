// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "BuildFeatures.h"
#include "UnifiedActuator.h"

#ifdef SC4AF_ENABLE_UNIFIED_AUTO

#include "FinalValidationSeam.h"
#include "UtilityProbe.h"

namespace sc4af {

bool GameUnifiedActuator::ReadForPlan(UtilitySnapshot& out) {
    if (!city_) return false;
    // includeProposals: ProposeDeals(list, false) refreshes the commodity cache and the neighbor market is read right after it. Only when the deal controller
    // may act (DealDiscoveryAllowed); otherwise the existing contracts are read and the proposals/markets stay "not read" (unknown, never "none").
    out = ReadUtilitySnapshot(city_, regional_, epoch_, true, discoverDeals_, false);
    return true;
}

bool GameUnifiedActuator::ReadServiceCounters(ServiceCounters& out) { return ReadServiceCountersFromCity(city_, regional_, epoch_, out); }

bool GameUnifiedActuator::RefreshCommodityStats() { return NeighborDealTransactor::RefreshCommodityStats(city_); }

Opt<std::int64_t> GameUnifiedActuator::Quote(const DealKey& key, std::int64_t amount) { return NeighborDealTransactor::Quote(city_, key, amount); }

DealStep GameUnifiedActuator::PumpDeal(DealWriteMachine& machine, std::uint32_t lifecycleEpoch) {
    return NeighborDealTransactor::Pump(FinalValidationExecutorSeam::DealToken(capability_), machine, city_, regional_, lifecycleEpoch);
}

SanitationStep GameUnifiedActuator::PumpSanitation(SanitationWriteMachine& machine, std::uint32_t lifecycleEpoch) {
    return SanitationFundingTransactor::Pump(FinalValidationExecutorSeam::SanitationToken(capability_), machine, city_, regional_, lifecycleEpoch);
}

} // namespace sc4af

#endif
