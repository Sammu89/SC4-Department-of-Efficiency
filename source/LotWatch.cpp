// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// LotWatch, game side: walks the lot grid and logs what the rules in LotWatch.h say; with DepartmentEfficiency.ini [LotDemolition] Mode=Auto it then demolishes the lots the rules pick, ONLY through the game's own
// `Demolish` command (cIGZCommandServer, the path sc4-auto-run-cheats uses for any command string). It writes nothing else.
//
// Runtime facts that shaped it:
//   * the game carries a `Demolish` order out a moment AFTER ExecuteCommand returned (the lot is still there on the very next read), so an order is confirmed on the NEXT scan, never right away;
//   * GetLocation() of a lot is not the top-left cell of its bounding rect (it depends on the lot's facing): the rect is validated against the lot's size and must contain the location cell;
//   * the walk of the grid is the costly part, so a lot is read with one call (its state) unless it is vacant / being demolished / destroyed.
#include "LotWatch.h"
#include "IniSections.h"

#include "Logging.h"
#include "GZServPtrs.h"
#include "SC4List.h"
#include "SC4Rect.h"
#include "cIGZCommandParameterSet.h"
#include "cIGZCommandServer.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4City.h"
#include "cISC4Demolition.h"
#include "cISC4Lot.h"
#include "cISC4LotManager.h"
#include "cISC4ZoneManager.h"
#include "cRZAutoRefCount.h"

#include <windows.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace sc4af {
namespace {

using namespace lotwatch;

constexpr float kCellMeters = 16.0f;
constexpr int kMinCells = 16, kMaxCells = 1024;     // sanity bounds of the grid walk
constexpr std::size_t kMaxLotLines = 60;           // lots that matter
constexpr std::size_t kBulkSample = 3;             // lots of the normal bulk (power / water, farm fields) listed as a sample
constexpr int kFirstRunOrders = 3;                 // until one order has been CONFIRMED in this session, a run sends at most this many

struct Row {
    int x = 0, z = 0;
    std::uint8_t w = 0, h = 0;
    Observation obs;
    Verdict verdict = Verdict::NotRelevant;
    std::uint32_t condition = 0;
};

// Lots that exist and wait for a building (state Ready): why do they stay empty? (a diagnostic of the monthly pass, read only)
struct ReadyStats {
    std::uint32_t n = 0, noRoad = 0, noJob = 0, oldest = 0;
    std::uint32_t byZone[kZoneCount] = {};
};

struct Last {
    std::uint32_t state = 0;
    int reason = 0;
};

// An order that was sent and whose result is judged on the next scan.
struct Pending {
    int x = 0, z = 0;
    std::uint32_t state = 0;
    std::string command;
    std::int64_t fundsBefore = 0;
    std::uint32_t daysInState = 0;
    int zoneBefore = -1;                              // the zone of the lot's origin cell before the order (-1 unknown)
};

std::map<std::pair<int, int>, Last> g_last;          // relevant lots only (Vacant / Destructing / Destroyed), by top-left cell
std::string g_lastSummary;
bool g_unavailableLogged = false;

DemolitionConfig g_config;
std::string g_configPath;
std::uint64_t g_configStamp = ~0ull;                 // never equal to a real stamp: the first call loads (or notes the missing file)
bool g_configRead = false;                           // the [LotDemolition] text below is what the current g_config was parsed from
std::string g_configSection;
FailureGuard g_guard;
bool g_trippedLogged = false;
std::vector<Pending> g_pending;
bool g_confirmedOnce = false;
std::uint32_t g_confirmed = 0, g_notDone = 0;        // orders confirmed / not carried out, this city session
std::uint32_t g_zoneKept = 0, g_zoneCleared = 0;     // after a demolition the zone of the lot is still there / was cleared
bool g_zoneStopLogged = false;

std::uint64_t FileWriteTime(const std::string& path) {
    WIN32_FILE_ATTRIBUTE_DATA data{};
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) return 0;
    return (static_cast<std::uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime;
}

double NowMs() {
    LARGE_INTEGER f{}, c{};
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return f.QuadPart ? 1000.0 * static_cast<double>(c.QuadPart) / static_cast<double>(f.QuadPart) : 0.0;
}

bool g_costQueryBroken = false;                       // the cost query raised or removed a lot: no demolition for this city session

// The game's own price of demolishing ONE lot: cISC4Demolition::DemolishLots with demolish = false, the bulldoze tool's preview (privilege 1, no flags, the zone kept).
// An exception means no price. The caller checks that the lot is still there afterwards.
bool SehDemolishLotsPreview(cISC4Demolition* dem, const SC4List<cISC4Lot*>* lots, std::int64_t* cost, bool& raised) {
    __try { return dem->DemolishLots(false, *lots, 1, 0, false, nullptr, cost, 0); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
}

bool PreviewCost(cISC4Demolition* dem, cISC4Lot* lot, std::int64_t& cost, std::string& problem) {
    if (!dem) { problem = "no demolition utility"; return false; }
    SC4List<cISC4Lot*> lots;
    lot->AddRef();                                          // the list releases its elements
    lots.push_back(lot);
    bool raised = false;
    cost = -1;
    const bool ok = SehDemolishLotsPreview(dem, &lots, &cost, raised);
    if (raised) { problem = "the cost preview raised an exception"; return false; }
    if (!ok || cost < 0) { problem = "the game gave no cost (returned " + std::to_string(ok ? 1 : 0) + ", cost " + std::to_string(cost) + ")"; return false; }
    return true;
}

void ResetSession() {
    g_guard.Reset();
    g_trippedLogged = false;
    g_pending.clear();
    g_confirmedOnce = false;
    g_confirmed = g_notDone = 0;
    g_zoneKept = g_zoneCleared = 0;
    g_zoneStopLogged = false;
    g_costQueryBroken = false;
}

} // namespace

bool LotDemolitionAutoOn() { return g_config.mode == DemolitionMode::Auto; }

std::string LotDemolitionStopReason() {
    if (g_config.mode != DemolitionMode::Auto) return std::string();
    if (g_zoneCleared > 0) return "a demolition cleared the zone of a lot (" + std::to_string(g_zoneCleared) + ")";
    if (g_guard.tripped()) return "the game did not carry out " + std::to_string(FailureGuard::kMaxConsecutive) + " confirmed demolition orders in a row";
    if (g_costQueryBroken) return "the cost preview of a lot changed the lot";
    return std::string();
}

namespace {

void LoadConfigIfChanged() {
    if (g_configPath.empty()) return;
    const auto stamp = FileWriteTime(g_configPath);
    if (stamp == g_configStamp) return;
    g_configStamp = stamp;
    std::string section;
    if (stamp != 0) {
        std::ifstream in(g_configPath, std::ios::binary);
        std::ostringstream buffer;
        buffer << in.rdbuf();
        section = IniSectionText(buffer.str(), kIniSecDemolition);
    }
    if (g_configRead && section == g_configSection) return;      // the file changed for another module (health, utilities): the demolition session goes on
    g_configRead = true;
    g_configSection = section;
    g_config = DemolitionConfig{};                    // Mode=Off unless the file says otherwise and is valid
    ResetSession();
    if (stamp == 0) { Log(LogLevel::Info, "[LotDemolish] no DepartmentEfficiency.ini: Mode=OFF (the lots are only observed)"); return; }
    std::string error;
    if (!ParseDemolitionConfig(section, g_config, error)) {
        g_config = DemolitionConfig{};
        Log(LogLevel::Warning, "[LotDemolish] DepartmentEfficiency.ini [LotDemolition] invalid (" + error + "): Mode=OFF");
        return;
    }
    const Rules& r = g_config.rules;
    Log(LogLevel::Info, std::string("[LotDemolish] config loaded: Mode=") + ModeName(g_config.mode) + " destroyed=" + (r.destroyedOn ? "on" : "off") + " after " +
                            std::to_string(r.destroyedAfterDays) + "d municipalAndHistoricRuins=" + (r.demolishMunicipalRuins ? "DEMOLISH" : "protected") + " | abandoned=" + (r.abandonedOn ? "on" : "off") +
                            " after " + std::to_string(r.abandonedAfterDays) + "d zones=" +
                            NameList(r.abandonedZones, false) + " reasons=" + NameList(r.abandonedReasons, true) + " | monthly limit=" + (g_config.limitMonthlyBudget ? std::to_string(g_config.monthlyBudget) : std::string("none")) +
                            " (never more than the treasury) maxPerRun=" + std::to_string(g_config.maxPerRun) + " (historical and municipal lots: only their ruins, and only if chosen)");
}

std::string Line(const Row& r) {
    return "[LotWatch][Lot] cell=" + std::to_string(r.x) + "," + std::to_string(r.z) + " size=" + std::to_string(r.w) + "x" + std::to_string(r.h) + " state=" + StateName(r.obs.state) +
           " daysInState=" + std::to_string(r.obs.daysSinceChange) + " reason=" + ReasonName(r.obs.vacateReason) + " zone=" + ZoneName(r.obs.zone) + " condition=" + std::to_string(r.condition) +
           " historical=" + (r.obs.historical ? "yes" : "no") + " demolishable=" + (r.obs.demolishable ? "yes" : "no") + " verdict=" + VerdictName(r.verdict);
}

bool Relevant(std::uint32_t state) { return state == kVacant || state == kDestructing || state == kDestroyed; }

// One call for the common case: only a lot that can matter is read in full.
Observation Read(cISC4Lot* lot) {
    Observation o;
    o.state = static_cast<std::uint32_t>(lot->GetState());
    if (!Relevant(o.state)) return o;
    o.daysSinceChange = lot->GetDaysSinceLastStateChange();
    o.vacateReason = static_cast<int>(lot->GetVacateReason());
    o.historical = lot->IsHistorical();
    o.zone = static_cast<int>(lot->GetZoneType());
    o.demolishable = lot->IsInDemolishableState();
    return o;
}

// One order to the game: `Demolish x1 z1 x2 z2 None`. True when the command was understood and handed to the game; the game carries it out a moment later.
bool SendDemolishCommand(const std::string& command, std::int32_t& status, std::string& problem) {
    cIGZCommandServerPtr server;
    if (!server) { problem = "the game's command server is unavailable"; return false; }
    std::uint32_t id = 0;
    cIGZCommandParameterSet* input = nullptr;
    if (!server->ConvertStringToCommand(command.c_str(), static_cast<std::uint32_t>(command.size()), id, input)) {
        problem = "the game did not recognise the command";
        return false;
    }
    bool sent = false;
    cRZAutoRefCount<cIGZCommandParameterSet> output;
    if (server->CreateCommandParameterSet(output.AsPPObj())) {
        status = server->ExecuteCommand(id, input, output);
        sent = true;
    } else {
        problem = "no output parameter set could be created";
    }
    if (input) input->Release();
    return sent;
}

// The orders of the previous scan: each lot must be gone (or no longer the same lot in the same state).
void ResolvePending(cISC4City* city, cISC4LotManager* lots) {
    if (g_pending.empty()) return;
    cISC4BudgetSimulator* budget = city->GetBudgetSimulator();
    cISC4ZoneManager* zones = city->GetZoneManager();
    const std::int64_t funds = budget ? budget->GetTotalFunds() : 0;
    int done = 0, notDone = 0, zoneKept = 0, zoneCleared = 0;
    for (const auto& p : g_pending) {
        cISC4Lot* lot = lots->GetLot(p.x, p.z, false);
        int ax = 0, az = 0;
        const bool stillThere = lot && lot->GetLocation(ax, az) && ax == p.x && az == p.z && static_cast<std::uint32_t>(lot->GetState()) == p.state;
        g_guard.Record(!stillThere);
        if (!stillThere) {
            ++done; ++g_confirmed; g_confirmedOnce = true;
            // the demolition must leave the ZONE alone (the land stays zoned and grows again); if it clears it, the player loses zoning: say so and stop
            if (zones && p.zoneBefore > 0) {
                const int after = static_cast<int>(zones->GetZoneType(p.x, p.z));
                if (after == 0) { ++zoneCleared; ++g_zoneCleared; Log(LogLevel::Warning, "[LotDemolish] ZONE CLEARED at cell=" + std::to_string(p.x) + "," + std::to_string(p.z) + ": " + ZoneName(p.zoneBefore) + " -> none"); }
                else { ++zoneKept; ++g_zoneKept; }
            }
        }
        else { ++notDone; ++g_notDone; Log(LogLevel::Warning, "[LotDemolish] NOT DONE cell=" + std::to_string(p.x) + "," + std::to_string(p.z) + " is still there after " + p.command); }
    }
    Log(notDone ? LogLevel::Warning : LogLevel::Info, "[LotDemolish] confirmed " + std::to_string(done) + " of " + std::to_string(g_pending.size()) + " order(s) of the previous run (" + std::to_string(notDone) +
                                                          " still there); zone kept " + std::to_string(zoneKept) + " cleared " + std::to_string(zoneCleared) + "; this session: " + std::to_string(g_confirmed) + " confirmed, " + std::to_string(g_notDone) + " not done, zone cleared " + std::to_string(g_zoneCleared) + "; treasury now " + std::to_string(funds));
    g_pending.clear();
}

void RunDemolitions(cISC4City* city, cISC4LotManager* lots, std::vector<Row> candidates, const char* reason) {
    // the destroyed first (rubble), then the oldest abandoned
    std::sort(candidates.begin(), candidates.end(), [](const Row& a, const Row& b) {
        const bool ad = a.verdict == Verdict::EligibleDestroyed, bd = b.verdict == Verdict::EligibleDestroyed;
        return ad != bd ? ad : a.obs.daysSinceChange > b.obs.daysSinceChange;
    });
    std::size_t limit = static_cast<std::size_t>(g_config.maxPerRun);
    if (!g_confirmedOnce) limit = std::min<std::size_t>(limit, kFirstRunOrders);       // prove the route with a few orders before the bulk
    if (g_config.mode == DemolitionMode::Shadow) {
        Log(LogLevel::Info, std::string("[LotDemolish] ") + reason + " SHADOW: " + std::to_string(std::min<std::size_t>(candidates.size(), g_config.maxPerRun)) + " lot(s) would be demolished now (Mode=Auto does it)");
        return;
    }
    if (g_zoneCleared > 0) {
        if (!g_zoneStopLogged) { g_zoneStopLogged = true; Log(LogLevel::Warning, "[LotDemolish] STOPPED for this city session: a demolition cleared the zone of a lot (" + std::to_string(g_zoneCleared) + "); the owner keeps the zoning"); }
        return;
    }
    if (g_guard.tripped()) {
        if (!g_trippedLogged) { g_trippedLogged = true; Log(LogLevel::Warning, "[LotDemolish] STOPPED for this city session: the game did not carry out " + std::to_string(FailureGuard::kMaxConsecutive) + " confirmed orders in a row"); }
        return;
    }
    cISC4BudgetSimulator* budget = city->GetBudgetSimulator();
    if (!budget) { Log(LogLevel::Warning, "[LotDemolish] the budget simulator is unavailable: no demolition (the treasury cannot be checked)"); return; }
    const std::int64_t fundsBefore = budget->GetTotalFunds();
    if (fundsBefore < g_config.minFunds) {
        Log(LogLevel::Info, "[LotDemolish] " + std::string(reason) + " funds " + std::to_string(fundsBefore) + " < MinFunds " + std::to_string(g_config.minFunds) + ": " + std::to_string(candidates.size()) + " lot(s) wait");
        return;
    }
    if (g_costQueryBroken) return;
    cISC4ZoneManager* zones = city->GetZoneManager();
    cISC4Demolition* dem = city->GetDemolitionUtility();
    std::size_t sent = 0, skipped = 0;
    std::int64_t spent = 0;                                  // the price of the orders sent in this run (the monthly run: the month's spending)
    for (const auto& c : candidates) {
        if (sent >= limit) break;
        // the lot is read again right before the order: it must still be the lot the rules picked
        cISC4Lot* lot = lots->GetLot(c.x, c.z, false);
        int lx = 0, lz = 0;
        if (!lot || !lot->GetLocation(lx, lz) || lx != c.x || lz != c.z) { ++skipped; continue; }
        const Observation now = Read(lot);
        const Verdict verdict = Classify(now, g_config.rules);
        if (!WouldDemolish(verdict)) { ++skipped; Log(LogLevel::Info, "[LotDemolish] skipped cell=" + std::to_string(c.x) + "," + std::to_string(c.z) + ": no longer a candidate (" + VerdictName(verdict) + ")"); continue; }
        SC4Rect<std::int32_t> bounds;
        std::uint8_t w = 0, h = 0;
        CellRect rect;
        // GetLocation() is the lot's origin cell, which is one of the four corners of its bounding rect (it depends on the lot's facing): the rect must fit the lot's size and contain that cell
        if (!lot->GetBoundingRect(bounds) || !lot->GetSize(w, h) || !ResolveInclusiveRect(bounds.topLeftX, bounds.topLeftY, bounds.bottomRightX, bounds.bottomRightY, w, h, rect) ||
            c.x < rect.x1 || c.x > rect.x2 || c.z < rect.z1 || c.z > rect.z2) {
            ++skipped;
            Log(LogLevel::Warning, "[LotDemolish] skipped cell=" + std::to_string(c.x) + "," + std::to_string(c.z) + ": the lot's rect (" + std::to_string(bounds.topLeftX) + "," + std::to_string(bounds.topLeftY) + " .. " +
                                       std::to_string(bounds.bottomRightX) + "," + std::to_string(bounds.bottomRightY) + ") does not fit its size " + std::to_string(w) + "x" + std::to_string(h));
            continue;
        }
        // the money: the game's own price of THIS lot, known before the order; never more than the treasury, never below MinFunds, never past the month's budget
        std::int64_t cost = -1;
        std::string why;
        const bool priced = PreviewCost(dem, lot, cost, why);
        {
            cISC4Lot* again = lots->GetLot(c.x, c.z, false);
            int ax = 0, az = 0;
            if (!again || !again->GetLocation(ax, az) || ax != c.x || az != c.z || static_cast<std::uint32_t>(again->GetState()) != now.state) {
                g_costQueryBroken = true;                    // the preview changed the lot: it is not a preview, stop
                Log(LogLevel::Warning, "[LotDemolish] STOPPED for this city session: the cost preview of cell=" + std::to_string(c.x) + "," + std::to_string(c.z) + " changed the lot");
                break;
            }
        }
        const Spend spend = SpendDecision(priced, cost, fundsBefore - spent, g_config.minFunds, spent, g_config.BudgetLimit());
        if (spend != Spend::Ok) {
            ++skipped;
            Log(LogLevel::Info, "[LotDemolish] not ordered cell=" + std::to_string(c.x) + "," + std::to_string(c.z) + ": " + SpendName(spend) + " (cost " + std::to_string(cost) + (priced ? "" : ", " + why) +
                                    ", treasury " + std::to_string(fundsBefore - spent) + ", spent " + std::to_string(spent) + " this month, limit " +
                                    (g_config.limitMonthlyBudget ? std::to_string(g_config.monthlyBudget) : std::string("none")) + ")");
            if (spend == Spend::OverBudget) break;          // the month's budget is used: the rest waits for next month
            continue;
        }
        const std::string command = DemolishCommand(rect);
        const int zoneBefore = zones ? static_cast<int>(zones->GetZoneType(c.x, c.z)) : -1;
        Log(LogLevel::Info, "[LotDemolish] ORDER " + command + "   lot cell=" + std::to_string(c.x) + "," + std::to_string(c.z) + " " + StateName(now.state) + " " + std::to_string(now.daysSinceChange) + "d reason=" +
                                ReasonName(now.vacateReason) + " zone=" + ZoneName(now.zone) + " size=" + std::to_string(w) + "x" + std::to_string(h));
        std::int32_t status = 0;
        std::string problem;
        if (SendDemolishCommand(command, status, problem)) {
            g_pending.push_back(Pending{c.x, c.z, static_cast<std::uint32_t>(now.state), command, budget->GetTotalFunds(), now.daysSinceChange, zoneBefore});
            ++sent;
            spent += cost;
        } else {
            ++skipped;
            g_guard.Record(false);
            Log(LogLevel::Warning, "[LotDemolish] NOT SENT cell=" + std::to_string(c.x) + "," + std::to_string(c.z) + ": " + problem);
            if (g_guard.tripped()) break;
        }
    }
    Log(LogLevel::Info, std::string("[LotDemolish] ") + reason + " AUTO run: candidates=" + std::to_string(candidates.size()) + " sent=" + std::to_string(sent) + " skipped=" + std::to_string(skipped) +
                            " price of the orders " + std::to_string(spent) + " (monthly limit " + (g_config.limitMonthlyBudget ? std::to_string(g_config.monthlyBudget) : std::string("none")) + ")" +
                            (g_confirmedOnce ? "" : " (first run of the session: a few orders only, the result is checked on the next scan)") + " treasury " + std::to_string(fundsBefore) + " -> " +
                            std::to_string(budget->GetTotalFunds()) + " (the game carries each order out a moment later; the result is confirmed on the next scan)");
}

} // namespace

void LotWatchReset() {
    g_last.clear();
    g_lastSummary.clear();
    g_unavailableLogged = false;
    ResetSession();
}

void LotWatchSetConfigDirectory(const std::string& directory) {
    g_configPath = directory + "DepartmentEfficiency.ini";      // the one file; this module reads its [LotDemolition] section
    g_configStamp = ~0ull;
    g_configRead = false;
    LoadConfigIfChanged();
}

void LotWatchReloadConfig() { LoadConfigIfChanged(); }

void RunLotWatch(cISC4City* city, const char* reason, bool full) {
    if (!city) return;
    try {
        LoadConfigIfChanged();
        cISC4LotManager* lots = city->GetLotManager();
        if (!lots) {
            if (!g_unavailableLogged) { g_unavailableLogged = true; Log(LogLevel::Warning, "[LotWatch] the lot manager is unavailable: nothing observed"); }
            return;
        }
        ResolvePending(city, lots);                       // what the previous run ordered
        const double t0 = NowMs();
        const Rules& rules = g_config.rules;
        const int cellsX = std::clamp(static_cast<int>(city->SizeX() / kCellMeters + 0.5f), kMinCells, kMaxCells);
        const int cellsZ = std::clamp(static_cast<int>(city->SizeZ() / kCellMeters + 0.5f), kMinCells, kMaxCells);
        Summary summary;
        ReadyStats ready;
        std::vector<Row> relevant;
        cISC4ZoneManager* zoneManager = full ? city->GetZoneManager() : nullptr;
        std::uint32_t zonedNoLot[kZoneCount] = {};          // zoned cells that have no lot at all: the land the player can still see as "to build on"
        for (int z = 0; z < cellsZ; ++z) {
            for (int x = 0; x < cellsX; ++x) {
                cISC4Lot* lot = lots->GetLot(x, z, false);
                if (!lot) {
                    if (zoneManager) {
                        const int zone = static_cast<int>(zoneManager->GetZoneType(x, z));
                        if (zone > 0 && zone < kZoneCount) ++zonedNoLot[zone];
                    }
                    continue;
                }
                int lx = 0, lz = 0;
                if (!lot->GetLocation(lx, lz) || lx != x || lz != z) continue;       // a lot is read once, from its origin cell
                Row r;
                r.x = x;
                r.z = z;
                r.obs = Read(lot);
                if (full && r.obs.state == kReady) {
                    ++ready.n;
                    const int zone = static_cast<int>(lot->GetZoneType());
                    if (zone >= 0 && zone < kZoneCount) ++ready.byZone[zone];
                    if (!lot->HasRoadAccess()) ++ready.noRoad;
                    if (!lot->HasJobAccess()) ++ready.noJob;
                    ready.oldest = std::max(ready.oldest, static_cast<std::uint32_t>(lot->GetDaysSinceLastStateChange()));
                }
                r.verdict = Classify(r.obs, rules);
                summary.Add(r.obs, r.verdict);
                if (r.verdict != Verdict::NotRelevant) {
                    lot->GetSize(r.w, r.h);
                    r.condition = lot->GetCondition();
                    relevant.push_back(r);
                }
            }
        }
        const double scanMs = NowMs() - t0;
        const std::string text = summary.Format();
        if (full || text != g_lastSummary) {
            Log(LogLevel::Info, std::string("[LotWatch] ") + reason + " " + text + " rules{destroyed>=" + std::to_string(rules.destroyedAfterDays) + "d abandoned>=" + std::to_string(rules.abandonedAfterDays) +
                                "d} demolition=" + ModeName(g_config.mode) + " scanMs=" + std::to_string(static_cast<int>(scanMs)));
            g_lastSummary = text;
        }
        if (full) {
            std::string zonesText;
            for (int i = 0; i < kZoneCount; ++i)
                if (ready.byZone[i]) zonesText += std::string(zonesText.empty() ? "" : " ") + ZoneName(i) + "=" + std::to_string(ready.byZone[i]);
            Log(LogLevel::Info, "[LotWatch][Ready] lots waiting for a building: n=" + std::to_string(ready.n) + " withoutRoadAccess=" + std::to_string(ready.noRoad) + " withoutJobAccess=" + std::to_string(ready.noJob) +
                                    " oldestDays=" + std::to_string(ready.oldest) + " byZone{" + zonesText + "}");
            if (cISC4ZoneManager* zm = city->GetZoneManager()) {
                std::string text;
                for (int t = 1; t <= 9; ++t) {
                    const auto type = static_cast<cISC4ZoneManager::ZoneType>(t);
                    const int zoned = zm->GetZoneCount(type);
                    if (zoned <= 0) continue;
                    // (GetUndevelopedTileCount answers 65536 for every type in this game version: not usable. The cells with no lot are counted from the grid instead.)
                    text += std::string(text.empty() ? "" : " ") + ZoneName(t) + "{zonedTiles=" + std::to_string(zoned) + " withNoLot=" + std::to_string(zonedNoLot[t]) + "}";
                }
                Log(LogLevel::Info, "[LotWatch][Zones] zoned tiles, and the zoned cells that have no lot at all: " + text);
            }
        }
        std::vector<Row> toList;
        std::map<std::pair<int, int>, Last> now;
        for (const auto& r : relevant) {
            const auto key = std::make_pair(r.x, r.z);
            now[key] = Last{r.obs.state, r.obs.vacateReason};
            const auto it = g_last.find(key);
            const bool changed = it == g_last.end() || it->second.state != r.obs.state || it->second.reason != r.obs.vacateReason;
            if (full || changed) toList.push_back(r);
        }
        // what matters first (lots the rules would demolish, unknown reasons, ...), the oldest first inside each class; the bulk that is normal (power / water, farm fields) only as a short sample
        std::sort(toList.begin(), toList.end(), [](const Row& a, const Row& b) {
            const int pa = ListPriority(a.verdict), pb = ListPriority(b.verdict);
            return pa != pb ? pa < pb : a.obs.daysSinceChange > b.obs.daysSinceChange;
        });
        std::size_t listed = 0, skipped = 0, bulk[2] = {0, 0};
        for (const auto& r : toList) {
            const bool isBulk = r.verdict == Verdict::KeepUtilityReason || r.verdict == Verdict::KeepAgriculture;
            if (isBulk) {
                auto& k = bulk[r.verdict == Verdict::KeepAgriculture ? 1 : 0];
                if (++k > kBulkSample) { ++skipped; continue; }
            } else if (listed >= kMaxLotLines) { ++skipped; continue; }
            Log(LogLevel::Info, Line(r));
            if (!isBulk) ++listed;
        }
        if (skipped) Log(LogLevel::Info, "[LotWatch] " + std::to_string(skipped) + " more relevant lot(s) not listed (the counts above are complete; power / water and farm-field lots are shown as a sample of " + std::to_string(kBulkSample) + " each)");
        g_last = std::move(now);

        // The demolition pass: once a month (the rules count days, a month is a fine cadence), never at the city's first read.
        if (g_config.mode != DemolitionMode::Off && std::strcmp(reason, "SimNewMonth") == 0) {
            std::vector<Row> candidates;
            for (const auto& r : relevant)
                if (WouldDemolish(r.verdict)) candidates.push_back(r);
            if (!candidates.empty()) RunDemolitions(city, lots, std::move(candidates), reason);
        }
    } catch (const std::exception& e) {
        Log(LogLevel::Warning, std::string("[LotWatch] observation failed: ") + e.what());
    } catch (...) {
        Log(LogLevel::Warning, "[LotWatch] observation failed with an unknown exception");
    }
}

} // namespace sc4af
