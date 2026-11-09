// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "CivicEnumeration.h"
#include "CivicEvidence.h"
#include "CivicPolicy.h"
#include "CivicBuildingNotices.h"
#include "CivicWatch.h"

#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

class cISC4City;

namespace sc4af {

// Unified civic funding controller. Off by default. One evaluation is:
// READ (CivicFacilityIO) -> DECISION (pure DecideFunding) -> WRITE (validated vanilla setter,
// compare-and-set, exact secondary pass-back) -> VERIFY (pure VerifyReadback).
// When it runs:
//   Month  every SimNewMonth: the full pass (raises, reductions, calibration, usage history, evidence);
//   Open   on the first simulated day after a city is opened (and, while a facility has no usage sample yet, on the next days): RAISES only. Never inside PostCityInit;
//   Watch  on a SimNewDay when a facility is on strike, its coverage is below the safe margin or its demand now needs more funding: RAISES only, once per change of risk and every
//          kRecheckDays while it persists.
// Reductions (savings) happen only in the monthly pass: the service first, the economy after.
enum class EvalTrigger { Month, Open, Watch };
inline const char* TriggerName(EvalTrigger t) { return t == EvalTrigger::Month ? "MONTH" : t == EvalTrigger::Open ? "OPEN" : "WATCH"; }
class AutoFinance {
public:
    void SetConfigDirectory(const std::string& directory) {
        configPath_ = directory + "DepartmentEfficiency.ini";      // the one file; this module reads its [AutoFinance] section
    }

    // (Re)loads the [AutoFinance] section of DepartmentEfficiency.ini when the file changed. Missing file or section => Mode=Off.
    void LoadConfigIfChanged();

    // One line describing mode, reserve and the classes/shapes that may be written.
    std::string Banner() const;

    // Called from SimNewMonth with the borrowed city. Shadow logs decisions, Auto also writes.
    void OnMonth(cISC4City* city, std::uint32_t month);

    // Called from SimNewDay with the borrowed city: the open evaluation, then the daily risk watch (see EvalTrigger). Read-only until it decides to evaluate.
    void OnDay(cISC4City* city, std::uint32_t month);

    // What the last evaluation found that funding CANNOT solve: facilities already at the maximum funding whose coverage is below the strike margin (or on strike). Source of the player notice.
    // Per facility TYPE (CivicType): the buildings PROVEN unable to cover the demand even at the maximum funding (ShortageKind::Proven). The worst of them (lowest attainable coverage) is the one a
    // notice can point to.
    struct KindShortage {
        int count = 0;
        std::string detail;                         // the worst building: cell, usage, capacity attainable
        double coverage = 1.0;                      // the lowest attainable coverage among the affected buildings
        std::vector<AffectedBuilding> buildings;    // every proven building of the type, NOT on a strike (a strike is the game's own alert), with its current cell, name and coverage
    };
    struct CivicShortage {
        bool evaluated = false;
        KindShortage kinds[kCivicKinds];
        int transientStrikes = 0;                   // on strike at the maximum funding, but the capacity without the strike covers the demand (not a missing facility)
    };
    const CivicShortage& shortage() const { return shortage_; }

    // City unloaded / (re)loaded: drop everything that belonged to the previous lifecycle.
    void ResetCity() {
        session_.ResetCity();
        notOfFamilyLogged_.clear();
        guard_.ResetCity();  // clears the Civic trust failure of the previous city; the external hard stop (blocked) stays
        eligibilitySeen_.clear();
        latch_.Reset();
        driftGate_.Reset();
        strikeStart_.clear();
        dayInCity_ = 0;
        open_.Reset();
        lastEvalComplete_ = false;
        shortage_ = CivicShortage{};
    }

    const AutoFinanceConfig& config() const { return config_; }

    // Hard stop: the controller keeps reading and logging but never writes again.
    void SetWritesBlocked(bool blocked) { guard_.blocked = blocked; }
    // false after a hard stop (failed unified session) or after the writer distrusted itself in this city: the Department then does NOT control the budgets and the game's own advice must come back
    bool WritesAllowed() const { return guard_.WritesAllowed(); }
    // false while the latest facility snapshot is INVALID (nothing readable, so nothing is written): the Department does not control the budgets then either.
    bool SnapshotUsable() const { return !session_.LastSnapshotInvalid(); }
    // The failure notices (FailureNotice.h): the civic writer distrusted itself in this city (3 different facilities could not be verified): a definitive failure until the city is loaded again.
    bool WriterTripped() const { return guard_.tripped; }

    // What the last OnMonth observed and verified (evidence only; never used for decisions).
    const CivicMonthEvidence& evidence() const { return evidence_; }

private:
    std::string configPath_;
    std::uint64_t lastWriteTime_ = ~0ull;
    AutoFinanceConfig config_;
    CivicSession session_;  // drift expectations and empty/invalid transitions for the current city
    std::string notOfFamilyLogged_;  // the listed occupants already reported as set aside (not a school / hospital) in this city
    CivicMonthEvidence evidence_;
    CivicShortage shortage_;
    AttentionLatch latch_;                                              // evaluation per change of risk (Watch)
    DriftGate driftGate_;                                               // early correction of a drift of the game, once per facility and monthly pass (CivicWatch.h)
    std::map<std::tuple<int, int, int>, std::uint32_t> strikeStart_;    // simulated day (since the city opened) a strike was first seen in a facility
    std::uint32_t dayInCity_ = 0;
    OpenSchedule open_;                                                 // the Open evaluation of this city lifecycle
    bool lastEvalComplete_ = false;                                     // the last evaluation found a usage sample for every managed facility
    std::map<std::tuple<int, int, int>, std::string> eligibilitySeen_;  // last logged eligibility verdict per facility
    // tripped: an unverified post-write state disables Civic writes for this city lifecycle (cleared by ResetCity).
    // blocked: external hard stop; ResetCity must NOT reopen it.
    CivicWriterGuard guard_;

    void TripOnUnverifiedWrite(CivicWriteOutcome outcome, const std::string& reason, const FacilityKey& facility, const CivicSession::FacilityIdentity& identity);
    // `driftTargets`: facilities whose funding the game moved on its own right after the monthly pass (DriftGate); only they may also be reduced outside the monthly pass.
    void Evaluate(cISC4City* city, std::uint32_t month, EvalTrigger trigger, const std::vector<std::tuple<int, int, int>>* driftTargets = nullptr);
    void WatchTick(cISC4City* city, std::uint32_t month, bool strikeNow);
};

// Writer shapes whose complete vanilla transaction has been validated at runtime.
bool IsValidatedWriterShape(WriterShape shape);
// Whether library/museum-like (non-person) signals have been verified for automation.
bool SpecialSignalsVerified();

} // namespace sc4af
