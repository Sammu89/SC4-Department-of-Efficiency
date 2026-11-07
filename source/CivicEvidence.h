// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure evidence of one SimNewMonth evaluation of the civic (Education and Health) controller. The civic controller is separate from the
// unified utility controller; these counters let the unified tracker report on it and show that the secondary funding lines (school bus,
// ambulance) are preserved. No game types here.

#include <vector>

namespace sc4af {

// One verified funding write of one facility, with the EXTERNAL inputs the decision rested on (never the funding the algorithm itself chose, nor the
// capacity that follows from it): the demand sample and the facility's base capacity. usageKnown=false means the cause cannot be verified.
struct CivicWriteEvent {
    int cellX = 0, cellZ = 0;
    int direction = 0;           // +1 raise, -1 cut
    bool usageKnown = false;
    long long usage = 0;
    long long baseCapacity = 0;
};

struct CivicFamilyEvidence {
    std::vector<CivicWriteEvent> writes;   // the same writes as raises/cuts, per facility; empty with raises/cuts > 0 = cause unknown
    int facilities = 0;          // facilities of this family evaluated this month
    int writeDecisions = 0;      // decisions that called for a write
    int raises = 0, cuts = 0;    // verified writes by direction
    int applied = 0;             // verified writes
    int failed = 0;              // writes that did not verify (or could not be re-read)
    int serviceChecked = 0;      // verified writes whose resulting capacity was compared with the requirement
    int serviceBreach = 0;       // ... and was below it
    int secondaryChecked = 0;    // writes on facilities with a secondary funding line (bus / ambulance) whose pass-back was verified
    int secondaryDrift = 0;      // ... and the secondary line changed
    int insufficient = 0;        // facilities that cannot meet the requirement even at the maximum funding (informational)
    int calibrationWrites = 0;   // calibration raises that verified (not counted in applied / raises / writes)
    int curveIncompatible = 0;   // calibration writes whose funding verified but whose capacity did not follow a constant factor (informational; the facility gets no automatic cuts)
    int notWritable = 0;         // facilities whose setter was shown to have NO effect this month (excluded; neither a write nor a failure)
};

struct CivicMonthEvidence {
    bool evaluated = false;      // Mode=Auto ran this month on a valid (or partial) snapshot
    CivicFamilyEvidence education, health;
};

}  // namespace sc4af
