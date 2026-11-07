// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sc4af {

enum class CivicKind { School, Hospital };
enum class Evidence { HeaderCandidate, StaticTrace, RuntimeVerified, Unknown, Unavailable, Error };

template <class T> struct Observed {
    std::optional<T> value;
    const char* source = "none";
    Evidence evidence = Evidence::Unknown;
};

struct BudgetItemObservation {
    std::uint32_t departmentId = 0;
    std::uint32_t lineId = 0;
    std::uint32_t purposeId = 0;
    std::int64_t rawCost = 0;
    float rawLocalFunding = 0;
};

struct CivicProbeRecord {
    CivicKind kind;
    std::uintptr_t runtimeAddress = 0; // Ephemeral diagnostic token, never persisted or reused.
    Observed<std::uint32_t> buildingTypeRaw;
    Observed<std::uint32_t> lotTagRaw;
    std::vector<std::uint32_t> matchingKnownGroups;
    Observed<std::uint32_t> capacityRawA;
    Observed<std::uint32_t> capacityRawB;
    bool capacityCallReturned = false;
    Observed<std::uint32_t> capacity;
    Observed<float> fundingRawA;
    Observed<float> fundingRawB;
    Observed<std::uint32_t> usage;
    Observed<std::int64_t> monthlyCost;          // building funding line only
    Observed<float> buildingFunding;             // "Building Funding" control
    Observed<float> secondaryFunding;            // School Bus / Ambulance control (never automated)
    Observed<std::uint32_t> staffCount;          // teachers / doctors (capacity out2)
    std::vector<BudgetItemObservation> budgetItems;
};

struct CivicSnapshot {
    std::uint32_t citySerial = 0;
    bool schoolsRead = false;
    bool hospitalsRead = false;
    std::vector<CivicProbeRecord> schools;
    std::vector<CivicProbeRecord> hospitals;
    std::uint32_t errors = 0;
};

} // namespace sc4af
