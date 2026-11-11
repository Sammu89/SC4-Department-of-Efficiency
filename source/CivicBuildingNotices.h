// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Player-facing notices about Education / Health buildings that funding cannot help (pure: no game types, no logging).
//
// The Department resolves what it can by itself and stays silent. The player gets ONE notice per facility type, listing the buildings that are at maximum funding, still cannot reach a
// safe coverage, are NOT on strike (strikes belong to the game's own alerts) and have stayed in that state for kNoticePersistDays simulated days.
// A notice carries at most kMaxLinks buildings, each with its own link, most severe first (deterministic order); the others stay under evaluation and can enter a later notice.
//
//   * First persistent shortage of a type: the notice is pushed. The three most severe buildings keep a humorous phrase, the others follow as numbered links.
//   * Different buildings, same count: the links of the shown notice are rewritten in place (the link variables are read when the player clicks); nothing is pushed.
//   * Different count: the shown notice is retired first (RetireAdvice; confirmed when the advice no longer carries the advisor's active flag 2), then the record for the new count is
//     pushed. There are never two notices of one type. Two replacements inside kReplaceWindowDays start a cooldown of kCooldownDays simulated days (no push; in-place updates continue).
//   * Same buildings: nothing is pushed (the record has a long timeout, so the notice stays shown). A notice the player dismissed is not pushed again while nothing changes.
//   * Every building recovered: the notice is retired and the type goes back to silence. A retire that failed or could not be checked leaves it active and is retried.
//   * At the first evaluation of a city, any notice of the plugin that the advisor shows but the board does not own (left by an earlier session) is retired.
//   * A push counts only when the advisor accepted it; a refused push is retried at the next evaluation.

#include "NativeNotice.h"
#include "NoticeMemory.h"
#include "ServiceNotices.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {

// The Education / Health notices keep their own, earlier policy (owner, 2026-11-11: restored as it was before the revision of the utility notices): discreet, 30 simulated days of proof, memory kept between sessions.
constexpr std::uint32_t kNoticePersistDays = 30;

constexpr int kMaxLinks = kCivicMaxLinks;
constexpr std::uint32_t kReplaceWindowDays = 30;
constexpr int kReplacementsPerWindow = 2;
constexpr std::uint32_t kCooldownDays = 30;

struct AffectedBuilding {
    int cellX = 0, cellZ = 0;       // the building's cell at the evaluation that proved the shortage
    float posX = 0.0f, posZ = 0.0f; // world coordinates (cell * 16)
    bool hasPos = false;
    double coverage = 1.0;          // attainable coverage at the maximum funding (lower = more severe)
};

using CellKey = std::pair<int, int>;
inline CellKey CellOf(const AffectedBuilding& b) { return {b.cellX, b.cellZ}; }

// What the advisor says about one notice record: listed (the advice carries the advisor's active flag 2, the state of the in-game notices list), not listed, or unknown (the call failed).
// SimCity 4.exe 1.1.641.0: GetAdviceByGUID returns the advisor's own advice object; PushEvent sets its flag 2 (0x50 -> 0x42);
// RetireAdvice requires flag 2 and clears it (0x42 -> 0x50, returns true).
// (Listing is defined in ServiceNotices.h)

// Everything that has to survive a reload for one type.
struct BoardMemory {
    int pushes = 0;
    int activeCount = 0;                           // the number of buildings of the shown notice (0 = none)
    std::vector<CellKey> cells;                    // the buildings of the shown notice, in link order
};

// The memory line of one type: "<city key>#<3 + type>|v2|<pushes>|<active count>|x,z;x,z". Only an identified city has a memory. A line in an older format is ignored:
// the reconciliation at the city's first evaluation retires whatever it left shown.
inline std::string BoardMemoryPrefix(const std::string& cityKey, int id) {
    const std::string base = NoticeKey(cityKey, id);
    return base.empty() ? std::string() : base + "|";
}
inline std::string BoardMemoryLine(const std::string& prefix, const BoardMemory& m) {
    std::ostringstream o;
    o << prefix << "v2|" << m.pushes << "|" << m.activeCount << "|";
    for (std::size_t i = 0; i < m.cells.size(); ++i) o << (i ? ";" : "") << m.cells[i].first << "," << m.cells[i].second;
    return o.str();
}
inline bool ParseBoardMemoryLine(const std::string& line, const std::string& prefix, BoardMemory& m) {
    if (prefix.empty() || line.compare(0, prefix.size(), prefix) != 0) return false;
    std::vector<std::string> f;
    std::istringstream in(line.substr(prefix.size()));
    std::string item;
    while (std::getline(in, item, '|')) f.push_back(item);
    if (f.size() < 3 || f[0] != "v2") return false;                           // older format: ignored
    m = BoardMemory();
    m.pushes = std::atoi(f[1].c_str());
    m.activeCount = std::atoi(f[2].c_str());
    if (f.size() > 3) {
        std::istringstream cs(f[3]);
        while (std::getline(cs, item, ';')) {
            int x = 0, z = 0;
            if (std::sscanf(item.c_str(), "%d,%d", &x, &z) == 2) m.cells.emplace_back(x, z);
        }
    }
    return m.pushes >= 0 && m.activeCount >= 0 && m.activeCount <= kMaxLinks && static_cast<int>(m.cells.size()) == m.activeCount;
}

// ONE notice per facility type, never two. When the buildings change:
//   * the same NUMBER of buildings: the links of the shown notice are rewritten in place (Refresh: the link variables are read when the player clicks), nothing is pushed;
//   * another number: the shown notice is RETIRED (and the retire confirmed) before the record for the new number is pushed (Push with `replacement`);
//   * every building recovered: the notice is retired (Retire).
// A notice the player dismissed is not pushed again while its buildings stay the same. Two replacements inside kReplaceWindowDays start a cooldown of kCooldownDays (no new push; the shown
// notice is still refreshed in place when the number of buildings allows it).
class CivicNoticeBoard {
public:
    enum class Do { Nothing, Push, Refresh, Retire };
    struct Action {
        Do what = Do::Nothing;
        int kind = 0;
        std::vector<AffectedBuilding> selected;  // Push / Refresh: the buildings of the notice, most severe first (1..kMaxLinks)
        bool replacement = false;                // Push: a notice of this type is shown and must be retired (and the retire confirmed) first
        int retireCount = 0;                     // Push with replacement / Retire: the number of buildings of the shown notice (its record)
    };

    void Reset() { kinds_.clear(); }

    // Restores the memory of a city announced in an earlier session.
    void Restore(int kind, const BoardMemory& m) {
        auto& k = kinds_[kind];
        k.pushes = m.pushes;
        if (m.activeCount > 0 && static_cast<int>(m.cells.size()) == m.activeCount) {
            k.active = true;
            k.activeCells = m.cells;
        }
    }
    BoardMemory Memory(int kind) const {
        BoardMemory m;
        const auto it = kinds_.find(kind);
        if (it == kinds_.end()) return m;
        m.pushes = it->second.pushes;
        if (it->second.active) {
            m.activeCount = static_cast<int>(it->second.activeCells.size());
            m.cells = it->second.activeCells;
        }
        return m;
    }

    // One evaluation of one facility type: `affected` = the buildings proven short now (not on strike).
    Action Update(int kind, std::uint32_t day, const std::vector<AffectedBuilding>& affected) {
        auto& k = kinds_[kind];
        std::map<CellKey, std::uint32_t> now;
        for (const auto& b : affected) {
            const CellKey key = CellOf(b);
            const auto it = k.firstDay.find(key);
            now[key] = it == k.firstDay.end() ? day : it->second;          // a building not proven at some evaluation starts its persistence again
        }
        k.firstDay = now;
        std::vector<AffectedBuilding> eligible;
        for (const auto& b : affected)
            if (day - k.firstDay[CellOf(b)] >= kNoticePersistDays) eligible.push_back(b);
        std::sort(eligible.begin(), eligible.end(), [](const AffectedBuilding& a, const AffectedBuilding& b) {
            if (a.coverage != b.coverage) return a.coverage < b.coverage;
            return CellOf(a) < CellOf(b);
        });
        if (static_cast<int>(eligible.size()) > kMaxLinks) eligible.resize(kMaxLinks);

        Action a;
        a.kind = kind;
        if (eligible.empty()) {
            if (k.active && affected.empty()) {                                // retired only when EVERY building recovered; one that is still maturing keeps the notice
                a.what = Do::Retire;
                a.retireCount = static_cast<int>(k.activeCells.size());
            }
            return a;
        }
        std::vector<CellKey> cells;
        for (const auto& b : eligible) cells.push_back(CellOf(b));
        a.selected = eligible;
        if (k.active) {
            if (cells == k.activeCells) return a;                                                 // the same buildings in the same order: nothing to say
            if (cells.size() == k.activeCells.size()) { a.what = Do::Refresh; return a; }          // same number: the links of the shown notice are rewritten in place
            if (day < k.cooldownUntil) return a;                                                  // cooldown: keep the shown notice
            a.replacement = true;
            a.retireCount = static_cast<int>(k.activeCells.size());
        }
        a.what = Do::Push;
        return a;
    }

    // The outcome of a push (a refused push changes nothing but what the caller already confirmed: it is tried again at the next evaluation).
    void ConfirmPush(const Action& a, std::uint32_t day, bool accepted) {
        if (a.what != Do::Push || !accepted) return;
        auto& k = kinds_[a.kind];
        if (a.replacement) {
            k.replacements.push_back(day);
            k.replacements.erase(std::remove_if(k.replacements.begin(), k.replacements.end(), [&](std::uint32_t d) { return day - d >= kReplaceWindowDays; }), k.replacements.end());
            if (static_cast<int>(k.replacements.size()) >= kReplacementsPerWindow) {
                k.cooldownUntil = day + kCooldownDays;
                k.replacements.clear();
            }
        }
        k.active = true;
        k.activeCells.clear();
        for (const auto& b : a.selected) k.activeCells.push_back(CellOf(b));
        ++k.pushes;
    }
    // The links of the shown notice were rewritten in place.
    void ConfirmRefresh(const Action& a) {
        if (a.what != Do::Refresh) return;
        auto& k = kinds_[a.kind];
        k.activeCells.clear();
        for (const auto& b : a.selected) k.activeCells.push_back(CellOf(b));
    }
    // The shown notice is gone (the retire was confirmed, or the advisor no longer shows it). A retire that failed or could not be checked changes nothing: it is tried again.
    void ConfirmRetire(int kind, bool gone) {
        if (!gone) return;
        auto& k = kinds_[kind];
        k.active = false;
        k.activeCells.clear();
    }

    int pushes(int kind) const { auto it = kinds_.find(kind); return it == kinds_.end() ? 0 : it->second.pushes; }
    bool active(int kind) const { auto it = kinds_.find(kind); return it != kinds_.end() && it->second.active; }
    int activeCount(int kind) const { auto it = kinds_.find(kind); return it == kinds_.end() || !it->second.active ? 0 : static_cast<int>(it->second.activeCells.size()); }
    std::vector<CellKey> activeCells(int kind) const { auto it = kinds_.find(kind); return it == kinds_.end() ? std::vector<CellKey>() : it->second.activeCells; }
    std::uint32_t cooldownUntil(int kind) const { auto it = kinds_.find(kind); return it == kinds_.end() ? 0 : it->second.cooldownUntil; }

private:
    struct PerKind {
        std::map<CellKey, std::uint32_t> firstDay;     // building -> first day of its continuous proof
        bool active = false;                            // a notice of this type is shown (pushed and not confirmed gone)
        std::vector<CellKey> activeCells;               // its buildings, in link order
        int pushes = 0;                                 // accepted pushes so far (also across sessions)
        std::vector<std::uint32_t> replacements;        // days of the recent replacements
        std::uint32_t cooldownUntil = 0;
    };
    std::map<int, PerKind> kinds_;
};

}  // namespace sc4af
