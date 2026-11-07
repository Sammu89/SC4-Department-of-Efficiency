// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "CivicFundingPlan.h"
#include "CivicPolicy.h"
#include "SnapshotValidity.h"

#include <cstdint>
#include <iterator>
#include <algorithm>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace sc4af {

// Facts about one civic enumeration attempt (pure data, produced by CivicFacilityIO).
struct EnumerationInput {
    bool cityPresent = false;
    bool residentialPresent = false;
    bool epochMatches = true;   // the city is still the one the session was built for
    ListAcquisition schools;
    ListAcquisition hospitals;
    std::size_t readOk = 0;     // facilities successfully read
    std::size_t readFailed = 0; // facilities returned by a list but not readable
    std::size_t notOfFamily = 0; // occupants a list returned that are not buildings of its family (BelongsToList): set aside, never counted
};

// Validity is decided from the read operation alone. History (previous counts) is never consulted.
inline SnapshotValidity ClassifyEnumeration(const EnumerationInput& in) {
    if (!in.cityPresent || !in.residentialPresent || !in.epochMatches) return SnapshotValidity::Invalid;
    if (!in.schools.available || !in.hospitals.available) return SnapshotValidity::Invalid;
    const std::size_t raw = in.schools.count + in.hospitals.count;
    if (raw == 0 && in.readOk == 0) return SnapshotValidity::ValidEmpty;
    // Lists reported buildings but none could be read: the reader is broken, not the city.
    if (in.readOk == 0) return SnapshotValidity::Invalid;
    if (in.readFailed > 0) return SnapshotValidity::Partial;
    return SnapshotValidity::Valid;
}

// Writes are only ever considered from a snapshot that observed something. Each write is still
// re-read (compare-and-set) individually, so a Partial snapshot cannot cause a blind write.
inline bool WritesAllowed(SnapshotValidity v) {
    return v == SnapshotValidity::Valid || v == SnapshotValidity::Partial;
}

using FacilityKey = std::tuple<int, int, int>;  // (family, cellX, cellZ)

struct ObservedFunding {
    FacilityKey key;
    float funding = 0.0f;
};

struct DriftReport {
    FacilityKey key;
    float expected = 0.0f;
    float found = 0.0f;
};

struct SessionEvent {
    SnapshotValidity validity = SnapshotValidity::Invalid;
    bool logTransition = false;  // a line worth writing to the log (state change or rate-limited repeat)
    LogSeverityHint severity = LogSeverityHint::None;
    std::string message;
    std::vector<DriftReport> drifts;
};

// Per-city civic session state: what was last written (drift watch) and the empty/invalid transitions.
// Everything here belongs to ONE city lifecycle; ResetCity() drops it all (unload/reload).
class CivicSession {
public:
    static constexpr int kInvalidRepeatEvery = 12;  // rate limit for repeated INVALID diagnostics

    void ResetCity() {
        ++epoch_;
        lastWritten_.clear();
        notWritable_.clear();
        capacity_.clear();
        usage_.clear();
        last_ = SnapshotValidity::ValidEmpty;
        haveLast_ = false;
        invalidStreak_ = 0;
    }

    std::uint32_t epoch() const { return epoch_; }
    std::size_t expectationCount() const { return lastWritten_.size(); }

    void RecordWrite(const FacilityKey& key, float value) { lastWritten_[key] = value; }

    // Facilities whose funding looked adjustable but ignored the setter (see SetterHadNoEffect).
    // Tracked individually per city lifecycle, never for a whole family. The identity is the cell
    // plus what the building is (subtype, base cost, base capacity): a different building later
    // standing on that cell does not inherit the exclusion.
    struct FacilityIdentity {
        std::string subtype;
        std::int64_t baseCost = 0, baseCapacity = 0;
        bool operator==(const FacilityIdentity& o) const { return subtype == o.subtype && baseCost == o.baseCost && baseCapacity == o.baseCapacity; }
    };
    void QuarantineNotWritable(const FacilityKey& key, const FacilityIdentity& id) { notWritable_[key] = id; }
    bool IsNotWritable(const FacilityKey& key, const FacilityIdentity& id) const {
        const auto it = notWritable_.find(key);
        return it != notWritable_.end() && it->second == id;
    }
    std::size_t notWritableCount() const { return notWritable_.size(); }

    // What this city lifecycle has learned about the capacity factor of one facility (see CapacityEvidence).
    // Bound to the building identity; forgotten with the city.
    std::optional<CapacityPrior> CapacityPriorFor(const FacilityKey& key, const FacilityIdentity& id) const {
        const auto it = capacity_.find(key);
        if (it == capacity_.end() || !(it->second.first == id)) return std::nullopt;
        return it->second.second;
    }
    void RememberCapacity(const FacilityKey& key, const FacilityIdentity& id, const std::optional<CapacityPrior>& prior) {
        if (prior) capacity_[key] = {id, *prior}; else capacity_.erase(key);
    }
    std::size_t capacityMemoryCount() const { return capacity_.size(); }

    // The monthly usage of one facility in this city lifecycle (at most kUsageHistory samples). The
    // coverage the game rolls strikes on is capacity / usage, so the reserve has to cover the largest
    // month-to-month RISE this building has shown (seasonal intakes of +17% occur), not only the fixed 5%.
    static constexpr std::size_t kUsageHistory = 36;
    static constexpr std::int64_t kVolatilityMinUsage = 50;       // below this the relative swings are noise
    static constexpr double kVolatilityCap = 0.5;
    // The yearly graduation wave can raise a small high school's usage by +46% in one month (coverage
    // 0.72) while the elementary schools feeding it fall ~8%. A session that has not yet lived a full
    // year cannot have seen that wave, so the tiers that receive intakes assume it until 13 monthly
    // samples exist.
    static constexpr std::size_t kIntakeWarmupSamples = 13;
    static constexpr double kIntakeAssumedRise = 0.5;
    static double WithIntakeAssumption(const std::string& subtype, std::size_t samples, double measured) {
        const bool intakeTier = subtype == "high_school" || subtype == "college_or_university";
        return (intakeTier && samples < kIntakeWarmupSamples) ? std::max(measured, kIntakeAssumedRise) : measured;
    }
    std::size_t UsageSamples(const FacilityKey& key) const { const auto it = usage_.find(key); return it == usage_.end() ? 0 : it->second.size(); }
    void ObserveUsage(const FacilityKey& key, std::int64_t usage) {
        auto& h = usage_[key];
        if (!h.empty() && h.back().second == usage && h.back().first == epochMonth_) return;
        h.push_back({epochMonth_, usage});
        if (h.size() > kUsageHistory) h.erase(h.begin());
    }
    void NextMonth() { ++epochMonth_; }
    double UpwardVolatility(const FacilityKey& key) const {
        const auto it = usage_.find(key);
        if (it == usage_.end()) return 0.0;
        double worst = 0.0;
        const auto& h = it->second;
        for (std::size_t i = 1; i < h.size(); ++i)
            if (h[i - 1].second >= kVolatilityMinUsage && h[i].first == h[i - 1].first + 1)
                worst = std::max(worst, static_cast<double>(h[i].second) / static_cast<double>(h[i - 1].second) - 1.0);
        return std::min(worst, kVolatilityCap);
    }

    // Feed one enumeration. Returns transitions worth logging and drift reports; never throws.
    SessionEvent Observe(SnapshotValidity validity, const std::vector<ObservedFunding>& present) {
        SessionEvent ev;
        ev.validity = validity;

        if (validity == SnapshotValidity::Invalid) {
            ++invalidStreak_;
            // Do NOT touch expectations: an invalid snapshot proves nothing about buildings.
            if (invalidStreak_ == 1 || invalidStreak_ % kInvalidRepeatEvery == 0) {
                ev.logTransition = true;
                ev.severity = LogSeverityHint::Warning;
                ev.message = "civic snapshot INVALID (no writes this month; retrying normally); consecutive=" +
                             std::to_string(invalidStreak_);
            }
            last_ = validity;
            haveLast_ = true;
            return ev;
        }

        if (haveLast_ && last_ == SnapshotValidity::Invalid) {
            ev.logTransition = true;
            ev.severity = LogSeverityHint::Info;
            ev.message = "civic snapshot recovered after " + std::to_string(invalidStreak_) + " invalid read(s)";
        }
        invalidStreak_ = 0;

        // Drift watch: a facility we wrote earlier must still hold what we wrote.
        for (const auto& p : present) {
            const auto it = lastWritten_.find(p.key);
            if (it == lastWritten_.end()) continue;
            if (!NearlyEqual(p.funding, it->second)) {
                ev.drifts.push_back({p.key, it->second, p.funding});
                lastWritten_.erase(it);
            }
        }
        // A complete observation (Valid / ValidEmpty) defines what exists: expectations about
        // buildings that are gone must not survive (a rebuilt school at the same cell is a NEW building).
        if (validity != SnapshotValidity::Partial) {
            for (auto it = lastWritten_.begin(); it != lastWritten_.end();) {
                bool found = false;
                for (const auto& p : present) found = found || p.key == it->first;
                it = found ? std::next(it) : lastWritten_.erase(it);
            }
        }

        const bool wasEmpty = haveLast_ && last_ == SnapshotValidity::ValidEmpty;
        if (validity == SnapshotValidity::ValidEmpty && haveLast_ && last_ != SnapshotValidity::ValidEmpty &&
            last_ != SnapshotValidity::Invalid) {
            ev.logTransition = true;
            ev.severity = LogSeverityHint::Info;
            ev.message = "no civic facilities in the city (VALID_EMPTY); nothing to manage, vanilla controls untouched";
        } else if (validity != SnapshotValidity::ValidEmpty && wasEmpty) {
            ev.logTransition = true;
            ev.severity = LogSeverityHint::Info;
            ev.message = "civic facilities present again; normal operation resumes automatically";
        }
        last_ = validity;
        haveLast_ = true;
        return ev;
    }

private:
    std::uint32_t epoch_ = 1;
    std::map<FacilityKey, float> lastWritten_;
    std::map<FacilityKey, FacilityIdentity> notWritable_;
    std::map<FacilityKey, std::pair<FacilityIdentity, CapacityPrior>> capacity_;
    std::map<FacilityKey, std::vector<std::pair<std::uint32_t, std::int64_t>>> usage_;
    std::uint32_t epochMonth_ = 0;
    SnapshotValidity last_ = SnapshotValidity::ValidEmpty;
    bool haveLast_ = false;
    int invalidStreak_ = 0;
};

// What became of one attempted Civic write. The setter's return value is NOT part of it: the read-back decides.
enum class CivicWriteOutcome {
    PreWriteSkip,          // compare-and-set refused before the setter ran: nothing changed
    PostWriteRereadFailed, // the setter ran, the facility could not be read again: result unknown
    PostWriteVerifyFailed, // the setter ran and VerifyReadback failed (any channel: building, buffer, budget, secondary, ...)
    PostWriteVerified      // the setter ran and every representation agrees
};

// Trust guard for Civic writes; not an economic policy. `tripped` is the Civic writer's own trust
// failure and ends with the city lifecycle; `blocked` is an external hard stop and deliberately
// survives ResetCity().
// ONE facility whose write cannot be verified is quarantined on its own (CivicSession::QuarantineNotWritable,
// by the caller): the other schools and clinics carry on, since one failing building must never block
// the chain. Only when kTripAfterFacilities DIFFERENT facilities failed in the same city lifecycle is
// the problem the writer itself, not a building: then every Civic write stops for the lifecycle.
struct CivicWriterGuard {
    static constexpr int kTripAfterFacilities = 3;
    bool tripped = false;
    bool blocked = false;
    int failedFacilities = 0;           // facilities whose post-write state could not be verified in this city lifecycle (each one is quarantined)

    static constexpr bool TripsWriter(CivicWriteOutcome o) {
        return o == CivicWriteOutcome::PostWriteRereadFailed || o == CivicWriteOutcome::PostWriteVerifyFailed;
    }
    // Counts one failed facility. Returns true exactly when this failure tripped a writer that was still trusted (log the safety line once).
    bool Observe(CivicWriteOutcome o) {
        if (!TripsWriter(o)) return false;
        ++failedFacilities;
        if (tripped || failedFacilities < kTripAfterFacilities) return false;
        tripped = true;
        return true;
    }
    bool WritesAllowed() const { return !tripped && !blocked; }
    void ResetCity() { tripped = false; failedFacilities = 0; }  // never touches `blocked`
};

} // namespace sc4af
