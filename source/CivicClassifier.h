// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

// Pure classification/math helpers (no SC4 dependency, unit-tested offline).
// The group and purpose IDs come from the vanilla civic building exemplars.
namespace sc4af {

namespace group {
constexpr std::uint32_t Police = 0x1500, Jail = 0x1501, Fire = 0x1502, School = 0x1503,
                        College = 0x1504, Library = 0x1505, Museum = 0x1506, Health = 0x1507,
                        Reward = 0x150B, Elementary = 0x150F, HighSchool = 0x1510,
                        ClinicOrMedical = 0x1513, PrivateSchool = 0x1514,
                        LargeMedical = 0x151A, DiseaseCenter = 0x151C;
} // namespace group

namespace purpose {
constexpr std::uint32_t SchoolStaff = 0xEA5654B6, SchoolBus = 0x4A5654BA,
                        HospitalStaff = 0xCA565486, HospitalAmbulance = 0xEA56549E;
} // namespace purpose

enum class CivicFamily { None, School, College, Health, OtherCivic };

struct CivicClass {
    CivicFamily family = CivicFamily::None;
    std::string subtype;
    bool operator==(const CivicClass& o) const { return family == o.family && subtype == o.subtype; }
};

inline const char* FamilyName(CivicFamily f) {
    switch (f) {
    case CivicFamily::School: return "school";
    case CivicFamily::College: return "college";
    case CivicFamily::Health: return "health";
    case CivicFamily::OtherCivic: return "other-civic";
    case CivicFamily::None: break;
    }
    return "none";
}

inline CivicClass ClassifyCivic(const std::vector<std::uint32_t>& groups,
                                const std::vector<std::uint32_t>& purposes = {}) {
    const auto has = [&](std::uint32_t id) {
        return std::find(groups.begin(), groups.end(), id) != groups.end();
    };
    const auto hasPurpose = [&](std::uint32_t id) {
        return std::find(purposes.begin(), purposes.end(), id) != purposes.end();
    };
    if (has(group::Health) || hasPurpose(purpose::HospitalStaff) ||
        hasPurpose(purpose::HospitalAmbulance)) {
        return {CivicFamily::Health,
                has(group::DiseaseCenter) ? "disease_research_center"
                : has(group::LargeMedical) ? "large_medical_center"
                : has(group::ClinicOrMedical) ? "clinic_or_medical" : "health_unspecified"};
    }
    if (has(group::School) || has(group::College)) {
        const char* sub = has(group::College) ? "college_or_university"
                        : has(group::PrivateSchool) ? "private_school"
                        : has(group::Elementary) ? "elementary"
                        : has(group::HighSchool) ? "high_school" : "school_unspecified";
        return {has(group::College) && !has(group::School) ? CivicFamily::College
                                                          : CivicFamily::School, sub};
    }
    // Libraries and museums share the Education staff budget purpose.
    if (has(group::Library)) return {CivicFamily::OtherCivic, "library"};
    if (has(group::Museum)) return {CivicFamily::OtherCivic, "museum"};
    if (hasPurpose(purpose::SchoolStaff) || hasPurpose(purpose::SchoolBus))
        return {CivicFamily::School, "school_by_budget_purpose_only"};
    if (has(group::Police)) return {CivicFamily::OtherCivic, "police"};
    if (has(group::Jail)) return {CivicFamily::OtherCivic, "jail"};
    if (has(group::Fire)) return {CivicFamily::OtherCivic, "fire"};
    return {};
}

// In game version 1.1.641.0, GetSchoolBuildings returns every building of the Education
// budget department: schools, colleges, libraries and museums.
inline bool ListedBySchoolSimulator(const CivicClass& c) {
    return c.family == CivicFamily::School || c.family == CivicFamily::College ||
           (c.family == CivicFamily::OtherCivic && (c.subtype == "library" || c.subtype == "museum"));
}

inline bool ListedByHospitalSimulator(const CivicClass& c) { return c.family == CivicFamily::Health; }

// The game's lists also return occupants that are NOT buildings of that family: GetHospitalBuildings
// can list a reward building (groups Reward 0x150B, 0x1006, DiseaseCenter 0x151C, no Health group,
// no hospital budget). Such an occupant is not a facility to fund: it is set aside and never counted
// as an unreadable hospital (which would make the whole civic snapshot INVALID every month).
inline bool BelongsToList(bool schoolList, const CivicClass& c) { return schoolList ? ListedBySchoolSimulator(c) : ListedByHospitalSimulator(c); }

// Capacity model: Estimate capacity out1 ~= base * buildingFunding within +-1 (the estimate
// jitters between calls, e.g. 181/182 or 4323/4324), out2 (staff) ~= ceil(baseStaff * buildingFunding);
// base values are query-buffer +0x14 / +0x1C.
inline bool CapacityWithinModel(std::int64_t observed, std::int64_t base, float fundingFraction);
// Capacity as a function of funding: linear up to 100%; ABOVE 100% only half of the extra funding
// adds capacity (e.g. base 3000 gives 3060 / 3180 / 3300 at 104% / 112% / 120% once a constant
// city factor of 1.15 is divided out).
inline double CapacityFactor(float fundingFraction) {
    const double f = static_cast<double>(fundingFraction);
    return f <= 1.0 ? f : 1.0 + (f - 1.0) * 0.5;
}
inline std::int64_t EstimateCapacityFromBase(std::int64_t base, float fundingFraction) {
    // Product in extended precision of the stored float fraction, truncated
    // (18400 * 0.235f = 4323.99999 -> 4323).
    return static_cast<std::int64_t>(static_cast<double>(base) * CapacityFactor(fundingFraction));
}

inline bool CapacityWithinModel(std::int64_t observed, std::int64_t base, float fundingFraction) {
    const auto model = EstimateCapacityFromBase(base, fundingFraction);
    return observed >= model - 1 && observed <= model + 1;
}

// Current visible monthly cost of one budget line: round-to-nearest of cost * localFunding.
inline std::int64_t RoundedFundingCost(std::int64_t rawCost, float localFunding) {
    return static_cast<std::int64_t>(std::llround(static_cast<double>(rawCost) * localFunding));
}

} // namespace sc4af
