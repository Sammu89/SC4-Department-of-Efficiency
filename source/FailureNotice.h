// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Failure notices (2026-11-09): when the Department can no longer manage, TECHNICALLY and for good, a service the player switched on, the player gets ONE short humorous news item for that service in
// the bottom news bar (BAD_JOB, priority MEDIUM: no pop-up, no advisor opened, no window). The title is the line of the bar, the detail (the message) opens when the player clicks it. The texts say only
// that the automatic management is unavailable in this session; the cause stays in the log. This file is the pure decision (no game types, unit tested); the calls into the game are in Plugin.cpp.
//
// A service FAILED when it is enabled by the player AND the product itself shut that management down after a technical failure (a funding writer that failed its verification and went fail-closed, the
// deal manager disabled, the civic writer that distrusted itself, demolitions stopped by a technical block). Those states last until the city is loaded again (or the manager is explicitly restarted).
// Recoverable states are NEVER a failure: NOT_READY readings, a snapshot that cannot be read this month, a city still loading, STANDBY, a shortage the Department is managing, a refused write that is
// normal and recoverable, a service the player switched off, Mode Off / Shadow.
//
// POLICY (owner, 2026-11-09): ONE notice per service and per CITY SESSION. The memory is a single flag per service that is cleared only when a city session starts (a city is loaded, the same one included):
// the notice is not repeated however many times the service fails, whether the player dismisses it, or when it disappears from the bar. Withdrawing a listed notice (the service was switched off, or its
// management was restarted so the failure is over) never clears the flag, and an Off->Auto cycle, an INI change or a recovery never authorizes a second notice. No recovery notice exists. Nothing here ever
// touches a controller: it only reads booleans the controllers already keep.

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace sc4af {

enum class FailureService { Power = 0, Water, Sanitation, Health, Education, Deals, Demolition };
constexpr int kFailureServices = 7;
constexpr int kFailureRetryDays = 30;          // a notice the advisor did not accept (or a withdrawal that could not be confirmed) is tried again after this many simulated days

inline const char* FailureServiceName(int s) {
    static const char* names[kFailureServices] = {"POWER", "WATER", "SANITATION", "HEALTH", "EDUCATION", "NEIGHBOR_DEALS", "DEMOLITION"};
    return s >= 0 && s < kFailureServices ? names[s] : "?";
}

// What the plugin reports about one service on a given day.
struct FailureInput {
    bool enabled = false;       // the player switched the automatic management of this service on (and its master switch)
    bool confirmed = false;     // the product shut that management down after a technical failure
    std::string cause;          // the technical cause, for the log only
};

enum class FailureDo { None, Announce, Retire };

struct FailureStep {
    int service = 0;
    FailureDo what = FailureDo::None;
    bool recovered = false;     // Retire: the failure is over (otherwise the player switched the service off)
    std::string cause;
};

class FailureNoticeBoard {
public:
    // A new city session starts (a city was loaded, the same one included): the controllers start from scratch and the advisor of the new session holds nothing. The ONLY place the memory is cleared.
    void StartSession() { slots_ = Slots{}; }

    // Called once per simulated day with the state of the 7 services. Returns what the plugin must do now, and the transitions to log.
    std::vector<FailureStep> Update(std::int64_t day, const std::array<FailureInput, kFailureServices>& in, std::vector<std::string>* transitions = nullptr) {
        std::vector<FailureStep> out;
        for (int s = 0; s < kFailureServices; ++s) {
            Slot& e = slots_[static_cast<std::size_t>(s)];
            const FailureInput& x = in[static_cast<std::size_t>(s)];
            const bool failed = x.enabled && x.confirmed;
            if (failed != e.failing) {
                if (transitions)
                    transitions->push_back(std::string("service=") + FailureServiceName(s) + " " + (e.failing ? "FAILED" : "OK") + " -> " + (failed ? "FAILED" : "OK") + " day=" + std::to_string(day) +
                                           (failed ? " cause: " + x.cause : x.enabled ? " (the management was restarted)" : " (the player switched it off)"));
                e.failing = failed;
            }
            if (failed) {
                if (!e.told && day >= e.nextTry) { FailureStep f; f.service = s; f.what = FailureDo::Announce; f.cause = x.cause; out.push_back(f); }
            } else if (e.listed && day >= e.nextTry) {
                FailureStep f; f.service = s; f.what = FailureDo::Retire; f.recovered = x.enabled; out.push_back(f);
            }
        }
        return out;
    }

    // The result of handing the notice to the advisor. Accepted = PushEvent returned true (NOT proof that it was displayed): it is then told for good in this city session.
    void ConfirmAnnounce(int s, bool accepted, std::int64_t day) {
        Slot& e = Entry(s);
        if (accepted) { e.told = true; e.listed = true; } else e.nextTry = day + kFailureRetryDays;
    }
    // The result of withdrawing it. gone = the advisor no longer lists it. NEVER clears `told`: a withdrawn notice is not allowed to be told again.
    void ConfirmRetire(int s, bool gone, std::int64_t day) {
        Slot& e = Entry(s);
        if (gone) e.listed = false; else e.nextTry = day + kFailureRetryDays;
    }

    bool failing(int s) const { return Peek(s).failing; }
    bool told(int s) const { return Peek(s).told; }
    bool listed(int s) const { return Peek(s).listed; }

private:
    struct Slot {
        bool failing = false;          // the service is in a confirmed failure now
        bool told = false;             // the notice was accepted by the advisor in this city session (never cleared before the next session)
        bool listed = false;           // it may still be listed (accepted, not yet withdrawn)
        std::int64_t nextTry = 0;
    };
    using Slots = std::array<Slot, kFailureServices>;
    Slot& Entry(int s) { return slots_[static_cast<std::size_t>(s)]; }
    const Slot& Peek(int s) const {
        static const Slot none;
        return s < 0 || s >= kFailureServices ? none : slots_[static_cast<std::size_t>(s)];
    }

    Slots slots_{};
};

} // namespace sc4af
