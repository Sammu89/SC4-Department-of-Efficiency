# SimCity 4 executable: reverse-engineering address reference

This reference records research on the **SimCity 4 Deluxe Steam 1.1.641.0 Windows x86 executable**, not the Department of Efficiency plugin version (1.0.0). Addresses refer to the executable image inspected at base `0x00400000`. Other builds, patches, rebasing and installations have not been checked. Do not assume an address is portable or safe to call or patch.

The first section identifies addresses whose roles are stated in the research notes. The second is a comprehensive **candidate index**, not a claim that every entry is a function: a value may refer to an instruction, function, table, global, vtable, range endpoint or even a coincidentally similar constant. A candidate with no identified role remains unclassified. Static inspection, runtime observation and hypotheses are distinct; historical notes can be superseded, so consult the referenced research before relying on a claim.

## Scope and provenance

The candidate index scans hexadecimal literals in all Markdown files under the development repository's `docs/` and C++ headers and sources under `src/`, including historical and research-only material. The scan includes values between `0x00400000` and `0x00BFFFFF`, written as six hex digits or with one or two leading zeroes. Repeated values are deduplicated; source paths and line numbers locate every matching line in the **local development repository**, which is separate from this public repository. Those paths are references, not links in this public repository. The index does not scan logs, generated files, external libraries or other numeric forms; it cannot establish that every candidate is an address, nor that every executable address in the project has been identified.

The roles and evidence notes below are summaries, not permission to write to game state. Some documents describe earlier experiments and may not reflect the final product. Where a function is tested in game, that does not establish that all its callers, arguments or effects have been tested.

## Identified functions and data

| Area | Address | Documented role | Evidence described in notes | Development source |
|---|---|---|---|---|
| Civic queries | `0x004D3A90` | Query dispatcher | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x00A92488` | Query handler table (data) | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C7980` | School information query handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C7A70` | School building funding slider handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C7BB0` | School bus funding slider handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C8160` | Teachers / doctors query handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C7CE0` | School desks / hospital beds query handler | Static mapping; capacity checked in game | `docs/SC4_INTEGRATION.md` |
| Civic queries | `0x004C7D30` | School students query handler | Static mapping and runtime comparison | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C7E10` | Hospital information query handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C7F00` | Hospital building funding slider handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C8030` | Hospital ambulance funding slider handler | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x004C81B0` | Hospital patients query handler | Static mapping and runtime comparison | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x006F8360` | Get occupant funding percentages | Static mapping; readback caveats | `docs/CIVIC_QUERY_MAP.md` |
| Civic queries | `0x006F8290` | Set occupant funding percentages | Static mapping | `docs/CIVIC_QUERY_MAP.md` |
| Budget and power | `0x006AC1B0` | Department funding setter entry | Static mapping and water runtime test | `docs/PHASE5_UTILITIES.md` |
| Budget and power | `0x006AB810` | Department funding setter implementation | Static mapping and water runtime test | `docs/PHASE5_UTILITIES.md` |
| Budget and power | `0x006EB520` | Read a power plant funding percentage | Static mapping | `docs/PHASE5_UTILITIES.md` |
| Budget and power | `0x006EB580` | Set a power plant funding percentage | Static mapping and runtime test | `docs/PHASE5_UTILITIES.md` |
| Budget and power | `0x006EB5F0` | Power plant efficiency calculation | Static mapping and runtime test | `docs/PHASE5_UTILITIES.md` |
| Sanitation | `0x006E3340` | Get garbage supply | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E32E0` | Add to garbage supply | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F10` | Get landfill tile capacity | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E5040` | Get garbage capacity | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E3000` | Get total uncollected garbage | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F30` | Get garbage produced last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F40` | Get garbage imported last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F50` | Get garbage exported last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F60` | Get garbage recycled last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F70` | Get garbage incinerated last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F80` | Get garbage converted to energy last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4F90` | Get garbage landfilled last month | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E7FD0` | Daily garbage processing | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E7B60` | Monthly garbage accounting rollover | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E3820` | Garbage disposal pipeline | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Sanitation | `0x006E4300` | Per-building treatment capacity calculation | Static mapping | `docs/PHASE8_GARBAGE_FUNDING_RE.md` |
| Landfills | `0x006B8C70` | Landfill tuning loader | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Landfills | `0x006B91D0` | Monthly landfill developer update | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Landfills | `0x006BD390` | Monthly processing of one landfill zone | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Landfills | `0x006BA330` | Decay in a landfill cell record | Static mapping | `docs/PHASE8_GARBAGE_RE.md` |
| Landfills | `0x006BB180` | Calculate landfill cell capacity | Static mapping and runtime measurements | `docs/LANDFILL_ZONE_RE.md` |
| Landfills | `0x006BB830` | Apply cell capacities | Static mapping and runtime measurements | `docs/LANDFILL_ZONE_RE.md` |
| Landfills | `0x006BCD70` | Reconcile landfill zone geometry | Static mapping and runtime measurements | `docs/LANDFILL_ZONE_RE.md` |
| Landfills | `0x006BAA70` | Sum maintenance for a zone | Static mapping and runtime measurements | `docs/LANDFILL_ZONE_RE.md` |
| Landfills | `0x006BAB30` | Get count of tiles in a zone | Static mapping and runtime measurements | `docs/LANDFILL_ZONE_RE.md` |
| Landfills | `0x00AB1610` | Landfill zone vtable (data) | Guarded object identification | `docs/LANDFILL_SPATIAL.md` |
| Landfills | `0x00AB15DC` | Landfill cell-record vtable (data) | Guarded object identification | `docs/LANDFILL_SPATIAL.md` |
| Neighbor deals | `0x006D1FC0` | Find a connection suitable for a deal | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006CF780` | Propose a deal | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D21A0` | Generate deal proposals | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D2870` | Add a deal and refresh regional flows | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D4670` | Find all deals | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D0840` | Find deals by type and city | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D0760` | Find deals by type | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D2A20` | Refresh commodity statistics | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D3250` | Calculate deal cost | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006D3620` | Calculate maximum deal amount | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006CF960` | Set deal amount | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006CF9C0` | Update deal cost | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006CE610` | Cancel a deal | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x006CE640` | Activate a deal | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x0078BCA0` | Neighbor deals window message handler | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x0077E600` | Build neighbor deals window | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Neighbor deals | `0x00781C90` | Refresh neighbor deal rows | Static mapping | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md` |
| Other | `0x006CAE30` | Water shortage icon decision for a lot | Static mapping; see runtime caveat | `docs/FARM_WATER_ZOT.md` |
| Other | `0x006CAF27` | Water shortage icon conditional patch site | Static mapping; guarded patch | `docs/FARM_WATER_ZOT.md` |
| Other | `0x0040E9B0` | Retire an advisor notice | Static analysis and measured flags | `docs/NATIVE_ADVISORS.md` |

## Complete candidate index for the stated scope

449 distinct values. Sources are sorted by path and line; the same line can document more than one value. A source mention alone does not prove the value is an executable address or identify its role.

| Candidate value | Development references (path:line) |
|---|---|
| `0x00400000` | `docs/FARM_WATER_ZOT.md:19`, `src/FarmWaterZot.cpp:73`, `src/FarmWaterZot.cpp:74`, `src/LandfillResearch.cpp:434`, `src/LandfillResearch.cpp:435` |
| `0x0040D73E` | `docs/POWER_GARBAGE_CLOSURE.md:154`, `docs/POWER_GARBAGE_CLOSURE.md:167` |
| `0x0040E9B0` | `docs/NATIVE_ADVISORS.md:74` |
| `0x00421660` | `docs/PHASE7_POWER_DEAL_RE.md:78` |
| `0x00421760` | `docs/PHASE7_POWER_DEAL_RE.md:50` |
| `0x00421AB0` | `docs/UTILITY_COMMUNITY_BASELINE.md:49`, `src/UtilityModel.h:42` |
| `0x00422070` | `docs/UTILITY_COMMUNITY_BASELINE.md:62`, `src/UtilityModel.h:83` |
| `0x004220A0` | `docs/PHASE8_GARBAGE_RE.md:7`, `docs/UTILITY_COMMUNITY_BASELINE.md:21` |
| `0x004222EF` | `docs/PHASE6_OPTIMIZER.md:41` |
| `0x00423710` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:34` |
| `0x004422A0` | `docs/LANDFILL_SPATIAL.md:56`, `docs/LANDFILL_ZONE_RE.md:288` |
| `0x004A7640` | `docs/PHASE7_POWER_DEAL_RE.md:127`, `docs/PHASE7_POWER_DEAL_RE.md:151` |
| `0x004A9EE0` | `docs/PHASE7_POWER_DEAL_RE.md:127` |
| `0x004A9EF0` | `docs/PHASE7_POWER_DEAL_RE.md:151` |
| `0x004A9F00` | `docs/PHASE7_POWER_DEAL_RE.md:127` |
| `0x004A9F50` | `docs/PHASE7_POWER_DEAL_RE.md:151` |
| `0x004A9FB0` | `docs/PHASE7_POWER_DEAL_RE.md:151` |
| `0x004AA010` | `docs/PHASE7_POWER_DEAL_RE.md:151` |
| `0x004AA070` | `docs/PHASE7_POWER_DEAL_RE.md:151` |
| `0x004AA080` | `docs/PHASE7_POWER_DEAL_RE.md:127` |
| `0x004AA0E0` | `docs/PHASE7_POWER_DEAL_RE.md:132`, `docs/PHASE7_POWER_DEAL_RE.md:139`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:135`, `src/DealAccounting.h:8`, `src/DealAccounting.h:80` |
| `0x004AA170` | `docs/PHASE7_POWER_DEAL_RE.md:132` |
| `0x004AA200` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:135`, `src/DealAccounting.h:8` |
| `0x004C3C50` | `docs/CIVIC_QUERY_MAP.md:70` |
| `0x004C3CA0` | `docs/CIVIC_QUERY_MAP.md:21` |
| `0x004C7980` | `docs/CIVIC_QUERY_MAP.md:21` |
| `0x004C7A70` | `docs/CIVIC_QUERY_MAP.md:22` |
| `0x004C7BB0` | `docs/CIVIC_QUERY_MAP.md:23` |
| `0x004C7CE0` | `docs/CIVIC_QUERY_MAP.md:25`, `docs/CIVIC_QUERY_MAP.md:27`, `docs/CIVIC_QUERY_MAP.md:29`, `docs/CIVIC_QUERY_MAP.md:34`, `docs/SC4_INTEGRATION.md:76`, `docs/SC4_INTEGRATION.md:78` |
| `0x004C7D30` | `docs/CIVIC_QUERY_MAP.md:26`, `docs/CIVIC_QUERY_MAP.md:28`, `docs/SC4_INTEGRATION.md:77` |
| `0x004C7E10` | `docs/CIVIC_QUERY_MAP.md:30` |
| `0x004C7F00` | `docs/CIVIC_QUERY_MAP.md:31` |
| `0x004C8030` | `docs/CIVIC_QUERY_MAP.md:32` |
| `0x004C8160` | `docs/CIVIC_QUERY_MAP.md:24`, `docs/CIVIC_QUERY_MAP.md:33` |
| `0x004C81B0` | `docs/CIVIC_QUERY_MAP.md:35`, `docs/SC4_INTEGRATION.md:79` |
| `0x004D3A90` | `docs/CIVIC_QUERY_MAP.md:13`, `docs/SC4_INTEGRATION.md:72` |
| `0x004D3C30` | `docs/SC4_INTEGRATION.md:81` |
| `0x004DD780` | `docs/LANDFILL_ZONE_RE.md:193` |
| `0x004DD924` | `docs/LANDFILL_ZONE_RE.md:195` |
| `0x004DDAD1` | `docs/LANDFILL_ZONE_RE.md:194` |
| `0x004DDAE6` | `docs/LANDFILL_ZONE_RE.md:194` |
| `0x004DDD08` | `docs/LANDFILL_ZONE_RE.md:197` |
| `0x004DDD17` | `docs/LANDFILL_ZONE_RE.md:196`, `docs/LANDFILL_ZONE_RE.md:248` |
| `0x004DDD58` | `docs/LANDFILL_ZONE_RE.md:196` |
| `0x004DDD60` | `docs/LANDFILL_ZONE_RE.md:196` |
| `0x004DDD70` | `docs/LANDFILL_ZONE_RE.md:196` |
| `0x004DDD76` | `docs/LANDFILL_ZONE_RE.md:247` |
| `0x004DDDCC` | `docs/LANDFILL_ZONE_RE.md:247` |
| `0x004DDDDB` | `docs/LANDFILL_ZONE_RE.md:197` |
| `0x004DDDFB` | `docs/LANDFILL_ZONE_RE.md:197` |
| `0x004DDFCC` | `docs/LANDFILL_ZONE_RE.md:242` |
| `0x004F5660` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:25` |
| `0x0050CF62` | `docs/POWER_GARBAGE_CLOSURE.md:258` |
| `0x00524952` | `docs/TRANSLATIONS.md:25` |
| `0x00530B7E` | `docs/POWER_GARBAGE_CLOSURE.md:234` |
| `0x00591484` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:49` |
| `0x005B9FC0` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:19` |
| `0x005BA002` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:13`, `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:19` |
| `0x005E16E0` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:25`, `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:31` |
| `0x005E16F5` | `docs/CURRENT_STATUS.md:57` |
| `0x005E55B0` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:49` |
| `0x005EF8D8` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:53` |
| `0x005F4250` | `docs/PHASE7_POWER_DEAL_RE.md:54`, `docs/PHASE7_POWER_DEAL_RE.md:118`, `docs/PHASE7_POWER_DEAL_RE.md:120` |
| `0x005F4620` | `docs/PHASE7_POWER_DEAL_RE.md:117`, `docs/PHASE7_POWER_DEAL_RE.md:118` |
| `0x005FD030` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:33`, `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:40`, `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:43` |
| `0x005FD4F0` | `docs/PHASE7_POWER_DEAL_RE.md:31`, `docs/PHASE7_POWER_DEAL_RE.md:79` |
| `0x005FD530` | `docs/PHASE7_POWER_DEAL_RE.md:58` |
| `0x0060FB80` | `docs/PHASE7_POWER_DEAL_RE.md:19`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x0066CDAC` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:49`, `docs/POWER_GARBAGE_CLOSURE.md:258` |
| `0x0066D0CD` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:49` |
| `0x006AA667` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:40` |
| `0x006AB810` | `docs/PHASE5_UTILITIES.md:91`, `docs/PHASE8_GARBAGE_FUNDING_RE.md:7`, `docs/PHASE8_GARBAGE_FUNDING_RE.md:40`, `src/SanitationWriterPlan.h:9` |
| `0x006AB940` | `docs/CIVIC_STRIKE.md:10` |
| `0x006AC1B0` | `docs/PHASE5_UTILITIES.md:91`, `docs/PHASE8_GARBAGE_FUNDING_RE.md:7` |
| `0x006B0000` | `docs/PHASE8_GARBAGE_RE.md:220` |
| `0x006B8BA0` | `docs/PHASE8_GARBAGE_RE.md:202`, `src/UtilityProbe.cpp:379` |
| `0x006B8C70` | `docs/PHASE8_GARBAGE_RE.md:128`, `docs/PHASE8_GARBAGE_RE.md:182` |
| `0x006B8CC7` | `docs/PHASE8_GARBAGE_RE.md:186` |
| `0x006B8CD7` | `docs/PHASE8_GARBAGE_RE.md:187` |
| `0x006B8CE7` | `docs/PHASE8_GARBAGE_RE.md:188` |
| `0x006B8CF7` | `docs/PHASE8_GARBAGE_RE.md:189` |
| `0x006B8D07` | `docs/PHASE8_GARBAGE_RE.md:190` |
| `0x006B8D17` | `docs/PHASE8_GARBAGE_RE.md:191` |
| `0x006B8D2A` | `docs/PHASE8_GARBAGE_RE.md:193` |
| `0x006B8D3A` | `docs/PHASE8_GARBAGE_RE.md:192` |
| `0x006B8DB0` | `docs/LANDFILL_ZONE_RE.md:55` |
| `0x006B8F20` | `docs/PHASE8_GARBAGE_RE.md:205` |
| `0x006B8FB0` | `docs/LANDFILL_ZONE_RE.md:18`, `docs/LANDFILL_ZONE_RE.md:245`, `docs/PHASE8_GARBAGE_RE.md:203`, `docs/PHASE8_GARBAGE_RE.md:205` |
| `0x006B9030` | `docs/PHASE8_GARBAGE_RE.md:187`, `docs/PHASE8_GARBAGE_RE.md:193` |
| `0x006B90CF` | `docs/PHASE8_GARBAGE_RE.md:187` |
| `0x006B9191` | `docs/PHASE8_GARBAGE_RE.md:193` |
| `0x006B91D0` | `docs/PHASE8_GARBAGE_RE.md:128`, `docs/PHASE8_GARBAGE_RE.md:201`, `docs/PHASE8_GARBAGE_RE.md:243`, `docs/PHASE8_GARBAGE_RE.md:297`, `src/GarbageControl.h:17`, `src/LandfillRollover.h:7`, `src/LandfillResearch.cpp:390` |
| `0x006B9230` | `docs/PHASE8_GARBAGE_RE.md:129` |
| `0x006B9239` | `docs/PHASE8_GARBAGE_RE.md:188`, `docs/PHASE8_GARBAGE_RE.md:201`, `docs/PHASE8_GARBAGE_RE.md:202`, `src/LandfillRollover.h:7` |
| `0x006B9270` | `docs/PHASE8_GARBAGE_RE.md:128` |
| `0x006B9272` | `docs/PHASE8_GARBAGE_RE.md:188`, `docs/PHASE8_GARBAGE_RE.md:267` |
| `0x006B927A` | `docs/PHASE8_GARBAGE_RE.md:128` |
| `0x006B92A2` | `docs/PHASE8_GARBAGE_RE.md:204`, `docs/PHASE8_GARBAGE_RE.md:259`, `docs/PHASE8_GARBAGE_RE.md:278`, `src/LandfillRollover.h:12` |
| `0x006B92B0` | `docs/PHASE8_GARBAGE_RE.md:204` |
| `0x006B92F6` | `docs/PHASE8_GARBAGE_RE.md:129` |
| `0x006B9360` | `src/LandfillZoneModel.h:15` |
| `0x006B96E0` | `docs/LANDFILL_ZONE_RE.md:54`, `docs/LANDFILL_ZONE_RE.md:245`, `src/LandfillZoneModel.h:15` |
| `0x006B9880` | `docs/LANDFILL_ZONE_RE.md:286` |
| `0x006B9900` | `docs/LANDFILL_ZONE_RE.md:286` |
| `0x006B9AC0` | `docs/LANDFILL_ZONE_RE.md:37`, `docs/LANDFILL_ZONE_RE.md:243`, `docs/PHASE8_GARBAGE_RE.md:187`, `src/LandfillSpatial.h:167`, `src/LandfillZoneModel.h:12` |
| `0x006B9C88` | `docs/PHASE8_GARBAGE_RE.md:187` |
| `0x006B9EA0` | `docs/PHASE8_GARBAGE_RE.md:200`, `docs/PHASE8_GARBAGE_RE.md:297`, `src/LandfillResearch.cpp:389` |
| `0x006B9EE8` | `docs/PHASE6_OPTIMIZER.md:41` |
| `0x006B9F93` | `docs/LANDFILL_ZONE_RE.md:242` |
| `0x006BA071` | `docs/PHASE8_GARBAGE_RE.md:200` |
| `0x006BA0EC` | `src/LandfillRollover.h:7` |
| `0x006BA0EF` | `docs/PHASE8_GARBAGE_RE.md:200` |
| `0x006BA330` | `docs/PHASE8_GARBAGE_RE.md:208`, `docs/PHASE8_GARBAGE_RE.md:212`, `docs/PHASE8_GARBAGE_RE.md:298`, `src/LandfillRollover.h:8`, `src/LandfillRollover.h:32`, `src/LandfillResearch.cpp:392` |
| `0x006BA34D` | `docs/PHASE8_GARBAGE_RE.md:186` |
| `0x006BA376` | `docs/PHASE8_GARBAGE_RE.md:190`, `docs/PHASE8_GARBAGE_RE.md:267` |
| `0x006BA3C0` | `docs/LANDFILL_ZONE_RE.md:61`, `docs/PHASE8_GARBAGE_RE.md:191`, `src/LandfillZoneModel.h:33`, `src/LandfillZoneReader.cpp:30`, `src/LandfillZoneReader.cpp:104` |
| `0x006BA3DE` | `docs/PHASE8_GARBAGE_RE.md:191` |
| `0x006BA415` | `docs/PHASE8_GARBAGE_RE.md:191` |
| `0x006BA470` | `docs/LANDFILL_ZONE_RE.md:244` |
| `0x006BA5C0` | `docs/LANDFILL_ZONE_RE.md:159`, `docs/LANDFILL_ZONE_RE.md:244`, `src/LandfillZoneModel.h:15` |
| `0x006BA810` | `docs/PHASE8_GARBAGE_RE.md:204` |
| `0x006BA900` | `docs/LANDFILL_SPATIAL.md:56`, `docs/LANDFILL_ZONE_RE.md:286`, `docs/LANDFILL_ZONE_RE.md:287` |
| `0x006BAA70` | `docs/LANDFILL_ZONE_RE.md:15`, `docs/LANDFILL_ZONE_RE.md:60`, `docs/LANDFILL_ZONE_RE.md:238`, `docs/PHASE8_GARBAGE_RE.md:203`, `src/LandfillZoneReader.h:9` |
| `0x006BAB30` | `docs/LANDFILL_ZONE_RE.md:15`, `docs/LANDFILL_ZONE_RE.md:23`, `docs/PHASE8_GARBAGE_RE.md:207`, `src/LandfillZoneReader.h:9`, `src/LandfillZoneReader.cpp:63` |
| `0x006BB180` | `docs/LANDFILL_SPATIAL.md:9`, `docs/LANDFILL_ZONE_RE.md:24`, `src/LandfillZoneModel.h:10`, `src/LandfillZoneModel.h:43` |
| `0x006BB1D0` | `docs/LANDFILL_ZONE_RE.md:245` |
| `0x006BB830` | `docs/LANDFILL_ZONE_RE.md:15`, `docs/LANDFILL_ZONE_RE.md:25`, `docs/LANDFILL_ZONE_RE.md:37`, `docs/LANDFILL_ZONE_RE.md:41`, `docs/LANDFILL_ZONE_RE.md:244`, `src/LandfillZoneModel.h:10`, `src/LandfillZoneModel.h:12`, `src/LandfillZoneModel.h:105`, `src/LandfillZoneReader.h:9` |
| `0x006BB8F0` | `docs/PHASE8_GARBAGE_RE.md:209`, `docs/PHASE8_GARBAGE_RE.md:259`, `docs/PHASE8_GARBAGE_RE.md:278`, `src/LandfillRollover.h:12` |
| `0x006BBD60` | `docs/LANDFILL_ZONE_RE.md:55`, `src/LandfillZoneReader.cpp:28` |
| `0x006BBEF0` | `docs/LANDFILL_SPATIAL.md:56`, `docs/LANDFILL_ZONE_RE.md:287`, `docs/LANDFILL_ZONE_RE.md:288` |
| `0x006BC150` | `docs/LANDFILL_ZONE_RE.md:15`, `docs/LANDFILL_ZONE_RE.md:37`, `docs/LANDFILL_ZONE_RE.md:243`, `src/LandfillZoneModel.h:12`, `src/LandfillZoneReader.h:9` |
| `0x006BC46F` | `docs/LANDFILL_ZONE_RE.md:44` |
| `0x006BC530` | `docs/LANDFILL_ZONE_RE.md:44` |
| `0x006BC576` | `docs/LANDFILL_ZONE_RE.md:44` |
| `0x006BCAA0` | `src/LandfillZoneModel.h:15` |
| `0x006BCCB8` | `docs/PHASE8_GARBAGE_RE.md:192` |
| `0x006BCCE0` | `docs/PHASE8_GARBAGE_RE.md:192` |
| `0x006BCD70` | `docs/LANDFILL_SPATIAL.md:9`, `docs/LANDFILL_ZONE_RE.md:37`, `docs/LANDFILL_ZONE_RE.md:50`, `docs/LANDFILL_ZONE_RE.md:237`, `docs/LANDFILL_ZONE_RE.md:243`, `docs/LANDFILL_ZONE_RE.md:245`, `src/LandfillSpatial.h:16`, `src/LandfillZoneModel.h:12` |
| `0x006BD390` | `docs/PHASE8_GARBAGE_RE.md:189`, `docs/PHASE8_GARBAGE_RE.md:204`, `docs/PHASE8_GARBAGE_RE.md:207`, `docs/PHASE8_GARBAGE_RE.md:298`, `src/LandfillRollover.h:7`, `src/LandfillRollover.h:9`, `src/LandfillRollover.h:46`, `src/LandfillResearch.cpp:391` |
| `0x006BD3EC` | `docs/PHASE8_GARBAGE_RE.md:189`, `docs/PHASE8_GARBAGE_RE.md:267` |
| `0x006BD44F` | `docs/PHASE8_GARBAGE_RE.md:230` |
| `0x006BD452` | `docs/PHASE8_GARBAGE_RE.md:342` |
| `0x006BD458` | `docs/PHASE8_GARBAGE_RE.md:216` |
| `0x006C9780` | `docs/FARM_WATER_ZOT.md:13`, `src/FarmWaterZot.h:9` |
| `0x006C98C0` | `docs/FARM_WATER_ZOT.md:12`, `src/FarmWaterZot.h:9` |
| `0x006CABB0` | `docs/FARM_WATER_ZOT.md:9` |
| `0x006CAE30` | `docs/FARM_WATER_ZOT.md:8`, `docs/FARM_WATER_ZOT.md:10`, `src/FarmWaterZot.h:7` |
| `0x006CAF27` | `docs/FARM_WATER_ZOT.md:11`, `docs/FARM_WATER_ZOT.md:16`, `docs/FARM_WATER_ZOT.md:19`, `src/FarmWaterZot.h:8`, `src/FarmWaterZot.h:17`, `src/FarmWaterZot.cpp:22`, `src/FarmWaterZot.cpp:78`, `src/FarmWaterZot.cpp:96` |
| `0x006CAF3E` | `docs/FARM_WATER_ZOT.md:8`, `docs/FARM_WATER_ZOT.md:19`, `src/FarmWaterZot.cpp:25`, `src/FarmWaterZot.cpp:78` |
| `0x006CAF5C` | `docs/FARM_WATER_ZOT.md:13`, `src/FarmWaterZot.cpp:22` |
| `0x006CD45A` | `docs/FARM_WATER_ZOT.md:9` |
| `0x006CD511` | `docs/FARM_WATER_ZOT.md:9` |
| `0x006CD531` | `docs/FARM_WATER_ZOT.md:8` |
| `0x006CD678` | `docs/FARM_WATER_ZOT.md:9` |
| `0x006CDFE0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x006CE2E0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21` |
| `0x006CE610` | `docs/PHASE6_OPTIMIZER.md:43`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:47` |
| `0x006CE640` | `docs/PHASE6_OPTIMIZER.md:43`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:48` |
| `0x006CE6A0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:108` |
| `0x006CF270` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:20` |
| `0x006CF360` | `docs/PHASE7_POWER_DEAL_RE.md:143`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:49` |
| `0x006CF3E0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21` |
| `0x006CF5C0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:41`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:88` |
| `0x006CF780` | `docs/PHASE6_OPTIMIZER.md:43`, `docs/PHASE7_POWER_DEAL_RE.md:14`, `docs/PHASE7_POWER_DEAL_RE.md:147`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x006CF860` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:136`, `src/DealAccounting.h:12` |
| `0x006CF8D0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:136`, `src/DealAccounting.h:13` |
| `0x006CF960` | `docs/PHASE6_OPTIMIZER.md:43`, `docs/PHASE7_POWER_DEAL_RE.md:147`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:45` |
| `0x006CF9C0` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:46`, `src/DealQuote.h:99` |
| `0x006CFA50` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:21` |
| `0x006CFD70` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:72` |
| `0x006D0520` | `docs/PHASE7_POWER_DEAL_RE.md:95`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:54`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:56`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:80`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:143` |
| `0x006D0760` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x006D0840` | `docs/PHASE7_POWER_DEAL_RE.md:16`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x006D09B8` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:106` |
| `0x006D0E50` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:25`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:36`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:41`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:62`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:80`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:108` |
| `0x006D0E97` | `docs/PHASE7_POWER_DEAL_RE.md:143`, `docs/PHASE7_POWER_DEAL_RE.md:146` |
| `0x006D0F68` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:171`, `src/DealQuote.h:125` |
| `0x006D0FD3` | `docs/PHASE7_POWER_DEAL_RE.md:143` |
| `0x006D1018` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:171`, `src/DealQuote.h:125` |
| `0x006D1089` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:171` |
| `0x006D1128` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:171` |
| `0x006D1177` | `docs/PHASE7_POWER_DEAL_RE.md:146`, `docs/PHASE7_POWER_DEAL_RE.md:180` |
| `0x006D120E` | `docs/PHASE7_POWER_DEAL_RE.md:146` |
| `0x006D12B7` | `docs/PHASE7_POWER_DEAL_RE.md:146` |
| `0x006D1FC0` | `docs/PHASE7_POWER_DEAL_RE.md:13`, `docs/PHASE7_POWER_DEAL_RE.md:107`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x006D2051` | `docs/PHASE7_POWER_DEAL_RE.md:109` |
| `0x006D20F9` | `docs/PHASE7_POWER_DEAL_RE.md:112` |
| `0x006D2100` | `docs/PHASE7_POWER_DEAL_RE.md:111` |
| `0x006D2180` | `docs/PHASE7_POWER_DEAL_RE.md:109` |
| `0x006D21A0` | `docs/PHASE7_POWER_DEAL_RE.md:15`, `docs/PHASE7_POWER_DEAL_RE.md:73`, `docs/PHASE7_POWER_DEAL_RE.md:75`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:54`, `src/DealQuote.h:6`, `src/NeighborSellModel.h:6` |
| `0x006D21B6` | `docs/PHASE7_POWER_DEAL_RE.md:76` |
| `0x006D21C7` | `docs/PHASE7_POWER_DEAL_RE.md:77` |
| `0x006D22B9` | `docs/PHASE7_POWER_DEAL_RE.md:80` |
| `0x006D2358` | `docs/PHASE7_POWER_DEAL_RE.md:82` |
| `0x006D2440` | `docs/PHASE7_POWER_DEAL_RE.md:86` |
| `0x006D2445` | `docs/PHASE7_POWER_DEAL_RE.md:84`, `src/DealQuote.h:111` |
| `0x006D2459` | `docs/PHASE7_POWER_DEAL_RE.md:87` |
| `0x006D2462` | `docs/PHASE7_POWER_DEAL_RE.md:87` |
| `0x006D2470` | `docs/PHASE7_POWER_DEAL_RE.md:88` |
| `0x006D2483` | `docs/PHASE7_POWER_DEAL_RE.md:89` |
| `0x006D2494` | `docs/PHASE7_POWER_DEAL_RE.md:90`, `docs/PHASE7_POWER_DEAL_RE.md:155` |
| `0x006D24B5` | `docs/PHASE7_POWER_DEAL_RE.md:155` |
| `0x006D24C6` | `docs/PHASE7_POWER_DEAL_RE.md:91` |
| `0x006D24D0` | `docs/PHASE7_POWER_DEAL_RE.md:92` |
| `0x006D24DD` | `docs/PHASE7_POWER_DEAL_RE.md:92` |
| `0x006D24DF` | `docs/PHASE7_POWER_DEAL_RE.md:93` |
| `0x006D24EC` | `docs/PHASE7_POWER_DEAL_RE.md:93` |
| `0x006D24EE` | `docs/PHASE7_POWER_DEAL_RE.md:94` |
| `0x006D24FF` | `docs/PHASE7_POWER_DEAL_RE.md:94` |
| `0x006D2517` | `docs/PHASE7_POWER_DEAL_RE.md:84` |
| `0x006D2519` | `docs/PHASE7_POWER_DEAL_RE.md:90` |
| `0x006D2528` | `docs/PHASE7_POWER_DEAL_RE.md:97` |
| `0x006D254E` | `docs/PHASE7_POWER_DEAL_RE.md:98` |
| `0x006D2571` | `docs/PHASE7_POWER_DEAL_RE.md:155` |
| `0x006D2592` | `docs/PHASE7_POWER_DEAL_RE.md:155` |
| `0x006D25F2` | `docs/PHASE7_POWER_DEAL_RE.md:97`, `src/DealQuote.h:111` |
| `0x006D26EF` | `docs/PHASE7_POWER_DEAL_RE.md:175` |
| `0x006D2856` | `docs/PHASE7_POWER_DEAL_RE.md:175` |
| `0x006D285F` | `docs/PHASE7_POWER_DEAL_RE.md:76` |
| `0x006D2870` | `docs/PHASE5_UTILITIES.md:98`, `docs/PHASE6_OPTIMIZER.md:43`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:58` |
| `0x006D2A20` | `docs/PHASE7_POWER_DEAL_RE.md:118`, `docs/PHASE7_POWER_DEAL_RE.md:125`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:169`, `src/DealQuote.h:6` |
| `0x006D2A7B` | `docs/PHASE7_POWER_DEAL_RE.md:128` |
| `0x006D2A87` | `docs/PHASE7_POWER_DEAL_RE.md:128` |
| `0x006D2AA1` | `docs/PHASE7_POWER_DEAL_RE.md:128` |
| `0x006D2AB8` | `docs/PHASE7_POWER_DEAL_RE.md:128` |
| `0x006D2B70` | `docs/PHASE7_POWER_DEAL_RE.md:133` |
| `0x006D2D25` | `docs/PHASE7_POWER_DEAL_RE.md:129` |
| `0x006D2D57` | `docs/PHASE7_POWER_DEAL_RE.md:132` |
| `0x006D2D83` | `docs/PHASE7_POWER_DEAL_RE.md:133`, `src/DealQuote.h:105` |
| `0x006D2DBA` | `docs/PHASE7_POWER_DEAL_RE.md:133`, `src/DealQuote.h:105` |
| `0x006D2E10` | `docs/PHASE7_POWER_DEAL_RE.md:132` |
| `0x006D2E49` | `docs/PHASE7_POWER_DEAL_RE.md:138` |
| `0x006D2E75` | `docs/PHASE7_POWER_DEAL_RE.md:129` |
| `0x006D30F3` | `docs/PHASE6_OPTIMIZER.md:41` |
| `0x006D3111` | `docs/PHASE7_POWER_DEAL_RE.md:118` |
| `0x006D3143` | `docs/PHASE7_POWER_DEAL_RE.md:118` |
| `0x006D3148` | `docs/PHASE7_POWER_DEAL_RE.md:118` |
| `0x006D3158` | `docs/PHASE7_POWER_DEAL_RE.md:118` |
| `0x006D3221` | `docs/PHASE7_POWER_DEAL_RE.md:138` |
| `0x006D3240` | `docs/PHASE7_POWER_DEAL_RE.md:127` |
| `0x006D3250` | `docs/PHASE7_POWER_DEAL_RE.md:17`, `docs/PHASE7_POWER_DEAL_RE.md:52`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18`, `src/DealQuote.h:6`, `src/NeighborSellModel.h:6`, `src/UtilityOptimizer.h:546` |
| `0x006D32A6` | `docs/PHASE7_POWER_DEAL_RE.md:56` |
| `0x006D34AD` | `docs/PHASE7_POWER_DEAL_RE.md:62` |
| `0x006D34B1` | `docs/PHASE7_POWER_DEAL_RE.md:62` |
| `0x006D3549` | `docs/PHASE7_POWER_DEAL_RE.md:57` |
| `0x006D35A2` | `docs/PHASE7_POWER_DEAL_RE.md:66` |
| `0x006D35D8` | `docs/PHASE7_POWER_DEAL_RE.md:66` |
| `0x006D35ED` | `docs/PHASE7_POWER_DEAL_RE.md:67` |
| `0x006D35FC` | `docs/PHASE7_POWER_DEAL_RE.md:23` |
| `0x006D3620` | `docs/PHASE7_POWER_DEAL_RE.md:18`, `docs/PHASE7_POWER_DEAL_RE.md:27`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18`, `src/DealQuote.h:6`, `src/NeighborSellModel.h:6` |
| `0x006D364D` | `docs/PHASE7_POWER_DEAL_RE.md:36` |
| `0x006D3659` | `docs/PHASE7_POWER_DEAL_RE.md:31` |
| `0x006D36B9` | `docs/PHASE7_POWER_DEAL_RE.md:25` |
| `0x006D376A` | `docs/PHASE7_POWER_DEAL_RE.md:32` |
| `0x006D3788` | `docs/PHASE7_POWER_DEAL_RE.md:32` |
| `0x006D37CD` | `docs/PHASE7_POWER_DEAL_RE.md:37` |
| `0x006D37E1` | `docs/PHASE7_POWER_DEAL_RE.md:38` |
| `0x006D37EC` | `docs/PHASE7_POWER_DEAL_RE.md:38` |
| `0x006D37F6` | `docs/PHASE7_POWER_DEAL_RE.md:39` |
| `0x006D3801` | `docs/PHASE7_POWER_DEAL_RE.md:39` |
| `0x006D3823` | `docs/PHASE7_POWER_DEAL_RE.md:39` |
| `0x006D382B` | `docs/PHASE7_POWER_DEAL_RE.md:40` |
| `0x006D386A` | `docs/PHASE7_POWER_DEAL_RE.md:40` |
| `0x006D386E` | `docs/PHASE7_POWER_DEAL_RE.md:41` |
| `0x006D3872` | `docs/PHASE7_POWER_DEAL_RE.md:41` |
| `0x006D388B` | `docs/PHASE7_POWER_DEAL_RE.md:41` |
| `0x006D38B8` | `docs/PHASE7_POWER_DEAL_RE.md:23` |
| `0x006D38F0` | `docs/PHASE7_POWER_DEAL_RE.md:117` |
| `0x006D3961` | `docs/PHASE7_POWER_DEAL_RE.md:117` |
| `0x006D3975` | `docs/PHASE7_POWER_DEAL_RE.md:117` |
| `0x006D44D6` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:72` |
| `0x006D4533` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:41` |
| `0x006D462F` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:41` |
| `0x006D4670` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:18` |
| `0x006D4760` | `docs/PHASE7_POWER_DEAL_RE.md:110` |
| `0x006D4770` | `docs/PHASE7_POWER_DEAL_RE.md:110` |
| `0x006DC9E0` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:40` |
| `0x006DF060` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:72` |
| `0x006DF253` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:7` |
| `0x006DF258` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:7` |
| `0x006E3000` | `docs/PHASE8_GARBAGE_RE.md:14` |
| `0x006E3150` | `docs/PHASE8_GARBAGE_RE.md:16` |
| `0x006E3250` | `docs/PHASE8_GARBAGE_RE.md:210`, `src/LandfillResearch.cpp:395` |
| `0x006E32E0` | `docs/PHASE8_GARBAGE_RE.md:15`, `docs/PHASE8_GARBAGE_RE.md:104`, `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:224`, `src/LandfillResearch.cpp:394` |
| `0x006E3340` | `docs/PHASE8_GARBAGE_RE.md:11`, `docs/PHASE8_GARBAGE_RE.md:225` |
| `0x006E3820` | `docs/PHASE8_GARBAGE_RE.md:51`, `docs/PHASE8_GARBAGE_RE.md:52`, `docs/PHASE8_GARBAGE_RE.md:118`, `src/GarbageControl.h:7` |
| `0x006E3850` | `docs/PHASE8_GARBAGE_RE.md:64` |
| `0x006E38A8` | `docs/PHASE8_GARBAGE_RE.md:59` |
| `0x006E38BF` | `docs/PHASE8_GARBAGE_RE.md:59` |
| `0x006E3963` | `docs/PHASE8_GARBAGE_RE.md:59` |
| `0x006E3A1E` | `docs/PHASE8_GARBAGE_RE.md:59` |
| `0x006E3A52` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:25`, `docs/PHASE8_GARBAGE_RE.md:118` |
| `0x006E3A73` | `docs/PHASE8_GARBAGE_RE.md:121` |
| `0x006E3A93` | `docs/PHASE8_GARBAGE_RE.md:122` |
| `0x006E3A9F` | `docs/PHASE8_GARBAGE_RE.md:123`, `docs/PHASE8_GARBAGE_RE.md:196` |
| `0x006E3AC7` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:25`, `docs/PHASE8_GARBAGE_RE.md:118`, `docs/PHASE8_GARBAGE_RE.md:123` |
| `0x006E3B1A` | `docs/PHASE8_GARBAGE_RE.md:124`, `docs/PHASE8_GARBAGE_RE.md:227` |
| `0x006E3B1D` | `docs/PHASE8_GARBAGE_RE.md:339` |
| `0x006E3B34` | `docs/PHASE8_GARBAGE_RE.md:227` |
| `0x006E3B37` | `docs/PHASE8_GARBAGE_RE.md:340` |
| `0x006E3B69` | `docs/PHASE8_GARBAGE_RE.md:98` |
| `0x006E3B94` | `docs/PHASE8_GARBAGE_RE.md:59` |
| `0x006E3BA9` | `docs/PHASE8_GARBAGE_RE.md:59` |
| `0x006E3BB5` | `docs/PHASE8_GARBAGE_RE.md:64` |
| `0x006E415D` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x006E4300` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:24`, `docs/PHASE8_GARBAGE_FUNDING_RE.md:34` |
| `0x006E4410` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:34` |
| `0x006E44B0` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:18`, `src/SanitationWriterPlan.h:122` |
| `0x006E4516` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:18`, `src/SanitationWriterPlan.h:122` |
| `0x006E4C0B` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x006E4F10` | `docs/PHASE8_GARBAGE_RE.md:12` |
| `0x006E4F11` | `docs/PHASE8_GARBAGE_RE.md:195` |
| `0x006E4F20` | `docs/PHASE8_GARBAGE_RE.md:17` |
| `0x006E4F30` | `docs/PHASE8_GARBAGE_RE.md:18` |
| `0x006E4F40` | `docs/PHASE8_GARBAGE_RE.md:19` |
| `0x006E4F50` | `docs/PHASE8_GARBAGE_RE.md:20` |
| `0x006E4F60` | `docs/PHASE8_GARBAGE_RE.md:21` |
| `0x006E4F70` | `docs/PHASE8_GARBAGE_RE.md:22` |
| `0x006E4F80` | `docs/PHASE8_GARBAGE_RE.md:23` |
| `0x006E4F90` | `docs/PHASE8_GARBAGE_RE.md:24` |
| `0x006E5040` | `docs/PHASE8_GARBAGE_RE.md:13` |
| `0x006E5050` | `docs/PHASE8_GARBAGE_RE.md:205`, `src/LandfillResearch.cpp:396` |
| `0x006E5B10` | `docs/PHASE8_GARBAGE_RE.md:127`, `docs/PHASE8_GARBAGE_RE.md:194` |
| `0x006E5BC4` | `docs/PHASE8_GARBAGE_RE.md:194` |
| `0x006E5E16` | `docs/PHASE8_GARBAGE_RE.md:194` |
| `0x006E5E1A` | `docs/PHASE8_GARBAGE_RE.md:194` |
| `0x006E5E2A` | `docs/PHASE8_GARBAGE_RE.md:195` |
| `0x006E5EA8` | `docs/PHASE8_GARBAGE_RE.md:127`, `docs/PHASE8_GARBAGE_RE.md:196` |
| `0x006E72A6` | `docs/PHASE8_GARBAGE_RE.md:104`, `docs/PHASE8_GARBAGE_RE.md:226`, `docs/PHASE8_GARBAGE_RE.md:343` |
| `0x006E7737` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:7` |
| `0x006E773D` | `docs/PHASE6_OPTIMIZER.md:41` |
| `0x006E7741` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:7` |
| `0x006E7799` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x006E77C6` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x006E77F2` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x006E7B5D` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:21` |
| `0x006E7B60` | `docs/PHASE8_GARBAGE_RE.md:30`, `docs/PHASE8_GARBAGE_RE.md:32`, `docs/PHASE8_GARBAGE_RE.md:239`, `docs/PHASE8_GARBAGE_RE.md:295`, `docs/RUNTIME_12A272A_SANITATION.md:60`, `src/LandfillResearch.cpp:393` |
| `0x006E7FAC` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:72` |
| `0x006E7FD0` | `docs/PHASE8_GARBAGE_RE.md:30`, `docs/PHASE8_GARBAGE_RE.md:47`, `docs/RUNTIME_12A272A_SANITATION.md:60` |
| `0x006E805D` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:34` |
| `0x006E8123` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:34` |
| `0x006E83DA` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:228` |
| `0x006E83DD` | `docs/PHASE8_GARBAGE_RE.md:341`, `docs/PHASE8_GARBAGE_RE.md:455` |
| `0x006E889C` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:228` |
| `0x006E892A` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:229` |
| `0x006E89BC` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:229` |
| `0x006E8CB0` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:229` |
| `0x006E8CE4` | `docs/PHASE8_GARBAGE_RE.md:194` |
| `0x006E8CF9` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:229` |
| `0x006EB2F0` | `docs/PHASE6_OPTIMIZER.md:9`, `docs/UTILITY_COMMUNITY_BASELINE.md:132`, `src/UtilityHealth.h:12` |
| `0x006EB520` | `docs/PHASE5_UTILITIES.md:53` |
| `0x006EB580` | `docs/PHASE5_UTILITIES.md:85` |
| `0x006EB5F0` | `docs/PHASE5_UTILITIES.md:50`, `docs/UTILITY_COMMUNITY_BASELINE.md:110`, `src/UtilityProbe.cpp:82` |
| `0x006EC820` | `src/UtilityProbe.cpp:82` |
| `0x006EC8F0` | `docs/PHASE6_OPTIMIZER.md:25`, `docs/PRODUCT_SPEC.md:123`, `docs/UTILITY_OPTIMIZER.md:98`, `src/UtilityHealth.h:10` |
| `0x006ECB02` | `docs/PHASE6_OPTIMIZER.md:37` |
| `0x006ED211` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:40` |
| `0x006EF350` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:72` |
| `0x006F7E80` | `docs/PHASE1_2_PROBE.md:82`, `docs/REVERSE_ENGINEERING_PLAN.md:64`, `docs/SC4_INTEGRATION.md:85`, `src/NativeCivicQueryReader.cpp:17` |
| `0x006F8000` | `docs/PHASE8_GARBAGE_RE.md:220` |
| `0x006F8290` | `docs/CIVIC_QUERY_MAP.md:53`, `docs/CIVIC_QUERY_MAP.md:72`, `src/CivicProbe.cpp:119` |
| `0x006F8360` | `docs/CIVIC_QUERY_MAP.md:53`, `src/CivicProbe.cpp:119` |
| `0x006F91C0` | `docs/PHASE1_2_PROBE.md:82`, `docs/REVERSE_ENGINEERING_PLAN.md:64`, `docs/SC4_INTEGRATION.md:85`, `src/NativeCivicQueryReader.cpp:17` |
| `0x006FA280` | `docs/CIVIC_STRIKE.md:5`, `docs/PROJECT_CLOSURE_REPORT.md:113`, `src/CivicFacilityIO.h:45` |
| `0x006FA31D` | `docs/CIVIC_STRIKE.md:6` |
| `0x006FAFF0` | `docs/PROJECT_CLOSURE_REPORT.md:113` |
| `0x006FD510` | `docs/PHASE1_2_PROBE.md:82`, `docs/REVERSE_ENGINEERING_PLAN.md:64`, `docs/SC4_INTEGRATION.md:85`, `docs/SC4_INTEGRATION.md:91` |
| `0x006FF520` | `docs/PROJECT_CLOSURE_REPORT.md:113` |
| `0x00762698` | `docs/POWER_GARBAGE_CLOSURE.md:234`, `docs/POWER_GARBAGE_CLOSURE.md:258` |
| `0x00770000` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:11` |
| `0x0077AB40` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:67` |
| `0x0077DAB0` | `docs/CURRENT_STATUS.md:202`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:112`, `src/DealQuantity.h:8` |
| `0x0077DBC9` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:115` |
| `0x0077DBE5` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:116` |
| `0x0077DC9F` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:118` |
| `0x0077DD44` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:119` |
| `0x0077DDCE` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:120` |
| `0x0077DE47` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:122` |
| `0x0077DF26` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:122` |
| `0x0077DF93` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:120` |
| `0x0077DFA2` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:123` |
| `0x0077DFB9` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:123` |
| `0x0077DFC1` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:124` |
| `0x0077DFDD` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:124` |
| `0x0077DFE3` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:125` |
| `0x0077E290` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:125` |
| `0x0077E295` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:126` |
| `0x0077E600` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:62`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:88` |
| `0x0077E6C5` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:106` |
| `0x00781C90` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:65`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:88` |
| `0x00785740` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11` |
| `0x0078695A` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x00788E3B` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x0078A7D7` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:9` |
| `0x0078B3F5` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:88` |
| `0x0078B4B5` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:88` |
| `0x0078BCA0` | `docs/CURRENT_STATUS.md:202`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:63`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:112`, `src/DealQuantity.h:9`, `src/DealQuantity.h:128` |
| `0x0078BEA5` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:129` |
| `0x0078BF8C` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:129` |
| `0x0078C053` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:11`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:88` |
| `0x00798370` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:34` |
| `0x007B0000` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:11` |
| `0x0080B000` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:11` |
| `0x0080EC3A` | `docs/PHASE6_OPTIMIZER.md:41` |
| `0x00811000` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:11` |
| `0x009F066E` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:23` |
| `0x00A81228` | `docs/PHASE7_POWER_DEAL_RE.md:37` |
| `0x00A92488` | `docs/CIVIC_QUERY_MAP.md:13`, `docs/PHASE1_2_PROBE.md:80`, `docs/SC4_INTEGRATION.md:72` |
| `0x00A92570` | `docs/UTILITY_COMMUNITY_BASELINE.md:102` |
| `0x00A92D28` | `docs/PHASE7_POWER_DEAL_RE.md:66` |
| `0x00AA6988` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:33`, `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:40`, `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:43`, `docs/POWER_GARBAGE_CLOSURE.md:260` |
| `0x00AB1554` | `docs/LANDFILL_ZONE_RE.md:286` |
| `0x00AB15D4` | `docs/LANDFILL_ZONE_RE.md:24` |
| `0x00AB15DC` | `docs/LANDFILL_SPATIAL.md:19`, `docs/LANDFILL_ZONE_RE.md:14`, `docs/PHASE8_GARBAGE_RE.md:309`, `src/LandfillZoneReader.h:8`, `src/LandfillZoneReader.cpp:29` |
| `0x00AB1610` | `docs/LANDFILL_SPATIAL.md:19`, `src/LandfillZoneReader.h:8`, `src/LandfillZoneReader.cpp:28` |
| `0x00AB1DD8` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:12`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:20` |
| `0x00AB1EE0` | `docs/PHASE7_POWER_DEAL_RE.md:7`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:12`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:17` |
| `0x00AB1F80` | `docs/PHASE7_POWER_DEAL_RE.md:133` |
| `0x00AB1F88` | `docs/PHASE7_POWER_DEAL_RE.md:56` |
| `0x00AB2034` | `docs/PHASE7_POWER_DEAL_RE.md:110` |
| `0x00AB2550` | `docs/PHASE8_GARBAGE_RE.md:49` |
| `0x00AB2630` | `docs/PHASE8_GARBAGE_RE.md:7` |
| `0x00AB26A4` | `docs/PHASE8_GARBAGE_RE.md:220` |
| `0x00AB2C10` | `docs/SC4_INTEGRATION.md:85` |
| `0x00B0D2A4` | `docs/PHASE8_GARBAGE_RE.md:128`, `docs/PHASE8_GARBAGE_RE.md:186`, `docs/PHASE8_GARBAGE_RE.md:212`, `src/LandfillResearch.cpp:159` |
| `0x00B0D2A8` | `docs/LANDFILL_ZONE_RE.md:55`, `docs/PHASE8_GARBAGE_RE.md:187` |
| `0x00B0D2AC` | `docs/PHASE8_GARBAGE_RE.md:128`, `docs/PHASE8_GARBAGE_RE.md:188`, `docs/PHASE8_GARBAGE_RE.md:202`, `src/LandfillResearch.cpp:159`, `src/LandfillResearch.cpp:214` |
| `0x00B0D2B0` | `docs/PHASE8_GARBAGE_RE.md:128`, `docs/PHASE8_GARBAGE_RE.md:189`, `docs/PHASE8_GARBAGE_RE.md:207`, `src/LandfillResearch.cpp:159`, `src/LandfillResearch.cpp:213`, `src/LandfillResearch.cpp:214` |
| `0x00B0D2B4` | `docs/PHASE8_GARBAGE_RE.md:128`, `docs/PHASE8_GARBAGE_RE.md:190`, `docs/PHASE8_GARBAGE_RE.md:213`, `src/LandfillResearch.cpp:159` |
| `0x00B0D2B8` | `docs/PHASE8_GARBAGE_RE.md:192` |
| `0x00B0D2BC` | `docs/PHASE8_GARBAGE_RE.md:193` |
| `0x00B43C94` | `docs/PHASE7_POWER_DEAL_RE.md:29`, `docs/PHASE7_POWER_DEAL_RE.md:78`, `docs/PHASE7_POWER_DEAL_RE.md:127`, `docs/PHASE7_POWER_DEAL_RE.md:182` |
| `0x00B43CCC` | `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:41` |
| `0x00B43CEC` | `docs/PHASE7_POWER_DEAL_RE.md:77`, `docs/PHASE7_POWER_DEAL_RE.md:127`, `docs/PHASE7_POWER_DEAL_RE.md:147` |
| `0x00B43D8C` | `docs/PHASE7_POWER_DEAL_RE.md:145`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:17`, `docs/PHASE8_DEAL_TRANSACTIONS_RE.md:106` |
| `0x00B43D94` | `docs/PHASE7_POWER_DEAL_RE.md:134` |
| `0x00B43D9C` | `docs/PHASE7_POWER_DEAL_RE.md:134`, `docs/PHASE8_GARBAGE_RE.md:7` |
| `0x00B43DA0` | `docs/PHASE7_POWER_DEAL_RE.md:129` |
| `0x00B43DA4` | `docs/CIVIC_QUERY_MAP.md:13` |
| `0x00B43DC0` | `docs/PHASE8_GARBAGE_RE.md:45` |
| `0x00B49030` | `docs/PHASE8_GARBAGE_FUNDING_RE.md:40`, `src/SanitationWriterPlan.h:10` |
| `0x00B490A8` | `docs/PHASE8_GARBAGE_RE.md:191` |
| `0x00B49B68` | `docs/PHASE8_GARBAGE_RE.md:129`, `docs/PHASE8_GARBAGE_RE.md:194`, `docs/PHASE8_GARBAGE_RE.md:238`, `src/LandfillResearch.cpp:160` |
| `0x00B49B70` | `docs/PHASE8_GARBAGE_RE.md:64`, `docs/PHASE8_GARBAGE_RE.md:65` |
| `0x00B49BA4` | `docs/PHASE8_GARBAGE_RE.md:12`, `docs/PHASE8_GARBAGE_RE.md:195`, `src/LandfillResearch.cpp:160` |
| `0x00B49BC0` | `docs/PHASE8_GARBAGE_RE.md:127`, `docs/PHASE8_GARBAGE_RE.md:196` |
| `0x00B62698` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:31` |
| `0x00B626A0` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:31` |
| `0x00B626D4` | `docs/CRASH_ANALYSIS_CONNECTA_20261013.md:75`, `docs/POWER_GARBAGE_CLOSURE.md:303` |

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
