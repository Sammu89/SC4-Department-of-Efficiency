// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Calibration of the landfill geometry (number of active cells N, capacity per cell C = T / N) from ordinary observations only: the month
// boundaries the plugin sees in any build. It does not read the game's landfill records from memory.
//
// What a normal build can observe at a month boundary b (all through the existing API snapshot):
//   usedBefore    landfillUsage in the last snapshot before the boundary (the usage does not change inside a month)
//   usedAfter     landfillUsage in the first snapshot after the boundary
//   absorbed      what the landfill took from the valve at that boundary. It is read exactly one month later as the published `Landfilled`
//                 (Landfilled published at b+1 == absorbed at b). It is NOT taken from supplyBefore - supplyAfter: the last daily snapshot is stale by
//                 the intake of the final day, and a funding change inside the month moves A/B by one unit.
//   observedDecay = usedBefore - usedAfter + absorbed          (so freeAfter - freeBefore + absorbed, the same number)
//
// A geometry candidate is an integer N that divides the total capacity T exactly (the cells are equal, T is their sum) and reproduces the observed decay of
// every boundary with the packed first-fit distribution:  D(U) = floor(U / C) x decay(C) + decay(U mod C). There is no tolerance: any boundary a candidate
// does not reproduce exactly eliminates it, and when no candidate survives the model does not fit (MODEL_MISMATCH, sticky until the city is reset).

#include "LandfillRollover.h"

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {

struct LandfillBoundaryObservation {
    std::uint64_t usedBefore = 0;
    std::uint64_t usedAfter = 0;
    std::uint64_t absorbed = 0;
};

struct LandfillGeometryCandidate {
    std::uint32_t activeCells = 0;
    std::uint64_t cellCapacity = 0;
    bool operator==(const LandfillGeometryCandidate& o) const { return activeCells == o.activeCells && cellCapacity == o.cellCapacity; }
};

// Total decay of a packed stock of `usage` units in `cells` cells of `cellCapacity` (the remainder 0 case adds nothing).
inline std::uint64_t PackedLandfillDecay(std::uint64_t usage, std::uint64_t cellCapacity) {
    if (cellCapacity == 0) return 0;
    return (usage / cellCapacity) * LandfillRecordDecay(static_cast<std::uint32_t>(cellCapacity)) + LandfillRecordDecay(static_cast<std::uint32_t>(usage % cellCapacity));
}

// The decay the boundary really applied; false when the numbers cannot belong to one boundary (decay would be negative).
inline bool ObservedLandfillDecay(const LandfillBoundaryObservation& o, std::uint64_t& decay) {
    const std::uint64_t gained = o.usedBefore + o.absorbed;
    if (gained < o.usedAfter) return false;
    decay = gained - o.usedAfter;
    return true;
}

// Every N that divides T and reproduces all the observations exactly. `maxCells` bounds the search (a cell of fewer than one unit is meaningless).
inline std::vector<LandfillGeometryCandidate> InferLandfillGeometry(std::uint64_t totalCapacity, const std::vector<LandfillBoundaryObservation>& observations,
                                                                    std::uint64_t maxCells = 1u << 20) {
    std::vector<LandfillGeometryCandidate> out;
    if (totalCapacity == 0) return out;
    std::vector<std::uint64_t> decays;
    for (const auto& o : observations) {
        std::uint64_t d = 0;
        if (o.usedBefore > totalCapacity || o.usedAfter > totalCapacity || !ObservedLandfillDecay(o, d)) return out;     // impossible numbers: no geometry fits
        decays.push_back(d);
    }
    const auto test = [&](std::uint64_t n) {
        if (n == 0 || n > maxCells || totalCapacity % n != 0) return;
        const std::uint64_t cell = totalCapacity / n;
        for (std::size_t i = 0; i < observations.size(); ++i)
            if (PackedLandfillDecay(observations[i].usedBefore, cell) != decays[i]) return;
        out.push_back(LandfillGeometryCandidate{static_cast<std::uint32_t>(n), cell});
    };
    for (std::uint64_t d = 1; d * d <= totalCapacity; ++d)
        if (totalCapacity % d == 0) { test(d); if (d * d != totalCapacity) test(totalCapacity / d); }
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.activeCells < b.activeCells; });
    return out;
}

enum class LandfillGeometryState { Unknown, Ambiguous, Calibrated, ModelMismatch };

// What the planner is told about the landfill geometry (never more than the calibrator proved).
struct LandfillGeometryInfo {
    LandfillGeometryState state = LandfillGeometryState::Unknown;
    std::uint32_t activeCells = 0;
    std::uint64_t cellCapacity = 0;
    std::size_t observations = 0;
    std::size_t candidates = 0;
    bool calibrated() const { return state == LandfillGeometryState::Calibrated && activeCells > 0 && cellCapacity > 0; }
};

inline const char* LandfillGeometryStateName(LandfillGeometryState s) {
    switch (s) {
    case LandfillGeometryState::Unknown: return "UNKNOWN";
    case LandfillGeometryState::Ambiguous: return "AMBIGUOUS";
    case LandfillGeometryState::Calibrated: return "CALIBRATED";
    case LandfillGeometryState::ModelMismatch: return "MODEL_MISMATCH";
    }
    return "?";
}

// Per-city calibration (reset with the city lifecycle; never carried to another city). The dual horizon may use the geometry only in the Calibrated
// state; every other state keeps the continuity lock.
class LandfillCalibrator {
public:
    void Reset() { *this = LandfillCalibrator{}; }
    LandfillGeometryState state() const { return state_; }
    bool DualHorizonAllowed() const { return state_ == LandfillGeometryState::Calibrated; }
    bool ContinuityLock() const { return !DualHorizonAllowed(); }
    const std::vector<LandfillGeometryCandidate>& candidates() const { return candidates_; }
    std::size_t observations() const { return observations_.size(); }
    // Valid only when Calibrated.
    LandfillGeometryCandidate geometry() const { return candidates_.size() == 1 ? candidates_.front() : LandfillGeometryCandidate{}; }

    LandfillGeometryInfo info() const {
        LandfillGeometryInfo i;
        i.state = state_;
        i.observations = observations_.size();
        i.candidates = candidates_.size();
        if (state_ == LandfillGeometryState::Calibrated) { i.activeCells = candidates_.front().activeCells; i.cellCapacity = candidates_.front().cellCapacity; }
        return i;
    }
    // The capacity read now differs from the one the evidence was gathered with (a landfill was built or demolished): the geometry is unknown again.
    // Returns true when evidence was dropped.
    bool NoteCapacity(std::uint64_t totalCapacity) {
        if (capacity_ == 0 || capacity_ == totalCapacity) return false;
        Reset();
        return true;
    }

    // One completed boundary. A change of the total capacity (a landfill built or demolished) means the geometry changed: the evidence restarts from this boundary.
    void Observe(std::uint64_t totalCapacity, const LandfillBoundaryObservation& o) {
        if (state_ == LandfillGeometryState::ModelMismatch && capacity_ == totalCapacity) return;       // sticky
        if (capacity_ != totalCapacity) { Reset(); capacity_ = totalCapacity; }
        observations_.push_back(o);
        candidates_ = InferLandfillGeometry(capacity_, observations_);
        state_ = candidates_.empty() ? LandfillGeometryState::ModelMismatch : candidates_.size() == 1 ? LandfillGeometryState::Calibrated : LandfillGeometryState::Ambiguous;
    }

    // [Garbage][LandfillCalibration] line. The diagnostic fields (supply, free) are logged but never used by the inference.
    std::string LogLine(std::uint32_t boundary, std::uint64_t totalCapacity, const LandfillBoundaryObservation& o, std::int64_t supplyBefore, std::int64_t supplyAfter) const {
        std::uint64_t decay = 0;
        const bool ok = ObservedLandfillDecay(o, decay);
        std::ostringstream s;
        s << "[Garbage][LandfillCalibration] boundary=" << boundary << " capacity=" << totalCapacity << " usedBefore=" << o.usedBefore << " usedAfter=" << o.usedAfter << " freeBefore="
          << (totalCapacity - std::min(totalCapacity, o.usedBefore)) << " freeAfter=" << (totalCapacity - std::min(totalCapacity, o.usedAfter)) << " supplyBefore=" << supplyBefore << " supplyAfter="
          << supplyAfter << " absorbed=" << o.absorbed << " observedDecay=" << (ok ? std::to_string(decay) : std::string("IMPOSSIBLE")) << " candidateCount=" << candidates_.size() << ' ';
        switch (state_) {
        case LandfillGeometryState::Calibrated:
            s << "CALIBRATED activeCells=" << candidates_.front().activeCells << " capacityPerCell=" << candidates_.front().cellCapacity << " observations=" << observations_.size() << " dualHorizon=ALLOWED";
            break;
        case LandfillGeometryState::Ambiguous: {
            s << "AMBIGUOUS candidates=";
            for (std::size_t i = 0; i < candidates_.size() && i < 8; ++i) s << (i ? "," : "") << candidates_[i].activeCells << 'x' << candidates_[i].cellCapacity;
            s << (candidates_.size() > 8 ? ",..." : "") << " continuityLock=ON";
            break;
        }
        case LandfillGeometryState::ModelMismatch: s << "MODEL_MISMATCH continuityLock=ON"; break;
        case LandfillGeometryState::Unknown: s << "UNKNOWN continuityLock=ON"; break;
        }
        return s.str();
    }

private:
    std::uint64_t capacity_ = 0;
    std::vector<LandfillBoundaryObservation> observations_;
    std::vector<LandfillGeometryCandidate> candidates_;
    LandfillGeometryState state_ = LandfillGeometryState::Unknown;
};

} // namespace sc4af
