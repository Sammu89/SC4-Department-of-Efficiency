// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Memory of the shortages already announced to the player, kept outside the saves (DepartmentEfficiency.told.txt beside the plugin). One notice per episode: an unresolved shortage
// that is proven again when the same city is reopened belongs to the same episode and is not announced again. The mark is forgotten once a session runs well past the persistence
// window without the shortage being proven again, so a later shortage starts a new episode. Pure (no game types).

#include <cstdint>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {

// Region directory + city serial + position in the region. Empty when any part is unknown: the city is not identified, so there is no memory and the notice follows the session rule only.
inline std::string BuildNoticeCityKey(const std::string& regionDirectory, std::uint32_t serial, bool positionKnown, std::int32_t x, std::int32_t z) {
    if (regionDirectory.empty() || serial == 0 || !positionKnown) return {};
    for (const char c : regionDirectory) if (c == '\n' || c == '\r' || c == '#') return {};
    return regionDirectory + "|" + std::to_string(serial) + "|" + std::to_string(x) + "," + std::to_string(z);
}

// Ids as in DeferredNotices: 0 power, 1 water, 2 garbage, 3 schools, 4 hospitals.
inline std::string NoticeKey(const std::string& cityKey, int id) { return cityKey.empty() ? std::string() : cityKey + "#" + std::to_string(id); }

// A mark is forgotten when, this far into a session, the shortage is not an active episode. The utilities start planning in the second month (about 60 simulated days) and the civic
// controller on the first day, so by this day a persisting shortage has been proven again.
constexpr std::uint32_t kNoticeForgetDay = 75;

class NoticeMemory {
public:
    bool Has(const std::string& key) const { return !key.empty() && keys_.count(key) != 0; }
    bool Add(const std::string& key) { return !key.empty() && keys_.insert(key).second; }
    bool Erase(const std::string& key) { return !key.empty() && keys_.erase(key) != 0; }
    std::size_t size() const { return keys_.size(); }
    // The keys that start with `prefix` (used for the per-building keys of one notice id: "<city>#<id>@").
    std::vector<std::string> KeysWithPrefix(const std::string& prefix) const {
        std::vector<std::string> out;
        if (prefix.empty()) return out;
        for (const auto& k : keys_) if (k.compare(0, prefix.size(), prefix) == 0) out.push_back(k);
        return out;
    }

    static NoticeMemory Parse(const std::string& text) {
        NoticeMemory m;
        std::istringstream in(text);
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty() || line[0] == ';') continue;
            m.keys_.insert(line);
        }
        return m;
    }
    std::string Serialize() const {
        std::string out = "; Department of Efficiency: shortages already told to the player, per city (outside the saves; safe to delete)\n";
        for (const auto& k : keys_) out += k + "\n";
        return out;
    }

private:
    std::set<std::string> keys_;
};

enum class NoticeAction { Push, AlreadyTold, Forget, None };

// What to do for one id on a given day. `due`: DeferredNotices reports the shortage still unresolved after the persistence window. `active`: its episode is open now.
inline NoticeAction DecideNotice(const NoticeMemory& told, const std::string& key, bool due, bool active, std::uint32_t day) {
    if (due) return told.Has(key) ? NoticeAction::AlreadyTold : NoticeAction::Push;
    if (!active && day >= kNoticeForgetDay && told.Has(key)) return NoticeAction::Forget;
    return NoticeAction::None;
}

}  // namespace sc4af
