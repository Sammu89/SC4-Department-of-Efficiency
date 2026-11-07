// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// Game access of the spatial landfill analysis: see LandfillZoneReader.h (the technical and safety evaluation of the read is documented there).
#include "LandfillZoneReader.h"

#include "GameVersion.h"
#include "SC4CellRegion.h"
#include "cISC4City.h"
#include "cISC4LandfillDeveloper.h"
#include "cISC4Occupant.h"
#include "cISC4OccupantManager.h"
#include "cISC4ZoneManager.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cIGZVariant.h"

#include <windows.h>

#include <chrono>
#include <cstdint>
#include <map>
#include <set>
#include <vector>

namespace sc4af {
namespace {

constexpr std::uintptr_t kLandfillZoneVtable = 0x00AB1610;      // cSC4LandfillZone (constructor 0x6BBD60)
constexpr std::uintptr_t kLandfillCellVtable = 0x00AB15DC;      // its 32-byte cell records
constexpr std::uint32_t kPileOccupantType = 0x6A11BD2Bu;         // the garbage pile occupant the maintenance routine 0x6BA3C0 asks for
constexpr std::uint32_t kPileCostProperty = 0xEA54D286u;
constexpr int kMaxBox = 1024;                                     // a city is at most 256 x 256 cells; anything larger is not a zone
constexpr int kMaxRecords = 256 * 256;

bool SupportedVersion() {
    static const int ok = [] { const auto v = ReadExecutableVersion(); return v && IsSupportedGameVersion(*v) ? 1 : 0; }();
    return ok == 1;
}

struct RawRecord { std::uintptr_t vptr; std::int32_t x, y, flags, capacity, stock; };
struct RawZone {
    std::uintptr_t vptr = 0;
    std::int32_t x1 = 0, y1 = 0, x2 = -1, y2 = -1, w = 0, h = 0;
    std::int32_t records = 0;
};

// Raw read of one zone object: box, bitmap (bits[(y - y1) * (x2 - x1 + 1) + (x - x1)]) and records. false = a fault or an impossible shape.
bool GuardedReadZone(std::uintptr_t zone, RawZone* z, std::uint8_t* bits, int maxBits, RawRecord* recs, int maxRecs) {
    __try {
        const auto* p = reinterpret_cast<const unsigned char*>(zone);
        z->vptr = *reinterpret_cast<const std::uintptr_t*>(p);
        if (z->vptr != kLandfillZoneVtable) return false;
        const auto* r = *reinterpret_cast<const unsigned char* const*>(p + 0x0C);
        if (!r) return false;
        z->x1 = *reinterpret_cast<const std::int32_t*>(r + 0); z->y1 = *reinterpret_cast<const std::int32_t*>(r + 4);
        z->x2 = *reinterpret_cast<const std::int32_t*>(r + 8); z->y2 = *reinterpret_cast<const std::int32_t*>(r + 12);
        z->w = *reinterpret_cast<const std::int32_t*>(r + 0x14); z->h = *reinterpret_cast<const std::int32_t*>(r + 0x18);
        const int bw = z->x2 - z->x1 + 1, bh = z->y2 - z->y1 + 1;
        if (bw < 1 || bh < 1 || bw > kMaxBox || bh > kMaxBox || z->w != bw || z->h != bh || bw * bh > maxBits) return false;
        const auto* const* rows = *reinterpret_cast<const std::uint32_t* const* const*>(r + 0x20);
        if (!rows) return false;
        for (int x = 0; x < bw; ++x) {
            const std::uint32_t* row = rows[x];                    // as 0x6BAB30: row index = x - x1, bit = y - y1
            if (!row) return false;
            for (int y = 0; y < bh; ++y) bits[y * bw + x] = (row[y >> 5] & (1u << (y & 31))) ? 1 : 0;
        }
        const auto* b = *reinterpret_cast<const unsigned char* const*>(p + 0x1C);
        const auto* e = *reinterpret_cast<const unsigned char* const*>(p + 0x20);
        if (!b || !e || e < b || (e - b) % 32 != 0 || (e - b) / 32 > maxRecs) return false;
        int k = 0;
        for (const unsigned char* q = b; q < e; q += 32, ++k) {
            recs[k].vptr = *reinterpret_cast<const std::uintptr_t*>(q);
            recs[k].x = *reinterpret_cast<const std::int32_t*>(q + 4); recs[k].y = *reinterpret_cast<const std::int32_t*>(q + 8);
            recs[k].flags = *reinterpret_cast<const std::int32_t*>(q + 0x0C);
            recs[k].capacity = *reinterpret_cast<const std::int32_t*>(q + 0x14); recs[k].stock = *reinterpret_cast<const std::int32_t*>(q + 0x18);
        }
        z->records = k;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

bool GuardedApi(cISC4LandfillDeveloper* dev, std::uint32_t& usage, std::uint32_t& capacity, std::int32_t& tiles, std::int64_t& maintenance, bool& ready) {
    __try {
        dev->GetLandfillUsage(usage, capacity);
        tiles = dev->GetLandfillTileCount();
        ready = *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(dev) + 0x2C) != nullptr;     // GetMaintenanceCosts needs it (UtilityProbe.cpp)
        maintenance = ready ? dev->GetMaintenanceCosts() : 0;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

bool GuardedZoneTypes(cISC4ZoneManager* zm, int nx, int nz, std::int8_t* out) {
    __try {
        for (int z = 0; z < nz; ++z)
            for (int x = 0; x < nx; ++x) out[static_cast<size_t>(z) * nx + x] = static_cast<std::int8_t>(zm->GetZoneType(x, z));
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

bool GuardedZoneFromCell(cISC4LandfillDeveloper* dev, int x, int z, std::uintptr_t& out) {
    __try { out = static_cast<std::uintptr_t>(dev->GetLandfillZoneFromCell(x, z)); return true; } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

// The pile on a cell and its monthly cost (property 0xEA54D286, as 0x6BA3C0 reads it).
LandfillPileInfo PileOf(cISC4OccupantManager* om, int x, int z) {
    LandfillPileInfo info;
    cISC4Occupant* occ = nullptr;
    if (om->GetFirstOccupantByStandardCityCell(occ, x, z, kPileOccupantType) && occ) {
        info.pile = true;
        if (auto* holder = occ->AsPropertyHolder())
            if (cISCProperty* prop = holder->GetProperty(kPileCostProperty))
                if (cIGZVariant* val = prop->GetPropertyValue()) {
                    const uint16_t t = val->GetType();
                    if (t == cIGZVariant::Sint64Array && val->RefSint64() && val->GetCount() > 0) info.cost = val->RefSint64()[0];
                    else if (t == cIGZVariant::Sint64) info.cost = val->GetValSint64();
                    else if (t == cIGZVariant::Uint32) info.cost = val->GetValUint32();
                    else if (t == cIGZVariant::Sint32) info.cost = val->GetValSint32();
                    else info.cost = -1;                                  // unknown shape: the maintenance cross-check will refuse the snapshot
                }
        occ->Release();
    }
    return info;
}

bool GuardedPlaceZone(cISC4ZoneManager* zm, SC4CellRegion<int32_t>& region, int type, bool place, bool slope, bool lot, std::int64_t& cost, std::int32_t& err, bool& raised) {
    __try {
        return zm->PlaceZone(region, static_cast<cISC4ZoneManager::ZoneType>(type), place, true, slope, false, lot, &cost, &err, 0);
    } __except (EXCEPTION_EXECUTE_HANDLER) { raised = true; return false; }
}

int RegionBits(const SC4CellRegion<int32_t>& r) {
    int n = 0;
    for (int x = r.bounds.topLeftX; x <= r.bounds.bottomRightX; ++x)
        for (int z = r.bounds.topLeftY; z <= r.bounds.bottomRightY; ++z) n += r.cellMap.GetValue(static_cast<uint32_t>(x - r.bounds.topLeftX), static_cast<uint32_t>(z - r.bounds.topLeftY)) ? 1 : 0;
    return n;
}

SC4CellRegion<int32_t> RegionOf(const LandfillPlacement& p) {
    SC4CellRegion<int32_t> region(p.rect.x1, p.rect.z1, p.rect.x2, p.rect.z2, false);
    for (const auto& [x, z] : p.tiles) region.cellMap.SetValue(static_cast<uint32_t>(x - p.rect.x1), static_cast<uint32_t>(z - p.rect.z1), true);
    return region;
}

// The zoning parameters of a placement: EXPAND faces the player's checks (slope, lots); REDUCE is a dezone; REPAIR uses exactly the arguments validated in game.
void ParamsOf(const LandfillPlacement& p, int& type, bool& slope, bool& lot) {
    if (IsReduce(p.kind)) { type = 0; slope = false; lot = false; }
    else if (IsExpand(p.kind)) { type = kLandfillZoneTypeLandfill; slope = true; lot = true; }
    else { type = kLandfillZoneTypeLandfill; slope = false; lot = false; }
}

}  // namespace

LandfillSpatialSnapshot ReadLandfillSpatial(cISC4City* city) {
    const auto t0 = std::chrono::steady_clock::now();
    LandfillSpatialSnapshot s;
    const auto fail = [&](const char* why) { s.read = false; s.problem = why; s.zones.clear(); s.readMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count(); return s; };
    if (!SupportedVersion()) return fail("GAME_VERSION_NOT_SUPPORTED");
    if (!city) return fail("NO_CITY");
    auto* dev = city->GetLandfillDeveloper();
    auto* zm = city->GetZoneManager();
    auto* om = city->GetOccupantManager();
    if (!dev || !zm || !om) return fail("NO_DEVELOPER");
    std::uint32_t usage = 0, capacity = 0;
    std::int32_t tiles = 0;
    std::int64_t maintenance = 0;
    bool ready = false;
    if (!GuardedApi(dev, usage, capacity, tiles, maintenance, ready)) return fail("API_RAISED");
    if (!ready) return fail("MAINTENANCE_NOT_READY");
    s.apiTiles = tiles; s.apiCapacity = capacity; s.apiUsage = usage; s.apiMaintenance = maintenance;
    s.mapW = static_cast<int>(city->SizeX() / 16.0f);
    s.mapH = static_cast<int>(city->SizeZ() / 16.0f);
    if (s.mapW <= 0 || s.mapH <= 0 || s.mapW > kMaxBox || s.mapH > kMaxBox) return fail("MAP_SIZE");
    s.zoneType.assign(static_cast<size_t>(s.mapW) * s.mapH, 0);
    if (!GuardedZoneTypes(zm, s.mapW, s.mapH, s.zoneType.data())) return fail("ZONE_TYPES_RAISED");
    std::vector<LandfillTile> typed;
    for (int z = 0; z < s.mapH; ++z)
        for (int x = 0; x < s.mapW; ++x)
            if (s.zoneType[static_cast<size_t>(z) * s.mapW + x] == kLandfillZoneTypeLandfill) typed.emplace_back(x, z);
    std::vector<std::uintptr_t> ids;
    for (const auto& [x, z] : typed) {
        std::uintptr_t id = 0;
        if (!GuardedZoneFromCell(dev, x, z, id)) return fail("ZONE_LOOKUP_RAISED");
        if (id && std::find(ids.begin(), ids.end(), id) == ids.end()) ids.push_back(id);
    }
    static std::vector<std::uint8_t> bits(static_cast<size_t>(kMaxBox) * kMaxBox);
    static std::vector<RawRecord> recs(kMaxRecords);
    std::set<LandfillTile> inZones;
    for (const auto id : ids) {
        RawZone raw;
        if (!GuardedReadZone(id, &raw, bits.data(), static_cast<int>(bits.size()), recs.data(), static_cast<int>(recs.size()))) return fail("ZONE_UNREADABLE");
        const int bw = raw.x2 - raw.x1 + 1, bh = raw.y2 - raw.y1 + 1;
        if (raw.records != (bw - 1) * (bh - 1)) return fail("ZONE_RECORD_COUNT");
        LandfillZoneView v;
        v.id = id;
        v.model = LandfillZoneModel(raw.x1, raw.y1, raw.x2, raw.y2);
        std::vector<std::uint8_t> seen(static_cast<size_t>((bw - 1) * (bh - 1)), 0);
        for (int k = 0; k < raw.records; ++k) {
            const auto& r = recs[k];
            if (r.vptr != kLandfillCellVtable || r.x < 0 || r.y < 0 || r.x >= bw - 1 || r.y >= bh - 1 || r.capacity < 0 || r.stock < 0) return fail("ZONE_RECORD");
            auto& flag = seen[static_cast<size_t>(r.y) * (bw - 1) + r.x];
            if (flag) return fail("ZONE_RECORD_TWICE");
            flag = 1;
            auto& c = v.model.Cell(r.x, r.y);
            c.active = (r.flags & 0xFF) != 0;
            c.capacity = r.capacity;
            c.stock = r.stock;
        }
        for (int y = 0; y < bh; ++y)
            for (int x = 0; x < bw; ++x) {
                if (!bits[static_cast<size_t>(y) * bw + x]) continue;
                const int cx = raw.x1 + x, cz = raw.y1 + y;
                if (cx < 0 || cz < 0 || cx >= s.mapW || cz >= s.mapH) return fail("ZONE_OUTSIDE_MAP");
                v.model.Set(cx, cz, true);
                inZones.insert({cx, cz});
                if (s.ZoneTypeAt(cx, cz) == kLandfillZoneTypeLandfill) v.landfillTiles.emplace_back(cx, cz);
                else v.ghosts.insert({cx, cz});
            }
        s.zones.push_back(std::move(v));
    }
    for (const auto& t : typed) if (!inZones.count(t)) ++s.orphanLandfillTiles;
    for (const auto& t : inZones) s.piles[t] = PileOf(om, t.first, t.second);
    for (const auto& t : typed) if (!s.piles.count(t)) s.piles[t] = PileOf(om, t.first, t.second);
    s.read = true;
    s.readMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count();
    return s;
}

int LandfillNativeCheck(cISC4City* city, const LandfillPlacement& p) {
    if (!SupportedVersion() || !city || p.tiles.empty()) return -1;
    auto* zm = city->GetZoneManager();
    if (!zm) return -1;
    int type = 0; bool slope = false, lot = false;
    ParamsOf(p, type, slope, lot);
    SC4CellRegion<int32_t> region = RegionOf(p);
    const int before = RegionBits(region);
    std::int64_t cost = 0;
    std::int32_t err = -1;
    bool raised = false;
    const bool ok = GuardedPlaceZone(zm, region, type, false, slope, lot, cost, err, raised);
    if (raised) return -1;
    if (!ok) return err > 0 ? err : -1;
    if (RegionBits(region) != before) return 100;         // accepted, but the game dropped squares (slope / water): the player could not zone the whole area
    return 0;
}

bool LandfillRezoneRegion(cISC4City* city, const LandfillPlacement& p, std::string& problem) {
    if (p.kind != LandfillPlacementKind::Repair) { problem = "NOT_A_REPAIR"; return false; }
    if (!SupportedVersion() || !city || p.tiles.empty()) { problem = "UNAVAILABLE"; return false; }
    auto* zm = city->GetZoneManager();
    if (!zm) { problem = "NO_ZONE_MANAGER"; return false; }
    int type = 0; bool slope = false, lot = false;
    ParamsOf(p, type, slope, lot);
    SC4CellRegion<int32_t> region = RegionOf(p);
    const int before = RegionBits(region);
    std::int64_t cost = 0;
    std::int32_t err = -1;
    bool raised = false;
    const bool ok = GuardedPlaceZone(zm, region, type, true, slope, lot, cost, err, raised);
    if (raised) { problem = "RAISED"; return false; }
    if (!ok) { problem = "REFUSED_" + std::to_string(err); return false; }
    if (RegionBits(region) != before) { problem = "SQUARES_DROPPED"; return false; }
    return true;
}

}  // namespace sc4af
