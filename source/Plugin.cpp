// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "BuildFeatures.h"
#ifndef SC4AF_GIT_SHA
#define SC4AF_GIT_SHA "unknown"
#endif
#define SC4AF_GIT_SHA_STR SC4AF_GIT_SHA
#ifndef SC4AF_VERSION
#define SC4AF_VERSION "0.0.0"      // the real value comes from project(... VERSION ...) in CMakeLists.txt (compile definition of this file)
#endif
#include "AutoFinance.h"
#include "AdviceSwitches.h"
#include "ServiceNotices.h"
#include "NoticeMemory.h"
#include "CivicBuildingNotices.h"
#include "FailureNotice.h"
#include <fstream>
#include <sstream>
#include "CivicFundingWriter.h"
#include "CivicInventory.h"
#include "CivicProbe.h"
#include "OmniProbe.h"
#include "LotWatch.h"
#include "FarmWaterZot.h"
#include "UtilityActuator.h"
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
#include "UnifiedActuator.h"
#include "UnifiedAuto.h"
#include "TimedActuator.h"
#include "NativeNotice.h"
#include "OptionsUI.h"
#include "MinistryOrdinance.h"
#endif
#include "PowerTrace.h"
#include "Phase8RuntimeProbe.h"
#ifdef SC4AF_LANDFILL_RESEARCH
#include "LandfillResearch.h"
#endif
#include "LandfillZoneReader.h"
#include "UtilityProbe.h"
#include "UtilityShadow.h"
#include "UtilityWriter.h"
#include "GameVersion.h"
#include "Logging.h"

#include "GZCLSIDDefs.h"
#include "GZServPtrs.h"
#include "cIGZApp.h"
#include "cIGZCOM.h"
#include "cIGZFrameWork.h"
#include "cIGZMessage2Standard.h"
#include "cIGZMessageServer2.h"
#include "cISC4Advice.h"
#include "cISC4AdviceSubject.h"
#include "cISC4Advisor.h"
#include "cISC4AdvisorSystem.h"
#include "cRZBaseString.h"
#include "cISC4App.h"
#include "cISC4City.h"
#include "cISC4Region.h"
#include "SC4String.h"
#include "cISC4RegionalCity.h"
#include "cRZAutoRefCount.h"
#include "cRZMessage2COMDirector.h"

#include <windows.h>
#include <cstdio>
#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <vector>
#include <exception>
#include <string>

static_assert(sizeof(void*) == 4, "Department of Efficiency must be built for Win32/x86");

namespace {
HMODULE gModule = nullptr;

// Must be unique among GZCOM DLL directors. Assigned for this project only.
constexpr std::uint32_t kDirectorId = 0x6B27B852;
constexpr std::uint32_t kPostCityInit = 0x26D31EC1;
constexpr std::uint32_t kPostCityShutdown = 0x26D31EC3;
constexpr std::uint32_t kSimNewMonth = 0x66956816;
constexpr std::uint32_t kSimNewDay = 0x66956814;
constexpr std::uint32_t kGarbageScalingRecompute = 0x66956810;
constexpr std::uint32_t kNeighborDealEvent = 0xEA418D8F;
// The Department ordinance never reaches a city file (MinistryOrdinance.h): out before every save, back after it, out before the city closes.
constexpr std::uint32_t kPreSave = 0x26C63343;           // kSC4MessagePreSave (cSC4App::SaveCity; the id other published plugins use)
constexpr std::uint32_t kPostSave = 0x26C63345;          // kSC4MessagePostSave
constexpr std::uint32_t kPreCityShutdown = 0x26D31EC2;
constexpr std::array<std::uint32_t, 7> kMessages{
    kPostCityInit, kPostCityShutdown, kSimNewMonth, kSimNewDay, kPreSave, kPostSave, kPreCityShutdown
};
// Occupant insert/remove notifications (IDs used by many subscribers inside the
// 1.1.641.0 executable). Optional and payload-agnostic: the handler only raises a
// dirty flag and never dereferences the message data. Failure to subscribe is
// harmless because the count poll and the monthly pass still detect changes.
constexpr std::uint32_t kInsertOccupant = 0x99EF1142;
constexpr std::uint32_t kRemoveOccupant = 0x99EF1143;
constexpr UINT kPollIntervalMs = 1000;
constexpr UINT kUiPollIntervalMs = 250;      // OptionsUI: the button must be in Play Options as soon as the game shows it

class AutoFinanceDirector;
AutoFinanceDirector* gDirector = nullptr;
VOID CALLBACK PollTimerThunk(HWND, UINT, UINT_PTR, DWORD);
VOID CALLBACK UiTimerThunk(HWND, UINT, UINT_PTR, DWORD);

class AutoFinanceDirector final : public cRZMessage2COMDirector {
public:
    std::uint32_t GetDirectorID() const override { return kDirectorId; }

    // The SDK's cRZMessage2COMDirector fallback recursively calls itself at
    // the pinned revision. Route non-message IIDs to its real base instead.
    bool QueryInterface(std::uint32_t iid, void** result) override {
        if (!result) return false;
        *result = nullptr;
        if (iid == GZCLSID::kcIGZMessageTarget2) {
            *result = static_cast<cIGZMessageTarget2*>(this);
            AddRef();
            return true;
        }
        return cRZCOMDllDirector::QueryInterface(iid, result);
    }

    // <dir of DepartmentEfficiency.dll>\ (trailing backslash), from the real module path; empty if the path is unknown. The DLL, the DAT and the INI are installed side by side, directly in Plugins:
    // there is NO subfolder and nothing is looked up in a former location (no fallback to the folders of earlier versions).
    static std::string DataDirectory() {
        wchar_t wide[MAX_PATH] = {};
        const DWORD n = GetModuleFileNameW(gModule, wide, MAX_PATH);
        if (n == 0 || n >= MAX_PATH) return {};
        std::wstring dir(wide, n);
        const auto slash = dir.find_last_of(L"\\/");
        if (slash == std::wstring::npos) return {};
        dir.resize(slash + 1);
        // the rest of the plugin opens files through the ANSI API: a path the ANSI code page cannot hold goes through its 8.3 form
        BOOL lossy = FALSE;
        auto narrow = [&](const std::wstring& w) {
            const int len = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, w.c_str(), -1, nullptr, 0, nullptr, &lossy);
            std::string out(len > 0 ? len - 1 : 0, '\0');
            if (len > 1) WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, w.c_str(), -1, out.data(), len, nullptr, &lossy);
            return out;
        };
        std::string result = narrow(dir);
        if (!lossy) return result;
        wchar_t shortPath[MAX_PATH] = {};
        const DWORD m = GetShortPathNameW(dir.c_str(), shortPath, MAX_PATH);
        if (m == 0 || m >= MAX_PATH) return result;
        lossy = FALSE;
        result = narrow(std::wstring(shortPath, m));
        return lossy ? std::string() : result;
    }

    static bool FileExists(const std::string& path) {
        const DWORD a = GetFileAttributesA(path.c_str());
        return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool OnStart(cIGZCOM* com) override {
        dataDir_ = DataDirectory();
        sc4af::InitLog(gModule);
        sc4af::Log(sc4af::LogLevel::Info, "Plugin loaded: version " SC4AF_VERSION " commit=" SC4AF_GIT_SHA_STR "; what the Department manages (education and health funding, power, water and sanitation funding, regional contracts, lot demolition) is set by DepartmentEfficiency.ini and logged below once the configuration is read; the observation probes only read");
#ifdef SC4AF_ENABLE_FINAL_AUTO_VALIDATION
        sc4af::Log(sc4af::LogLevel::Warning, "[FinalValidation] BUILD=FINAL_VALIDATION (unified Power/Water/Sanitation/Deals Auto is compiled in; NOT a release binary)");
#endif
        sc4af::Log(sc4af::LogLevel::Info,
                   "Dependency: gzcom-dll 779b669bf88112146d58fba6c7ed44f13ac6ce7d");
#ifdef NDEBUG
        sc4af::Log(sc4af::LogLevel::Info, "Build=Release Architecture=x86 Target=Steam 1.1.641.0");
#else
        sc4af::Log(sc4af::LogLevel::Info, "Build=Debug Architecture=x86 Target=Steam 1.1.641.0");
#endif
        const auto version = sc4af::ReadExecutableVersion();
        if (!version) {
            sc4af::Log(sc4af::LogLevel::Error,
                       "GameVersion=UNKNOWN Supported=NO; all civic subscriptions disabled");
            return true;
        }
        sc4af::Log(sc4af::LogLevel::Info,
                   "GameVersion=" + sc4af::FormatGameVersion(*version) +
                   (sc4af::IsSupportedGameVersion(*version) ? " Supported=YES" : " Supported=NO"));
        if (!sc4af::IsSupportedGameVersion(*version)) return true;
        // A folder of an earlier version is never read: say so once, so that a copy laid over an old installation is not mistaken for a working one.
        if (!dataDir_.empty()) {
            const DWORD legacyAttr = GetFileAttributesA((dataDir_ + "Department of Efficiency").c_str());
            if (legacyAttr != INVALID_FILE_ATTRIBUTES && (legacyAttr & FILE_ATTRIBUTE_DIRECTORY))
                sc4af::Log(sc4af::LogLevel::Warning, "the folder \"" + dataDir_ + "Department of Efficiency\" of an earlier version is present: nothing in it is read any more (the INI and the DAT are DepartmentEfficiency.ini and DepartmentEfficiency.dat next to the DLL). Delete the old files of the Department and install the three files again.");
        }
        // Fail-safe: without the main resource file next to the DLL the plugin stays inert (nothing subscribed, no timer, no write), rather than run partially.
        if (dataDir_.empty() || !FileExists(dataDir_ + "DepartmentEfficiency.dat")) {
            sc4af::Log(sc4af::LogLevel::Error,
                       dataDir_.empty() ? std::string("the plugin folder could not be determined; the plugin stays inert (read-only, nothing is managed)")
                                        : "\"" + dataDir_ + "DepartmentEfficiency.dat\" is missing; the plugin stays inert (read-only, nothing is managed). Install the three files (DepartmentEfficiency.dll, .dat and .ini) together in Plugins.");
            return true;
        }
        supported_ = true;
        if (!com || !com->FrameWork()) {
            sc4af::Log(sc4af::LogLevel::Error, "Framework unavailable; probe disabled");
            supported_ = false;
            return true;
        }
        auto* framework = com->FrameWork();
        if (framework->GetState() < cIGZFrameWork::kStatePostAppInit) {
            if (!framework->AddHook(this)) {
                sc4af::Log(sc4af::LogLevel::Error, "Failed to register framework hook");
                supported_ = false;
            }
        } else {
            PostAppInit();
        }
        return true;
    }

    bool PostAppInit() override {
        if (!supported_ || subscribed_) return true;
        cIGZMessageServer2Ptr messages;
        if (!messages) {
            sc4af::Log(sc4af::LogLevel::Error, "MessageServer2 unavailable; probe disabled");
            return true;
        }
        std::size_t added = 0;
        for (const auto id : kMessages) {
            if (!messages->AddNotification(this, id)) {
                for (std::size_t i = 0; i < added; ++i)
                    messages->RemoveNotification(this, kMessages[i]);
                sc4af::Log(sc4af::LogLevel::Error, "City lifecycle subscription failed");
                return true;
            }
            ++added;
        }
        subscribed_ = true;
        sc4af::Log(sc4af::LogLevel::Info,
                   "Subscribed: PostCityInit, PostCityShutdown, SimNewMonth, SimNewDay");
        occupantMessages_ = messages->AddNotification(this, kInsertOccupant) &&
                            messages->AddNotification(this, kRemoveOccupant);
        if (!occupantMessages_) {
            messages->RemoveNotification(this, kInsertOccupant);
            messages->RemoveNotification(this, kRemoveOccupant);
        }
        sc4af::Log(sc4af::LogLevel::Info,
                   std::string("Occupant insert/remove notifications: ") +
                   (occupantMessages_ ? "subscribed" : "unavailable (poll + monthly only)"));
        // SetTimer with no window delivers WM_TIMER to this (the game's main) thread,
        // between frames, so city state is read on the same thread as the simulation.
        {
                const std::string& dir = dataDir_;
                writer_.SetConfigDirectory(dir);
                autoFinance_.SetConfigDirectory(dir);
                sc4af::LotWatchSetConfigDirectory(dir);             // DepartmentEfficiency.ini [LotDemolition] (missing = the lots are only observed)
                toldPath_ = dir + "DepartmentEfficiency.told.txt";
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
                LoadTold();
                // Landfill Size Savings Advisor (LandfillAdvisor.h): the controller only calls back; the notice and the told.txt record belong to the plugin.
                unifiedAuto_->landfillAdvisor().push = [this](const sc4af::LandfillNotice& n) { return PushLandfillAdvice(n); };
                unifiedAuto_->landfillAdvisor().persist = [this](const sc4af::LandfillAdviceRecord& r) { PersistLandfillRecord(r); };
                // the spatial landfill view (LandfillZoneReader.h): read-only, version-gated, cross-checked against the game's totals by the pure code
                unifiedAuto_->landfillSpatialRead = [this]() { return sc4af::ReadLandfillSpatial(CityNow()); };
                unifiedAuto_->landfillNativeCheck = [this](const sc4af::LandfillPlacement& pl) { return sc4af::LandfillNativeCheck(CityNow(), pl); };
                // ghost squares: repaired by the native re-zoning whenever the Department manages sanitation. The game restores the ghosts when the city is loaded
                // again, so the repair is redone in every session and a handled state is remembered for the session only
                unifiedAuto_->landfillGhosts().repair = [this](const sc4af::LandfillPlacement& pl) {
                    std::string problem;
                    const bool ok = sc4af::LandfillRezoneRegion(CityNow(), pl, problem);
                    sc4af::Log(ok ? sc4af::LogLevel::Info : sc4af::LogLevel::Warning, std::string("[LandfillGhosts] native re-zoning of ") + std::to_string(pl.tiles.size()) + " landfill squares " +
                               (ok ? "returned success" : "FAILED: " + problem));
                    return ok;
                };

#endif
                autoFinance_.LoadConfigIfChanged();
                sc4af::Log(sc4af::LogLevel::Info, autoFinance_.Banner());
                utility_.SetConfigDirectory(dir);
                utilityWriter_.SetConfigDirectory(dir);
                utility_.LoadConfigIfChanged();
                // the options window: Accept writes the two ini files, then the plugin reads them at once (no wait for the next day / month)
                optionsUi_.SetConfigDirectory(dir);
                ordinance_.SetOnPlayerToggle([this](bool on) {                                 // the player enacted / repealed the Department ordinance
                    optionsUi_.SetMaster(on);
                    autoFinance_.LoadConfigIfChanged();                        // SetMaster rewrote the INI: read it now, and the game's advice follows at once
                    utility_.LoadConfigIfChanged();
                    RefreshAdviceSwitches(false, "ordinance");
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
                    TickNotices();                                             // a utility switched off withdraws its notice at once, whether or not the game is paused
#endif
                });
                optionsUi_.SetOnApplied([this, dir] {
                    autoFinance_.LoadConfigIfChanged();
                    utility_.LoadConfigIfChanged();
                    sc4af::LotWatchReloadConfig();
                    UpdatePhase8Subscriptions();
                    RefreshAdviceSwitches(false, "options applied");                    // anything switched in the window: the game's advice follows at once
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
                    TickNotices();                                             // the same: no wait for a simulated day that a paused game does not run
#endif
                    ordinance_.FollowSwitch(CurrentCity(), optionsUi_.MasterIsOn());   // the Ordinances window shows the same switch
                    sc4af::Log(sc4af::LogLevel::Info, "[OptionsUI] options applied: " + autoFinance_.Banner() + " | " + utility_.Banner());
                });
                UpdatePhase8Subscriptions();
                sc4af::Log(sc4af::LogLevel::Info, std::string("Department of Efficiency " SC4AF_VERSION " | CivicAutoFinance=") + sc4af::AutoModeName(autoFinance_.config().mode) + " | " + utility_.Banner());
                LogFinalReadiness();
        }
        sc4af::farm_water_zot::Install();                           // no "no water" icon over agricultural lots (version-gated byte check; refuses on any mismatch)
        gDirector = this;
        pollTimer_ = SetTimer(nullptr, 0, kPollIntervalMs, PollTimerThunk);
        uiTimer_ = SetTimer(nullptr, 0, kUiPollIntervalMs, UiTimerThunk);
        sc4af::Log(sc4af::LogLevel::Info,
                   pollTimer_ ? "Live refresh poll timer started (1 s, main thread)"
                              : "Live refresh poll timer unavailable; monthly pass only");
        return true;
    }

    // The game's own advice follows the Department (src/AdviceSwitches.h): the Lua of DepartmentEfficiency.dat (its two Lua scripts, built by the project tools) extends the trigger of the
    // Education / Health, neighbor-deal and garbage advice with six switches that this plugin publishes. A switch is 1 only while the Department can REALLY manage what the advice is about
    // (OWNER RULE 2026-11-08): switched off, another mode, a controller that failed (fail-closed) or a plugin that did not load all leave the game's advice exactly as the game ships it.
    sc4af::AdviceOperational AdviceOperationalNow() {
        sc4af::AdviceOperational op;
        op.civicWritesAllowed = autoFinance_.WritesAllowed();
        op.civicSnapshotUsable = autoFinance_.SnapshotUsable();
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        op.unifiedActive = UnifiedActive();
        if (op.unifiedActive && unifiedAuto_) {
            op.unifiedFailed = unifiedAuto_->sessionFailed();
            op.dealsDisabled = unifiedAuto_->dealsDisabled();
            op.garbageDisabled = unifiedAuto_->garbageDisabled();
            for (int i = 0; i < 3; ++i) { op.ready[i] = unifiedAuto_->commodityReady(i); op.contractsReady[i] = unifiedAuto_->commodityContractsReady(i); }
        }
#endif
        return op;
    }

    // Publishes the six switches of the CURRENT city. force: the city was just initialised (its advisor holds nothing). Without an advisor system (no city, a failed query) nothing is
    // written and the game's own advice stays as it is: the Lua reads a missing switch as 0. Written only when a value changed (a set is never written half).
    void RefreshAdviceSwitches(bool force, const char* reason) {
        if (!cityActive_) return;
        const auto op = AdviceOperationalNow();
        const sc4af::AdviceSwitches want = sc4af::AdviceSwitchesFor(autoFinance_.config(), utility_.config(), op);
        auto* advisor = AdvisorSystemNow();
        if (!advisor) {
            adviceSwitches_.ResetCity();
            if (!adviceSwitchesWarned_) { adviceSwitchesWarned_ = true; sc4af::Log(sc4af::LogLevel::Warning, std::string("[AdviceSwitches] ") + reason + ": no advisor system, the switches are not published (the game's advice stays as it is); retried every day"); }
            return;
        }
        const sc4af::AdviceSwitches before = adviceSwitches_.last();
        const bool had = adviceSwitches_.published();
        bool raised = false;
        const bool ok = adviceSwitches_.Publish(want, force, [&](const char* name, double value) { return GuardedSetGlobalNumber(advisor, name, value, raised); });
        if (raised && !adviceSwitchesWarned_) sc4af::Log(sc4af::LogLevel::Error, std::string("[AdviceSwitches] ") + reason + ": SetGlobalValue raised an exception");
        if (!ok) {
            if (!adviceSwitchesWarned_) { adviceSwitchesWarned_ = true; sc4af::Log(sc4af::LogLevel::Warning, std::string("[AdviceSwitches] ") + reason + ": the game refused the switches; retried every day (" + sc4af::AdviceSwitchesText(want) + ")"); }
            return;
        }
        adviceSwitchesWarned_ = false;
        if (force || !had || before != want)
            sc4af::Log(sc4af::LogLevel::Info, std::string("[AdviceSwitches] ") + reason + ": published " + sc4af::AdviceSwitchesText(want) + " (civic Mode=" + sc4af::AutoModeName(autoFinance_.config().mode) +
                                                  " civicWrites=" + (op.civicWritesAllowed ? "allowed" : "STOPPED") + " unified=" + (op.unifiedActive ? (op.unifiedFailed ? "FAILED" : "active") : "inactive") +
                                                  " dealsDisabled=" + (op.dealsDisabled ? "yes" : "no") + " sanitationDisabled=" + (op.garbageDisabled ? "yes" : "no") + ")");
    }

    bool PreAppShutdown() override {
        if (pollTimer_) KillTimer(nullptr, pollTimer_);
        pollTimer_ = 0;
        if (uiTimer_) KillTimer(nullptr, uiTimer_);
        uiTimer_ = 0;
        optionsUi_.Reset();
        gDirector = nullptr;
        if (utility_.config().phase8Runtime) phase8_.LogStatus();
        Unsubscribe();
        cityActive_ = false;
        citySerial_ = 0;
        sc4af::Log(sc4af::LogLevel::Info, "PreAppShutdown; city state cleared");
        sc4af::CloseLog();
        return true;
    }

    bool DoMessage(cIGZMessage2* message) override {
        if (!supported_ || !subscribed_ || !message) return true;
        try {
        switch (message->GetType()) {
        case kPostCityInit: {
            cRZAutoRefCount<cIGZMessage2Standard> standard;
            if (!message->QueryInterface(GZIID_cIGZMessage2Standard,
                                         standard.AsPPVoid()) || !standard->GetHasData1()) {
                sc4af::Log(sc4af::LogLevel::Error, "PostCityInit lacks city payload");
                break;
            }
            auto* city = static_cast<cISC4City*>(standard->GetVoid1());
            cityActive_ = city != nullptr && city->GetResidentialSimulator() != nullptr;
            citySerial_ = city ? city->GetCitySerialNumber() : 0;
            observedMonths_ = 0;
            daysInCity_ = 0;
            sc4af::Log(sc4af::LogLevel::Info,
                       "[CityLifecycle] PostCityInit CitySerial=" +
                       std::to_string(citySerial_));
            adviceSwitches_.ResetCity();                                       // every city has its own advisor system (and Lua state): nothing is inherited
            adviceSwitchesWarned_ = false;
            if (!cityActive_) {
                sc4af::Log(sc4af::LogLevel::Error,
                           "Civic probe disabled for this city: city or ResidentialSimulator unavailable");
            } else {
                autoFinance_.LoadConfigIfChanged();                            // an INI edited between two cities is read before the switches are published
                utility_.LoadConfigIfChanged();
                RefreshAdviceSwitches(true, "PostCityInit");                   // before the first simulated day
                sc4af::LogCivicSnapshot(sc4af::ReadCivicSnapshot(city), true);
                sc4af::OmniReset();
                writer_.ResetCity();
                autoFinance_.ResetCity();
                utility_.ResetCity();
                utilityWriter_.ResetCity();
                utilityShadow_.ResetCity();
                utilityAuto_.ResetCity();
                ResetUnified();
                phase8_.ResetCity();
                cityInitMs_ = GetTickCount64();
                dirtySinceMs_ = 0;
                sc4af::RunOmniRefresh(city, "PostCityInit", true);
                sc4af::LotWatchReset();
                sc4af::RunLotWatch(city, "PostCityInit", true);      // read-only: which abandoned / destroyed lots the demolition rules would pick
#ifdef SC4AF_LANDFILL_RESEARCH
                utility_.LoadConfigIfChanged();
                if (utility_.config().landfillResearch) sc4af::landfill_research::Install();
#endif
                ObserveUtilities(city, "PostCityInit");
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
                unifiedAuto_->ObserveLandfillBaseline(utility_.last());
                notices_.StartSession();                          // a NEW session of the utility notices: no counter, dismissal, deadline or episode of an earlier one; a notice an earlier session left listed is withdrawn on the first tick
                civicBoard_.Reset();                              // Education / Health: the committed behaviour (memory of the notices kept between sessions)
                civicRestored_ = false;
                for (auto& c : civicCount_) c = 0;
                for (bool& k : enabledKnown_) k = false;
                for (bool& l : listedLogged_) l = false;
                noticeSessionLogged_ = false;
                unifiedRanDay_ = 0xFFFFFFFFu;
                simDays_ = 0;
                cityKey_.clear();
                failureKey_.clear();                              // the failure notices link to the city again on its first simulated day
#endif
                LogFinalReadiness();
                ordinance_.AddToCity(city, optionsUi_.MasterIsOn(), "PostCityInit");
            }
            break;
        }
        case kPreSave:
            ordinance_.RemoveFromCity(CurrentCity(), "PreSave");               // the save never holds it: the city opens the same without the plugin
            break;
        case kPostSave:
            if (cityActive_) ordinance_.AddToCity(CurrentCity(), optionsUi_.MasterIsOn(), "PostSave");
            break;
        case kPreCityShutdown:
            ordinance_.RemoveFromCity(CurrentCity(), "PreCityShutdown");
            break;
        case kInsertOccupant:
        case kRemoveOccupant:
            if (cityActive_ && dirtySinceMs_ == 0) dirtySinceMs_ = GetTickCount64();
            break;
        case kPostCityShutdown:
            adviceSwitches_.ResetCity();                                       // the advisor system of this city is gone with its Lua state: nothing to write, nothing to clear
            if (utility_.config().phase8Runtime) phase8_.NoteCityShutdown(citySerial_);
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
            civicBoard_.Reset();
            civicRestored_ = false;
            for (auto& c : civicCount_) c = 0;
            notices_ = sc4af::NoticeBoard();                  // the advisor of this city goes with it: nothing is left to withdraw
            simDays_ = 0;
            cityKey_.clear();
            failureKey_.clear();                              // the failure notices link to the city again on its first simulated day
            if (cityActive_) sc4af::Log(sc4af::LogLevel::Info, "[Contracts] SHUTDOWN city=" + std::to_string(citySerial_) + " " + unifiedAuto_->contractTable());
#endif
            cityActive_ = false;
            dirtySinceMs_ = 0;
            sc4af::OmniReset();
            sc4af::LotWatchReset();
            writer_.ResetCity();
            autoFinance_.ResetCity();
            utility_.ResetCity();
            utilityWriter_.ResetCity();
            utilityShadow_.ResetCity();
            utilityAuto_.ResetCity();
            ResetUnified();
            phase8_.ResetCity();
            citySerial_ = 0;
            observedMonths_ = 0;
#ifdef SC4AF_LANDFILL_RESEARCH
            sc4af::landfill_research::OnCityShutdown();
#endif
            sc4af::Log(sc4af::LogLevel::Info,
                       "[CityLifecycle] PostCityShutdown; no city pointers retained");
            break;
        case kSimNewMonth: {
            const double t0 = PerfNowMs();
            OnNewMonth();
            perfMonthMs_ = PerfNowMs() - t0;
            LogPerf();
            break;
        }
        case kSimNewDay: {
            const double t0 = PerfNowMs();
            OnNewDay();
            const double ms = PerfNowMs() - t0;
            perfDayMs_ += ms;
            ++perfDays_;
            if (ms > perfMaxDayMs_) perfMaxDayMs_ = ms;
            break;
        }
        case kNeighborDealEvent:
            OnNeighborDealMessage(message);
            break;
        case kGarbageScalingRecompute:
            OnGarbageScalingMessage();
            break;
        default:
            break;
        }
        } catch (const std::exception& error) {
            sc4af::Log(sc4af::LogLevel::Error,
                       std::string("Civic callback failed: ") + error.what());
        } catch (...) {
            sc4af::Log(sc4af::LogLevel::Error, "Civic callback failed with unknown C++ exception");
        }
        return true;
    }

private:
    // Read-only utility snapshot. The regional city is borrowed for this call only.
    void ObserveUtilities(cISC4City* city, const char* reason) {
        cISC4RegionalCity* regional = nullptr;
        if (auto* framework = FrameWork()) {
            if (framework->Application()) {
                cRZAutoRefCount<cISC4App> app;
                if (framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid()))
                    regional = app->GetRegionalCity();
            }
        }
        utility_.Observe(city, regional, reason, true);
        if (utility_.config().phase8Runtime) phase8_.Observe(utility_.last(), reason);
#ifdef SC4AF_LANDFILL_RESEARCH
        if (utility_.config().landfillResearch) sc4af::landfill_research::PluginSnapshot(reason, utility_.last());
#endif
    }

#ifdef SC4AF_LANDFILL_RESEARCH
    // Passive snapshot (own read, independent of Phase8Runtime): API values + Garbage deals, placed in the landfill boundary event sequence.
    void ResearchCapture(cISC4City* city, const char* reason) {
        if (!utility_.config().landfillResearch || !city) return;
        cISC4RegionalCity* regional = nullptr;
        if (auto* framework = FrameWork()) {
            if (framework->Application()) {
                cRZAutoRefCount<cISC4App> app;
                if (framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) regional = app->GetRegionalCity();
            }
        }
        const auto snapshot = sc4af::ReadUtilitySnapshot(city, regional, utility_.epoch(), true, observedMonths_ > 0, false);
        sc4af::landfill_research::PluginSnapshot(reason, snapshot);
    }
#endif

    void CapturePhase8Snapshot(cISC4City* city, cISC4RegionalCity* regional, const char* reason) {
        if (!city) return;
#ifdef SC4AF_LANDFILL_RESEARCH
        if (utility_.config().landfillResearch && !utility_.config().phase8Runtime) { ResearchCapture(city, reason); return; }
#endif
        if (!utility_.config().phase8Runtime) return;
        // This is a fresh, passive readback after a message. It uses ProposeDeals(..., false)
        // and the existing guarded pricing calls; it never creates or modifies a deal.
        const auto snapshot = sc4af::ReadUtilitySnapshot(city, regional, utility_.epoch(), true, observedMonths_ > 0, true);
        phase8_.Observe(snapshot, reason);
#ifdef SC4AF_LANDFILL_RESEARCH
        if (utility_.config().landfillResearch) sc4af::landfill_research::PluginSnapshot(reason, snapshot);
#endif
    }

    void OnNeighborDealMessage(cIGZMessage2* message) {
        if (!cityActive_ || !utility_.config().phase8Runtime) return;
        cRZAutoRefCount<cIGZMessage2Standard> standard;
        const bool typed = message && message->QueryInterface(GZIID_cIGZMessage2Standard, standard.AsPPVoid());
        const bool has1 = typed && standard->GetHasData1();
        const bool has2 = typed && standard->GetHasData2();
        phase8_.DealMessage(has1 ? standard->GetData1() : 0, has1, has2 ? standard->GetData2() : 0, has2);
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return;
        auto* city = app->GetCity();
        if (city && city->GetCitySerialNumber() == citySerial_) CapturePhase8Snapshot(city, app->GetRegionalCity(), "NeighborDealEvent");
    }

    void OnGarbageScalingMessage() {
        if (!cityActive_ || !utility_.config().phase8Runtime) return;
        phase8_.GarbageScalingMessage();
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return;
        auto* city = app->GetCity();
        if (city && city->GetCitySerialNumber() == citySerial_) CapturePhase8Snapshot(city, app->GetRegionalCity(), "GarbageScalingRecompute");
    }

    // Validation builds only: READY / NOT_READY with the first unmet precondition, logged whenever it changes.
    void LogFinalReadiness() {
#ifdef SC4AF_ENABLE_FINAL_AUTO_VALIDATION
        const auto& civic = autoFinance_.config();
        const bool civicConfigured = civic.mode == sc4af::AutoMode::Auto && civic.education && civic.health;
        const std::string line = sc4af::FinalValidationReadiness(utility_.config(), supported_, sc4af::UnifiedAutoValidationCapability::kBuildEnabled, unifiedAuto_ != nullptr, civicConfigured);
        if (line != lastReadiness_) {
            lastReadiness_ = line;
            sc4af::Log(line.find("NOT_READY") != std::string::npos ? sc4af::LogLevel::Error : sc4af::LogLevel::Info, line);
        }
        if (cityActive_ && line != notifiedReadiness_) {     // announced once a city is loaded (never during the start-up screens)
            notifiedReadiness_ = line;
            Notice(sc4af::CueForReadiness(line));
        }
#endif
    }

    void ResetUnified() {
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        unifiedAuto_->ResetCity();
        unifiedAuto_->landfillAdvisor().ResetCity();     // the landfill record belongs to the city: cleared here, restored from told.txt once the city key is known
        unifiedAuto_->landfillGhosts().ResetCity();      // the handled ghost states belong to the session of the city
        landfillRestored_ = false;
        landfillKeyLogged_ = false;
        landfillKey_.clear();
#endif
    }

    // True only when both halves of the capability are open (compile-time capability + EnableUnifiedAutoValidation, Mode=Auto,
    // not the passive recorder mode). In every other build and configuration the Power/Water Auto below is untouched.
    bool UnifiedActive() const {
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        return sc4af::UnifiedGateOpen(utility_.config());
#else
        return false;
#endif
    }

#ifdef SC4AF_ENABLE_UNIFIED_AUTO
#ifdef SC4AF_ENABLE_FINAL_AUTO_VALIDATION
    // Validation overlay only: player cues without any external window (see NativeNotice.h): beeps plus text on the game's own title bar. The product never touches the window.
    static BOOL CALLBACK FindOwnWindow(HWND h, LPARAM lp) {
        DWORD pid = 0;
        GetWindowThreadProcessId(h, &pid);
        if (pid == GetCurrentProcessId() && IsWindowVisible(h) && GetWindow(h, GW_OWNER) == nullptr) { *reinterpret_cast<HWND*>(lp) = h; return FALSE; }
        return TRUE;
    }
#endif
    // The loaded city (nullptr outside a city).
    cISC4City* CityNow() {
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return nullptr;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return nullptr;
        return app->GetCity();
    }
    // Primary channel: the native SC4 Urgent Advice dialog (inside the game UI, visible in fullscreen) bound to an event of our own DepartmentEfficiency.dat.
    // Whether the player has Urgent Advice enabled cannot be read from the game, so the beeps / title-bar text below stay as a window-less fallback.
    // Hands one event to the advisor system. true = PushEvent returned true (accepted); that is NOT proof that the advice was displayed.
    bool PushAdvice(std::int32_t event) {
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return false;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return false;
        auto* city = app->GetCity();
        auto* advisor = city ? city->GetAdvisorSystem() : nullptr;
        const bool accepted = advisor && advisor->PushEvent(event, nullptr);
        sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] native Urgent Advice event 0x") + sc4af::HexId(event) + (accepted ? " handed to the advisor (PushEvent returned true; this is NOT proof that it was displayed)" : " NOT accepted (no advisor system)"));
        return accepted;
    }

    // ---- the landfill size savings advice (NativeNotice.h): called by the controller through the landfill advisor callback ----------------------------------------------------------------------------------------------
    static bool GuardedSetGlobalNumber(cISC4AdvisorSystem* advisor, const char* name, double value, bool& raised) {
        __try { return advisor->SetGlobalValue(name, value); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
    }
    static bool GuardedPushEvent(cISC4AdvisorSystem* advisor, std::int32_t event, bool& raised) {
        __try { return advisor->PushEvent(event, nullptr); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
    }
    static bool GuardedSetLink(cISC4AdvisorSystem* advisor, const char* name, cISC4AdviceSubject* subject, bool& raised) {
        __try { return advisor->SetLocalValue(name, 1.0, subject); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
    }
    // Hands one landfill advice to the advisor (NativeNotice.h LandfillNotice): the numbers are written as numeric advisor global values and the link as the advice local value
    // kLandfillLinkVariable (an advice subject on the CELL coordinates of the area's centre), then the event is pushed. true = everything was set and PushEvent returned true; that is not
    // proof that the notice was displayed. The caller decides whether the option is on and the cadence; this function only delivers.
    bool PushLandfillAdvice(const sc4af::LandfillNotice& n) {
        const std::int32_t event = sc4af::LandfillAdviceEvent(n.kind);
        const std::string what = std::string("[Notice] landfill advice REDUCE") + " event 0x" + sc4af::HexId(event) + " (tiles=" + std::to_string(n.tiles) + " area=" +
                                 std::to_string(n.across) + "x" + std::to_string(n.along) + " percent=" + std::to_string(n.percent) + " savings=" + std::to_string(n.savings) + " link=" +
                                 std::to_string(n.linkX) + "," + std::to_string(n.linkZ) + ")";
        if (!sc4af::LandfillNoticeValid(n)) { sc4af::Log(sc4af::LogLevel::Warning, what + " NOT pushed: the numbers are not valid"); return false; }
        auto* advisor = AdvisorSystemNow();
        if (!advisor) { sc4af::Log(sc4af::LogLevel::Info, what + " NOT pushed: no advisor system"); return false; }
        bool raised = false;
        const bool set = GuardedSetGlobalNumber(advisor, sc4af::kLandfillTilesGlobal, static_cast<double>(n.tiles), raised) &&
                         GuardedSetGlobalNumber(advisor, sc4af::kLandfillPercentGlobal, static_cast<double>(n.percent), raised) &&
                         GuardedSetGlobalNumber(advisor, sc4af::kLandfillSavingsGlobal, static_cast<double>(n.savings), raised) &&
                         GuardedSetGlobalNumber(advisor, sc4af::kLandfillAcrossGlobal, static_cast<double>(n.across), raised) &&
                         GuardedSetGlobalNumber(advisor, sc4af::kLandfillAlongGlobal, static_cast<double>(n.along), raised);
        if (!set) { sc4af::Log(sc4af::LogLevel::Warning, what + (raised ? " NOT pushed: an advisor call raised an exception" : " NOT pushed: the text values were refused")); return false; }
        cISC4AdviceSubject* subject = nullptr;
        if (!mpCOM || !mpCOM->GetClassObject(sc4af::kAdviceSubjectClsid, sc4af::kAdviceSubjectIid, reinterpret_cast<void**>(&subject)) || !subject) {
            sc4af::Log(sc4af::LogLevel::Warning, what + " NOT pushed: the advice subject of the link could not be created");
            return false;
        }
        subject->SetLocation(n.linkX, n.linkZ);              // CELL coordinates (verified in the game with the civic links)
        const bool linked = GuardedSetLink(advisor, sc4af::kLandfillLinkVariable, subject, raised);
        subject->Release();                                  // the game keeps its own reference
        if (!linked) { sc4af::Log(sc4af::LogLevel::Warning, what + (raised ? " NOT pushed: setting the link raised an exception" : " NOT pushed: the link was refused")); return false; }
        const bool accepted = GuardedPushEvent(advisor, event, raised);
        sc4af::Log(sc4af::LogLevel::Info, what + (accepted ? " handed to the advisor (PushEvent returned true; this is NOT proof that it was displayed)" : raised ? " NOT accepted (the call raised an exception)" : " NOT accepted"));
        return accepted;
    }

    // ---- the Education / Health capacity notices: ONE notice per facility type (CivicBuildingNotices.h) ---------------------------------------------------------------------------------------------
    // Advisor calls (SimCity 4.exe 1.1.641.0): GetAdviceByGUID returns the advisor's own advice object; the advisor's active flag 2 on it is set by
    // PushEvent and cleared by RetireAdvice (which requires it). Every call runs under an exception guard; if one raises, the plugin stops using them for the session.
    static cISC4Advice* GuardedAdviceByGuid(cISC4Advisor* advisor, std::int32_t guid, bool& raised) {
        __try { return advisor->GetAdviceByGUID(guid); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return nullptr; }
    }
    static bool GuardedRetire(cISC4Advisor* advisor, cISC4Advice* advice, bool& raised) {
        __try { return advisor->RetireAdvice(advice); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
    }
    static bool GuardedShown(cISC4Advice* advice, bool& raised) {
        __try { return advice->CheckFlags(2); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
    }

    const char* LinkVariable(int kind, int version, int slot) {
        if (linkVariables_.empty()) {
            linkVariables_.reserve(static_cast<std::size_t>(sc4af::kCivicKinds) * sc4af::kCivicVersions * sc4af::kCivicMaxLinks);
            for (int k = 0; k < sc4af::kCivicKinds; ++k)
                for (int v = 0; v < sc4af::kCivicVersions; ++v)
                    for (int i = 1; i <= sc4af::kCivicMaxLinks; ++i) linkVariables_.push_back("l_sc4af_" + std::to_string(k) + "_" + std::to_string(v) + "_" + std::to_string(i) + "_h");
        }
        return linkVariables_[static_cast<std::size_t>((kind * sc4af::kCivicVersions + version) * sc4af::kCivicMaxLinks + (slot - 1))].c_str();
    }

    cISC4AdvisorSystem* AdvisorSystemNow() {
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return nullptr;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return nullptr;
        auto* city = app->GetCity();
        return city ? city->GetAdvisorSystem() : nullptr;
    }

    static std::string CellsText(const std::vector<sc4af::AffectedBuilding>& selected) {
        std::string cells;
        for (const auto& b : selected) cells += (cells.empty() ? "" : ";") + std::to_string(b.cellX) + "," + std::to_string(b.cellZ);
        return cells;
    }

    // The link of slot i of the notice of one type reads the advice local value l_sc4af_<type>_0_<i>_h when the player clicks it: writing them is how a notice is pushed AND how a shown notice is
    // updated in place.
    bool SetCivicLinks(int kind, const std::vector<sc4af::AffectedBuilding>& selected) {
        auto* advisor = AdvisorSystemNow();
        const int count = static_cast<int>(selected.size());
        if (!advisor || count < 1 || count > sc4af::kCivicMaxLinks) return false;
        for (int i = 0; i < count; ++i) {
            const auto& b = selected[i];
            cISC4AdviceSubject* subject = nullptr;
            if (!b.hasPos || !mpCOM || !mpCOM->GetClassObject(sc4af::kAdviceSubjectClsid, sc4af::kAdviceSubjectIid, reinterpret_cast<void**>(&subject)) || !subject) {
                sc4af::Log(sc4af::LogLevel::Warning, "[Notice] the advice subject could not be created: the links are not written (tried again at the next evaluation)");
                return false;
            }
            bool raised = false;
            GuardedSetLocation(subject, b.posX / 16.0f, b.posZ / 16.0f, raised);          // CELL coordinates (verified in the game)
            if (!raised) GuardedSetLink(advisor, LinkVariable(kind, 0, i + 1), subject, raised);
            subject->Release();                                              // the game keeps its own reference
            if (raised) { MarkAdvisorBroken(); return false; }
        }
        return true;
    }

    static cISC4Advisor* GuardedAdvisor(cISC4AdvisorSystem* system, int type, bool& raised) {
        __try { return system ? system->GetAdvisor(type) : nullptr; } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return nullptr; }
    }
    static void GuardedSetLocation(cISC4AdviceSubject* subject, float x, float z, bool& raised) {
        __try { subject->SetLocation(x, z); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; }
    }
    void MarkAdvisorBroken() {
        advisorApiBroken_ = true;
        sc4af::Log(sc4af::LogLevel::Warning, "[Notice] a native advisor call raised an exception: the notices are not checked, pushed or retired again in this session");
    }

    // The advice records of the notice of one utility slot: the pop-up and the discreet variant (slots 0..2), or the single discreet record of the stagnant-backlog notice (slot 3).
    static int UtilityGuids(int slot, std::int32_t (&guids)[2]) {
        guids[0] = sc4af::NoSupplyAdviceGuid(slot, true);
        if (slot == sc4af::kStagnantSlot) return 1;
        guids[1] = sc4af::NoSupplyAdviceGuid(slot, false);
        return 2;
    }

    // What the advisor says about the notice of a utility slot: Listed when the advice of ANY of its records carries the active flag 2, NotListed when the records exist and none does, Unknown when the
    // call is unavailable, raised or the records are not known. Flag 2 is all the interface shows: it does not say WHY a notice left the list (dismissed, expired or retired), only that it is not listed.
    sc4af::Listing UtilityListing(int slot) {
        auto* system = AdvisorSystemNow();
        bool raised = false, known = false, listed = false;
        auto* advisor = advisorApiBroken_ ? nullptr : GuardedAdvisor(system, 21, raised);
        if (raised) { MarkAdvisorBroken(); return sc4af::Listing::Unknown; }
        if (!advisor || advisorApiBroken_) return sc4af::Listing::Unknown;
        std::int32_t guids[2];
        const int n = UtilityGuids(slot, guids);
        for (int i = 0; i < n; ++i) {
            cISC4Advice* advice = GuardedAdviceByGuid(advisor, guids[i], raised);
            if (raised) break;
            if (!advice) continue;
            known = true;
            if (GuardedShown(advice, raised)) listed = true;
            if (raised) break;
        }
        if (raised) { MarkAdvisorBroken(); return sc4af::Listing::Unknown; }
        return listed ? sc4af::Listing::Listed : known ? sc4af::Listing::NotListed : sc4af::Listing::Unknown;
    }

    // Withdraws every record of the notice of a utility slot that the advisor lists, then asks the advisor again: TRUE only when it no longer lists any (also when nothing was listed); FALSE when the call is
    // unavailable, raised, or the notice is still listed (it is tried again later, after a bounded back-off; no duplicate is ever pushed on top of it). The retirement is judged by the advisor's flag, not by
    // the return value of RetireAdvice, and nothing here claims the player ever saw or dismissed it.
    bool RetireNotice(int slot) {
        const std::string name = std::string("[Notice] ") + sc4af::NoticeSlotName(slot);
        auto* system = AdvisorSystemNow();
        bool raised = false;
        auto* advisor = advisorApiBroken_ ? nullptr : GuardedAdvisor(system, 21, raised);
        if (raised) { MarkAdvisorBroken(); return false; }
        if (!advisor || advisorApiBroken_) { sc4af::Log(sc4af::LogLevel::Info, name + ": retirement: the advisor cannot be asked (NOT confirmed, tried again later)"); return false; }
        int called = 0, refused = 0;
        std::int32_t guids[2];
        const int n = UtilityGuids(slot, guids);
        for (int i = 0; i < n && !raised; ++i) {
            cISC4Advice* advice = GuardedAdviceByGuid(advisor, guids[i], raised);
            if (raised || !advice) continue;
            if (!GuardedShown(advice, raised) || raised) continue;
            ++called;
            if (!GuardedRetire(advisor, advice, raised)) ++refused;
        }
        if (raised) { MarkAdvisorBroken(); return false; }
        const auto after = UtilityListing(slot);
        sc4af::Log(sc4af::LogLevel::Info, name + ": retirement: " + (called ? "RetireAdvice called for " + std::to_string(called) + " listed record(s)" + (refused ? " (" + std::to_string(refused) + " returned false)" : std::string()) : std::string("nothing was listed (expired, dismissed or retired earlier: the interface does not say which)")) +
                   "; the advisor " + (after == sc4af::Listing::NotListed ? "no longer lists it (confirmed)" : after == sc4af::Listing::Listed ? "STILL lists it (NOT confirmed, tried again later)" : "could not be asked afterwards (NOT confirmed, tried again later)"));
        return after == sc4af::Listing::NotListed;
    }

    // Hands the notice of a utility slot to the advisor. true = PushEvent returned true: it is NOT proof that the notice was displayed (the advisor lists it a moment later, on its next evaluation, and whether a
    // pop-up window opens depends on the player's own advisor settings, which the plugin cannot read).
    bool PushNotice(int slot, bool popup) {
        const std::int32_t event = sc4af::NoSupplyAdviceEvent(slot, popup);
        const std::string name = std::string("[Notice] ") + sc4af::NoticeSlotName(slot) + ": " + (popup ? "pop-up" : "discreet") + " event 0x" + sc4af::HexId(event);
        auto* advisor = AdvisorSystemNow();
        if (!advisor || advisorApiBroken_) { sc4af::Log(sc4af::LogLevel::Info, name + " NOT pushed: the advisor cannot be asked (tried again later)"); return false; }
        bool raised = false;
        const bool accepted = GuardedPushEvent(advisor, event, raised);
        if (raised) MarkAdvisorBroken();
        sc4af::Log(sc4af::LogLevel::Info, name + (accepted ? " handed to the advisor (PushEvent returned true; this is NOT proof that it was displayed)" : raised ? " NOT accepted (the call raised an exception)" : " NOT accepted by the advisor (tried again later)"));
        return accepted;
    }

    // ---- the Education / Health notices (committed behaviour, restored 2026-11-11): see CivicBuildingNotices.h ----
    // Whether the Health and Education advisor shows the notice of one (type, number of buildings): its advice carries the active flag 2. Unknown when the call is unavailable or raised.
    sc4af::Listing ShownState(int kind, int count) {
        auto* advisor = AdvisorSystemNow();
        auto* adv22 = advisor ? advisor->GetAdvisor(22) : nullptr;
        if (!adv22 || advisorApiBroken_) return sc4af::Listing::Unknown;
        bool raised = false;
        cISC4Advice* advice = GuardedAdviceByGuid(adv22, sc4af::CivicAdviceGuid(kind, 0, count), raised);
        const bool shown = advice && !raised && GuardedShown(advice, raised);
        if (raised) { advisorApiBroken_ = true; sc4af::Log(sc4af::LogLevel::Warning, "[Notice] a native advisor call raised an exception: the notices are not checked or retired again in this session"); return sc4af::Listing::Unknown; }
        if (!advice) return sc4af::Listing::Unknown;
        return shown ? sc4af::Listing::Listed : sc4af::Listing::NotListed;
    }

    // Pushes the notice of one type (links first, then the event of the record for that many buildings). True = the advisor accepted it; the log says whether it shows it afterwards.
    bool PushCivicNotice(const sc4af::CivicNoticeBoard::Action& a) {
        auto* advisor = AdvisorSystemNow();
        const int count = static_cast<int>(a.selected.size());
        if (!advisor || !SetCivicLinks(a.kind, a.selected)) return false;
        const std::int32_t event = sc4af::CivicAdviceEvent(a.kind, 0, count);
        const bool accepted = advisor->PushEvent(event, nullptr);
        const auto shown = accepted ? ShownState(a.kind, count) : sc4af::Listing::Unknown;
        sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] native advice event 0x") + sc4af::HexId(event) + " (" + std::to_string(count) + " building(s) " + CellsText(a.selected) + ") " +
                   (accepted ? std::string("handed to the advisor; ") + (shown == sc4af::Listing::Listed ? "the advisor shows it" : shown == sc4af::Listing::NotListed ? "the advisor does NOT show it" : "shown state unknown")
                             : std::string("NOT accepted")));
        return accepted;
    }

    // Retires the notice (type, number of buildings). TRUE when the advisor no longer shows it afterwards (also when it was not shown: dismissed by the player or timed out); FALSE when the retire
    // failed, raised or could not be checked (the notice may still be shown: it is tried again).
    bool RetireCivicNotice(int kind, int count) {
        const std::int32_t guid = sc4af::CivicAdviceGuid(kind, 0, count);
        const auto before = ShownState(kind, count);
        if (before == sc4af::Listing::Unknown) { sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] retire of advice 0x") + sc4af::HexId(guid) + ": the advisor cannot be asked (NOT confirmed)"); return false; }
        if (before == sc4af::Listing::NotListed) { sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] retire of advice 0x") + sc4af::HexId(guid) + ": not shown (dismissed by the player, timed out or already retired)"); return true; }
        auto* advisor = AdvisorSystemNow();
        auto* adv22 = advisor ? advisor->GetAdvisor(22) : nullptr;
        bool raised = false;
        cISC4Advice* advice = adv22 ? GuardedAdviceByGuid(adv22, guid, raised) : nullptr;
        bool ok = false;
        if (advice && !raised) ok = GuardedRetire(adv22, advice, raised);
        if (raised) { advisorApiBroken_ = true; sc4af::Log(sc4af::LogLevel::Warning, "[Notice] a native advisor call raised an exception: the notices are not checked or retired again in this session"); return false; }
        const auto after = ShownState(kind, count);
        sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] retire of advice 0x") + sc4af::HexId(guid) + ": RetireAdvice returned " + (ok ? "true" : "false") + ", the advisor " +
                   (after == sc4af::Listing::NotListed ? "no longer shows it (confirmed)" : after == sc4af::Listing::Listed ? "STILL shows it (NOT confirmed)" : "could not be asked afterwards (NOT confirmed)"));
        return after == sc4af::Listing::NotListed;
    }

    // ---- the failure notices (FailureNotice.h): ONE news item per service the Department can no longer manage technically in this session ----------------------------------------------------------------------
    // The record is BAD_JOB, MEDIUM (below the pop-up priority of every skill level): a plain entry of the news, never a pop-up. All the technical detail goes to the log only.
    sc4af::Listing FailureShown(int service) {
        auto* advisor = AdvisorSystemNow();
        auto* adv = advisor ? advisor->GetAdvisor(sc4af::FailureAdvisorType(service)) : nullptr;
        if (!adv || advisorApiBroken_) return sc4af::Listing::Unknown;
        bool raised = false;
        cISC4Advice* advice = GuardedAdviceByGuid(adv, sc4af::FailureAdviceGuid(service), raised);
        const bool shown = advice && !raised && GuardedShown(advice, raised);
        if (raised) { advisorApiBroken_ = true; sc4af::Log(sc4af::LogLevel::Warning, "[Notice] a native advisor call raised an exception: the notices are not checked or retired again in this session"); return sc4af::Listing::Unknown; }
        if (!advice) return sc4af::Listing::Unknown;
        return shown ? sc4af::Listing::Listed : sc4af::Listing::NotListed;
    }

    // true = the advisor accepted the event (NOT proof that the player saw it: the log also says whether the advisor lists it afterwards).
    bool PushFailureNotice(int service) {
        auto* advisor = AdvisorSystemNow();
        const std::int32_t event = sc4af::FailureAdviceEvent(service);
        if (!advisor) { sc4af::Log(sc4af::LogLevel::Info, std::string("[FailureNotice] ") + sc4af::FailureServiceName(service) + ": event 0x" + sc4af::HexId(event) + " NOT pushed: no advisor system (tried again later)"); return false; }
        bool raised = false;
        const bool accepted = GuardedPushEvent(advisor, event, raised);
        const auto shown = accepted ? FailureShown(service) : sc4af::Listing::Unknown;
        sc4af::Log(sc4af::LogLevel::Info, std::string("[FailureNotice] ") + sc4af::FailureServiceName(service) + ": news item event 0x" + sc4af::HexId(event) + " " +
                   (accepted ? std::string("handed to the advisor (PushEvent returned true; this is NOT proof that it was displayed); the advisor ") +
                               (shown == sc4af::Listing::Listed ? "lists it" : shown == sc4af::Listing::NotListed ? "does not list it yet (measured: it is listed a moment later, on the next advisor evaluation)" : "could not be asked")
                             : raised ? std::string("NOT accepted (the call raised an exception)") : std::string("NOT accepted by the advisor (tried again later)")));
        return accepted;
    }

    // true = the advisor no longer lists it afterwards (also when it was not listed: dismissed by the player or timed out).
    bool RetireFailureNotice(int service) {
        const std::string name = std::string("[FailureNotice] ") + sc4af::FailureServiceName(service);
        const auto before = FailureShown(service);
        if (before == sc4af::Listing::NotListed) { sc4af::Log(sc4af::LogLevel::Info, name + ": withdrawal: it is not listed any more (dismissed by the player, timed out or already withdrawn)"); return true; }
        if (before == sc4af::Listing::Unknown) { sc4af::Log(sc4af::LogLevel::Info, name + ": withdrawal: the advisor cannot be asked (NOT confirmed, tried again later)"); return false; }
        auto* advisor = AdvisorSystemNow();
        auto* adv = advisor ? advisor->GetAdvisor(sc4af::FailureAdvisorType(service)) : nullptr;
        bool raised = false;
        cISC4Advice* advice = adv ? GuardedAdviceByGuid(adv, sc4af::FailureAdviceGuid(service), raised) : nullptr;
        bool ok = false;
        if (advice && !raised) ok = GuardedRetire(adv, advice, raised);
        if (raised) { advisorApiBroken_ = true; sc4af::Log(sc4af::LogLevel::Warning, "[Notice] a native advisor call raised an exception: the notices are not checked or retired again in this session"); return false; }
        const auto after = FailureShown(service);
        sc4af::Log(sc4af::LogLevel::Info, name + ": withdrawal: RetireAdvice returned " + (ok ? "true" : "false") + ", the advisor " +
                   (after == sc4af::Listing::NotListed ? "no longer lists it (confirmed)" : after == sc4af::Listing::Listed ? "STILL lists it (NOT confirmed, tried again later)" : "could not be asked afterwards (NOT confirmed)"));
        return after == sc4af::Listing::NotListed;
    }

    // What the controllers already know about each service, as plain booleans (nothing is read from the game here, nothing is evaluated: the cost is a few comparisons per simulated day).
    void FillFailureInputs(std::array<sc4af::FailureInput, sc4af::kFailureServices>& in) {
        using sc4af::FailureService;
        const auto at = [&](FailureService s) -> sc4af::FailureInput& { return in[static_cast<std::size_t>(s)]; };
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        if (UnifiedActive() && unifiedAuto_) {
            const auto& u = utility_.config();
            const auto& ua = *unifiedAuto_;
            // only DEFINITIVE failures (fail-closed): a writer that failed its verification stays shut down until the city is loaded again. NOT_READY readings, STANDBY and the like are recoverable: never reported.
            at(FailureService::Power) = {u.autoPower, ua.powerDisabled(), ua.powerDisabled() ? "the power funding writer failed its verification and was shut down: " + ua.powerReason() : std::string()};
            at(FailureService::Water) = {u.autoWater, ua.waterDisabled(), ua.waterDisabled() ? "the water funding writer failed its verification and was shut down: " + ua.waterReason() : std::string()};
            at(FailureService::Sanitation) = {u.autoGarbage, ua.garbageDisabled(), ua.garbageDisabled() ? "the sanitation funding writer failed its verification and was shut down: " + ua.garbageReason() : std::string()};
            // the contracts: a failed deal manager is global (it covers power, water and garbage together), so one message is true
            at(FailureService::Deals) = {u.autoDeals && (u.tradePower || u.tradeWater || u.tradeGarbage), ua.dealsDisabled(), ua.dealsDisabled() ? "the neighbor-deal manager failed and was shut down: " + ua.dealsReason() : std::string()};
        }
#endif
        const auto& civic = autoFinance_.config();
        const bool civicAuto = civic.mode == sc4af::AutoMode::Auto;
        const bool tripped = autoFinance_.WriterTripped();      // one writer serves both: each is reported only when the player enabled it
        const std::string civicCause = tripped ? "the civic writer distrusted itself (3 different facilities could not be verified)" : std::string();
        at(FailureService::Health) = {civicAuto && civic.health, tripped, civicCause};
        at(FailureService::Education) = {civicAuto && civic.education, tripped, civicCause};
        const std::string stop = sc4af::LotDemolitionStopReason();
        at(FailureService::Demolition) = {sc4af::LotDemolitionAutoOn(), !stop.empty(), stop};
    }

    // Once per simulated day. The first call of a city session starts a NEW memory (one notice per service and city session; nothing is kept from an earlier session, not even of the same city) and
    // withdraws any notice of an older session that the advisor still lists.
    void ObserveFailures() {
        if (!cityActive_) return;
        if (failureKey_.empty()) {
            const std::string& key = CityKeyNow();
            failureKey_ = key.empty() ? "serial:" + std::to_string(citySerial_) : key;               // for the log only
            failures_.StartSession();
            int stray = 0;
            for (int s = 0; s < sc4af::kFailureServices; ++s)
                if (FailureShown(s) == sc4af::Listing::Listed && RetireFailureNotice(s)) ++stray;
            if (stray) sc4af::Log(sc4af::LogLevel::Info, "[FailureNotice] city session started: " + std::to_string(stray) + " failure notice(s) of an older session withdrawn");
        }
        std::array<sc4af::FailureInput, sc4af::kFailureServices> in;
        FillFailureInputs(in);
        std::vector<std::string> transitions;
        const auto steps = failures_.Update(simDays_, in, &transitions);
        for (const auto& t : transitions) sc4af::Log(sc4af::LogLevel::Info, "[FailureNotice] city=" + failureKey_ + " " + t);
        for (const auto& step : steps) {
            if (step.what == sc4af::FailureDo::Announce) {
                sc4af::Log(sc4af::LogLevel::Info, std::string("[FailureNotice] ") + sc4af::FailureServiceName(step.service) + ": failure confirmed on simulated day " + std::to_string(simDays_) + " (" + step.cause + "): the player is told once");
                failures_.ConfirmAnnounce(step.service, PushFailureNotice(step.service), simDays_);
            } else if (step.what == sc4af::FailureDo::Retire) {
                sc4af::Log(sc4af::LogLevel::Info, std::string("[FailureNotice] ") + sc4af::FailureServiceName(step.service) + (step.recovered ? ": the management is available again: the notice is withdrawn" : ": the player switched the service off: the notice is withdrawn"));
                failures_.ConfirmRetire(step.service, RetireFailureNotice(step.service), simDays_);
            }
        }
    }

    // Once per city: every notice of the plugin that the advisor shows but the board does not own (an older session, a crash between a retire and a push) is retired,
    // so that a type never shows two notices.
    void ReconcileCivicNotices() {
        int retired = 0, kept = 0;
        for (int k = 0; k < sc4af::kCivicKinds; ++k)
            for (int n = 1; n <= sc4af::kCivicMaxLinks; ++n) {
                if (ShownState(k, n) != sc4af::Listing::Listed) continue;
                if (civicBoard_.active(k) && civicBoard_.activeCount(k) == n) { ++kept; continue; }
                if (RetireCivicNotice(k, n)) ++retired;
            }
        if (retired || kept) sc4af::Log(sc4af::LogLevel::Info, "[Notice] city opened: " + std::to_string(kept) + " notice(s) of the Ministry kept, " + std::to_string(retired) + " stray one(s) retired");
    }

    // The notice of one type is told only when funding cannot solve the shortage (CivicBuildingNotices.h): max funding insufficient, no strike, persistent for 30 simulated days. One notice per
    // type, updated in place or replaced (the shown one is retired first) when its buildings change, retired when everything recovered. Silent otherwise.
    void UpdateCivicNotices() {
        const auto& s = autoFinance_.shortage();
        if (!s.evaluated) return;
        const std::string& city = CityKeyNow();
        bool memoryChanged = false;
        if (!civicRestored_) {
            for (int k = 0; k < sc4af::kCivicKinds; ++k) {
                const std::string prefix = sc4af::BoardMemoryPrefix(city, 3 + k);
                if (prefix.empty()) continue;
                for (const auto& line : told_.KeysWithPrefix(prefix)) {
                    sc4af::BoardMemory m;
                    if (sc4af::ParseBoardMemoryLine(line, prefix, m)) civicBoard_.Restore(k, m);
                }
            }
            ReconcileCivicNotices();
        }
        for (int k = 0; k < sc4af::kCivicKinds; ++k) {
            const auto& ks = s.kinds[k];
            const int id = 3 + k;
            const char* name = sc4af::kCivicKindTable[k].name;
            const std::string prefix = sc4af::BoardMemoryPrefix(city, id);
            if (ks.count != civicCount_[k]) {
                if (ks.count > 0)
                    sc4af::Log(sc4af::LogLevel::Info, std::string("[Service][NoSupply] ") + name + ": " + std::to_string(ks.count) + " building(s) cannot reach a safe coverage even at the maximum funding (worst: " + ks.detail +
                               "); funding cannot solve it, a new facility of this type is needed");
                else
                    sc4af::Log(sc4af::LogLevel::Info, std::string("[Service][EpisodeEnded] ") + name + ": coverage restored (proven by the next evaluation)");
                civicCount_[k] = ks.count;
            }
            const auto action = civicBoard_.Update(k, simDays_, ks.buildings);
            const auto before = civicBoard_.Memory(k);
            if (action.what == sc4af::CivicNoticeBoard::Do::Push) {
                sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + ": " + std::to_string(action.selected.size()) + " building(s) short after " + std::to_string(sc4af::kNoticePersistDays) +
                           " simulated days (" + std::to_string(ks.count) + " affected now): " + (action.replacement ? "the shown notice is retired and replaced" : "told to the player"));
                bool go = true;
                if (action.replacement) {
                    const bool gone = RetireCivicNotice(k, action.retireCount);
                    civicBoard_.ConfirmRetire(k, gone);
                    if (!gone) { go = false; sc4af::Log(sc4af::LogLevel::Warning, std::string("[Notice] ") + name + ": the shown notice could not be retired: no second notice is pushed, tried again at the next evaluation"); }
                }
                if (go) {
                    const bool accepted = PushCivicNotice(action);
                    civicBoard_.ConfirmPush(action, simDays_, accepted);
                    if (accepted && civicBoard_.cooldownUntil(k) > simDays_) sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + ": two replacements inside 30 days: cooldown until simulated day " + std::to_string(civicBoard_.cooldownUntil(k)));
                }
            } else if (action.what == sc4af::CivicNoticeBoard::Do::Refresh) {
                if (SetCivicLinks(k, action.selected)) {
                    civicBoard_.ConfirmRefresh(action);
                    sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + ": the buildings changed, not their number: the links of the shown notice now lead to " + CellsText(action.selected) + " (updated in place)");
                }
            } else if (action.what == sc4af::CivicNoticeBoard::Do::Retire) {
                sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + ": every building recovered, the notice is retired");
                const bool gone = RetireCivicNotice(k, action.retireCount);
                civicBoard_.ConfirmRetire(k, gone);
                if (!gone) sc4af::Log(sc4af::LogLevel::Warning, std::string("[Notice] ") + name + ": the retire is NOT confirmed, the notice stays active and the retire is tried again at the next evaluation");
            }
            const auto after = civicBoard_.Memory(k);
            if (!prefix.empty() && sc4af::BoardMemoryLine(prefix, before) != sc4af::BoardMemoryLine(prefix, after)) {
                for (const auto& old : told_.KeysWithPrefix(prefix)) told_.Erase(old);
                if (after.pushes > 0) told_.Add(sc4af::BoardMemoryLine(prefix, after));
                memoryChanged = true;
            }
        }
        civicRestored_ = true;
        if (memoryChanged) SaveTold();
    }

    // ---- the shortage notices of the three utilities (ServiceNotices.h) ----------------------------------------------------------------------------------------------------------------------------------------
    // The management of a service is what makes its notices exist: they are shown only while the player has it on, and its switch (or the master switch) withdraws the notice, cancels the reminders and closes
    // the episode. The regional CONTRACTS have their own switches and are never touched here. Slot 3 is the informative notice of a STAGNANT garbage backlog (it follows the sanitation management).
    bool UtilityNoticeEnabled(int slot) const {
        if (!unifiedAuto_) return false;
        const auto& c = utility_.config();
        sc4af::UtilityManagement m;
        m.controllerRuns = UnifiedActive();
        m.sessionFailed = unifiedAuto_->sessionFailed();
        m.localOn[0] = c.autoPower; m.localOn[1] = c.autoWater; m.localOn[2] = c.autoGarbage;
        m.failedClosed[0] = unifiedAuto_->powerDisabled(); m.failedClosed[1] = unifiedAuto_->waterDisabled(); m.failedClosed[2] = unifiedAuto_->garbageDisabled();
        for (int i = 0; i < 3; ++i) m.contractsOn[i] = c.tradeActive(i);
        return sc4af::UtilityNoticeEnabled(m, slot);
    }

    // Once per simulated day (at the end of the day's work: OnNewDay), on every change of the options, on every month, and from the 1-second poll while something must still be withdrawn: one look at each
    // slot, in the order of importance. Calling it again on the same day is harmless: the clocks only move with the simulated days.
    void TickNotices() {
        if (!cityActive_ || !unifiedAuto_) return;
        const bool ran = unifiedRanDay_ == simDays_;                   // the unified controller has read today's counters
        if (!noticeSessionLogged_) {
            noticeSessionLogged_ = true;
            sc4af::Log(sc4af::LogLevel::Info, "[Notice] utility notices: city session started (simulated day " + std::to_string(simDays_) + "): no counter, dismissal, deadline or episode of an earlier session is kept; a notice of the Department that an earlier session left listed is withdrawn");
        }
        for (const int slot : sc4af::kSlotsByImportance) {
            const char* name = sc4af::NoticeSlotName(slot);
            sc4af::NoticeInput in;
            in.enabled = UtilityNoticeEnabled(slot);
            if (enabledKnown_[slot] && enabledNow_[slot] != in.enabled)
                sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + " (simulated day " + std::to_string(simDays_) + "): the management is now " + (in.enabled ? "ON: a shortage must be proven again with the normal persistence before any notice (an old episode is not resumed)"
                                                                                                                                                                      : "OFF: its notice is withdrawn, the reminders are cancelled and the episode is closed; the regional contracts are untouched"));
            if (slot < sc4af::kUtilitySlots && enabledKnown_[slot] && enabledNow_[slot] != in.enabled) unifiedAuto_->ForgetProofs();      // what an earlier plan proved must be proven again
            enabledKnown_[slot] = true;
            enabledNow_[slot] = in.enabled;
            in.evidence = in.enabled && ran ? unifiedAuto_->noticeEvidence(slot) : sc4af::ShortageEvidence::Uncertain;
            if (notices_.outstanding(slot)) in.listing = UtilityListing(slot);
            if (in.listing == sc4af::Listing::Listed && !listedLogged_[slot]) {
                listedLogged_[slot] = true;
                sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + ": the advisor lists the notice (active flag observed on simulated day " + std::to_string(simDays_) + ")");
            }
            const auto step = notices_.Update(slot, simDays_, in);
            if (step.what == sc4af::NoticeDo::Retire) {
                sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + " (simulated day " + std::to_string(simDays_) + "): the notice is withdrawn: " +
                           (step.why == sc4af::RetireWhy::Recovered ? "recovery confirmed (or the situation changed to another notice)" : step.why == sc4af::RetireWhy::Disabled ? "the management of this service is off" : "left listed by an earlier session"));
                notices_.ConfirmRetire(slot, RetireNotice(slot), simDays_);
            } else if (step.what == sc4af::NoticeDo::Emit) {
                if (step.retireFirst) {
                    const bool gone = RetireNotice(slot);
                    notices_.ConfirmRetire(slot, gone, simDays_);
                    if (!gone) {
                        sc4af::Log(sc4af::LogLevel::Warning, std::string("[Notice] ") + name + ": the previous notice could not be withdrawn: no second notice is pushed, tried again after the back-off");
                        continue;
                    }
                }
                const auto before = notices_.State(slot);
                sc4af::Log(sc4af::LogLevel::Info, std::string("[Notice] ") + name + " (simulated day " + std::to_string(simDays_) + "): notice #" + std::to_string(step.emission) + " (" + (step.popup ? "pop-up" : "discreet") + ") after " + std::to_string(before.clock) + " simulated days with the situation proven" +
                           (step.emission > 1 ? ", " + std::to_string(sc4af::ReminderDays(sc4af::ClassOfSlot(slot), 0)) + " days after the previous one (the interface cannot tell a dismissal from an expiry, so no dismissal is claimed: fixed interval)" : std::string()));
                const bool accepted = PushNotice(slot, step.popup);
                notices_.ConfirmEmit(slot, accepted, simDays_);
                if (accepted) listedLogged_[slot] = false;
            }
        }
    }

    void Notice(const sc4af::NoticeCue& cue) {
        if (cue.kind == sc4af::NoticeKind::None) return;
#ifndef SC4AF_ENABLE_FINAL_AUTO_VALIDATION
        // The shortage notices of the product come from the notice board (TickNotices), from the evidence of every day: a log line hands nothing to the advisor. No caption change, no beep, no validation cue.
        (void)cue;
#else
        if (cue.advice != 0) PushAdvice(cue.advice);
        for (int i = 0; i < cue.beeps; ++i) MessageBeep(cue.problem ? MB_ICONHAND : MB_ICONASTERISK);
        HWND game = nullptr;
        EnumWindows(&FindOwnWindow, reinterpret_cast<LPARAM>(&game));
        if (game) SetWindowTextA(game, cue.title.c_str());
#endif
    }
    void LogLines(const std::vector<sc4af::AutoLine>& lines) {
        for (const auto& line : lines) {
            sc4af::Log(line.level == 2 ? sc4af::LogLevel::Error : line.level == 1 ? sc4af::LogLevel::Warning : sc4af::LogLevel::Info, line.text);
            Notice(sc4af::CueForLogLine(line.text));
        }
    }
#endif

    // Utility Auto: power plant funding and water department funding only, per-subsystem opt-in. Runs after the
    // monthly snapshot; one write cycle per subsystem per month; every failure is contained to its subsystem.
    void RunUtilityAuto(cISC4City* city) {
        const auto& config = utility_.config();
        cISC4RegionalCity* regional = nullptr;
        if (auto* framework = FrameWork()) {
            if (framework->Application()) {
                cRZAutoRefCount<cISC4App> app;
                if (framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) regional = app->GetRegionalCity();
            }
        }
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        if (UnifiedActive()) {
            sc4af::GameUnifiedActuator unified(city, regional, utility_.epoch(), config, sc4af::UnifiedAutoValidationSeam::Arm());
            EnsureLandfillRestored();
            unifiedAuto_->OnMonth(config, unified);
            LogLines(unifiedAuto_->lines());
            return;
        }
#endif
        sc4af::GameUtilityActuator actuator(city, regional, utility_.epoch(), config);
        utilityAuto_.OnWaterCycle(utility_.last(), config, actuator);   // water follows the month; power runs on the SimNewDay cadence
        for (const auto& line : utilityAuto_.lines())
            sc4af::Log(line.level == 2 ? sc4af::LogLevel::Error : line.level == 1 ? sc4af::LogLevel::Warning : sc4af::LogLevel::Info, line.text);
    }

    void RunUtilityWriter(cISC4City* city) {
        cISC4RegionalCity* regional = nullptr;
        if (auto* framework = FrameWork()) {
            if (framework->Application()) {
                cRZAutoRefCount<cISC4App> app;
                if (framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) regional = app->GetRegionalCity();
            }
        }
        utilityWriter_.OnMonth(city, regional, utility_.last());
    }

    // Diagnostic trace: one compact line per simulated day, only when DepartmentEfficiency.ini [UtilityResearch] says DailyTrace=true. Read-only.
    // The Civic controller's early reaction: four getters a day; a full evaluation only when the game reports a strike or a strike chance.
    void CivicDailyWatch() {
        if (!cityActive_ || autoFinance_.config().mode != sc4af::AutoMode::Auto) return;
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return;
        auto* city = app->GetCity();
        if (!city || city->GetCitySerialNumber() != citySerial_) return;
        autoFinance_.OnDay(city, observedMonths_);
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        UpdateCivicNotices();
#endif
    }

#ifdef SC4AF_ENABLE_UNIFIED_AUTO
    // ---- memory file (NoticeMemory.h, DepartmentEfficiency.told.txt): the record of the landfill advisor ONLY. The shortage notices keep nothing between sessions ----
    void LoadTold() {
        std::ifstream in(toldPath_, std::ios::binary);
        if (!in) return;
        std::ostringstream buffer;
        buffer << in.rdbuf();
        told_ = sc4af::NoticeMemory::Parse(buffer.str());
        sc4af::Log(sc4af::LogLevel::Info, "[Notice] memory file loaded: " + std::to_string(told_.size()) + " line(s) (the record of the landfill advisor; the shortage notices keep no memory between sessions)");
    }
    void SaveTold() {
        std::ofstream out(toldPath_, std::ios::binary | std::ios::trunc);
        if (!out) { sc4af::Log(sc4af::LogLevel::Warning, "[Notice] memory file could not be written; the landfill advisor may repeat an advice in a later session"); return; }
        out << told_.Serialize();
    }
    // Landfill advisor record (LandfillAdvisor.h): restored once per city, when the key is known; a city with no key keeps a session-only record (nothing is persisted).
    void EnsureLandfillRestored() {
        if (landfillRestored_) return;
        landfillKey_ = CityKeyNow();
        if (landfillKey_.empty()) {                  // the city is not identified yet: try again next time (latching now would keep the record session-only for good)
            if (!landfillKeyLogged_) { landfillKeyLogged_ = true; sc4af::Log(sc4af::LogLevel::Info, "[LandfillAdvisor] city not identified yet: session-only record until it is"); }
            return;
        }
        landfillRestored_ = true;
        sc4af::LandfillAdviceRecord r;
        if (!landfillKey_.empty() && sc4af::ParseLandfillRecord(told_, sc4af::LandfillMemoryPrefix(landfillKey_), r)) {
            unifiedAuto_->landfillAdvisor().Restore(r);
            sc4af::Log(sc4af::LogLevel::Info, "[LandfillAdvisor] record restored: tiles=" + std::to_string(r.tiles) + " lastObserved=" + std::to_string(r.ref) + " lastChange=" + std::to_string(r.chg) +
                       (r.hasAdvice ? " lastAdvice=" + std::to_string(r.adv) : std::string(" lastAdvice=none")));
        } else {
            sc4af::Log(sc4af::LogLevel::Info, landfillKey_.empty() ? "[LandfillAdvisor] city not identified: session-only record (not persisted)" : "[LandfillAdvisor] no record for this city: history unknown, protection starts at the first observation");
        }
    }
    void PersistLandfillRecord(const sc4af::LandfillAdviceRecord& r) {
        if (landfillKey_.empty()) return;
        if (sc4af::StoreLandfillRecord(told_, sc4af::LandfillMemoryPrefix(landfillKey_), r)) SaveTold();
    }
    // Identity of the loaded city (region directory + serial + position); computed once per session, read only.
    const std::string& CityKeyNow() {
        if (!cityKey_.empty()) return cityKey_;
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return cityKey_;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return cityKey_;
        auto* city = app->GetCity();
        auto* region = app->GetRegion();
        auto* regional = app->GetRegionalCity();
        if (city && region && regional && regional->GetCitySerialNumber() == city->GetCitySerialNumber()) {
            std::int32_t x = 0, z = 0;
            regional->GetPosition(x, z);
            auto* dir = region->GetDirectoryName();
            cityKey_ = sc4af::BuildNoticeCityKey(dir ? dir->ToChar() : "", city->GetCitySerialNumber(), true, x, z);
        }
        return cityKey_;
    }

#endif

    // Time spent by the plugin itself in the simulation callbacks (a measurement, not a guess): one line per simulated month.
    static double PerfNowMs() {
        LARGE_INTEGER f{}, c{};
        QueryPerformanceFrequency(&f);
        QueryPerformanceCounter(&c);
        return f.QuadPart ? 1000.0 * static_cast<double>(c.QuadPart) / static_cast<double>(f.QuadPart) : 0.0;
    }
    struct PerfScope {
        double& acc;
        double t0;
        explicit PerfScope(double& a) : acc(a), t0(PerfNowMs()) {}
        ~PerfScope() { acc += PerfNowMs() - t0; }
    };
    void LogPerf() {
        if (!cityActive_) return;
        char text[420];
        std::snprintf(text, sizeof text,
                      "[Perf] last month: SimNewDay x%u total=%.0f ms avg=%.1f ms max=%.0f ms {civicWatch=%.0f lotWatch=%.0f snapshot=%.0f unified=%.0f notices=%.0f}; SimNewMonth=%.0f ms {omni=%.0f lotWatch=%.0f civic=%.0f utilities=%.0f}; the plugin used %.0f ms of the month; utilities{observe=%.0f shadow=%.0f auto=%.0f writer=%.0f}",
                      perfDays_, perfDayMs_, perfDays_ ? perfDayMs_ / perfDays_ : 0.0, perfMaxDayMs_, perfCivicDayMs_, perfLotDayMs_, perfSnapDayMs_, perfUnifiedDayMs_, perfNoticeDayMs_, perfMonthMs_,
                      perfOmniMs_, perfLotMonthMs_, perfCivicMonthMs_, perfUtilMonthMs_, perfDayMs_ + perfMonthMs_, perfUtilObserveMs_, perfUtilShadowMs_, perfUtilAutoMs_, perfUtilWriterMs_);
        sc4af::Log(sc4af::LogLevel::Info, text);
        perfDays_ = 0;
        perfDayMs_ = perfMaxDayMs_ = perfMonthMs_ = perfCivicDayMs_ = perfLotDayMs_ = perfSnapDayMs_ = perfUnifiedDayMs_ = perfNoticeDayMs_ = 0.0;
        perfOmniMs_ = perfLotMonthMs_ = perfCivicMonthMs_ = perfUtilMonthMs_ = 0.0;
        perfUtilObserveMs_ = perfUtilShadowMs_ = perfUtilAutoMs_ = perfUtilWriterMs_ = 0.0;
    }
    std::uint32_t perfDays_ = 0;
    double perfDayMs_ = 0.0, perfMaxDayMs_ = 0.0, perfMonthMs_ = 0.0;
    double perfCivicDayMs_ = 0.0, perfLotDayMs_ = 0.0, perfSnapDayMs_ = 0.0, perfUnifiedDayMs_ = 0.0, perfNoticeDayMs_ = 0.0;
    double perfOmniMs_ = 0.0, perfLotMonthMs_ = 0.0, perfCivicMonthMs_ = 0.0, perfUtilMonthMs_ = 0.0;
    bool shadowSkipLogged_ = false;
    double perfUtilObserveMs_ = 0.0, perfUtilShadowMs_ = 0.0, perfUtilAutoMs_ = 0.0, perfUtilWriterMs_ = 0.0;   // the parts of `utilities`

    void LotWatchTick() {
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return;
        auto* city = app->GetCity();                   // Borrowed only for this callback.
        if (!city || city->GetCitySerialNumber() != citySerial_) return;
        sc4af::RunLotWatch(city, "SimNewDay", false);
    }

    void OnNewDay() {
        ++simDays_;
        if (cityActive_) ObserveFailures();                                                           // the failure notices: a few booleans per day, never touches a controller
        if (cityActive_) RefreshAdviceSwitches(false, "SimNewDay");                                   // a controller that failed, an earlier publish that failed: the switches follow within a day (a hand-edited INI is read monthly; the options window and the ordinance apply at once)
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        struct DayEnd { AutoFinanceDirector* self; ~DayEnd() { try { self->noticePollBudget_ = kNoticePollsPerDay; if (self->cityActive_) { PerfScope p(self->perfNoticeDayMs_); self->TickNotices(); } } catch (...) {} } } dayEnd{this};       // the notices of the day, whichever way OnNewDay returns, after the day's evidence is in
#endif
        { PerfScope p(perfCivicDayMs_); CivicDailyWatch(); }
        if (cityActive_ && simDays_ % 10 == 0) { PerfScope p(perfLotDayMs_); LotWatchTick(); }      // logs only the lots that appeared or changed state / reason (the monthly pass lists them all)
        if (!cityActive_ || !utility_.config().enabled) return;
        utility_.LoadConfigIfChanged();
        UpdatePhase8Subscriptions();
        const auto& config = utility_.config();
        ++daysInCity_;
        if (config.phase8Runtime) phase8_.SimulationEvent("SimNewDay");
        const bool trace = config.dailyTrace;
        const bool powerCycle = config.mode != sc4af::UtilityMode::Off && daysInCity_ % static_cast<std::uint32_t>(config.powerCadenceDays) == 0;     // no month counter: the readings decide (ServiceReadiness.h)
        const bool unifiedDay = UnifiedActive();
#ifdef SC4AF_LANDFILL_RESEARCH
        const bool research = config.landfillResearch;
#else
        const bool research = false;
#endif
        if (!trace && !powerCycle && !unifiedDay && !config.phase8Runtime && !research) return;
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return;
        auto* city = app->GetCity();
        if (!city || city->GetCitySerialNumber() != citySerial_) return;
        cISC4RegionalCity* regional = app->GetRegionalCity();
        if (config.phase8Runtime || research) CapturePhase8Snapshot(city, regional, "SimNewDay");
        const bool probe = config.latentProbe || config.mode != sc4af::UtilityMode::Off;
        // New proposals are asked for only when the deal controller may act on them (DealDiscoveryAllowed); the existing contracts are read either way.
        // The daily snapshot serves only the optional daily trace and the legacy power cycle: the unified controller reads (and plans) on its own, so in unified mode it is not read at all.
        sc4af::UtilitySnapshot snap;
        if (trace || (!unifiedDay && powerCycle)) {
            PerfScope p(perfSnapDayMs_);
            snap = sc4af::ReadUtilitySnapshot(city, regional, utility_.epoch(), true, observedMonths_ > 0 && sc4af::DealDiscoveryAllowed(config), probe);
        }
        if (trace) sc4af::Log(sc4af::LogLevel::Info, sc4af::FormatPowerTraceLine(snap));
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        if (unifiedDay) {
            PerfScope p(perfUnifiedDayMs_);
            const double t0 = PerfNowMs();
            sc4af::GameUnifiedActuator unified(city, regional, utility_.epoch(), config, sc4af::UnifiedAutoValidationSeam::Arm());
            const double tBuilt = PerfNowMs();
            sc4af::TimedUnifiedActuator timed(unified);       // diagnostics only: where a slow day's time went
            EnsureLandfillRestored();
            unifiedAuto_->OnDay(config, timed);
            unifiedRanDay_ = simDays_;                        // the evidence of the notices is today's
            LogLines(unifiedAuto_->lines());
            const double total = PerfNowMs() - t0;
            if (total > 100.0) {                              // a slow day (a quiet day costs well under 10 ms): say where the time went, once per such day
                char text[160];
                std::snprintf(text, sizeof text, "[Perf][SlowDay] unified day=%u total=%.0f ms: actuator built %.1f ms, game calls %.1f ms, controller (plans, logs) %.1f ms; calls: ",
                              simDays_, total, tBuilt - t0, timed.totalMs(), total - (tBuilt - t0) - timed.totalMs());
                sc4af::Log(sc4af::LogLevel::Info, std::string(text) + timed.Text());
            }
        } else
#endif
        if (powerCycle) {
            // one plan per cycle, shared by the Shadow log and Auto
            const auto plan = sc4af::PlanPower(snap, sc4af::MakeOptimizerConfig(config), sc4af::VanillaUtilityTuning());
            utilityShadow_.OnPowerCycle(snap, config, plan);
            sc4af::GameUtilityActuator actuator(city, regional, utility_.epoch(), config);
            utilityAuto_.OnPowerCycle(snap, config, actuator, &plan);
            for (const auto& line : utilityAuto_.lines())
                sc4af::Log(line.level == 2 ? sc4af::LogLevel::Error : line.level == 1 ? sc4af::LogLevel::Warning : sc4af::LogLevel::Info, line.text);
        }
    }

    void OnNewMonth() {
        if (!cityActive_) return;
        utility_.LoadConfigIfChanged();
        LogFinalReadiness();
        UpdatePhase8Subscriptions();
        auto* framework = FrameWork();
        if (!framework || !framework->Application()) return;
        cRZAutoRefCount<cISC4App> app;
        if (!framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) {
            sc4af::Log(sc4af::LogLevel::Warning, "SimNewMonth: SC4App unavailable");
            return;
        }
        auto* city = app->GetCity(); // Borrowed only for this callback.
        if (!city || city->GetCitySerialNumber() != citySerial_) {
            sc4af::Log(sc4af::LogLevel::Warning,
                       "SimNewMonth: no matching loaded city; snapshot skipped");
            return;
        }
#ifdef SC4AF_LANDFILL_RESEARCH
        if (utility_.config().landfillResearch) {
            sc4af::landfill_research::Mark("SimNewMonth_ENTER");
            ResearchCapture(city, "PluginSimNewMonth_ENTER");
        }
#endif
        ++observedMonths_;
        if (utility_.config().phase8Runtime) phase8_.SimulationEvent("SimNewMonth");
        sc4af::Log(sc4af::LogLevel::Info,
                   "[CityLifecycle] SimNewMonth observed=" +
                   std::to_string(observedMonths_));
        // Reconciliation fallback: diff-only, so a quiet month logs nothing but the month marker.
        dirtySinceMs_ = 0;
        { PerfScope p(perfOmniMs_); sc4af::RunOmniRefresh(city, "SimNewMonth", false); }
        { PerfScope p(perfLotMonthMs_); sc4af::RunLotWatch(city, "SimNewMonth", true); }
        if (sc4af::farm_water_zot::Installed())
            sc4af::Log(sc4af::LogLevel::Info, "[FarmWaterZot] no-water icons kept off agricultural lots this month: " + std::to_string(sc4af::farm_water_zot::TakeSuppressedCount()));
        writer_.ObserveMonth(city, observedMonths_); // read-only drift observation of the test target
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        if (UnifiedActive()) autoFinance_.SetWritesBlocked(unifiedAuto_->sessionFailed());   // a failed unified session (fail-closed) stops Civic writes too
#endif
        { PerfScope p(perfCivicMonthMs_); autoFinance_.OnMonth(city, observedMonths_); }  // Off by default; Shadow logs, Auto writes
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        UpdateCivicNotices();
        TickNotices();
#endif
#ifdef SC4AF_ENABLE_FINAL_AUTO_VALIDATION
        if (UnifiedActive()) {                         // the Civic controller's evidence is graded with the rest of the validation
            const auto& civic = autoFinance_.config();
            unifiedAuto_->SetCivicExpected(civic.mode == sc4af::AutoMode::Auto && civic.education, civic.mode == sc4af::AutoMode::Auto && civic.health);
            unifiedAuto_->OnCivicMonth(autoFinance_.evidence());
        }
#endif
        PerfScope utilitiesScope(perfUtilMonthMs_);   // everything from here to the end of the month callback is the utilities side
        { PerfScope p(perfUtilObserveMs_); ObserveUtilities(city, "SimNewMonth"); }        // read-only probe
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
        unifiedAuto_->ObserveLandfillMonth(utility_.last());   // the month boundary as the normal snapshot shows it (calibrates the landfill geometry)
#endif
        // Shadow/Auto plan log: zero writes. With the unified controller running, that log would be a second whole-system plan per month (~40 ms) that only repeats what
        // the controller has just planned and logged itself ([UnifiedAuto][Plan], [Unified][Action]); it is kept for Shadow mode and for the legacy controllers.
        if (utility_.config().enabled && !UnifiedActive()) { PerfScope p(perfUtilShadowMs_); utilityShadow_.OnMonth(utility_.last(), utility_.config()); }
        else if (utility_.config().enabled && !shadowSkipLogged_) {
            shadowSkipLogged_ = true;
            sc4af::Log(sc4af::LogLevel::Info, "[UtilityOptimizer] monthly Shadow plan not recomputed: the unified controller plans and logs the same pass ([UnifiedAuto][Plan])");
        }
        if (utility_.config().enabled) { PerfScope p(perfUtilAutoMs_); RunUtilityAuto(city); }                                          // Mode=Auto only; otherwise resets/returns
        if (!utility_.config().phase8Runtime) { PerfScope p(perfUtilWriterMs_); RunUtilityWriter(city); }   // the passive recorder mode disables even the isolated writer harness
    }

public:
    // Called from the main-thread timer. Read-only: cheap count poll, and a
    // debounced refresh after an occupant insert/remove notification.
    void OnPollTimer() {
        if (!cityActive_ || inPoll_) return;
        inPoll_ = true;
        try {
            auto* framework = FrameWork();
            if (framework && framework->Application()) {
                cRZAutoRefCount<cISC4App> app;
                if (framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) {
                    auto* city = app->GetCity(); // Borrowed for this call only.
                    if (city && city->GetCitySerialNumber() == citySerial_) {
                        if (utility_.config().phase8Runtime)
                            phase8_.ObserveDealUiDisabled(sc4af::ReadNeighborDealsUiDisabled(city), "Poll");
                        writer_.Poll(city, GetTickCount64() - cityInitMs_);
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
                        if (noticePollBudget_ > 0 && notices_.NeedsAttention(simDays_)) { --noticePollBudget_; TickNotices(); }       // a notice that must be withdrawn (the switch was turned off while the game is paused, an event still pending in the advisor): do not wait for a day
#endif
                        const bool countsChanged = sc4af::OmniCountsChanged(city);
                        if (kRefreshPolicy.ShouldRefresh(countsChanged, dirtySinceMs_, GetTickCount64())) {
                            dirtySinceMs_ = 0;
                            sc4af::RunOmniRefresh(city, countsChanged ? "count-poll" : "occupant-message", false);
                        }
                    }
                }
            }
        } catch (const std::exception& error) {
            sc4af::Log(sc4af::LogLevel::Error, std::string("Poll refresh failed: ") + error.what());
        } catch (...) {
            sc4af::Log(sc4af::LogLevel::Error, "Poll refresh failed with unknown C++ exception");
        }
        inPoll_ = false;
    }

    // The city open now (borrowed), or nullptr.
    cISC4City* CurrentCity() {
        auto* framework = FrameWork();
        cRZAutoRefCount<cISC4App> app;
        if (!framework || !framework->Application() || !framework->Application()->QueryInterface(GZIID_cISC4App, app.AsPPVoid())) return nullptr;
        return app->GetCity();
    }

    // The game's windows are looked at on the main thread only; in the region view too (Play Options is reachable there).
    void OnUiTimer() {
        try {
            optionsUi_.Tick();
        } catch (...) {
            sc4af::Log(sc4af::LogLevel::Error, "[OptionsUI] the options button failed with a C++ exception");
        }
    }

private:
    static constexpr sc4af::RefreshPolicy kRefreshPolicy{};

    void Unsubscribe() {
        if (!subscribed_) return;
        cIGZMessageServer2Ptr messages;
        if (messages) {
            for (const auto id : kMessages) messages->RemoveNotification(this, id);
            if (occupantMessages_) {
                messages->RemoveNotification(this, kInsertOccupant);
                messages->RemoveNotification(this, kRemoveOccupant);
            }
            if (phase8Messages_) {
                messages->RemoveNotification(this, kNeighborDealEvent);
                messages->RemoveNotification(this, kGarbageScalingRecompute);
            }
        }
        occupantMessages_ = false;
        phase8Messages_ = false;
        subscribed_ = false;
    }

    void UpdatePhase8Subscriptions() {
        const bool want = supported_ && subscribed_ && utility_.config().enabled && utility_.config().phase8Runtime;
        if (want == phase8Messages_) {
            phase8_.SetReady(phase8Messages_, phase8Messages_, utility_.config().phase8Runtime);
            return;
        }
        cIGZMessageServer2Ptr messages;
        if (!messages) return;
        if (want) {
            const bool deal = messages->AddNotification(this, kNeighborDealEvent);
            const bool garbage = deal && messages->AddNotification(this, kGarbageScalingRecompute);
            if (!garbage) {
                if (deal) messages->RemoveNotification(this, kNeighborDealEvent);
                sc4af::Log(sc4af::LogLevel::Warning, "[Phase8Runtime] passive message subscription unavailable; READY withheld");
                phase8_.SetReady(false, false, true);
                return;
            }
            phase8Messages_ = true;
            sc4af::Log(sc4af::LogLevel::Info, "[Phase8Runtime] subscribed passive 0xEA418D8F and 0x66956810");
        } else {
            messages->RemoveNotification(this, kNeighborDealEvent);
            messages->RemoveNotification(this, kGarbageScalingRecompute);
            phase8Messages_ = false;
        }
        phase8_.SetReady(phase8Messages_, phase8Messages_, utility_.config().phase8Runtime);
    }

    bool supported_ = false;
    bool subscribed_ = false;
    bool cityActive_ = false;
    bool adviceSwitchesWarned_ = false;                                                // one warning per city when the switches cannot be published
    sc4af::AdviceSwitchPublisher adviceSwitches_;                                      // what this city's advisor holds of the six switches
    bool occupantMessages_ = false;
    bool phase8Messages_ = false;
    bool inPoll_ = false;
    UINT_PTR pollTimer_ = 0;
    UINT_PTR uiTimer_ = 0;
    sc4af::OptionsUI optionsUi_;
    sc4af::MinistryOrdinance ordinance_;           // the Department in the game's Ordinances window: the same global switch (MinistryOrdinance.h)
    std::uint64_t dirtySinceMs_ = 0;
    std::uint64_t cityInitMs_ = 0;
    sc4af::FailureNoticeBoard failures_;          // the failure notices: one per service and city for the whole game session (FailureNotice.h)
    std::string failureKey_;                      // the city key the board is linked to (empty until the first day of a city session)
    sc4af::NoticeBoard notices_;                  // the lifecycle of the notice of each utility slot (power, water, garbage, stagnant garbage backlog): ServiceNotices.h; nothing of it survives the session
    sc4af::CivicNoticeBoard civicBoard_;          // the one notice per facility type: selection, replacement, cooldown, retirement, memory between sessions (CivicBuildingNotices.h, committed behaviour)
    bool civicRestored_ = false;                  // the memory of an earlier session was loaded into civicBoard_ for this city
    int civicCount_[sc4af::kCivicKinds] = {};     // affected buildings per type at the last evaluation (log on change only)
    bool noticeSessionLogged_ = false;            // the "city session started" line of the notices was logged for this session
    bool listedLogged_[sc4af::kNoticeSlots] = {}; // the "the advisor lists the notice" line was logged since the last push (once per notice)
    bool enabledKnown_[sc4af::kNoticeSlots] = {}, enabledNow_[sc4af::kNoticeSlots] = {};      // the management switch of each slot at the last look (log on change only)
    static constexpr int kNoticePollsPerDay = 40;   // how many times between two simulated days the poll may look again at a notice that must still be withdrawn (a paused game never ends the day)
    int noticePollBudget_ = kNoticePollsPerDay;
    std::uint32_t unifiedRanDay_ = 0xFFFFFFFFu;   // the simulated day on which the unified controller last read the counters: its evidence is only valid that day
    std::vector<std::string> linkVariables_;      // the names of the advice local values, kept for the life of the plugin (the game may keep the pointer of a name)
    bool advisorApiBroken_ = false;               // a native advisor call raised an exception: those calls are not used again in this session
    std::uint32_t simDays_ = 0;                // simulated days since the city opened
    sc4af::NoticeMemory told_;                 // DepartmentEfficiency.told.txt: the record of the landfill advisor (outside the saves)
    std::string toldPath_, cityKey_, dataDir_;
    std::string landfillKey_;                  // key under which the landfill advisor record is persisted (empty = session only)
    bool landfillRestored_ = false, landfillKeyLogged_ = false;
    sc4af::CivicFundingWriter writer_;
    sc4af::AutoFinance autoFinance_;
    sc4af::UtilityProbe utility_;
    sc4af::UtilityWriter utilityWriter_;
    sc4af::UtilityShadow utilityShadow_;
    sc4af::UtilityAutoController utilityAuto_;
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
    // The unified controller needs the compile-time capability to exist at all (the product DLL has it; it still needs EnableUnifiedAuto + Mode=Auto at runtime).
    std::unique_ptr<sc4af::UnifiedAutoController> unifiedAuto_ =
        std::make_unique<sc4af::UnifiedAutoController>(sc4af::UnifiedAutoValidationSeam::Arm());
#endif
#ifdef SC4AF_ENABLE_FINAL_AUTO_VALIDATION
    std::string lastReadiness_, notifiedReadiness_;
#endif
    sc4af::Phase8RuntimeRecorder phase8_;
    std::uint32_t citySerial_ = 0;
    std::uint32_t observedMonths_ = 0;
    std::uint32_t daysInCity_ = 0;
};

VOID CALLBACK PollTimerThunk(HWND, UINT, UINT_PTR, DWORD) {
    if (gDirector) gDirector->OnPollTimer();
}

VOID CALLBACK UiTimerThunk(HWND, UINT, UINT_PTR, DWORD) {
    if (gDirector) gDirector->OnUiTimer();
}
} // namespace

extern "C" BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) gModule = module;
    return TRUE;
}

cRZCOMDllDirector* RZGetCOMDllDirector() {
    static AutoFinanceDirector director;
    return &director;
}
