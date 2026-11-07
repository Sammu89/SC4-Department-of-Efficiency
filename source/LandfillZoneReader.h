// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Game access of the spatial landfill analysis (LandfillSpatial.h): the read of the zones and the two native zoning calls. SimCity 4 1.1.641.0 only.
//
// The zone read:
//   * READ ONLY: nothing is ever written to a game structure; every raw read runs under an exception guard (a fault ends the read and the snapshot is refused);
//   * the object comes from the public API (cISC4LandfillDeveloper::GetLandfillZoneFromCell) and must carry the vtable of cSC4LandfillZone (0x00AB1610); its cell records carry the vtable 0x00AB15DC;
//   * the layout was decoded statically (0x6BAA70, 0x6BAB30, 0x6BB830, 0x6BC150) and checked against in-game measurements;
//   * every read is cross-checked against the game's own totals (ValidateLandfillSpatial: capacity, stored garbage, maintenance, landfill square count): one difference and nothing
//     spatial is used. A different executable version never reads at all.
// The two zoning calls are the game's own cISC4ZoneManager::PlaceZone (the zoning tool's path): place = false is a dry run with no effect; place = true
// is used only by the ghost repair (re-zoning squares that are already landfill: no zone type changes).

#include "LandfillSpatial.h"

class cISC4City;

namespace sc4af {

// The snapshot of every landfill zone of the city (read == false + problem when it cannot be read).
LandfillSpatialSnapshot ReadLandfillSpatial(cISC4City* city);

// PlaceZone(place = false) on the placement's region with the parameters the player's tool would face: 0 = accepted with every square kept, the game's error code otherwise,
// 100 = accepted but the game dropped squares (a slope), -1 = the call is unavailable or raised.
int LandfillNativeCheck(cISC4City* city, const LandfillPlacement& p);

// The ghost repair: PlaceZone(place = true) of the region as landfill. true = the game returned success with every square kept.
bool LandfillRezoneRegion(cISC4City* city, const LandfillPlacement& p, std::string& problem);

}  // namespace sc4af
