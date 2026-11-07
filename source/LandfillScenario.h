// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Landfill size counterfactuals. Pure, header-only, no game types.
//
// This file builds the COUNTERFACTUAL INPUT of "what if the landfill had delta more / fewer tiles". It contains no simulator: the funding search and the service
// verdict stay in PlanGarbageFunding / EvaluateGarbageServiceAt, which the caller runs on `LandfillScenario::input`. Only quantities the game's own fields prove are
// changed; everything else (reserve, contracts, accumulated garbage, W2E / incinerators, backlog, calendar, curves) is copied untouched.
//
// What changes with n added tiles (EXPAND):
//   * landfillTiles += n              -> the valve allowance round(curve(tiles) x eff) - supply moves exactly (5000 per tile at the installed curve): PROVEN, it is the
//                                        same function ComputeLandfillValve / LandfillHeadroomAt evaluate, and the per-zone intake cap tiles x 5000 of the month boundary.
//   * landfillMaintenance += ceil(maint x n / tiles) (the landfill maintenance B). The Department ideal INCLUDES B at 100% funding (ideal == B in a city with no other line)
//                                        and the charge is ideal x percent, so idealMonthly' = idealMonthly + the same delta, ONCE.
//   * landfillCapacity, landfillUsage, geometry (activeCells, cellCapacity): UNCHANGED. A zone tile is not an active storage cell (240 tiles / 182 active cells), so the
//                                        storage a new tile brings is NOT provable: the scenario is credited ZERO new storage (conservative: it can only make the service
//                                        verdict harder to pass). The snapshot stays self-consistent (capacity == activeCells x cellCapacity), so a CALIBRATED geometry keeps
//                                        the dual horizon exactly as the model runs it; an unknown / ambiguous one keeps the continuity lock.
// What changes with n removed EMPTY tiles (REDUCE): nothing the aggregate data can prove (see LandfillRemovableEvidence). Without a validated reader of the removable
// tiles the direction is disabled: maxReduce = 0 and BuildLandfillScenario(delta < 0) is not demonstrable.

#include "GarbageControl.h"

#include <algorithm>
#include <cstdint>
#include <string>

namespace sc4af {

// Largest zone tile count the installed transport curve describes (x of its last point). Beyond it the allowance would be extrapolated: not provable.
constexpr std::int64_t kLandfillCurveMaxTiles = 65536;
// Largest expansion recommended relative to the current size: the average maintenance per tile (below) is only an observation of the CURRENT mix of tiles, so it is not
// extrapolated beyond doubling the landfill.
constexpr int kLandfillExpandMaxFactor = 1;      // maxExpand <= tiles x factor
// UNVERIFIED community figure (50 per tile). Informational one-off cost only, never part of a monthly saving.
constexpr std::int64_t kLandfillTileBuildCostCommunity = 50;

// Extension point for REDUCE. A future VALIDATED reader (per-tile occupancy of the landfill zone) fills this; nothing in the product fills it today.
// `validated` must be true only when the reader proves that `removableTiles` tiles hold no garbage and can be removed by the player. The quantities below must come
// from the same validated source: none of them is derivable from the aggregate fields.
struct LandfillRemovableEvidence {
    bool validated = false;
    int removableTiles = 0;                          // proven empty AND removable tiles (a lower bound)
    std::int64_t capacityPerTile = 0;                // storage lost per removed tile (> 0 required)
    Opt<std::uint32_t> activeCellsPerTile;           // active storage cells lost per removed tile; unknown -> the geometry falls back to the continuity lock
    Opt<std::int64_t> maintenancePerEmptyTile;       // maintenance saved per removed (empty) tile
    Opt<std::int64_t> removalCostPerTile;            // one-off cost
    std::string source;
};

struct LandfillScenario {
    int delta = 0;                         // +n expand, -n reduce (tiles)
    bool demonstrable = false;             // geometry/physics (and for REDUCE removability) PROVEN, else excluded from advice
    std::string reason;                    // stable code when not demonstrable
    GarbageServiceInput input;             // the counterfactual input
    Opt<std::int64_t> idealMonthly;        // counterfactual department ideal monthly (maintenance delta applied exactly once)
    Opt<std::int64_t> initialCost;         // one-off build/removal cost when demonstrable; reported separately, never in the monthly saving
};

struct LandfillLimits {
    int maxExpand = 0;                     // largest n whose EXPAND scenario can be proven; 0 = direction disabled
    int maxReduce = 0;                     // largest n whose REDUCE scenario can be proven; 0 = direction disabled
    std::string expandNote, reduceNote;    // stable code (or the bound that applies) per direction
};

namespace landfill_scenario_detail {

// Everything the EXPAND / REDUCE scenarios need from the base. Returns "" when all is readable.
inline std::string BaseProblem(const GarbageServiceInput& base, Opt<std::int64_t> ideal) {
    const auto& a = base.aggregate;
    if (!a.landfillTiles) return "TILES_UNKNOWN";
    if (*a.landfillTiles <= 0) return "NO_LANDFILL";
    if (!ideal) return "IDEAL_UNKNOWN";
    if (!a.landfillMaintenance || *a.landfillMaintenance <= 0) return "MAINTENANCE_UNKNOWN";
    if (*ideal < *a.landfillMaintenance) return "IDEAL_DOES_NOT_COVER_MAINTENANCE";   // the ideal must contain B (observed); otherwise the delta would be applied to a number that lacks it
    if (!base.transportCurve.valid()) return "TRANSPORT_CURVE_INVALID";
    if (!a.garbageSupply || !a.landfillUsage || !a.landfillCapacity || *a.landfillCapacity < *a.landfillUsage || !a.fundingEfficiency) return "VALVE_INPUTS_UNKNOWN";
    if (!a.produced || !a.recycled || !a.imported || !a.incineratorCapacityMonthly || !a.w2eCapacityMonthly) return "SERVICE_INPUTS_UNKNOWN";
    return {};
}

// ceil(maint x n / tiles): the average maintenance per tile of the CURRENT landfill, rounded up. A new tile is empty, and the observed maintenance rises with the
// occupancy at constant tiles (66 tiles: 1290 -> 1385 while usage grew), so an empty tile costs no more than the average: the cost of an expansion is never under-stated.
inline std::int64_t ExpandMaintenanceDelta(std::int64_t maintenance, std::int64_t tiles, int n) {
    return (maintenance * n + tiles - 1) / tiles;
}

inline int ExpandBound(std::int64_t tiles) {
    const std::int64_t byCurve = kLandfillCurveMaxTiles - tiles;
    const std::int64_t byProportion = tiles * kLandfillExpandMaxFactor;
    return static_cast<int>(std::max<std::int64_t>(0, std::min(byCurve, byProportion)));
}

// Largest n the evidence proves removable without breaking the storage (the capacity left must hold the usage) and keeping at least one tile.
inline int ReduceBound(const GarbageServiceInput& base, const LandfillRemovableEvidence* ev) {
    if (!ev || !ev->validated || ev->removableTiles <= 0 || ev->capacityPerTile <= 0 || !ev->maintenancePerEmptyTile || *ev->maintenancePerEmptyTile < 0) return 0;
    const auto& a = base.aggregate;
    const std::int64_t free = static_cast<std::int64_t>(*a.landfillCapacity) - static_cast<std::int64_t>(*a.landfillUsage);
    std::int64_t n = std::min<std::int64_t>(ev->removableTiles, *a.landfillTiles - 1);
    n = std::min<std::int64_t>(n, free / ev->capacityPerTile);
    return static_cast<int>(std::max<std::int64_t>(0, n));
}

} // namespace landfill_scenario_detail

// REDUCE verdict with the data available: no field of the snapshot says which SPATIAL tiles are empty, so without `removable` maxReduce is 0.
inline LandfillLimits LandfillScenarioLimits(const GarbageServiceInput& base, Opt<std::int64_t> idealMonthly, const LandfillRemovableEvidence* removable = nullptr) {
    LandfillLimits lim;
    const std::string problem = landfill_scenario_detail::BaseProblem(base, idealMonthly);
    if (!problem.empty()) {
        lim.expandNote = problem;
        lim.reduceNote = problem;
        return lim;
    }
    lim.maxExpand = landfill_scenario_detail::ExpandBound(*base.aggregate.landfillTiles);
    lim.expandNote = lim.maxExpand > 0 ? "VALVE_EXACT_STORAGE_NOT_CREDITED" : "TILES_BEYOND_CURVE";
    if (!removable || !removable->validated) {
        lim.reduceNote = "REDUCE_REMOVABLE_TILES_NOT_READABLE";
    } else {
        lim.maxReduce = landfill_scenario_detail::ReduceBound(base, removable);
        lim.reduceNote = lim.maxReduce > 0 ? "REMOVABLE_TILES_VALIDATED" : "REDUCE_EVIDENCE_INCOMPLETE_OR_NO_FREE_STORAGE";
    }
    return lim;
}

inline LandfillScenario BuildLandfillScenario(const GarbageServiceInput& base, Opt<std::int64_t> idealMonthly, int delta, const LandfillRemovableEvidence* removable = nullptr) {
    LandfillScenario s;
    s.delta = delta;
    s.input = base;
    if (delta == 0) { s.reason = "DELTA_ZERO"; return s; }
    const std::string problem = landfill_scenario_detail::BaseProblem(base, idealMonthly);
    if (!problem.empty()) { s.reason = problem; return s; }
    auto& a = s.input.aggregate;
    const std::int64_t tiles = *base.aggregate.landfillTiles;
    const std::int64_t maint = *base.aggregate.landfillMaintenance;

    if (delta > 0) {
        if (delta > landfill_scenario_detail::ExpandBound(tiles)) { s.reason = tiles + delta > kLandfillCurveMaxTiles ? "TILES_BEYOND_CURVE" : "EXPAND_EXCEEDS_PROVEN_LIMIT"; return s; }
        const std::int64_t dm = landfill_scenario_detail::ExpandMaintenanceDelta(maint, tiles, delta);
        a.landfillTiles = static_cast<std::int32_t>(tiles + delta);
        a.landfillMaintenance = maint + dm;
        s.idealMonthly = *idealMonthly + dm;                       // the ONLY place the maintenance delta enters the ideal
        s.initialCost = static_cast<std::int64_t>(delta) * kLandfillTileBuildCostCommunity;
        s.demonstrable = true;
        return s;
    }

    const int n = -delta;
    if (!removable || !removable->validated) { s.reason = "REDUCE_REMOVABLE_TILES_NOT_READABLE"; return s; }
    if (removable->capacityPerTile <= 0 || !removable->maintenancePerEmptyTile || *removable->maintenancePerEmptyTile < 0) { s.reason = "REDUCE_EVIDENCE_INCOMPLETE"; return s; }
    if (n > removable->removableTiles || n >= tiles) { s.reason = "REDUCE_EXCEEDS_REMOVABLE"; return s; }
    if (n > landfill_scenario_detail::ReduceBound(base, removable)) { s.reason = "REDUCE_EXCEEDS_FREE_STORAGE"; return s; }
    const std::int64_t dm = static_cast<std::int64_t>(n) * *removable->maintenancePerEmptyTile;
    if (dm > maint) { s.reason = "REDUCE_EVIDENCE_INCOMPLETE"; return s; }
    const std::uint64_t newCapacity = *base.aggregate.landfillCapacity - static_cast<std::uint64_t>(n) * static_cast<std::uint64_t>(removable->capacityPerTile);
    a.landfillTiles = static_cast<std::int32_t>(tiles - n);
    a.landfillCapacity = static_cast<std::uint32_t>(newCapacity);
    a.landfillMaintenance = maint - dm;
    // geometry: updated only when it stays exactly consistent, otherwise the model's own continuity lock (never half updated)
    const auto& g = base.geometry;
    bool keep = false;
    if (g.calibrated() && removable->activeCellsPerTile && *removable->activeCellsPerTile > 0) {
        const std::uint64_t lost = static_cast<std::uint64_t>(n) * *removable->activeCellsPerTile;
        if (lost < g.activeCells && static_cast<std::uint64_t>(g.activeCells - lost) * g.cellCapacity == newCapacity) {
            s.input.geometry.activeCells = static_cast<std::uint32_t>(g.activeCells - lost);
            keep = true;
        }
    }
    if (!keep) s.input.geometry = LandfillGeometryInfo{};
    s.idealMonthly = *idealMonthly - dm;
    if (removable->removalCostPerTile) s.initialCost = static_cast<std::int64_t>(n) * *removable->removalCostPerTile;
    s.demonstrable = true;
    return s;
}

} // namespace sc4af
