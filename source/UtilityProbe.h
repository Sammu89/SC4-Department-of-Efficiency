// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "ServiceMonitor.h"
#include "UtilityConfig.h"
#include "UtilityModel.h"

#include <cstdint>
#include <map>
#include <string>

class cISC4City;
class cISC4RegionalCity;

namespace sc4af {

// READ layer: builds one immutable snapshot from the loaded city. Never writes anything.
UtilitySnapshot ReadUtilitySnapshot(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t epoch,
                                    bool epochMatches, bool includeProposals, bool probeLatent = false);

// Light read for the fast service check: a handful of authoritative counters from the power and plumbing simulators. It does not enumerate buildings beyond the plants' output and
// reads no budget lines or deal market. Returns false when neither half could be read.
bool ReadServiceCountersFromCity(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t epoch, ServiceCounters& out);

// Version-gated passive read of one byte (NeighborsSimulator +0x3E): the Neighbor Deals UI disabled flag.
Opt<bool> ReadNeighborDealsUiDisabled(cISC4City* city);

// Logs snapshots in the concise `[Utility][Subsystem]` format. Summary lines every month; per-building
// rows only on the first scan, when they change, or with detail=true. Empty subsystems are silent after
// their first mention.
class UtilityProbe {
public:
    void SetConfigDirectory(const std::string& directory) { configPath_ = directory + "DepartmentEfficiencyUtility.ini"; }
    void LoadConfigIfChanged();
    const UtilityResearchConfig& config() const { return config_; }
    std::string Banner() const;

    // City unloaded / reloaded: forgets all per-city state.
    void ResetCity() { ++epoch_; monthsSeen_ = 0; vtablesLogged_ = false; lastSummary_.clear(); lastDetail_.clear(); lastValidity_.clear(); initialDone_ = false; }
    std::uint32_t epoch() const { return epoch_; }

    // `reason` is only used in the log ("PostCityInit", "SimNewMonth", ...). The pointers are borrowed and used only during the call.
    void Observe(cISC4City* city, cISC4RegionalCity* regional, const char* reason, bool includeProposals);

    void LogVtables(cISC4City* city, cISC4RegionalCity* regional);

    const UtilitySnapshot& last() const { return last_; }

private:
    std::string configPath_;
    std::uint64_t lastWriteTime_ = ~0ull;
    UtilityResearchConfig config_;
    std::uint32_t epoch_ = 1;
    bool initialDone_ = false;
    std::uint32_t monthsSeen_ = 0;   // SimNewMonth observations in this epoch (proposals need simulation progress)
    bool vtablesLogged_ = false;
    UtilitySnapshot last_;
    std::map<std::string, std::string> lastSummary_;
    std::map<std::string, std::string> lastDetail_;
    std::map<std::string, std::string> lastValidity_;
};

} // namespace sc4af
