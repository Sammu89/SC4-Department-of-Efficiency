// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// Pacing of the Sanitation funding writes, so the garbage department's funding does not thrash.
//
// The game's efficiency (+0x228) and the landfill valve follow a funding change with a delay, so a plan made right after a write is made on a
// state that is still moving; reacting to every write with an immediate replan makes the funding oscillate and multiplies the controller's
// cycles per game month. The pacing only has to give the efficiency time to settle (the writer confirms it in 1-3 simulated days).
// The plan itself is unchanged; only its EXECUTION is paced:
//   * a funding CUT moves at most kSanitationMaxCutStep points per write and waits kSanitationMinDaysBetweenCuts simulated days after the previous write;
//   * a funding RAISE (service first) waits only kSanitationMinDaysBetweenRaises days after the previous write;
//   * the target is still the optimizer's; it is reached in steps.
#pragma once

#include <cstdint>
#include <string>

namespace sc4af {

constexpr int kSanitationMaxCutStep = 25;               // points
constexpr std::uint32_t kSanitationMinDaysBetweenCuts = 7;
constexpr std::uint32_t kSanitationMinDaysBetweenRaises = 3;

struct PacedSanitation {
    bool allowed = false;
    int target = 0;                 // the target to write now (equal to the plan's unless the cut was limited)
    std::string note;               // why it was limited or deferred
};

// `daysSinceLastWrite` is only read when `haveLastWrite`.
inline PacedSanitation PaceSanitationWrite(int current, int target, bool haveLastWrite, std::uint32_t daysSinceLastWrite) {
    PacedSanitation p;
    p.target = target;
    if (target == current) { p.note = "no change"; return p; }
    const bool cut = target < current;
    const std::uint32_t wait = cut ? kSanitationMinDaysBetweenCuts : kSanitationMinDaysBetweenRaises;
    if (haveLastWrite && daysSinceLastWrite < wait) {
        p.target = current;
        p.note = std::string(cut ? "cut" : "raise") + " deferred: " + std::to_string(wait - daysSinceLastWrite) + " more simulated day(s) after the previous write";
        return p;
    }
    p.allowed = true;
    if (cut && current - target > kSanitationMaxCutStep) {
        p.target = current - kSanitationMaxCutStep;
        p.note = "cut limited to " + std::to_string(kSanitationMaxCutStep) + " points (plan target " + std::to_string(target) + "%)";
    }
    return p;
}

} // namespace sc4af
