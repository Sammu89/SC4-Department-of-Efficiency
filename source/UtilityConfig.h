// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstdlib>
#include <set>
#include <string>
#include <vector>

namespace sc4af {

enum class UtilityMode { Off, Shadow, Auto };

inline const char* UtilityModeName(UtilityMode m) { return m == UtilityMode::Auto ? "Auto" : m == UtilityMode::Shadow ? "Shadow" : "Off"; }

// ---- configurable reserves ----------------------------------------------------------------
// A reserve is the headroom kept for the own city ABOVE its demand / load. It is a FLOOR, never a ceiling, and it can only
// be set inside a safe range; a value outside the range is a configuration error (the strict parser rejects the file, nothing is clamped).
//   user-facing option          INI percent key            INI factor key (compatible)   default   range
//   Power reserve               PowerReservePercent        PowerReserve                  20 %      10 .. 50 %   (factor 1.10 .. 1.50)
//   Water reserve               WaterReservePercent        WaterReserve                  10 %      10 .. 50 %   (factor 1.10 .. 1.50)
//   Garbage reserve             GarbageReservePercent      GarbageReserve                 2 %       2 .. 20 %   (factor 1.02 .. 1.20)
// factor = 1 + percent / 100. Both spellings of the same reserve in one file is a conflict (rejected).
constexpr double kPowerReserveMin = 1.10, kPowerReserveMax = 1.50, kPowerReserveDefault = 1.20;
constexpr double kWaterReserveMin = 1.10, kWaterReserveMax = 1.50, kWaterReserveDefault = 1.10;
constexpr double kGarbageReserveMin = 1.02, kGarbageReserveMax = 1.20, kGarbageReserveDefault = 1.02;

inline double ReserveFactorFromPercent(double percent) { return 1.0 + percent / 100.0; }
inline double ReservePercentFromFactor(double factor) { return (factor - 1.0) * 100.0; }
inline bool ReserveFactorInRange(double factor, double lo, double hi) { return factor >= lo - 1e-9 && factor <= hi + 1e-9; }

// Landfill savings alert threshold: INI LandfillSavingsAlertThreshold, whole percent, 20..50, default 25. Outside the range (or not a whole number) the file is rejected,
// like every other bad value. The search (LandfillSavings.h) clamps it again before use.
constexpr int kLandfillAlertThresholdMin = 20, kLandfillAlertThresholdMax = 50, kLandfillAlertThresholdDefault = 25;

// DepartmentEfficiencyUtility.ini beside the DLL. `Mode=Shadow` logs the optimizer plan and performs ZERO writes. `Mode=Auto` executes the plan only
// for the subsystems opted in with AutoPower / AutoWater / AutoGarbage / AutoDeals, and only while the unified controller is enabled
// (EnableUnifiedAuto=true; EnableUnifiedAutoValidation additionally needs a build with the compile-time capability SC4_FINAL_AUTO_VALIDATION).
//
// Policy (not SimCity rules): aggressive micromanagement. Reserves are FLOORS, never ceilings; funding jumps straight
// to the optimum; there is no step limit and no post-write rollback watch.
//
// The defaults below (reserves, ConsiderPowerAging, ProtectWaterPipes, AllowUtilityArbitrage) are the product defaults;
// they never authorise a writer: Mode stays Off and every Auto* key stays false until the file says otherwise.
// Option name  ->  INI key:  ManagePower=AutoPower, ManageWater=AutoWater, ManageGarbage=AutoGarbage, ManageNeighborDeals=AutoDeals.
struct UtilityResearchConfig {
    bool enabled = true;          // read-only observation; safe default
    bool detail = false;          // per-building rows on every month (otherwise: initial scan and changes only)
    bool debug = false;           // deal cards also print the optimizer internals
    UtilityMode mode = UtilityMode::Off;
    double powerReserve = kPowerReserveDefault;     // own-city target >= max(GetMonthlyRequirement, powerReserve x GetMonthlyDemand)
    double waterReserve = kWaterReserveDefault;     // own-city water capacity >= waterReserve x demand
    double garbageReserve = kGarbageReserveDefault; // protected own garbage = ceil(garbageReserve x (Produced - Recycled)); backlog on top
    // Deprecated and IGNORED: the landfill reserve in months is not part of any decision (physical free storage and the vanilla import
    // limit are the only storage constraints). The key is still parsed so existing files stay valid.
    int deprecatedGarbageLandfillReserveMonths = 120;
    bool considerPowerAging = false;    // ConsiderPowerAging: ON prices plant wear (replacement cost x aging) into the Power optimum; OFF (the default) = cash only
    bool protectWaterPipes = true;      // ProtectWaterPipes: never let the pipe curve decay or cut while pipes are damaged
    bool allowUtilityArbitrage = false; // AllowUtilityArbitrage: OFF by default (BUY xor SELL per commodity). ON: a commodity may BUY from some neighbors and SELL to others in the same
                                        // plan when that portfolio pays more (only for profit; the own city's service and reserve still come first). Active only with AutoDeals.
    // Full contract management per resource (default ON each, independent of each other). ON = the Department may create, adjust and cancel the contracts of that
    // resource in BOTH directions (buy and sell). OFF = it never touches the contracts of that resource (existing ones stay under manual control). They act only with AutoDeals and are
    // INDEPENDENT of the funding managers (AutoPower / AutoWater / AutoGarbage). The value is stored while AutoDeals is off.
    // Obsolete (parsed, IGNORED, flagged in obsoleteKeys): AutoImportPower, AutoImportWater, AutoExportGarbage.
    bool tradePower = true;       // AutoTradePower
    bool tradeWater = true;       // AutoTradeWater
    bool tradeGarbage = true;     // AutoTradeGarbage
    // Landfill size savings advice (default OFF): the Department only TELLS the player, with one informative notice, when changing the landfill size (adding tiles / removing empty
    // tiles) would lower the monthly sanitation bill. It never builds or removes anything and never changes the sanitation funding or the regional contracts. Computed inside the sanitation funding evaluation, so it needs the sanitation manager
    // (AutoGarbage) on; the other managers and the contract switches are independent of it.
    bool warnLandfillSizeSavings = false;   // WarnLandfillSizeSavings
    // LandfillSavingsAlertThreshold: the advice is given only when the best change saves STRICTLY more than this percentage of the monthly sanitation expense
    // (saving x 100 > threshold x expense, integers). Whole percent, kLandfillAlertThresholdMin..Max; a file without the key keeps the default. Stored while the advice is off.
    int landfillSavingsAlertThreshold = kLandfillAlertThresholdDefault;
    std::vector<std::string> obsoleteKeys;   // obsolete keys present in the file, in file order (value ignored)
    bool countImports = true;     // plainly active, non-suspended imports lower the own requirement
    // Legacy compatibility only: parsed but ignored. Vanilla lifecycle, not a guessed time window, governs availability.
    int importSafeDays = 10;
    int minContractBenefit = 0;    // MinContractBenefit: a FACULTATIVE contract must improve the net by at least this many $ per month. 0 (default) = any strictly positive net benefit justifies creating, keeping, changing or cancelling it (the game charges no fee for these operations); never applied to contracts the service needs
    int waterImportSafeDays = 31;
    int healthHorizonMonths = 120; // legacy compatibility/diagnostics only; NO EFFECT on Power funding eligibility
    int waterMinPercent = 50;     // Derived from the executable: the pipe-damage curve 0xCA7138C4 is stable at exactly 50% (below it the network decays). NOT a strike threshold: the exe has none for water
    bool power = true;            // Shadow scope per subsystem
    bool water = true;
    bool autoPower = false;       // Mode=Auto executes power plant funding changes only when this is true
    bool autoWater = false;       // Mode=Auto executes the global water funding change only when this is true
    bool exportReserve = true;    // keep capacity for economically worthwhile (visible or latent) power sell opportunities
    bool useRequirement = true;   // own power target never below GetMonthlyRequirement
    int powerCadenceDays = 10;    // power is re-evaluated every N simulation days (SimNewDay)
    int waterCadenceDays = 10;    // WaterCadenceDays: the full periodic review of the water, every N simulation days since its last evaluation; independent of the power cadence
    // Deprecated and IGNORED: a building can lack power or water only because the player did not connect it, so unserved buildings never suspend
    // Power / Water. The key is still parsed so existing files stay valid (the strict parser would otherwise turn the plugin Off).
    int deprecatedShortageCycles = 3;
    bool dailyTrace = false;      // research: one compact [PowerTrace] line per SimNewDay (read-only)
    bool latentProbe = false;     // research: ask the game for GetDealMaxAmount/GetDealCost per power connection even without a visible opportunity
    bool phase8Runtime = false;   // research: passive runtime recorder of deal-simulator messages; never writes
    bool landfillResearch = false; // LandfillResearch: passive landfill-rollover observer (only effective in a build with SC4_LANDFILL_RESEARCH); forces Mode=Off, never writes
    // ---- unified controller gate (runtime half; the compile-time half is UnifiedAutoCapability.h) ----
    bool unifiedAuto = false;             // EnableUnifiedAuto: the PRODUCT gate - the unified Power+Water+Sanitation+Deals controller may run (Mode=Auto and the per-manager opt-ins still apply)
    bool detailedLog = false;             // DetailedDecisionLog: the full per-plan decision dump (optimizer candidates, projections); off in the product to keep the log small
    bool unifiedAutoValidation = false;   // EnableUnifiedAutoValidation: the unified Power+Water+Sanitation+Deals controller may run
    bool autoGarbage = false;             // AutoGarbage: unified controller may write Sanitation funding
    bool autoDeals = false;               // AutoDeals: unified controller may create / change / cancel Neighbor Deals

    // Dependent options are stored but INACTIVE while their manager is off (the Shadow scope `power` / `water` is the planning half of
    // ManagePower / ManageWater).
    bool powerAgingActive() const { return considerPowerAging && power; }
    bool waterPipeProtectionActive() const { return protectWaterPipes && water; }
    bool utilityArbitrageActive() const { return allowUtilityArbitrage && autoDeals; }   // the plugin must manage the deals itself (ManageNeighborDeals)
    bool tradePowerActive() const { return tradePower && autoDeals; }
    bool tradeWaterActive() const { return tradeWater && autoDeals; }
    bool tradeGarbageActive() const { return tradeGarbage && autoDeals; }
    bool tradeActive(int ci) const { return ci == 0 ? tradePowerActive() : ci == 1 ? tradeWaterActive() : ci == 2 ? tradeGarbageActive() : false; }   // 0 power, 1 water, 2 garbage
    double powerReservePercent() const { return ReservePercentFromFactor(powerReserve); }
    double waterReservePercent() const { return ReservePercentFromFactor(waterReserve); }
    double garbageReservePercent() const { return ReservePercentFromFactor(garbageReserve); }
};

// "PowerReserve=20% WaterReserve=10% GarbageReserve=2% ConsiderPowerAging=true ProtectWaterPipes=true AllowUtilityArbitrage=false"
inline std::string PolicyText(const UtilityResearchConfig& c) {
    const auto pct = [](double factor) {
        std::string t = std::to_string(ReservePercentFromFactor(factor) + 1e-9);
        t.erase(t.find_last_not_of('0') + 1);
        if (!t.empty() && t.back() == '.') t.pop_back();
        return t + "%";
    };
    return "PowerReserve=" + pct(c.powerReserve) + " WaterReserve=" + pct(c.waterReserve) + " GarbageReserve=" + pct(c.garbageReserve) +
           " ConsiderPowerAging=" + (c.powerAgingActive() ? "true" : "false") + " ProtectWaterPipes=" + (c.waterPipeProtectionActive() ? "true" : "false") +
           " AllowUtilityArbitrage=" + (c.utilityArbitrageActive() ? "true(buy and sell together when it pays)" : c.allowUtilityArbitrage ? "true(INACTIVE: AutoDeals is off)" : "false") +
           " TradePower=" + (c.tradePowerActive() ? "true" : c.tradePower ? "true(inactive: AutoDeals is off)" : "false") +
           " TradeWater=" + (c.tradeWaterActive() ? "true" : c.tradeWater ? "true(inactive: AutoDeals is off)" : "false") +
           " TradeGarbage=" + (c.tradeGarbageActive() ? "true" : c.tradeGarbage ? "true(inactive: AutoDeals is off)" : "false") +
           " WarnLandfillSizeSavings=" + (c.warnLandfillSizeSavings ? "true" : "false") + " LandfillSavingsAlertThreshold=" + std::to_string(c.landfillSavingsAlertThreshold) + "%";
}

// The unified controller may run only when these runtime conditions hold. Missing, invalid or Phase8Runtime configurations never open it.
inline bool UnifiedGateOpenRuntime(const UtilityResearchConfig& c) {
    return (c.unifiedAuto || c.unifiedAutoValidation) && c.mode == UtilityMode::Auto && c.enabled && !c.phase8Runtime && !c.landfillResearch;
}

// Discovery of NEW neighbor-deal proposals = ProposeDeals(list, false) (it refreshes the commodity cache and makes the game post a message while the
// simulator is already dispatching one) plus the per-neighbor market read that relies on that refresh. Reading the EXISTING contracts (GetLocalDeals)
// never needs it. The Department discovers proposals only when the deal controller may act on them (AutoDeals). Every product path that would ask
// for proposals goes through this one predicate.
// EXPLICIT research exceptions, off by default and off in the product sample: Phase8Runtime and LandfillResearch (passive recorders of the deal
// simulator), LatentProbe (its whole purpose is to quote the connections the game is not proposing). Logging options (DailyTrace, Detail, Debug,
// DetailedDecisionLog) NEVER open it: a trace prints what was read and says NOT_READ for the rest.
inline bool DealDiscoveryAllowed(const UtilityResearchConfig& c) {
    return c.autoDeals || c.phase8Runtime || c.landfillResearch || c.latentProbe;
}

// Strict parser: unknown key / duplicate / bad value => defaults (read-only, quiet, Mode=Off).
inline bool ParseUtilityResearchConfig(const std::string& text, UtilityResearchConfig& config, std::string& error) {
    UtilityResearchConfig parsed;
    std::set<std::string> seen;
    std::size_t pos = 0;
    const auto trim = [](std::string s) {
        const auto b = s.find_first_not_of(" \t\r");
        if (b == std::string::npos) return std::string();
        return s.substr(b, s.find_last_not_of(" \t\r") - b + 1);
    };
    const auto toInt = [](const std::string& v, int lo, int hi, int& dst) {
        if (v.empty()) return false;
        char* end = nullptr;
        const long n = std::strtol(v.c_str(), &end, 10);
        if (*end != '\0' || n < lo || n > hi) return false;
        dst = static_cast<int>(n);
        return true;
    };
    const auto toBool = [](const std::string& v, bool& dst) {
        if (v == "true") dst = true; else if (v == "false") dst = false; else return false;
        return true;
    };
    const auto toNumber = [](const std::string& v, double& dst) {
        char* end = nullptr;
        if (v.empty()) return false;
        const double d = std::strtod(v.c_str(), &end);
        if (*end != '\0' || !(d == d)) return false;
        dst = d;
        return true;
    };
    // a reserve given as a FACTOR (compatible key) or as a PERCENT (product key); the range is checked on the factor
    const auto toFactor = [&](const std::string& v, double lo, double hi, double& dst) {
        double d = 0.0;
        if (!toNumber(v, d) || !ReserveFactorInRange(d, lo, hi)) return false;
        dst = d;
        return true;
    };
    const auto toPercent = [&](const std::string& v, double lo, double hi, double& dst) {
        double d = 0.0;
        if (!toNumber(v, d)) return false;
        const double factor = ReserveFactorFromPercent(d);
        if (!ReserveFactorInRange(factor, lo, hi)) return false;
        dst = factor;
        return true;
    };
    while (pos <= text.size()) {
        auto eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size();
        const std::string line = trim(text.substr(pos, eol - pos));
        pos = eol + 1;
        if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') continue;
        const auto eq = line.find('=');
        if (eq == std::string::npos) { error = "malformed line: " + line; config = UtilityResearchConfig{}; return false; }
        const std::string key = trim(line.substr(0, eq)), value = trim(line.substr(eq + 1));
        bool ok = true, known = true;
        if (key == "Enabled") ok = toBool(value, parsed.enabled);
        else if (key == "Detail") ok = toBool(value, parsed.detail);
        else if (key == "Debug") ok = toBool(value, parsed.debug);
        else if (key == "Mode") {
            if (value == "Off") parsed.mode = UtilityMode::Off;
            else if (value == "Shadow") parsed.mode = UtilityMode::Shadow;
            else if (value == "Auto") parsed.mode = UtilityMode::Auto;
            else ok = false;
        }
        else if (key == "PowerReserve") ok = toFactor(value, kPowerReserveMin, kPowerReserveMax, parsed.powerReserve);
        else if (key == "WaterReserve") ok = toFactor(value, kWaterReserveMin, kWaterReserveMax, parsed.waterReserve);
        else if (key == "GarbageReserve") ok = toFactor(value, kGarbageReserveMin, kGarbageReserveMax, parsed.garbageReserve);
        else if (key == "PowerReservePercent") ok = toPercent(value, kPowerReserveMin, kPowerReserveMax, parsed.powerReserve);
        else if (key == "WaterReservePercent") ok = toPercent(value, kWaterReserveMin, kWaterReserveMax, parsed.waterReserve);
        else if (key == "GarbageReservePercent") ok = toPercent(value, kGarbageReserveMin, kGarbageReserveMax, parsed.garbageReserve);
        else if (key == "GarbageLandfillReserveMonths") ok = toInt(value, 1, 1200, parsed.deprecatedGarbageLandfillReserveMonths);   // deprecated, ignored
        else if (key == "ConsiderPowerAging") ok = toBool(value, parsed.considerPowerAging);
        else if (key == "ProtectWaterPipes") ok = toBool(value, parsed.protectWaterPipes);
        else if (key == "AllowUtilityArbitrage") ok = toBool(value, parsed.allowUtilityArbitrage);
        else if (key == "AutoTradePower") ok = toBool(value, parsed.tradePower);
        else if (key == "AutoTradeWater") ok = toBool(value, parsed.tradeWater);
        else if (key == "AutoTradeGarbage") ok = toBool(value, parsed.tradeGarbage);
        else if (key == "WarnLandfillSizeSavings") ok = toBool(value, parsed.warnLandfillSizeSavings);
        else if (key == "LandfillSavingsAlertThreshold") ok = toInt(value, kLandfillAlertThresholdMin, kLandfillAlertThresholdMax, parsed.landfillSavingsAlertThreshold);
        else if (key == "AutoImportPower" || key == "AutoImportWater" || key == "AutoExportGarbage") {   // obsolete: accepted, value ignored (never reinterpreted as AutoTrade*)
            if (value != "true" && value != "false") ok = false;
            else parsed.obsoleteKeys.push_back(key);
        }
        else if (key == "CountImports") ok = toBool(value, parsed.countImports);
        else if (key == "ImportSafeDays") ok = toInt(value, 0, 730, parsed.importSafeDays);
        else if (key == "MinContractBenefit") ok = toInt(value, 0, 100000, parsed.minContractBenefit);
        else if (key == "WaterImportSafeDays") ok = toInt(value, 0, 730, parsed.waterImportSafeDays);
        else if (key == "HealthHorizonMonths") ok = toInt(value, 1, 2400, parsed.healthHorizonMonths);
        else if (key == "WaterMinPercent") ok = toInt(value, 0, 120, parsed.waterMinPercent);
        else if (key == "AutoPower") ok = toBool(value, parsed.autoPower);
        else if (key == "AutoWater") ok = toBool(value, parsed.autoWater);
        else if (key == "AutoGarbage") ok = toBool(value, parsed.autoGarbage);
        else if (key == "AutoDeals") ok = toBool(value, parsed.autoDeals);
        else if (key == "EnableUnifiedAutoValidation") ok = toBool(value, parsed.unifiedAutoValidation);
        else if (key == "EnableUnifiedAuto") ok = toBool(value, parsed.unifiedAuto);
        else if (key == "DetailedDecisionLog") ok = toBool(value, parsed.detailedLog);
        else if (key == "ExportReserve") ok = toBool(value, parsed.exportReserve);
        else if (key == "UseRequirement") ok = toBool(value, parsed.useRequirement);
        else if (key == "PowerCadenceDays") ok = toInt(value, 1, 30, parsed.powerCadenceDays);
        else if (key == "WaterCadenceDays") ok = toInt(value, 1, 30, parsed.waterCadenceDays);
        else if (key == "ShortageCycles") ok = toInt(value, 1, 24, parsed.deprecatedShortageCycles);   // legacy: no effect
        else if (key == "DailyTrace") ok = toBool(value, parsed.dailyTrace);
        else if (key == "LatentProbe") ok = toBool(value, parsed.latentProbe);
        else if (key == "Phase8Runtime") ok = toBool(value, parsed.phase8Runtime);
        else if (key == "LandfillResearch") ok = toBool(value, parsed.landfillResearch);
        else if (key == "Power") ok = toBool(value, parsed.power);
        else if (key == "Water") ok = toBool(value, parsed.water);
        else known = false;
        if (!known) { error = "unknown key: " + key; config = UtilityResearchConfig{}; return false; }
        if (!ok) { error = "bad value for " + key; config = UtilityResearchConfig{}; return false; }
        if (!seen.insert(key).second) { error = "duplicate key: " + key; config = UtilityResearchConfig{}; return false; }
        for (const char* base : {"PowerReserve", "WaterReserve", "GarbageReserve"}) {      // factor and percent spellings of one reserve conflict
            const std::string b = base, p = b + "Percent";
            if ((key == b && seen.count(p)) || (key == p && seen.count(b))) { error = "conflicting keys: " + b + " and " + p; config = UtilityResearchConfig{}; return false; }
        }
    }
    if (parsed.phase8Runtime) { parsed.unifiedAuto = false; parsed.unifiedAutoValidation = false; parsed.autoGarbage = false; parsed.autoDeals = false; }   // the passive recorder never writes
    if (parsed.landfillResearch) {                                                                                                 // the research observer never writes either
        parsed.mode = UtilityMode::Off; parsed.unifiedAuto = false; parsed.unifiedAutoValidation = false; parsed.autoPower = false; parsed.autoWater = false; parsed.autoGarbage = false; parsed.autoDeals = false;
    }
    config = parsed;
    error.clear();
    return true;
}

} // namespace sc4af
