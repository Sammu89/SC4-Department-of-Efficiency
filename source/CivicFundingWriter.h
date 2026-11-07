// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstdint>
#include <string>

class cISC4City;

namespace sc4af {

// Isolated civic funding writer (development test harness, not the production controller).
// Disabled unless a well-formed, explicitly armed config file exists next to the DLL.
// One-shot per nonce; every write is compare-and-set and fully read back.
//
// Layering: READ (candidates) -> DECISION (PlanBuildingFundingWrite, pure) ->
// WRITE (vanilla SetOccupantFundingPercentages only) -> VERIFY (VerifyReadback, pure rules).
class CivicFundingWriter {
public:
    // Directory that contains DepartmentEfficiency.dll (config lives there).
    void SetConfigDirectory(const std::string& directory) { configPath_ = directory + "DepartmentEfficiency.writertest.ini"; }

    // Called from the main-thread poll timer. Re-reads the config when it changes and runs at
    // most one action per nonce. `msSinceCityInit` prevents acting while a city is still loading.
    void Poll(cISC4City* city, std::uint64_t msSinceCityInit);

    // Read-only monthly observation of the configured target (drift detection).
    void ObserveMonth(cISC4City* city, std::uint32_t month);

    // Clears per-city state (baseline). Executed nonces stay recorded for the process.
    void ResetCity();

private:
    bool LoadConfigIfChanged();

    std::string configPath_;
    std::uint64_t lastWriteTime_ = 0;
    struct Impl;
    // State kept in the .cpp (config, completed nonces, baseline).
    static Impl& State();
};

} // namespace sc4af
