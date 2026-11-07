// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Episodes of the one intrusive service warning ("no way to supply this utility"), kept apart from the economic optimiser state.
//
// Principle: the player never sees the automatic operations (funding, purchases, cancelled sales, renegotiations). The only intrusive service notice is a PROVEN impossibility:
//  * the regional data the proof relies on is complete (markets and proposals read, every book of the commodity complete);
//  * the solver exhausted the best admissible combination (maximum usable local capacity, every regional contract on offer, sales cancelled) and the city still cannot be served;
//  * Power / Water: the game's own counters confirm the deficit (need above service delivered, supply short); an internal connection problem is not a regional deficit;
//  * Garbage: the validated physical model says so (it is a model, never presented as a game measurement);
//  * no transaction of that commodity is still pending.
// One warning per utility per continuous episode. The episode ends only on PROVEN recovery (Power / Water: a fresh authoritative sample with the service delivered; Garbage: the model is
// feasible again and the game's own backlog is zero), never on a portfolio that is merely theoretically feasible. There is no timer: a new episode after a proven recovery warns at once.

#include "UtilityModel.h"

namespace sc4af {

class NoticeEpisodes {
public:
    struct Result {
        bool emit = false;      // warn now (first proof of this episode)
        bool rearmed = false;   // the episode ended (proven recovery); a later proof is a NEW episode
    };

    // slots: 0 power, 1 water, 2 garbage, 3 + CivicType for the Education / Health facility types
    static constexpr int kSlots = 12;
    void Reset() { for (bool& a : active_) a = false; }
    bool active(int utility) const { return utility >= 0 && utility < kSlots && active_[utility]; }

    Result Update(int utility, bool proven, bool recovered) {
        Result r;
        bool& a = active_[utility];
        if (a) {
            if (recovered) { a = false; r.rearmed = true; }
            return r;                       // inside the episode: never again
        }
        if (proven && !recovered) { a = true; r.emit = true; }
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
    bool active_[kSlots] = {};
};

// Persistence filter in front of the player notice. The episode machinery above decides WHEN a shortage is proven; the player is only told if it is still unresolved a month later (an unresolved
// episode, i.e. no proven recovery in between). A shortage the Department resolves inside that month is never announced.
// The notice is an URGENT pop-up and it is REPEATED while the shortage stays unresolved: electricity and water every two simulated months, garbage every four, counted from
// the previous notice. The first one still waits the 30 days; a proven recovery ends the episode (no more reminders), a later shortage is a new episode that starts again from the 30 days.
constexpr std::uint32_t kNoticePersistDays = 30;
constexpr std::uint32_t kNoticeRepeatDaysPowerWater = 60, kNoticeRepeatDaysGarbage = 120;
// Days between two reminders of the same unresolved shortage; 0 = never repeated (the Education / Health facility notices have their own board).
constexpr std::uint32_t NoticeRepeatDays(int id) { return id == 0 || id == 1 ? kNoticeRepeatDaysPowerWater : id == 2 ? kNoticeRepeatDaysGarbage : 0; }

class DeferredNotices {
public:
    static constexpr int kSlots = 12;        // 0 power, 1 water, 2 garbage, 3 + CivicType
    void Reset() { for (int i = 0; i < kSlots; ++i) { armed_[i] = false; shown_[i] = false; day_[i] = 0; last_[i] = 0; } }
    // The episode of `id` has just been opened (proof on `day`).
    void Armed(int id, std::uint32_t day) {
        if (id < 0 || id >= kSlots || armed_[id] || shown_[id]) return;
        armed_[id] = true;
        day_[id] = day;
    }
    // Once per simulated day for every id. `stillActive`: the episode has not been closed by a proven recovery. true = tell the player now (exactly once per episode).
    bool Due(int id, bool stillActive, std::uint32_t day) {
        if (id < 0 || id >= kSlots) return false;
        if (armed_[id]) {
            if (!stillActive) { armed_[id] = false; return false; }                     // resolved before the delay: the player never hears of it
            if (day - day_[id] >= kNoticePersistDays) { armed_[id] = false; shown_[id] = true; last_[id] = day; return true; }
            return false;
        }
        if (shown_[id] && !stillActive) shown_[id] = false;                             // the episode ended: a later one is new
        return false;
    }
    // A reminder of the SAME unresolved episode: true once per interval (NoticeRepeatDays) after the previous notice, while the episode has not been closed by a proven recovery. Call after Due() on
    // the same day (Due() is what ends an episode and re-arms a later one).
    bool RepeatDue(int id, bool stillActive, std::uint32_t day) {
        if (id < 0 || id >= kSlots || !shown_[id] || !stillActive) return false;
        const std::uint32_t every = NoticeRepeatDays(id);
        if (every == 0 || day - last_[id] < every) return false;
        last_[id] = day;
        return true;
    }
    // The episode was already told in an earlier session (the memory outside the saves): nothing is shown now, and the next reminder comes one interval from today.
    void NoteToldEarlier(int id, std::uint32_t day) { if (id >= 0 && id < kSlots) last_[id] = day; }
    bool pending(int id) const { return id >= 0 && id < kSlots && armed_[id]; }
    bool shown(int id) const { return id >= 0 && id < kSlots && shown_[id]; }

private:
    bool armed_[kSlots] = {};
    bool shown_[kSlots] = {};
    std::uint32_t day_[kSlots] = {};
    std::uint32_t last_[kSlots] = {};    // the day of the latest notice (or of the silent one of an earlier session) of this episode
};

}  // namespace sc4af
