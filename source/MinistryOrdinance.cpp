// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "MinistryOrdinance.h"

#include "Logging.h"
#include "OptionsModel.h"
#include "OptionsUI.h"

#include "SC4Percentage.h"
#include "cISC4City.h"
#include "cISC4OrdinanceSimulator.h"

#include <windows.h>

namespace sc4af {
namespace {

cISC4OrdinanceSimulator* Simulator(cISC4City* city) { return city ? city->GetOrdinanceSimulator() : nullptr; }

// Exception guards (no C++ object in these frames): an ordinance call that raises never takes the game down.
bool SehHas(cISC4OrdinanceSimulator* sim, bool& raised) {
    __try { return sim->GetOrdinanceByID(kMinistryOrdinanceId) != nullptr; } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
}
bool SehAdd(cISC4OrdinanceSimulator* sim, cISC4Ordinance* o, bool& raised) {
    __try { return sim->AddOrdinance(*o); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
}
bool SehRemove(cISC4OrdinanceSimulator* sim, cISC4Ordinance* o, bool& raised) {
    __try { return sim->RemoveOrdinance(*o); } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
}
bool SehSetOn(cISC4OrdinanceSimulator* sim, bool on, bool& raised) {
    __try {
        sim->SetOrdinanceAvailable(kMinistryOrdinanceId, true);
        return sim->SetOrdinanceOn(kMinistryOrdinanceId, on);
    } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
}

}  // namespace

bool NoProperties::QueryInterface(std::uint32_t riid, void** ppvObj) {
    if (riid == GZIID_cISCPropertyHolder || riid == GZIID_cIGZUnknown) {
        *ppvObj = static_cast<cISCPropertyHolder*>(this);
        AddRef();
        return true;
    }
    return false;
}

// Only the ordinance interface: NOT cIGZSerializable (the game must never be able to write this ordinance into a city file).
bool MinistryOrdinance::QueryInterface(std::uint32_t riid, void** ppvObj) {
    if (riid == GZIID_cISC4Ordinance || riid == GZIID_cIGZUnknown) {
        *ppvObj = static_cast<cISC4Ordinance*>(this);
        AddRef();
        return true;
    }
    return false;
}

void MinistryOrdinance::LoadTexts() {
    const std::string name = OptionsUI::Text(kTextOrdinanceName), text = OptionsUI::Text(kTextOrdinanceDescription);
    name_.FromChar((name.empty() ? std::string("Establish a Department of Efficiency") : name).c_str());
    description_.FromChar(text.c_str());
}

bool MinistryOrdinance::Init() {
    LoadTexts();
    enabled_ = true;
    return true;
}

bool MinistryOrdinance::Shutdown() { return true; }

SC4Percentage MinistryOrdinance::GetChanceAvailability() {
    SC4Percentage p{1.0f};
    return p;
}

bool MinistryOrdinance::SetOn(bool isOn) {
    const bool changed = on_ != isOn;
    on_ = isOn;
    if (!syncing_ && changed) {
        Log(LogLevel::Info, std::string("[Ordinance] the player ") + (isOn ? "enacted" : "repealed") + " the Ministry ordinance: the Ministry is switched " + (isOn ? "on" : "off"));
        if (onPlayerToggle_) onPlayerToggle_(isOn);
    }
    return true;
}

void MinistryOrdinance::AddToCity(cISC4City* city, bool on, const char* why) {
    cISC4OrdinanceSimulator* sim = Simulator(city);
    if (!sim) return;
    bool raised = false;
    if (SehHas(sim, raised)) { inCity_ = true; FollowSwitch(city, on); return; }
    if (raised) { Log(LogLevel::Warning, "[Ordinance] the ordinance simulator raised an exception: no ordinance in this city"); return; }
    syncing_ = true;
    LoadTexts();
    available_ = true;
    enabled_ = true;
    on_ = on;
    const bool added = SehAdd(sim, this, raised);
    if (added && !raised) Init();
    const bool set = added && !raised && SehSetOn(sim, on, raised);
    syncing_ = false;
    inCity_ = added && !raised;
    Log(inCity_ ? LogLevel::Info : LogLevel::Warning, std::string("[Ordinance] ") + why + ": " + (inCity_ ? "added" : "NOT added") + " (" + (on ? "enacted" : "repealed") +
                                                          (set ? "" : ", state not confirmed") + (raised ? ", EXCEPTION" : "") + ")");
}

void MinistryOrdinance::RemoveFromCity(cISC4City* city, const char* why) {
    cISC4OrdinanceSimulator* sim = Simulator(city);
    if (!sim) { inCity_ = false; return; }
    bool raised = false;
    if (!SehHas(sim, raised)) { inCity_ = false; return; }
    syncing_ = true;
    SehRemove(sim, this, raised);
    syncing_ = false;
    const bool still = !raised && SehHas(sim, raised);
    inCity_ = still;
    // log whether the simulator really dropped it (a save cannot then contain it)
    Log(still || raised ? LogLevel::Warning : LogLevel::Info, std::string("[Ordinance] ") + why + ": " + (still ? "STILL in the ordinance simulator" : raised ? "EXCEPTION" : "taken out of the ordinance simulator"));
}

void MinistryOrdinance::FollowSwitch(cISC4City* city, bool on) {
    cISC4OrdinanceSimulator* sim = Simulator(city);
    if (!sim || !inCity_ || IsOn() == on) return;
    bool raised = false;
    syncing_ = true;
    SehSetOn(sim, on, raised);
    on_ = on;
    syncing_ = false;
    Log(LogLevel::Info, std::string("[Ordinance] follows the switch: ") + (on ? "enacted" : "repealed") + (raised ? " (EXCEPTION)" : ""));
}

}  // namespace sc4af
