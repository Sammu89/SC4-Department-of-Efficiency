// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "OptionsUI.h"

#include "Logging.h"

#include "GZServPtrs.h"
#include "SC4UI.h"
#include "cGZMessage.h"
#include "cGZPersistResourceKey.h"
#include "cIGZWin.h"
#include "cIGZWinGen.h"
#include "cIGZLanguageManager.h"
#include "cIGZWinBtn.h"
#include "cIGZWinSpinner.h"
#include "cIGZWinText.h"
#include "StringResourceKey.h"
#include "StringResourceManager.h"
#include "cISC4App.h"
#include "cRZAutoRefCount.h"
#include "cRZBaseString.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <windows.h>

namespace sc4af {
namespace {

constexpr std::uint32_t kGZWin_MessageType_Command = 3;
constexpr std::uint32_t kGZWin_Command_ButtonClicked = 0x287259F6;   // a button sends ...F9, F8, F7 while it is pressed and F6 when it is clicked
constexpr std::uint32_t kGZWin_MessageType_SpinnerValue = 0x682876C9;  // a spinner whose value changed: data1 = its id, data2 = the value
constexpr std::uint32_t kGZWin_WinSC4App = 0x6104489A;          // the game's application window (SC4UI.cpp)
constexpr std::uint32_t kScriptWinClsid = 0x0B1E1F95;             // the window class sc4-region-census passes to CreateWindowFromScript (cGZWinGen 0x4386D516 fails)

cIGZWin* MainWindow() {
    cISC4AppPtr app;
    return app ? app->GetMainWindow() : nullptr;
}

// Creates a window from one of our UI scripts as a child of parent and returns it with one reference for the caller, or nullptr.
// With a parent, CreateWindowFromScript returns false and no pointer, for our scripts and for the game's own Play Options script, yet the window is built
// and attached to the parent. So the result is the root found by its id in the parent, whatever the call returned.
cIGZWin* CreateFromScript(std::uint32_t instance, std::uint32_t rootId, cIGZWin* parent, std::string& how) {
    cGZPersistResourceKey key(0x00000000, kOptionsUiGroup, instance);
    cIGZWin* win = nullptr;
    const bool ok = SC4UI::CreateWindowFromScript(key, parent, kScriptWinClsid, GZIID_cIGZWin, reinterpret_cast<void**>(&win));
    how = std::string("CreateWindowFromScript=") + (ok ? "true" : "false") + (win ? " +pointer" : " no pointer");
    if (win) return win;
    if (cIGZWin* built = parent ? parent->GetChildWindowFromID(rootId) : nullptr) {
        built->AddRef();
        how += ", root found in the parent";
        return built;
    }
    return nullptr;
}

// SetArea, never GZWinMoveTo: GZWinMoveTo(GetL(), GetT() + 36) put a bar at (18,532) instead of (9,284); it does not move to an absolute place
void MoveTo(cIGZWin* win, int x, int y) {
    const int w = win->GetW(), h = win->GetH();
    win->SetArea(x, y, x + w, y + h);
}

void SetWinProc(cIGZWin* win, cIGZWinProc* proc) {
    cRZAutoRefCount<cIGZWinGen> gen;
    if (win && win->QueryInterface(GZIID_cIGZWinGen, gen.AsPPVoid())) gen->SetWinProc(proc);
}

// Exception guards: a window call that raises (a wrong id, a window the game destroyed) never takes the game down; the options UI then stops for the session.
cIGZWin* SehFind(cIGZWin* from, std::uint32_t id, bool& raised) {
    __try { return from ? from->GetChildWindowFromIDRecursive(id) : nullptr; } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return nullptr; }
}

// A button click as the game sends it (type 3, data1 = clicked, data2 = the button), to a window's handler. An exception means no click.
bool SendClick(cIGZWin* target, std::uint32_t buttonId) {
    cGZMessage msg(kGZWin_MessageType_Command);
    msg.dwData1 = kGZWin_Command_ButtonClicked;
    msg.dwData2 = buttonId;
    __try { return target->DoMessage(msg); } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

// Caption of a text or button control anywhere under `root`; false when the control is not there.
bool SetControlCaption(cIGZWin* root, std::uint32_t id, const std::string& caption) {
    if (!root) return false;
    cRZBaseString s(caption.c_str());
    cRZAutoRefCount<cIGZWinBtn> btn;                                         // a button first: its own caption call, never a text interface of the button
    if (root->GetChildAsRecursive(id, GZIID_cIGZWinBtn, btn.AsPPVoid())) return btn->SetCaption(s), true;
    cRZAutoRefCount<cIGZWinText> text;
    if (root->GetChildAsRecursive(id, GZIID_cIGZWinText, text.AsPPVoid())) return text->SetCaption(s), true;
    return false;
}

void SetEnabled(cIGZWin* root, std::uint32_t id, bool enabled) {
    if (cIGZWin* w = root ? root->GetChildWindowFromIDRecursive(id) : nullptr) w->SetFlag(cIGZWin::WinFlag_Enabled, enabled);
}

std::string ReadFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::string();
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

// The whole file or nothing: the new text goes to a temporary file first, which then replaces the old one.
bool WriteFileAtomic(const std::string& path, const std::string& text, std::string& error) {
    const std::string tmp = path + ".options-tmp";
    {
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        out << text;
        if (!out.good()) { error = "cannot write " + tmp; return false; }
    }
    if (!MoveFileExA(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        error = "cannot replace " + path + " (error " + std::to_string(GetLastError()) + ")";
        DeleteFileA(tmp.c_str());
        return false;
    }
    return true;
}

std::string DraftText(const OptionsDraft& d) {
    static const char* names[] = {"Master", "Education", "EducationReserve", "Health", "HealthReserve", "Power", "PowerReserve", "PowerAging",
                                              "Water", "WaterReserve", "WaterPipes", "Sanitation", "SanitationReserve", "Deals", "Arbitrage",
                                              "Destroyed", "DestroyedMonths", "DemolishMunicipal", "Abandoned", "AbandonedMonths",
                                              "ZoneR", "ZoneC", "ZoneI", "ZoneA", "ReasonDesirability", "ReasonCommute", "ReasonDemand",
                                              "ReasonPower", "ReasonWater", "BudgetLimit", "DemolitionBudget", "TradePower", "TradeWater", "TradeGarbage", "LandfillAdvice", "LandfillThreshold"};
    static_assert(sizeof(names) / sizeof(names[0]) == kOptionCount, "one name per option");
    std::string s;
    for (int i = 0; i < kOptionCount; ++i) s += (i ? " " : "") + std::string(names[i]) + "=" + std::to_string(d.at(i));
    return s;
}

std::string NumberText(const OptionSpec& s, int value) {
    if (s.kind == OptionKind::Percent) return FillText(OptionsUI::Text(kTextPercent), std::to_string(value));
    if (s.kind == OptionKind::Money) return FillText(OptionsUI::Text(kTextMoney), FormatThousands(value, OptionsUI::Text(kTextThousands)));
    return value == 1 ? OptionsUI::Text(kTextOneMonth) : FillText(OptionsUI::Text(kTextMonths), std::to_string(value));
}

}  // namespace

std::string OptionsUI::Text(std::uint32_t id) {
    cRZAutoRefCount<cIGZString> s;
    if (!StringResourceManager::GetLocalizedString(StringResourceKey(kOptionsUiTextGroup, id), s) || !s || !s->ToChar()) return std::string();
    return std::string(s->ToChar(), s->Strlen());
}

std::string OptionsUI::LanguageText() {
    cIGZLanguageManagerPtr lm;
    if (!lm) return "language=unknown";
    const std::uint32_t id = lm->GetCurrentLanguage();
    cRZBaseString name;
    lm->GetLanguageEnglishName(name, id);
    char hex[16];
    std::snprintf(hex, sizeof hex, "0x%02X", static_cast<unsigned>(id));
    return std::string("language=") + hex + " (" + (name.ToChar() ? name.ToChar() : "?") + ")";
}

void OptionsUI::Tick() {
    if (broken_) return;
    if (window_) PollSpinners();
    if (openRequested_) {
        openRequested_ = false;
        const std::string closed = ClosePlayOptions();
        if (OpenWindow()) Log(LogLevel::Info, "[OptionsUI] Play Options: " + closed);
    }
    bool raised = false;
    cIGZWin* main = MainWindow();
    cIGZWin* panel = SehFind(main, kPlayOptionsPanel, raised);
    if (raised) { broken_ = true; Log(LogLevel::Warning, "[OptionsUI] looking for the Play Options window raised an exception: the options button is off for this session"); return; }
    if (!panel || panel->GetChildWindowFromIDRecursive(kOptionsUiButtonRoot)) return;   // no Play Options window, or the button is already in it
    InjectButton(panel);
}

bool OptionsUI::InjectButton(cIGZWin* panel) {
    cIGZWin* credits = panel->GetChildWindowFromIDRecursive(kPlayOptionsCredits);
    cIGZWin* content = credits ? credits->GetParentWin() : nullptr;
    cIGZWin* footer = panel->GetChildWindowFromID(kPlayOptionsFooter);
    if (!content || !footer) {
        if (!layoutLogged_) { layoutLogged_ = true; Log(LogLevel::Warning, "[OptionsUI] the Play Options window does not have the expected layout: it is left as it is"); }
        return false;
    }
    std::string how;
    cIGZWin* button = CreateFromScript(kOptionsUiButtonScript, kOptionsUiButtonRoot, content, how);
    if (!button) {
        broken_ = true;                                                      // never retried in this session: Play Options stays the game's own
        Log(LogLevel::Warning, "[OptionsUI] the options button could not be created (" + how + "; is DepartmentEfficiencyUI.dat in the Department of Efficiency folder?): Play Options is left as it is");
        return false;
    }
    // room for one more button: the panel grows, the game's Accept / Cancel bar moves down, nothing of the game is replaced
    panel->SetH(panel->GetH() + kOptionsUiGrow);
    content->SetH(content->GetH() + kOptionsUiGrow);
    MoveTo(footer, footer->GetL(), footer->GetT() + kOptionsUiGrow);
    if (!content->ChildExists(button)) content->ChildAdd(button);
    MoveTo(button, credits->GetL(), credits->GetB() + 6);
    SetWinProc(button, this);
    const bool named = SetControlCaption(button, kOptionsUiButton, Text(kTextName));
    button->ShowWindow();
    Log(LogLevel::Info, "[OptionsUI] the Department of Efficiency button was added to the game's Play Options window (" + how + "; " + LanguageText() + ", caption '" +
                            Text(kTextName) + "'" + (named ? "" : " NOT SET") + "): " + Describe(button, "button") + " " +
                            Describe(content, "content") + " " + Describe(panel, "panel") + " " + Describe(footer, "footer"));
    button->Release();                                                       // the content window holds it now
    return true;
}

std::string OptionsUI::Describe(cIGZWin* w, const char* name) {
    if (!w) return std::string(name) + "=none";
    return std::string(name) + "=(" + std::to_string(w->GetL()) + "," + std::to_string(w->GetT()) + "," + std::to_string(w->GetR()) + "," + std::to_string(w->GetB()) +
           (w->IsVisible() ? ")" : " hidden)");
}

bool OptionsUI::DoWinProcMessage(cIGZWin*, cGZMessage& msg) {
    if (msg.dwMessageType == kGZWin_MessageType_SpinnerValue && window_ && msg.dwData1 >= kOptionsUiSpin0 && msg.dwData1 < kOptionsUiSpin0 + kOptionCount) {
        PollSpinners();                                                      // data1 = the spinner, data2 = its new value; read back from the control
        return true;
    }
    if (msg.dwMessageType != kGZWin_MessageType_Command) {
        // diagnostics: the first message of each other type only
        if (seenTypes_.size() < 40 && seenTypes_.insert(msg.dwMessageType).second) {
            char line[128];
            std::snprintf(line, sizeof line, "[OptionsUI] message type 0x%08X data 0x%08X 0x%08X", static_cast<unsigned>(msg.dwMessageType),
                          static_cast<unsigned>(msg.dwData1), static_cast<unsigned>(msg.dwData2));
            Log(LogLevel::Info, line);
        }
        return false;
    }
    if (commandsLogged_ < 60) {                                              // diagnostics: what the game's controls send (bounded)
        ++commandsLogged_;
        char line[96];
        std::snprintf(line, sizeof line, "[OptionsUI] command 0x%08X control 0x%08X", static_cast<unsigned>(msg.dwData1), static_cast<unsigned>(msg.dwData2));
        Log(LogLevel::Info, line);
    }
    Control(msg.dwData1, msg.dwData2);
    return true;
}

void OptionsUI::Control(std::uint32_t notification, std::uint32_t id) {
    const bool clicked = notification == kGZWin_Command_ButtonClicked;
    // the window opens at the next UI tick: Play Options is closed first, and never while one of its own buttons is still handling this click
    if (id == kOptionsUiButton) { if (clicked) openRequested_ = true; return; }
    if (!window_) return;
    if (id == kOptionsUiAccept) { if (clicked) Accept(); return; }
    if (id == kOptionsUiCancel) { if (clicked) CloseWindow("Cancel: nothing written"); return; }
    if (id >= kOptionsUiTab0 && id < kOptionsUiTab0 + kOptionsUiPages) { if (clicked) ShowPage(static_cast<int>(id - kOptionsUiTab0)); return; }
    if (id >= kOptionsUiCheck0 && id < kOptionsUiCheck0 + kOptionCount) {
        if (!clicked) return;
        const int i = static_cast<int>(id - kOptionsUiCheck0);
        draft_.toggle(i);
        Log(LogLevel::Info, "[OptionsUI] check box " + std::to_string(i) + " -> " + std::to_string(draft_.value[static_cast<std::size_t>(i)]) + " (pending until Accept)");
        ShowDraft();                                                         // the box itself, and the sub-options of a manager
        return;
    }
    if (id >= kOptionsUiSpin0 && id < kOptionsUiSpin0 + kOptionCount) PollSpinners();   // the spinner changed its own value: read it, whatever the notification
}

// A spinner changes its own value when its arrows are clicked; whatever it notifies, the value is read back here (UI timer) and becomes the pending choice.
void OptionsUI::PollSpinners() {
    if (syncing_) return;                                                    // the plugin itself is writing the controls: their notifications are not the player's
    bool changed = false;
    for (const auto& s : OptionSpecs()) {
        if (s.kind == OptionKind::Check) continue;
        cRZAutoRefCount<cIGZWinSpinner> spin;
        if (!window_->GetChildAsRecursive(kOptionsUiSpin0 + static_cast<std::uint32_t>(s.index), GZIID_cIGZWinSpinner, spin.AsPPVoid())) continue;
        const int shown = spin->GetValue(), before = draft_.value[static_cast<std::size_t>(s.index)];
        if (shown == before) continue;
        draft_.setNumber(s.index, shown);
        changed = true;
        Log(LogLevel::Info, "[OptionsUI] spinner " + std::to_string(s.index) + " -> " + std::to_string(draft_.at(s.index)) + " (pending until Accept)");
    }
    if (changed) ShowDraft();
}

// Play Options is closed the way the player would: its own Cancel, sent to the handler its Cancel button notifies (nothing of Play Options is changed). If no handler
// closes it, it is hidden. Returns what happened, for the log.
std::string OptionsUI::ClosePlayOptions() {
    bool raised = false;
    cIGZWin* main = MainWindow();
    cIGZWin* panel = SehFind(main, kPlayOptionsPanel, raised);
    cIGZWin* root = panel ? panel->GetParentWin() : nullptr;
    if (!root || !root->IsVisible()) return "not open";
    playOptionsParent_ = root->GetParentWin();
    cIGZWin* cancel = root->GetChildWindowFromIDRecursive(kPlayOptionsCancel);
    cIGZWin* target = cancel ? cancel->GetNotificationTarget() : nullptr;
    if (!target && cancel) target = cancel->GetParentWin();
    if (target && SendClick(target, kPlayOptionsCancel)) {
        cIGZWin* still = SehFind(main, kPlayOptionsPanel, raised);
        if (!still || !still->IsVisible() || !still->GetParentWin() || !still->GetParentWin()->IsVisible()) return "closed by its own Cancel";
    }
    root->HideWindow();
    return "hidden (its Cancel handler did not close it)";
}

OptionsIniTexts OptionsUI::ReadFiles() const {
    return {ReadFile(configDir_ + "DepartmentEfficiency.ini"), ReadFile(configDir_ + "DepartmentEfficiencyUtility.ini"), ReadFile(configDir_ + "DepartmentEfficiencyDemolition.ini")};
}

bool OptionsUI::OpenWindow() {
    if (window_) { window_->ShowWindow(); window_->PullToFront(); return true; }
    bool raised = false;
    cIGZWin* main = MainWindow();
    cIGZWin* app = SehFind(main, kGZWin_WinSC4App, raised);
    cIGZWin* parent = playOptionsParent_ ? playOptionsParent_ : app ? app : main;   // the layer of Play Options: our window is not under other game windows
    playOptionsParent_ = nullptr;
    if (!parent) return false;
    std::string how;
    cIGZWin* win = CreateFromScript(kOptionsUiWindowScript, kOptionsUiWindowRoot, parent, how);
    if (!win) {
        Log(LogLevel::Warning, "[OptionsUI] the options window could not be created (" + how + "; is DepartmentEfficiencyUI.dat in the Department of Efficiency folder?)");
        return false;
    }
    if (!parent->ChildExists(win)) parent->ChildAdd(win);
    MoveTo(win, (parent->GetW() - win->GetW()) / 2, (parent->GetH() - win->GetH()) / 2);   // centred in the game's window
    SetWinProc(win, this);
    window_ = win;                                                           // our reference
    windowParent_ = parent;
    // A control notifies its parent; ours sit in the content background and in the Accept / Cancel bar, which have no handler (clicks on the check boxes
    // would never reach the window's handler): every control notifies the window instead.
    int targets = 0;
    const auto target = [&](std::uint32_t id) {
        if (cIGZWin* c = win->GetChildWindowFromIDRecursive(id)) targets += c->SetNotificationTarget(win) ? 1 : 0;
    };
    for (const auto& s : OptionSpecs()) target((s.kind == OptionKind::Check ? kOptionsUiCheck0 : kOptionsUiSpin0) + static_cast<std::uint32_t>(s.index));
    for (int p = 0; p < kOptionsUiPages; ++p) target(kOptionsUiTab0 + static_cast<std::uint32_t>(p));
    target(kOptionsUiAccept);
    target(kOptionsUiCancel);
    draft_ = DraftFromTexts(ReadFiles());                                    // what the plugin does now
    const bool titled = SetControlCaption(win, kOptionsUiTitle, Text(kTextName));
    int labels = 0;
    for (const auto& s : OptionSpecs()) labels += SetControlCaption(win, kOptionsUiLabel0 + static_cast<std::uint32_t>(s.index), Text(kTextLabel0 + static_cast<std::uint32_t>(s.index))) ? 1 : 0;
    labels += SetControlCaption(win, kOptionsUiTab0, Text(kTextPageServices)) ? 1 : 0;
    labels += SetControlCaption(win, kOptionsUiTab0 + 1, Text(kTextPageDemolition)) ? 1 : 0;
    ShowPage(0);
    const int controls = ShowDraft();
    // tooltips, for a player who never used the plugin: "label|explanation" (the game's own title|text form), on the label (the whole row up to the value) and on the check box
    int tips = 0;
    for (const auto& s : OptionSpecs()) {
        const auto i = static_cast<std::uint32_t>(s.index);
        cRZBaseString tip((Text(kTextLabel0 + i) + "|" + Text(kTextTip0 + i)).c_str());
        for (const std::uint32_t id : {kOptionsUiTip0 + i, kOptionsUiCheck0 + i}) {
            cRZAutoRefCount<cIGZWinBtn> b;
            if (win->GetChildAsRecursive(id, GZIID_cIGZWinBtn, b.AsPPVoid()) && b->SetTipText(tip)) ++tips;
        }
    }
    win->PullToFront();
    win->ShowWindow();
    // log what was actually written into the window's controls, not what was meant to be
    Log(LogLevel::Info, "[OptionsUI] the Department of Efficiency window was opened (" + how + "; " + LanguageText() + "): " + Describe(win, "window") + " title " +
                            (titled ? "set" : "NOT SET") + ", labels set " + std::to_string(labels) + "/" + std::to_string(kOptionCount + kOptionsUiPages) + ", controls set " +
                            std::to_string(controls) + "/" + std::to_string(kOptionCount) + ", notification targets " + std::to_string(targets) + "/" +
                            std::to_string(kOptionCount + kOptionsUiPages + 2) + ", tooltips set " + std::to_string(tips) + "; " + DraftText(draft_));
    return true;
}

int OptionsUI::ShowDraft() {
    if (!window_) return 0;
    // SetMinMax / SetValue make the spinner notify "value changed" at once, with the script's initial value (the minimum). Read back, that would turn the
    // pending choices into the minimums (health margin 0%, power reserve 10%, abandoned after 1 month) and Accept would write them. While the controls are
    // written, those notifications are ignored.
    syncing_ = true;
    int written = 0;
    for (const auto& s : OptionSpecs()) {
        const auto i = static_cast<std::uint32_t>(s.index);
        const bool enabled = draft_.enabled(s.index);
        SetEnabled(window_, kOptionsUiLabel0 + i, enabled);
        if (s.kind == OptionKind::Check) {
            cRZAutoRefCount<cIGZWinBtn> box;
            if (!window_->GetChildAsRecursive(kOptionsUiCheck0 + i, GZIID_cIGZWinBtn, box.AsPPVoid())) continue;
            const bool on = draft_.on(s.index);
            if (on) box->ToggleOn(); else box->ToggleOff();
            box->SetChecked(on);
            SetEnabled(window_, kOptionsUiCheck0 + i, enabled);
            ++written;
        } else {
            cRZAutoRefCount<cIGZWinSpinner> spin;
            if (!window_->GetChildAsRecursive(kOptionsUiSpin0 + i, GZIID_cIGZWinSpinner, spin.AsPPVoid())) continue;
            const int value = draft_.at(s.index);
            spin->SetStepSize(s.step);
            spin->SetMinMax(s.min, s.max);
            if (spin->GetValue() != value) spin->SetValue(value);
            SetEnabled(window_, kOptionsUiSpin0 + i, enabled);
            SetEnabled(window_, kOptionsUiValue0 + i, enabled);
            if (SetControlCaption(window_, kOptionsUiValue0 + i, NumberText(s, value)) && spin->GetValue() == value) ++written;
        }
    }
    syncing_ = false;
    return written;
}

// Writes the files of an accepted result (each one whole or not at all) and makes the plugin read them; `keys` = what was written. False = nothing more is written.
bool OptionsUI::WriteResult(const OptionsApplyResult& r, std::string& keys, std::string& error) {
    if (r.civicChanged && !WriteFileAtomic(configDir_ + "DepartmentEfficiency.ini", r.texts.civic, error)) return false;
    if (r.utilityChanged && !WriteFileAtomic(configDir_ + "DepartmentEfficiencyUtility.ini", r.texts.utility, error)) return false;
    if (r.demolitionChanged && !WriteFileAtomic(configDir_ + "DepartmentEfficiencyDemolition.ini", r.texts.demolition, error)) return false;
    keys.clear();
    for (const auto& k : r.keys) keys += (keys.empty() ? "" : ", ") + k;
    if ((r.civicChanged || r.utilityChanged || r.demolitionChanged) && onApplied_) onApplied_();   // the plugin reads the files at once (no wait for the next month)
    return true;
}

void OptionsUI::Accept() {
    const OptionsIniTexts current = ReadFiles();
    const OptionsApplyResult r = ApplyOptions(current, draft_);
    if (!r.ok) {                                                             // nothing is written; the window stays open with the player's choices
        Log(LogLevel::Warning, "[OptionsUI] Accept refused, nothing written: " + r.error);
        return;
    }
    std::string keys, error;
    if (!WriteResult(r, keys, error)) { Log(LogLevel::Error, "[OptionsUI] Accept failed: " + error); return; }
    if (!keys.empty()) CloseWindow(("Accept: wrote " + keys + "; " + DraftText(draft_)).c_str());
    else CloseWindow("Accept: nothing changed, nothing written");
}

bool OptionsUI::MasterIsOn() const { return DraftFromTexts(ReadFiles()).on(kOptMaster); }

// The master switch from elsewhere (the Department ordinance): the same edit as the window's Accept with only "Department active" changed.
bool OptionsUI::SetMaster(bool on) {
    OptionsDraft want = DraftFromTexts(ReadFiles());
    if (want.on(kOptMaster) == on) return true;
    want.value[kOptMaster] = on ? 1 : 0;
    const OptionsApplyResult r = ApplyOptions(ReadFiles(), want);
    std::string keys, error;
    if (!r.ok || !WriteResult(r, keys, error)) {
        Log(LogLevel::Warning, "[OptionsUI] the master switch could not be written (" + (r.ok ? error : r.error) + "): nothing written");
        return false;
    }
    Log(LogLevel::Info, "[OptionsUI] master switch " + std::string(on ? "on" : "off") + " from the ordinance: wrote " + keys);
    if (window_) { draft_.value[kOptMaster] = on ? 1 : 0; ShowDraft(); }
    return true;
}

void OptionsUI::CloseWindow(const char* why) {
    if (!window_) return;
    SetWinProc(window_, nullptr);
    if (windowParent_) windowParent_->ChildDelete(window_);
    window_->Release();
    window_ = nullptr;
    windowParent_ = nullptr;
    Log(LogLevel::Info, std::string("[OptionsUI] the Department of Efficiency window was closed (") + why + ")");
}

// One page at a time: the page buttons switch the visible content background (the page's controls are its children). The page shown has a dark bar under its
// button: a greyed, disabled button looked switched off, and a toggle button kept the mouse captured and looked the same as the other.
void OptionsUI::ShowPage(int page) {
    if (!window_) return;
    for (int p = 0; p < kOptionsUiPages; ++p) {
        const auto id = static_cast<std::uint32_t>(p);
        if (cIGZWin* bg = window_->GetChildWindowFromIDRecursive(kOptionsUiPage0 + id)) {
            if (p == page) bg->ShowWindow(); else bg->HideWindow();
        }
        if (cIGZWin* mark = window_->GetChildWindowFromIDRecursive(kOptionsUiTabMark0 + id)) {
            if (p == page) mark->ShowWindow(); else mark->HideWindow();
        }
    }
    if (page_ != page) Log(LogLevel::Info, "[OptionsUI] page " + std::to_string(page));
    page_ = page;
}

void OptionsUI::Reset() { CloseWindow("the city or the game is closing: nothing written"); }

}  // namespace sc4af
