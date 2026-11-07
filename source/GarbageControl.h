// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure garbage control model for 1.1.641.0. No game types.
//
// What the simulator does every SimNewDay (disposal pipeline 0x006E3820):
//   exports -> waste-to-energy / incineration (building capacities already scaled by +0x228 and by the building condition)
//   -> landfill:  taken = min(remaining, free landfill storage, round(curve(landfillTiles) * eff) - GarbageSupply)
//   -> abduction (a real sink, a service FAILURE, never capacity).
// `curve` is GarbageTransportVersusLandfillTiles (0x8C19C3E9, installed (0,0)-(65536, 3.2768e8) = 5000 per tile) and
// `eff` is PollutionSimulator +0x228 = powerCurve(sanitation funding * 100) * 0.01.
//
// There is NO stable monthly landfill capacity to read: `GarbageSupply` is a valve accumulator that grows with the daily intake
// and is re-based by the landfill developer's own update (its value on the first day of month k+1 equals the
// previous month's Landfilled; the code that does it is only attributed to the landfill developer family
// 0x006B91D0, not decoded). So the model does not invent a monthly throughput. It uses the CURRENT valve headroom,
//     headroomNow = max(0, min(freeStorage, round(curve(tiles) * eff) - supply)),
// which is exactly what tomorrow's pipeline will use. It is re-evaluated on the same native daily cadence, so no runway is assumed.
//
// Policy (configurable reserve):
//   OwnGarbage          = max(0, Produced - Recycled)            (the Imported counter is LAST month's deliveries of neighbor deals: never own load)
//   ProtectedOwnGarbage = ceil(OwnGarbage x GarbageReserveFactor)  factor 1.02 .. 1.20, default 1.02; the standing backlog is recovered on top
// The reserve has absolute priority: a foreign import is only accepted when, with the funding chosen for it, the rest of the month projects a
// backlog of ZERO at the month end (PROJECTED_END_OF_MONTH_BACKLOG == 0 is the hard constraint). A transient backlog on the way is allowed, no peak
// ceiling is invented, no buffer is added to the daily peak. IMPORT_GARBAGE can economically justify a higher Sanitation funding when its revenue
// exceeds the incremental cost (the portfolio compares the complete states). Physical limits stay hard: free landfill storage, the valve, the
// building capacities and the vanilla import limit.
//
// How deliveries flow: a deal moves `round(Amount * 0.1)` per day while `Amount - AmountUsed > 0`, so an
// IMPORT arrives, and an EXPORT credit accrues, in the first ~10 days of the month (a contract created today delivers 10% of its Amount a day until
// AmountUsed >= Amount; an IMPORT can overshoot on its final step, an EXPORT credit is conservatively clamped to what is left). That is a property of EACH CONTRACT: the month is projected contract by contract (GarbageDealFlow, GarbageDealStream) and the daily
// deliveries / credits are summed; totals of Amount / AmountUsed are never used (round(sum x 0.1) is not the sum of round(each x 0.1)). The
// day-by-day order of the real pipeline is exports -> buildings -> landfill (valve).
//
// CONTROL_CRITICAL inputs : Produced - Recycled (own load), the funded/conditioned incinerator and W2E monthly getters, the export / import
//                           contracts with their AmountUsed, the valve (tiles, +0x228, supply, free storage), the standing backlog, Abducted.
// DIAGNOSTIC_ONLY         : the monthly mass-balance residual and every output counter
//                           (Incinerated, ToEnergy, Landfilled, Exported): see ResidualIsControlCritical().

#include "DealAccounting.h"
#include "LandfillGeometry.h"
#include "SafeMoney.h"
#include "UtilityHealth.h"
#include "UtilityModel.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sc4af {

// Work counters (diagnostics only, never read by a decision): how many funding plans / service evaluations the controller asked for. Single-threaded (game thread).
inline std::uint64_t gGarbagePlanCalls = 0;
inline std::uint64_t gGarbageEvalCalls = 0;

// Installed vanilla transport curve; an installation that overrides 0x8C19C3E9 must supply its own.
inline Curve VanillaLandfillTransportCurve() {
    Curve c;
    ParseCurve({0, 0, 65536, 327680000.0}, c);
    return c;
}

struct LandfillValve {
    bool hasLandfill = false;              // at least one landfill tile
    Opt<std::int64_t> tiles;
    Opt<std::int64_t> supply;              // PollutionSimulator +0x22C
    Opt<std::int64_t> freeStorage;         // GetLandfillUsage: capacity - usage
    Opt<double> transport;                 // curve(tiles), before the funding efficiency
    Opt<double> efficiency;                // PollutionSimulator +0x228
    Opt<std::int64_t> allowance;           // round(transport * efficiency)
    Opt<std::int64_t> headroomNow;         // max(0, min(freeStorage, allowance - supply))
};

inline std::int64_t RoundNearestEven(double v) { return static_cast<std::int64_t>(std::nearbyint(v)); }

// headroom for a hypothetical efficiency (everything else as read now)
inline Opt<std::int64_t> LandfillHeadroomAt(const LandfillValve& v, double efficiency) {
    if (!v.tiles) return std::nullopt;                  // missing is not "no landfill"
    if (!v.hasLandfill) return std::int64_t{0};
    if (!v.transport || !v.supply || !v.freeStorage) return std::nullopt;
    const std::int64_t allowance = RoundNearestEven(*v.transport * efficiency);
    return std::max<std::int64_t>(0, std::min<std::int64_t>(*v.freeStorage, allowance - *v.supply));
}

inline LandfillValve ComputeLandfillValve(const GarbageAggregate& a, const Curve& transportCurve) {
    LandfillValve v;
    if (a.landfillTiles) v.tiles = *a.landfillTiles;
    v.hasLandfill = v.tiles && *v.tiles > 0;
    if (a.garbageSupply) v.supply = static_cast<std::int64_t>(*a.garbageSupply);
    if (a.landfillCapacity && a.landfillUsage && *a.landfillCapacity >= *a.landfillUsage)
        v.freeStorage = static_cast<std::int64_t>(*a.landfillCapacity) - *a.landfillUsage;
    if (a.fundingEfficiency) v.efficiency = *a.fundingEfficiency;
    if (v.hasLandfill && transportCurve.valid()) v.transport = transportCurve.Eval(static_cast<double>(*v.tiles));
    if (v.hasLandfill && v.transport && v.efficiency) v.allowance = RoundNearestEven(*v.transport * *v.efficiency);
    if (v.hasLandfill && v.efficiency) v.headroomNow = LandfillHeadroomAt(v, *v.efficiency);
    if (!v.hasLandfill && v.tiles) v.headroomNow = 0;
    return v;
}

// SC4 calendar: 365-day years, months start on days 1, 32, 60, 91, 121, 152, ...
struct GarbageCalendar {
    int dayOfMonth = 1;       // 1-based
    int daysInMonth = 30;
    int dayOfYear = 1;        // 1..365
    double remainingFraction() const { return static_cast<double>(daysInMonth - dayOfMonth + 1) / daysInMonth; }
};

inline GarbageCalendar CalendarFromDayOfYear(int dayOfYear) {
    static const int kMonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    GarbageCalendar c;
    int d = std::clamp(dayOfYear, 1, 365);
    for (int m = 0; m < 12; ++m) {
        if (d <= kMonthDays[m]) {
            c.dayOfMonth = d;
            c.daysInMonth = kMonthDays[m];
            c.dayOfYear = std::clamp(dayOfYear, 1, 365);
            return c;
        }
        d -= kMonthDays[m];
    }
    return c;
}

// Length of the month that starts after the current one.
inline int DaysInNextMonth(const GarbageCalendar& c) {
    int next = c.dayOfYear - c.dayOfMonth + c.daysInMonth + 1;
    if (next > 365) next = 1;
    return CalendarFromDayOfYear(next).daysInMonth;
}

// One garbage contract as it stands now and as it is planned. Every contract moves its OWN schedule (round(Amount x 0.1) per day while
// Amount - AmountUsed > 0), so the month is projected contract by contract and never from totals of Amount / AmountUsed.
struct GarbageDealFlow {
    std::uint32_t neighbor = 0;             // other city serial (0: anonymous)
    bool isImport = true;                   // IMPORT_GARBAGE (foreign load, deliveries) or EXPORT_GARBAGE (relief credit)
    bool fixed = false;                     // suspended: the obligation cannot be changed by the plan
    std::int64_t amount = 0;                // Amount today (0: not contracted yet)
    std::int64_t used = 0;                  // AmountUsed this month (0 for a new contract)
    bool usedKnown = true;                  // AmountUsed was readable
    std::int64_t target = 0;                // planned Amount (== amount when kept; 0: cancelled; a fixed contract is never changed)
    bool usedRestarted = false;             // AmountUsed still showed LAST month's value at the start of this month: taken as 0 (StaleMonthStartAmountUsed)
};

// During the first days of a month the game still shows the AmountUsed of the PREVIOUS month; the rollover resets it a little later, to one day of deliveries
// (e.g. "7000/7000" on day 1, then "700/7000"). Read as is, an EXPORT would look exhausted for the whole month (no relief at all) and an IMPORT would look
// delivered, which would make the plan raise the Sanitation funding to 120% on the first day of every month for nothing. A value that the contract cannot have
// moved since the month began (more than (dayOfMonth - 1) daily steps of round(Amount x 0.1)) is last month's: it restarts at 0. Only in the first
// kStaleAmountUsedDays days (the observed window), so a contract whose Amount was cut later in the month is never re-credited.
constexpr int kStaleAmountUsedDays = 3;
inline bool StaleMonthStartAmountUsed(std::int64_t amount, std::int64_t used, int dayOfMonth) {
    if (dayOfMonth < 1 || dayOfMonth > kStaleAmountUsedDays || amount <= 0 || used <= 0) return false;
    const std::int64_t cap = std::max<std::int64_t>(1, static_cast<std::int64_t>(std::nearbyint(0.1 * static_cast<double>(amount))));
    return used > static_cast<std::int64_t>(dayOfMonth - 1) * cap;
}

struct GarbageServiceInput {
    GarbageAggregate aggregate;             // the Imported counter is never read: foreign garbage is not own load
    // The contracts, one entry each, with their planned target. Suspended EXPORT contracts are listed with `fixed`; their future relief is not credited
    // (no evidence that a suspended deal keeps flowing: fail closed) while they stay in the steady-state handling total.
    std::vector<GarbageDealFlow> deals;
    // Anonymous NEW contracts on top of `deals` (AmountUsed = 0, target = the amount): the whole amount starts today.
    std::int64_t extraImportLoad = 0;
    std::int64_t extraExportRelief = 0;
    std::int64_t backlog = 0;               // uncollected stock on the grid: a real need to recover, never capacity
    bool traceDays = false;                 // diagnostics: GarbageService::dayProjection
    // DUAL HORIZON. Only a CALIBRATED landfill geometry (LandfillGeometry.h, from normal boundary observations) lets the plan look one month ahead; every other state keeps the
    // continuity lock and this struct is ignored.
    LandfillGeometryInfo geometry;
    bool DualHorizonReady() const {
        return geometry.calibrated() && aggregate.landfillUsage && aggregate.landfillCapacity && aggregate.landfillTiles && aggregate.garbageSupply && aggregate.landfillMaintenance &&
               *aggregate.landfillMaintenance > 0 && aggregate.produced && aggregate.recycled;
    }
    bool fundingFrozen = false;             // the Sanitation funding is not under the Department's hands (AutoGarbage off): the only legal funding is the current one
    double reserve = 1.02;                  // Policy: protected own garbage = ceil(reserve x own load), 1.02 .. 1.20
    GarbageCalendar calendar;
    Curve transportCurve = VanillaLandfillTransportCurve();
    Curve fundingCurve;                     // 0x6911E2DF (the same funding -> efficiency curve as power)
};

// The garbage contracts of the snapshot, one flow each, kept as they are (target = Amount). A suspended contract is fixed. An AmountUsed that is not
// readable is flagged (never silently 0).
inline std::vector<GarbageDealFlow> GarbageFlowsOf(const std::vector<ExistingDealRow>& deals) {
    std::vector<GarbageDealFlow> out;
    for (const auto& d : deals) {
        if (d.type != utility_id::kDealImportGarbage && d.type != utility_id::kDealExportGarbage) continue;
        if (!d.amount || !DealIsCommitted(d)) continue;
        GarbageDealFlow f;
        f.neighbor = d.otherCitySerial ? *d.otherCitySerial : 0;
        f.isImport = d.type == utility_id::kDealImportGarbage;
        f.amount = std::max<std::int64_t>(0, *d.amount);
        f.usedKnown = d.amountUsed.has_value();
        // A suspended contract is an obligation we cannot change. An EXPORT whose AmountUsed cannot be read is one too: its relief cannot be projected, and
        // changing it would only trade a relief we cannot see for a new contract that starts from 0 (cancel / recreate). Unreadable is never 0.
        f.fixed = (d.flags && (*d.flags & deal_flag::kSuspensionMask)) || (!f.isImport && !f.usedKnown);
        f.used = d.amountUsed ? std::max<std::int64_t>(0, *d.amountUsed) : 0;
        f.target = f.amount;
        out.push_back(f);
    }
    return out;
}

// ---- the rest of the month, day by day ----------------------------------------------------------------------------------------------------
// The daily step of ONE contract: while Amount - AmountUsed > 0 it moves cap = round(Amount x 0.1) and adds it to
// AmountUsed. The step is NOT clamped to what is left, so an IMPORT can overshoot its Amount in its last day (the projection keeps the overshoot: the
// load is never underestimated). An EXPORT credit is clamped to what is left: the relief is never overestimated.
inline std::int64_t DealDailyCap(std::int64_t amount) {
    return amount > 0 ? std::max<std::int64_t>(1, static_cast<std::int64_t>(std::nearbyint(0.1 * static_cast<double>(amount)))) : 0;
}

struct GarbageDealStream {
    std::int64_t left = 0;                  // max(0, target - used)
    std::int64_t cap = 0;                   // round(target x 0.1)
    bool clampToLeft = false;               // exports: never credit more than what is left
    std::uint32_t id = 0;                   // neighbor serial (diagnostics only)
};

inline GarbageDealStream ImportStream(std::int64_t target, std::int64_t used) {
    target = std::max<std::int64_t>(0, target);
    return GarbageDealStream{std::max<std::int64_t>(0, target - std::max<std::int64_t>(0, used)), DealDailyCap(target), false};
}

// `known == false` (AmountUsed unreadable) and a suspended contract credit nothing in the future (fail closed): their AmountUsed is never replaced by 0.
inline GarbageDealStream ExportStream(std::int64_t target, std::int64_t used, bool known, bool fixed) {
    target = std::max<std::int64_t>(0, target);
    if (fixed || !known) return GarbageDealStream{0, DealDailyCap(target), true};
    return GarbageDealStream{std::max<std::int64_t>(0, target - std::max<std::int64_t>(0, used)), DealDailyCap(target), true};
}

// The sum, over the contracts, of what each one still delivers in `days` days (diagnostic and tests).
inline std::int64_t StreamsDelivery(std::vector<GarbageDealStream> streams, int days) {
    std::int64_t total = 0;
    for (int d = 0; d < days; ++d)
        for (auto& st : streams) {
            if (st.left <= 0) continue;
            total += st.clampToLeft ? std::min(st.left, st.cap) : st.cap;
            st.left -= st.cap;
        }
    return total;
}

struct GarbageProjectionInput {
    int daysLeft = 30;                      // including today
    int daysInMonth = 30;
    double ownProtectedMonthly = 0.0;       // ceil(own load x reserve)
    double backlog = 0.0;                   // standing stock to recover
    std::vector<GarbageDealStream> imports;   // one stream per contract: the deliveries arrive from each one's own AmountUsed
    std::vector<GarbageDealStream> exports;   // one stream per contract: the export credit accrues from each one's own AmountUsed
    double buildingsMonthly = 0.0;          // incinerator + W2E at the evaluated efficiency
    double valveHeadroom = -1.0;            // landfill intake still allowed this month; < 0: unlimited (need measurement)
    bool trace = false;                     // keep the per-day rows
};

// One simulated day (diagnostics, filled only when GarbageProjectionInput::trace is set).
struct GarbageDayRow {
    int day = 0;                            // 1 = today
    double ownGenerated = 0.0, importDelivery = 0.0, exportCreditAccrued = 0.0, exportCreditUsed = 0.0, buildingDisposed = 0.0, landfillDisposed = 0.0, backlogEnd = 0.0;
    std::vector<std::pair<std::uint32_t, std::int64_t>> importBy, exportBy;   // per contract
};

struct GarbageMonthProjection {
    bool computed = false;
    std::vector<GarbageDayRow> days;
    std::int64_t landfillIntake = 0;        // what the landfill absorbed (or would, with an unlimited valve)
    std::int64_t peakBacklog = 0;           // highest backlog at the end of any remaining day
    std::int64_t endBacklog = 0;            // backlog left at the month rollover: the hard constraint is 0
    std::int64_t importDelivered = 0, exportCredit = 0;
};

inline GarbageMonthProjection ProjectGarbageMonth(const GarbageProjectionInput& in) {
    GarbageMonthProjection r;
    const int dim = std::max(1, in.daysInMonth);
    const int days = std::max(0, in.daysLeft);
    const double own = in.ownProtectedMonthly / dim;
    const double buildings = in.buildingsMonthly / dim;
    double backlog = std::max(0.0, in.backlog), peak = backlog, landfilled = 0.0;
    auto imports = in.imports;
    auto exports = in.exports;
    double credit = 0.0, creditTotal = 0.0;
    double valveLeft = in.valveHeadroom;
    const bool unlimited = in.valveHeadroom < 0.0;
    for (int d = 0; d < days; ++d) {
        double imp = 0.0, accrue = 0.0;
        GarbageDayRow row;
        for (auto& st : imports) {
            if (st.left <= 0) continue;
            imp += static_cast<double>(st.cap);
            if (in.trace) row.importBy.emplace_back(st.id, st.cap);
            st.left -= st.cap;
        }
        for (auto& st : exports) {
            if (st.left <= 0) continue;
            const std::int64_t step = st.clampToLeft ? std::min(st.left, st.cap) : st.cap;
            accrue += static_cast<double>(step);
            if (in.trace) row.exportBy.emplace_back(st.id, step);
            st.left -= st.cap;
        }
        r.importDelivered += static_cast<std::int64_t>(std::llround(imp));
        credit += accrue;
        creditTotal += accrue;
        const double avail = backlog + own + imp;
        const double ex = std::min(avail, credit);
        credit -= ex;
        const double b = std::min(avail - ex, buildings);
        const double rest = avail - ex - b;
        const double l = unlimited ? rest : std::min(rest, std::max(0.0, valveLeft));
        if (!unlimited) valveLeft -= l;
        landfilled += l;
        backlog = rest - l;
        peak = std::max(peak, backlog);
        if (in.trace) {
            row.day = d + 1;
            row.ownGenerated = own;
            row.importDelivery = imp;
            row.exportCreditAccrued = accrue;
            row.exportCreditUsed = ex;
            row.buildingDisposed = b;
            row.landfillDisposed = l;
            row.backlogEnd = backlog;
            r.days.push_back(std::move(row));
        }
    }
    r.computed = true;
    r.exportCredit = static_cast<std::int64_t>(std::llround(creditTotal));
    r.landfillIntake = static_cast<std::int64_t>(std::ceil(landfilled - 1e-6));
    r.peakBacklog = static_cast<std::int64_t>(std::ceil(peak - 1e-6));
    r.endBacklog = static_cast<std::int64_t>(std::ceil(backlog - 1e-6));
    if (r.landfillIntake < 0) r.landfillIntake = 0;
    if (r.peakBacklog < 0) r.peakBacklog = 0;
    if (r.endBacklog < 0) r.endBacklog = 0;
    return r;
}

struct GarbageService {
    bool computable = false;
    Opt<std::int64_t> baseDisposalLoad;     // OWN load: Produced - Recycled (the Imported counter and foreign garbage are not in it)
    Opt<std::int64_t> requiredHandling;     // ceil(reserve x own load) + backlog + planned imports (monthly steady-state view)
    std::int64_t fixedHandling = 0;         // contracted + planned exports + funded incinerator + funded W2E (monthly, authoritative)
    std::int64_t landfillNeedMonthly = 0;   // what the landfill must absorb per month beyond the fixed handling
    std::int64_t landfillNeedRemaining = 0; // the intake the REST of this month needs (day-by-day projection, unlimited valve)
    Opt<std::int64_t> landfillHeadroomNow;
    std::int64_t projectedPeakBacklog = 0;  // transient backlog on the way (allowed, no ceiling)
    std::int64_t projectedEndBacklog = 0;   // backlog at the month rollover (must be 0)
    std::int64_t importRemaining = 0;       // foreign deliveries still to come this month
    // ---- dual horizon (only when the geometry is calibrated and the funding percent is given) ----
    bool dualActive = false;
    bool h2Feasible = false;                // the NEXT full month (AmountUsed = 0, the targets persist) ends with no backlog, under BOTH quantizations of A
    std::int64_t h1EndSupply = 0;           // supply at the end of H1 (supply now + the landfill intake of the projection)
    std::uint64_t boundaryDecay = 0, boundaryHanded = 0, boundaryAbsorbed = 0, nextUsage = 0, nextFree = 0, ratioA = 0, ratioB = 0;
    std::int64_t nextSupply = 0, h2PeakBacklog = 0, h2EndBacklog = 0;
    std::vector<GarbageDayRow> dayProjection;   // the day-by-day rows of the projection the verdict rests on (only when traceDays)
    bool landfillUnknown = false;
    bool serviceShortage = false;           // Abducted > 0: the pipeline has already overflowed (a structural failure, never capacity)
    bool capacitySufficient = false;        // the projection recovers everything by the month end
    bool sufficient = false;                // capacitySufficient and no standing structural failure
    std::string basis;
};

// Structural failure signal: none exists. The standing uncollected stock is not one: it is a real need that the projection recovers (or not) by the month end.
// Abducted garbage is the game's own decay: surplus garbage that the pipeline could not place is deleted for free (capped at 2000 a month, tuning 0x09135479).
// Policy: it is neither a failure nor a ban on cutting the funding; it stays in the accounting balance as an output and in the log. Pinning the funding to its
// current value on Abducted > 0 would let it only rise, ratcheting it up while the capacity stays far above the garbage.
inline bool GarbageShortageSignal(const GarbageAggregate&) {
    return false;
}

// The month after the current one, from the state at the end of H1. Fills the dual-horizon fields of `s`; fail closed (h2Feasible = false) when anything is missing.
// Boundary = LandfillMonthBoundary (measured). The landfill ratio A / B is the LINEAR funding ratio (A = trunc(B x funding)), not the
// funding-efficiency curve; A was observed once at trunc + 1 (a funding change inside the month), so BOTH A and A + 1 must leave the next month feasible.
inline void EvaluateNextMonth(const GarbageServiceInput& in, double efficiency, int fundingPercent, std::int64_t protectedOwn, std::int64_t scaledBuildings, std::int64_t h1Intake,
                              GarbageService& s) {
    const auto& a = in.aggregate;
    s.dualActive = true;
    s.h2Feasible = false;
    const std::int64_t supplyNow = static_cast<std::int64_t>(*a.garbageSupply);
    s.h1EndSupply = supplyNow + std::max<std::int64_t>(0, h1Intake);
    const std::uint64_t B = static_cast<std::uint64_t>(*a.landfillMaintenance);
    const float f = static_cast<float>(fundingPercent) / 100.0f;
    const std::uint64_t A0 = f >= 1.0f ? B : static_cast<std::uint64_t>(static_cast<float>(B) * f);
    const auto valve = ComputeLandfillValve(a, in.transportCurve);
    if (!valve.transport) return;
    const std::int64_t allowance = RoundNearestEven(*valve.transport * efficiency);
    const int nextDays = DaysInNextMonth(in.calendar);
    bool feasible = true;
    for (const std::uint64_t A : {A0, std::min<std::uint64_t>(A0 + 1, B)}) {
        LandfillBoundaryState st;
        st.usage = *a.landfillUsage;
        st.capacityTotal = *a.landfillCapacity;
        st.activeCells = in.geometry.activeCells;
        st.zoneTiles = static_cast<std::uint64_t>(std::max<std::int32_t>(0, *a.landfillTiles));
        st.supply = s.h1EndSupply;
        st.effNumerator = A;
        st.effDenominator = B;
        const auto r = LandfillMonthBoundary(st);
        if (!r.computable) { feasible = false; break; }
        if (A == A0) {
            s.boundaryDecay = r.decayed; s.boundaryHanded = r.handedOver; s.boundaryAbsorbed = r.absorbed; s.nextUsage = r.usageAfter; s.nextFree = r.freeAfter; s.nextSupply = r.supplyAfter;
            s.ratioA = A; s.ratioB = B;
        }
        GarbageProjectionInput p2;
        p2.daysInMonth = nextDays;
        p2.daysLeft = nextDays;
        p2.ownProtectedMonthly = static_cast<double>(protectedOwn);
        p2.backlog = 0.0;                                           // H1 feasible = no backlog at the boundary; the uncollected stock persists unchanged
        p2.buildingsMonthly = static_cast<double>(scaledBuildings);
        if (in.extraImportLoad > 0) p2.imports.push_back(ImportStream(in.extraImportLoad, 0));
        if (in.extraExportRelief > 0) p2.exports.push_back(ExportStream(in.extraExportRelief, 0, true, false));
        for (const auto& fl : in.deals) {
            const std::int64_t target = std::max<std::int64_t>(0, fl.fixed ? fl.amount : fl.target);
            if (fl.isImport) p2.imports.push_back(ImportStream(target, 0));                       // AmountUsed restarts at 0
            else p2.exports.push_back(ExportStream(target, 0, true, fl.fixed));                   // a suspended export still credits nothing
        }
        const std::int64_t headroom = std::max<std::int64_t>(0, std::min<std::int64_t>(static_cast<std::int64_t>(r.freeAfter), allowance - r.supplyAfter));
        p2.valveHeadroom = static_cast<double>(headroom);
        const auto h2 = ProjectGarbageMonth(p2);
        if (A == A0) { s.h2PeakBacklog = h2.peakBacklog; s.h2EndBacklog = h2.endBacklog; }
        if (h2.endBacklog != 0) feasible = false;
    }
    s.h2Feasible = feasible;
}

// Service at an efficiency different from the current one (used to evaluate funding candidates). Building capacities scale with
// the efficiency (e.g. 34088 -> 21238 for 0.40125 -> 0.25), the valve allowance scales with it, the load does not.
inline GarbageService EvaluateGarbageServiceAt(const GarbageServiceInput& in, double efficiency, int fundingPercent = -1) {
    ++gGarbageEvalCalls;
    GarbageService s;
    const auto& a = in.aggregate;
    s.basis = "own load = Produced - Recycled; protected = ceil(reserve x own) + backlog; deliveries day by day: exports -> buildings -> landfill valve";
    s.serviceShortage = GarbageShortageSignal(a);
    if (!a.produced || !a.imported || !a.recycled) return s;
    if (!a.incineratorCapacityMonthly || !a.w2eCapacityMonthly) return s;
    if (!a.fundingEfficiency || *a.fundingEfficiency < 0.0f) return s;

    const std::int64_t own = std::max<std::int64_t>(0, static_cast<std::int64_t>(*a.produced) - *a.recycled);
    s.baseDisposalLoad = own;
    const std::int64_t protectedOwn = CeilTarget(static_cast<double>(own) * in.reserve);
    std::int64_t importTotal = std::max<std::int64_t>(0, in.extraImportLoad);
    std::int64_t exportTotal = std::max<std::int64_t>(0, in.extraExportRelief);
    GarbageProjectionInput p;
    if (in.extraImportLoad > 0) p.imports.push_back(ImportStream(in.extraImportLoad, 0));
    if (in.extraExportRelief > 0) p.exports.push_back(ExportStream(in.extraExportRelief, 0, true, false));
    for (const auto& f : in.deals) {
        const std::int64_t target = std::max<std::int64_t>(0, f.fixed ? f.amount : f.target);
        if (f.isImport) {
            importTotal += target;                        // a suspended import keeps being hosted: the load is never underestimated
            p.imports.push_back(ImportStream(target, f.usedKnown ? f.used : 0));
            p.imports.back().id = f.neighbor;
        } else {
            exportTotal += target;
            p.exports.push_back(ExportStream(target, f.used, f.usedKnown, f.fixed));
            p.exports.back().id = f.neighbor;
        }
    }
    const std::int64_t backlog = std::max<std::int64_t>(0, in.backlog);
    s.requiredHandling = protectedOwn + backlog + importTotal;

    const double now = *a.fundingEfficiency;
    const std::int64_t buildings = std::max<std::int64_t>(0, *a.incineratorCapacityMonthly) + std::max<std::int64_t>(0, *a.w2eCapacityMonthly);
    std::int64_t scaledBuildings = buildings;
    if (std::fabs(efficiency - now) > 1e-9) {
        if (now <= 0.0) {
            if (buildings > 0) return s;   // capacity under a zero efficiency cannot be rescaled: unknown, fail closed
        } else {
            scaledBuildings = RoundNearestEven(static_cast<double>(buildings) * efficiency / now);
        }
    }
    s.fixedHandling = exportTotal + scaledBuildings;
    s.landfillNeedMonthly = std::max<std::int64_t>(0, *s.requiredHandling - s.fixedHandling);

    p.daysInMonth = in.calendar.daysInMonth;
    p.daysLeft = in.calendar.daysInMonth - in.calendar.dayOfMonth + 1;
    p.ownProtectedMonthly = static_cast<double>(protectedOwn);
    p.backlog = static_cast<double>(backlog);
    p.buildingsMonthly = static_cast<double>(scaledBuildings);
    p.trace = in.traceDays;

    const auto need = ProjectGarbageMonth(p);            // unlimited valve: how much the landfill has to take
    s.importRemaining = need.importDelivered;
    s.landfillNeedRemaining = need.landfillIntake;

    const auto valve = ComputeLandfillValve(a, in.transportCurve);
    s.landfillHeadroomNow = LandfillHeadroomAt(valve, efficiency);
    s.landfillUnknown = !s.landfillHeadroomNow.has_value();
    s.computable = true;
    if (s.landfillNeedRemaining == 0) {
        s.capacitySufficient = true;   // the buildings and exports carry the rest of the month; no landfill is needed
    } else if (s.landfillHeadroomNow) {
        p.valveHeadroom = static_cast<double>(*s.landfillHeadroomNow);
        const auto real = ProjectGarbageMonth(p);
        s.projectedPeakBacklog = real.peakBacklog;
        s.projectedEndBacklog = real.endBacklog;
        s.dayProjection = real.days;
        s.capacitySufficient = real.endBacklog == 0;
    } else {
        s.capacitySufficient = false;   // the load needs a landfill whose valve cannot be read: never "sufficient"
    }
    if (s.landfillNeedRemaining == 0) { s.projectedPeakBacklog = need.peakBacklog; s.projectedEndBacklog = need.endBacklog; s.dayProjection = need.days; }
    if (fundingPercent >= 0 && in.DualHorizonReady()) {
        // the valve-limited intake of the rest of this month (0 when the buildings and exports carry it)
        std::int64_t h1Intake = 0;
        if (s.landfillNeedRemaining > 0 && s.landfillHeadroomNow) {
            auto q = p;
            q.valveHeadroom = static_cast<double>(*s.landfillHeadroomNow);
            q.trace = false;
            h1Intake = ProjectGarbageMonth(q).landfillIntake;
        }
        EvaluateNextMonth(in, efficiency, fundingPercent, protectedOwn, scaledBuildings, h1Intake, s);
    }
    s.sufficient = s.capacitySufficient && !s.serviceShortage;
    return s;
}

inline GarbageService EvaluateGarbageService(const GarbageServiceInput& in) {
    if (!in.aggregate.fundingEfficiency) return GarbageService{};
    return EvaluateGarbageServiceAt(in, *in.aggregate.fundingEfficiency);
}

// The monthly mass-balance residual is DIAGNOSTIC_ONLY: EvaluateGarbageService never reads a counter on
// the output side nor the residual, and the failure signal it does use (Abducted) is independent of it. In
// observed data (Abducted = 0, Uncollected = 0) the 3.1-3.8% residual is the sum of fractional parts dropped by the truncating
// counter updates (_ftol of old + taken after every pipeline call while the exact float is removed from the garbage), not garbage
// that escaped disposal.
inline bool ResidualIsControlCritical() { return false; }

// ---- funding plan -------------------------------------------------------------------------------------------------------
struct GarbageFundingChoice {
    int percent = 0;
    std::int64_t expense = 0;
    GarbageService service;
};

enum class GarbagePlanStatus { Optimal, NoChange, InsufficientCapacity, NotAvailable };

inline const char* GarbagePlanStatusName(GarbagePlanStatus s) {
    switch (s) {
    case GarbagePlanStatus::Optimal: return "OPTIMAL";
    case GarbagePlanStatus::NoChange: return "NO_CHANGE";
    case GarbagePlanStatus::InsufficientCapacity: return "INSUFFICIENT_CAPACITY";
    case GarbagePlanStatus::NotAvailable: return "NOT_AVAILABLE";
    }
    return "?";
}

struct GarbageFundingPlan {
    GarbagePlanStatus status = GarbagePlanStatus::NotAvailable;
    int currentPercent = 0;
    int targetPercent = 0;
    std::int64_t currentExpense = 0;
    std::int64_t targetExpense = 0;
    GarbageService serviceNow;
    GarbageService serviceAfter;
    std::string note;
    // Economic expense the portfolio is compared on. Continuity fallback: the cash target expense. DUAL HORIZON: the expense of the lowest funding that keeps the NEXT full
    // month feasible (a monthly run-rate, like the deal money); the applied funding is the joint H1 + H2 minimum (safety), its cash expense is targetExpense.
    std::int64_t steadyStateExpense = 0;
    int steadyStatePercent = 0;
    bool dualHorizon = false;
    std::int64_t saving() const { return currentExpense - targetExpense; }
};

// Lowest-expense funding whose projection recovers the whole month. A standing structural failure (Abducted) forbids any reduction below the
// current funding. `departmentIdeal` is DepartmentBudget::GetIdealMonthlyFunding (charged expense = ideal * percent, as for Water).
// Feasibility is monotone in the funding (the efficiency curve is non-decreasing and every capacity grows with it), so the cheapest feasible
// funding is found by bisection; a curve that is not monotone falls back to the full scan.
inline GarbageFundingPlan PlanGarbageFunding(const GarbageServiceInput& in, Opt<float> funding, Opt<float> maxAllowed, Opt<std::int64_t> departmentIdeal) {
    ++gGarbagePlanCalls;
    GarbageFundingPlan p;
    if (!funding || !maxAllowed || !departmentIdeal || !in.fundingCurve.valid() || !in.aggregate.fundingEfficiency) {
        p.note = "department funding, maximum, ideal expense, curve or +0x228 efficiency unknown";
        return p;
    }
    p.currentPercent = UtilityPercentRound(*funding);
    p.currentExpense = ChargedExpense(*departmentIdeal, p.currentPercent);
    p.serviceNow = EvaluateGarbageService(in);
    if (!p.serviceNow.computable) { p.note = "service not computable (load, capacities or valve unknown)"; return p; }
    const int hi = in.fundingFrozen ? p.currentPercent : UtilityPercentRound(*maxAllowed);
    const bool noReduction = p.serviceNow.serviceShortage || in.fundingFrozen;
    const int first = noReduction ? p.currentPercent : 0;
    const bool dual = in.DualHorizonReady();
    const auto at = [&](int pct) { return EvaluateGarbageServiceAt(in, in.fundingCurve.Eval(pct) / 100.0, dual ? pct : -1); };
    bool monotone = true;
    for (int v = 1; v <= 120 && monotone; ++v) monotone = in.fundingCurve.Eval(v) >= in.fundingCurve.Eval(v - 1) - 1e-12;
    // Lowest-expense funding that satisfies `ok` (monotone in the funding), ties on the expense go to the percent closest to the current one.
    const auto search = [&](auto ok) -> std::optional<GarbageFundingChoice> {
        std::optional<GarbageFundingChoice> best;
        const auto consider = [&](int pct, const GarbageService& sv) {
            const auto expense = ChargedExpense(*departmentIdeal, pct);
            if (!best || expense < best->expense || (expense == best->expense && std::abs(pct - p.currentPercent) < std::abs(best->percent - p.currentPercent)))
                best = GarbageFundingChoice{pct, expense, sv};
        };
        if (monotone) {
            int lo = first, top = hi;                       // smallest feasible percent in [first, hi]
            if (lo <= top) {
                const auto sTop = at(top);
                if (ok(sTop)) {
                    while (lo < top) {
                        const int mid = lo + (top - lo) / 2;
                        if (ok(at(mid))) top = mid; else lo = mid + 1;
                    }
                    // same expense (rounding) above the minimum: take the one closest to the current funding
                    const auto base = ChargedExpense(*departmentIdeal, lo);
                    for (int pct = lo; pct <= hi && ChargedExpense(*departmentIdeal, pct) == base; ++pct) consider(pct, at(pct));
                }
            }
        } else {
            for (int pct = first; pct <= hi; ++pct) {
                const auto sv = at(pct);
                if (ok(sv)) consider(pct, sv);
            }
        }
        return best;
    };
    // H1 (the rest of this month) is HARD SAFETY; with a calibrated geometry the next full month (H2) must be feasible as well.
    const auto joint = search([](const GarbageService& sv) { return sv.computable && sv.capacitySufficient && (!sv.dualActive || sv.h2Feasible); });
    if (!joint) {
        // Nothing is sufficient: the best SERVICE the department can give is the one of the highest legal funding (the projected backlog never grows with the funding). The
        // plan takes the CHEAPEST funding that gives exactly that service, not the maximum itself: when the landfill's free STORAGE is what limits the month (a nearly
        // full landfill: headroom == free storage), a higher funding buys nothing. When the funding does help (the valve or the buildings limit), the cheapest funding
        // of the best service is the maximum.
        const int top = std::max(p.currentPercent, hi);
        const auto best = at(top);
        p.status = GarbagePlanStatus::InsufficientCapacity;
        p.targetPercent = top;
        p.serviceAfter = best;
        if (best.computable) {
            const std::int64_t endBest = best.projectedEndBacklog, h2Best = best.h2EndBacklog;
            const bool h2 = best.dualActive;
            const auto sameService = search([endBest, h2Best, h2](const GarbageService& sv) {
                return sv.computable && sv.projectedEndBacklog <= endBest && (!h2 || (sv.dualActive && sv.h2EndBacklog <= h2Best));
            });
            if (sameService && sameService->percent < top) {
                p.targetPercent = sameService->percent;
                p.serviceAfter = sameService->service;
                p.note = "the funding cannot recover the month; above " + std::to_string(sameService->percent) + "% it buys no less backlog (the landfill storage, not the funding, limits it)";
            }
        }
        p.targetExpense = ChargedExpense(*departmentIdeal, p.targetPercent);
        p.steadyStateExpense = p.targetExpense;
        p.dualHorizon = dual;
        if (p.note.empty()) p.note = dual ? "no legal funding keeps both the rest of the month and the next month feasible" : "no legal funding recovers the whole month";
        return p;
    }
    p.targetPercent = joint->percent;
    p.targetExpense = joint->expense;
    p.serviceAfter = joint->service;
    p.dualHorizon = dual;
    p.steadyStatePercent = joint->percent;
    p.steadyStateExpense = joint->expense;
    if (dual) {
        // the economic comparison: the lowest funding that keeps the NEXT month feasible, whatever the end of this month needs
        const auto steady = search([](const GarbageService& sv) { return sv.computable && sv.dualActive && sv.h2Feasible; });
        if (steady) { p.steadyStatePercent = steady->percent; p.steadyStateExpense = std::min(steady->expense, joint->expense); }
    }
    p.status = joint->percent == p.currentPercent ? GarbagePlanStatus::NoChange : GarbagePlanStatus::Optimal;
    if (p.serviceNow.serviceShortage) p.note = "Abducted garbage is standing: funding cannot be reduced below the current value";
    return p;
}

// DIAGNOSTIC_ONLY. NOT a global maximum and NOT used by the optimizer or by any decision (it only feeds the `recoverableBoundaryProbe=` field of the
// [Garbage][ImportSchedule] log). It bisects the amount of ONE fresh IMPORT contract (AmountUsed = 0, the whole amount starts today, on top of the
// suspended contracts; the changeable imports are set aside) at funding `percent`. Feasibility is NOT monotone in the amount (the last daily step of an
// IMPORT is not clamped to the remainder and can overshoot the Amount), so the result is only A boundary found by bisection: feasible there, and not
// proven to be the largest feasible amount. `limit` bounds the search.
inline std::int64_t RecoverableBoundaryProbe(const GarbageServiceInput& in, int percent, std::int64_t limit) {
    GarbageServiceInput x = in;
    for (auto& f : x.deals) if (f.isImport && !f.fixed) f.target = 0;
    const double eff = in.fundingCurve.Eval(percent) / 100.0;
    const auto ok = [&](std::int64_t amount) {
        x.extraImportLoad = amount;
        const auto s = EvaluateGarbageServiceAt(x, eff);
        return s.computable && s.capacitySufficient;
    };
    if (limit <= 0 || !ok(0)) return 0;
    if (ok(limit)) return limit;
    std::int64_t lo = 0, hi = limit;                  // ok(lo), !ok(hi): a boundary of the bisection, not a proven maximum
    while (hi - lo > 1) {
        const std::int64_t mid = lo + (hi - lo) / 2;
        if (ok(mid)) lo = mid; else hi = mid;
    }
    return lo;
}

} // namespace sc4af
