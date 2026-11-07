// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Game-facing executor of the Neighbor Deal transaction machine (NeighborDealWriterPlan.h). HARD-DISABLED:
//
//  * No production code calls it.
//  * Executing a primitive needs a DealWriterResearchToken. The only way to obtain one is DealWriterResearchSeam::Arm(), which is
//    defined only when the translation unit is compiled with SC4AF_ENABLE_DEAL_WRITER_RESEARCH. No INI key, no environment
//    variable and no default CMake target defines it, so a normal build cannot even express a call.
//  * Reading (Observe, Quote, RefreshCommodityStats) needs no token: they never change a deal.

#include "NeighborDealWriterPlan.h"

#include <cstdint>

class cISC4City;
class cISC4RegionalCity;

namespace sc4af {

class DealWriterResearchToken {
public:
    DealWriterResearchToken(const DealWriterResearchToken&) = default;
    DealWriterResearchToken& operator=(const DealWriterResearchToken&) = default;

private:
    DealWriterResearchToken() = default;
    friend class DealWriterResearchSeam;
    friend class FinalValidationExecutorSeam;
};

#ifdef SC4AF_ENABLE_DEAL_WRITER_RESEARCH
class DealWriterResearchSeam {
public:
    static DealWriterResearchToken Arm() { return DealWriterResearchToken(); }
};
#endif

class NeighborDealTransactor {
public:
    // Read-only. A fresh semantic observation for the key (rows, RegionalCity flows, money totals, GetDealMaxAmount, tuned minimum).
    // `valid == false` when any required part could not be read; nothing is defaulted to zero.
    static DealObservation Observe(cISC4City* city, cISC4RegionalCity* regional, const DealKey& key, std::uint32_t lifecycleEpoch);

    // Read-only for deals. UpdateCommodityStats(city) refreshes the LOCAL RegionalCity commodity cache exactly as vanilla does
    // monthly and at the start of every ProposeDeals; GetDealCost/GetDealMaxAmount read that cache, so a quote taken after a
    // local capacity change is stale until this runs.
    static bool RefreshCommodityStats(cISC4City* city);

    // Read-only. GetDealCost(type, amount, neighbor) exactly as the game answers it right now (truncated to whole money).
    static Opt<std::int64_t> Quote(cISC4City* city, const DealKey& key, std::int64_t amount);

    // One host iteration: Observe -> machine.Next -> execute AT MOST ONE primitive -> machine.Executed. Returns the step taken.
    static DealStep Pump(const DealWriterResearchToken& token, DealWriteMachine& machine, cISC4City* city,
                         cISC4RegionalCity* regional, std::uint32_t lifecycleEpoch);
};

} // namespace sc4af
