// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Per-city collector of the landfill month boundaries that feeds the LandfillCalibrator from NORMAL snapshot fields only.
//
// The absorption of boundary k is published as `Landfilled` at boundary k+1, so every boundary waits one month in a one-slot state machine:
//   month event j (after the boundary j): reading = {capacity T, usage U_j, published P_j}
//     P_j  is the absorption of boundary j-1   -> closes the pending observation {T, U_{j-2}.., U_{j-1}} with absorbed = P_j
//     the usage of the previous reading is the usage BEFORE boundary j (the usage does not change inside a month) -> opens {T, U_{j-1}, U_j}
// A capacity change, an unreadable field or a city reset drops the pending boundary (and, for a capacity change, the evidence). The published value is never associated
// with any other boundary. No memory read of the game, no research data.

#include "LandfillGeometry.h"

#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {

struct LandfillReading {
    std::optional<std::uint64_t> capacity, usage;
    std::optional<std::int64_t> published;      // the API's last-month Landfilled
};

class LandfillGeometryTracker {
public:
    void Reset() {
        cal_.Reset();
        last_.reset();
        pending_.reset();
        boundaries_ = 0;
        usable_ = 0;
        logs_.clear();
        wasCalibrated_ = false;
        anchored_ = false;
        lastUnanchored_ = false;
    }

    // A reading that is not a month event (city init, a day): only the usage baseline moves. A readable baseline ANCHORS the city: the first month event
    // after it can then be paired with it. Without one (the init snapshot can report landfillCapacity=0 and usage=0) the first month event's usage cannot be
    // shown to follow its own boundary: the usage read at that event can be the state BEFORE its boundary (the next observation then spans two chained
    // boundaries), so that reading is kept only as a position for the next event and never as a `usedBefore`.
    void OnBaseline(const LandfillReading& r) {
        if (!r.capacity || !r.usage || *r.capacity == 0) { last_.reset(); return; }
        NoteCapacity(*r.capacity);
        last_ = Last{*r.capacity, *r.usage};
        anchored_ = true;
        lastUnanchored_ = false;
    }

    // The month event, taken AFTER the boundary (the plugin handler runs last).
    void OnMonthEvent(const LandfillReading& r) {
        ++boundaries_;
        if (!r.capacity || !r.usage || *r.capacity == 0) {
            pending_.reset();
            last_.reset();
            logs_.push_back(StateLine("reading unavailable: no observation this month"));
            return;
        }
        const std::uint64_t T = *r.capacity;
        NoteCapacity(T);
        if (pending_ && pending_->capacity == T && r.published && *r.published >= 0) {
            const LandfillBoundaryObservation obs{pending_->usedBefore, pending_->usedAfter, static_cast<std::uint64_t>(*r.published)};
            const std::size_t before = cal_.state() == LandfillGeometryState::Unknown ? 0 : cal_.candidates().size();
            cal_.Observe(T, obs);
            ++usable_;
            std::uint64_t decay = 0;
            const bool ok = ObservedLandfillDecay(obs, decay);
            std::ostringstream o;
            o << "[Garbage][LandfillCalibration] boundary=" << pending_->boundary << " usedBefore=" << obs.usedBefore << " usedAfter=" << obs.usedAfter << " publishedAbsorbed=" << obs.absorbed
              << " observedDecay=" << (ok ? std::to_string(decay) : std::string("IMPOSSIBLE")) << " candidateCountBefore=" << (before ? std::to_string(before) : std::string("all")) << " candidateCountAfter="
              << cal_.candidates().size();
            logs_.push_back(o.str());
        } else if (pending_) {
            logs_.push_back(std::string("[Garbage][LandfillCalibration] boundary=") + std::to_string(pending_->boundary) + " DROPPED " +
                            (pending_->capacity != T ? "reason=CAPACITY_CHANGED" : "reason=PUBLISHED_LANDFILLED_UNAVAILABLE"));
        }
        if (last_ && last_->capacity == T && lastUnanchored_) {
            pending_.reset();
            logs_.push_back(std::string("[Garbage][LandfillCalibration] boundary=") + std::to_string(boundaries_) +
                            " DROPPED reason=UNANCHORED_FIRST_READING (no readable usage baseline at city init: the first month event's usage is not proven to follow its own boundary; "
                            "usedBefore=" + std::to_string(last_->usage) + " usedAfter=" + std::to_string(*r.usage) + ")");
        } else if (last_ && last_->capacity == T) pending_ = Pending{T, last_->usage, *r.usage, boundaries_};
        else pending_.reset();
        lastUnanchored_ = !anchored_ && boundaries_ == 1;     // only the first month event of a city that has no readable init baseline
        last_ = Last{T, *r.usage};
        logs_.push_back(StateLine(""));
        const bool nowCal = cal_.state() == LandfillGeometryState::Calibrated;
        if (wasCalibrated_ && !nowCal) logs_.push_back(std::string("[Garbage][DualHorizon] DUAL_HORIZON_DISABLED reason=") + LandfillGeometryStateName(cal_.state()) + " continuityLock=ON");
        if (!wasCalibrated_ && nowCal)
            logs_.push_back("[Garbage][LandfillCalibration] CALIBRATED activeCells=" + std::to_string(cal_.geometry().activeCells) + " cellCapacity=" + std::to_string(cal_.geometry().cellCapacity) +
                            " observations=" + std::to_string(cal_.observations()));
        if (cal_.state() == LandfillGeometryState::ModelMismatch && cal_.observations() > 0 && !mismatchLogged_) {
            mismatchLogged_ = true;
            logs_.push_back("[Garbage][LandfillCalibration] MODEL_MISMATCH reason=no (activeCells, cellCapacity) with T = N x C reproduces every observed boundary; continuityLock=ON");
        }
        wasCalibrated_ = nowCal;
    }

    LandfillGeometryInfo View() const { return cal_.info(); }
    const LandfillCalibrator& calibrator() const { return cal_; }
    std::size_t usableBoundaries() const { return usable_; }
    std::vector<std::string> TakeLogs() { auto out = std::move(logs_); logs_.clear(); return out; }

private:
    struct Last { std::uint64_t capacity = 0, usage = 0; };
    struct Pending { std::uint64_t capacity = 0, usedBefore = 0, usedAfter = 0; std::size_t boundary = 0; };

    void NoteCapacity(std::uint64_t T) {
        if (cal_.NoteCapacity(T)) {
            pending_.reset();
            mismatchLogged_ = false;
            logs_.push_back(std::string("[Garbage][DualHorizon] DUAL_HORIZON_DISABLED reason=CAPACITY_CHANGED evidenceReset=YES continuityLock=ON"));
            wasCalibrated_ = false;
        }
        if (last_ && last_->capacity != T) last_.reset();
    }
    std::string StateLine(const std::string& note) const {
        const auto i = cal_.info();
        std::ostringstream o;
        o << "[Garbage][LandfillCalibration] state=" << LandfillGeometryStateName(i.state) << " observationCount=" << i.observations << " usableBoundaries=" << usable_ << " candidateCount=" << i.candidates
          << " totalCapacity=" << (last_ ? std::to_string(last_->capacity) : std::string("?")) << " activeCells=" << (i.calibrated() ? std::to_string(i.activeCells) : std::string("?")) << " cellCapacity="
          << (i.calibrated() ? std::to_string(i.cellCapacity) : std::string("?")) << (note.empty() ? "" : " note=" + note);
        return o.str();
    }

    LandfillCalibrator cal_;
    std::optional<Last> last_;
    std::optional<Pending> pending_;
    std::size_t boundaries_ = 0, usable_ = 0;
    std::vector<std::string> logs_;
    bool wasCalibrated_ = false, mismatchLogged_ = false;
    bool anchored_ = false, lastUnanchored_ = false;
};

} // namespace sc4af
