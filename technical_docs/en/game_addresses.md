# SimCity 4 executable: reverse-engineering address reference

This reference records research on the **SimCity 4 Deluxe Steam 1.1.641.0 Windows x86 executable**, not the Department of Efficiency plugin version (1.0.0). Addresses refer to the executable image inspected at base `0x00400000`. Other builds, patches, rebasing and installations have not been checked. Do not assume an address is portable or safe to call or patch.

The table describes addresses whose roles have been identified in the research. It distinguishes static analysis from observations in the running game; a runtime test of one use does not validate every possible caller or argument. The list is not exhaustive: values whose roles have not been established are omitted.

## Identified functions and data

| Area | Address | Documented role | Evidence described in notes |
|---|---|---|---|
| Civic queries | `0x004D3A90` | Query dispatcher | Static mapping |
| Civic queries | `0x00A92488` | Query handler table (data) | Static mapping |
| Civic queries | `0x004C7980` | School information query handler | Static mapping |
| Civic queries | `0x004C7A70` | School building funding slider handler | Static mapping |
| Civic queries | `0x004C7BB0` | School bus funding slider handler | Static mapping |
| Civic queries | `0x004C8160` | Teachers / doctors query handler | Static mapping |
| Civic queries | `0x004C7CE0` | School desks / hospital beds query handler | Static mapping; capacity checked in game |
| Civic queries | `0x004C7D30` | School students query handler | Static mapping and runtime comparison |
| Civic queries | `0x004C7E10` | Hospital information query handler | Static mapping |
| Civic queries | `0x004C7F00` | Hospital building funding slider handler | Static mapping |
| Civic queries | `0x004C8030` | Hospital ambulance funding slider handler | Static mapping |
| Civic queries | `0x004C81B0` | Hospital patients query handler | Static mapping and runtime comparison |
| Civic queries | `0x006F8360` | Get occupant funding percentages | Static mapping; readback caveats |
| Civic queries | `0x006F8290` | Set occupant funding percentages | Static mapping |
| Budget and power | `0x006AC1B0` | Department funding setter entry | Static mapping and water runtime test |
| Budget and power | `0x006AB810` | Department funding setter implementation | Static mapping and water runtime test |
| Budget and power | `0x006EB520` | Read a power plant funding percentage | Static mapping |
| Budget and power | `0x006EB580` | Set a power plant funding percentage | Static mapping and runtime test |
| Budget and power | `0x006EB5F0` | Power plant efficiency calculation | Static mapping and runtime test |
| Sanitation | `0x006E3340` | Get garbage supply | Static mapping |
| Sanitation | `0x006E32E0` | Add to garbage supply | Static mapping |
| Sanitation | `0x006E4F10` | Get landfill tile capacity | Static mapping |
| Sanitation | `0x006E5040` | Get garbage capacity | Static mapping |
| Sanitation | `0x006E3000` | Read the total garbage left uncollected on the city grid | Static mapping |
| Sanitation | `0x006E4F30` | Get garbage produced last month | Static mapping |
| Sanitation | `0x006E4F40` | Get garbage imported last month | Static mapping |
| Sanitation | `0x006E4F50` | Get garbage exported last month | Static mapping |
| Sanitation | `0x006E4F60` | Get garbage recycled last month | Static mapping |
| Sanitation | `0x006E4F70` | Get garbage incinerated last month | Static mapping |
| Sanitation | `0x006E4F80` | Get garbage converted to energy last month | Static mapping |
| Sanitation | `0x006E4F90` | Get garbage landfilled last month | Static mapping |
| Sanitation | `0x006E7FD0` | Daily garbage processing | Static mapping |
| Sanitation | `0x006E7B60` | Monthly garbage accounting rollover | Static mapping |
| Sanitation | `0x006E3820` | Garbage disposal pipeline | Static mapping |
| Sanitation | `0x006E4300` | Per-building treatment capacity calculation | Static mapping |
| Landfills | `0x006B8C70` | Landfill tuning loader | Static mapping |
| Landfills | `0x006B91D0` | Monthly landfill developer update | Static mapping |
| Landfills | `0x006BD390` | Monthly processing of one landfill zone | Static mapping |
| Landfills | `0x006BA330` | Decay in a landfill cell record | Static mapping |
| Landfills | `0x006BB180` | Calculate landfill cell capacity | Static mapping and runtime measurements |
| Landfills | `0x006BB830` | Apply cell capacities | Static mapping and runtime measurements |
| Landfills | `0x006BCD70` | Reconcile landfill zone geometry | Static mapping and runtime measurements |
| Landfills | `0x006BAA70` | Sum maintenance for a zone | Static mapping and runtime measurements |
| Landfills | `0x006BAB30` | Get count of tiles in a zone | Static mapping and runtime measurements |
| Landfills | `0x00AB1610` | Landfill zone vtable (data) | Guarded object identification |
| Landfills | `0x00AB15DC` | Landfill cell-record vtable (data) | Guarded object identification |
| Neighbor deals | `0x006D1FC0` | Find a connection suitable for a deal | Static mapping |
| Neighbor deals | `0x006CF780` | Propose a deal | Static mapping |
| Neighbor deals | `0x006D21A0` | Generate deal proposals | Static mapping |
| Neighbor deals | `0x006D2870` | Add a deal and refresh regional flows | Static mapping |
| Neighbor deals | `0x006D4670` | Find all deals | Static mapping |
| Neighbor deals | `0x006D0840` | Find deals by type and city | Static mapping |
| Neighbor deals | `0x006D0760` | Find deals by type | Static mapping |
| Neighbor deals | `0x006D2A20` | Refresh commodity statistics | Static mapping |
| Neighbor deals | `0x006D3250` | Calculate deal cost | Static mapping |
| Neighbor deals | `0x006D3620` | Calculate maximum deal amount | Static mapping |
| Neighbor deals | `0x006CF960` | Set deal amount | Static mapping |
| Neighbor deals | `0x006CF9C0` | Update deal cost | Static mapping |
| Neighbor deals | `0x006CE610` | Cancel a deal | Static mapping |
| Neighbor deals | `0x006CE640` | Activate a deal | Static mapping |
| Neighbor deals | `0x0078BCA0` | Neighbor deals window message handler | Static mapping |
| Neighbor deals | `0x0077E600` | Build neighbor deals window | Static mapping |
| Neighbor deals | `0x00781C90` | Refresh neighbor deal rows | Static mapping |
| Other | `0x006CAE30` | Water shortage icon decision for a lot | Static mapping; see runtime caveat |
| Other | `0x006CAF27` | Water shortage icon conditional patch site | Static mapping; guarded patch |
| Other | `0x0040E9B0` | Retire an advisor notice | Static analysis and measured flags |

| Civic queries | `0x006FA280` | Calculate the strike roll using the facility coverage response curve | Static mapping |
| Budget and power | `0x006EB2F0` | Read a power plant’s nominal monthly generation cost before applying its funding percentage | Static mapping and runtime comparison |
| Budget and power | `0x006EC8F0` | Update power plant aging | Static mapping |
| Sanitation | `0x006E4F20` | Read the amount of garbage abducted during the previous month | Static mapping |
| Landfills | `0x006B9239` | Distribute the monthly supply among landfill zones and update available capacity | Static mapping |
| Landfills | `0x006B96E0` | Merge landfill zones when zoning overlaps them | Static mapping |
| Landfills | `0x006BC150` | Add tiles to a landfill zone and rebuild its cell grid | Static mapping and runtime measurements |
| Neighbor deals | `0x004AA200` | Recalculate a regional city’s import and export flows from its deal list | Static mapping |
| Neighbor deals | `0x004AA0E0` | Add one deal’s contribution to a regional city’s import and export flows | Static mapping |
| Neighbor deals | `0x006CF860` | Sum monthly income from eligible neighbor deals | Static mapping |
| Neighbor deals | `0x006CF8D0` | Sum monthly expenses from eligible neighbor deals | Static mapping |
| Neighbor deals | `0x0077DAB0` | Build the quantity choices for a neighbor deal row | Static mapping |

## References in the published source

The headers below contain references to these values. An appearance in source code does not by itself establish an address’s role or runtime validation.

| Published header | Values found in the header |
|---|---|
| [CivicFacilityIO.h](../../source/CivicFacilityIO.h) | `0x006FA280` |
| [DealAccounting.h](../../source/DealAccounting.h) | `0x004AA0E0`, `0x004AA200`, `0x006CF860`, `0x006CF8D0` |
| [DealQuantity.h](../../source/DealQuantity.h) | `0x0077DAB0`, `0x0078BCA0` |
| [DealQuote.h](../../source/DealQuote.h) | `0x006CF9C0`, `0x006D21A0`, `0x006D2A20`, `0x006D3250`, `0x006D3620` |
| [FarmWaterZot.h](../../source/FarmWaterZot.h) | `0x006CAE30`, `0x006CAF27` |
| [GarbageControl.h](../../source/GarbageControl.h) | `0x006B91D0`, `0x006E3820` |
| [LandfillRollover.h](../../source/LandfillRollover.h) | `0x006B91D0`, `0x006B9239`, `0x006BA330`, `0x006BD390` |
| [LandfillSpatial.h](../../source/LandfillSpatial.h) | `0x006BCD70` |
| [LandfillZoneModel.h](../../source/LandfillZoneModel.h) | `0x006B96E0`, `0x006BB180`, `0x006BB830`, `0x006BC150`, `0x006BCD70` |
| [LandfillZoneReader.h](../../source/LandfillZoneReader.h) | `0x006BAA70`, `0x006BAB30`, `0x006BB830`, `0x006BC150`, `0x00AB15DC`, `0x00AB1610` |
| [NeighborSellModel.h](../../source/NeighborSellModel.h) | `0x006D21A0`, `0x006D3250`, `0x006D3620` |
| [SanitationWriterPlan.h](../../source/SanitationWriterPlan.h) | `0x006AB810` |
| [UtilityHealth.h](../../source/UtilityHealth.h) | `0x006EB2F0`, `0x006EC8F0` |
| [UtilityOptimizer.h](../../source/UtilityOptimizer.h) | `0x006D3250` |


<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
