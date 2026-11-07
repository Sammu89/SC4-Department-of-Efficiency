// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "UtilityModel.h"
#include "UtilityWriterConfig.h"
#include "UtilityWriterPlan.h"

#include <cstdint>
#include <set>
#include <string>

class cISC4City;
class cISC4RegionalCity;

namespace sc4af {

// Isolated writer harness for power plant funding and water department funding (development only). Disarmed unless an
// armed DepartmentEfficiencyUtilityWriterTest.ini exists. It applies at most one change per nonce, verifies it by
// re-reading, and from then on only observes every month (drift watch, expense and service telemetry, save/reload
// persistence). It never applies any automatic policy.
class UtilityWriter {
public:
    void SetConfigDirectory(const std::string& directory) {
        configPath_ = directory + "DepartmentEfficiencyUtilityWriterTest.ini";
        appliedPath_ = directory + "DepartmentEfficiencyUtilityWriterTest.applied";
    }
    // City unloaded / (re)loaded. If a change is being observed, this marks a reload: the following months verify that
    // the game kept the funding that was written and did not move the slider.
    void ResetCity();
    // Called once per SimNewMonth, after the utility probe refreshed its snapshot.
    void OnMonth(cISC4City* city, cISC4RegionalCity* regional, const UtilitySnapshot& snapshot);
    bool armed() const { return config_.enabled; }

private:
    void LoadConfigIfChanged();
    bool AlreadyApplied(const std::string& nonce);
    void MarkApplied(const std::string& nonce);
    void Observe(const UtilitySnapshot& snapshot);
    UtilityWriteResult ApplyPower(cISC4City* city, cISC4RegionalCity* regional, const UtilitySnapshot& snapshot);
    UtilityWriteResult ApplyWater(cISC4City* city, cISC4RegionalCity* regional, const UtilitySnapshot& snapshot);

    std::string configPath_, appliedPath_;
    std::uint64_t lastWriteTime_ = ~0ull;
    UtilityWriterConfig config_;
    std::uint32_t monthsInCity_ = 0;

    // The change that was written and is still being watched (survives city reloads within one game process).
    struct Watch {
        bool active = false;
        int cellX = 0, cellZ = 0;
        float fraction = 0.0f;
    };
    Watch power_, water_;
    std::string appliedNonce_;
    std::uint32_t reloadsSinceApply_ = 0;
    std::uint32_t monthsSinceReload_ = 0;
    bool anyWatch() const { return power_.active || water_.active; }
};

} // namespace sc4af
