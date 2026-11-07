// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Neighbor-deal diagnostic summary (pure formatting). One question only: is this deal currently net-beneficial for
// the city, and by how much per month? The internals (reserve, plant-by-plant funding, solver) appear only in Debug mode.
//
//   SELL POWER - 12,000            BUY POWER - 5,000
//   Revenue       +§2,100/month    Deal cost       §600/month
//   Extra cost      §500/month     Local saving    §850/month
//   Profit        +§1,600/month    Saving         +§250/month
//   PROFITABLE                     PROFITABLE
//
// Profit = deal revenue - INCREMENTAL cost of the cheapest safe utility state with the deal versus without it (never the whole
// utility budget). Nothing here is realized income: cards are diagnostics only.

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

namespace sc4af {

enum class DealVerdict { Profitable, NotProfitable, Neutral, CannotCalculate };

inline const char* DealVerdictName(DealVerdict v) {
    switch (v) {
    case DealVerdict::Profitable: return "PROFITABLE";
    case DealVerdict::NotProfitable: return "NOT PROFITABLE";
    case DealVerdict::Neutral: return "NEUTRAL";
    case DealVerdict::CannotCalculate: return "CANNOT CALCULATE";
    }
    return "?";
}

struct DealCard {
    bool sell = true;                  // false = buy
    std::string resource = "POWER";
    std::uint32_t neighbor = 0;
    bool visible = true;               // false: latent (would become available once capacity is held)
    std::int64_t amount = 0;
    bool calculable = false;
    std::int64_t deal = 0;             // sell: revenue; buy: deal cost
    std::int64_t utility = 0;          // sell: extra plant cost to make it available; buy: local saving
    std::int64_t profit = 0;           // sell: deal - utility; buy: utility - deal
    std::string internals;             // Debug only

    DealVerdict verdict() const {
        if (!calculable) return DealVerdict::CannotCalculate;
        return profit > 0 ? DealVerdict::Profitable : profit < 0 ? DealVerdict::NotProfitable : DealVerdict::Neutral;
    }
};

inline std::string GroupThousands(std::int64_t v) {
    std::string digits = std::to_string(v < 0 ? -v : v);
    std::string out;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (i && (digits.size() - i) % 3 == 0) out += ',';
        out += digits[i];
    }
    return v < 0 ? "-" + out : out;
}

inline std::string Money(std::int64_t v, bool signedPlus) {
    const std::string s = "\xC2\xA7" + GroupThousands(v < 0 ? -v : v) + "/month";
    return (v < 0 ? "-" : signedPlus ? "+" : "") + s;
}

inline std::string FormatDealCard(const DealCard& c, bool debug) {
    std::string out = std::string(c.sell ? "SELL " : "BUY ") + c.resource + " - " + GroupThousands(c.amount);
    if (!c.calculable) return out + " | CANNOT CALCULATE";
    if (c.sell) {
        out += " | Revenue " + Money(c.deal, true) + " | Extra cost " + Money(c.utility, false);
        out += c.profit >= 0 ? " | Profit " + Money(c.profit, true) : " | Loss " + Money(-c.profit, false);
    } else {
        out += " | Deal cost " + Money(c.deal, false) + " | Local saving " + Money(c.utility, false);
        out += c.profit >= 0 ? " | Saving " + Money(c.profit, true) : " | Extra cost " + Money(-c.profit, false);
    }
    out += std::string(" | ") + DealVerdictName(c.verdict());
    if (debug) out += std::string(" | neighbor=") + std::to_string(c.neighbor) + (c.visible ? " visible" : " latent") + (c.internals.empty() ? "" : " | " + c.internals);
    return out;
}

} // namespace sc4af
