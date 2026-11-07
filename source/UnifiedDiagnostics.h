// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Structured, deterministic, searchable diagnostics of the deal / Garbage decision. Nothing here is read by a decision: it only
// describes the snapshot, the candidates, the legal portfolios, the optimum, its policy-legal counterfactual and the day-by-day Garbage schedule, so a
// session can be reconstructed offline:
//   snapshot -> candidates -> legal portfolios -> optimum -> counterfactual -> action -> quote -> write -> readback -> new snapshot -> new optimum.
// Every line carries `plan=` and `decisionId=`; deal lines also `commodity=`, `type=` and `neighbor=`.

#include "DealCausal.h"
#include "UnifiedPlan.h"

#include <sstream>
#include <string>
#include <vector>

namespace sc4af {

inline std::string DecisionId(const UtilitySnapshot& s, std::uint32_t planId) {
    return "d" + std::to_string(s.simYear) + '-' + std::to_string(s.simDayOfYear) + "-p" + std::to_string(planId);
}

inline std::string CommodityName(Commodity c) { return c == Commodity::Power ? "POWER" : c == Commodity::Water ? "WATER" : "GARBAGE"; }

// The exogenous state of the Garbage side: everything that can legitimately change the optimum without any deal write of ours.
inline DealCausal DescribeGarbageCausal(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t) {
    DealCausal c;
    const auto base = GarbageBaseInput(s, cfg, t);
    const auto& a = s.garbage.aggregate;
    const auto num = [](const auto& o) { return o ? std::to_string(static_cast<long long>(*o)) : std::string("?"); };
    c.fields["day"] = std::to_string(s.simYear) + '/' + std::to_string(s.simDayOfYear);
    c.fields["ownLoad"] = std::to_string(static_cast<long long>(base.noDealLoad));
    c.fields["backlog"] = std::to_string(static_cast<long long>(base.input.backlog));
    c.fields["abducted"] = num(a.abducted);
    c.fields["funding"] = s.garbage.department.funding ? std::to_string(UtilityPercentRound(*s.garbage.department.funding)) : std::string("?");
    c.fields["efficiency"] = a.fundingEfficiency ? std::to_string(static_cast<int>(*a.fundingEfficiency * 10000.0f)) : std::string("?");
    c.fields["supply"] = num(a.garbageSupply);
    c.fields["freeStorage"] = (a.landfillCapacity && a.landfillUsage) ? std::to_string(static_cast<long long>(*a.landfillCapacity) - static_cast<long long>(*a.landfillUsage)) : std::string("?");
    c.fields["buildings"] = num(a.incineratorCapacityMonthly) + '+' + num(a.w2eCapacityMonthly);
    return c;
}

// Stability fingerprints of a Power / Water funding change: the physical inputs the plan read, NEVER the funding itself nor what follows from it directly (produced, used,
// efficiency, cost, capacity), and not the calendar by itself. Equal fingerprints = equivalent conditions; any different field = the world moved.
inline std::string DescribePowerCausal(const UtilitySnapshot& s, int cellX, int cellZ) {
    const auto num = [](const auto& o) { return o ? std::to_string(static_cast<long long>(*o)) : std::string("?"); };
    const auto flt = [](const auto& o) { return o ? std::to_string(static_cast<long double>(*o)) : std::string("?"); };
    const auto& a = s.power.aggregate;
    std::ostringstream c;
    c << "demand=" << num(a.demand) << ";consumption=" << num(a.consumption) << ";import=" << num(a.import_) << ";export=" << num(a.export_) << ";plants=" << num(a.plantCount) << ";aged=" << num(a.aged)
      << ";strike=" << (a.onStrike ? (*a.onStrike ? 1 : 0) : -1);
    for (const auto& p : s.power.plants)
        if (p.ref.cellX == cellX && p.ref.cellZ == cellZ) {
            c << ";condition=" << flt(p.conditionPercent) << ";maxProduced=" << num(p.maxProduced) << ";baseAging=" << flt(p.baseAgingRate) << ";agedThreshold=" << flt(p.agedThreshold) << ";replacement=" << num(p.replacementCost);
            break;
        }
    return c.str();
}
inline std::string DescribeWaterCausal(const UtilitySnapshot& s) {
    const auto num = [](const auto& o) { return o ? std::to_string(static_cast<long long>(*o)) : std::string("?"); };
    const auto flt = [](const auto& o) { return o ? std::to_string(static_cast<long double>(*o)) : std::string("?"); };
    const auto& a = s.water.aggregate;
    std::ostringstream c;
    c << "demand=" << num(a.demand) << ";consumed=" << num(a.consumed) << ";imported=" << num(a.imported) << ";exported=" << num(a.exported) << ";sources=" << num(a.sources) << ";polluted=" << num(a.pollutedSources)
      << ";pipes=" << num(a.pipes) << ";distressed=" << num(a.distressedPipes) << ";burst=" << num(a.burstPipes);
    for (const auto& p : s.water.producers) c << ";producer@" << p.ref.cellX << ',' << p.ref.cellZ << "=" << flt(p.conditionPercent) << '/' << num(p.maxCapacity);
    return c.str();
}

// The causal fingerprint of one deal write: the commodity's exogenous state, the AmountUsed of every contract of the commodity (the slot's own included)
// and the amounts of the OTHER contracts, and the BASE market of the slot's neighbor (our own contracts removed). The controlled amount of the slot is not
// part of it (it would differ after every write and explain everything).
inline DealCausal DescribeDealCausal(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const DealKey& key) {
    const Commodity com = CommodityOfDealType(key.type);
    const int ci = com == Commodity::Power ? 0 : com == Commodity::Water ? 1 : 2;
    DealCausal c;
    if (com == Commodity::Garbage) c = DescribeGarbageCausal(s, cfg, t);
    c.fields["day"] = std::to_string(s.simYear) + '/' + std::to_string(s.simDayOfYear);
    double soldTotal = 0.0, boughtTotal = 0.0, theirs = 0.0;
    for (const auto& r : s.deals.deals) {
        if (!r.amount || !IsKnownDealType(r.type) || CommodityOfDealType(r.type) != com || !DealIsCommitted(r)) continue;
        const std::string slot = std::to_string(r.type) + '/' + std::to_string(r.otherCitySerial ? *r.otherCitySerial : 0u);
        const bool self = r.type == key.type && r.otherCitySerial && *r.otherCitySerial == key.neighbor;
        c.fields["used:" + slot] = r.amountUsed ? std::to_string(static_cast<long long>(*r.amountUsed)) : std::string("?");
        if (!self) c.fields["amount:" + slot] = std::to_string(*r.amount);
        if (!(r.flags && (*r.flags & deal_flag::kSuspensionMask))) (LocalIsBuyer(r.type) ? boughtTotal : soldTotal) += *r.amount;
        if (r.otherCitySerial && *r.otherCitySerial == key.neighbor) theirs += LocalIsBuyer(r.type) ? -*r.amount : *r.amount;
    }
    const auto& lrow = s.deals.localMarket.book[ci];
    if (lrow.complete()) {
        const CommodityBook b = lrow.get();
        c.fields["local.produced"] = std::to_string(static_cast<long long>(b.produced));
        c.fields["local.demanded"] = std::to_string(static_cast<long long>(b.demanded));
        c.fields["local.exportedBase"] = std::to_string(static_cast<long long>(std::max(0.0, b.exported - soldTotal)));
        c.fields["local.importedBase"] = std::to_string(static_cast<long long>(std::max(0.0, b.imported - boughtTotal)));
    }
    for (const auto& n : s.deals.neighbors) {
        if (n.serial != key.neighbor || !n.book[ci].complete()) continue;
        const CommodityBook b = n.book[ci].get();
        c.fields["nb.produced"] = std::to_string(static_cast<long long>(b.produced));
        c.fields["nb.demanded"] = std::to_string(static_cast<long long>(b.demanded));
        c.fields["nb.exportedBase"] = std::to_string(static_cast<long long>(std::max(0.0, b.exported - std::max(0.0, -theirs))));
        c.fields["nb.importedBase"] = std::to_string(static_cast<long long>(std::max(0.0, b.imported - std::max(0.0, theirs))));
    }
    return c;
}

inline std::string OptionList(const std::vector<std::int32_t>& v) {
    std::ostringstream o;
    o << '[';
    const auto put = [&](std::size_t i) { o << (i ? "," : "") << v[i]; };
    if (v.size() <= 40) { for (std::size_t i = 0; i < v.size(); ++i) put(i); o << ']'; return o.str(); }
    std::size_t h = 1469598103u;
    for (const auto x : v) h = (h ^ static_cast<std::size_t>(x)) * 1099511628211ull;
    return "count=" + std::to_string(v.size()) + " hash=" + std::to_string(h) + " first=[" + std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' + std::to_string(v[2]) + "] last=[" +
           std::to_string(v[v.size() - 3]) + ',' + std::to_string(v[v.size() - 2]) + ',' + std::to_string(v[v.size() - 1]) + ']';
}

// The full decision log of one plan. `full` adds the per-candidate / per-mode / per-day detail (DEBUG-level content).
inline std::vector<std::string> DescribeDealOptimizer(const UtilitySnapshot& s, const OptimizerConfig& cfg, const UtilityTuning& t, const UnifiedPlan& u, std::uint32_t planId, bool full) {
    std::vector<std::string> out;
    const std::string id = "plan=" + std::to_string(planId) + " decisionId=" + DecisionId(s, planId);
    const auto calendar = CalendarFromDayOfYear(static_cast<int>(s.simDayOfYear));
    const int daysLeft = calendar.daysInMonth - calendar.dayOfMonth + 1;
    const auto num = [](const auto& o) { return o ? std::to_string(static_cast<long long>(*o)) : std::string("?"); };
    {
        std::ostringstream o;
        o << "[DealOptimizer][Plan] " << id << " epoch=" << s.lifecycleEpoch << " simYear=" << s.simYear << " simDayOfYear=" << s.simDayOfYear << " dayOfMonth=" << calendar.dayOfMonth << " daysInMonth=" << calendar.daysInMonth
          << " daysLeft=" << daysLeft;
        out.push_back(o.str());
    }
    // ---- Garbage snapshot and per-contract state
    {
        const auto base = GarbageBaseInput(s, cfg, t);
        const auto& a = s.garbage.aggregate;
        std::ostringstream o;
        o << "[Garbage][Snapshot] " << id << " year=" << s.simYear << " day=" << s.simDayOfYear << " dayOfMonth=" << calendar.dayOfMonth << " daysLeft=" << daysLeft << " Produced=" << num(a.produced) << " Recycled=" << num(a.recycled)
          << " ownLoad=" << base.noDealLoad << " reservePercent=" << ReservePercentFromFactor(base.input.reserve) << " protectedOwn=" << CeilTarget(static_cast<double>(base.noDealLoad) * base.input.reserve)
          << " backlog=" << base.input.backlog << " Abducted=" << num(a.abducted) << " fundingCurrent=" << (s.garbage.department.funding ? std::to_string(UtilityPercentRound(*s.garbage.department.funding)) : std::string("?"))
          << " fundingEfficiency=" << (a.fundingEfficiency ? std::to_string(*a.fundingEfficiency) : std::string("?")) << " incineratorCapacity=" << num(a.incineratorCapacityMonthly) << " w2eCapacity=" << num(a.w2eCapacityMonthly)
          << " landfillTiles=" << num(a.landfillTiles) << " landfillSupply=" << num(a.garbageSupply) << " landfillFreeStorage="
          << ((a.landfillCapacity && a.landfillUsage) ? std::to_string(static_cast<long long>(*a.landfillCapacity) - static_cast<long long>(*a.landfillUsage)) : std::string("?"))
          << " landfillValveHeadroom=" << num(ComputeLandfillValve(a, base.input.transportCurve).headroomNow) << " historicalImportedCounter=" << num(a.imported) << " historicalExportedCounter=" << num(a.exported);
        out.push_back(o.str());
        for (const auto& r : s.deals.deals) {
            if (r.type != utility_id::kDealImportGarbage && r.type != utility_id::kDealExportGarbage) continue;
            const bool imp = r.type == utility_id::kDealImportGarbage;
            const bool committed = DealIsCommitted(r);
            const bool suspended = r.flags && (*r.flags & deal_flag::kSuspensionMask);
            std::ostringstream d;
            d << "[Garbage][DealState] " << id << " commodity=GARBAGE type=" << DealTypeName(r.type) << " neighbor=" << (r.otherCitySerial ? std::to_string(*r.otherCitySerial) : std::string("?")) << " flags=0x" << std::hex
              << (r.flags ? *r.flags : 0) << std::dec << " active=" << (committed ? "YES" : "NO") << " suspended=" << (suspended ? "YES" : "NO") << " fixed=" << (suspended ? "YES" : "NO") << " Amount=" << num(r.amount)
              << " AmountUsed=" << (r.amountUsed ? std::to_string(*r.amountUsed) : std::string("UNKNOWN"));
            if (r.amount && r.amountUsed) {
                const std::int64_t target = *r.amount, used = *r.amountUsed;
                const auto stream = imp ? ImportStream(target, used) : ExportStream(target, used, true, suspended);
                d << ' ' << (imp ? "remainingFutureLoad=" : "remainingFutureRelief=") << StreamsDelivery({stream}, daysLeft) << " dailyCap=" << DealDailyCap(target) << " completedThisMonth=" << (used >= target && target > 0 ? "YES" : "NO");
            } else {
                d << (imp ? " remainingFutureLoad=UNKNOWN" : " remainingFutureRelief=UNKNOWN") << " dailyCap=" << (r.amount ? std::to_string(DealDailyCap(*r.amount)) : std::string("?")) << " completedThisMonth=UNKNOWN";
            }
            d << " storedMonthlyCost=" << num(r.cost);
            out.push_back(d.str());
        }
    }
    if (full)
        for (const auto& n : u.portfolio.notes)
            if (n.rfind("[DealOptimizer][Rejected]", 0) == 0) out.push_back(n + " " + id);
    // ---- candidates, modes, chosen portfolio, counterfactual, search
    for (std::size_t ci = 0; ci < u.portfolio.commodities.size(); ++ci) {
        const auto& r = u.portfolio.commodities[ci];
        const std::string com = CommodityName(ci == 0 ? Commodity::Power : ci == 1 ? Commodity::Water : Commodity::Garbage);
        if (full)
            for (const auto& c : r.candidateDiag) {
                std::ostringstream o;
                o << "[DealOptimizer][Candidate] " << id << " commodity=" << com << " type=" << DealTypeName(c.key.type) << " neighbor=" << c.key.neighbor << " currentAmount=" << c.current << " currentAmountUsed="
                  << (c.amountUsed ? std::to_string(*c.amountUsed) : std::string(c.existing ? "UNKNOWN" : "none")) << " fixed=" << (c.fixed ? "YES" : "NO") << " suspended=" << (c.fixed ? "YES" : "NO")
                  << " completedThisMonth=" << (c.completed ? "YES" : "NO") << " ladderStep=" << c.ladderStep << " ladderMax=" << c.ladderMax << " legalOptions=" << OptionList(c.options)
                  << " lockedBy=" << (c.lockedBy.empty() ? "none" : c.lockedBy);
                out.push_back(o.str());
            }
        if (full)
            for (const auto& m : r.modes) {
                std::ostringstream o;
                o << "[DealOptimizer][ModeResult] " << id << " commodity=" << com << " mode=" << m.mode << " legal=" << (m.legal ? "YES" : "NO") << " feasible=" << (m.feasible ? "YES" : "NO") << " economicNet=" << m.economicNet
                  << " cashNet=" << m.cashNet << " localExpense=" << m.localExpense << " dealIncome=" << m.dealIncome << " dealExpense=" << m.dealExpense << " dealCount=" << m.dealCount << " changes=" << m.changes
                  << " reasonIfRejected=" << (m.reasonIfRejected.empty() ? "none" : m.reasonIfRejected);
                if (ci == 2 && u.garbage.dualHorizon) o << " H1Feasible=" << (m.feasible ? "YES" : "NO") << " H2Feasible=" << (m.feasible ? "YES" : "NO") << " H2EconomicNet=" << m.economicNet;   // a legal mode is feasible on both
                out.push_back(o.str());
            }
        if (!r.evaluated && r.note.empty()) continue;
        {
            std::ostringstream o;
            o << "[DealOptimizer][ChosenPortfolio] " << id << " commodity=" << com << " evaluated=" << (r.evaluated ? "YES" : "NO") << " mode=" << r.mode << " requiredLocalNeed=" << r.requiredNeed << " localCapacityNow=" << r.localCapacityNow << " economicNet=" << r.optimumNet << " cashNet=" << r.optimumCashNet
              << " keepFeasible=" << (r.keepFeasible ? "YES" : "NO") << " keepNet=" << r.keepNet;
            if (ci == 2) {
                const auto& sv = u.garbage.serviceAfter;
                o << " fundingTarget=" << u.garbage.targetPercent << " currentMonthFeasible=" << (sv.capacitySufficient ? "YES" : "NO") << " projectedPeakBacklog=" << sv.projectedPeakBacklog
                  << " projectedEndMonthBacklog=" << sv.projectedEndBacklog;
            }
            if (ci == 2) {
                if (u.garbage.dualHorizon) o << " decisionBasis=DUAL_HORIZON H1Feasible=" << (u.garbage.serviceAfter.capacitySufficient ? "YES" : "NO") << " H2Feasible=" << (u.garbage.serviceAfter.h2Feasible ? "YES" : "NO")
                                              << " H2EconomicNet=" << r.optimumNet;
                else o << " decisionBasis=CONTINUITY_FALLBACK calibrationState=" << LandfillGeometryStateName(s.garbage.geometry.state);
            }
            o << " searchNodes=" << r.searchNodes << " searchPruned=" << r.searchPruned << " searchEvaluated=" << r.searchEvaluated << " optimumProven=" << (r.optimumProven ? "YES" : "NO");
            if (!r.note.empty()) o << " note=" << r.note;
            if (!r.policyConflict.empty()) o << " " << r.policyConflict;
            out.push_back(o.str());
        }
        {
            std::ostringstream o;
            o << "[DealOptimizer][Search] " << id << " commodity=" << com << " theoreticalCompositions=" << static_cast<double>(r.theoreticalCompositions) << " searchNodes=" << r.searchNodes << " searchPruned=" << r.searchPruned
              << " searchEvaluated=" << r.searchEvaluated << " proofLimit=5000000 optimumProven=" << (r.optimumProven ? "YES" : "NO") << (r.optimumProven ? "" : " OPTIMUM_NOT_PROVEN");
            out.push_back(o.str());
        }
        for (const auto& c : r.continuity) out.push_back("[Garbage][Continuity] " + id + " " + c);
        for (const auto& d : r.decisions) {
            if (d.from == d.to && !full) continue;
            const std::string who = "commodity=" + com + " type=" + DealTypeName(d.key.type) + " neighbor=" + std::to_string(d.key.neighbor);
            std::ostringstream o;
            std::optional<std::int64_t> used;
            for (const auto& cd : r.candidateDiag) if (cd.key == d.key) used = cd.amountUsed;
            o << "[DealOptimizer][ChosenDeal] " << id << ' ' << who << " from=" << d.from << " to=" << d.to << " AmountUsed=" << (used ? std::to_string(*used) : std::string("UNKNOWN")) << " remainingCurrentMonth="
              << (d.to > 0 && used ? StreamsDelivery({d.sellLike ? ImportStream(d.to, *used) : ExportStream(d.to, *used, true, false)}, daysLeft) : 0) << " moneyFrom=" << d.fromMoney << " moneyTo=" << d.toMoney;
            out.push_back(o.str());
            std::ostringstream m;
            m << "[DealOptimizer][Marginal] " << id << ' ' << who << " from=" << d.from << " to=" << d.to << " optimumNet=" << r.optimumNet << " counterfactualConstraint=currentAmount counterfactualLegal="
              << (d.counterfactualLegal ? "YES" : "NO") << " counterfactualFeasible=" << (d.counterfactualLegal ? "YES" : "NO") << " counterfactualBestMode=" << (d.counterfactualLegal ? d.counterfactualMode : std::string("none"))
              << " counterfactualBestNet=" << (d.counterfactualLegal ? std::to_string(d.counterfactualNet) : std::string("none")) << " economicMarginal=" << d.marginal << " cashMarginal=" << d.cashMarginal << " marginalKnown="
              << (d.marginalKnown ? "YES" : "NO") << " requiredReason=" << (d.requiredReason.empty() ? "none" : d.requiredReason) << (d.counterfactualBeatOptimum ? " counterfactualBeatOptimum=YES(clamped to 0)" : "");
            out.push_back(m.str());
        }
    }
    // ---- Garbage horizon + the day-by-day projection of the chosen plan
    {
        auto base = GarbageBaseInput(s, cfg, t);
        ApplyGarbageTargets(base.input, u.garbageTargets);
        base.input.traceDays = true;
        const double eff = base.input.fundingCurve.Eval(u.garbage.targetPercent) / 100.0;
        const auto sv = EvaluateGarbageServiceAt(base.input, eff, base.input.DualHorizonReady() ? u.garbage.targetPercent : -1);
        bool anyCompleted = false;
        for (const auto& c : u.portfolio.commodities.size() > 2 ? u.portfolio.commodities[2].candidateDiag : std::vector<PortfolioCommodityResult::CandidateDiag>{}) anyCompleted = anyCompleted || c.completed;
        bool blocked = false;
        if (u.portfolio.commodities.size() > 2) for (const auto& c : u.portfolio.commodities[2].continuity) blocked = blocked || c.find("ROTATION_BLOCKED") != std::string::npos;
        out.push_back("[Garbage][Horizon] " + id + " currentMonthFeasible=" + (sv.capacitySufficient ? "YES" : "NO") + " currentMonthEndBacklog=" + std::to_string(sv.projectedEndBacklog) +
                      " nextMonthEvaluated=NO (the landfill rollover of GarbageSupply is not decoded: no future physics is invented) continuityRule=PRESERVE_COMPLETED_DEAL_UNTIL_ROLLOVER completedContractsPresent=" +
                      (anyCompleted ? "YES" : "NO") + " churnBlocked=" + (blocked ? "YES" : "NO") + " churnReason=" + (blocked ? "AMOUNT_USED_EXHAUSTION_ONLY" : "none"));
        {   // [Garbage][DualHorizon]: H1 (rest of the month, safety) -> the proven boundary -> H2 (next full month, economics)
            const auto& geo = s.garbage.geometry;
            std::ostringstream d;
            d << "[Garbage][DualHorizon] " << id << " calibrationState=" << LandfillGeometryStateName(geo.state) << " N=" << (geo.calibrated() ? std::to_string(geo.activeCells) : std::string("?")) << " C="
              << (geo.calibrated() ? std::to_string(geo.cellCapacity) : std::string("?"));
            if (!u.garbage.dualHorizon || !sv.dualActive) {
                d << " DUAL_HORIZON_OFF decisionBasis=CONTINUITY_FALLBACK continuityLock=ON";
            } else {
                std::ostringstream targets;
                for (const auto& t : u.garbageTargets) targets << (targets.tellp() > 0 ? "," : "") << DealTypeName(t.key.type) << '/' << t.key.neighbor << '=' << t.amount;
                std::int64_t income = 0, expense = 0;
                if (u.portfolio.commodities.size() > 2)
                    for (const auto& dd : u.portfolio.commodities[2].decisions) if (dd.to > 0) (dd.sellLike ? income : expense) += dd.toMoney;
                const auto& a = base.input.aggregate;
                d << " H1{day=" << calendar.dayOfMonth << " daysRemaining=" << daysLeft << " startUsage=" << num(a.landfillUsage) << " startSupply=" << num(a.garbageSupply) << " endUsageBeforeBoundary=" << num(a.landfillUsage)
                  << " endSupplyBeforeBoundary=" << sv.h1EndSupply << " endBacklog=" << sv.projectedEndBacklog << " feasible=" << (sv.capacitySufficient ? "YES" : "NO") << "} Boundary{decay=" << sv.boundaryDecay
                  << " fundingRatio=" << sv.ratioA << '/' << sv.ratioB << " handed=" << sv.boundaryHanded << " absorbed=" << sv.boundaryAbsorbed << " nextUsage=" << sv.nextUsage << " nextFree=" << sv.nextFree
                  << " nextSupply=" << sv.nextSupply << "} H2{targetDeals=[" << targets.str() << "] startBacklog=0 projectedPeakBacklog=" << sv.h2PeakBacklog << " projectedEndBacklog=" << sv.h2EndBacklog << " sanitationFunding="
                  << u.garbage.targetPercent << "% steadyStateFunding=" << u.garbage.steadyStatePercent << "% localExpense=" << u.garbage.steadyStateExpense << " dealIncome=" << income << " dealExpense=" << expense
                  << " economicNet=" << (income - expense - u.garbage.steadyStateExpense) << " feasible=" << (sv.h2Feasible ? "YES" : "NO") << '}';
            }
            out.push_back(d.str());
        }
        bool deals = false;
        for (const auto& f : base.input.deals) deals = deals || f.target > 0 || f.amount > 0;
        const bool interesting = full || sv.projectedPeakBacklog > 0;
        if (deals && interesting)
            for (const auto& d : sv.dayProjection) {
                std::ostringstream o;
                o << "[Garbage][DayProjection] " << id << " day=" << calendar.dayOfMonth + d.day - 1 << " ownGenerated=" << static_cast<long long>(d.ownGenerated + 0.5) << " importDelivery=" << static_cast<long long>(d.importDelivery)
                  << " exportCreditAccrued=" << static_cast<long long>(d.exportCreditAccrued) << " exportCreditUsed=" << static_cast<long long>(d.exportCreditUsed + 0.5) << " buildingDisposed=" << static_cast<long long>(d.buildingDisposed + 0.5)
                  << " landfillDisposed=" << static_cast<long long>(d.landfillDisposed + 0.5) << " backlogEnd=" << static_cast<long long>(d.backlogEnd + 0.5) << " imports={";
                bool first = true;
                for (const auto& [n, v] : d.importBy) { o << (first ? "" : ",") << n << ':' << v; first = false; }
                o << "} exports={";
                first = true;
                for (const auto& [n, v] : d.exportBy) { o << (first ? "" : ",") << n << ':' << v; first = false; }
                o << '}';
                out.push_back(o.str());
            }
    }
    return out;
}

} // namespace sc4af
