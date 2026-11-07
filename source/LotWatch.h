// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// LotWatch: observes the lots the game has left abandoned or destroyed and logs which ones the demolition rules pick. With DepartmentEfficiencyDemolition.ini Mode=Auto (Off by default) it
// also demolishes them, through the game's OWN command (`Demolish x1 z1 x2 z2 None`, the documented command that community DLLs already issue): no demolition parameter is guessed.
//
// Rules (the defaults; each can be changed in the options window (OptionsModel.h) or DepartmentEfficiencyDemolition.ini):
//   * a destroyed lot (Destroyed: fire, disaster) is a candidate after one month (DemolishDestroyed, DestroyedAfterDays=30);
//   * an abandoned lot (Vacant) is a candidate after one year in that state (DemolishAbandoned, AbandonedAfterDays=365), when its zone type (AbandonedZones: residential,
//     commercial, industrial by default; agriculture off) and the reason it was abandoned (AbandonedReasons: every reason but power and water by default) are selected;
//     occupants sent away (KickOut) and an unknown reason are not selectable: they always count;
//   * municipal lots (plopped, special or without a zone) and historical lots are PROTECTED by default. One option, DemolishMunicipalAndHistoricRuins (default false),
//     lets their RUINS (a Destroyed lot) go after the same DestroyedAfterDays; it never makes a working or an abandoned municipal / historical building a candidate. An ABANDONED municipal
//     lot is always kept: it practically never happens (none of about 17 400 abandoned lots observed was municipal) and a historical one is never abandoned-demolished either.
//     The legacy keys DestroyedKeepMunicipal (the OPPOSITE meaning) and AbandonedKeepMunicipal are still accepted in old files and IGNORED: they can never lift the protection.
// Vacant AGRICULTURE lots (1x1 fields, vacant for ever and without water by nature) are kept unless agriculture is selected in AbandonedZones.
//
// The state, the days in it and the vacate reason are cISC4Lot's own (GetState, GetDaysSinceLastStateChange, GetVacateReason); this header only holds the pure rules (testable without the game).
#pragma once

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {
namespace lotwatch {

// cISC4Lot::HabitationState (gzcom-dll cISC4Lot.h)
enum : std::uint32_t { kCreated = 0, kReady = 1, kConstructing = 2, kOccupied = 3, kVacant = 4, kDestructing = 5, kDestroyed = 6, kStateCount = 7 };
// cISC4ZoneManager::ZoneType
enum : int { kZoneAgriculture = 7, kZonePlopped = 15, kZoneCount = 16 };
// cISC4Lot::VacateReason
enum : int { kReasonNone = 0, kLowDesirability = 1, kNoPower = 2, kNoWater = 3, kCommuteTime = 4, kKickOut = 5, kLowDemand = 6, kReasonCount = 7 };

// The reasons and zone types of abandoned lots that may be demolished, as bits.
constexpr unsigned ReasonBit(int reason) { return 1u << reason; }
constexpr unsigned kDefaultReasons = ReasonBit(kLowDesirability) | ReasonBit(kCommuteTime) | ReasonBit(kLowDemand);
// Always demolished with DemolishAbandoned, whatever AbandonedReasons says (no option for them). The names stay valid in old files.
constexpr unsigned kAlwaysReasons = ReasonBit(kKickOut) | ReasonBit(kReasonNone);
enum : unsigned { kZoneResidential = 1, kZoneCommercial = 2, kZoneIndustrial = 4, kZoneFarm = 8 };
constexpr unsigned kDefaultZones = kZoneResidential | kZoneCommercial | kZoneIndustrial;

struct Rules {
    bool destroyedOn = true, abandonedOn = true;   // DemolishDestroyed / DemolishAbandoned
    std::uint32_t destroyedAfterDays = 30;      // one month (the game's months have 28 to 31 days)
    std::uint32_t abandonedAfterDays = 365;     // one year (the game's years have 365 days)
    bool demolishMunicipalRuins = false;        // DemolishMunicipalAndHistoricRuins: the RUINS of municipal (plopped, special zones - military, airport, seaport, spaceport, landfill - or no zone) and historical lots
    unsigned abandonedReasons = kDefaultReasons;   // bits ReasonBit(reason); kAlwaysReasons count whether set or not
    unsigned abandonedZones = kDefaultZones;       // kZone* bits
};

// The zone group of a lot for AbandonedZones (0 = municipal / not a growable zone).
inline unsigned ZoneGroup(int z) {
    if (z >= 1 && z <= 3) return kZoneResidential;
    if (z >= 4 && z <= 6) return kZoneCommercial;
    if (z == 8 || z == 9) return kZoneIndustrial;
    if (z == 7) return kZoneFarm;
    return 0;
}

struct Observation {
    std::uint32_t state = kOccupied;
    std::uint32_t daysSinceChange = 0;
    int vacateReason = kReasonNone;
    bool historical = false;
    bool demolishable = true;                   // cISC4Lot::IsInDemolishableState
    int zone = 0;                               // cISC4ZoneManager::ZoneType of the lot
};

enum class Verdict {
    NotRelevant,            // occupied, under construction, ...: not this module's business
    Transitional,           // Destructing: the game is still demolishing it
    KeepHistorical,
    KeepAgriculture,        // a vacant farm field: its normal state, not an abandoned building
    KeepNotGrowable,        // a plopped / civic / special lot (stations, schools, parks, landmarks, military, airport, seaport, landfill, no zone): never demolished
    KeepUtilityReason,      // abandoned for lack of power or water: easy to restore
    TooRecent,              // Destroyed / Vacant for less than the rule asks
    EligibleDestroyed,
    EligibleAbandoned,
    EligibleButBlocked,     // the rule says yes but the game says the lot is not in a demolishable state
    KeepOptionOff,          // the player switched this kind of demolition off (destroyed or abandoned)
    KeepZoneOff,            // abandoned in a zone type the player did not choose
    KeepReasonOff,          // abandoned for a reason the player did not choose (power and water have their own verdict)
    kVerdictCount
};

// cISC4ZoneManager::ZoneType 1..6 (residential, commercial) and 8..9 (industrial). Agriculture (7) is handled apart; Plopped (15), Military, Airport, Seaport, Spaceport, Landfill and None are not growable.
inline bool IsGrowableZone(int z) { return (z >= 1 && z <= 6) || z == 8 || z == 9; }

inline Verdict Classify(const Observation& o, const Rules& r) {
    if (o.state == kDestructing) return Verdict::Transitional;
    if (o.state != kVacant && o.state != kDestroyed) return Verdict::NotRelevant;
    const bool destroyed = o.state == kDestroyed;
    const bool ruinsOfProtected = destroyed && r.demolishMunicipalRuins;   // the only way a historical or municipal lot becomes a candidate: its RUINS, never a working or abandoned building
    if (o.historical && !ruinsOfProtected) return Verdict::KeepHistorical;
    if (!(destroyed ? r.destroyedOn : r.abandonedOn)) return Verdict::KeepOptionOff;
    const unsigned group = ZoneGroup(o.zone);
    if (group == 0) {                                                    // municipal: plopped, special or no zone
        if (!ruinsOfProtected) return Verdict::KeepNotGrowable;
    } else if (!destroyed && !(r.abandonedZones & group)) {             // a destroyed lot is rubble whatever its zone; an abandoned one only in the chosen zone types
        return group == kZoneFarm ? Verdict::KeepAgriculture : Verdict::KeepZoneOff;
    }
    if (!destroyed) {
        const int reason = o.vacateReason > kReasonNone && o.vacateReason < kReasonCount ? o.vacateReason : kReasonNone;
        if (!((r.abandonedReasons | kAlwaysReasons) & ReasonBit(reason)))
            return reason == kNoPower || reason == kNoWater ? Verdict::KeepUtilityReason : Verdict::KeepReasonOff;
    }
    const std::uint32_t need = o.state == kDestroyed ? r.destroyedAfterDays : r.abandonedAfterDays;
    if (o.daysSinceChange < need) return Verdict::TooRecent;
    if (!o.demolishable) return Verdict::EligibleButBlocked;
    return o.state == kDestroyed ? Verdict::EligibleDestroyed : Verdict::EligibleAbandoned;
}

inline const char* StateName(std::uint32_t s) {
    static const char* const n[kStateCount] = {"Created", "Ready", "Constructing", "Occupied", "Vacant", "Destructing", "Destroyed"};
    return s < kStateCount ? n[s] : "Unknown";
}
inline const char* ReasonName(int r) {
    static const char* const n[kReasonCount] = {"None", "LowDesirability", "NoPower", "NoWater", "CommuteTime", "KickOutOccupants", "LowDemand"};
    return (r >= 0 && r < kReasonCount) ? n[r] : "Unknown";
}
inline const char* VerdictName(Verdict v) {
    switch (v) {
    case Verdict::NotRelevant: return "NOT_RELEVANT";
    case Verdict::Transitional: return "TRANSITIONAL";
    case Verdict::KeepHistorical: return "KEEP_HISTORICAL";
    case Verdict::KeepAgriculture: return "KEEP_FARM_FIELD";
    case Verdict::KeepNotGrowable: return "KEEP_CIVIC_OR_SPECIAL";
    case Verdict::KeepUtilityReason: return "KEEP_POWER_OR_WATER";
    case Verdict::TooRecent: return "TOO_RECENT";
    case Verdict::EligibleDestroyed: return "WOULD_DEMOLISH_DESTROYED";
    case Verdict::EligibleAbandoned: return "WOULD_DEMOLISH_ABANDONED";
    case Verdict::EligibleButBlocked: return "ELIGIBLE_BUT_NOT_DEMOLISHABLE";
    case Verdict::KeepOptionOff: return "KEEP_OPTION_OFF";
    case Verdict::KeepZoneOff: return "KEEP_ZONE_NOT_CHOSEN";
    case Verdict::KeepReasonOff: return "KEEP_REASON_NOT_CHOSEN";
    case Verdict::kVerdictCount: break;
    }
    return "?";
}
inline const char* ZoneName(int z) {
    static const char* const n[kZoneCount] = {"None", "ResLow", "ResMed", "ResHigh", "ComLow", "ComMed", "ComHigh", "Agriculture", "IndMed", "IndHigh", "Military", "Airport", "Seaport", "Spaceport", "Landfill", "Plopped"};
    return (z >= 0 && z < kZoneCount) ? n[z] : "Unknown";
}
// Listing order: what the rules would demolish first, the boring bulk (power / water, farm fields) last.
inline int ListPriority(Verdict v) {
    switch (v) {
    case Verdict::EligibleDestroyed: case Verdict::EligibleAbandoned: return 0;
    case Verdict::EligibleButBlocked: return 1;
    case Verdict::KeepHistorical: return 3;
    case Verdict::KeepNotGrowable: return 3;
    case Verdict::KeepZoneOff: case Verdict::KeepReasonOff: return 5;
    case Verdict::KeepOptionOff: return 8;
    case Verdict::kVerdictCount: return 8;
    case Verdict::Transitional: return 4;
    case Verdict::TooRecent: return 5;
    case Verdict::KeepUtilityReason: return 6;
    case Verdict::KeepAgriculture: return 7;
    case Verdict::NotRelevant: return 8;
    }
    return 8;
}
inline bool WouldDemolish(Verdict v) { return v == Verdict::EligibleDestroyed || v == Verdict::EligibleAbandoned; }

// ---- configuration (DepartmentEfficiencyDemolition.ini) -----------------------------------------------------------------------------------------------------------------------------------------------------------
enum class DemolitionMode { Off, Shadow, Auto };
inline const char* ModeName(DemolitionMode m) { return m == DemolitionMode::Auto ? "AUTO" : m == DemolitionMode::Shadow ? "SHADOW" : "OFF"; }

struct DemolitionConfig {
    DemolitionMode mode = DemolitionMode::Off;      // Off: nothing is done; Shadow: the log says what would be demolished; Auto: it is demolished
    Rules rules;
    int maxPerRun = 150;                            // lots demolished in one monthly pass at most (a technical cap; the player's limit is the budget below)
    std::int64_t minFunds = 0;                      // MinFunds: kept for old files only (no minimum treasury; the rule is "never more than the treasury")
    bool limitMonthlyBudget = false;                // LimitMonthlyBudget: off = no monthly limit
    std::int64_t monthlyBudget = 1000;              // MonthlyBudget: while the limit is on, the money the demolitions may spend in one month (free demolitions are not limited); 1000 is what the window shows when the limit is ticked
    std::int64_t BudgetLimit() const { return limitMonthlyBudget ? monthlyBudget : -1; }
};

// ---- money: a demolition is ordered only when the game's own cost of it is known and affordable ----------------------------------------------
// `projected` = the treasury minus what the orders already sent this month will cost (the game carries an order out a moment later, so the treasury has not moved yet):
// the month's demolitions together never cost more than the treasury. `monthlyBudget` < 0 = no monthly limit.
enum class Spend { Ok, CostUnknown, ExceedsTreasury, BelowMinFunds, OverBudget };
inline Spend SpendDecision(bool costKnown, std::int64_t cost, std::int64_t projected, std::int64_t minFunds, std::int64_t spentThisMonth, std::int64_t monthlyBudget) {
    if (!costKnown || cost < 0) return Spend::CostUnknown;           // no budget can be kept without the cost: not demolished
    if (cost > projected) return Spend::ExceedsTreasury;
    if (projected - cost < minFunds) return Spend::BelowMinFunds;
    if (cost > 0 && monthlyBudget >= 0 && spentThisMonth + cost > monthlyBudget) return Spend::OverBudget;
    return Spend::Ok;
}
inline const char* SpendName(Spend s) {
    switch (s) {
    case Spend::Ok: return "OK";
    case Spend::CostUnknown: return "COST_UNKNOWN";
    case Spend::ExceedsTreasury: return "COST_EXCEEDS_TREASURY";
    case Spend::BelowMinFunds: return "BELOW_MIN_FUNDS";
    case Spend::OverBudget: return "MONTHLY_BUDGET_REACHED";
    }
    return "?";
}

inline std::string TrimText(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}
inline std::string LowerText(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// The names of AbandonedReasons / AbandonedZones (comma separated, any case; an empty list = none).
inline const char* const* ReasonKeyNames() {
    static const char* const n[kReasonCount] = {"Unknown", "LowDesirability", "NoPower", "NoWater", "CommuteTime", "KickOut", "LowDemand"};
    return n;
}
inline const char* const* ZoneKeyNames() {
    static const char* const n[4] = {"Residential", "Commercial", "Industrial", "Agriculture"};
    return n;
}
inline bool ParseNameList(const std::string& value, bool reasons, unsigned& bits) {
    bits = 0;
    std::string item;
    std::istringstream in(value);
    while (std::getline(in, item, ',')) {
        item = LowerText(TrimText(item));
        if (item.empty()) continue;
        bool found = false;
        const int n = reasons ? kReasonCount : 4;
        for (int i = 0; i < n && !found; ++i)
            if (item == LowerText(reasons ? ReasonKeyNames()[i] : ZoneKeyNames()[i])) { bits |= reasons ? ReasonBit(i) : (1u << i); found = true; }
        if (!found) return false;
    }
    return true;
}
inline std::string NameList(unsigned bits, bool reasons) {
    std::string s;
    const int n = reasons ? kReasonCount : 4;
    for (int i = 0; i < n; ++i)
        if (bits & (reasons ? ReasonBit(i) : (1u << i))) s += (s.empty() ? "" : ",") + std::string(reasons ? ReasonKeyNames()[i] : ZoneKeyNames()[i]);
    return s;
}

// Strict: an unknown key, a duplicate, a bad value or a bad range makes the whole file invalid (the caller then runs with Mode=Off).
inline bool ParseDemolitionConfig(const std::string& text, DemolitionConfig& out, std::string& error) {
    DemolitionConfig c;
    std::vector<std::string> seen;
    std::istringstream in(text);
    std::string raw;
    int lineNo = 0;
    const auto number = [&](const std::string& v, long long lo, long long hi, long long& result) {
        char* end = nullptr;
        const long long n = std::strtoll(v.c_str(), &end, 10);
        if (v.empty() || *end != '\0' || n < lo || n > hi) return false;
        result = n;
        return true;
    };
    while (std::getline(in, raw)) {
        ++lineNo;
        std::string line = raw;
        const auto cut = line.find_first_of(";#");
        if (cut != std::string::npos) line.resize(cut);
        line = TrimText(line);
        if (line.empty()) continue;
        const std::string where = "line " + std::to_string(lineNo) + ": ";
        if (line.front() == '[') {
            if (LowerText(line) != "[lotdemolition]") { error = where + "unknown section " + line; return false; }
            continue;
        }
        const auto eq = line.find('=');
        if (eq == std::string::npos) { error = where + "expected key=value"; return false; }
        const std::string key = LowerText(TrimText(line.substr(0, eq))), value = TrimText(line.substr(eq + 1));
        if (std::find(seen.begin(), seen.end(), key) != seen.end()) { error = where + "duplicate key " + key; return false; }
        seen.push_back(key);
        long long n = 0;
        if (key == "mode") {
            const std::string v = LowerText(value);
            if (v == "off") c.mode = DemolitionMode::Off;
            else if (v == "shadow") c.mode = DemolitionMode::Shadow;
            else if (v == "auto") c.mode = DemolitionMode::Auto;
            else { error = where + "Mode must be Off, Shadow or Auto"; return false; }
        } else if (key == "destroyedafterdays") {
            if (!number(value, 1, 3650, n)) { error = where + "DestroyedAfterDays must be 1..3650"; return false; }
            c.rules.destroyedAfterDays = static_cast<std::uint32_t>(n);
        } else if (key == "abandonedafterdays") {
            if (!number(value, 1, 7300, n)) { error = where + "AbandonedAfterDays must be 1..7300"; return false; }
            c.rules.abandonedAfterDays = static_cast<std::uint32_t>(n);
        } else if (key == "demolishdestroyed" || key == "demolishabandoned" || key == "demolishmunicipalandhistoricruins" || key == "destroyedkeepmunicipal" || key == "abandonedkeepmunicipal" ||
                   key == "limitmonthlybudget") {
            const std::string v = LowerText(value);
            if (v != "true" && v != "false") { error = where + key + " must be true or false"; return false; }
            const bool on = v == "true";
            if (key == "demolishdestroyed") c.rules.destroyedOn = on;
            else if (key == "demolishabandoned") c.rules.abandonedOn = on;
            else if (key == "demolishmunicipalandhistoricruins") c.rules.demolishMunicipalRuins = on;
            else if (key == "limitmonthlybudget") c.limitMonthlyBudget = on;
            // destroyedkeepmunicipal / abandonedkeepmunicipal: legacy, valid and IGNORED (the first one meant the opposite of the new option and an old file must never lift the protection)
        } else if (key == "abandonedreasons" || key == "abandonedzones") {
            unsigned bits = 0;
            if (!ParseNameList(value, key == "abandonedreasons", bits)) { error = where + "unknown name in " + key; return false; }
            (key == "abandonedreasons" ? c.rules.abandonedReasons : c.rules.abandonedZones) = bits;
        } else if (key == "maxperrun") {
            if (!number(value, 1, 1000, n)) { error = where + "MaxPerRun must be 1..1000"; return false; }
            c.maxPerRun = static_cast<int>(n);
        } else if (key == "monthlybudget") {
            if (!number(value, 0, 2000000000LL, n)) { error = where + "MonthlyBudget must be 0..2000000000"; return false; }
            c.monthlyBudget = n;
        } else if (key == "minfunds") {
            if (!number(value, 0, 2000000000LL, n)) { error = where + "MinFunds must be 0..2000000000"; return false; }
            c.minFunds = n;
        } else {
            error = where + "unknown key " + key;
            return false;
        }
    }
    out = c;
    return true;
}

// ---- the lot's cells and the game command ----------------------------------------------------------------------------------------------------------------------------------------------------------
struct CellRect { int x1 = 0, z1 = 0, x2 = 0, z2 = 0; };

// The bounding rect the game reports for a lot, checked against the lot's own size (in either orientation). Whether the bottom-right cell is inclusive is verified here, not assumed: the INCLUSIVE
// rect is returned when its area is the lot's area (or the rect is exclusive and shrinks to it). Anything else is refused: a command is never sent for a rect that does not fit the lot.
inline bool ResolveInclusiveRect(int tlx, int tlz, int brx, int brz, int w, int h, CellRect& out) {
    if (w <= 0 || h <= 0 || brx < tlx || brz < tlz) return false;
    const long long want = static_cast<long long>(w) * h;
    const long long inclusive = static_cast<long long>(brx - tlx + 1) * (brz - tlz + 1);
    const long long exclusive = static_cast<long long>(brx - tlx) * (brz - tlz);
    if (inclusive == want) { out = {tlx, tlz, brx, brz}; return true; }
    if (exclusive == want && brx > tlx && brz > tlz) { out = {tlx, tlz, brx - 1, brz - 1}; return true; }
    return false;
}

// The game's documented command (sc4-game-command-documentation, AppCommands.md): `Demolish x1 y1 x2 y2 [Sim] [None] [Explode]`; None = no demolition effect.
inline std::string DemolishCommand(const CellRect& r) {
    return "Demolish " + std::to_string(r.x1) + " " + std::to_string(r.z1) + " " + std::to_string(r.x2) + " " + std::to_string(r.z2) + " None";
}

// Fail closed: after this many consecutive orders that the game did not carry out, the demolition stops for the city session.
class FailureGuard {
public:
    static constexpr int kMaxConsecutive = 3;
    void Record(bool carriedOut) {
        if (carriedOut) consecutive_ = 0;
        else if (++consecutive_ >= kMaxConsecutive) tripped_ = true;
    }
    bool tripped() const { return tripped_; }
    int consecutive() const { return consecutive_; }
    void Reset() { consecutive_ = 0; tripped_ = false; }
private:
    int consecutive_ = 0;
    bool tripped_ = false;
};

// Counts of one scan.
struct Summary {
    std::uint32_t lots = 0;
    std::uint32_t byState[kStateCount] = {};
    std::uint32_t vacantByReason[kReasonCount] = {};
    std::uint32_t verdicts[static_cast<int>(Verdict::kVerdictCount)] = {};
    std::uint32_t maxVacantDays = 0, maxDestroyedDays = 0;

    void Add(const Observation& o, Verdict v) {
        ++lots;
        if (o.state < kStateCount) ++byState[o.state];
        if (o.state == kVacant) {
            if (o.vacateReason >= 0 && o.vacateReason < kReasonCount) ++vacantByReason[o.vacateReason];
            if (o.daysSinceChange > maxVacantDays) maxVacantDays = o.daysSinceChange;
        }
        if (o.state == kDestroyed && o.daysSinceChange > maxDestroyedDays) maxDestroyedDays = o.daysSinceChange;
        ++verdicts[static_cast<int>(v)];
    }
    std::uint32_t count(Verdict v) const { return verdicts[static_cast<int>(v)]; }
    std::uint32_t wouldDemolish() const { return count(Verdict::EligibleDestroyed) + count(Verdict::EligibleAbandoned); }

    // One line, stable text: the log line changes only when a count changes.
    std::string Format() const {
        std::string s = "lots=" + std::to_string(lots) + " states{";
        for (std::uint32_t i = 0; i < kStateCount; ++i) {
            if (!byState[i]) continue;
            s += std::string(s.back() == '{' ? "" : " ") + StateName(i) + "=" + std::to_string(byState[i]);
        }
        s += "} vacantBy{";
        bool first = true;
        for (int i = 0; i < kReasonCount; ++i) {
            if (!vacantByReason[i]) continue;
            s += std::string(first ? "" : " ") + ReasonName(i) + "=" + std::to_string(vacantByReason[i]);
            first = false;
        }
        s += "} verdicts{wouldDemolishDestroyed=" + std::to_string(count(Verdict::EligibleDestroyed)) + " wouldDemolishAbandoned=" + std::to_string(count(Verdict::EligibleAbandoned)) +
             " keepPowerWater=" + std::to_string(count(Verdict::KeepUtilityReason)) +
             " keepHistorical=" + std::to_string(count(Verdict::KeepHistorical)) + " keepFarmField=" + std::to_string(count(Verdict::KeepAgriculture)) + " keepCivicOrSpecial=" + std::to_string(count(Verdict::KeepNotGrowable)) + " tooRecent=" + std::to_string(count(Verdict::TooRecent)) +
             " blocked=" + std::to_string(count(Verdict::EligibleButBlocked)) + " transitional=" + std::to_string(count(Verdict::Transitional)) +
             " keepOptionOff=" + std::to_string(count(Verdict::KeepOptionOff)) + " keepZoneNotChosen=" + std::to_string(count(Verdict::KeepZoneOff)) +
             " keepReasonNotChosen=" + std::to_string(count(Verdict::KeepReasonOff)) + "}";
        return s;
    }
};

} // namespace lotwatch
} // namespace sc4af

class cISC4City;
namespace sc4af {
// READ ONLY. `full`: also list the relevant lots one by one (the oldest first, at most 60); otherwise only the lots that appeared or changed state / reason since the last call.
void RunLotWatch(cISC4City* city, const char* reason, bool full);
void LotWatchReset();
void LotWatchSetConfigDirectory(const std::string& directory);
void LotWatchReloadConfig();                                           // re-reads the ini after the options window wrote it; DepartmentEfficiencyDemolition.ini lives next to the DLL (missing file = Off)
} // namespace sc4af
