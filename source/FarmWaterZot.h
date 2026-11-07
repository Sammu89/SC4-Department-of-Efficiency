// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Suppresses the "no water" icon (zot) over agricultural lots. Every other zot, of every other lot, stays the game's own.
//
// SimCity 4.exe 1.1.641.0 decides the water zot of one lot in 0x6CAE30 (thiscall, 3 args: the cISC4Lot and its cell). It computes "this lot needs water" in the byte
// [esp+0x1C] and, at 0x6CAF27, tests it: true (and the cell not watered) = Zot_NoWater (exemplar group 0xC977C536, instance 0x1C430000) is put on the lot
// (0x6C98C0); false = the lot's current water zot, if any, is taken away (0x6C9780). The 6 bytes `mov al,[esp+1C]; test al,al` are replaced by a call that
// gives "needs water" only for a lot that is not in the Agriculture zone; the `je` after them is the game's own. So an agricultural lot never gets the icon,
// and one that already has it loses it at the next zot pass. Nothing else in the simulation reads this byte: the water service itself is unchanged.

#include <cstdint>

namespace sc4af::farm_water_zot {

// Verifies the image base and the bytes around 0x6CAF27 (version check) and only then patches. Idempotent. Returns false (nothing patched) on any mismatch.
bool Install();
bool Installed();
// How many times the game wanted the icon on an agricultural lot since the last call (the counter restarts at 0).
std::uint32_t TakeSuppressedCount();

} // namespace sc4af::farm_water_zot
