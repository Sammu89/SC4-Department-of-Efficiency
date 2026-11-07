// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstdint>
#include <string>

namespace sc4af {

// Player cues. The player cannot read the log, but an external top-level window (a system message box) corrupts the game's own graphics,
// so NO window is ever created and no focus is taken. A cue is an audible pattern plus a short text on the title bar of the game's OWN
// window (existing window, no new one).
enum class NoticeKind { None, Ready, NotReady, CityPartial, PreReloadComplete, ValidationComplete, ValidationFailed, CityScout, NoSupply };

struct NoticeCue {
    NoticeKind kind = NoticeKind::None;
    std::string title;     // single line, ASCII, shown on the game window title
    int beeps = 0;         // 1 = information, 2 = needs the player, 3 = problem
    bool problem = false;
    std::int32_t advice = 0;   // event id of the native SC4 Urgent Advice record in DepartmentEfficiency.dat; 0 = none
};

// Event ids bound to the advice records of DepartmentEfficiency.dat.
constexpr std::int32_t kAdviceReady = 0x5C4AF001, kAdviceFailed = 0x5C4AF002, kAdvicePart1 = 0x5C4AF003, kAdviceComplete = 0x5C4AF004, kAdvicePartial = 0x5C4AF005;

// City scout: one record per combination: bit 0 BUY_POWER is offered by the game, bit 1 the city is short of power.
constexpr std::int32_t kAdviceScoutBase = 0x5C4AF020;

// No way to supply a utility (local production and the neighbor deals are insufficient): one record per commodity, Power / Water / Garbage.
constexpr std::int32_t kAdviceNoSupplyBase = 0x5C4AF030;

// Education / Health capacity notices (Health and Education advisor, BAD_JOB 45: red in the news list, no pop-up): ONE notice per facility type (CivicType, CivicWatch.h) with one native link per
// affected building, 1 to kCivicMaxLinks of them. Per type there is one static record for every number of buildings; the plugin retires the shown notice before it pushes another one.
// The link of slot i reads the advice local value l_sc4af_<type>_0_<i>_h (cISC4AdvisorSystem::SetLocalValue, set right before the event is pushed, and rewritten in place when the
// buildings change but not their number).
// The ids of a block start at offset 2: the records at offsets 0 and 1 do not show in the game (cause unknown); this is a workaround.
constexpr int kCivicMaxLinks = 10;            // at most ten buildings per notice (the ten most severe; the next ones enter as these are solved)
constexpr int kCivicVersions = 1;
constexpr std::int32_t kAdviceCivicBase = 0x5C4AF400;          // events
constexpr std::int32_t kAdviceCivicGuidBase = 0x5C4AD600;      // advice records (the id the advisor lists the notice under)
constexpr int CivicAdviceIndex(int kind, int version, int buildings) { return (kind * kCivicVersions + version) * kCivicMaxLinks + (buildings - 1) + 2; }
constexpr std::int32_t CivicAdviceEvent(int kind, int version, int buildings) { return kAdviceCivicBase + CivicAdviceIndex(kind, version, buildings); }
constexpr std::int32_t CivicAdviceGuid(int kind, int version, int buildings) { return kAdviceCivicGuidBase + CivicAdviceIndex(kind, version, buildings); }
// Advice subject (a location the advice's links can move the camera to): class / interface ids found in SimCity 4.exe 1.1.641.0. SetLocation takes CELL coordinates
// (world metres / 16); the news entry keeps its own reference; the link game.camera_jump_and_zoom(game.event_subject(), ...) moves the camera to the building.
constexpr std::uint32_t kAdviceSubjectClsid = 0x6A26D049u;
constexpr std::uint32_t kAdviceSubjectIid = 0x6A26D050u;

// Landfill notices of the Utilities advisor.
//   EXPAND / REDUCE (option "Warn me when checking landfill size can help me save", INI WarnLandfillSizeSavings, threshold LandfillSavingsAlertThreshold): GREAT_JOB mood, priority URGENT (100):
//     good news, green in the news list, shown in the game's own pop-up when the player's settings allow urgent advice. They name a CONCRETE area: {ACROSS} x {ALONG} squares
//     and a link that moves the camera to its centre (the view can be rotated, so the text never says "right" or "bottom").
//   Ghost squares are repaired silently (log only): no notice.
// The numbers are numeric advisor GLOBAL values the plugin writes right before the event is pushed (cISC4AdvisorSystem::SetGlobalValue(name, double)); the link reads the advice local value
// kLandfillLinkVariable (an advice subject placed on CELL coordinates, as the civic notices do). The records read them with #game.<name># tokens.
enum class LandfillAdviceKind { Expand = 0, Reduce = 1 };
constexpr int kLandfillAdviceKinds = 2;
constexpr std::int32_t kAdviceLandfillBase = 0x5C4AF070;        // events: + LandfillAdviceKind (0x5C4AF070 expand, 0x5C4AF071 reduce); advice records 0x5C4AD070 + kind, texts 0x5C4AE070 / 0x5C4AE170 + kind
constexpr std::int32_t kAdviceLandfillGuidBase = 0x5C4AD070;
constexpr std::int32_t LandfillAdviceEvent(LandfillAdviceKind k) { return kAdviceLandfillBase + static_cast<std::int32_t>(k); }
constexpr std::int32_t LandfillAdviceGuid(LandfillAdviceKind k) { return kAdviceLandfillGuidBase + static_cast<std::int32_t>(k); }
constexpr const char* kLandfillTilesGlobal = "g_sc4af_lf_tiles";        // {TILES}: whole squares to add / remove
constexpr const char* kLandfillPercentGlobal = "g_sc4af_lf_percent";    // {PERCENT}: whole percent of the monthly sanitation bill
constexpr const char* kLandfillSavingsGlobal = "g_sc4af_lf_savings";    // {SAVINGS}: whole simoleons per month (the text puts the section sign before it)
constexpr const char* kLandfillAcrossGlobal = "g_sc4af_lf_across";      // {ACROSS}: squares away from the landfill edge (depth of the area, or rows / columns removed)
constexpr const char* kLandfillAlongGlobal = "g_sc4af_lf_along";        // {ALONG}: squares along the landfill edge
constexpr const char* kLandfillLinkVariable = "l_sc4af_lf_h";           // the link: game.l_sc4af_lf_h_subject

// A notice is only worth showing with real numbers: at least one tile, a percentage in 1..100 and at least one simoleon a month. The bound keeps every value an exact whole number in the game's
// Lua numbers (printed without exponent, 14 digits).
constexpr std::int64_t kLandfillAdviceMaxValue = 99999999999999LL;
constexpr bool LandfillAdviceValuesValid(std::int64_t tiles, int percent, std::int64_t savings) {
    return tiles >= 1 && tiles <= kLandfillAdviceMaxValue && percent >= 1 && percent <= 100 && savings >= 1 && savings <= kLandfillAdviceMaxValue;
}

// Everything one landfill notice carries.
struct LandfillNotice {
    LandfillAdviceKind kind = LandfillAdviceKind::Expand;
    std::int64_t tiles = 0;          // squares to add / remove
    int percent = 0;                 // of the monthly sanitation expense
    std::int64_t savings = 0;        // monthly
    int across = 0, along = 0;       // EXPAND / REDUCE area
    float linkX = 0, linkZ = 0;      // cell coordinates of the link (centre of the area / of the landfill)
};
inline bool LandfillNoticeValid(const LandfillNotice& n) {
    const bool link = n.linkX >= 0 && n.linkZ >= 0 && n.linkX < 4096 && n.linkZ < 4096;
    return link && (n.kind == LandfillAdviceKind::Expand || n.kind == LandfillAdviceKind::Reduce) && LandfillAdviceValuesValid(n.tiles, n.percent, n.savings) && n.across >= 1 && n.along >= 1 &&
           static_cast<std::int64_t>(n.across) * n.along == n.tiles;
}

// What the player reads once the game has substituted the tokens: the pattern of a text of the DAT with {TILES}, {PERCENT} and {SAVINGS} replaced by the plain whole numbers (a preview:
// in the game the substitution adds the thousands separator of the game's language, e.g. §1.234 for 1234). Pure; the plugin itself never builds the final text (the game does, from the record).
inline std::string FillLandfillAdviceText(std::string pattern, std::int64_t tiles, int percent, std::int64_t savings) {
    const auto put = [&pattern](const char* token, const std::string& value) {
        const std::string t = token;
        for (std::size_t at = pattern.find(t); at != std::string::npos; at = pattern.find(t, at + value.size())) pattern.replace(at, t.size(), value);
    };
    put("{TILES}", std::to_string(tiles));
    put("{PERCENT}", std::to_string(percent));
    put("{SAVINGS}", std::to_string(savings));
    return pattern;
}
inline std::string FillLandfillAdviceText(std::string pattern, const LandfillNotice& n) {
    pattern = FillLandfillAdviceText(std::move(pattern), n.tiles, n.percent, n.savings);
    const auto put = [&pattern](const char* token, const std::string& value) {
        const std::string t = token;
        for (std::size_t at = pattern.find(t); at != std::string::npos; at = pattern.find(t, at + value.size())) pattern.replace(at, t.size(), value);
    };
    put("{ACROSS}", std::to_string(n.across));
    put("{ALONG}", std::to_string(n.along));
    return pattern;
}

// Id of the activation greeting of the Department (City Planning advisor). Unused: no record ships for it.
constexpr std::int32_t kAdviceGreeting = 0x5C4AF040;

// Formats an id as eight upper-case hex digits.
inline std::string HexId(std::int32_t v) {
    static const char* d = "0123456789ABCDEF";
    std::string s(8, '0');
    for (int i = 0; i < 8; ++i) s[7 - i] = d[(static_cast<std::uint32_t>(v) >> (4 * i)) & 0xF];
    return s;
}

// Maps a log line to the cue it announces; any other line yields kind None.
inline NoticeCue CueForLogLine(const std::string& t) {
    NoticeCue c;
    const auto has = [&](const char* s) { return t.find(s) != std::string::npos; };
    if (has("[Service][NoSupply]")) {
        const int idx = has("] POWER") ? 0 : has("] WATER") ? 1 : 2;
        static const char* titles[3] = {"SC4AF: insufficient electricity - build new power plants", "SC4AF: insufficient water supply - build new water facilities", "SC4AF: insufficient waste treatment - build new waste facilities"};
        c = {NoticeKind::NoSupply, titles[idx], 1, true, kAdviceNoSupplyBase + idx};
    } else if (has("[FinalValidation][Scout]")) {
        const bool b = has("buyPower=YES"), d = has("powerDeficit=YES");
        const int code = (b ? 1 : 0) | (d ? 2 : 0);
        c = {NoticeKind::CityScout, std::string("SC4AF SCOUT: BUY_POWER offered ") + (b ? "YES" : "NO") + ", city short of power " + (d ? "YES" : "NO"), 1, false, kAdviceScoutBase + code};
    } else if (has("[FinalValidation] SESSION_PRE_RELOAD_COMPLETE")) {
        c = {NoticeKind::PreReloadComplete, "SC4AF VALIDATION: PART 1 DONE - save, go to Region View (do not close the game), load the SAME city, let it run", 2, false, kAdvicePart1};
    } else if (has("[FinalValidation] SESSION_COMPLETE")) {
        c = {NoticeKind::ValidationComplete, "SC4AF VALIDATION COMPLETE - answer the graphics question, then close the game and send DepartmentEfficiency.log", 2, false, kAdviceComplete};
    } else if (has("[FinalValidation] SESSION_FAILED")) {
        c = {NoticeKind::ValidationFailed, "SC4AF VALIDATION FAILED (stopped safely) - close the game and send DepartmentEfficiency.log", 3, true, kAdviceFailed};
    } else if (has("[FinalValidation] CITY_PARTIAL")) {
        if (has("CONTINUE with this city"))
            c = {NoticeKind::CityPartial, "SC4AF VALIDATION: partial city (no water) - CONTINUE, do nothing", 1, false, kAdvicePartial};
        else
            c = {NoticeKind::CityPartial, "SC4AF VALIDATION: unsuitable city - go to Region View (do not close the game) and load a disposable city with water, garbage and neighbor connections", 3, true, kAdvicePartial};
    }
    return c;
}

inline NoticeCue CueForReadiness(const std::string& line) {
    NoticeCue c;
    if (line.find("READY_BUDGETS_ONLY") != std::string::npos) return c;     // deliberate configuration: no cue
    if (line.find("NOT_READY") != std::string::npos)
        c = {NoticeKind::NotReady, "SC4AF VALIDATION NOT READY - close the game and send DepartmentEfficiency.log", 3, true, kAdviceFailed};
    else
        c = {NoticeKind::Ready, "SC4AF VALIDATION READY - run at high speed, touch nothing", 1, false, kAdviceReady};
    return c;
}

}  // namespace sc4af
