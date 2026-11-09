// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Readiness of the readings. The Department acts as soon as the readings needed for a safe decision exist, are current and agree with each other,
// commodity by commodity (a service that is not ready never holds back one that is). There is no fixed warm-up after a city load.
//
// Pure (no game types). A reading the game has not given is unknown and is never read as zero; a count the game reports as 0 until the simulation has run is
// "not populated", not a real zero (UtilityModel.h ServiceCountsReady). Nothing here is a timer: the verdict depends on the readings only, so a ready city
// acts on its first day and an unready one waits as long as the data does.
//
// Two levels:
//   * the full snapshot (PowerReadiness, WaterReadiness, GarbageReadiness, DealsReadiness): the authoritative verdict a plan and a write rest on;
//   * the light counters (PowerCountersPrecheck, WaterCountersPrecheck): a cheap pre-check that only saves a full plan on a day when the counters already prove the
//     commodity is not ready. It never says "ready" on its own: unknown counters pass through to the full verdict.
//
// Not covered here: the physical calibration of components that need it (the landfill geometry is learnt from the month boundaries: until it is calibrated the
// Garbage plan runs in its conservative continuity-lock mode and invents no capacity, see GarbageControl.h), the setters' own compare-and-set, the fresh re-read
// before every write and the read-back after it.

#include "ServiceMonitor.h"
#include "UtilityModel.h"

#include <algorithm>
#include <cstdint>
#include <string>

namespace sc4af {

struct Readiness {
    bool ready = false;
    bool standby = false;        // NOT an error: there is nothing to manage in this city (yet); the Department waits and re-evaluates every cycle
    std::string reason;          // why it is not ready (empty when ready), or what the standby is waiting for
    explicit operator bool() const { return ready; }
};

inline Readiness ReadyNow() { Readiness r; r.ready = true; return r; }
inline Readiness NotReady(std::string why) { Readiness r; r.reason = std::move(why); return r; }
inline Readiness Standby(std::string why) { Readiness r; r.standby = true; r.reason = std::move(why); return r; }

// The game's monthly counters carry rounding: a service delivered a few units above the demand is not a contradiction, a service delivered well above it is.
constexpr double kConsistencyTolerance = 0.01;
inline bool ConsumptionContradictsDemand(std::int64_t demand, std::int64_t consumed) {
    return static_cast<double>(consumed) > static_cast<double>(demand) * (1.0 + kConsistencyTolerance) + 1.0;
}

// Power: the game's monthly consumption counter INCLUDES what the city exports (measured: consumption 15 303 = demand 9 559 + export 5 744, and 26 659 = 15 394 + 11 265), so it is only comparable with the
// city's own demand once the export is taken off: net consumption = consumption - export (same simulator, same monthly accounting period, read together). An export that was not read is never taken as 0:
// the unexplained excess then stays a contradiction (fail closed). The 1% tolerance is the same. Water has no such rule: its consumed counter already excludes the exports.
inline bool PowerConsumptionContradictsDemand(std::int64_t demand, std::int64_t consumption, bool exportKnown, std::int64_t exported) {
    const std::int64_t net = exportKnown ? consumption - std::max<std::int64_t>(0, exported) : consumption;
    return ConsumptionContradictsDemand(demand, net);
}

inline Readiness PowerReadiness(const UtilitySnapshot& s) {
    const auto& p = s.power;
    // A city with no plant at all (VALID_EMPTY) has nothing to fund but is still served by purchases: its demand decides, so it is ready when the demand is read.
    const bool noPlants = p.validity == SnapshotValidity::ValidEmpty;
    if (!noPlants && p.validity != SnapshotValidity::Valid && p.validity != SnapshotValidity::Partial) return NotReady("power snapshot not valid (" + p.invalidReason + ")");
    if (!p.aggregate.demand) return NotReady("power demand not read");
    if (*p.aggregate.demand <= 0) return NotReady("power demand reads 0: the counters are not populated yet (an unknown reading is never taken as zero)");
    bool plantRead = noPlants;
    for (const auto& plant : p.plants) plantRead = plantRead || (plant.funding && plant.maxProduced);
    if (!plantRead) return NotReady("no power plant with a readable funding and output");
    if (p.aggregate.consumption && PowerConsumptionContradictsDemand(*p.aggregate.demand, *p.aggregate.consumption, p.aggregate.export_.has_value(), p.aggregate.export_.value_or(0)))
        return NotReady("contradictory power readings: the service delivered (" + std::to_string(*p.aggregate.consumption) + ") exceeds the demand (" + std::to_string(*p.aggregate.demand) + ")");
    return ReadyNow();
}

inline Readiness WaterReadiness(const UtilitySnapshot& s) {
    const auto& w = s.water;
    if (w.validity == SnapshotValidity::ValidEmpty) return NotReady("no water supply to manage");
    if (w.validity != SnapshotValidity::Valid && w.validity != SnapshotValidity::Partial) return NotReady("water snapshot not valid (" + w.invalidReason + ")");
    if (!w.department.present || !w.department.funding) return NotReady("water department or its funding not read");
    if (!w.aggregate.demand || !w.aggregate.productionCapacity) return NotReady("water demand or production capacity not read");
    if (*w.aggregate.demand <= 0) return NotReady("water demand reads 0: the counters are not populated yet");
    if (!ServiceCountsReady(w.aggregate)) return NotReady("watered / unwatered building counts are not populated yet (the game reads 0 and 0 until the simulation has run)");
    if (w.aggregate.consumed && ConsumptionContradictsDemand(*w.aggregate.demand, *w.aggregate.consumed))
        return NotReady("contradictory water readings: the water consumed (" + std::to_string(*w.aggregate.consumed) + ") exceeds the demand (" + std::to_string(*w.aggregate.demand) + ")");
    return ReadyNow();
}

// The LOCAL sanitation (the funding of the city's own garbage structures). A city WITHOUT any of them (no landfill, incinerator, recycling center or waste-to-energy plant: a PROVEN absence, the
// building list was read and is empty, ValidEmpty) has nothing to fund: STANDBY, not an error; the structures that appear later are picked up by the next snapshot. Only a reading that is
// missing, inconsistent or refused (Partial / Invalid) stops the local sanitation.
inline Readiness GarbageReadiness(const UtilitySnapshot& s) {
    const auto& g = s.garbage;
    if (g.validity == SnapshotValidity::ValidEmpty) return Standby("no garbage structure to manage (no landfill, incinerator, recycling center or waste-to-energy plant); waiting for one");
    if (g.validity != SnapshotValidity::Valid) return NotReady("garbage snapshot not valid (" + g.invalidReason + ")");
    if (!g.department.present) return NotReady("sanitation department not read");
    if (!g.aggregate.produced) return NotReady("garbage produced last month not read");
    if (!g.aggregate.fundingEfficiency) return NotReady("sanitation funding efficiency not read");
    return ReadyNow();
}

// The sanitation BUDGET of a city with no garbage structure (STANDBY). It is NOT the local readiness (nothing is managed, no structure becomes "ready"): it only says that the department figures a
// funding cut rests on are read, so that a cost the city pays for nothing can be removed. The absence is the proven one (ValidEmpty); the funding, its ideal cost and the funding efficiency must be read.
inline Readiness GarbageBudgetReadiness(const UtilitySnapshot& s) {
    const auto& g = s.garbage;
    if (g.validity != SnapshotValidity::ValidEmpty) return NotReady("the absence of garbage structures is not proven");
    if (!g.department.present || !g.department.funding) return NotReady("sanitation department or its funding not read");
    if (!g.department.idealMonthly) return NotReady("sanitation ideal monthly cost not read");
    if (!g.aggregate.fundingEfficiency) return NotReady("sanitation funding efficiency not read");
    return ReadyNow();
}

// The garbage CONTRACTS (regional export / import). They need the city's garbage load (produced, uncollected) read from a snapshot that is not refused, and the contract book (DealsReadiness);
// they do NOT need a landfill, a local structure or the local funding readings: a city with no garbage structure at all can still export, and a missing local funding reading does not stop
// them (the portfolio planner prices what it can read and plans nothing it cannot).
inline Readiness GarbageContractsReadiness(const UtilitySnapshot& s) {
    const auto& g = s.garbage;
    if (g.validity != SnapshotValidity::Valid && g.validity != SnapshotValidity::ValidEmpty) return NotReady("garbage snapshot not valid (" + g.invalidReason + ")");
    if (!g.aggregate.produced || !g.aggregate.uncollected) return NotReady("garbage produced / uncollected not read");
    return ReadyNow();
}

// The neighbor-deal book: the existing contracts must have been read. The proposals and the markets are requested only when the deal controller may act
// (DealDiscoveryAllowed): their absence is not an error here, the portfolio planner reports it (MARKETS_NOT_READ_YET) and plans nothing it cannot price.
inline Readiness DealsReadiness(const UtilitySnapshot& s) {
    if (s.deals.validity != SnapshotValidity::Valid && s.deals.validity != SnapshotValidity::Partial && s.deals.validity != SnapshotValidity::ValidEmpty)
        return NotReady("neighbor-deal contracts not read");
    return ReadyNow();
}

// ---- the light pre-check ------------------------------------------------------------------------------------------------------------------------------
// true = the counters prove the commodity is not ready (do not spend a full plan on it today). false = ready or unknown: the full verdict decides.
inline bool PowerCountersShowNotReady(const ServiceCounters& c) {
    if (!c.power) return false;
    if (c.pDemand && *c.pDemand <= 0) return true;
    if (c.pDemand && c.pConsumption && PowerConsumptionContradictsDemand(*c.pDemand, *c.pConsumption, c.pExport.has_value(), c.pExport.value_or(0))) return true;
    return false;
}
inline bool WaterCountersShowNotReady(const ServiceCounters& c) {
    if (!c.water) return false;
    if (c.wDemand && *c.wDemand <= 0) return true;
    if (c.wWatered && c.wUnwatered && *c.wWatered == 0 && *c.wUnwatered == 0) return true;
    if (c.wDemand && c.wConsumed && ConsumptionContradictsDemand(*c.wDemand, *c.wConsumed)) return true;
    return false;
}

} // namespace sc4af
