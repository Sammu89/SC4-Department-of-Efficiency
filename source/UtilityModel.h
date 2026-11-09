// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure (game-free) model of the unified utility snapshot: Power, Water, Garbage, Neighbor deals.
// Missing stays missing: every reading that can fail is an Opt<T>, never a silently defaulted 0.

#include "CommodityBook.h"
#include "LandfillGeometry.h"
#include "NeighborSellModel.h"
#include "SnapshotValidity.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace sc4af {

template <class T> using Opt = std::optional<T>;

// ---- vanilla identifiers ------------------------------------
namespace utility_id {
constexpr std::uint32_t kPowerDepartment = 0x8910BC8Au;
constexpr std::uint32_t kWaterDepartment = 0xAA4014B4u;
constexpr std::uint32_t kGarbageDepartment = 0x6A42A8F5u;
constexpr std::uint32_t kPowerPurpose = 0xCA58E540u;
constexpr std::uint32_t kWaterPurpose = 0xCA58C9D5u;
constexpr std::uint32_t kGarbagePurpose = 0xCA58EDC7u;
// Occupant groups (exemplar property 0xAA1DD396)
constexpr std::uint32_t kGroupPowerPlant = 0x1400u;
constexpr std::uint32_t kGroupWaterSource = 0x1401u;
constexpr std::uint32_t kGroupWasteHandling = 0x1402u;  // Waste-to-Energy, recycling, water purification
constexpr std::uint32_t kGroupNuclear = 0x1403u;
constexpr std::uint32_t kGroupRecycling = 0x1404u;
constexpr std::uint32_t kGroupLandfill = 0x1406u;
// Neighbor deal types = cISC4NeighborDeal::GetType() (executable 1.1.641.0, function 0x00421AB0)
constexpr std::int32_t kDealBuyPower = 0x4773FFE0;
constexpr std::int32_t kDealSellPower = 0x4773FFE1;
constexpr std::int32_t kDealBuyWater = 0x4773FFE2;
constexpr std::int32_t kDealSellWater = 0x4773FFE3;
constexpr std::int32_t kDealExportGarbage = 0x4773FFE4;
constexpr std::int32_t kDealImportGarbage = 0x4773FFE5;
// RegionalCity commodity indices used by the game's own g_*_imported / g_*_exported globals
constexpr std::int32_t kCommodityPower = 1;
constexpr std::int32_t kCommodityWater = 2;
constexpr std::int32_t kCommodityGarbage = 3;
} // namespace utility_id

inline bool IsKnownDealType(std::int32_t t) {
    return t >= utility_id::kDealBuyPower && t <= utility_id::kDealImportGarbage;
}

inline const char* DealTypeName(std::int32_t t) {
    switch (t) {
    case utility_id::kDealBuyPower: return "BUY_POWER";
    case utility_id::kDealSellPower: return "SELL_POWER";
    case utility_id::kDealBuyWater: return "BUY_WATER";
    case utility_id::kDealSellWater: return "SELL_WATER";
    case utility_id::kDealExportGarbage: return "EXPORT_GARBAGE";
    case utility_id::kDealImportGarbage: return "IMPORT_GARBAGE";
    default: return "UNKNOWN";
    }
}

enum class Commodity { Power, Water, Garbage, None };

inline Commodity CommodityOfDealType(std::int32_t t) {
    switch (t) {
    case utility_id::kDealBuyPower: case utility_id::kDealSellPower: return Commodity::Power;
    case utility_id::kDealBuyWater: case utility_id::kDealSellWater: return Commodity::Water;
    case utility_id::kDealExportGarbage: case utility_id::kDealImportGarbage: return Commodity::Garbage;
    default: return Commodity::None;
    }
}

// Which commodity a neighbor connection type can carry, exactly as the game's own g_nd_connection_present_*
// scan decides (jump table at 0x422070, indexed by ConnectionType - 1): Rail and Road carry garbage,
// PowerPole power, Pipe water. Highway and every other type carry nothing.
inline Commodity CommodityOfConnectionType(std::uint32_t connectionType) {
    switch (connectionType) {
    case 1: case 2: return Commodity::Garbage;  // Rail, Road
    case 4: return Commodity::Power;            // PowerPole
    case 5: return Commodity::Water;            // Pipe
    default: return Commodity::None;
    }
}

// ---- piecewise-linear curves parsed from tuning properties ---------------------------------------------------
struct Curve {
    std::vector<std::pair<double, double>> points;  // strictly increasing x

    bool valid() const { return points.size() >= 2; }

    // Linear interpolation, clamped at both ends (the game's tuning curves are clamped tables).
    double Eval(double x) const {
        if (points.empty()) return 0.0;
        if (x <= points.front().first) return points.front().second;
        if (x >= points.back().first) return points.back().second;
        for (std::size_t i = 1; i < points.size(); ++i) {
            if (x <= points[i].first) {
                const auto& a = points[i - 1];
                const auto& b = points[i];
                const double t = (x - a.first) / (b.first - a.first);
                return a.second + t * (b.second - a.second);
            }
        }
        return points.back().second;
    }
};

// Flat [x0,y0,x1,y1,...] as stored in exemplar properties. Rejects odd length and non-increasing x.
inline bool ParseCurve(const std::vector<double>& flat, Curve& out) {
    out.points.clear();
    if (flat.size() < 4 || flat.size() % 2 != 0) return false;
    for (std::size_t i = 0; i < flat.size(); i += 2) {
        if (!out.points.empty() && !(flat[i] > out.points.back().first)) { out.points.clear(); return false; }
        if (!std::isfinite(flat[i]) || !std::isfinite(flat[i + 1])) { out.points.clear(); return false; }
        out.points.emplace_back(flat[i], flat[i + 1]);
    }
    return true;
}

// ---- department (budget) level control ---------------------------------------------------------------------
struct DepartmentControl {
    std::uint32_t departmentId = 0;
    bool present = false;              // GetDepartmentBudget returned an object
    Opt<float> funding;                // fraction (1.0 = 100%)
    Opt<float> maxAllowed;             // fraction
    Opt<bool> fixedFunding;
    Opt<std::int64_t> idealMonthly;
    Opt<std::int64_t> totalExpenses;
    Opt<std::int64_t> totalSpending;
};

// ---- Power -------------------------------------------------------------------------------------------------
struct BudgetLine {
    std::uint32_t department = 0;
    std::uint32_t purpose = 0;
    std::int64_t cost = 0;
    float localFunding = 0.0f;
};

// Mechanism verdict for a facility-type utility building (same structural rule as civic Auto Finance).
enum class MechanismClass {
    Compatible,          // standard vanilla list + standard budget purpose + working getter: same mechanism
    NotLocallyFunded,    // no local funding line: nothing to control
    UnsupportedMechanism // listed but its funding path is not the standard one
};

inline const char* MechanismName(MechanismClass m) {
    switch (m) {
    case MechanismClass::Compatible: return "COMPATIBLE_VANILLA_MECHANISM";
    case MechanismClass::NotLocallyFunded: return "NOT_LOCALLY_FUNDED";
    case MechanismClass::UnsupportedMechanism: return "UNSUPPORTED_MECHANISM";
    }
    return "?";
}

inline MechanismClass ClassifyMechanism(bool listedBySimulator, bool hasStandardBudgetLine, Opt<std::int64_t> lineCost,
                                        bool fundingGetterOk) {
    if (!hasStandardBudgetLine || !lineCost || *lineCost <= 0) return MechanismClass::NotLocallyFunded;
    if (!listedBySimulator || !fundingGetterOk) return MechanismClass::UnsupportedMechanism;
    return MechanismClass::Compatible;
}

struct BuildingRef {
    int cellX = 0;
    int cellZ = 0;
    std::string exemplarName;             // logging only, never an identity
    std::vector<std::uint32_t> groups;    // occupant groups found in the utility ranges
    bool HasGroup(std::uint32_t g) const { return std::find(groups.begin(), groups.end(), g) != groups.end(); }
    bool operator<(const BuildingRef& o) const { return std::tie(cellX, cellZ) < std::tie(o.cellX, o.cellZ); }
};

struct PowerPlantRow {
    BuildingRef ref;
    Opt<std::uint32_t> produced;          // GetOccupantPowerProduced out1
    Opt<std::uint32_t> used;              // out2 (output actually used)
    Opt<std::uint32_t> maxProduced;
    Opt<float> efficiencyPercent;
    Opt<float> conditionPercent;
    Opt<std::int64_t> monthlyCost;        // GetMonthlyPowerGenerationCost
    Opt<float> funding;                   // GetPowerPlantFundingPercentage (fraction)
    Opt<bool> powered;                    // IsOccupantPowered
    Opt<float> baseAgingRate;             // occupant property 0xEA1CF220 (per-month base aging)
    Opt<float> agedThreshold;             // occupant property 0xEA1CF221 (condition below which the plant is "aged")
    // occupant property 0x49CAC341: the building's plop cost, an int64 on the winning exemplar. Installed vanilla values (SimCity_1.dat): Wind 500,
    // Coal 10000, Natural Gas 9000, Oil 17000, Waste-to-Energy 25000, Nuclear 40000, Solar 30000, Hydrogen 100000. Used as the plant's replacement cost.
    Opt<std::int64_t> replacementCost;
    Opt<BudgetLine> budgetLine;           // standard power budget line, when present
    MechanismClass mechanism = MechanismClass::NotLocallyFunded;
    bool wasteToEnergy() const { return ref.HasGroup(utility_id::kGroupWasteHandling); }
};

struct PowerAggregate {
    Opt<std::int32_t> requirement, production, import_, export_, demand, consumption;
    Opt<std::uint32_t> annualConsumption;
    Opt<std::int32_t> powered, unpowered, plantCount, overworked, aged, distressed;
    Opt<std::uint32_t> poleCount;
    Opt<float> simulatorFunding;          // PowerSimulator::GetFundingPercent (fraction)
    Opt<bool> onStrike;
};

struct PowerState {
    SnapshotValidity validity = SnapshotValidity::Invalid;
    std::string invalidReason;
    PowerAggregate aggregate;
    DepartmentControl department;
    std::vector<PowerPlantRow> plants;
    Opt<std::int32_t> gameImported, gameExported;   // RegionalCity::GetImported/Exported(1): the game's g_power_* source
};

// ---- Water -------------------------------------------------------------------------------------------------
struct WaterProducerRow {
    BuildingRef ref;
    Opt<std::uint32_t> currentCapacity, maxCapacity, waterUsed, production;
    Opt<float> efficiencyPercent;
    Opt<bool> shutDownByPollution;
    Opt<float> conditionPercent;
    Opt<std::int64_t> upkeep;
    Opt<BudgetLine> budgetLine;
    MechanismClass mechanism = MechanismClass::NotLocallyFunded;
    bool infoOk = false;
};

struct WaterAggregate {
    Opt<std::int32_t> productionCapacity, consumed, imported, exported, demand;
    Opt<std::uint32_t> sources, pollutedSources, pipes, distressedPipes, burstPipes, wateredBuildings, unwateredBuildings;
};

struct WaterState {
    SnapshotValidity validity = SnapshotValidity::Invalid;
    std::string invalidReason;
    WaterAggregate aggregate;
    DepartmentControl department;          // THE water control: one funding value for every producer
    std::vector<WaterProducerRow> producers;
    Opt<std::int32_t> gameImported, gameExported;
};

// ---- Garbage -----------------------------------------------------------------------------------------------
struct GarbageBuildingRow {
    BuildingRef ref;
    Opt<std::uint32_t> capacity;           // GetBuildingGarbageCapacity
    Opt<float> conditionPercent, efficiencyPercent;
    Opt<std::uint32_t> incineratorUsed, w2eUsed, incineratorCurrent, w2eCurrent;
    Opt<BudgetLine> budgetLine;
    bool alsoPowerPlant = false;           // same occupant appears in the power list (Waste-to-Energy)
};

struct GarbageAggregate {
    Opt<std::uint32_t> garbageSupply;
    // `uncollected` is the current grid stock. Start/end are populated by the monthly probe when both samples exist.
    Opt<std::int32_t> uncollected, uncollectedStart, uncollectedEnd;
    Opt<std::int32_t> produced, imported, exported, recycled, incinerated, toEnergy, toLandfill, abducted;
    Opt<std::int32_t> incineratorCapacityMonthly, w2eCapacityMonthly, recyclingEffect;
    Opt<std::uint32_t> incineratorCount, w2eCount, recyclingCount, waterTreatmentCount;
    Opt<std::uint32_t> landfillUsage, landfillCapacity;
    // Monthly landfill throughput only when read from/reconstructed from the vanilla disposal pipeline. Storage is not throughput.
    Opt<std::int32_t> landfillIntakeCapacityMonthly;
    Opt<std::int32_t> landfillTiles;
    Opt<float> landfillEfficiency;
    Opt<std::int64_t> landfillMaintenance, landfillBudget;
    // Property 0x09135482 is the garbage UI-unit conversion, not Department
    // Sanitation efficiency. It is logged only as display metadata.
    Opt<float> uiScalingFactor;
    // 1.1.641.0 PollutionSimulator +0x228, read independently.
    // This is the funding-to-efficiency result used by vanilla
    // capacity and landfill transport paths; missing stays unknown.
    Opt<float> fundingEfficiency;
};

// The LOCAL capacity a utility must keep, with the reserve applied to the part the city produces itself and NOT to what it imports:
//     LocalRequired = ceil(R x max(0, D - B)) + S
// D = the city's effective demand (no reserve), B = the planned/dependable imports, S = contracted exports, R = the reserve factor (Power 1.20 / Water 1.10 by default). An import is never multiplied
// by the reserve, so a contract cannot be bought just to create a regional margin. `requirementFloor` (Power: GetMonthlyRequirement, which includes the need of unpowered consumers and is
// slightly above the demand) is a floor on the LOCAL part only (requirement - B): it can raise what the city must produce, never what it may buy (purchases are capped by D).
inline std::int64_t LocalRequiredCapacity(double reserveFactor, std::int64_t demand, std::int64_t imports, std::int64_t exports, std::int64_t requirementFloor = 0) {
    const std::int64_t owned = std::max<std::int64_t>(0, demand - imports);
    std::int64_t local = static_cast<std::int64_t>(std::ceil(reserveFactor * static_cast<double>(owned) - 1e-7));
    if (requirementFloor > 0) local = std::max(local, std::max<std::int64_t>(0, requirementFloor - imports));
    return local + std::max<std::int64_t>(0, exports);
}

// Can the Sanitation funding change anything in this city? Funding scales the capacity of the garbage processing structures (landfill, incinerator, waste-to-energy plant, recycling
// center); a city with none of them has nothing to fund, and the only way to dispose of its garbage is to export it. Only a KNOWN absence counts: every structure must be read and be zero
// (a missing reading never switches the funding off).
inline bool GarbageFundingCanBenefit(const GarbageAggregate& a) {
    const auto zero = [](const auto& o) { return o && *o == 0; };
    const bool noStructures = zero(a.landfillTiles) && zero(a.incineratorCount) && zero(a.w2eCount) && zero(a.recyclingCount);
    const bool noCapacity = zero(a.landfillTiles) && zero(a.incineratorCapacityMonthly) && zero(a.w2eCapacityMonthly) && zero(a.recyclingCount);
    return !(noStructures || noCapacity);
}

// A landfill the game has just zoned reports its tiles at once but its CAPACITY only at the next month boundary (measured: 15 tiles read 0 capacity for the first month, then 450000). Until
// then the capacity 0 says "not initialised yet", not "this landfill can never work": with no other known processing capacity the sanitation funding must not be CUT on that reading (a raise,
// when the service needs one, is still allowed). Only a KNOWN 0 counts; an unread capacity is handled by the readiness rules, never by this predicate.
inline bool GarbageLandfillCapacityPending(const GarbageAggregate& a) {
    const bool landfillWithoutCapacity = a.landfillTiles && *a.landfillTiles > 0 && a.landfillCapacity && *a.landfillCapacity == 0;
    const bool otherCapacity = (a.incineratorCapacityMonthly && *a.incineratorCapacityMonthly > 0) || (a.w2eCapacityMonthly && *a.w2eCapacityMonthly > 0) || (a.recyclingCount && *a.recyclingCount > 0);
    return landfillWithoutCapacity && !otherCapacity;
}

struct GarbageState {
    SnapshotValidity validity = SnapshotValidity::Invalid;
    std::string invalidReason;
    GarbageAggregate aggregate;
    DepartmentControl department;
    LandfillGeometryInfo geometry;          // filled by the controller from its per-city calibrator, never by the game reader
    std::vector<GarbageBuildingRow> buildings;
    Opt<std::int32_t> gameImported, gameExported;
};

// ---- Neighbor deals ----------------------------------------------------------------------------------------
struct ConnectionRow {
    // LatentProbe: the neighbor's regional-city record (float getters, commodity power) for NeighborSellModel.h
    std::optional<CommodityStats> nbStats;
    Opt<double> nbBudget;
    std::uint32_t type = 0;
    int cellX = 0, cellZ = 0;
    bool active = false;
    Opt<std::uint32_t> otherCitySerial;
    Commodity carries = Commodity::None;
    // LatentProbe (research): what the game answers for this connection even when ProposeDeals shows no opportunity
    Opt<std::int64_t> sellMax, buyMax, sellCostAtMin, buyCostAtMin;
    std::vector<std::pair<std::int64_t, std::int64_t>> sellQuotes;   // (amount, GetDealCost total) at grid amounts <= sellMax, asked from the game
    std::vector<std::pair<std::int64_t, std::int64_t>> buyQuotes;
};

struct DateStamp {
    std::uint32_t year = 0, dayOfYear = 0;
    bool operator==(const DateStamp& o) const { return year == o.year && dayOfYear == o.dayOfYear; }
};

struct ExistingDealRow {
    Opt<std::uintptr_t> pointer;           // runtime identity for timeline correlation; never retained or dereferenced
    std::int32_t type = 0;
    Opt<std::uint32_t> otherCitySerial;
    Opt<std::int32_t> amount, lastAmount, incAmount, incAmount2, amountUsed, flags;
    Opt<std::int64_t> cost;
    Opt<DateStamp> start, expiration, current, suspension;
};

struct DealOpportunityRow {
    std::int32_t type = 0;
    Opt<std::uint32_t> otherCitySerial;
    Opt<std::int32_t> proposedAmount;
    Opt<std::int64_t> proposedCost;
    Opt<std::int64_t> maxAmount;           // GetDealMaxAmount(type, city)
    Opt<std::int64_t> costAtMin;           // GetDealCost(type, minAmount, city) for the tuned minimum
    std::vector<std::pair<std::int64_t, std::int64_t>> quotes;  // (amount, GetDealCost total) asked from the game, never inferred
};

struct TuningValue {
    std::uint32_t id = 0;
    std::string type;                      // as reported by cIGZVariant
    std::vector<double> values;
    bool present = false;
};

// A RegionalCity record read through its getters; a part that could not be read stays missing (never zero).
struct CommodityBookRow {
    Opt<double> produced, imported, demanded, exported, costPerUnit;
    bool complete() const { return produced && imported && demanded && exported && costPerUnit; }
    CommodityBook get() const { return CommodityBook{*produced, *imported, *demanded, *exported, *costPerUnit}; }
};

// The deal market of one neighbor city exactly as GetDealMaxAmount / GetDealCost / ProposeDeals see it. `suitable[t]` is
// FindConnectionSuitableForDeal(t, city) != null for deal type t (index = type - kDealBuyPower); maxAmount[t] is
// trunc(GetDealMaxAmount(t, city)) read after the commodity cache was refreshed.
struct NeighborMarketRow {
    std::uint32_t serial = 0;
    Opt<double> budget;
    CommodityBookRow book[3];            // the neighbor's saved record: power, water, garbage
    bool suitable[6] = {false, false, false, false, false, false};
    Opt<std::int64_t> maxAmount[6];
};

struct LocalMarketRow {
    Opt<double> budget;                  // RegionalCity::GetBudget(): the treasury the BUY-side gates use
    CommodityBookRow book[3];
    Opt<std::int64_t> freeLandfillStorage;   // for the IMPORT_GARBAGE contract limit
};

struct DealState {
    SnapshotValidity validity = SnapshotValidity::Invalid;
    std::string invalidReason;
    std::vector<ConnectionRow> connections;
    Opt<double> localPowerUnitCost;                  // regional GetCostPerUnit(power): the seller's average cost per unit used by GetDealCost
    std::optional<CommodityStats> localPowerStats;   // LOCAL regional-city record, same getters
    std::vector<ExistingDealRow> deals;
    std::vector<std::size_t> findDealsCount;   // FindDeals(type) list sizes for the six types, in type order (cross-check)
    std::vector<DealOpportunityRow> opportunities;
    bool proposalsRead = false;            // false until the deal simulator has had simulation progress: NOT "no opportunities"
    std::vector<TuningValue> tuning;       // read from GetTuningExemplar() = the installation's effective values
    // Commodity cache + market of every neighbor, read right after ProposeDeals refreshed the cache. False until then.
    bool marketsRead = false;
    LocalMarketRow localMarket;
    std::vector<NeighborMarketRow> neighbors;
    Opt<std::int64_t> monthlyIncome, monthlyExpense;
    // What the game's own Lua globals say (for cross-checking the scan)
    Opt<bool> canBuyPower, canSellPower, canBuyWater, canSellWater, canExportGarbage, canImportGarbage;
    Opt<bool> connectionPower, connectionWater, connectionGarbage;
    // 1.1.641.0 NeighborsSimulator +0x3E. Passive observation of
    // the vanilla Neighbor Deals dialog lifecycle; never written by this DLL.
    Opt<bool> uiDisabled;
};

// Sum of contracted / used amounts of existing deals per type, for reconciling against simulator aggregates.
struct DealSums {
    std::int64_t contracted[6] = {0, 0, 0, 0, 0, 0};  // indexed by dealType - kDealBuyPower
    std::int64_t used[6] = {0, 0, 0, 0, 0, 0};
    std::int64_t cost[6] = {0, 0, 0, 0, 0, 0};
    int count[6] = {0, 0, 0, 0, 0, 0};
};

inline DealSums SumDeals(const std::vector<ExistingDealRow>& deals) {
    DealSums s;
    for (const auto& d : deals) {
        if (!IsKnownDealType(d.type)) continue;
        const int i = d.type - utility_id::kDealBuyPower;
        s.count[i]++;
        if (d.amount) s.contracted[i] += *d.amount;
        if (d.amountUsed) s.used[i] += *d.amountUsed;
        if (d.cost) s.cost[i] += *d.cost;
    }
    return s;
}

// Contracted exports (sell power / sell water / export garbage). They consume LOCAL capacity: a city must keep
// capacity >= own demand + these. A sell-water deal can appear right after a deep funding cut and leave
// buildings unwatered until capacity is restored.
struct ContractedExports {
    std::int64_t power = 0, water = 0, garbage = 0;
};

inline ContractedExports ContractedExportsOf(const std::vector<ExistingDealRow>& deals, Opt<std::int32_t> gamePower,
                                             Opt<std::int32_t> gameWater, Opt<std::int32_t> gameGarbageExport) {
    ContractedExports c;
    for (const auto& d : deals) {
        const std::int64_t a = d.amount ? *d.amount : 0;
        if (d.type == utility_id::kDealSellPower) c.power += a;
        else if (d.type == utility_id::kDealSellWater) c.water += a;
        else if (d.type == utility_id::kDealExportGarbage) c.garbage += a;
    }
    // The game's own exported total is authoritative when larger (e.g. deals not yet listed).
    if (gamePower && *gamePower > c.power) c.power = *gamePower;
    if (gameWater && *gameWater > c.water) c.water = *gameWater;
    if (gameGarbageExport && *gameGarbageExport > c.garbage) c.garbage = *gameGarbageExport;
    return c;
}

// Right after a city load `watered` and `unwatered` both read 0 until the simulation has run: not a real reading.
inline bool ServiceCountsReady(const WaterAggregate& a) {
    return a.wateredBuildings && a.unwateredBuildings && (*a.wateredBuildings > 0 || *a.unwateredBuildings > 0);
}

// Unwatered buildings forbid cutting the Water funding only while the production cannot cover the demand: then more funding is the answer and a cut would make it worse. When the production
// already exceeds the demand (e.g. capacity 34081 for a demand of 20323 with 55 buildings unwatered) the missing water is a PIPE / connection problem that no funding level solves, so a
// blanket ban would keep the budget at 120% indefinitely. The cut itself is still bound by the reserve (required capacity) and the pipe constraints.
inline bool UnwateredBlocksCut(const WaterAggregate& a) {
    if (!a.unwateredBuildings || *a.unwateredBuildings == 0) return false;
    if (!a.productionCapacity || !a.demand) return true;                 // unknown: never cut
    return *a.productionCapacity < *a.demand;
}

// Pure recomputation of the game's g_nd_can_* flags from a list of proposed deal types.
struct EligibilityFlags {
    bool canBuyPower = false, canSellPower = false, canBuyWater = false, canSellWater = false,
         canExportGarbage = false, canImportGarbage = false;
};

inline EligibilityFlags EligibilityFromProposals(const std::vector<std::int32_t>& proposedTypes) {
    EligibilityFlags f;
    for (const auto t : proposedTypes) {
        switch (t) {
        case utility_id::kDealBuyPower: f.canBuyPower = true; break;
        case utility_id::kDealSellPower: f.canSellPower = true; break;
        case utility_id::kDealBuyWater: f.canBuyWater = true; break;
        case utility_id::kDealSellWater: f.canSellWater = true; break;
        case utility_id::kDealExportGarbage: f.canExportGarbage = true; break;
        case utility_id::kDealImportGarbage: f.canImportGarbage = true; break;
        default: break;
        }
    }
    return f;
}

struct ConnectionPresence {
    bool power = false, water = false, garbage = false;
};

inline ConnectionPresence PresenceFromConnections(const std::vector<std::uint32_t>& connectionTypes) {
    ConnectionPresence p;
    for (const auto t : connectionTypes) {
        switch (CommodityOfConnectionType(t)) {
        case Commodity::Power: p.power = true; break;
        case Commodity::Water: p.water = true; break;
        case Commodity::Garbage: p.garbage = true; break;
        default: break;
        }
    }
    return p;
}

// ---- unified snapshot --------------------------------------------------------------------------------------
struct UtilitySnapshot {
    std::uint32_t lifecycleEpoch = 0;
    std::uint32_t citySerial = 0;
    std::uint32_t simYear = 0, simDayOfYear = 0;
    PowerState power;
    WaterState water;
    GarbageState garbage;
    DealState deals;
};

// Which decisions each validity level may support. A PARTIAL snapshot never authorises a decision that
// needs the missing part; callers must check the fields they depend on.
inline bool MayReadForDecisions(SnapshotValidity v) {
    return v == SnapshotValidity::Valid || v == SnapshotValidity::ValidEmpty || v == SnapshotValidity::Partial;
}
inline bool MayWriteFrom(SnapshotValidity v) {
    return v == SnapshotValidity::Valid || v == SnapshotValidity::Partial;  // ValidEmpty: nothing to write
}

// Derive a subsystem validity from what was read. `criticalMissing` = a field the subsystem cannot be
// trusted without; `itemsSeen` = number of facility rows returned by the authoritative list.
inline SnapshotValidity DeriveValidity(bool simulatorPresent, bool listAvailable, std::size_t itemsSeen,
                                       bool criticalMissing, bool epochMatches) {
    if (!simulatorPresent || !epochMatches) return SnapshotValidity::Invalid;
    if (!listAvailable) return SnapshotValidity::Invalid;
    if (criticalMissing) return SnapshotValidity::Partial;
    return itemsSeen == 0 ? SnapshotValidity::ValidEmpty : SnapshotValidity::Valid;
}

// The garbage snapshot validity. "No garbage structure at all" (ValidEmpty) is only claimed when it is PROVEN: the building list was read and is empty AND the landfill tile
// count was read and is 0. A landfill developer that could not be read leaves the tile count unknown, and unknown is never zero: the snapshot is then Partial.
inline SnapshotValidity DeriveGarbageValidity(bool listAvailable, std::size_t buildings, const Opt<std::int32_t>& landfillTiles, bool producedUncollectedMissing, bool epochMatches) {
    const bool tilesKnown = landfillTiles.has_value();
    const std::size_t infrastructure = buildings + (tilesKnown && *landfillTiles > 0 ? 1 : 0);
    const bool landfillUnknownAndNothingElse = !tilesKnown && buildings == 0;
    return DeriveValidity(true, listAvailable, infrastructure, producedUncollectedMissing || landfillUnknownAndNothingElse, epochMatches);
}

} // namespace sc4af
