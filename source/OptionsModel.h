// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// The plugin's options window (OptionsUI.h) without the game: which options it shows, on which page, how they depend on each other, the pending choices of the
// player (nothing is applied before Accept), and the edit of the three ini files that Accept makes. Pure.
//
// Accept rewrites ONLY the keys of the options the player changed (plus the Mode keys that the master switch decides), keeps every other line (comments,
// research keys, order, line ends), and the result is parsed back with the plugin's own strict parsers: if it does not give exactly the chosen options, nothing
// is written (a wrong ini would turn the plugin Off).
//
// The master switch ("Department active"): off = Mode=Off in the three sections of DepartmentEfficiency.ini ([AutoFinance], [UtilityResearch], [LotDemolition]); every other
// choice is kept, so switching it on again gives back exactly the previous configuration.
#pragma once

#include "CivicPolicy.h"
#include "IniSections.h"
#include "LotWatch.h"
#include "UtilityConfig.h"

#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <string>
#include <utility>
#include <vector>

namespace sc4af {

// ---- window and script ids, shared with the UI resources of DepartmentEfficiency.dat (the two must agree) ---------------------------------------------
constexpr std::uint32_t kOptionsUiGroup = 0x5C4A6000;
constexpr std::uint32_t kOptionsUiButtonScript = 0x5C4A6001, kOptionsUiWindowScript = 0x5C4A6002;
constexpr std::uint32_t kOptionsUiButtonRoot = 0x5C4A6010, kOptionsUiButton = 0x5C4A6011;
constexpr std::uint32_t kOptionsUiWindowRoot = 0x5C4A6020, kOptionsUiTitle = 0x5C4A6021, kOptionsUiAccept = 0x5C4A6023, kOptionsUiCancel = 0x5C4A6024;
constexpr std::uint32_t kOptionsUiTab0 = 0x5C4A6027, kOptionsUiPage0 = 0x5C4A602A, kOptionsUiTabMark0 = 0x5C4A602C;   // + page index: page buttons, backgrounds, the bar under the page shown
// + option index:
constexpr std::uint32_t kOptionsUiLabel0 = 0x5C4A6200, kOptionsUiCheck0 = 0x5C4A6300, kOptionsUiSpin0 = 0x5C4A6400, kOptionsUiValue0 = 0x5C4A6500;
constexpr std::uint32_t kOptionsUiTip0 = 0x5C4A6600;    // a transparent "label" button over the label that shows the tooltip (the game's own pattern)
// The game's Play Options window (type 0, group 0x96A006B0, instance 0x0A7DF315 in SimCity_1.dat).
constexpr std::uint32_t kPlayOptionsPanel = 0x2A57DB84;        // the visible panel (403 x 286)
constexpr std::uint32_t kPlayOptionsCredits = 0x0A57DA65;      // its last button, inside the content background (which has no id: it is found as the parent of Credits)
constexpr std::uint32_t kPlayOptionsCancel = 0x6A57DA48;       // its Cancel button (our button closes Play Options with it before our window opens)
constexpr std::uint32_t kPlayOptionsFooter = 0xEA7E0475;       // its Accept / Cancel bar (a direct child of the panel; a separator line inside the content has the same id)
constexpr int kOptionsUiGrow = 36;                              // content, panel and footer move by one button height; the button goes 6 px under "Credits"

// ---- texts (LTEXT, group kOptionsUiTextGroup + the game's language code; the same ids carry every translation) -------------------------------------------
// Accept, Cancel and the "On" column header are the game's own texts (the scripts keep the game's captionres).
constexpr std::uint32_t kOptionsUiTextGroup = 0x5C4A6100;
enum OptionsText : std::uint32_t {
    kTextName = 0x01,
    kTextPercent = 0x03,          // "{0}%"
    kTextOneMonth = 0x04,         // "1 month"
    kTextMonths = 0x05,           // "{0} months"
    kTextPageServices = 0x06,
    kTextPageDemolition = 0x07,
    kTextMoney = 0x08,            // "§{0}"
    kTextThousands = 0x09,        // the thousands separator of the language
    kTextOrdinanceDescription = 0x0A,   // the Department ordinance's description in the Ordinances window
    kTextOrdinanceName = 0x0B,          // its name: the act that creates the Department ("Establish a Department of Efficiency")
    kTextLabel0 = 0x100,          // + option index: the option's label
    kTextTip0 = 0x200,            // + option index: its tooltip ("title|text" in the game: the title is the label)
};

// ---- the options --------------------------------------------------------------------------------------------------------------------------------------------
enum OptionIndex : int {
    // page 0: services
    kOptMaster, kOptEducation, kOptEducationReserve, kOptHealth, kOptHealthReserve,
    kOptPower, kOptPowerReserve, kOptPowerAging, kOptWater, kOptWaterReserve, kOptWaterPipes, kOptSanitation, kOptSanitationReserve, kOptDeals, kOptArbitrage,
    // page 1: demolition
    kOptDemolishDestroyed, kOptDestroyedMonths, kOptDemolishMunicipal,   // kOptDemolishMunicipal: "Demolish municipal and historic buildings" (their ruins), off by default
    kOptDemolishAbandoned, kOptAbandonedMonths,
    kOptZoneResidential, kOptZoneCommercial, kOptZoneIndustrial, kOptZoneAgriculture,
    kOptReasonLowDesirability, kOptReasonCommute, kOptReasonLowDemand, kOptReasonNoPower, kOptReasonNoWater,   // KickOut and Unknown always count (lotwatch::kAlwaysReasons)
    kOptBudgetLimit, kOptDemolitionBudget,    // added at the end so the text and control ids of the other options stay stable
    // page 0 again: full contract management per resource, under "Manage neighbor deals"; independent of the funding managers
    kOptTradePower, kOptTradeWater, kOptTradeGarbage,
    // page 0 again: "Warn me when checking landfill size can help me save" (INI WarnLandfillSizeSavings, default off). Sits in the sanitation area (row 13), depends only on the master
    // switch (not on the funding managers); the Department only tells, it never builds or removes landfill tiles
    kOptLandfillAdvice,
    // page 0 again: "Landfill savings alert threshold" (INI LandfillSavingsAlertThreshold, 20..50, default 25), the row right under the landfill advice; its spinner is
    // disabled (value kept) while the advice is off
    kOptLandfillThreshold,
    kOptionCount
};
constexpr int kOptionsUiPages = 2;
constexpr int kOptionsUiRows = 20;   // the most rows of one page (the services page uses 20, the demolition page 16)

enum class OptionKind { Check, Percent, Months, Money };
struct OptionSpec {
    OptionIndex index;
    OptionKind kind;
    int parent;          // the option this one depends on (its controls are disabled while that one is off), or -1
    int min, max;        // Percent / Months only
    int page, row, indent;
    int step = 1;        // one click of the spinner
    int parent2 = -1;    // a second option this one depends on (its controls are disabled while that one, or anything above it, is off), or -1
};

inline int ReservePercentInt(double factor) { return static_cast<int>(std::lround(ReservePercentFromFactor(factor))); }

inline const std::array<OptionSpec, kOptionCount>& OptionSpecs() {
    using K = OptionKind;
    static const std::array<OptionSpec, kOptionCount> specs = {{
        {kOptMaster, K::Check, -1, 0, 1, 0, 0, 0},
        {kOptEducation, K::Check, kOptMaster, 0, 1, 0, 1, 1},
        {kOptEducationReserve, K::Percent, kOptEducation, 0, 50, 0, 2, 2},
        {kOptHealth, K::Check, kOptMaster, 0, 1, 0, 3, 1},
        {kOptHealthReserve, K::Percent, kOptHealth, 0, 50, 0, 4, 2},
        {kOptPower, K::Check, kOptMaster, 0, 1, 0, 5, 1},
        {kOptPowerReserve, K::Percent, kOptPower, ReservePercentInt(kPowerReserveMin), ReservePercentInt(kPowerReserveMax), 0, 6, 2},
        {kOptPowerAging, K::Check, kOptPower, 0, 1, 0, 7, 2},
        {kOptWater, K::Check, kOptMaster, 0, 1, 0, 8, 1},
        {kOptWaterReserve, K::Percent, kOptWater, ReservePercentInt(kWaterReserveMin), ReservePercentInt(kWaterReserveMax), 0, 9, 2},
        {kOptWaterPipes, K::Check, kOptWater, 0, 1, 0, 10, 2},
        {kOptSanitation, K::Check, kOptMaster, 0, 1, 0, 11, 1},
        {kOptSanitationReserve, K::Percent, kOptSanitation, ReservePercentInt(kGarbageReserveMin), ReservePercentInt(kGarbageReserveMax), 0, 12, 2},
        {kOptDeals, K::Check, kOptMaster, 0, 1, 0, 15, 1},
        {kOptArbitrage, K::Check, kOptDeals, 0, 1, 0, 16, 2},
        // demolition page: "Demolish destroyed buildings" first with its wait and the destroyed municipal / historic buildings, then the abandoned ones, then the budget
        {kOptDemolishDestroyed, K::Check, kOptMaster, 0, 1, 1, 0, 0},
        {kOptDestroyedMonths, K::Months, kOptDemolishDestroyed, 1, 24, 1, 1, 1},
        {kOptDemolishMunicipal, K::Check, kOptDemolishDestroyed, 0, 1, 1, 2, 1},
        {kOptDemolishAbandoned, K::Check, kOptMaster, 0, 1, 1, 3, 0},
        {kOptAbandonedMonths, K::Months, kOptDemolishAbandoned, 1, 60, 1, 4, 1},
        {kOptZoneResidential, K::Check, kOptDemolishAbandoned, 0, 1, 1, 5, 1},
        {kOptZoneCommercial, K::Check, kOptDemolishAbandoned, 0, 1, 1, 6, 1},
        {kOptZoneIndustrial, K::Check, kOptDemolishAbandoned, 0, 1, 1, 7, 1},
        {kOptZoneAgriculture, K::Check, kOptDemolishAbandoned, 0, 1, 1, 8, 1},
        {kOptReasonLowDesirability, K::Check, kOptDemolishAbandoned, 0, 1, 1, 9, 1},
        {kOptReasonCommute, K::Check, kOptDemolishAbandoned, 0, 1, 1, 10, 1},
        {kOptReasonLowDemand, K::Check, kOptDemolishAbandoned, 0, 1, 1, 11, 1},
        {kOptReasonNoPower, K::Check, kOptDemolishAbandoned, 0, 1, 1, 12, 1},
        {kOptReasonNoWater, K::Check, kOptDemolishAbandoned, 0, 1, 1, 13, 1},
        {kOptBudgetLimit, K::Check, kOptMaster, 0, 1, 1, 14, 0},          // the last rows: one limit for the destroyed AND the abandoned buildings
        {kOptDemolitionBudget, K::Money, kOptBudgetLimit, 0, 200000, 1, 15, 1, 1000},
        {kOptTradePower, K::Check, kOptDeals, 0, 1, 0, 17, 2},
        {kOptTradeWater, K::Check, kOptDeals, 0, 1, 0, 18, 2},
        {kOptTradeGarbage, K::Check, kOptDeals, 0, 1, 0, 19, 2},
        {kOptLandfillAdvice, K::Check, kOptSanitation, 0, 1, 0, 13, 1},
        {kOptLandfillThreshold, K::Percent, kOptLandfillAdvice, kLandfillAlertThresholdMin, kLandfillAlertThresholdMax, 0, 14, 2},
    }};
    return specs;
}

// The abandoned-lot reasons and zone types of the window, in LotWatch's terms.
inline const std::array<std::pair<int, int>, 5>& ReasonOptions() {
    static const std::array<std::pair<int, int>, 5> r = {{{kOptReasonLowDesirability, lotwatch::kLowDesirability}, {kOptReasonCommute, lotwatch::kCommuteTime},
                                                          {kOptReasonLowDemand, lotwatch::kLowDemand}, {kOptReasonNoPower, lotwatch::kNoPower}, {kOptReasonNoWater, lotwatch::kNoWater}}};
    return r;
}
inline const std::array<std::pair<int, unsigned>, 4>& ZoneOptions() {
    static const std::array<std::pair<int, unsigned>, 4> z = {{{kOptZoneResidential, lotwatch::kZoneResidential}, {kOptZoneCommercial, lotwatch::kZoneCommercial},
                                                              {kOptZoneIndustrial, lotwatch::kZoneIndustrial}, {kOptZoneAgriculture, lotwatch::kZoneFarm}}};
    return z;
}

// Days <-> months of the demolition delays (a game year has 365 days).
inline int MonthsFromDays(std::uint32_t days) { return static_cast<int>(std::lround(static_cast<double>(days) * 12.0 / 365.0)); }
inline std::uint32_t DaysFromMonths(int months) { return static_cast<std::uint32_t>(std::lround(months * 365.0 / 12.0)); }

// The player's choices: 0 / 1 for a check box, the number for a percentage or a delay.
struct OptionsDraft {
    std::array<int, kOptionCount> value{};
    bool operator==(const OptionsDraft& o) const { return value == o.value; }
    bool operator!=(const OptionsDraft& o) const { return !(*this == o); }
    int at(int i) const { return value[static_cast<std::size_t>(i)]; }
    bool on(int i) const { return at(i) != 0; }
    // an option can be changed only while every option above it is on (its value is kept while one is off)
    bool enabled(int i) const {
        const auto& spec = OptionSpecs()[static_cast<std::size_t>(i)];
        for (int p = spec.parent; p >= 0; p = OptionSpecs()[static_cast<std::size_t>(p)].parent)
            if (!on(p)) return false;
        for (int p = spec.parent2; p >= 0; p = OptionSpecs()[static_cast<std::size_t>(p)].parent)      // the second dependency: it and everything above it
            if (!on(p)) return false;
        return true;
    }
    void toggle(int i) { if (OptionSpecs()[static_cast<std::size_t>(i)].kind == OptionKind::Check && enabled(i)) value[static_cast<std::size_t>(i)] = on(i) ? 0 : 1; }
    void setNumber(int i, int n) {
        const auto& s = OptionSpecs()[static_cast<std::size_t>(i)];
        if (s.kind == OptionKind::Check || !enabled(i)) return;
        value[static_cast<std::size_t>(i)] = n < s.min ? s.min : n > s.max ? s.max : n;
    }
};

struct OptionsConfigs {
    AutoFinanceConfig civic;
    UtilityResearchConfig utility;
    lotwatch::DemolitionConfig demolition;
};

// The master switch is on while any part of the Department runs.
inline bool MasterOn(const OptionsConfigs& c) {
    return c.civic.mode == AutoMode::Auto || UnifiedGateOpenRuntime(c.utility) || c.demolition.mode == lotwatch::DemolitionMode::Auto;
}

// What the files say (the stored choice of each option, also while the master switch is off).
inline OptionsDraft DraftFromConfigs(const OptionsConfigs& c) {
    const auto& u = c.utility;
    const auto& r = c.demolition.rules;
    OptionsDraft d;
    auto& v = d.value;
    const auto clampMonths = [](std::uint32_t days, int lo, int hi) { const int m = MonthsFromDays(days); return m < lo ? lo : m > hi ? hi : m; };
    v[kOptMaster] = MasterOn(c);
    v[kOptEducation] = c.civic.education;
    v[kOptEducationReserve] = static_cast<int>(std::lround(c.civic.EducationReserve()));
    v[kOptHealth] = c.civic.health;
    v[kOptHealthReserve] = static_cast<int>(std::lround(c.civic.HealthReserve()));
    v[kOptPower] = u.autoPower;
    v[kOptPowerReserve] = ReservePercentInt(u.powerReserve);
    v[kOptPowerAging] = u.considerPowerAging;
    v[kOptWater] = u.autoWater;
    v[kOptWaterReserve] = ReservePercentInt(u.waterReserve);
    v[kOptWaterPipes] = u.protectWaterPipes;
    v[kOptSanitation] = u.autoGarbage;
    v[kOptSanitationReserve] = ReservePercentInt(u.garbageReserve);
    v[kOptDeals] = u.autoDeals;
    v[kOptArbitrage] = u.allowUtilityArbitrage;
    v[kOptDemolishDestroyed] = r.destroyedOn;
    v[kOptDestroyedMonths] = clampMonths(r.destroyedAfterDays, 1, 24);
    v[kOptDemolishMunicipal] = r.demolishMunicipalRuins;
    v[kOptDemolishAbandoned] = r.abandonedOn;
    v[kOptAbandonedMonths] = clampMonths(r.abandonedAfterDays, 1, 60);
    for (const auto& [opt, bit] : ZoneOptions()) v[static_cast<std::size_t>(opt)] = (r.abandonedZones & bit) != 0;
    for (const auto& [opt, reason] : ReasonOptions()) v[static_cast<std::size_t>(opt)] = (r.abandonedReasons & lotwatch::ReasonBit(reason)) != 0;
    const std::int64_t budget = c.demolition.monthlyBudget;
    v[kOptBudgetLimit] = c.demolition.limitMonthlyBudget;
    v[kOptDemolitionBudget] = budget < 0 ? 0 : budget > 200000 ? 200000 : static_cast<int>(budget);
    v[kOptTradePower] = u.tradePower;
    v[kOptTradeWater] = u.tradeWater;
    v[kOptTradeGarbage] = u.tradeGarbage;
    v[kOptLandfillAdvice] = u.warnLandfillSizeSavings;
    v[kOptLandfillThreshold] = u.landfillSavingsAlertThreshold;
    return d;
}

// ---- ini editing ----------------------------------------------------------------------------------------------------------------------------------------------

inline std::string IniTrim(const std::string& s) {
    const auto b = s.find_first_not_of(" \t\r");
    if (b == std::string::npos) return std::string();
    return s.substr(b, s.find_last_not_of(" \t\r") - b + 1);
}
inline bool IniSameKey(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i)
        if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i]))) return false;
    return true;
}
// The key of a "key=value" line, or "" (comments, sections, blank lines).
inline std::string IniLineKey(const std::string& line) {
    const std::string t = IniTrim(line);
    const auto eq = t.find('=');
    if (t.empty() || t[0] == ';' || t[0] == '#' || t[0] == '[' || eq == std::string::npos) return std::string();
    return IniTrim(t.substr(0, eq));
}

inline bool IniHasKey(const std::string& text, const std::string& key) {
    std::size_t pos = 0;
    while (pos < text.size()) {
        auto eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size();
        if (IniSameKey(IniLineKey(text.substr(pos, eol - pos)), key)) return true;
        pos = eol + 1;
    }
    return false;
}

// Sets each key to its value: an existing line is rewritten in place (a comment after the value is kept: only [LotDemolition] has them), a missing key is
// added at the end; every other line is kept as it is. A missing file (empty text) gets the section header first. The file's own line end is used.
inline std::string IniSetKeys(const std::string& text, const std::string& section, const std::vector<std::pair<std::string, std::string>>& keys) {
    const std::string nl = text.find("\r\n") != std::string::npos ? "\r\n" : "\n";
    std::vector<std::string> lines;
    for (std::size_t pos = 0; pos < text.size();) {
        auto eol = text.find('\n', pos);
        if (eol == std::string::npos) eol = text.size();
        std::string line = text.substr(pos, eol - pos);
        if (!line.empty() && line.back() == '\r') line.pop_back();
        lines.push_back(line);
        pos = eol + 1;
    }
    if (lines.empty()) lines.push_back("[" + section + "]");
    for (const auto& [key, value] : keys) {
        bool done = false;
        for (auto& line : lines) {
            if (!IniSameKey(IniLineKey(line), key)) continue;
            const auto eq = line.find('=');
            const auto comment = line.find(';', eq);
            std::string next = key + "=" + value;
            if (comment != std::string::npos) {                                        // keep the comment, in its column when there is room
                const std::size_t col = comment;
                next += next.size() + 1 < col ? std::string(col - next.size(), ' ') : std::string(" ");
                next += line.substr(comment);
            }
            line = next;
            done = true;
        }
        if (!done) {
            auto at = lines.end();
            while (at != lines.begin() && IniTrim(*(at - 1)).empty()) --at;   // after the last non-empty line
            lines.insert(at, key + "=" + value);
        }
    }
    std::string out;
    for (std::size_t i = 0; i < lines.size(); ++i) out += lines[i] + (i + 1 < lines.size() || (!text.empty() && text.back() == '\n') || text.empty() ? nl : "");
    return out;
}

// DepartmentEfficiency.ini as ONE text (`file`) and the three sections that each module's own parser is given (each with its header; "" = the section is absent).
struct OptionsIniTexts {
    std::string file, civic, utility, demolition;
    OptionsIniTexts() = default;
    static OptionsIniTexts FromFile(const std::string& f) {
        OptionsIniTexts t;
        t.file = f;
        t.civic = IniSectionText(f, kIniSecAutoFinance);
        t.utility = IniSectionText(f, kIniSecUtility);
        t.demolition = IniSectionText(f, kIniSecDemolition);
        return t;
    }
    // the three sections joined into a file (tests, an empty file); an absent section stays absent
    OptionsIniTexts(const std::string& c, const std::string& u, const std::string& d) {
        auto part = [](const std::string& s) { return s.empty() || s.back() == '\n' ? s : s + "\n"; };
        *this = FromFile(part(c) + part(u) + part(d));
    }
    OptionsIniTexts(const char* c, const char* u, const char* d) : OptionsIniTexts(std::string(c), std::string(u), std::string(d)) {}
};

// Sets keys inside ONE section of the file (a repeated key such as Mode only ever changes in its own section); every other byte of the file is kept.
inline std::string IniSetKeysInSection(const std::string& file, const std::string& section, const std::vector<std::pair<std::string, std::string>>& keys) {
    return IniReplaceSection(file, section, IniSetKeys(IniSectionText(file, section), section, keys));
}

// What the three sections make the plugin do (a missing section = the parser's defaults; an invalid section = what the plugin does with it: its defaults, all Off; the other two are not affected).
inline OptionsConfigs ConfigsFromTexts(const OptionsIniTexts& t, std::string* error = nullptr) {
    OptionsConfigs c;
    std::string e;
    if (!t.civic.empty() && !ParseAutoFinanceConfig(t.civic, c.civic, e)) { c.civic = AutoFinanceConfig{}; if (error) *error = "DepartmentEfficiency.ini [AutoFinance]: " + e; }
    if (!t.utility.empty() && !ParseUtilityResearchConfig(t.utility, c.utility, e)) { c.utility = UtilityResearchConfig{}; if (error) *error = "DepartmentEfficiency.ini [UtilityResearch]: " + e; }
    if (!t.demolition.empty() && !lotwatch::ParseDemolitionConfig(t.demolition, c.demolition, e)) { c.demolition = lotwatch::DemolitionConfig{}; if (error) *error = "DepartmentEfficiency.ini [LotDemolition]: " + e; }
    return c;
}
inline OptionsDraft DraftFromTexts(const OptionsIniTexts& t) { return DraftFromConfigs(ConfigsFromTexts(t)); }

struct OptionsApplyResult {
    bool ok = false;
    bool civicChanged = false, utilityChanged = false, demolitionChanged = false;
    OptionsIniTexts texts;
    std::vector<std::string> keys;   // "DepartmentEfficiency.ini[Section]:Key=value" of every key written (for the log)
    std::string error;
};

// The new contents of the ini file for the player's choices `want`, starting from the current files.
inline OptionsApplyResult ApplyOptions(const OptionsIniTexts& current, const OptionsDraft& want) {
    OptionsApplyResult r;
    std::string error;
    const OptionsConfigs c = ConfigsFromTexts(current, &error);
    if (!error.empty()) { r.error = error + " (invalid now): not changed"; return r; }
    const OptionsDraft now = DraftFromConfigs(c);
    const auto changed = [&](int i) { return now.at(i) != want.at(i); };
    const auto b = [](bool v) { return std::string(v ? "true" : "false"); };
    const bool master = want.on(kOptMaster);

    // [AutoFinance]
    std::vector<std::pair<std::string, std::string>> ck;
    if (changed(kOptEducation) || changed(kOptHealth)) {                                      // both together: a missing key defaults to true
        ck.push_back({"Education", b(want.on(kOptEducation))});
        ck.push_back({"Health", b(want.on(kOptHealth))});
    }
    if (changed(kOptEducationReserve)) ck.push_back({"EducationReservePercent", std::to_string(want.at(kOptEducationReserve))});
    if (changed(kOptHealthReserve)) ck.push_back({"HealthReservePercent", std::to_string(want.at(kOptHealthReserve))});
    const AutoMode civicMode = master && (want.on(kOptEducation) || want.on(kOptHealth)) ? AutoMode::Auto : AutoMode::Off;
    if (c.civic.mode != civicMode) ck.push_back({"Mode", civicMode == AutoMode::Auto ? "Auto" : "Off"});

    // [UtilityResearch]
    std::vector<std::pair<std::string, std::string>> uk;
    const std::pair<int, const char*> managers[] = {{kOptPower, "AutoPower"}, {kOptWater, "AutoWater"}, {kOptSanitation, "AutoGarbage"}, {kOptDeals, "AutoDeals"}};
    bool anyChanged = false;
    for (const auto& [i, key] : managers) anyChanged = anyChanged || changed(i);
    if (anyChanged)                                                                           // all four: opening the controller must not turn on a manager the player did not choose
        for (const auto& [i, key] : managers) uk.push_back({key, b(want.on(i))});
    if (want.on(kOptPower) && changed(kOptPower) && !c.utility.power) uk.push_back({"Power", "true"});      // the planning scope of the power manager
    if (want.on(kOptWater) && changed(kOptWater) && !c.utility.water) uk.push_back({"Water", "true"});
    const struct { int i; const char* factorKey; const char* percentKey; } reserves[] = {{kOptPowerReserve, "PowerReserve", "PowerReservePercent"},
                                                                                         {kOptWaterReserve, "WaterReserve", "WaterReservePercent"},
                                                                                         {kOptSanitationReserve, "GarbageReserve", "GarbageReservePercent"}};
    for (const auto& res : reserves) {
        if (!changed(res.i)) continue;
        if (IniHasKey(current.utility, res.factorKey)) {                                      // the file uses the factor spelling: keep it (both spellings conflict)
            char f[16];
            std::snprintf(f, sizeof f, "%.2f", ReserveFactorFromPercent(want.at(res.i)));
            uk.push_back({res.factorKey, f});
        } else {
            uk.push_back({res.percentKey, std::to_string(want.at(res.i))});
        }
    }
    if (changed(kOptPowerAging)) uk.push_back({"ConsiderPowerAging", b(want.on(kOptPowerAging))});
    if (changed(kOptWaterPipes)) uk.push_back({"ProtectWaterPipes", b(want.on(kOptWaterPipes))});
    if (changed(kOptArbitrage)) uk.push_back({"AllowUtilityArbitrage", b(want.on(kOptArbitrage))});
    if (changed(kOptTradePower)) uk.push_back({"AutoTradePower", b(want.on(kOptTradePower))});     // the obsolete AutoImport* / AutoExportGarbage keys are never written
    if (changed(kOptTradeWater)) uk.push_back({"AutoTradeWater", b(want.on(kOptTradeWater))});
    if (changed(kOptTradeGarbage)) uk.push_back({"AutoTradeGarbage", b(want.on(kOptTradeGarbage))});
    if (changed(kOptLandfillAdvice)) uk.push_back({"WarnLandfillSizeSavings", b(want.on(kOptLandfillAdvice))});     // advice only: it changes no manager, no funding and no contract
    if (changed(kOptLandfillThreshold)) uk.push_back({"LandfillSavingsAlertThreshold", std::to_string(want.at(kOptLandfillThreshold))});   // advice only, like the box above
    // the unified controller runs (Mode=Auto) while the master switch is on, even with no utility chosen: the switch itself is stored there
    if (master) {
        if (!c.utility.enabled) uk.push_back({"Enabled", "true"});
        if (c.utility.mode != UtilityMode::Auto) uk.push_back({"Mode", "Auto"});
        if (!c.utility.unifiedAuto && !c.utility.unifiedAutoValidation) uk.push_back({"EnableUnifiedAuto", "true"});
    } else if (c.utility.mode != UtilityMode::Off) {
        uk.push_back({"Mode", "Off"});
    }

    // [LotDemolition]
    std::vector<std::pair<std::string, std::string>> dk;
    if (changed(kOptDemolishDestroyed)) dk.push_back({"DemolishDestroyed", b(want.on(kOptDemolishDestroyed))});
    if (changed(kOptDestroyedMonths)) dk.push_back({"DestroyedAfterDays", std::to_string(DaysFromMonths(want.at(kOptDestroyedMonths)))});
    if (changed(kOptDemolishMunicipal)) dk.push_back({"DemolishMunicipalAndHistoricRuins", b(want.on(kOptDemolishMunicipal))});   // the legacy, inverse key DestroyedKeepMunicipal is never written
    if (changed(kOptDemolishAbandoned)) dk.push_back({"DemolishAbandoned", b(want.on(kOptDemolishAbandoned))});
    if (changed(kOptAbandonedMonths)) dk.push_back({"AbandonedAfterDays", std::to_string(DaysFromMonths(want.at(kOptAbandonedMonths)))});
    bool zonesChanged = false, reasonsChanged = false;
    unsigned zones = 0, reasons = 0;
    for (const auto& [opt, bit] : ZoneOptions()) { zonesChanged = zonesChanged || changed(opt); if (want.on(opt)) zones |= bit; }
    for (const auto& [opt, reason] : ReasonOptions()) { reasonsChanged = reasonsChanged || changed(opt); if (want.on(opt)) reasons |= lotwatch::ReasonBit(reason); }
    if (zonesChanged) dk.push_back({"AbandonedZones", lotwatch::NameList(zones, false)});
    if (reasonsChanged) dk.push_back({"AbandonedReasons", lotwatch::NameList(reasons, true)});
    if (changed(kOptDemolitionBudget)) dk.push_back({"MonthlyBudget", std::to_string(want.at(kOptDemolitionBudget))});
    if (changed(kOptBudgetLimit)) dk.push_back({"LimitMonthlyBudget", b(want.on(kOptBudgetLimit))});
    const bool demolish = master && (want.on(kOptDemolishDestroyed) || want.on(kOptDemolishAbandoned));
    const auto demolitionMode = c.demolition.mode;
    if (demolish && demolitionMode != lotwatch::DemolitionMode::Auto) dk.push_back({"Mode", "Auto"});
    if (!demolish && demolitionMode == lotwatch::DemolitionMode::Auto) dk.push_back({"Mode", "Off"});

    std::string file = current.file;
    if (!ck.empty()) { file = IniSetKeysInSection(file, kIniSecAutoFinance, ck); r.civicChanged = true; }
    if (!uk.empty()) { file = IniSetKeysInSection(file, kIniSecUtility, uk); r.utilityChanged = true; }
    if (!dk.empty()) { file = IniSetKeysInSection(file, kIniSecDemolition, dk); r.demolitionChanged = true; }
    r.texts = OptionsIniTexts::FromFile(file);
    for (const auto& [k, val] : ck) r.keys.push_back(std::string("DepartmentEfficiency.ini[") + kIniSecAutoFinance + "]:" + k + "=" + val);
    for (const auto& [k, val] : uk) r.keys.push_back(std::string("DepartmentEfficiency.ini[") + kIniSecUtility + "]:" + k + "=" + val);
    for (const auto& [k, val] : dk) r.keys.push_back(std::string("DepartmentEfficiency.ini[") + kIniSecDemolition + "]:" + k + "=" + val);

    // the files must give back exactly the chosen options, read by the plugin's own parsers
    error.clear();
    const OptionsConfigs back = ConfigsFromTexts(r.texts, &error);
    if (!error.empty()) { r.error = "the new " + error + " would be invalid"; return r; }
    if (DraftFromConfigs(back) != want) { r.error = "the new ini files would not give the chosen options (a research option keeps the controller off?)"; return r; }
    r.ok = true;
    return r;
}

// 12500 -> "12.500" with the language's separator.
inline std::string FormatThousands(long long n, const std::string& separator) {
    std::string digits = std::to_string(n < 0 ? -n : n), out;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (i && (digits.size() - i) % 3 == 0) out += separator;
        out += digits[i];
    }
    return (n < 0 ? "-" : "") + out;
}

// "{0}" replaced by the argument (a translation may move it).
inline std::string FillText(std::string pattern, const std::string& arg) {
    const auto at = pattern.find("{0}");
    if (at != std::string::npos) pattern.replace(at, 3, arg);
    return pattern;
}

}  // namespace sc4af
