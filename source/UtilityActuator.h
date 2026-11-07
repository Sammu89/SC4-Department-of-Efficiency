// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "UtilityAuto.h"

class cISC4City;
class cISC4RegionalCity;

namespace sc4af {

// Game-facing half of utility Auto. Borrowed pointers, valid only for the SimNewMonth callback that created it.
// Every write is: fresh read -> pure precondition plan (same planner as the isolated writers) -> structural
// re-acquire of the target -> compare-and-set -> the one vanilla setter -> immediate read-back verification.
class GameUtilityActuator final : public IUtilityActuator {
public:
    GameUtilityActuator(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t epoch, const UtilityResearchConfig& config)
        : city_(city), regional_(regional), epoch_(epoch), config_(config) {}

    bool Reread(UtilitySnapshot& out) override;
    ActuatorResult WritePlant(int cellX, int cellZ, int fromPercent, int toPercent, UtilitySnapshot& after) override;
    ActuatorResult WriteWater(int fromPercent, int toPercent, UtilitySnapshot& after) override;

private:
    cISC4City* city_;
    cISC4RegionalCity* regional_;
    std::uint32_t epoch_;
    const UtilityResearchConfig& config_;
};

} // namespace sc4af
