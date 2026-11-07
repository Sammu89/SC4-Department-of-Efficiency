// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// The part of the landfill MONTHLY rollover that the executable proves. No game types, no policy.
//
// SimCity 4.exe 1.1.641.0, landfill developer, message SimNewMonth (0x66956816) -> 0x006BA0EC -> 0x006B91D0 -> 0x006B9239 -> per zone 0x006BD390:
//   * 0x006BA330  decay of ONE landfill record (32-byte record, stock at +0x18):  d = clamp(round(stock x 0.07), 1, 1500); stock -= min(stock, d)
//   * 0x006BD390  per zone: limit = tiles x 5000 (property 0x4A677E5C), every record is decayed, then, if the zone is operating, it absorbs
//                 min(free of the record, limit left, supply handed over) from the supply (AddToGarbageSupply(-take), AddToLandfilled(+take)).
// NOT modelled here (unresolved): how many records a zone has and their stock distribution, the operating condition of a zone
// (0x006BB8F0), the A/B scaling of the supply handed over (0x006B92A2), and the order of this handler relative to the PollutionSimulator rollover.
// So this is NOT a next-month predictor; it only pins the arithmetic that is proven.

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

namespace sc4af {

constexpr float kLandfillDecayRate = 0.07f;                 // property 0x4A280510 (raw, vanilla)
constexpr std::uint32_t kLandfillMaxDecayPerRecord = 1500;  // property 0x4A677E5E (raw, vanilla)
constexpr std::uint32_t kLandfillMaxIntakePerTile = 5000;   // property 0x4A677E5C (raw, vanilla): per tile of the zone, not per city

struct LandfillRecord {
    std::uint32_t capacity = 0;   // +0x14
    std::uint32_t stock = 0;      // +0x18
    bool active = true;           // +0x0C
};

// 0x006BA330. The product is formed in the x87 stack and stored through the 2^23 rounding constant (0x4B400000): round to nearest, ties to even.
inline std::uint32_t LandfillRecordDecay(std::uint32_t stock, float rate = kLandfillDecayRate, std::uint32_t maxDecay = kLandfillMaxDecayPerRecord) {
    if (stock == 0) return 0;
    std::int64_t d = static_cast<std::int64_t>(std::nearbyint(static_cast<long double>(stock) * static_cast<long double>(rate)));
    if (d < 1) d = 1;
    if (d > static_cast<std::int64_t>(maxDecay)) d = maxDecay;
    return static_cast<std::uint32_t>(std::min<std::int64_t>(d, stock));
}

struct LandfillZoneBoundary {
    std::uint64_t decayed = 0;
    std::uint64_t absorbed = 0;
};

// 0x006BD390 for one zone. `amount` is the supply still to hand over (reduced by what the zone absorbs).
inline LandfillZoneBoundary LandfillZoneMonthly(std::vector<LandfillRecord>& records, std::uint32_t tiles, std::uint32_t& amount, bool zoneOperating,
                                                std::uint32_t perTile = kLandfillMaxIntakePerTile) {
    LandfillZoneBoundary r;
    const std::uint64_t cap = static_cast<std::uint64_t>(tiles) * perTile;
    std::uint64_t left = std::min<std::uint64_t>(amount, cap);
    for (auto& rec : records) {
        if (!rec.active) continue;
        const std::uint32_t d = LandfillRecordDecay(rec.stock);
        rec.stock -= d;
        r.decayed += d;
        const std::uint32_t free = rec.capacity > rec.stock ? rec.capacity - rec.stock : 0;
        if (free == 0 || !zoneOperating || left == 0) continue;
        const std::uint32_t take = static_cast<std::uint32_t>(std::min<std::uint64_t>(free, left));
        rec.stock += take;
        amount -= take;
        left -= take;
        r.absorbed += take;
    }
    return r;
}

// Aggregate of a set of records (what the research probe logs at every boundary point). expectedDecay = what the monthly decay would remove from the CURRENT stocks.
struct LandfillRecordStats {
    std::uint64_t count = 0, withStock = 0, sumStock = 0, sumFree = 0, sumCapacity = 0, maxStock = 0, minNonZeroStock = 0, sumExpectedDecay = 0;
};

inline LandfillRecordStats ComputeLandfillRecordStats(const std::vector<LandfillRecord>& records) {
    LandfillRecordStats s;
    for (const auto& r : records) {
        ++s.count;
        s.sumStock += r.stock;
        s.sumCapacity += r.capacity;
        s.sumFree += r.capacity > r.stock ? r.capacity - r.stock : 0;
        if (r.stock > 0) {
            ++s.withStock;
            s.maxStock = std::max<std::uint64_t>(s.maxStock, r.stock);
            s.minNonZeroStock = s.minNonZeroStock == 0 ? r.stock : std::min<std::uint64_t>(s.minNonZeroStock, r.stock);
            if (r.active) s.sumExpectedDecay += LandfillRecordDecay(r.stock);
        }
    }
    return s;
}

// ---- the whole month boundary of one landfill zone, as measured on consecutive boundaries ------------------------------------
// Inputs are values the game exposes before the boundary. The records are the CELLS of the landfill grid (an active cell holds `capacityTotal / activeCells` raw units,
// the stock is kept packed first-fit by the update, so the stock distribution is a function of the total usage).
//   D(U)      = floor(U / C) x decay(C) + decay(U mod C),   C = floor(capacityTotal / activeCells)            [exact on every observed boundary]
//   handed    = min(supply, totalMonthlyMax); if A < B: floor(handed x A / B)     (A / B = the landfill efficiency the API reports, a stat ratio, NOT the funding curve)
//   absorbed  = min(handed, tiles x perTile, capacityTotal - (U - D(U)))
//   usageNext = U - D(U) + absorbed;  freeNext = capacityTotal - usageNext;  supplyNext = supply - absorbed
// Single landfill zone only; a city with several zones is NOT covered. Not a model of the daily pipeline: `supply` is the supply AT the boundary.
struct LandfillBoundaryState {
    std::uint64_t usage = 0;              // landfillUsage before the boundary
    std::uint64_t capacityTotal = 0;      // landfillCapacity
    std::uint32_t activeCells = 0;        // active records of the zone (e.g. 182)
    std::uint64_t zoneTiles = 0;          // tile count of the zone (e.g. 240)
    std::int64_t supply = 0;              // GarbageSupply at the boundary
    std::uint64_t effNumerator = 1, effDenominator = 1;   // A / B; A >= B means no scaling
    std::uint64_t totalMonthlyMax = 327680000;            // property 0x4A677E5B
    std::uint32_t perTile = kLandfillMaxIntakePerTile;    // property 0x4A677E5C
};

struct LandfillBoundaryResult {
    bool computable = false;
    std::uint64_t decayed = 0, handedOver = 0, absorbed = 0, usageAfter = 0, freeAfter = 0;
    std::int64_t supplyAfter = 0;
};

inline LandfillBoundaryResult LandfillMonthBoundary(const LandfillBoundaryState& in) {
    LandfillBoundaryResult r;
    r.supplyAfter = in.supply;
    r.usageAfter = in.usage;
    r.freeAfter = in.capacityTotal > in.usage ? in.capacityTotal - in.usage : 0;
    if (in.activeCells == 0 || in.capacityTotal == 0) { r.computable = in.activeCells == 0 && in.capacityTotal == 0; return r; }   // no landfill: the update does nothing
    if (in.effDenominator == 0) return r;
    const std::uint64_t cell = in.capacityTotal / in.activeCells;
    if (cell == 0) return r;
    const std::uint64_t usage = std::min(in.usage, in.capacityTotal);
    r.decayed = (usage / cell) * LandfillRecordDecay(static_cast<std::uint32_t>(cell)) + LandfillRecordDecay(static_cast<std::uint32_t>(usage % cell));
    const std::uint64_t afterDecay = usage - r.decayed;
    std::uint64_t handed = static_cast<std::uint64_t>(std::max<std::int64_t>(0, in.supply));
    handed = std::min(handed, in.totalMonthlyMax);
    if (in.effNumerator < in.effDenominator) handed = handed * in.effNumerator / in.effDenominator;
    r.handedOver = handed;
    r.absorbed = std::min({handed, static_cast<std::uint64_t>(in.zoneTiles) * in.perTile, in.capacityTotal - afterDecay});
    r.usageAfter = afterDecay + r.absorbed;
    r.freeAfter = in.capacityTotal - r.usageAfter;
    r.supplyAfter = in.supply - static_cast<std::int64_t>(r.absorbed);
    r.computable = true;
    return r;
}

} // namespace sc4af
