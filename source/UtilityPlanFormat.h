// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Decision-oriented text for the Shadow log (pure). No candidate tables here: those are debug-only.

#include "UtilityOptimizer.h"

#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace sc4af {

inline std::string FormatActions(const SubsystemPlan& p) {
    if (p.actions.empty()) return "none";
    std::ostringstream o;
    for (std::size_t i = 0; i < p.actions.size(); ++i) {
        const auto& a = p.actions[i];
        o << (i ? "; " : "");
        if (a.kind == ActionKind::SetPlantFunding)
            o << "plant@" << a.cellX << ',' << a.cellZ << ' ' << a.fromPercent << "%->" << a.toPercent << "% (" << (a.monthlyDelta >= 0 ? "+" : "") << a.monthlyDelta << ")";
        else if (a.kind == ActionKind::SetWaterFunding)
            o << "water-dept " << a.fromPercent << "%->" << a.toPercent << "% (" << (a.monthlyDelta >= 0 ? "+" : "") << a.monthlyDelta << ")";
        else
            o << DealTypeName(a.dealType) << " neighbor=" << (a.neighbor ? std::to_string(*a.neighbor) : "?") << " amount=" << a.amount << " value=+" << a.monthlyDelta
              << " note=\"" << a.note << '"';
        o << " [" << CapabilityName(a.capability) << "]";
    }
    return o.str();
}

inline std::string FormatSubsystem(const char* name, const SubsystemPlan& p) {
    std::ostringstream o;
    o << "[UtilityOptimizer][" << name << "] status=" << PlanStatusName(p.status) << " currentExpense=" << p.currentExpense
      << " targetExpense=" << p.targetExpense << " saving=" << p.saving() << " required=" << p.required << " availableNow=" << p.availableNow
      << " availableAfter=" << p.availableAfter << " actions=" << FormatActions(p);
    // identical notes (e.g. several buy deals expiring on the same day) are folded into one with a count
    std::vector<std::pair<std::string, int>> folded;
    for (const auto& n : p.notes) {
        auto it = std::find_if(folded.begin(), folded.end(), [&](const auto& e) { return e.first == n; });
        if (it == folded.end()) folded.emplace_back(n, 1); else ++it->second;
    }
    for (const auto& [n, count] : folded) o << " note=\"" << n << (count > 1 ? " (x" + std::to_string(count) + ")" : "") << '"';
    return o.str();
}

// How the power target was composed, including HYPOTHETICAL export economics (never booked as realized income).
inline std::string FormatPowerTarget(const SubsystemPlan& p) {
    std::ostringstream o;
    o << "[UtilityOptimizer][PowerTarget] ownTarget=" << p.ownTarget << " contractedExports=" << p.contractedExports << " dependableImports=" << p.dependableImports
      << " exportReserve=" << p.exportReserve << " => requiredLocalCapacity=" << p.required << " | HYPOTHETICAL exportRevenue=" << p.exportRevenue
      << " extraExpense=" << p.exportExtraExpense << " net=" << (p.exportRevenue - p.exportExtraExpense) << " picks=";
    if (p.exportPicks.empty()) o << "none";
    for (std::size_t i = 0; i < p.exportPicks.size(); ++i)
        o << (i ? "," : "") << (p.exportPicks[i].visible ? "VISIBLE" : "LATENT") << '@' << p.exportPicks[i].serial << ':' << p.exportPicks[i].amount << '=' << p.exportPicks[i].revenue;
    o << " frontier=";
    if (p.exportFrontier.empty()) o << "none";
    std::size_t shown = 0;
    for (const auto& f : p.exportFrontier) {
        if (shown++ >= 8) { o << ",..."; break; }
        o << (shown > 1 ? "," : "") << f.serial << ':' << f.amount << "/rev" << f.revenue << "/cost" << f.extraExpense << "/net" << f.net << (f.feasible ? "" : "/infeasible");
    }
    return o.str();
}

inline std::string FormatTransition(const char* name, const TransitionSummary& t) {
    std::ostringstream o;
    o << "[UtilityOptimizer][Transition][" << name << "] changes=" << t.changes << " maxPointsMoved=" << t.maxPointsMoved << " capacity=" << t.capacityBefore
      << "->" << t.capacityAfter << " (" << t.capacityDeltaPercent << "%) expenseDelta=" << t.expenseDelta;
    if (t.departmentFundingBefore && t.departmentFundingAfter)
        o << " departmentFunding=" << *t.departmentFundingBefore << "%->" << *t.departmentFundingAfter << "%";
    return o.str();
}

inline std::string FormatGarbage(const UtilityPlan& plan) {
    std::ostringstream o;
    o << "[UtilityOptimizer][Garbage][ACCOUNTING_BALANCE] inputs=" << (plan.garbage.inputs ? std::to_string(*plan.garbage.inputs) : "?")
      << " outputs=" << (plan.garbage.outputs ? std::to_string(*plan.garbage.outputs) : "?")
      << " deltaStock=" << (plan.garbage.deltaStock ? std::to_string(*plan.garbage.deltaStock) : "?") << " residual="
      << (plan.garbage.unexplainedResidual ? std::to_string(*plan.garbage.unexplainedResidual) : "?");
    if (plan.garbage.residualFraction) o << " (" << *plan.garbage.residualFraction * 100.0 << "%)";
    o << " serviceShortage=" << (plan.garbage.serviceShortage ? "YES" : "no");
    return o.str();
}

inline std::string FormatGarbageHandling(const UtilityPlan& plan, double reserve) {
    const auto& h = plan.garbageHandling;
    std::ostringstream o;
    o << "[UtilityOptimizer][Garbage][SERVICE_SUFFICIENCY] READ_ONLY handlingReserve=" << reserve << " baseDisposalLoad=" << (h.baseDisposalLoad ? std::to_string(*h.baseDisposalLoad) : "?")
      << " requiredHandling=" << (h.requiredHandling ? std::to_string(*h.requiredHandling) : "?") << " handling=" << h.handling << " ratio="
      << (h.ratio ? std::to_string(*h.ratio) : "?") << " meetsReserve=" << (h.meetsReserve ? "yes" : "NO") << " serviceShortage=" << (h.serviceShortage ? "YES" : "no")
      << " landfillIntake=" << (h.landfillIntakeUnknown ? "UNKNOWN" : "known") << " unexplainedResidual="
      << (h.unexplainedResidual ? std::to_string(*h.unexplainedResidual) : "?") << " basis=\"" << h.basis << '"';
    return o.str();
}

inline std::string FormatGarbageFunding(const UtilityPlan& plan) {
    const auto& g = plan.garbageFunding;
    const auto& n = g.serviceNow;
    std::ostringstream o;
    o << "[Garbage][LEGACY_STANDALONE_DIAGNOSTIC] NOT_AUTO_AUTHORITY (the current contracts only, no portfolio: the unified plan decides) READ_ONLY status=" << GarbagePlanStatusName(g.status) << " funding=" << g.currentPercent << "%->" << g.targetPercent
      << "% expense=" << g.currentExpense << "->" << g.targetExpense << " load=" << (n.baseDisposalLoad ? std::to_string(*n.baseDisposalLoad) : "?")
      << " required=" << (n.requiredHandling ? std::to_string(*n.requiredHandling) : "?") << " fixedHandling=" << n.fixedHandling
      << " landfillNeedRemaining=" << n.landfillNeedRemaining << " landfillHeadroomNow=" << (n.landfillHeadroomNow ? std::to_string(*n.landfillHeadroomNow) : "?")
      << " capacitySufficient=" << (n.capacitySufficient ? "yes" : "NO") << " serviceShortage=" << (n.serviceShortage ? "YES" : "no");
    if (!g.note.empty()) o << " note=\"" << g.note << '"';
    return o.str();
}

inline std::string FormatDeals(const UtilityPlan& plan, std::size_t opportunities) {
    std::ostringstream o;
    o << "[UtilityOptimizer][Deals] READ_ONLY status=" << PlanStatusName(plan.deals.status) << " opportunities=" << opportunities
      << " recommendations=" << FormatActions(plan.deals);
    for (const auto& n : plan.deals.notes) o << " note=\"" << n << '"';
    return o.str();
}

// Flags a recommendation that flips between two consecutive months while the real state did not change.
class PlanStability {
public:
    void Reset() { last_.clear(); }
    // key -> recommended target percent; state -> current percent. Returns human text for every flip.
    std::vector<std::string> Observe(const std::map<std::pair<int, int>, std::pair<int, int>>& targetAndCurrent) {
        std::vector<std::string> flips;
        for (const auto& [key, tc] : targetAndCurrent) {
            const auto it = last_.find(key);
            if (it != last_.end() && it->second.second == tc.second && it->second.first != tc.first)
                flips.push_back("FLIP plant@" + std::to_string(key.first) + ',' + std::to_string(key.second) + " target " +
                                std::to_string(it->second.first) + "% -> " + std::to_string(tc.first) + "% with unchanged state");
        }
        last_ = targetAndCurrent;
        return flips;
    }

private:
    std::map<std::pair<int, int>, std::pair<int, int>> last_;
};

} // namespace sc4af
