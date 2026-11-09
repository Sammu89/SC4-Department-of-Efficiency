// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Fast service check for Power and Water (pure, no game types).
//
// The full optimiser (snapshot enumeration, quotes, portfolio search, logging) is far too heavy to run on every SimNewDay just to learn that nothing changed, and Power / Water otherwise react
// only on their cadence (Power every PowerCadenceDays days, Water monthly). This monitor reads a handful of authoritative counters, decides whether the city is REALLY short of a service or
// something material changed, and only then asks the controller for an immediate replan of that commodity.
//
// Rules:
//  * the primary evidence is the game's own counters, never the number of unserved buildings (that can be an internal connection problem):
//        Power: demand (the real need), consumption (the service delivered), the effective local capacity and the usable imports;
//        Water: demand, consumed, the production capacity and the usable imports.
//  * a REGIONAL deficit is confirmed only when the service is short (consumption < demand) AND the supply that exists (local capacity + usable imports) cannot cover the demand. Consumption
//    below demand with a supply that covers it is an internal problem of the city: it is reported, it never triggers a replan.
//  * the monthly counters change when the game updates them, not every day. A sample identical to the previous one is NOT a new observation: it never repeats a decision.
//  * a material change is any change of the local capacity, of the contracted flows (imports / exports) or of the demand beyond kDemandStepFraction since the commodity was last evaluated.
//  * no cooldown: every fresh observation that qualifies triggers, once.

#include "UtilityModel.h"

#include <algorithm>
#include <cmath>
#include <optional>
#include <cstdint>
#include <string>

namespace sc4af {

struct ServiceCounters {
    bool power = false, water = false;                       // which halves were read
    Opt<std::int64_t> pDemand, pRequirement, pConsumption, pProduction, pImport, pExport, pCapacity;
    Opt<std::int64_t> pGameImported, pGameExported;          // RegionalCity flows (immediate): the contracts as the game accounts them
    Opt<std::int64_t> wDemand, wConsumed, wCapacity, wImport, wExport;
    Opt<std::int64_t> wWatered, wUnwatered;                   // both read 0 until the simulation has run: not a real reading (UtilityModel.h ServiceCountsReady)
    Opt<std::int64_t> wGameImported, wGameExported;
    // Garbage: the handful of pollution-simulator counters that decide whether the garbage plan can have changed (no building enumeration). Absent (garbage = false) means the controller keeps
    // evaluating Garbage every day.
    bool garbage = false;
    Opt<std::int64_t> gProduced, gUncollected, gW2e, gIncinerator, gEfficiency, gGameImported, gGameExported;
    std::uint32_t epoch = 0;
};

constexpr double kDemandStepFraction = 0.01;                 // a demand move of 1% of the last evaluated value is material (a performance filter: the replan itself is exact)

enum class ServiceCause { None, RegionalDeficit, InternalShortfall, MaterialChange, Stale };

inline const char* ServiceCauseName(ServiceCause c) {
    switch (c) {
    case ServiceCause::None: return "NONE";
    case ServiceCause::RegionalDeficit: return "REGIONAL_DEFICIT";
    case ServiceCause::InternalShortfall: return "INTERNAL_SHORTFALL_NOT_REGIONAL";
    case ServiceCause::MaterialChange: return "MATERIAL_CHANGE";
    case ServiceCause::Stale: return "STALE_COUNTERS";
    }
    return "?";
}

struct ServiceVerdict {
    bool trigger = false;
    ServiceCause cause = ServiceCause::None;
    std::string text;
};

// What a purchase written earlier has to show on the first AUTHORITATIVE update that follows it.
enum class ProofStatus { Pending, Restored, StillShort };
struct ServiceProof {
    ProofStatus status = ProofStatus::Pending;
    std::string cause;       // when StillShort: why
    std::string text;
};

class ServiceMonitor {
public:
    void Reset() { *this = ServiceMonitor{}; }

    // The service the city really received, comparable with ITS OWN demand. Power: the game's monthly consumption counter INCLUDES the exports (consumption = demand + export when the city is served), so
    // it is only the city's own consumption once the export is taken off (same simulator, same monthly counters). Without the export reading nothing is claimed (nullopt: the callers treat the sample as
    // unreadable): a sale could otherwise make a short city look served. Water: the consumed counter already excludes the exports.
    static Opt<std::int64_t> ServedOf(const ServiceCounters& c, bool power) {
        if (!power) return c.wConsumed;
        if (!c.pConsumption || !c.pExport) return std::nullopt;
        return *c.pConsumption - std::max<std::int64_t>(0, *c.pExport);
    }

    // Pure: the service state of a sample.
    // `exports`: what the city has sold. A sale eats the supply the city needs for itself, so it is subtracted from what the supply can cover.
    static ServiceVerdict Judge(const std::string& label, std::optional<std::int64_t> demand, std::optional<std::int64_t> served, std::optional<std::int64_t> capacity, std::optional<std::int64_t> usableImport,
                                std::int64_t exports = 0) {
        ServiceVerdict v;
        if (!demand || !served) return v;
        if (*served >= *demand) return v;                                         // the service delivered covers the need
        const std::int64_t supply = (capacity ? *capacity : 0) + (usableImport ? *usableImport : 0) - std::max<std::int64_t>(0, exports);
        if (capacity && supply < *demand) {
            v.trigger = true;
            v.cause = ServiceCause::RegionalDeficit;
            v.text = label + " deficit: demand=" + std::to_string(*demand) + " served=" + std::to_string(*served) + " supply(local capacity " + std::to_string(*capacity) + " + usable imports " +
                     std::to_string(usableImport ? *usableImport : 0) + ")=" + std::to_string(supply) + " cannot cover the demand";
        } else {
            v.cause = ServiceCause::InternalShortfall;
            v.text = label + " served=" + std::to_string(*served) + " < demand=" + std::to_string(*demand) + " but the supply covers it (capacity " + std::to_string(capacity ? *capacity : -1) +
                     " + imports " + std::to_string(usableImport ? *usableImport : 0) + "): an internal connection problem, not a regional deficit";
        }
        return v;
    }

    // Observe one sample. Returns the verdict per commodity; a sample identical to the previous one of that commodity is stale and decides nothing.
    ServiceVerdict ObservePower(const ServiceCounters& c) { return Observe(c.power, Signature(c, true), pSig_, pBase_, pHaveBase_, pBaseCaptured_, c, true); }
    ServiceVerdict ObserveWater(const ServiceCounters& c) { return Observe(c.water, Signature(c, false), wSig_, wBase_, wHaveBase_, wBaseCaptured_, c, false); }

    // The game's own counters confirm a deficit of this commodity (set by a fresh sample, cleared by a fresh sample that shows the service delivered).
    bool InDeficit(bool power) const { return power ? pDeficit_ : wDeficit_; }

    // The commodity was just evaluated (a cycle ran for it): its baseline is the latest sample.
    void NoteEvaluated(const ServiceCounters& c, bool power) {
        if (power) { pBase_ = c; pHaveBase_ = true; pBaseDeficit_ = pDeficit_; } else { wBase_ = c; wHaveBase_ = true; wBaseDeficit_ = wDeficit_; }
    }

    // Garbage: a material change of the counters that drive the plan (production, processing capacity, funding efficiency, regional flows, the backlog) since Garbage was last evaluated.
    // The first sample is the reference. A sample identical to the previous one decides nothing.
    ServiceVerdict ObserveGarbage(const ServiceCounters& c) {
        ServiceVerdict v;
        if (!c.garbage) return v;
        const auto n = [](const Opt<std::int64_t>& o) { return o ? std::to_string(*o) : std::string("?"); };
        const std::string sig = n(c.gProduced) + '|' + n(c.gUncollected) + '|' + n(c.gW2e) + '|' + n(c.gIncinerator) + '|' + n(c.gEfficiency) + '|' + n(c.gGameImported) + '|' + n(c.gGameExported);
        if (sig == gSig_) { v.cause = ServiceCause::Stale; return v; }
        gSig_ = sig;
        if (!gHaveBase_) { gBase_ = c; gHaveBase_ = true; return v; }
        const auto moved = [](const Opt<std::int64_t>& now, const Opt<std::int64_t>& then, std::int64_t floor) {
            if (!now || !then) return false;
            return std::llabs(*now - *then) >= std::max<std::int64_t>(floor, static_cast<std::int64_t>(kDemandStepFraction * static_cast<double>(std::max<std::int64_t>(1, std::llabs(*then)))));
        };
        std::string why;
        if (moved(c.gProduced, gBase_.gProduced, 1)) why += " produced";
        if (moved(c.gW2e, gBase_.gW2e, 1)) why += " wasteToEnergyCapacity";
        if (moved(c.gIncinerator, gBase_.gIncinerator, 1)) why += " incineratorCapacity";
        if (moved(c.gEfficiency, gBase_.gEfficiency, 1)) why += " fundingEfficiency";
        if (moved(c.gGameImported, gBase_.gGameImported, 1)) why += " imports";
        if (moved(c.gGameExported, gBase_.gGameExported, 1)) why += " exports";
        // The backlog is an OUTCOME: while it grows nothing new can be planned (capacity, production and contracts are what the plan acts on, and they have their own triggers). Only its
        // appearance or its disappearance changes what the plan must do. (Triggering on its growth would run a full plan every simulated day in a city that cannot process its garbage.)
        if (c.gUncollected && gBase_.gUncollected && ((*c.gUncollected > 0) != (*gBase_.gUncollected > 0))) why += " backlog";
        if (!why.empty()) { v.trigger = true; v.cause = ServiceCause::MaterialChange; v.text = "GARBAGE material change since the last evaluation:" + why; }
        return v;
    }
    void NoteEvaluatedGarbage(const ServiceCounters& c) { gBase_ = c; gHaveBase_ = true; }

    // A purchase was written: from now on the first update that CHANGES the authoritative counters is judged against the expected service.
    void ExpectProof(bool power, std::int64_t contractedImport, const ServiceCounters& atWrite) {
        Proof& p = power ? pProof_ : wProof_;
        p = Proof{};
        p.active = true;
        p.contracted = contractedImport;
        p.sigAtWrite = Signature(atWrite, power);
    }
    // Returns a proof result the first time the counters have moved since the write (never from the stale sample of the moment of the write).
    bool CheckProof(const ServiceCounters& c, bool power, ServiceProof& out) {
        Proof& p = power ? pProof_ : wProof_;
        if (!p.active) return false;
        const std::string sig = Signature(c, power);
        if (sig == p.sigAtWrite) return false;                                    // the game has not updated the counters since the write: nothing to conclude
        p.active = false;
        const auto demand = power ? c.pDemand : c.wDemand;
        const auto served = ServedOf(c, power);
        const auto capacity = power ? c.pCapacity : c.wCapacity;
        const auto imported = power ? c.pImport : c.wImport;
        out = ServiceProof{};
        const std::string label = power ? "POWER" : "WATER";
        if (!demand || !served) { out.status = ProofStatus::StillShort; out.cause = "counters unreadable"; out.text = label + " service proof: counters unreadable"; return true; }
        if (*served >= *demand) {
            out.status = ProofStatus::Restored;
            out.text = label + " service restored: served=" + std::to_string(*served) + " >= demand=" + std::to_string(*demand) + " (contracted import " + std::to_string(p.contracted) + ", usable import " +
                       std::to_string(imported ? *imported : -1) + ")";
            return true;
        }
        out.status = ProofStatus::StillShort;
        const std::int64_t usable = imported ? *imported : 0;
        const std::int64_t supply = (capacity ? *capacity : 0) + usable;
        if (usable < p.contracted) out.cause = "CONTRACT_NOT_YET_EFFECTIVE: the game uses " + std::to_string(usable) + " of the contracted " + std::to_string(p.contracted);
        else if (supply < *demand) out.cause = "SUPPLY_INSUFFICIENT: local capacity " + std::to_string(capacity ? *capacity : -1) + " + imports " + std::to_string(usable) + " < demand " + std::to_string(*demand);
        else out.cause = "INTERNAL_OR_STALE: the supply covers the demand but the service delivered does not (connection problem, or the counter has not caught up)";
        out.text = label + " service NOT restored: served=" + std::to_string(*served) + " < demand=" + std::to_string(*demand) + " (" + out.cause + ")";
        return true;
    }

private:
    struct Proof { bool active = false; std::int64_t contracted = 0; std::string sigAtWrite; };

    static std::string Signature(const ServiceCounters& c, bool power) {
        const auto n = [](const Opt<std::int64_t>& o) { return o ? std::to_string(*o) : std::string("?"); };
        if (power) return n(c.pDemand) + '|' + n(c.pRequirement) + '|' + n(c.pConsumption) + '|' + n(c.pExport) + '|' + n(c.pProduction) + '|' + n(c.pImport) + '|' + n(c.pCapacity) + '|' + n(c.pGameImported) + '|' + n(c.pGameExported);
        return n(c.wDemand) + '|' + n(c.wConsumed) + '|' + n(c.wCapacity) + '|' + n(c.wImport) + '|' + n(c.wExport) + '|' + n(c.wGameImported) + '|' + n(c.wGameExported);
    }

    ServiceVerdict Observe(bool read, const std::string& sig, std::string& lastSig, ServiceCounters& base, bool& haveBase, bool&, const ServiceCounters& c, bool power) {
        ServiceVerdict v;
        if (!read) return v;
        if (sig == lastSig) { v.cause = ServiceCause::Stale; return v; }          // not a new observation: never repeat a decision on an old counter
        lastSig = sig;
        const auto demand = power ? c.pDemand : c.wDemand;
        const auto served = ServedOf(c, power);
        const auto capacity = power ? c.pCapacity : c.wCapacity;
        const auto imported = power ? c.pGameImported : c.wGameImported;           // the contracts as the game accounts them right now
        const auto usable = power ? c.pImport : c.wImport;
        const auto ex = power ? c.pGameExported : c.wGameExported;
        v = Judge(power ? "POWER" : "WATER", demand, served, capacity, usable ? usable : imported, ex ? *ex : 0);
        (power ? pDeficit_ : wDeficit_) = v.trigger ? true : (demand && served && *served >= *demand) ? false : (power ? pDeficit_ : wDeficit_);
        if (!haveBase) { base = c; haveBase = true; return v; }                    // the first sample is the reference (a deficit already visible on it triggers)
        // material change since this commodity was last evaluated
        const auto bd = power ? base.pDemand : base.wDemand;
        const auto bc = power ? base.pCapacity : base.wCapacity;
        const auto bi = power ? base.pGameImported : base.wGameImported;
        const auto be = power ? base.pGameExported : base.wGameExported;
        const auto ce = power ? c.pGameExported : c.wGameExported;
        // every quantity moves a little with rounding and with the neighbors' own consumption: only a move of kDemandStepFraction of the last evaluated value is material
        const auto moved = [](std::int64_t now, std::int64_t then) { return static_cast<double>(std::llabs(now - then)) >= kDemandStepFraction * static_cast<double>(std::max<std::int64_t>(1, std::llabs(then))); };
        std::string why;
        if (capacity && bc && moved(*capacity, *bc)) why += " capacity " + std::to_string(*bc) + "->" + std::to_string(*capacity);
        if (imported && bi && moved(*imported, *bi)) why += " imports " + std::to_string(*bi) + "->" + std::to_string(*imported);
        if (ce && be && moved(*ce, *be)) why += " exports " + std::to_string(*be) + "->" + std::to_string(*ce);
        if (demand && bd && moved(*demand, *bd)) why += " demand " + std::to_string(*bd) + "->" + std::to_string(*demand);
        if (v.trigger) {
            // A deficit that was already evaluated and whose supply side is unchanged (capacity, imports, exports, contracts) has nothing new to plan: the demand wobbling by a fraction of a percent
            // from one day to the next must not run the whole optimiser every day. Any material change, or a deficit that begins now, triggers at once.
            if ((power ? pBaseDeficit_ : wBaseDeficit_) && why.empty()) v.trigger = false;
            return v;
        }
        if (!why.empty()) {
            v.trigger = true;
            v.cause = ServiceCause::MaterialChange;
            v.text = std::string(power ? "POWER" : "WATER") + " material change since the last evaluation:" + why;
        }
        return v;
    }

    std::string pSig_, wSig_;
    ServiceCounters pBase_, wBase_;
    bool pHaveBase_ = false, wHaveBase_ = false, pBaseCaptured_ = false, wBaseCaptured_ = false;
    Proof pProof_, wProof_;
    bool pDeficit_ = false, wDeficit_ = false, pBaseDeficit_ = false, wBaseDeficit_ = false;
    std::string gSig_;
    ServiceCounters gBase_;
    bool gHaveBase_ = false;
};

} // namespace sc4af
