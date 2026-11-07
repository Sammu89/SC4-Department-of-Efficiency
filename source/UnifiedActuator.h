// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "BuildFeatures.h"

// Game-facing half of the unified controller. Declared only when SC4AF_ENABLE_UNIFIED_AUTO is defined.

#ifdef SC4AF_ENABLE_UNIFIED_AUTO

#include "UnifiedAuto.h"
#include "UtilityActuator.h"

namespace sc4af {

// Borrowed pointers, valid only for the callback that created it (same contract as GameUtilityActuator, whose Power / Water
// write paths it reuses unchanged).
class GameUnifiedActuator final : public IUnifiedActuator {
public:
    GameUnifiedActuator(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t epoch, const UtilityResearchConfig& config,
                        const UnifiedAutoValidationCapability& capability)
        : base_(city, regional, epoch, config), city_(city), regional_(regional), epoch_(epoch), capability_(capability), discoverDeals_(DealDiscoveryAllowed(config)) {}

    bool Reread(UtilitySnapshot& out) override { return base_.Reread(out); }
    ActuatorResult WritePlant(int x, int z, int from, int to, UtilitySnapshot& after) override { return base_.WritePlant(x, z, from, to, after); }
    ActuatorResult WriteWater(int from, int to, UtilitySnapshot& after) override { return base_.WriteWater(from, to, after); }

    bool ReadForPlan(UtilitySnapshot& out) override;
    bool ReadServiceCounters(ServiceCounters& out) override;
    bool RefreshCommodityStats() override;
    Opt<std::int64_t> Quote(const DealKey& key, std::int64_t amount) override;
    DealStep PumpDeal(DealWriteMachine& machine, std::uint32_t lifecycleEpoch) override;
    SanitationStep PumpSanitation(SanitationWriteMachine& machine, std::uint32_t lifecycleEpoch) override;

private:
    GameUtilityActuator base_;
    cISC4City* city_;
    cISC4RegionalCity* regional_;
    std::uint32_t epoch_;
    UnifiedAutoValidationCapability capability_;
    bool discoverDeals_;   // DealDiscoveryAllowed(config): false = existing contracts are read, ProposeDeals is never called
};

} // namespace sc4af

#endif
