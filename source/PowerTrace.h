// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Compact per-day power trace line (diagnostic, read-only). Pure formatting.
//   [PowerTrace] d=<year>/<dayOfYear> demand= req= cons= prod= eff= used= impUsed= impContr= expContr= unpow= plants= |
//   opp=<type>@<neighbor>:max/cost1000,... | conn=<neighbor>:sellMax/buyMax/sellCost1000,...

#include "UtilityModel.h"
#include "UtilityOptimizer.h"

#include <sstream>
#include <string>

namespace sc4af {

inline std::string FormatPowerTraceLine(const UtilitySnapshot& s) {
    const auto num = [](const auto& v) { return v ? std::to_string(static_cast<long long>(*v)) : std::string("?"); };
    const auto& a = s.power.aggregate;
    std::int64_t eff = 0, used = 0;
    for (const auto& p : s.power.plants) {
        if (p.produced) eff += *p.produced;
        if (p.used) used += *p.used;
    }
    std::ostringstream o;
    o << "[PowerTrace] d=" << s.simYear << '/' << s.simDayOfYear << " demand=" << num(a.demand) << " req=" << num(a.requirement)
      << " cons=" << num(a.consumption) << " prod=" << num(a.production) << " eff=" << eff << " used=" << used << " impUsed=" << num(a.import_)
      << " impContr=" << num(s.power.gameImported) << " expContr=" << num(s.power.gameExported) << " unpow=" << num(a.unpowered)
      << " plants=" << s.power.plants.size();
    {
        const auto& w = s.water.aggregate;
        o << " | water wdem=" << num(w.demand) << " wcap=" << num(w.productionCapacity) << " wcons=" << num(w.consumed) << " wimp=" << num(s.water.gameImported)
          << " wexp=" << num(s.water.gameExported) << " unwat=" << num(w.unwateredBuildings) << " wfund=" << (s.water.department.funding ? std::to_string(static_cast<int>(*s.water.department.funding * 100.0f + 0.5f)) : "?");
    }
    o << " |";
    o << " opp=";
    bool first = true;
    for (const auto& op : s.deals.opportunities) {
        if (op.type != utility_id::kDealSellPower && op.type != utility_id::kDealBuyPower) continue;
        o << (first ? "" : ",") << (op.type == utility_id::kDealSellPower ? "SELL" : "BUY") << '@' << num(op.otherCitySerial) << ':' << num(op.maxAmount) << '/'
          << (op.quotes.empty() ? std::string("?") : std::to_string(op.quotes.front().second));
        first = false;
    }
    if (first) o << (s.deals.proposalsRead ? "none" : "NOT_READ");   // proposals not asked for (AutoDeals off) are unknown, not "none"
    o << " | conn=";
    first = true;
    for (const auto& c : s.deals.connections) {
        if (c.carries != Commodity::Power) continue;
        o << (first ? "" : ",") << num(c.otherCitySerial) << ':' << num(c.sellMax) << '/' << num(c.buyMax) << '/' << num(c.sellCostAtMin);
        for (const auto& q : c.sellQuotes) o << (&q == &c.sellQuotes.front() ? "[" : ";") << q.first << '=' << q.second;
        if (!c.sellQuotes.empty()) o << ']';
        // pure reproduction of the vanilla inputs next to the game's own answer (divergence = the model is wrong)
        if (c.nbStats && s.deals.localPowerStats) {
            const auto t = SellTuningOf(s.deals);
            const auto r = EvaluateLatentSell(*s.deals.localPowerStats, *c.nbStats, c.nbBudget ? *c.nbBudget : 0.0,
                                               c.sellCostAtMin ? static_cast<double>(*c.sellCostAtMin) : 0.0, t);
            o << "{calcMax=" << static_cast<long long>(r.gameMax) << " need=" << static_cast<long long>(r.buyerNeed) << " surplus=" << static_cast<long long>(r.sellerSurplus)
              << " budget=" << (c.nbBudget ? static_cast<long long>(*c.nbBudget) : -1) << " extra=" << static_cast<long long>(r.extraSurplusNeeded)
              << (r.eligibleNow ? " ELIGIBLE" : r.everEligible ? " LATENT" : " BLOCKED_BUYER") << '}';
        }
        first = false;
    }
    if (first) o << "none";
    return o.str();
}

} // namespace sc4af
