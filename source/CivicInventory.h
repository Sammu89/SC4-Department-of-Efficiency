// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

// Pure inventory-diff logic (no SC4 dependency).
namespace sc4af {

struct InventoryEntry {
    std::uintptr_t key = 0;   // session-local runtime address; never persisted
    std::string list;         // "School" or "Hospital" (the simulator list containing it)
    std::string label;        // human-readable, e.g. exemplar name
    std::string signature;    // everything that should trigger a "changed" report (not usage)
};

struct InventoryDiff {
    std::vector<InventoryEntry> added;
    std::vector<InventoryEntry> removed;
    std::vector<std::pair<InventoryEntry, InventoryEntry>> changed; // (previous, current)
    bool empty() const { return added.empty() && removed.empty() && changed.empty(); }
};

inline InventoryDiff DiffInventory(const std::vector<InventoryEntry>& previous,
                                   const std::vector<InventoryEntry>& current) {
    InventoryDiff diff;
    const auto same = [](const InventoryEntry& a, const InventoryEntry& b) {
        return a.key == b.key && a.list == b.list;
    };
    for (const auto& now : current) {
        const auto it = std::find_if(previous.begin(), previous.end(),
                                     [&](const InventoryEntry& p) { return same(p, now); });
        if (it == previous.end()) diff.added.push_back(now);
        else if (it->signature != now.signature) diff.changed.emplace_back(*it, now);
    }
    for (const auto& before : previous) {
        const auto it = std::find_if(current.begin(), current.end(),
                                     [&](const InventoryEntry& c) { return same(before, c); });
        if (it == current.end()) diff.removed.push_back(before);
    }
    return diff;
}

// Debounce/poll policy for the refresh timer.
struct RefreshPolicy {
    std::uint64_t debounceMs = 750;

    // dirtySinceMs == 0 means "not dirty".
    bool ShouldRefresh(bool countsChanged, std::uint64_t dirtySinceMs, std::uint64_t nowMs) const {
        if (countsChanged) return true;
        return dirtySinceMs != 0 && nowMs >= dirtySinceMs && nowMs - dirtySinceMs >= debounceMs;
    }
};

} // namespace sc4af
