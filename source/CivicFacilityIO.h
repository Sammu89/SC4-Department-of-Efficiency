// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "CivicEnumeration.h"
#include "CivicPolicy.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class cISC4City;
class cISC4Occupant;

namespace sc4af {

// One civic facility read from the live city in a single call.
struct LiveFacility {
    cISC4Occupant* occupant = nullptr;  // valid only inside the callback that produced it
    FacilityState state;
    Readback readback;                  // getter / query buffer / budget item representations
    std::string exemplarName;           // log only, never an identity
    bool usageSampled = false;
};

// Result of one authoritative enumeration: validity comes from the read itself (never from history).
struct CivicEnumeration {
    EnumerationInput input;
    SnapshotValidity validity = SnapshotValidity::Invalid;
    std::vector<LiveFacility> facilities;
    std::string skipReasons;  // distinct per-facility read failures, for diagnostics only
    std::string notOfFamily;  // exemplar names of the listed occupants that are not of the list's family (BelongsToList), for diagnostics only
};

// The single shared civic enumeration. `epochMatches` is supplied by the caller, which knows the
// city lifecycle it was built for.
CivicEnumeration EnumerateFacilities(cISC4City* city, bool epochMatches);

// Reads every facility returned by GetSchoolBuildings (education department: schools, colleges,
// libraries, museums, ...) and GetHospitalBuildings. Facilities without a readable capacity are
// skipped. `specialSignalsVerified` selects whether library/museum-like signals may be used.
std::vector<LiveFacility> ReadFacilities(cISC4City* city, bool specialSignalsVerified);

// Read-only strike signals of one facility, taken from the game's own per-building properties: the strike roll (0x006FA280) reads the building's
// COVERAGE ratio (school 0x292453C5, hospital 0x092453CB; a float, strike chance 0 for >= 0.8, 100% at 0) and the strike state
// (0xEAA69F83: 0 = none, otherwise the start of the strike). Absent = unreadable.
struct StrikeSignals {
    std::optional<float> coverage;
    std::optional<std::uint32_t> strikeSince;
};
StrikeSignals ReadStrikeSignals(cISC4Occupant* occupant, FundingFamily family);

// Re-reads the current state of `occupant` (which must come from the same callback).
bool ReadFacility(cISC4City* city, cISC4Occupant* occupant, FundingFamily family,
                  bool specialSignalsVerified, LiveFacility& out);

// The single vanilla funding call. The secondary value is the exact live value that
// `target` was read with; it is passed back untouched. Returns the setter's boolean.
bool WriteBuildingFunding(cISC4City* city, const LiveFacility& target, float newBuildingFraction);

std::string DescribeFacility(const LiveFacility& f);

} // namespace sc4af
