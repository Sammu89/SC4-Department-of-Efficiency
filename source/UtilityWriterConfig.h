// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstdlib>
#include <string>

namespace sc4af {

// Configuration of the one-shot utility funding writer (DepartmentEfficiencyUtilityWriterTest.ini beside the DLL). Disabled by default.
// This is not automatic control: it executes ONE explicitly requested funding change.
struct UtilityWriterConfig {
    enum class Target { None, Power, Water, Both };
    bool enabled = false;
    std::string nonce;             // each nonce is applied at most once (persisted next to the DLL)
    Target target = Target::None;
    int cellX = 0, cellZ = 0;      // power plant address
    int expectedPercent = 100;     // compare-and-set precondition
    int targetPercent = 95;
    int waterExpectedPercent = 100;  // Target=Both: the water leg (Power uses ExpectedPercent/TargetPercent)
    int waterTargetPercent = 95;
    int minMonths = 2;             // simulation months observed in this city before writing
};

inline bool ParseUtilityWriterConfig(const std::string& text, UtilityWriterConfig& out, std::string& error) {
    UtilityWriterConfig parsed;
    unsigned seen = 0;
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
    while (pos <= text.size()) {
        auto eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size();
        const std::string line = trim(text.substr(pos, eol - pos));
        pos = eol + 1;
        if (line.empty() || line[0] == ';' || line[0] == '#' || line[0] == '[') continue;
        const auto eq = line.find('=');
        if (eq == std::string::npos) { error = "malformed line: " + line; out = UtilityWriterConfig{}; return false; }
        const std::string key = trim(line.substr(0, eq)), value = trim(line.substr(eq + 1));
        unsigned bit = 0;
        bool ok = true;
        if (key == "Enabled") { bit = 1; if (value == "true") parsed.enabled = true; else if (value == "false") parsed.enabled = false; else ok = false; }
        else if (key == "Nonce") { bit = 2; parsed.nonce = value; ok = !value.empty(); }
        else if (key == "Target") {
            bit = 4;
            if (value == "Power") parsed.target = UtilityWriterConfig::Target::Power;
            else if (value == "Water") parsed.target = UtilityWriterConfig::Target::Water;
            else if (value == "Both") parsed.target = UtilityWriterConfig::Target::Both;
            else ok = false;
        }
        else if (key == "CellX") { bit = 8; ok = toInt(value, -1000, 100000, parsed.cellX); }
        else if (key == "CellZ") { bit = 16; ok = toInt(value, -1000, 100000, parsed.cellZ); }
        else if (key == "ExpectedPercent") { bit = 32; ok = toInt(value, 0, 120, parsed.expectedPercent); }
        else if (key == "TargetPercent") { bit = 64; ok = toInt(value, 0, 120, parsed.targetPercent); }
        else if (key == "WaterExpectedPercent") { bit = 256; ok = toInt(value, 0, 120, parsed.waterExpectedPercent); }
        else if (key == "WaterTargetPercent") { bit = 512; ok = toInt(value, 0, 120, parsed.waterTargetPercent); }
        else if (key == "MinMonths") { bit = 128; ok = toInt(value, 0, 240, parsed.minMonths); }
        else { error = "unknown key: " + key; out = UtilityWriterConfig{}; return false; }
        if (!ok) { error = "bad value for " + key; out = UtilityWriterConfig{}; return false; }
        if (seen & bit) { error = "duplicate key: " + key; out = UtilityWriterConfig{}; return false; }
        seen |= bit;
    }
    if (parsed.enabled && (parsed.nonce.empty() || parsed.target == UtilityWriterConfig::Target::None)) {
        error = "Enabled=true needs Nonce and Target";
        out = UtilityWriterConfig{};
        return false;
    }
    out = parsed;
    error.clear();
    return true;
}

} // namespace sc4af
