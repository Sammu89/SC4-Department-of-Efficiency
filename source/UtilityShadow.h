// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "UtilityConfig.h"
#include "UtilityModel.h"
#include "UtilityOptimizer.h"
#include "UtilityPlanFormat.h"

#include <cstdint>

namespace sc4af {

// Shadow mode: every month, build a plan from the REAL current snapshot and log it. Performs zero writes and
// keeps no state that could turn a plan into an action (no memory of "what we recommended" is ever applied).
class UtilityShadow {
public:
    void ResetCity() { months_ = 0; }
    // Called once per SimNewMonth after the probe refreshed its snapshot.
    void OnMonth(const UtilitySnapshot& snapshot, const UtilityResearchConfig& config);
    // Power cadence (every PowerCadenceDays SimNewDay events): power plan only.
    void OnPowerCycle(const UtilitySnapshot& snapshot, const UtilityResearchConfig& config, const SubsystemPlan& plan);

private:
    std::uint32_t months_ = 0;
};

} // namespace sc4af
