// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure (SC4-free) decisions of the Civic controller about WHEN to act and WHAT to tell the player,
// not about the funding number itself:
//  * NeedsAttention: is this facility at risk of a strike (or already in one), so that an evaluation must not wait for the monthly pass?
//  * AttentionLatch: one evaluation per change of risk, plus a periodic re-check while a risk persists (no daily full evaluation of the same unchanged problem).
//  * JudgeShortage: is the service PROVEN impossible to provide even at the maximum funding? Only that may ever reach the player.

#include "CivicPolicy.h"

#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <tuple>

namespace sc4af {

// The supported VANILLA facility types a shortage notice is written for (one notice per type and episode,
// however many buildings of that type are affected). The order must match the advice text tables; the
// advisor events are kAdviceCivicBase + index (NativeNotice.h). Any other subtype (private schools, a
// subtype the classifier cannot name, a non-vanilla mechanism) is NOT a kind: nothing is written for it
// and nothing is announced.
enum class CivicType { Elementary = 0, HighSchool, College, Library, Museum, Clinic, Hospital, ResearchCenter, None = -1 };
constexpr int kCivicKinds = 8;
struct CivicKindRow { const char* subtype; CivicType kind; const char* name; };
constexpr CivicKindRow kCivicKindTable[kCivicKinds] = {
    {"elementary", CivicType::Elementary, "ELEMENTARY"},
    {"high_school", CivicType::HighSchool, "HIGH_SCHOOL"},
    {"college_or_university", CivicType::College, "COLLEGE"},
    {"library", CivicType::Library, "LIBRARY"},
    {"museum", CivicType::Museum, "MUSEUM"},
    {"clinic_or_medical", CivicType::Clinic, "CLINIC"},
    {"large_medical_center", CivicType::Hospital, "HOSPITAL"},
    {"disease_research_center", CivicType::ResearchCenter, "RESEARCH_CENTER"},
};
inline CivicType CivicKindOf(const std::string& subtype) {
    for (const auto& row : kCivicKindTable) if (subtype == row.subtype) return row.kind;
    return CivicType::None;
}
inline const char* CivicKindName(CivicType k) { return k == CivicType::None ? "NONE" : kCivicKindTable[static_cast<int>(k)].name; }

// The game rolls for a strike from coverage 0.8 downwards (chance 0 at 0.8, 100 at 0) and the native pre-strike advice needs a chance above 10, i.e. coverage below 0.72
// (Lua: `game.g_school_strike_chance > STRIKE_LOW`, STRIKE_LOW = 10). The Department keeps well clear of both.
constexpr float kWatchCoverage = kStrikeSafeCoverage;

// The coverage the game rolls on is min(1, capacity / usage). The risk watch looks at the same ratio from
// the live numbers: the capacity in force against the usage now. At or above this the service is assured
// and the fine tuning (the 5% reserve) waits for the monthly pass; below it the demand has already outrun the funding.
constexpr double kAttentionCoverage = 0.97;

// Re-check of a risk that funding has not (yet) removed: every kRecheckDays simulated days. A CHANGE of the risk triggers at once.
constexpr std::uint32_t kRecheckDays = 7;

struct Attention {
    bool needed = false;
    std::string why;
};

// Is there something the Department can DO now that must not wait for the monthly pass? Only a RAISE that the policy would make: a strike in progress (funding below the maximum), or a demand
// that has outrun the capacity in force. A facility the Department cannot write (not locally funded, not a vanilla mechanism, no signal), one already at the maximum funding (nothing left to
// raise: that is a shortage question, judged by JudgeShortage and told by the persistence rule) and a possible saving are NOT attention. `f` must be prepared like the monthly pass prepares
// it (strike signals, usage volatility, capacity prior). Never writes, never logs.
inline Attention NeedsAttention(const FacilityState& f, const PolicyConfig& config) {
    Attention a;
    if (PercentForFraction(f.buildingFunding) >= config.maxFundingPercent) return a;
    const Decision d = DecideFunding(f, config);
    if (d.action != Action::Write || d.calibration || !(d.targetFraction > f.buildingFunding + 0.005f)) return a;      // not writable, no signal, or nothing to raise
    if (f.onStrike) { a.needed = true; a.why = "strike in progress"; return a; }
    if (f.usage && *f.usage > 0 && static_cast<double>(f.capacity) < static_cast<double>(*f.usage) * kAttentionCoverage) {
        std::ostringstream o;
        o << "capacity " << f.capacity << " against usage " << *f.usage << " (coverage " << static_cast<double>(f.capacity) / static_cast<double>(*f.usage) << "): the demand now needs "
          << d.targetPercent << "% (funding " << PercentForFraction(f.buildingFunding) << "%)";
        a.needed = true;
        a.why = o.str();
    }
    return a;
}

// Per facility: the signature of the risk that last triggered an evaluation. A new or changed signature triggers; the same one triggers again only after kRecheckDays.
class AttentionLatch {
public:
    using Key = std::tuple<int, int, int>;
    void Reset() { latched_.clear(); }
    bool Trigger(const Key& key, const std::string& signature, std::uint32_t day) {
        auto it = latched_.find(key);
        if (it == latched_.end() || it->second.signature != signature || day - it->second.day >= kRecheckDays) {
            latched_[key] = Entry{signature, day};
            return true;
        }
        return false;
    }
    void Clear(const Key& key) { latched_.erase(key); }
    bool Active(const Key& key) const { return latched_.count(key) != 0; }

private:
    struct Entry { std::string signature; std::uint32_t day = 0; };
    std::map<Key, Entry> latched_;
};

// The Open evaluation: on the first simulated day after a city is opened (never inside PostCityInit). If some facility has no usage sample yet, it is retried on the following days, a bounded number of times.
constexpr int kOpenAttempts = 6;
class OpenSchedule {
public:
    void Reset() { pending_ = true; attempts_ = 0; }
    bool pending() const { return pending_; }
    int attempts() const { return attempts_; }
    // After an Open evaluation. complete = every managed facility had a usage sample.
    void Evaluated(bool complete) {
        ++attempts_;
        if (complete || attempts_ >= kOpenAttempts) pending_ = false;
    }

private:
    bool pending_ = true;
    int attempts_ = 0;
};

// What funding can and cannot do for one facility. Only `Proven` may reach the player.
enum class ShortageKind {
    None,             // funding can still solve it (not at the maximum yet), or the service is assured
    Proven,           // at the maximum funding the capacity attainable is still below the safe coverage of the demand: a new facility is needed
    TransientStrike,  // on strike at the maximum funding, but the capacity attainable WITHOUT the strike covers the demand: the strike cut the capacity, it is not a missing facility
    BelowTarget       // at the maximum funding, below the 5% reserve / 0.95 target but above the safe coverage: the service is assured, nothing to tell
};

struct ShortageJudgement {
    ShortageKind kind = ShortageKind::None;
    double potentialCoverage = 1.0;   // min(1, capacity attainable at the maximum funding / usage)
    std::int64_t potentialCapacity = 0;
    std::string detail;
};

inline ShortageJudgement JudgeShortage(const FacilityState& f, const PolicyConfig& config) {
    ShortageJudgement j;
    if (!f.usage || *f.usage <= 0 || f.baseCapacity <= 0) return j;
    if (PercentForFraction(f.buildingFunding) < config.maxFundingPercent) return j;     // the Department can still raise it: not a shortage yet
    // During a strike the measured capacity has the staff term cut: the capacity that matters is what the building gives at this funding WITHOUT the strike. Outside a strike the measured
    // capacity at the maximum funding is the best there is.
    double potential = static_cast<double>(f.capacity);
    if (f.onStrike) {
        const double factor = f.capacityPrior ? f.capacityPrior->factor : 1.0;
        potential = factor * static_cast<double>(f.baseCapacity) * CapacityFactor(FractionForPercent(config.maxFundingPercent)) - static_cast<double>(config.capacityJitter);
    }
    j.potentialCapacity = static_cast<std::int64_t>(potential);
    j.potentialCoverage = std::min(1.0, potential / static_cast<double>(*f.usage));
    std::ostringstream o;
    o << "cell " << f.cellX << ',' << f.cellZ << " usage " << *f.usage << " capacity attainable at the maximum funding " << j.potentialCapacity << " (coverage " << j.potentialCoverage << ")";
    j.detail = o.str();
    if (j.potentialCoverage < static_cast<double>(kStrikeSafeCoverage)) j.kind = ShortageKind::Proven;
    else if (f.onStrike) j.kind = ShortageKind::TransientStrike;
    else if (j.potentialCoverage < kStrikeTargetCoverage) j.kind = ShortageKind::BelowTarget;
    return j;
}

// Early correction of a drift of the game (the Phantom Funding Slider Bug: the game itself moves a slider the Department had set, after an exemplar cost changed in a saved city).
// The Department does not diagnose that bug. It only recognises, with evidence, a change of funding that it did not make: its own last VERIFIED write of the facility (CivicSession) differs
// from the funding now, and the change is first seen right after a monthly pass. In four LIVE runs every one of the 30 changes the game made on its own came within one day of the monthly pass
// (offsets -1, 0, +1 days), none in the middle of a month; a change in the middle of a month is therefore never taken for the simulator's (it may be the player's) and waits for the monthly pass as before.
// The Watch ticks every second day, so a change one day after the pass can be seen at +1 or +2. At most one early correction per facility and monthly pass: no loop.
constexpr std::uint32_t kDriftWindowDays = 2;

class DriftGate {
public:
    using Key = std::tuple<int, int, int>;
    void Reset() { rollDay_ = 0; haveRoll_ = false; ++pass_; handled_.clear(); }
    void NoteMonthlyPass(std::uint32_t day) { rollDay_ = day; haveRoll_ = true; ++pass_; }
    // `expected`: the last verified write of this facility, if any. `day`: simulated day of the Watch tick.
    bool Qualifies(const Key& key, std::optional<float> expected, float current, std::uint32_t day) const {
        if (!haveRoll_ || !expected || NearlyEqual(current, *expected)) return false;
        if (day <= rollDay_ || day - rollDay_ > kDriftWindowDays) return false;
        const auto it = handled_.find(key);
        return it == handled_.end() || it->second != pass_;
    }
    void MarkHandled(const Key& key) { handled_[key] = pass_; }

private:
    std::uint32_t rollDay_ = 0;
    bool haveRoll_ = false;
    std::uint32_t pass_ = 0;                          // identifies the current monthly pass
    std::map<Key, std::uint32_t> handled_;            // the pass in which a facility's drift was already answered
};

// Outside the monthly pass only RAISES are written (the service first, the economy after). The one exception: the facility whose drift was just confirmed may also be reduced, to the minimum
// sufficient funding, at once. A calibration probe always waits for the monthly pass.
inline bool DeferToMonthlyPass(bool monthly, bool confirmedDriftTarget, bool reduction, bool calibration) {
    if (monthly) return false;
    if (calibration) return true;
    return reduction && !confirmedDriftTarget;
}

}  // namespace sc4af
