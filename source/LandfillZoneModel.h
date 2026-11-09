// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Exact model of ONE landfill zone of SimCity 4 1.1.641.0. Pure, header-only, no game types, no I/O.
// It reproduces the game's own arithmetic and predicted every validated in-game operation (zoning, dezoning, ghost repair) to the unit.
//
// State: the zone box (x1, y1, x2, y2), the bitmap of its tiles (bit set = the tile belongs to the zone; it can hold GHOSTS: tiles whose zone type is no longer landfill), and the
// cell records: a (w-1) x (h-1) grid of 2x2 windows, each {active, capacity, stock}, addressed by coordinates RELATIVE to the box.
//
//   Recompute (0x6BB830 / 0x6BB180): T = bitmap tiles, N = active cells, C = round(float(T x 30000) / N) (x87 division, + 1.5 x 2^23 into a float32: ties to even); every
//              active cell gets capacity C, every inactive one 0; stock above the new capacity goes back to the valve (AddToGarbageSupply).
//   Add (zoning landfill: 0x6B9AC0 -> 0x6BC150 -> 0x6BCD70 -> 0x6BB830): bitmap |= region; the box becomes the union; the RECONCILIATION clears every ghost bit; the grid is
//              rebuilt: an old record keeps ITS relative (x, y), its flag and its stock; a position with no old record starts active and empty; a cell whose window is not entirely
//              in the bitmap becomes inactive (its stock goes to the valve through the recompute). Growing the box up or left therefore MOVES every old cell in space.
//   Remove (dezoning: 0x6B96E0 -> 0x6B9360 -> 0x6BCAA0 -> 0x6BA5C0): bitmap -= tiles; the up to four cells whose window holds a removed tile become inactive; no grid rebuild, no
//              reconciliation (ghosts stay); then the recompute.
//   Re-zoning tiles that are ALREADY landfill is an Add with no new tile: it only reconciles (the ghost repair).
//
// The rounding: the executable divides on the x87 unit and stores the sum with 1.5 x 2^23 into a float32. The precision the unit runs at is not proven (53-bit by default, 24-bit if a
// renderer changed it), so a quotient whose fraction lies within 2^-8 of one half is flagged `roundingAmbiguous`: the two precisions could round it differently, and a decision that
// depends on C must then take the worse of the two.

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

namespace sc4af {

constexpr std::int64_t kLandfillTileCapacity = 30000;      // 0x4A65267A (verified in game)
constexpr std::int64_t kLandfillEmptyTileCost = 5;         // monthly maintenance of a bitmap tile without a pile (0x6BA3C0 default)

struct LandfillCell {
    bool active = false;
    std::int64_t capacity = 0;
    std::int64_t stock = 0;
};

using LandfillTile = std::pair<int, int>;                  // (x, z) city cell

// C = round(T x 30000 / N) as 0x6BB180 computes it. `ambiguous` = the 24-bit and 53-bit x87 precisions could round differently.
inline std::int64_t LandfillCellCapacity(std::int64_t tiles, std::int64_t cells, bool* ambiguous = nullptr) {
    if (ambiguous) *ambiguous = false;
    if (cells <= 0 || tiles <= 0) return 0;
    const std::uint32_t num = static_cast<std::uint32_t>(tiles * kLandfillTileCapacity);   // imul in 32 bits; read as unsigned (fadd 2^32 when negative)
    const double q = static_cast<double>(num) / static_cast<double>(cells);
    const double f = std::floor(q);
    const double d = q - f;
    std::int64_t c = static_cast<std::int64_t>(f) + ((d > 0.5 || (d == 0.5 && std::fmod(f, 2.0) != 0.0)) ? 1 : 0);
    if (ambiguous && std::fabs(d - 0.5) < 1.0 / 256.0) *ambiguous = true;
    // the 24-bit variant: the quotient rounded to a float32 first, then to an integer (ties to even)
    const float q24 = static_cast<float>(q);
    const float s24 = q24 + 12582912.0f;
    const std::int64_t c24 = static_cast<std::int64_t>(s24) - 12582912;
    if (ambiguous && c24 != c) *ambiguous = true;
    return c;
}

class LandfillZoneModel {
public:
    int x1 = 0, y1 = 0, x2 = -1, y2 = -1;

    LandfillZoneModel() = default;
    LandfillZoneModel(int ax1, int ay1, int ax2, int ay2) : x1(ax1), y1(ay1), x2(ax2), y2(ay2) {
        bits_.assign(static_cast<size_t>(W()) * H(), 0);
        cells_.assign(static_cast<size_t>(GW()) * GH(), LandfillCell{});
    }

    int W() const { return x2 - x1 + 1; }
    int H() const { return y2 - y1 + 1; }
    int GW() const { return std::max(0, W() - 1); }
    int GH() const { return std::max(0, H() - 1); }
    bool InBox(int x, int y) const { return x >= x1 && x <= x2 && y >= y1 && y <= y2; }
    bool Has(int x, int y) const { return InBox(x, y) && bits_[Idx(x, y)] != 0; }
    void Set(int x, int y, bool on) { if (InBox(x, y)) bits_[Idx(x, y)] = on ? 1 : 0; }
    LandfillCell& Cell(int rx, int ry) { return cells_[static_cast<size_t>(ry) * GW() + rx]; }
    const LandfillCell& Cell(int rx, int ry) const { return cells_[static_cast<size_t>(ry) * GW() + rx]; }
    bool HasCell(int rx, int ry) const { return rx >= 0 && ry >= 0 && rx < GW() && ry < GH(); }

    std::int64_t T() const { std::int64_t n = 0; for (auto b : bits_) n += b; return n; }
    std::int64_t N() const { std::int64_t n = 0; for (const auto& c : cells_) n += c.active ? 1 : 0; return n; }
    std::int64_t Capacity() const { std::int64_t n = 0; for (const auto& c : cells_) n += c.capacity; return n; }
    std::int64_t Usage() const { std::int64_t n = 0; for (const auto& c : cells_) n += c.stock; return n; }
    std::int64_t CellCapacity() const { for (const auto& c : cells_) if (c.active) return c.capacity; return 0; }
    std::vector<LandfillTile> Tiles() const {
        std::vector<LandfillTile> t;
        for (int y = y1; y <= y2; ++y) for (int x = x1; x <= x2; ++x) if (Has(x, y)) t.emplace_back(x, y);
        return t;
    }
    // The window of cell (rx, ry) covers tiles (x1 + rx .. +1, y1 + ry .. +1).
    bool WindowFull(int rx, int ry) const {
        const int x = x1 + rx, y = y1 + ry;
        return Has(x, y) && Has(x + 1, y) && Has(x, y + 1) && Has(x + 1, y + 1);
    }

    struct Result {
        std::int64_t pushed = 0;           // units sent back to the valve
        std::int64_t cellCapacity = 0;     // C after the operation
        bool roundingAmbiguous = false;
        int tilesAdded = 0, tilesRemoved = 0, ghostsReconciled = 0;
    };

    // 0x6BB830
    Result Recompute() {
        Result r;
        const std::int64_t n = N();
        r.cellCapacity = n ? LandfillCellCapacity(T(), n, &r.roundingAmbiguous) : 0;
        for (auto& c : cells_) {
            c.capacity = c.active ? r.cellCapacity : 0;
            if (c.stock > c.capacity) { r.pushed += c.stock - c.capacity; c.stock = c.capacity; }
        }
        return r;
    }

    // Zoning landfill on `region` (tiles, any shape; tiles already in the bitmap are allowed: that is the re-zoning repair). `ghosts` = the bitmap tiles the reconciliation clears
    // (zone type no longer landfill, or a network piece on them), read by the caller from the game.
    Result Add(const std::vector<LandfillTile>& region, const std::set<LandfillTile>& ghosts) {
        Result r;
        int nx1 = x1, ny1 = y1, nx2 = x2, ny2 = y2;
        bool empty = W() <= 0 || H() <= 0;
        for (const auto& [x, y] : region) {
            if (empty) { nx1 = nx2 = x; ny1 = ny2 = y; empty = false; continue; }
            nx1 = std::min(nx1, x); ny1 = std::min(ny1, y); nx2 = std::max(nx2, x); ny2 = std::max(ny2, y);
        }
        LandfillZoneModel g(nx1, ny1, nx2, ny2);
        for (const auto& t : Tiles()) g.Set(t.first, t.second, true);
        for (const auto& [x, y] : region) { if (!g.Has(x, y)) ++r.tilesAdded; g.Set(x, y, true); }
        for (const auto& [x, y] : ghosts) if (g.Has(x, y) && !Contains(region, x, y)) { g.Set(x, y, false); ++r.ghostsReconciled; }
        for (int ry = 0; ry < g.GH(); ++ry)
            for (int rx = 0; rx < g.GW(); ++rx) {
                LandfillCell c;
                if (HasCell(rx, ry)) c = Cell(rx, ry);        // an old record keeps ITS relative position, flag and stock
                else c.active = true;                          // a new position starts active and empty
                if (c.active && !g.WindowFull(rx, ry)) c.active = false;
                g.Cell(rx, ry) = c;
            }
        *this = std::move(g);
        const Result rc = Recompute();
        r.pushed = rc.pushed; r.cellCapacity = rc.cellCapacity; r.roundingAmbiguous = rc.roundingAmbiguous;
        return r;
    }

    // Dezoning `tiles` (the ones outside the bitmap are ignored).
    Result Remove(const std::vector<LandfillTile>& tiles) {
        Result r;
        for (const auto& [x, y] : tiles) {
            if (!Has(x, y)) continue;
            Set(x, y, false);
            ++r.tilesRemoved;
            const int rx = x - x1, ry = y - y1;
            for (const auto& [cx, cy] : {std::pair{rx, ry}, std::pair{rx - 1, ry}, std::pair{rx, ry - 1}, std::pair{rx - 1, ry - 1}})
                if (HasCell(cx, cy)) Cell(cx, cy).active = false;
        }
        const Result rc = Recompute();
        r.pushed = rc.pushed; r.cellCapacity = rc.cellCapacity; r.roundingAmbiguous = rc.roundingAmbiguous;
        return r;
    }

    // The cells whose window holds tile (x, y) (up to four).
    std::vector<std::pair<int, int>> CellsCovering(int x, int y) const {
        std::vector<std::pair<int, int>> out;
        const int rx = x - x1, ry = y - y1;
        for (const auto& [cx, cy] : {std::pair{rx, ry}, std::pair{rx - 1, ry}, std::pair{rx, ry - 1}, std::pair{rx - 1, ry - 1}})
            if (HasCell(cx, cy)) out.emplace_back(cx, cy);
        return out;
    }

private:
    std::vector<std::uint8_t> bits_;
    std::vector<LandfillCell> cells_;
    size_t Idx(int x, int y) const { return static_cast<size_t>(y - y1) * W() + (x - x1); }
    static bool Contains(const std::vector<LandfillTile>& v, int x, int y) { return std::find(v.begin(), v.end(), LandfillTile{x, y}) != v.end(); }
};

}  // namespace sc4af
