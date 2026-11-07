// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Query-window semantics of a facility, keyed by its query layout (occupant property
// 0x2A499F85). The compiled vanilla table is generated offline from the game's own UI layouts.
// Pure logic.

#include "CivicPolicy.h"
#include "QuerySemanticsTable.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

namespace sc4af {

inline const char* QueryUnitName(QueryUnit u) {
    switch (u) {
    case QueryUnit::Students: return "students";
    case QueryUnit::Patients: return "patients";
    case QueryUnit::Books: return "books";
    case QueryUnit::Visits: return "visits";
    case QueryUnit::Patrons: return "patrons";
    case QueryUnit::None: break;
    }
    return "none";
}

inline bool ParseQueryUnit(const std::string& s, QueryUnit& out) {
    for (auto u : {QueryUnit::None, QueryUnit::Students, QueryUnit::Patients, QueryUnit::Books,
                   QueryUnit::Visits, QueryUnit::Patrons}) {
        if (s == QueryUnitName(u)) { out = u; return true; }
    }
    return false;
}

// Demand/capacity pair => policy signal (units must match; no magnitude heuristics).
inline PolicyKind PolicyKindForLayout(const QueryLayoutSemantics* layout) {
    if (!layout) return PolicyKind::UnknownSignal;
    if (layout->usageUnit == QueryUnit::None || layout->capacityUnit == QueryUnit::None)
        return PolicyKind::NoDemandSignal;
    if (layout->usageUnit != layout->capacityUnit) return PolicyKind::UnknownSignal;
    return (layout->usageUnit == QueryUnit::Students || layout->usageUnit == QueryUnit::Patients)
               ? PolicyKind::DemandCapacity : PolicyKind::SpecialDemandCapacity;
}

// Only the compiled VANILLA layouts are recognised. A custom query layout is never mapped: it is a
// different mechanism (policy: support vanilla mechanisms, never a mod as a target).
inline const QueryLayoutSemantics* LookupQueryLayout(std::uint32_t layout) { return FindQueryLayout(layout); }

// A vanilla layout is compatible with a family only if its unit pair belongs to that family's
// vanilla mechanism: education = students/books/visits, health = patients.
inline bool LayoutMatchesFamily(const QueryLayoutSemantics* layout, FundingFamily family) {
    if (!layout) return false;
    if (family == FundingFamily::Health) return layout->usageUnit == QueryUnit::Patients;
    return layout->usageUnit == QueryUnit::Students || layout->usageUnit == QueryUnit::Books ||
           layout->usageUnit == QueryUnit::Visits;
}

} // namespace sc4af
