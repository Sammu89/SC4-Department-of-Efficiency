// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// The exogenous inputs a deal decision was planned from (simulation day, AmountUsed of the contracts, the commodity's physical state, the base market of
// the neighbor ...). NEVER the controlled amount of the slot itself. Equal fields = same causal state; an empty set = unknown (fail closed).

#include <functional>
#include <map>
#include <string>

namespace sc4af {

struct DealCausal {
    std::map<std::string, std::string> fields;
    bool empty() const { return fields.empty(); }
    std::string Text() const {
        std::string o;
        for (const auto& [k, v] : fields) o += (o.empty() ? "" : ",") + k + "=" + v;
        return o;
    }
    // The EXOGENOUS part, for the stability checks: what the world did, never what our own plan did. Excluded: the calendar by itself (its real effects show in the physical fields),
    // the funding we set and the efficiency that follows from it, and every contract's state (amount and AmountUsed are consequences of our own writes and of elapsed days).
    // Without this the same import can flip 0 -> N -> 0 on consecutive days when the only differing fields are `day` and the slot's own AmountUsed.
    DealCausal Exogenous() const {
        DealCausal out;
        for (const auto& [k, v] : fields) {
            if (k == "day" || k == "funding" || k == "efficiency" || k.rfind("used:", 0) == 0 || k.rfind("amount:", 0) == 0) continue;
            out.fields[k] = v;
        }
        return out;
    }
    std::string Hash() const { return std::to_string(std::hash<std::string>{}(Text())); }
    // names of the fields that differ (present in one side only counts as different)
    static std::string Changed(const DealCausal& a, const DealCausal& b) {
        std::string o;
        const auto add = [&](const std::string& k) { o += (o.empty() ? "" : ",") + k; };
        for (const auto& [k, v] : a.fields) { const auto it = b.fields.find(k); if (it == b.fields.end() || it->second != v) add(k); }
        for (const auto& [k, v] : b.fields) { (void)v; if (a.fields.find(k) == a.fields.end()) add(k); }
        return o;
    }
};

} // namespace sc4af
