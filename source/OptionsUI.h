// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// The plugin's options window, reached from the game's own Play Options window.
//
// NOTHING of the game is replaced. When the game shows its Play Options window (its panel has the window id 0x2A57DB84), the plugin adds ONE button, in the style of the game's
// "Credits" button, right under it: the panel grows by one button height and the game's Accept / Cancel bar moves down by the same amount. The button opens the plugin's own window,
// built only from the game's controls: the Play Options check boxes, the Budget window's funding spinners for the reserve percentages, the Play Options Accept / Cancel bar.
// Nothing changes before Accept; Accept writes the changed keys of the three ini files (OptionsModel.h) and makes the plugin read them at once; Cancel writes nothing.
// Both UI scripts and the texts are in DepartmentEfficiencyUI.dat (derived from the game's own scripts at build time); without that DAT nothing is added.
//
// Mechanism (the one of 0xC0000054's sc4-region-census, MIT): SC4UI::CreateWindowFromScript + cIGZWin::ChildAdd; the clicks reach a cRZBaseWinProc attached with cIGZWinGen::SetWinProc as
// command messages (type 3, data1 = the notification, data2 = the control id).
#pragma once

#include "OptionsModel.h"
#include "cRZBaseWinProc.h"

#include <cstdint>
#include <functional>
#include <set>
#include <string>

class cIGZWin;

namespace sc4af {

class OptionsUI final : public cRZBaseWinProc {
public:
    // The folder of the two ini files, and what the plugin does after Accept wrote them (read them again at once).
    void SetConfigDirectory(const std::string& dir) { configDir_ = dir; }
    void SetOnApplied(std::function<void()> fn) { onApplied_ = std::move(fn); }

    // Called by the plugin's UI timer (main thread, 250 ms): adds the button to a Play Options window the game has just created. One recursive id lookup when there is
    // nothing to do.
    void Tick();
    // The city is closing / the game is quitting: the plugin's window is closed without writing anything.
    void Reset();

    bool DoWinProcMessage(cIGZWin* pWin, cGZMessage& msg) override;

    // One text of DepartmentEfficiencyUI.dat in the game's language (English when that language has no translation); empty when the DAT is missing.
    static std::string Text(std::uint32_t id);
    static std::string LanguageText();   // "language=0x23 (Portuguese)" for the log

    // The global switch "Department active" (the ordinance reads and writes it): the same files and checks as the window's Accept.
    bool MasterIsOn() const;
    bool SetMaster(bool on);

private:
    bool InjectButton(cIGZWin* panel);
    bool OpenWindow();
    void CloseWindow(const char* why);
    void Accept();
    bool WriteResult(const OptionsApplyResult& r, std::string& keys, std::string& error);
    void Control(std::uint32_t notification, std::uint32_t controlId);
    void PollSpinners();
    void ShowPage(int page);
    std::string ClosePlayOptions();
    int ShowDraft();                     // writes the pending choices into the controls; returns how many controls were written
    OptionsIniTexts ReadFiles() const;
    static std::string Describe(cIGZWin* w, const char* name);   // "name=(l,t,r,b)" for the log

    std::string configDir_;
    std::function<void()> onApplied_;
    int page_ = 0;
    bool syncing_ = false;               // ShowDraft is writing the controls: their own notifications are ignored
    OptionsDraft draft_;                 // the player's pending choices (what the window shows)
    cIGZWin* window_ = nullptr;          // the plugin's window while it exists (we hold a reference)
    cIGZWin* windowParent_ = nullptr;
    cIGZWin* playOptionsParent_ = nullptr;   // the parent of the Play Options window just closed: our window opens in the same layer
    bool openRequested_ = false;           // our button was clicked: open at the next UI tick
    int commandsLogged_ = 0;
    std::set<std::uint32_t> seenTypes_;  // message types already logged once
    bool layoutLogged_ = false, broken_ = false;   // broken_: a creation or a lookup failed, the button is not tried again in this session
};

}  // namespace sc4af
