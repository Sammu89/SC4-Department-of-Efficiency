// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Spatial landfill analysis: the concrete EXPAND / REDUCE placements and the ghost-tile repair. Pure, header-only, no game types, no I/O.
//
// The plugin fills a LandfillSpatialSnapshot once per relevant evaluation (LandfillZoneReader.cpp): the zone type of every map cell and the pile of the landfill tiles (public API),
// and, for each landfill zone, its box, bitmap and cell records (a guarded read-only view, SimCity 4 1.1.641.0 only). ValidateLandfillSpatial cross-checks that view against the game's
// own totals (capacity, usage, maintenance, tile count): one difference and nothing spatial is used (fail closed).
//
// Everything a placement does to the zone is computed by LandfillZoneModel (exact, validated in game). A placement is offered only when:
//   EXPAND  a rectangle of at least 2 x 2 tiles (the game's minimum zone size) touching the RIGHT or BOTTOM side of the zone box and inside its span (growing up or left would move every
//           cell away from its piles: never offered), anchored at a corner of that side so the player can find it; every tile unzoned (zone type 0); nothing sent to the valve, under
//           both possible roundings; the native dry run (PlaceZone, place = false) accepts it (the caller checks that for the short list).
//   REDUCE  the k outermost full rows / columns of the zone's landfill tiles on one side, forming a clean rectangle of landfill tiles of this zone; no pile on any of them; every
//           cell whose window holds one of them EMPTY; nothing sent to the valve; the remaining tiles one piece holding a 2 x 2 block; the native dezone dry run accepts it.
//   REPAIR  (ghost tiles) re-zoning as landfill the pile-free landfill tiles of the zone (no new tile): the game's add path reconciles the bitmap (0x6BCD70). Automatic only when the
//           capacity does not fall, nothing goes to the valve, no other zone is within one tile of the region (the add would merge it), and the native dry run accepts the region.
// Only zones read and validated are used; a city with more than one landfill zone gets no EXPAND / REDUCE advice (the notice cannot say which landfill), the repair works per zone.

#include "GarbageControl.h"
#include "LandfillScenario.h"
#include "LandfillZoneModel.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace sc4af {

constexpr int kLandfillZoneTypeLandfill = 14;           // cISC4ZoneManager::ZoneType::Landfill
constexpr int kLandfillMinZoneSize = 2;                 // GetMinZoneSize(Landfill) (PlaceZone error 3 below)
constexpr int kLandfillMaxZoneSize = 48;                // GetMaxZoneSize(Landfill) (PlaceZone error 4 above)
constexpr int kLandfillExpandMaxDepth = 8;              // largest depth of an expansion strip that is offered
constexpr int kLandfillReduceMaxRows = 8;               // largest number of edge rows / columns offered for removal
constexpr int kLandfillPlacementMaxTiles = 64;          // as the advisor's domain (kLandfillMaxTilesPerDirection)

struct LandfillPileInfo {
    bool pile = false;
    std::int64_t cost = kLandfillEmptyTileCost;         // property 0xEA54D286 of the pile, or 5
};

struct LandfillZoneView {
    std::uintptr_t id = 0;                              // identity only (never dereferenced by the pure code)
    LandfillZoneModel model;                            // box, bitmap, cells as read
    std::set<LandfillTile> ghosts;                      // bitmap tiles whose zone type is not landfill (zone-type ghosts)
    std::vector<LandfillTile> landfillTiles;            // bitmap tiles whose zone type IS landfill
};

struct LandfillSpatialSnapshot {
    bool read = false;                                  // the reader produced zones (false: problem says why)
    std::string problem;
    int mapW = 0, mapH = 0;
    std::vector<std::int8_t> zoneType;                  // mapW x mapH, index z * mapW + x
    std::map<LandfillTile, LandfillPileInfo> piles;     // every bitmap tile and every landfill-typed tile
    std::vector<LandfillZoneView> zones;
    std::int64_t apiTiles = 0, apiCapacity = 0, apiUsage = 0, apiMaintenance = 0;
    std::int64_t orphanLandfillTiles = 0;               // landfill-typed tiles in no zone bitmap
    long long readMicros = 0;

    int ZoneTypeAt(int x, int z) const { return (x < 0 || z < 0 || x >= mapW || z >= mapH) ? -1 : zoneType[static_cast<size_t>(z) * mapW + x]; }
    LandfillPileInfo PileAt(int x, int z) const { auto it = piles.find({x, z}); return it == piles.end() ? LandfillPileInfo{} : it->second; }
    std::int64_t GhostCount() const { std::int64_t n = 0; for (const auto& z : zones) n += static_cast<std::int64_t>(z.ghosts.size()); return n; }
};

// The public indication (no zone read): maintenance - sum of the landfill-typed tiles' costs = 5 x tiles paid for but not landfill. Only an INDICATION: it cannot say where, and a
// pile cost read wrongly would fake it. Returns nullopt when the difference is not a non-negative multiple of 5.
inline std::optional<std::int64_t> LandfillGhostIndication(std::int64_t apiMaintenance, std::int64_t sumLandfillTileCosts) {
    const std::int64_t d = apiMaintenance - sumLandfillTileCosts;
    if (d < 0 || d % kLandfillEmptyTileCost != 0) return std::nullopt;
    return d / kLandfillEmptyTileCost;
}

// Cross-checks the zone views against the game's totals. "" = consistent; otherwise a stable code (nothing spatial may be used).
inline std::string ValidateLandfillSpatial(const LandfillSpatialSnapshot& s) {
    if (!s.read) return s.problem.empty() ? "NOT_READ" : s.problem;
    if (s.mapW <= 0 || s.mapH <= 0 || s.zoneType.size() != static_cast<size_t>(s.mapW) * s.mapH) return "MAP_SIZE";
    if (s.zones.empty()) return "NO_ZONE";
    std::int64_t cap = 0, use = 0, cost = 0, typed = 0;
    std::set<LandfillTile> seen;
    for (const auto& z : s.zones) {
        const auto& m = z.model;
        if (m.W() < kLandfillMinZoneSize || m.H() < kLandfillMinZoneSize) return "ZONE_BOX";
        const std::int64_t c = m.CellCapacity();
        for (int ry = 0; ry < m.GH(); ++ry)
            for (int rx = 0; rx < m.GW(); ++rx) {
                const auto& cell = m.Cell(rx, ry);
                if (cell.stock < 0 || cell.stock > cell.capacity) return "CELL_STOCK";
                if (cell.active ? cell.capacity != c : cell.capacity != 0) return "CELL_CAPACITY";
            }
        cap += m.Capacity();
        use += m.Usage();
        for (const auto& t : m.Tiles()) {
            if (!seen.insert(t).second) return "TILE_IN_TWO_ZONES";
            cost += s.PileAt(t.first, t.second).cost;
            const int zt = s.ZoneTypeAt(t.first, t.second);
            const bool ghost = z.ghosts.count(t) != 0;
            if ((zt == kLandfillZoneTypeLandfill) == ghost) return "GHOST_LIST";
            if (!ghost) ++typed;
        }
    }
    if (cap != s.apiCapacity) return "CAPACITY_DIFFERS";
    if (use != s.apiUsage) return "USAGE_DIFFERS";
    if (cost != s.apiMaintenance) return "MAINTENANCE_DIFFERS";
    if (typed + s.orphanLandfillTiles != s.apiTiles) return "TILE_COUNT_DIFFERS";
    if (s.orphanLandfillTiles != 0) return "LANDFILL_TILES_OUTSIDE_ZONES";
    return {};
}

enum class LandfillPlacementKind { ExpandRightTop, ExpandRightBottom, ExpandDownLeft, ExpandDownRight, ReduceTop, ReduceBottom, ReduceLeft, ReduceRight, Repair };
inline const char* LandfillPlacementKindName(LandfillPlacementKind k) {
    switch (k) {
    case LandfillPlacementKind::ExpandRightTop: return "EXPAND_RIGHT_FROM_TOP";
    case LandfillPlacementKind::ExpandRightBottom: return "EXPAND_RIGHT_FROM_BOTTOM";
    case LandfillPlacementKind::ExpandDownLeft: return "EXPAND_BOTTOM_FROM_LEFT";
    case LandfillPlacementKind::ExpandDownRight: return "EXPAND_BOTTOM_FROM_RIGHT";
    case LandfillPlacementKind::ReduceTop: return "REDUCE_TOP_ROWS";
    case LandfillPlacementKind::ReduceBottom: return "REDUCE_BOTTOM_ROWS";
    case LandfillPlacementKind::ReduceLeft: return "REDUCE_LEFT_COLUMNS";
    case LandfillPlacementKind::ReduceRight: return "REDUCE_RIGHT_COLUMNS";
    case LandfillPlacementKind::Repair: return "REPAIR";
    }
    return "?";
}
inline bool IsExpand(LandfillPlacementKind k) { return k == LandfillPlacementKind::ExpandRightTop || k == LandfillPlacementKind::ExpandRightBottom || k == LandfillPlacementKind::ExpandDownLeft || k == LandfillPlacementKind::ExpandDownRight; }
inline bool IsReduce(LandfillPlacementKind k) { return k == LandfillPlacementKind::ReduceTop || k == LandfillPlacementKind::ReduceBottom || k == LandfillPlacementKind::ReduceLeft || k == LandfillPlacementKind::ReduceRight; }

struct LandfillRect { int x1 = 0, z1 = 0, x2 = -1, z2 = -1; int W() const { return x2 - x1 + 1; } int H() const { return z2 - z1 + 1; } };

struct LandfillPlacement {
    LandfillPlacementKind kind = LandfillPlacementKind::Repair;
    int zone = 0;                                       // index in the snapshot
    LandfillRect rect;                                  // the rectangle the player drags (REPAIR: the box of `tiles`)
    std::vector<LandfillTile> tiles;                    // the region (REPAIR: possibly not a full rectangle)
    int n = 0;                                          // tiles added / removed (REPAIR: 0)
    int across = 0, along = 0;                          // depth (columns for right / rows for bottom / rows or columns removed) and length along the side
    // the model's outcome for the zone
    std::int64_t tilesBefore = 0, tilesAfter = 0, cellsBefore = 0, cellsAfter = 0, cellCapBefore = 0, cellCapAfter = 0;
    std::int64_t capacityBefore = 0, capacityAfter = 0, usageBefore = 0, usageAfter = 0, pushed = 0;
    int ghostsReconciled = 0;
    std::int64_t maintenanceDelta = 0;                  // B' - B at the operation (piles grow later: not included)
    bool roundingAmbiguous = false;
    std::string refusal;                                // "" = physically safe as far as the pure checks go
};

namespace landfill_spatial_detail {

inline bool Connected(const std::set<LandfillTile>& tiles) {
    if (tiles.empty()) return false;
    std::set<LandfillTile> seen;
    std::vector<LandfillTile> todo{*tiles.begin()};
    while (!todo.empty()) {
        const auto t = todo.back(); todo.pop_back();
        if (!seen.insert(t).second) continue;
        for (const auto& n : {LandfillTile{t.first + 1, t.second}, LandfillTile{t.first - 1, t.second}, LandfillTile{t.first, t.second + 1}, LandfillTile{t.first, t.second - 1}})
            if (tiles.count(n) && !seen.count(n)) todo.push_back(n);
    }
    return seen.size() == tiles.size();
}
inline bool HasBlock2x2(const std::set<LandfillTile>& t) {
    for (const auto& [x, z] : t) if (t.count({x + 1, z}) && t.count({x, z + 1}) && t.count({x + 1, z + 1})) return true;
    return false;
}
// A rectangle any other zone's box (grown by one tile, as 0x6B9AC0 collects zones) touches: the operation would merge that zone.
inline bool TouchesOtherZone(const LandfillSpatialSnapshot& s, int zone, const LandfillRect& r) {
    for (size_t i = 0; i < s.zones.size(); ++i) {
        if (static_cast<int>(i) == zone) continue;
        const auto& m = s.zones[i].model;
        if (r.x1 <= m.x2 + 1 && r.x2 >= m.x1 - 1 && r.z1 <= m.y2 + 1 && r.z2 >= m.y1 - 1) return true;
    }
    return false;
}
// The cell capacity the other x87 precision would give (for a worst case), from the same T and N.
inline std::int64_t WorstPush(const LandfillZoneModel& before, const LandfillZoneModel& after, std::int64_t pushed, bool ambiguous) {
    if (!ambiguous || pushed > 0) return pushed;
    // one unit less per cell: every cell stocked above C - 1 would lose the difference
    const std::int64_t c = after.CellCapacity() - 1;
    std::int64_t p = 0;
    for (int ry = 0; ry < after.GH(); ++ry)
        for (int rx = 0; rx < after.GW(); ++rx)
            if (after.Cell(rx, ry).active && after.Cell(rx, ry).stock > c) p += after.Cell(rx, ry).stock - c;
    (void)before;
    return p;
}

inline void Fill(LandfillPlacement& p, const LandfillZoneView& z, const LandfillZoneModel& after, const LandfillZoneModel::Result& r) {
    const auto& m = z.model;
    p.tilesBefore = m.T(); p.tilesAfter = after.T();
    p.cellsBefore = m.N(); p.cellsAfter = after.N();
    p.cellCapBefore = m.CellCapacity(); p.cellCapAfter = after.CellCapacity();
    p.capacityBefore = m.Capacity(); p.capacityAfter = after.Capacity();
    p.usageBefore = m.Usage(); p.usageAfter = after.Usage();
    p.pushed = WorstPush(m, after, r.pushed, r.roundingAmbiguous);
    p.roundingAmbiguous = r.roundingAmbiguous;
    p.ghostsReconciled = r.ghostsReconciled;
}

}  // namespace landfill_spatial_detail

// EXPAND candidates of one zone (deterministic order: side right then bottom, anchor first corner then second, depth, length). Physically safe ones only (refusal empty) unless
// `withRefused` (diagnostics).
inline std::vector<LandfillPlacement> LandfillExpandCandidates(const LandfillSpatialSnapshot& s, int zone, bool withRefused = false) {
    using namespace landfill_spatial_detail;
    std::vector<LandfillPlacement> out;
    const auto& z = s.zones[static_cast<size_t>(zone)];
    const auto& m = z.model;
    if (z.landfillTiles.empty()) return out;
    // anchored on the landfill the player SEES (its landfill tiles), never on ghost rows of the box; the box may only grow right / down
    int tx1 = z.landfillTiles.front().first, tx2 = tx1, tz1 = z.landfillTiles.front().second, tz2 = tz1;
    for (const auto& [x, zz] : z.landfillTiles) { tx1 = std::min(tx1, x); tx2 = std::max(tx2, x); tz1 = std::min(tz1, zz); tz2 = std::max(tz2, zz); }
    struct Side { LandfillPlacementKind kind; bool right; bool fromStart; };
    const Side sides[] = {{LandfillPlacementKind::ExpandRightTop, true, true}, {LandfillPlacementKind::ExpandRightBottom, true, false},
                          {LandfillPlacementKind::ExpandDownLeft, false, true}, {LandfillPlacementKind::ExpandDownRight, false, false}};
    std::set<std::tuple<int, int, int, int>> done;      // a full-length strip is the same rectangle from both corners
    for (const auto& sd : sides) {
        const int span = sd.right ? tz2 - tz1 + 1 : tx2 - tx1 + 1;
        for (int depth = kLandfillMinZoneSize; depth <= kLandfillExpandMaxDepth; ++depth)
            for (int len = kLandfillMinZoneSize; len <= std::min(span, kLandfillMaxZoneSize); ++len) {
                if (depth * len > kLandfillPlacementMaxTiles) break;
                LandfillRect r;
                if (sd.right) { r.x1 = tx2 + 1; r.x2 = tx2 + depth; r.z1 = sd.fromStart ? tz1 : tz2 - len + 1; r.z2 = r.z1 + len - 1; }
                else { r.z1 = tz2 + 1; r.z2 = tz2 + depth; r.x1 = sd.fromStart ? tx1 : tx2 - len + 1; r.x2 = r.x1 + len - 1; }
                if (!done.insert({r.x1, r.z1, r.x2, r.z2}).second) continue;
                LandfillPlacement p;
                p.kind = sd.kind; p.zone = zone; p.rect = r; p.across = depth; p.along = len;
                for (int zz = r.z1; zz <= r.z2; ++zz) for (int x = r.x1; x <= r.x2; ++x) p.tiles.emplace_back(x, zz);
                p.n = static_cast<int>(p.tiles.size());
                for (const auto& [x, zz] : p.tiles) if (s.ZoneTypeAt(x, zz) != 0) { p.refusal = s.ZoneTypeAt(x, zz) < 0 ? "OUTSIDE_MAP" : "LAND_ZONED_OR_USED"; break; }
                if (p.refusal.empty() && (r.x1 < m.x1 || r.z1 < m.y1)) p.refusal = "BOX_WOULD_GROW_UP_OR_LEFT";
                if (p.refusal.empty() && TouchesOtherZone(s, zone, r)) p.refusal = "TOUCHES_ANOTHER_ZONE";
                if (p.refusal.empty()) {
                    LandfillZoneModel after = m;
                    const auto res = after.Add(p.tiles, z.ghosts);
                    Fill(p, z, after, res);
                    p.maintenanceDelta = kLandfillEmptyTileCost * res.tilesAdded - kLandfillEmptyTileCost * res.ghostsReconciled;
                    if (p.pushed > 0) p.refusal = "SENDS_GARBAGE_TO_THE_VALVE";
                }
                if (p.refusal.empty() || withRefused) out.push_back(std::move(p));
            }
    }
    return out;
}

// REDUCE candidates of one zone: k outermost rows / columns of its landfill tiles.
inline std::vector<LandfillPlacement> LandfillReduceCandidates(const LandfillSpatialSnapshot& s, int zone, bool withRefused = false) {
    using namespace landfill_spatial_detail;
    std::vector<LandfillPlacement> out;
    const auto& z = s.zones[static_cast<size_t>(zone)];
    const auto& m = z.model;
    if (z.landfillTiles.empty()) return out;
    const std::set<LandfillTile> typed(z.landfillTiles.begin(), z.landfillTiles.end());
    int tx1 = z.landfillTiles.front().first, tx2 = tx1, tz1 = z.landfillTiles.front().second, tz2 = tz1;
    for (const auto& [x, zz] : z.landfillTiles) { tx1 = std::min(tx1, x); tx2 = std::max(tx2, x); tz1 = std::min(tz1, zz); tz2 = std::max(tz2, zz); }
    for (const auto kind : {LandfillPlacementKind::ReduceTop, LandfillPlacementKind::ReduceBottom, LandfillPlacementKind::ReduceLeft, LandfillPlacementKind::ReduceRight}) {
        const bool rows = kind == LandfillPlacementKind::ReduceTop || kind == LandfillPlacementKind::ReduceBottom;
        for (int k = 1; k <= kLandfillReduceMaxRows; ++k) {
            LandfillRect r{tx1, tz1, tx2, tz2};
            if (kind == LandfillPlacementKind::ReduceTop) r.z2 = tz1 + k - 1;
            if (kind == LandfillPlacementKind::ReduceBottom) r.z1 = tz2 - k + 1;
            if (kind == LandfillPlacementKind::ReduceLeft) r.x2 = tx1 + k - 1;
            if (kind == LandfillPlacementKind::ReduceRight) r.x1 = tx2 - k + 1;
            if (r.W() <= 0 || r.H() <= 0) break;
            LandfillPlacement p;
            p.kind = kind; p.zone = zone; p.rect = r; p.across = k; p.along = rows ? r.W() : r.H();
            for (int zz = r.z1; zz <= r.z2; ++zz) for (int x = r.x1; x <= r.x2; ++x) p.tiles.emplace_back(x, zz);
            p.n = static_cast<int>(p.tiles.size());
            if (p.n > kLandfillPlacementMaxTiles) break;
            for (const auto& t : p.tiles) {
                if (!typed.count(t)) { p.refusal = "NOT_A_FULL_RECTANGLE_OF_LANDFILL"; break; }
                if (s.PileAt(t.first, t.second).pile) { p.refusal = "PILE_ON_A_TILE"; break; }
                for (const auto& [cx, cy] : m.CellsCovering(t.first, t.second))
                    if (m.Cell(cx, cy).stock != 0) { p.refusal = "STOCKED_CELL"; break; }
                if (!p.refusal.empty()) break;
            }
            if (p.refusal.empty()) {
                std::set<LandfillTile> rest = typed;
                for (const auto& t : p.tiles) rest.erase(t);
                if (rest.size() < 4 || !HasBlock2x2(rest)) p.refusal = "REMAINDER_TOO_SMALL";
                else if (!Connected(rest)) p.refusal = "WOULD_SPLIT_THE_ZONE";
            }
            if (p.refusal.empty()) {
                LandfillZoneModel after = m;
                const auto res = after.Remove(p.tiles);
                Fill(p, z, after, res);
                p.maintenanceDelta = -kLandfillEmptyTileCost * res.tilesRemoved;
                if (p.pushed > 0) p.refusal = "SENDS_GARBAGE_TO_THE_VALVE";
            }
            const std::string refusal = p.refusal;
            if (p.refusal.empty() || withRefused) out.push_back(std::move(p));
            if (refusal == "NOT_A_FULL_RECTANGLE_OF_LANDFILL" || refusal == "PILE_ON_A_TILE" || refusal == "WOULD_SPLIT_THE_ZONE" || refusal == "REMAINDER_TOO_SMALL") break;  // deeper is no better
        }
    }
    return out;
}

// The ghost repair of one zone: re-zone its pile-free landfill tiles (region box at most 48 x 48: the pile-free tiles inside the first such window from the top-left).
inline std::optional<LandfillPlacement> LandfillRepairPlacement(const LandfillSpatialSnapshot& s, int zone) {
    using namespace landfill_spatial_detail;
    const auto& z = s.zones[static_cast<size_t>(zone)];
    if (z.ghosts.empty()) return std::nullopt;
    LandfillPlacement p;
    p.kind = LandfillPlacementKind::Repair; p.zone = zone;
    std::vector<LandfillTile> free;
    for (const auto& t : z.landfillTiles) if (!s.PileAt(t.first, t.second).pile) free.push_back(t);
    std::sort(free.begin(), free.end(), [](const LandfillTile& a, const LandfillTile& b) { return a.second != b.second ? a.second < b.second : a.first < b.first; });
    if (!free.empty()) {
        int x1 = free.front().first, z1 = free.front().second;
        for (const auto& t : free) x1 = std::min(x1, t.first);
        for (const auto& t : free) if (t.first <= x1 + kLandfillMaxZoneSize - 1 && t.second <= z1 + kLandfillMaxZoneSize - 1) p.tiles.push_back(t);
    }
    if (p.tiles.empty()) { p.refusal = "NO_PILE_FREE_LANDFILL_TILE"; return p; }
    LandfillRect r{p.tiles.front().first, p.tiles.front().second, p.tiles.front().first, p.tiles.front().second};
    for (const auto& [x, zz] : p.tiles) { r.x1 = std::min(r.x1, x); r.x2 = std::max(r.x2, x); r.z1 = std::min(r.z1, zz); r.z2 = std::max(r.z2, zz); }
    // PlaceZone checks the minimum size on the region's BOX and only touches the cells whose bit is set (a 2 x 2 box with its pile cell cleared is accepted). A narrower set of
    // squares gets a 2 x 2 box with the extra cells cleared (RegionOf sets only `tiles`): no square is added to the re-zoning.
    if (r.W() < kLandfillMinZoneSize) { if (r.x2 + 1 < s.mapW) ++r.x2; else --r.x1; }
    if (r.H() < kLandfillMinZoneSize) { if (r.z2 + 1 < s.mapH) ++r.z2; else --r.z1; }
    p.rect = r;
    if (r.x1 < 0 || r.z1 < 0 || r.W() < kLandfillMinZoneSize || r.H() < kLandfillMinZoneSize) { p.refusal = "REGION_SMALLER_THAN_2X2"; return p; }
    if (TouchesOtherZone(s, zone, r)) { p.refusal = "TOUCHES_ANOTHER_ZONE"; return p; }
    LandfillZoneModel after = z.model;
    const auto res = after.Add(p.tiles, z.ghosts);
    Fill(p, z, after, res);
    p.maintenanceDelta = -kLandfillEmptyTileCost * res.ghostsReconciled;
    if (res.tilesAdded != 0) p.refusal = "REGION_ADDS_TILES";
    else if (p.pushed > 0) p.refusal = "SENDS_GARBAGE_TO_THE_VALVE";
    else if (p.capacityAfter < p.capacityBefore) p.refusal = "CAPACITY_WOULD_FALL";
    else if (p.roundingAmbiguous) p.refusal = "ROUNDING_AMBIGUOUS";
    return p;
}

// One placement per tile count n (the advisor's search domain), the best physical outcome for that n: largest capacity after, then the deterministic generation order.
inline std::vector<LandfillPlacement> LandfillShortList(std::vector<LandfillPlacement> all) {
    std::map<int, LandfillPlacement> best;
    for (auto& p : all) {
        if (!p.refusal.empty() || p.n <= 0 || p.n > kLandfillPlacementMaxTiles) continue;
        auto it = best.find(p.n);
        if (it == best.end() || p.capacityAfter > it->second.capacityAfter) best[p.n] = std::move(p);
    }
    std::vector<LandfillPlacement> out;
    for (auto& [n, p] : best) out.push_back(std::move(p));
    return out;
}

// The counterfactual input of a placement for the existing economic engine (PlanGarbageFunding through AdviseLandfillSize): API tile count, city capacity, usage, maintenance and the
// department ideal move by exactly what the model says; the calibrated geometry follows when it was calibrated AND agrees with the zone read; everything else is copied.
inline LandfillScenario LandfillPlacementScenario(const GarbageServiceInput& base, Opt<std::int64_t> idealMonthly, const LandfillSpatialSnapshot& s, const LandfillPlacement& p) {
    LandfillScenario sc;
    sc.delta = IsReduce(p.kind) ? -p.n : p.n;
    sc.input = base;
    const std::string problem = landfill_scenario_detail::BaseProblem(base, idealMonthly);
    if (!problem.empty()) { sc.reason = problem; return sc; }
    if (!p.refusal.empty()) { sc.reason = p.refusal; return sc; }
    if (p.pushed != 0) { sc.reason = "SENDS_GARBAGE_TO_THE_VALVE"; return sc; }
    auto& a = sc.input.aggregate;
    const std::int64_t tiles = *base.aggregate.landfillTiles;
    const std::int64_t capacity = static_cast<std::int64_t>(*base.aggregate.landfillCapacity) - p.capacityBefore + p.capacityAfter;
    const std::int64_t usage = static_cast<std::int64_t>(*base.aggregate.landfillUsage) - p.pushed;
    if (capacity <= 0 || capacity > 0xFFFFFFFFLL || usage < 0 || usage > capacity) { sc.reason = "CAPACITY_OUT_OF_RANGE"; return sc; }
    const std::int64_t newTiles = tiles + sc.delta;
    if (newTiles <= 0 || newTiles > kLandfillCurveMaxTiles) { sc.reason = "TILES_BEYOND_CURVE"; return sc; }
    const std::int64_t maint = *base.aggregate.landfillMaintenance + p.maintenanceDelta;
    if (maint < 0) { sc.reason = "MAINTENANCE_OUT_OF_RANGE"; return sc; }
    const auto& g = base.geometry;
    if (g.calibrated()) {
        if (s.zones.size() != 1 || g.activeCells != static_cast<std::uint32_t>(p.cellsBefore) || g.cellCapacity != static_cast<std::uint64_t>(p.cellCapBefore)) { sc.reason = "GEOMETRY_DISAGREES_WITH_THE_ZONE"; return sc; }
        sc.input.geometry.activeCells = static_cast<std::uint32_t>(p.cellsAfter);
        sc.input.geometry.cellCapacity = static_cast<std::uint64_t>(p.cellCapAfter);
        if (static_cast<std::int64_t>(p.cellsAfter) * p.cellCapAfter != capacity) { sc.reason = "GEOMETRY_INCONSISTENT"; return sc; }
    }
    a.landfillTiles = static_cast<std::int32_t>(newTiles);
    a.landfillCapacity = static_cast<std::uint32_t>(capacity);
    a.landfillUsage = static_cast<std::uint32_t>(usage);
    a.landfillMaintenance = maint;
    sc.idealMonthly = *idealMonthly + p.maintenanceDelta;
    sc.demonstrable = true;
    sc.reason = LandfillPlacementKindName(p.kind);
    return sc;
}

inline std::string LandfillPlacementLine(const LandfillPlacement& p) {
    std::string s = std::string(LandfillPlacementKindName(p.kind)) + " zone=" + std::to_string(p.zone) + " rect=" + std::to_string(p.rect.x1) + "," + std::to_string(p.rect.z1) + ".." +
                    std::to_string(p.rect.x2) + "," + std::to_string(p.rect.z2) + " n=" + std::to_string(p.n) + " T " + std::to_string(p.tilesBefore) + "->" + std::to_string(p.tilesAfter) +
                    " N " + std::to_string(p.cellsBefore) + "->" + std::to_string(p.cellsAfter) + " C " + std::to_string(p.cellCapBefore) + "->" + std::to_string(p.cellCapAfter) +
                    " capacity " + std::to_string(p.capacityBefore) + "->" + std::to_string(p.capacityAfter) + " pushed=" + std::to_string(p.pushed) + " ghosts=" + std::to_string(p.ghostsReconciled) +
                    " dB=" + std::to_string(p.maintenanceDelta) + (p.roundingAmbiguous ? " ROUNDING_AMBIGUOUS" : "");
    if (!p.refusal.empty()) s += " REFUSED(" + p.refusal + ")";
    return s;
}

}  // namespace sc4af
