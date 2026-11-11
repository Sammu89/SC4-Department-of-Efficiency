// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// The player notices about a shortage of a utility (electricity, water, garbage), kept apart from the economic optimiser. Pure: no game types, unit tested with a virtual clock.
//
// WHAT may reach the player: a PROVEN impossibility. The producer of the evidence (UnifiedAuto) says, every simulated day and per
// service, which of three things it can PROVE now:
//   * Proven     the shortage is demonstrated by current, coherent data and the Department already used (or showed it cannot use) every means it is allowed to use;
//   * Uncertain  the data no longer demonstrate the shortage but do not yet confirm the recovery either (counters that move at different moments, a contract still confirming, an unreadable value,
//                contradictory data, a model that is feasible while the game still reports something to collect). Nothing is concluded: no notice, no reminder, the clocks stand still;
//   * Recovered  the service-specific checks show that the shortage is over.
// A fourth state is not evidence but the player's choice: the management of the service is OFF (its own switch or the master switch). Then the notice of the Department is withdrawn, the reminders are
// cancelled and nothing is shown again until the management is on AND the shortage is proven again with the normal persistence. The regional CONTRACTS are not touched by any of this.
//
// WHEN (simulated days, owner policy 2026-11-10). The first notice waits for the persistence in PROVEN state (clock suspended while UNCERTAIN); the reminders follow after the interval of the table; the interval
// grows with the number of CONFIRMED dismissals (1st, 2nd, 3rd; the last interval is kept after that, no cap on the number of notices). Presentation: the first two notices are pop-ups, the third and later ones
// are discreet news items. Order of importance: electricity, water, garbage.
//
//     service       persistence   after 1st   after 2nd   after 3rd+
//     electricity        15          120         180         240
//     water              30          150         240         360
//     garbage            60          240         360         540
//
// DISMISSAL. The game's advisor interface lets the plugin see whether a notice is listed (the advisor's active flag) and retire it, but a notice that leaves the list may have been dismissed by the player,
// have expired, or have been retired by the plugin: the flag does not say which. The plugin therefore NEVER claims a dismissal: it calls NoteConfirmedDismissal only if a reliable observation exists
// (kDismissalObservable in Plugin.cpp, false today) and otherwise uses the fallback interval of the table, counted from the last emission the advisor accepted. The state keeps the
// four facts apart: emissions, confirmed dismissals, last emission, last retirement confirmed.
//
// SESSION. Nothing of this survives the session: a city that is opened (the same one again included) starts with empty state, and any notice of the Department that an earlier session left listed is withdrawn.

#include "UtilityModel.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <string>

namespace sc4af {

// What the advisor says about one notice record: listed (the advisor's active flag 2 is set: it is in the list of the game's notices), not listed, or unknown (the call failed or the record is not known).
enum class Listing { Listed, NotListed, Unknown };

enum class ShortageEvidence { Uncertain, Proven, Recovered };
inline const char* EvidenceName(ShortageEvidence e) { return e == ShortageEvidence::Proven ? "PROVEN" : e == ShortageEvidence::Recovered ? "RECOVERED" : "UNCERTAIN"; }

// ---- the policy table ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Electricity, water and garbage. (Education and health keep their own earlier policy: CivicBuildingNotices.h, 30 days, discreet, memory between sessions.)
enum class NoticeClass { Power = 0, Water, Garbage };          // in the order of importance
constexpr int kNoticeClasses = 3;

struct NoticePolicy {
    std::uint32_t persist;           // PROVEN days before the first notice
    std::uint32_t after[3];          // days between a dismissal (1st, 2nd, 3rd and later) and the next reminder
    std::uint32_t fallback;          // without an observable dismissal: days from the last emission to the next reminder (equal to after[0])
};
constexpr NoticePolicy kNoticePolicy[kNoticeClasses] = {
    {15, {120, 180, 240}, 120},      // electricity
    {30, {150, 240, 360}, 150},      // water
    {60, {240, 360, 540}, 240},      // garbage
};
constexpr const NoticePolicy& PolicyOf(NoticeClass c) { return kNoticePolicy[static_cast<int>(c)]; }
constexpr std::uint32_t PersistDays(NoticeClass c) { return PolicyOf(c).persist; }
// The interval to the next reminder: with `dismissals` confirmed dismissals (0 = none observed: the fallback of the table). Beyond the third the last interval is kept.
constexpr std::uint32_t ReminderDays(NoticeClass c, int dismissals) {
    return dismissals <= 0 ? PolicyOf(c).fallback : PolicyOf(c).after[dismissals >= 3 ? 2 : dismissals - 1];
}
constexpr int kPopupEmissions = 2;                                                  // notices 1 and 2 are pop-ups, the 3rd and the following are discreet
constexpr bool PopupFor(int emission) { return emission >= 1 && emission <= kPopupEmissions; }

// Slots: 0 power, 1 water, 2 garbage (insufficiency: urgent), 3 garbage with a STAGNANT backlog (informative: always discreet, never a pop-up, same persistence and reminders as the garbage).
constexpr int kUtilitySlots = 3;
constexpr int kStagnantSlot = 3;
constexpr int kNoticeSlots = 4;
constexpr NoticeClass ClassOfSlot(int slot) { return slot == 0 ? NoticeClass::Power : slot == 1 ? NoticeClass::Water : NoticeClass::Garbage; }
constexpr bool DiscreetOnly(int slot) { return slot == kStagnantSlot; }
// Electricity, water, garbage (insufficiency first, then the stagnant backlog).
constexpr std::array<int, kNoticeSlots> kSlotsByImportance = {0, 1, 2, 3};
inline const char* NoticeSlotName(int slot) {
    static const char* names[kNoticeSlots] = {"POWER", "WATER", "GARBAGE", "GARBAGE_STAGNANT"};
    return slot >= 0 && slot < kNoticeSlots ? names[slot] : "?";
}

// ---- the episodes of the three utilities (evidence only) ---------------------------------------------------------------------------------------------------------------------------------------------------
// The state the controller reports per utility, and the transitions the log mentions (once per change, never per day). It decides nothing about the player: the board below does.
class NoticeEpisodes {
public:
    static constexpr int kSlots = kUtilitySlots;
    struct Result {
        bool emit = false;      // the shortage is proven now and was not in the previous evaluation (log line of a proof)
        bool rearmed = false;   // the recovery is confirmed (an episode that was open ends)
        bool uncertain = false; // the evidence changed to uncertain (log line)
        bool resumed = false;   // proven again after an uncertain interval
    };
    void Reset() { for (auto& e : evidence_) e = ShortageEvidence::Uncertain; for (bool& o : open_) o = false; }
    ShortageEvidence evidence(int i) const { return i >= 0 && i < kSlots ? evidence_[i] : ShortageEvidence::Uncertain; }
    bool active(int i) const { return i >= 0 && i < kSlots && open_[i]; }       // a proven shortage has not been closed by a confirmed recovery

    Result Update(int i, ShortageEvidence now) {
        Result r;
        if (i < 0 || i >= kSlots) return r;
        const ShortageEvidence before = evidence_[i];
        evidence_[i] = now;
        if (now == ShortageEvidence::Proven) {
            if (!open_[i]) { open_[i] = true; r.emit = true; }
            else if (before == ShortageEvidence::Uncertain) r.resumed = true;
        } else if (now == ShortageEvidence::Recovered) {
            if (open_[i]) { open_[i] = false; r.rearmed = true; }
        } else if (before == ShortageEvidence::Proven) {
            r.uncertain = true;
        }
        return r;
    }

    // The regional data a proof of impossibility relies on is complete and current. Unread markets, unread proposals or a neighbor whose book of this commodity is incomplete
    // are UNKNOWN alternatives, not absent ones.
    static bool DataComplete(const UtilitySnapshot& s, int commodityIndex) {
        if (commodityIndex < 0 || commodityIndex > 2) return false;
        const auto& d = s.deals;
        if (d.validity != SnapshotValidity::Valid) return false;
        if (!d.marketsRead || !d.proposalsRead) return false;
        if (!d.localMarket.book[commodityIndex].complete()) return false;
        for (const auto& n : d.neighbors) {
            if (!n.suitable[2 * commodityIndex] && !n.suitable[2 * commodityIndex + 1]) continue;   // the game finds no connection for this commodity: it cannot trade it
            if (!n.book[commodityIndex].complete()) return false;
        }
        return true;
    }

private:
    ShortageEvidence evidence_[kSlots] = {};
    bool open_[kSlots] = {};
};

// ---- which management makes a notice exist ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// The notice of a utility exists only while the player has the LOCAL management of that service on (and the Department is able to run it): electricity (AutoPower), water (AutoWater), sanitation (AutoGarbage),
// all of them under the master switch (the unified controller runs only in Mode=Auto). The regional CONTRACTS (AutoDeals and AutoTrade*) are a different thing: they are never a reason for a notice and a notice
// never changes them. Nothing here touches a contract.
struct UtilityManagement {
    bool controllerRuns = false;                       // the master switch is on and the unified controller is active (UnifiedGateOpen)
    bool sessionFailed = false;                        // the unified session failed (fail-closed)
    bool localOn[3] = {false, false, false};           // AutoPower, AutoWater, AutoGarbage
    bool failedClosed[3] = {false, false, false};      // the funding writer of the service was shut down after a technical failure
    bool contractsOn[3] = {false, false, false};       // AutoDeals AND AutoTradePower / AutoTradeWater / AutoTradeGarbage (informational: it never decides a notice)
};
inline bool UtilityNoticeEnabled(const UtilityManagement& m, int utility) {
    if (utility < 0 || utility >= kNoticeSlots) return false;
    const int u = utility == kStagnantSlot ? 2 : utility;                      // the stagnant-backlog notice follows the garbage management
    return m.controllerRuns && !m.sessionFailed && m.localOn[u] && !m.failedClosed[u];
}

// ---- garbage: the physical model against the game's own backlog -----------------------------------------------------------------------------------------------------------------------------------------------
// There is no counter of the need for garbage: the MODEL says whether the garbage can be processed (facilities, exports on offer, landfill valve), and the game's own backlog (`uncollected`, a stock updated
// every day and never reset by the month) says whether reality agrees. The backlog is read at the start of consecutive months and judged against the garbage the city PRODUCES in a month: the reference of
// "residual" is 1 % of the monthly production (owner, 2026-11-11; the same fraction the service monitor already treats as a material change). Ruralia (log 23:34-23:56): production ~140 000 a month, so
// 1 % ~ 1 400: the permanent residue of 4 is far below it, the spikes of 4 394 and 9 453 are above it but drain the following month, and the growth after the sanitation management was switched off (+6 790,
// +3 264, +6 000, +2 500 ... a month) is far above it.
//   Insufficient  the backlog grew by MORE than that in each of the last two months: the capacity does not keep up with the production (urgent notice, pop-up policy of the table);
//   Stagnant      the backlog is material (above 1 % of the monthly production) and changed by no more than that in each of the last two months, and the model has been evaluated (the city treats practically
//                 all the new garbage, the old pile does not go away): informative notice, discreet, never a pop-up;
//   Recovering    the backlog fell by more than that in the last month: no new notice of insufficiency (nor of stagnation) while it lasts;
//   Recovered     the model is feasible and the backlog is gone or immaterial (not above 1 % of the monthly production);
//   Uncertain     anything else (no comparable months yet, unreadable counters, an operation still being confirmed, a model deficit that a negligible backlog does not confirm).
struct BacklogTrend {
    std::int64_t v[3] = {0, 0, 0};        // the backlog at the start of the last months, the newest last
    std::int64_t produced[3] = {0, 0, 0}; // the garbage produced in the month that ended at each of those starts
    int n = 0;
    void Reset() { n = 0; }
    void Note(std::int64_t backlog, std::int64_t monthProduced) {
        if (n == 3) { v[0] = v[1]; v[1] = v[2]; produced[0] = produced[1]; produced[1] = produced[2]; v[2] = backlog; produced[2] = monthProduced; }
        else { v[n] = backlog; produced[n] = monthProduced; ++n; }
    }
    std::int64_t Reference() const { return n > 0 && produced[n - 1] > 0 ? std::max<std::int64_t>(1, produced[n - 1] / 100) : 0; }      // 1 % of the latest monthly production (0 = unknown)
    std::int64_t Delta(int k) const { return v[k] - v[k - 1]; }                                                                          // change over the month that ended at sample k
};
enum class BacklogState { Uncertain, Insufficient, Stagnant, Recovering, Recovered };
inline const char* BacklogStateName(BacklogState s) {
    return s == BacklogState::Insufficient ? "INSUFFICIENT" : s == BacklogState::Stagnant ? "STAGNANT" : s == BacklogState::Recovering ? "RECOVERING" : s == BacklogState::Recovered ? "RECOVERED" : "UNCERTAIN";
}
struct BacklogFacts {
    bool countersRead = false;              // the garbage counters of today were read
    std::int64_t uncollected = 0;           // the game's backlog today
    bool modelViable = false;               // the last plan says the garbage can be processed
    bool modelProof = false;                // the last plan proved that nothing the Department may do solves it (regional data complete, nothing pending, valid snapshot)
    bool pending = false;                   // an export / sanitation write is still being confirmed
};
inline BacklogState ClassifyBacklog(const BacklogFacts& f, const BacklogTrend& t, std::string& why) {
    if (!f.countersRead) { why = "the garbage counters could not be read"; return BacklogState::Uncertain; }
    if (f.modelViable && f.uncollected == 0) return BacklogState::Recovered;                  // nothing left to collect
    if (f.pending) { why = "an automatic export or sanitation write is still being confirmed"; return BacklogState::Uncertain; }
    const std::int64_t r = t.Reference();
    if (t.n < 2 || r == 0) { why = "no two comparable months of the backlog against the production yet: nothing is concluded"; return BacklogState::Uncertain; }
    const std::int64_t last = t.v[t.n - 1];
    const bool evaluated = f.modelViable || f.modelProof;
    if (last <= r) {                                                                           // the backlog is residual
        if (f.modelViable) return BacklogState::Recovered;
        why = "the model says the garbage cannot be processed but the backlog (" + std::to_string(last) + ") is within 1 % of the monthly production: the model alone does not prove it";
        return BacklogState::Uncertain;
    }
    const std::int64_t d2 = t.Delta(t.n - 1);
    if (d2 < -r) { why = "the backlog is falling (" + std::to_string(d2) + " last month): recovering"; return BacklogState::Recovering; }
    if (t.n >= 3) {
        const std::int64_t d1 = t.Delta(t.n - 2);
        if (evaluated && d1 > r && d2 > r) return BacklogState::Insufficient;                  // grew significantly two months running
        if (evaluated && std::llabs(d1) <= r && std::llabs(d2) <= r) return BacklogState::Stagnant;   // stuck above the residual, neither growing nor draining
    }
    why = "the backlog (" + std::to_string(last) + ") changed by " + std::to_string(d2) + " last month against a reference of " + std::to_string(r) + ": neither insufficiency nor stagnation is proven yet";
    return BacklogState::Uncertain;
}
// What each notice slot reads from the state: the urgent notice (slot 2) and the stagnant one (slot 3) are never proven together, and a change from one to the other withdraws the first (no
// contradictory messages). Recovering and uncertain hold both (no new notice, nothing withdrawn, nothing declared recovered).
inline ShortageEvidence UrgentGarbageEvidence(BacklogState s) { return s == BacklogState::Insufficient ? ShortageEvidence::Proven : (s == BacklogState::Recovered || s == BacklogState::Stagnant) ? ShortageEvidence::Recovered : ShortageEvidence::Uncertain; }
inline ShortageEvidence StagnantGarbageEvidence(BacklogState s) { return s == BacklogState::Stagnant ? ShortageEvidence::Proven : (s == BacklogState::Recovered || s == BacklogState::Insufficient) ? ShortageEvidence::Recovered : ShortageEvidence::Uncertain; }

// ---- the board: one lifecycle per slot ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr std::uint32_t kListingGraceDays = 2;       // a pushed event is listed by the advisor a moment later (its next evaluation): until then "not listed" does not mean "gone"
constexpr int kStaleGiveUpFailures = 4;              // a notice an earlier session left that the advisor cannot be asked about after this many failed attempts is given up (nothing of ours is known to be there)
constexpr std::uint32_t kRetryMaxDays = 30;          // a failed push / retire is tried again after 1, 2, 4, 8, 16 and then every 30 simulated days: bounded, never a call per day for ever

struct NoticeInput {
    bool enabled = false;                      // the player's switch of this service (and the master switch) is on
    ShortageEvidence evidence = ShortageEvidence::Uncertain;
    Listing listing = Listing::Unknown;        // what the advisor says about the notice of this slot (any variant of it); only read while a notice may be outstanding
};

enum class NoticeDo { None, Emit, Retire };
enum class RetireWhy { None, Recovered, Disabled, Stale };    // Stale: a notice an earlier session left listed

struct NoticeStep {
    int slot = 0;
    NoticeDo what = NoticeDo::None;
    bool popup = false;          // Emit: the pop-up variant (emissions 1 and 2); otherwise the discreet one
    int emission = 0;            // Emit: the number of this notice in the episode (1-based)
    bool retireFirst = false;    // Emit: a notice of this slot may still be listed: retire it (confirmed) before the push, so the same episode never shows two
    RetireWhy why = RetireWhy::None;
};

// The state of one slot, for the log and the tests.
struct NoticeState {
    bool episode = false;                  // a proven shortage is being counted or has been announced
    int emissions = 0;                     // notices the advisor accepted in this episode
    int confirmedDismissals = 0;           // dismissals CONFIRMED by a reliable observation (never by an expiry or by a retirement of the plugin)
    bool hasLastEmission = false;
    std::uint32_t lastEmissionDay = 0;
    bool hasLastRetire = false;
    std::uint32_t lastRetireDay = 0;       // the last day a notice was confirmed gone (retired by the plugin, expired or dismissed: not distinguished)
    std::uint32_t clock = 0;               // PROVEN days since the episode began (before the 1st notice) or since the last notice / dismissal
    bool outstanding = false;              // a notice of this slot may still be listed
};

class NoticeBoard {
public:
    // A city session starts (a city was opened, the same one again included): nothing is remembered; a notice an earlier session left listed is withdrawn first (the advisor of the new session may still
    // hold it).
    void StartSession() {
        for (auto& e : slots_) {
            e = Slot{};
            e.outstanding = true;
            e.listedSeen = true;           // "not listed" means gone at once: there is no pending push of this session
            e.stale = true;
        }
    }

    // One call per slot per simulated day (in the order of importance, NoticeSlotsByImportance), AND whenever the options changed. Returns the one thing the plugin must do now for this slot.
    NoticeStep Update(int slot, std::uint32_t day, const NoticeInput& in) {
        NoticeStep step;
        step.slot = slot;
        if (slot < 0 || slot >= kNoticeSlots) return step;
        Slot& e = slots_[static_cast<std::size_t>(slot)];
        const ShortageEvidence now = in.enabled ? in.evidence : ShortageEvidence::Uncertain;

        // the clock counts one day for each day on which the proof held at two consecutive readings; it stands still while uncertain, and a gap between two readings (days nobody looked at) is not credited
        if (e.haveTick && day > e.lastDay && e.prev == ShortageEvidence::Proven && now == ShortageEvidence::Proven) e.clock += 1;
        e.lastDay = day;
        e.haveTick = true;
        e.prev = now;

        // what the advisor says about a notice that may be listed
        if (e.outstanding) {
            if (in.listing == Listing::Listed) e.listedSeen = true;
            else if (in.listing == Listing::NotListed && (e.listedSeen || day - e.pushedDay >= kListingGraceDays)) MarkGone(e, day);
        }

        RetireWhy why = RetireWhy::None;
        if (!in.enabled) { why = RetireWhy::Disabled; ResetEpisode(e); }
        else if (in.evidence == ShortageEvidence::Recovered) { why = RetireWhy::Recovered; ResetEpisode(e); }
        else if (in.evidence == ShortageEvidence::Proven) e.episode = true;
        else if (e.stale && !e.outstanding) e.stale = false;
        // the records of a stale notice are not known to the advisor (another DAT): there is nothing of ours to withdraw, and the slot must not stay blocked for the whole session
        if (e.stale && e.outstanding && in.listing == Listing::Unknown && e.failures >= kStaleGiveUpFailures) MarkGone(e, day);
        if (e.stale && why == RetireWhy::None && e.outstanding) why = RetireWhy::Stale;   // the notice of an earlier session goes first, whatever the evidence is

        e.retireWanted = why != RetireWhy::None && e.outstanding;
        if (why != RetireWhy::None) {
            if (e.outstanding && in.listing != Listing::NotListed && day >= e.nextTry) { step.what = NoticeDo::Retire; step.why = why; }
            return step;
        }
        if (in.evidence != ShortageEvidence::Proven) return step;               // uncertain: no notice, no reminder, the clocks stand still, nothing is concluded

        const NoticeClass cls = ClassOfSlot(slot);
        const std::uint32_t due = e.emissions == 0 ? PersistDays(cls) : ReminderDays(cls, e.dismissals);
        if (e.clock >= due && day >= e.nextTry) {
            step.what = NoticeDo::Emit;
            step.emission = e.emissions + 1;
            step.popup = PopupFor(step.emission) && !DiscreetOnly(slot);          // the stagnant-backlog notice is informative: never a pop-up
            step.retireFirst = e.outstanding;
        }
        return step;
    }

    // The advisor accepted the event (PushEvent returned true: NOT proof that it was displayed) or not. A refused push counts as nothing and is tried again later.
    void ConfirmEmit(int slot, bool accepted, std::uint32_t day) {
        Slot* e = At(slot);
        if (!e) return;
        if (!accepted) { e->nextTry = day + Backoff(++e->failures); return; }
        ++e->emissions;
        e->failures = 0;
        e->nextTry = day;
        e->outstanding = true;
        e->listedSeen = false;
        e->stale = false;
        e->pushedDay = day;
        e->hasLastEmission = true;
        e->lastEmissionDay = day;
        e->clock = 0;
    }
    // The result of a retirement: gone = the advisor no longer lists it. A retirement that could not be confirmed leaves the notice outstanding and is tried again later.
    void ConfirmRetire(int slot, bool gone, std::uint32_t day) {
        Slot* e = At(slot);
        if (!e) return;
        if (gone) { MarkGone(*e, day); e->failures = 0; }
        else e->nextTry = day + Backoff(++e->failures);
    }
    // A dismissal by the player was CONFIRMED by a reliable observation (not an expiry, not a retirement by the plugin): the next interval is the following one of the table, counted from now.
    void NoteConfirmedDismissal(int slot, std::uint32_t day) {
        Slot* e = At(slot);
        if (!e || !e->episode) return;
        ++e->dismissals;
        e->clock = 0;
        MarkGone(*e, day);
    }

    NoticeState State(int slot) const {
        NoticeState s;
        const Slot* e = Peek(slot);
        if (!e) return s;
        s.episode = e->episode;
        s.emissions = e->emissions;
        s.confirmedDismissals = e->dismissals;
        s.hasLastEmission = e->hasLastEmission;
        s.lastEmissionDay = e->lastEmissionDay;
        s.hasLastRetire = e->hasLastRetire;
        s.lastRetireDay = e->lastRetireDay;
        s.clock = e->clock;
        s.outstanding = e->outstanding;
        return s;
    }
    bool outstanding(int slot) const { const Slot* e = Peek(slot); return e && e->outstanding; }
    // Something must be withdrawn and the last look has not completed it: the plugin may look again between two simulated days (a game on pause does not run the daily tick, an event pushed a moment ago may
    // still be pending in the advisor). A failed attempt waits for its retry day, so this never asks for a call per second for ever.
    bool NeedsAttention(std::uint32_t day) const {
        for (const auto& e : slots_) if (e.outstanding && e.retireWanted && day >= e.nextTry) return true;
        return false;
    }

private:
    struct Slot {
        bool episode = false;
        int emissions = 0, dismissals = 0;
        std::uint32_t clock = 0;
        bool hasLastEmission = false, hasLastRetire = false;
        std::uint32_t lastEmissionDay = 0, lastRetireDay = 0;
        bool outstanding = false, listedSeen = false, stale = false, retireWanted = false;
        std::uint32_t pushedDay = 0, nextTry = 0, lastDay = 0;
        int failures = 0;
        bool haveTick = false;
        ShortageEvidence prev = ShortageEvidence::Uncertain;
    };
    static std::uint32_t Backoff(int failures) {
        std::uint32_t d = 1;
        for (int i = 1; i < failures && d < kRetryMaxDays; ++i) d *= 2;
        return d > kRetryMaxDays || failures > 5 ? kRetryMaxDays : d;
    }
    // The episode ends (recovery confirmed, or the player switched the service off): counters and clocks go; a notice that may still be listed stays recorded so that it is withdrawn.
    static void ResetEpisode(Slot& e) {
        e.episode = false;
        e.emissions = 0;
        e.dismissals = 0;
        e.clock = 0;
        e.hasLastEmission = false;
    }
    static void MarkGone(Slot& e, std::uint32_t day) {
        e.outstanding = false;
        e.listedSeen = false;
        e.stale = false;
        e.hasLastRetire = true;
        e.lastRetireDay = day;
    }
    Slot* At(int slot) { return slot >= 0 && slot < kNoticeSlots ? &slots_[static_cast<std::size_t>(slot)] : nullptr; }
    const Slot* Peek(int slot) const { return slot >= 0 && slot < kNoticeSlots ? &slots_[static_cast<std::size_t>(slot)] : nullptr; }

    std::array<Slot, kNoticeSlots> slots_{};
};

}  // namespace sc4af
