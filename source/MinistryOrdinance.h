// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// The Department as an ordinance: a shortcut in the game's Ordinances window. It is the same switch as the Department's on/off setting in the plugin's options
// window (one global switch, shared by the three ini files): enacting it switches the Department on everywhere, repealing it switches it off, and when the switch
// changes in the options window the ordinance follows. Monthly cost 0, available from the start, name and description in the game's language.
//
// A city must never depend on the plugin. The game saves its ordinances in the city file; this one never reaches a file:
//   * it is taken out of the ordinance simulator before every save (kSC4MessagePreSave), put back after it (kSC4MessagePostSave), and removed before the city closes;
//   * it is not a serializable class (no cIGZSerializable, no class object registered with the framework), so the game can neither write nor rebuild it.
// A city saved with the plugin therefore opens exactly the same without it.
//
// The cISC4Ordinance contract follows 0xC0000054's sc4-humane-society-ordinance (MIT): the ordinance simulator calls CheckConditions / SetAvailable / Simulate /
// GetMonthlyAdjustedIncome every month and SetOn when the player enacts or repeals it.
#pragma once

#include "cISC4Ordinance.h"
#include "cISCPropertyHolder.h"
#include "cRZBaseString.h"

#include <cstdint>
#include <functional>
#include <string>

class cISC4City;

namespace sc4af {

constexpr std::uint32_t kMinistryOrdinanceId = 0x5C4A6F01;   // the ordinance's class id (plugin range 0x5C4A....; no built-in ordinance uses it)

// No properties: the ordinance has no game effect (no mayor rating, no demand); it only toggles the plugin.
class NoProperties final : public cISCPropertyHolder {
public:
    bool QueryInterface(std::uint32_t riid, void** ppvObj) override;
    std::uint32_t AddRef() override { return ++refs_; }
    std::uint32_t Release() override { return refs_ > 0 ? --refs_ : 0; }
    bool HasProperty(std::uint32_t) const override { return false; }
    bool GetPropertyList(cIGZUnknownList**) const override { return false; }
    cISCProperty* GetProperty(std::uint32_t) const override { return nullptr; }
    bool GetProperty(std::uint32_t, std::uint32_t&) const override { return false; }
    bool GetProperty(std::uint32_t, cIGZString&) const override { return false; }
    bool GetProperty(std::uint32_t, std::uint32_t, void**) const override { return false; }
    bool GetProperty(std::uint32_t, void*, std::uint32_t&) const override { return false; }
    bool AddProperty(cISCProperty*, bool) override { return false; }
    bool AddProperty(std::uint32_t, cIGZVariant const*, bool) override { return false; }
    bool AddProperty(std::uint32_t, std::uint32_t, bool) override { return false; }
    bool AddProperty(std::uint32_t, cIGZString const&) override { return false; }
    bool AddProperty(std::uint32_t, std::int32_t, bool) override { return false; }
    bool AddProperty(std::uint32_t, void*, std::uint32_t, bool) override { return false; }
    bool CopyAddProperty(cISCProperty*, bool) override { return false; }
    bool RemoveProperty(std::uint32_t) override { return false; }
    bool RemoveAllProperties() override { return true; }
    bool EnumProperties(FunctionPtr1, void*) const override { return true; }
    bool EnumProperties(FunctionPtr2, FunctionPtr1) const override { return true; }
    bool CompactProperties() override { return true; }
private:
    std::uint32_t refs_ = 0;
};

class MinistryOrdinance final : public cISC4Ordinance {
public:
    // Called when the player enacts / repeals the ordinance (writes the global switch). Not called while the plugin itself sets the state.
    void SetOnPlayerToggle(std::function<void(bool)> fn) { onPlayerToggle_ = std::move(fn); }

    // Plugin side (the city's ordinance simulator): add at city start and after a save, remove before a save and before the city closes, follow the switch.
    void AddToCity(cISC4City* city, bool on, const char* why);
    void RemoveFromCity(cISC4City* city, const char* why);
    void FollowSwitch(cISC4City* city, bool on);
    bool InCity() const { return inCity_; }

    // cIGZUnknown
    bool QueryInterface(std::uint32_t riid, void** ppvObj) override;
    std::uint32_t AddRef() override { return ++refs_; }
    std::uint32_t Release() override { return refs_ > 0 ? --refs_ : 0; }
    // cISC4Ordinance
    bool Init() override;
    bool Shutdown() override;
    std::int64_t GetCurrentMonthlyIncome() override { return 0; }
    std::uint32_t GetID() const override { return kMinistryOrdinanceId; }
    cIGZString* GetName() override { return &name_; }
    cIGZString* GetDescription() override { return &description_; }
    std::uint32_t GetYearFirstAvailable() override { return 0; }
    SC4Percentage GetChanceAvailability() override;
    std::int64_t GetEnactmentIncome() override { return 0; }
    std::int64_t GetRetracmentIncome() override { return 0; }
    std::int64_t GetMonthlyConstantIncome() override { return 0; }
    float GetMonthlyIncomeFactor() override { return 0.0f; }
    std::uint32_t GetAdvisorID() override { return 0; }
    cISCPropertyHolder* GetMiscProperties() override { return &properties_; }
    bool IsAvailable() override { return available_; }
    bool IsOn() override { return available_ && on_; }
    bool IsEnabled() override { return enabled_; }
    std::int64_t GetMonthlyAdjustedIncome() override { return 0; }
    bool CheckConditions() override { return enabled_; }   // always available (year 0, no building needed)
    bool IsIncomeOrdinance() override { return true; }   // listed on the income side of the Ordinances window; the income itself is 0
    bool Simulate() override { return true; }
    bool SetAvailable(bool isAvailable) override { available_ = isAvailable; return true; }
    bool SetOn(bool isOn) override;
    bool SetEnabled(bool isEnabled) override { enabled_ = isEnabled; return true; }
    bool ForceAvailable(bool isAvailable) override { return SetAvailable(isAvailable); }
    bool ForceOn(bool isOn) override { return SetOn(isOn); }
    bool ForceMonthlyAdjustedIncome(std::int64_t) override { return true; }

private:
    void LoadTexts();

    std::function<void(bool)> onPlayerToggle_;
    cRZBaseString name_, description_;
    NoProperties properties_;
    std::uint32_t refs_ = 0;
    bool available_ = true, on_ = false, enabled_ = true;
    bool inCity_ = false;
    bool syncing_ = false;      // the plugin itself is setting the state: SetOn is not a player action
};

}  // namespace sc4af
