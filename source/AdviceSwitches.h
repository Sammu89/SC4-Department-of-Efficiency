// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// The game's own advice follows the Department. DepartmentEfficiency.dat carries two small Lua scripts of the Department's own (its two Lua scripts, built by the project tools) that extend, in
// place, the trigger of existing advice records of the game with switches; this file decides their value and keeps what was published in the current city. The plugin writes them with
// cISC4AdvisorSystem::SetGlobalValue:
//
//   g_sc4af_health_quiet / g_sc4af_school_quiet                          Education / Health funding advice (civic Mode=Auto + the service + the civic writer trusted)
//   g_sc4af_nd_power_quiet / g_sc4af_nd_water_quiet / g_sc4af_nd_garbage_quiet   the neighbor-deal advice of each resource (the unified controller + AutoDeals + AutoTrade<Resource> + the deals writer trusted)
//   g_sc4af_garbage_quiet                                                 the garbage alerts (the unified controller + AutoGarbage + the sanitation writer trusted)
//
//   1 = the Department really manages that: the advice follows the Department's policy;
//   0 = it does not (switched off, another mode, a writer that failed): the game's own advice is exactly as the game ships it. A switch nobody wrote is nil, which the Lua reads as 0.
//
// OWNER RULE (2026-11-08): the game's advice is silenced ONLY while the Department can really manage what the advice is about. The advisor system, and with it the Lua state that holds the
// switches, is rebuilt for every city (measured), so the plugin publishes at every PostCityInit and again whenever something that decides a value changes. Pure logic here
// (unit-tested); the calls into the game are in Plugin.cpp.
#pragma once

#include "CivicPolicy.h"
#include "UtilityConfig.h"

#include <array>
#include <functional>
#include <string>

namespace sc4af {

constexpr int kAdviceSwitchCount = 6;
constexpr std::array<const char*, kAdviceSwitchCount> kAdviceSwitchNames = {
    "g_sc4af_health_quiet", "g_sc4af_school_quiet", "g_sc4af_nd_power_quiet", "g_sc4af_nd_water_quiet", "g_sc4af_nd_garbage_quiet", "g_sc4af_garbage_quiet"};

struct AdviceSwitches {
    bool v[kAdviceSwitchCount] = {};
    bool health() const { return v[0]; }
    bool school() const { return v[1]; }
    bool dealsPower() const { return v[2]; }
    bool dealsWater() const { return v[3]; }
    bool dealsGarbage() const { return v[4]; }
    bool garbage() const { return v[5]; }
    bool operator==(const AdviceSwitches& o) const { for (int i = 0; i < kAdviceSwitchCount; ++i) if (v[i] != o.v[i]) return false; return true; }
    bool operator!=(const AdviceSwitches& o) const { return !(*this == o); }
};

// What the controllers report about their own ability to act (all true / false = healthy by default)
struct AdviceOperational {
    bool unifiedActive = false;     // the unified controller runs (compile-time capability + Mode=Auto + Enabled + EnableUnifiedAuto ...): UnifiedGateOpen
    bool unifiedFailed = false;     // its session failed (fail-closed): no write is made any more
    bool dealsDisabled = false;     // Deal Auto was disabled for this city session after a technical failure
    bool garbageDisabled = false;   // the sanitation funding writer was disabled for this city session after a technical failure
    bool civicWritesAllowed = true; // the civic writer is trusted and not hard-stopped (AutoFinance::WritesAllowed)
    bool civicSnapshotUsable = true; // the latest civic facility snapshot is not INVALID (AutoFinance::SnapshotUsable)
    bool ready[3] = {true, true, true}; // the unified controller's readings of power, water, garbage (and the contracts) are ready; false = it neither writes nor trades for that commodity
};

inline AdviceSwitches AdviceSwitchesFor(const AutoFinanceConfig& civic, const UtilityResearchConfig& util, const AdviceOperational& op) {
    AdviceSwitches s;
    const bool civicOn = civic.mode == AutoMode::Auto && op.civicWritesAllowed && op.civicSnapshotUsable && !op.unifiedFailed;      // a failed unified session also hard-stops the civic writer (applied monthly by Plugin.cpp): the advice returns at once
    const bool unified = op.unifiedActive && !op.unifiedFailed;
    s.v[0] = civicOn && civic.health;
    s.v[1] = civicOn && civic.education;
    s.v[2] = unified && op.ready[0] && !op.dealsDisabled && util.tradePowerActive();       // tradeXActive() = AutoTradeX && AutoDeals
    s.v[3] = unified && op.ready[1] && !op.dealsDisabled && util.tradeWaterActive();
    s.v[4] = unified && op.ready[2] && !op.dealsDisabled && util.tradeGarbageActive();
    s.v[5] = unified && op.ready[2] && !op.garbageDisabled && util.autoGarbage;
    return s;
}

// What the plugin has written into the advisor system of the CURRENT city. The six values are written as a set when any differs from what the city already holds, when the caller forces
// it (the city has just been initialised: its Lua state holds nothing), or when an earlier attempt failed. `set(name, value)` returns whether the game accepted the value.
class AdviceSwitchPublisher {
public:
    using SetFn = std::function<bool(const char*, double)>;

    // true = the advisor holds `want` after the call
    bool Publish(const AdviceSwitches& want, bool force, const SetFn& set) {
        if (have_ && !force && last_ == want) return true;
        bool all = true;
        for (int i = 0; i < kAdviceSwitchCount; ++i) all = set(kAdviceSwitchNames[static_cast<std::size_t>(i)], want.v[i] ? 1.0 : 0.0) && all;
        have_ = all;
        if (have_) last_ = want;
        return have_;
    }
    void ResetCity() { have_ = false; last_ = AdviceSwitches{}; }       // a new city (or none): nothing is held any more
    bool published() const { return have_; }
    AdviceSwitches last() const { return last_; }

private:
    bool have_ = false;
    AdviceSwitches last_;
};

inline std::string AdviceSwitchesText(const AdviceSwitches& s) {
    std::string out;
    for (int i = 0; i < kAdviceSwitchCount; ++i) out += std::string(i ? " " : "") + kAdviceSwitchNames[static_cast<std::size_t>(i)] + "=" + (s.v[i] ? "1" : "0");
    return out;
}

}  // namespace sc4af
