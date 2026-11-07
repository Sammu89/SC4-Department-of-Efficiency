// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure, game-free tuning bundle and asset-health model for utilities.
//
// Model (validated at runtime against the game):
//  * plant efficiency = funding curve(funding %) x condition
//  * plant aging per simulated month = baseRate(0xEA1CF220) x usageAging(usage) x fundingAging(funding) / 16
//    The structure comes from the aging routine at 0x006EC8F0; the 1/16 factor is empirical (ratio 0.0625 +-0.7% at
//    100% funding, 0.99-1.0 of that at 95% funding).
//  * GetMonthlyPowerGenerationCost returns the budget item's nominal cost (0x006EB2F0); the charged expense is
//    cost x local funding (water department: 4492 -> 4268 at 95%).
// The curves below are the vanilla tables read from the game data; an installation that overrides them must supply
// its own UtilityTuning.

#include "UtilityModel.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace sc4af {

struct UtilityTuning {
    Curve powerEfficiency;   // 0x6911E2DF  funding % -> efficiency %
    Curve waterEfficiency;   // 0x6A410BF2  funding % -> efficiency %
    Curve fundingAging;      // 0x0911E2E8  funding fraction -> aging multiplier
    Curve usageAging;        // 0x0911E2E9  usage fraction -> aging multiplier
    Curve pipeDecay;         // 0xCA7138C4  water funding % -> pipe decay(-)/repair(+)
    Curve poleDecay;         // 0xAA713782  POWER DEPARTMENT funding % -> pole decay(-)/repair(+); department funding is the
                             // cost-weighted average of the plants ((11660-15)/11660 = 0.998713 in a measured city)
    int powerMinPercent = 60;    // 0x29121E8B
    int powerMaxPercent = 120;   // 0xE911E2D1
    int waterMaxPercent = 120;   // 0xEA4025A9
    double agingTimeScale = 1.0 / 16.0;
};

inline UtilityTuning VanillaUtilityTuning() {
    UtilityTuning t;
    ParseCurve({0, 0, 50, 25, 90, 80, 100, 100, 120, 110}, t.powerEfficiency);
    ParseCurve({0, 0, 50, 30, 90, 80, 100, 100, 120, 110}, t.waterEfficiency);
    ParseCurve({0, 1.1, 1.0, 1.0, 1.2, 0.9}, t.fundingAging);
    ParseCurve({0, 0.1, 0.25, 0.2, 0.5, 0.45, 0.75, 0.85, 0.9, 1.25, 1.0, 1.55, 1.1, 2.0}, t.usageAging);
    ParseCurve({0, -0.01, 20, -0.005, 50, 0, 100, 0.02, 110, 0.05}, t.pipeDecay);
    ParseCurve({0, -0.02, 25, -0.005, 50, 0, 100, 0.025, 110, 0.05}, t.poleDecay);
    return t;
}

inline bool TuningValid(const UtilityTuning& t) {
    return t.powerEfficiency.valid() && t.waterEfficiency.valid() && t.fundingAging.valid() && t.usageAging.valid() &&
           t.pipeDecay.valid() && t.poleDecay.valid();
}

// Condition lost per simulated month (positive = aging).
inline double AgingPerMonth(const UtilityTuning& t, double baseRate, double usage, double fundingFraction) {
    return baseRate * t.usageAging.Eval(std::clamp(usage, 0.0, 2.0)) * t.fundingAging.Eval(fundingFraction) * t.agingTimeScale;
}

// Months until the condition falls to `threshold` at constant usage/funding. +inf when it never does.
inline double MonthsUntilThreshold(const UtilityTuning& t, double condition, double threshold, double baseRate, double usage,
                                   double fundingFraction) {
    if (condition <= threshold) return 0.0;
    const double rate = AgingPerMonth(t, baseRate, usage, fundingFraction);
    if (rate <= 0.0) return std::numeric_limits<double>::infinity();
    return (condition - threshold) / rate;
}

inline int UtilityPercentRound(float fraction) { return static_cast<int>(std::lround(fraction * 100.0f)); }

// Monthly expense actually charged for a budget line (same rounding as the civic lines).
inline std::int64_t ChargedExpense(std::int64_t nominalCost, int fundingPercent) {
    return static_cast<std::int64_t>(std::llround(static_cast<double>(nominalCost) * fundingPercent / 100.0));
}

// Days between two calendar stamps, assuming 365-day years (deal terms are whole years).
inline std::int64_t DaysBetween(const DateStamp& from, const DateStamp& to) {
    return (static_cast<std::int64_t>(to.year) - from.year) * 365 + (static_cast<std::int64_t>(to.dayOfYear) - from.dayOfYear);
}

} // namespace sc4af
