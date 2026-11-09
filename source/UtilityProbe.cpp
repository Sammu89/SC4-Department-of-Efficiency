// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "UtilityProbe.h"
#include "IniSections.h"

#include "Logging.h"
#include "NeighborGameAbi.h"

#include "SC4List.h"
#include "SC4Percentage.h"
#include "SC4Vector.h"
#include "cIGZDate.h"
#include "cIGZString.h"
#include "cIGZVariant.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4BuildingOccupant.h"
#include "cISC4City.h"
#include "cISC4DepartmentBudget.h"
#include "cISC4LandfillDeveloper.h"
#include "cISC4NeighborConnection.h"
#include "cISC4NeighborDeal.h"
#include "cISC4NeighborsSimulator.h"
#include "cISC4Occupant.h"
#include "cISC4PlumbingSimulator.h"
#include "cISC4PollutionSimulator.h"
#include "cISC4PowerSimulator.h"
#include "cISC4RegionalCity.h"
#include "cISC4Simulator.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cISCResExemplar.h"
#include "cRZAutoRefCount.h"
#include "cS3DVector3.h"

#include <windows.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <type_traits>

namespace sc4af {
namespace {

using namespace gameabi;

// ---- formatting ---------------------------------------------------------------------------------------------
template <class T> std::string O(const Opt<T>& v) {
    if (!v) return "?";
    std::ostringstream s;
    if constexpr (std::is_same_v<T, bool>) s << (*v ? "true" : "false");
    else if constexpr (std::is_floating_point_v<T>) s << std::setprecision(9) << *v;
    else s << +*v;
    return s.str();
}

std::string OPct(const Opt<float>& v) {
    if (!v) return "?";
    std::ostringstream s;
    s << std::setprecision(6) << (*v * 100.0f) << '%';
    return s.str();
}

std::string Serial(const Opt<std::uint32_t>& v) {
    if (!v) return "?";
    std::ostringstream s;
    s << v.value();
    return s.str();
}

std::string GroupsText(const std::vector<std::uint32_t>& g) {
    std::ostringstream s;
    for (std::size_t i = 0; i < g.size(); ++i) s << (i ? "," : "") << std::hex << "0x" << g[i];
    return s.str();
}

std::string Date(const Opt<DateStamp>& d) {
    if (!d) return "?";
    return std::to_string(d->year) + "/" + std::to_string(d->dayOfYear);
}

// SDK header bug on 1.1.641.0: vtable slots 45/46 of cISC4PowerSimulator are
// `SC4Percentage* f(SC4Percentage* out, cISC4Occupant*)` (executable 0x006EB5F0 / 0x006EC820, `ret 8`), not
// `float f(occ)`. Calling them through the header corrupts the stack, so they are invoked with the real signature.
using PlantPercentFn = SC4Percentage*(__thiscall*)(cISC4PowerSimulator*, SC4Percentage*, cISC4Occupant*);
constexpr int kSlotPlantEfficiency = 45;
constexpr int kSlotPlantCondition = 46;
Opt<float> PlantPercent(cISC4PowerSimulator* sim, int slot, cISC4Occupant* occ) {
    auto** vtable = *reinterpret_cast<void***>(sim);
    const auto fn = reinterpret_cast<PlantPercentFn>(vtable[slot]);
    SC4Percentage out{};
    out.percentage = -1.0f;
    fn(sim, &out, occ);
    if (!(out.percentage >= 0.0f)) return std::nullopt;
    return out.percentage;
}

// More 1.1.641.0 ABI facts, found by disassembly:
//  * cISC4PlumbingSimulator getters declared int64 (capacity, consumed, imported, exported, demand) return a plain
//    32-bit value in EAX (`mov eax,[ecx+off]; ret`); EDX is garbage. They must be read as int32.
//  * cISC4LandfillDeveloper::GetEfficiency returns SC4Percentage through a hidden pointer (`ret 4`).
//  * cISC4NeighborsSimulator::GetDealCost / GetDealMaxAmount return a double in ST0.
using VtableInt32Fn = std::int32_t(__thiscall*)(void*);
template <class Sim> Opt<std::int32_t> CallInt32(Sim* sim, int slot) {
    auto** vtable = *reinterpret_cast<void***>(sim);
    return reinterpret_cast<VtableInt32Fn>(vtable[slot])(sim);
}
constexpr int kPlumbCapacity = 14, kPlumbConsumed = 15, kPlumbImported = 16, kPlumbExported = 17, kPlumbDemand = 23;

using PercentOutFn = SC4Percentage*(__thiscall*)(void*, SC4Percentage*);
Opt<float> CallPercentOut(void* obj, int slot) {
    auto** vtable = *reinterpret_cast<void***>(obj);
    SC4Percentage out{};
    out.percentage = -1.0f;
    reinterpret_cast<PercentOutFn>(vtable[slot])(obj, &out);
    if (!(out.percentage >= 0.0f)) return std::nullopt;
    return out.percentage;
}
constexpr int kLandfillEfficiency = 5;

// Valid for SC4 Deluxe 1.1.641.0 only: PollutionSimulator holds the
// post-curve Sanitation efficiency at +0x228. It is not exposed by the
// public ABI. The read is SEH-guarded and never writes.
Opt<float> PollutionFundingEfficiency(cISC4PollutionSimulator* sim) {
    if (!sim) return std::nullopt;
    __try {
        const float value = *reinterpret_cast<const float*>(reinterpret_cast<const char*>(sim) + 0x228);
        return std::isfinite(value) && value >= 0.0f && value <= 2.0f ? Opt<float>(value) : std::nullopt;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return std::nullopt;
    }
}

Opt<bool> NeighborDealsUiDisabled(cISC4NeighborsSimulator* sim) {
    if (!sim) return std::nullopt;
    __try {
        const auto value = *reinterpret_cast<const std::uint8_t*>(reinterpret_cast<const char*>(sim) + 0x3E);
        return value <= 1 ? Opt<bool>(value != 0) : std::nullopt;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return std::nullopt;
    }
}

Opt<float> OccupantFloat(cISC4Occupant* occupant, std::uint32_t id) {
    auto* holder = occupant->AsPropertyHolder();
    if (!holder) return std::nullopt;
    cISCProperty* p = holder->GetProperty(id);
    if (!p) return std::nullopt;
    cIGZVariant* v = p->GetPropertyValue();
    if (!v || (v->GetType() & cIGZVariant::TypeArray) != 0 || v->GetType() != cIGZVariant::Float32) return std::nullopt;
    return v->GetValFloat32();
}

// A scalar integer exemplar property (the plop cost 0x49CAC341 is a Sint64 in the vanilla exemplars; other widths are accepted).
Opt<std::int64_t> OccupantInteger(cISC4Occupant* occupant, std::uint32_t id) {
    auto* holder = occupant->AsPropertyHolder();
    if (!holder || !holder->HasProperty(id)) return std::nullopt;
    cISCProperty* p = holder->GetProperty(id);
    if (!p) return std::nullopt;
    cIGZVariant* v = p->GetPropertyValue();
    if (!v || (v->GetType() & cIGZVariant::TypeArray) != 0) return std::nullopt;
    switch (v->GetType()) {
    case cIGZVariant::Sint64: return v->GetValSint64();
    case cIGZVariant::Sint32: return static_cast<std::int64_t>(v->GetValSint32());
    case cIGZVariant::Uint32: return static_cast<std::int64_t>(v->GetValUint32());
    default: return std::nullopt;
    }
}

// ---- game readers -------------------------------------------------------------------------------------------
std::vector<std::uint32_t> UtilityGroups(cISC4Occupant* occupant) {
    std::vector<std::uint32_t> groups;
    for (std::uint32_t id = 0x1400; id <= 0x141F; ++id)
        if (occupant->IsOccupantGroup(id)) groups.push_back(id);
    if (occupant->IsOccupantGroup(0x1928u)) groups.push_back(0x1928u);
    return groups;
}

BuildingRef RefOf(cISC4Occupant* occupant) {
    BuildingRef ref;
    cS3DVector3 position(0.0f, 0.0f, 0.0f);
    if (occupant->GetPosition(&position)) {
        ref.cellX = static_cast<int>(std::floor(position.fX / 16.0f));
        ref.cellZ = static_cast<int>(std::floor(position.fZ / 16.0f));
    }
    ref.groups = UtilityGroups(occupant);
    cRZAutoRefCount<cISC4BuildingOccupant> building;
    if (occupant->QueryInterface(GZIID_cISC4BuildingOccupant, building.AsPPVoid()))
        if (cIGZString* name = building->GetExemplarName()) ref.exemplarName = name->ToChar() ? name->ToChar() : "";
    return ref;
}

std::vector<BudgetLine> BudgetLines(cISC4BudgetSimulator* budget, cISC4Occupant* occupant) {
    std::vector<BudgetLine> lines;
    auto* holder = occupant->AsPropertyHolder();
    if (!budget || !holder) return lines;
    SC4Vector<cISC4BudgetSimulator::BudgetItem> items;
    if (budget->GetBudgetItemInfo(holder, items))
        for (const auto& item : items) lines.push_back({item.departmentID, item.purpose, item.cost, item.localFundingPercentage});
    return lines;
}

Opt<BudgetLine> LineForPurpose(const std::vector<BudgetLine>& lines, std::uint32_t purpose) {
    for (const auto& l : lines)
        if (l.purpose == purpose) return l;
    return std::nullopt;
}

DepartmentControl ReadDepartment(cISC4BudgetSimulator* budget, std::uint32_t id) {
    DepartmentControl d;
    d.departmentId = id;
    if (!budget) return d;
    auto* dept = budget->GetDepartmentBudget(id);
    if (!dept) return d;
    d.present = true;
    if (auto* f = dept->GetFundingPercentage()) d.funding = f->percentage;
    if (auto* m = dept->GetMaxAllowedFundingPercentage()) d.maxAllowed = m->percentage;
    d.fixedFunding = dept->GetIsFixedFunding();
    d.idealMonthly = dept->GetIdealMonthlyFunding();
    d.totalExpenses = dept->GetTotalExpenses();
    d.totalSpending = dept->GetTotalSpending();
    return d;
}

PowerState ReadPower(cISC4City* city, cISC4BudgetSimulator* budget, cISC4RegionalCity* regional, bool epochMatches) {
    PowerState state;
    auto* sim = city ? city->GetPowerSimulator() : nullptr;
    if (!sim) { state.invalidReason = "PowerSimulator unavailable"; state.validity = DeriveValidity(false, false, 0, false, epochMatches); return state; }
    auto& a = state.aggregate;
    a.requirement = sim->GetMonthlyRequirement();
    a.production = sim->GetMonthlyProduction();
    a.import_ = sim->GetMonthlyImport();
    a.export_ = sim->GetMonthlyExport();
    a.demand = sim->GetMonthlyDemand();
    a.consumption = sim->GetMonthlyConsumption();
    a.annualConsumption = sim->GetAnnualConsumption();
    a.powered = sim->GetPoweredConsumerCount();
    a.unpowered = sim->GetUnpoweredConsumerCount();
    a.plantCount = sim->GetPowerPlantCount();
    a.overworked = sim->GetOverworkedPowerPlantCount();
    a.aged = sim->GetAgedPowerPlantCount();
    a.distressed = sim->GetDistressedPowerPlantCount();
    a.poleCount = sim->GetPowerPoleCount();
    a.onStrike = sim->IsOnStrike();
    if (auto* f = sim->GetFundingPercent()) a.simulatorFunding = f->percentage;
    state.department = ReadDepartment(budget, utility_id::kPowerDepartment);
    if (regional) {
        state.gameImported = static_cast<std::int32_t>(regional->GetImported(utility_id::kCommodityPower));
        state.gameExported = static_cast<std::int32_t>(regional->GetExported(utility_id::kCommodityPower));
    }

    ilist<cISC4Occupant*> plants;
    sim->GetPowerPlants(plants);
    for (auto* occ : plants) {
        if (!occ) continue;
        PowerPlantRow row;
        row.ref = RefOf(occ);
        std::uint32_t produced = 0, used = 0, maxOut = 0;
        sim->GetOccupantPowerProduced(occ, produced, used);
        sim->GetOccupantMaxPowerProduced(occ, maxOut);
        row.produced = produced;
        row.used = used;
        row.maxProduced = maxOut;
        row.efficiencyPercent = PlantPercent(sim, kSlotPlantEfficiency, occ);
        row.conditionPercent = PlantPercent(sim, kSlotPlantCondition, occ);
        row.powered = sim->IsOccupantPowered(occ);
        row.baseAgingRate = OccupantFloat(occ, 0xEA1CF220u);
        row.agedThreshold = OccupantFloat(occ, 0xEA1CF221u);
        row.replacementCost = OccupantInteger(occ, 0x49CAC341u);
        std::int64_t cost = 0;
        if (sim->GetMonthlyPowerGenerationCost(occ, cost)) row.monthlyCost = cost;
        SC4Percentage funding{};
        funding.percentage = -1.0f;
        const bool fundingOk = sim->GetPowerPlantFundingPercentage(occ, funding) && funding.percentage >= 0.0f;
        if (fundingOk) row.funding = funding.percentage;
        const auto lines = BudgetLines(budget, occ);
        row.budgetLine = LineForPurpose(lines, utility_id::kPowerPurpose);
        row.mechanism = ClassifyMechanism(true, row.budgetLine.has_value(),
                                          row.budgetLine ? Opt<std::int64_t>(row.budgetLine->cost) : std::nullopt, fundingOk);
        state.plants.push_back(std::move(row));
    }
    std::sort(state.plants.begin(), state.plants.end(), [](const auto& x, const auto& y) { return x.ref < y.ref; });
    state.validity = DeriveValidity(true, true, state.plants.size(), !a.production || !a.consumption, epochMatches);
    return state;
}

WaterState ReadWater(cISC4City* city, cISC4BudgetSimulator* budget, cISC4RegionalCity* regional, bool epochMatches) {
    WaterState state;
    auto* sim = city ? city->GetPlumbingSimulator() : nullptr;
    if (!sim) { state.invalidReason = "PlumbingSimulator unavailable"; state.validity = DeriveValidity(false, false, 0, false, epochMatches); return state; }
    auto& a = state.aggregate;
    a.productionCapacity = CallInt32(sim, kPlumbCapacity);
    a.consumed = CallInt32(sim, kPlumbConsumed);
    a.imported = CallInt32(sim, kPlumbImported);
    a.exported = CallInt32(sim, kPlumbExported);
    a.demand = CallInt32(sim, kPlumbDemand);
    a.sources = sim->GetNumWaterSources();
    a.pollutedSources = sim->GetNumPollutedWaterSources();
    a.pipes = sim->GetNumPipes();
    a.distressedPipes = sim->GetNumDistressedPipes();
    a.burstPipes = sim->GetNumBurstPipes();
    a.wateredBuildings = sim->GetNumWateredBuildings();
    a.unwateredBuildings = sim->GetNumUnwateredBuildings();
    state.department = ReadDepartment(budget, utility_id::kWaterDepartment);
    if (regional) {
        state.gameImported = static_cast<std::int32_t>(regional->GetImported(utility_id::kCommodityWater));
        state.gameExported = static_cast<std::int32_t>(regional->GetExported(utility_id::kCommodityWater));
    }

    ilist<cISC4Occupant*> producers;
    const bool listOk = sim->GetWaterProducers(producers);
    if (listOk) {
        for (auto* occ : producers) {
            if (!occ) continue;
            WaterProducerRow row;
            row.ref = RefOf(occ);
            std::uint32_t cur = 0, max = 0, used = 0;
            SC4Percentage eff{};
            bool shut = false;
            row.infoOk = sim->GetWaterProducerInfo(occ, &cur, &max, &eff, &shut, &used);
            if (row.infoOk) {
                row.currentCapacity = cur;
                row.maxCapacity = max;
                row.waterUsed = used;
                row.efficiencyPercent = eff.percentage;
                row.shutDownByPollution = shut;
            }
            row.production = sim->GetWaterProduction(occ);
            SC4Percentage cond{};
            if (sim->GetWaterProducerConditionPercent(occ, cond)) row.conditionPercent = cond.percentage;
            std::int64_t upkeep = 0;
            if (sim->GetOccupantMonthlyUpkeepCost(occ, upkeep)) row.upkeep = upkeep;
            const auto lines = BudgetLines(budget, occ);
            row.budgetLine = LineForPurpose(lines, utility_id::kWaterPurpose);
            // Water is controlled at department level: the "funding getter" for the mechanism check is the
            // department control itself.
            row.mechanism = ClassifyMechanism(true, row.budgetLine.has_value(),
                                              row.budgetLine ? Opt<std::int64_t>(row.budgetLine->cost) : std::nullopt,
                                              state.department.present && state.department.funding.has_value());
            state.producers.push_back(std::move(row));
        }
        std::sort(state.producers.begin(), state.producers.end(), [](const auto& x, const auto& y) { return x.ref < y.ref; });
    }
    state.validity = DeriveValidity(true, listOk, state.producers.size(), !a.productionCapacity || !a.consumed, epochMatches);
    return state;
}

GarbageState ReadGarbage(cISC4City* city, cISC4BudgetSimulator* budget, cISC4RegionalCity* regional,
                         const PowerState& power, bool epochMatches) {
    GarbageState state;
    auto* sim = city ? city->GetPollutionSimulator() : nullptr;
    auto* landfill = city ? city->GetLandfillDeveloper() : nullptr;
    if (!sim) { state.invalidReason = "PollutionSimulator unavailable"; state.validity = DeriveValidity(false, false, 0, false, epochMatches); return state; }
    auto& a = state.aggregate;
    a.garbageSupply = sim->GetGarbageSupply();
    a.uncollected = sim->GetTotalGarbageUncollected();
    a.produced = sim->GetGarbageProducedLastMonth();
    a.imported = sim->GetGarbageImportedLastMonth();
    a.exported = sim->GetGarbageExportedLastMonth();
    a.recycled = sim->GetGarbageRecycledLastMonth();
    a.incinerated = sim->GetGarbageIncineratedLastMonth();
    a.toEnergy = sim->GetGarbageConvertedToEnergyLastMonth();
    a.toLandfill = sim->GetGarbageLandfilledLastMonth();
    a.abducted = sim->GetGarbageAbductedPrevMonth();
    a.incineratorCapacityMonthly = sim->GetMonthlyIncineratorCapacity();
    a.w2eCapacityMonthly = sim->GetMonthlyWasteToEnergyCapacity();
    a.recyclingEffect = sim->GetRecyclingCenterEffect();
    a.incineratorCount = sim->GetIncineratorCount();
    a.w2eCount = sim->GetWasteToEnergyBuildingCount();
    a.recyclingCount = sim->GetRecyclingCenterCount();
    a.waterTreatmentCount = sim->GetWaterTreatmentPlantCount();
    a.uiScalingFactor = sim->GetGarbageScalingFactor();
    a.fundingEfficiency = PollutionFundingEfficiency(sim);
    if (landfill) {
        std::uint32_t usage = 0, capacity = 0;
        landfill->GetLandfillUsage(usage, capacity);
        a.landfillUsage = usage;
        a.landfillCapacity = capacity;
        a.landfillTiles = landfill->GetLandfillTileCount();
            // 1.1.641.0: cISC4LandfillDeveloper::GetEfficiency (0x006B8BA0) dereferences this+0x2C without a null check;
        // the member is null while the city has no landfill infrastructure and the game itself would crash. The
        // methods are only called when it is set; otherwise the values stay missing (not zero).
        const bool landfillReady = *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(landfill) + 0x2C) != nullptr;
        if (landfillReady) {
            a.landfillEfficiency = CallPercentOut(landfill, kLandfillEfficiency);
            a.landfillMaintenance = landfill->GetMaintenanceCosts();
            a.landfillBudget = landfill->GetCurrentBudget();
        }
    }
    state.department = ReadDepartment(budget, utility_id::kGarbageDepartment);
    if (regional) {
        state.gameImported = static_cast<std::int32_t>(regional->GetImported(utility_id::kCommodityGarbage));
        state.gameExported = static_cast<std::int32_t>(regional->GetExported(utility_id::kCommodityGarbage));
    }

    SC4List<cISC4Occupant*> buildings;
    const bool listOk = sim->GetGarbageBuildings(buildings);
    if (listOk) {
        for (auto* occ : buildings) {
            if (!occ) continue;
            // no AddRef: GetGarbageBuildings already AddRefs each occupant and the SC4List destructor releases it
            GarbageBuildingRow row;
            row.ref = RefOf(occ);
            row.capacity = sim->GetBuildingGarbageCapacity(occ);
            SC4Percentage cond{}, eff{};
            if (sim->GetCurrentOccupantCondition(occ, cond)) row.conditionPercent = cond.percentage;
            if (sim->GetCurrentOccupantEfficiency(occ, eff)) row.efficiencyPercent = eff.percentage;
            std::uint32_t v = 0;
            if (sim->GetIncineratorUsedCapacity(occ, v)) row.incineratorUsed = v;
            if (sim->GetWasteToEnergyUsedCapacity(occ, v)) row.w2eUsed = v;
            if (sim->GetIncineratorCurrentCapacity(occ, v)) row.incineratorCurrent = v;
            if (sim->GetWasteToEnergyCurrentCapacity(occ, v)) row.w2eCurrent = v;
            row.budgetLine = LineForPurpose(BudgetLines(budget, occ), utility_id::kGarbagePurpose);
            for (const auto& p : power.plants)
                if (p.ref.cellX == row.ref.cellX && p.ref.cellZ == row.ref.cellZ) row.alsoPowerPlant = true;
            state.buildings.push_back(std::move(row));
        }
        std::sort(state.buildings.begin(), state.buildings.end(), [](const auto& x, const auto& y) { return x.ref < y.ref; });
    }
    // Landfill tiles count as "garbage infrastructure" even without garbage buildings.
    state.validity = DeriveGarbageValidity(listOk, state.buildings.size(), a.landfillTiles, !a.produced || !a.uncollected, epochMatches);
    if (state.validity != SnapshotValidity::Valid && state.invalidReason.empty()) {      // say WHY the snapshot is not Valid (an empty reason hides the cause)
        state.invalidReason = std::string("validity=") + (state.validity == SnapshotValidity::ValidEmpty ? "ValidEmpty" : state.validity == SnapshotValidity::Partial ? "Partial" : "Invalid") +
                              " epochMatches=" + (epochMatches ? "1" : "0") + " garbageBuildingList=" + (listOk ? "ok" : "REFUSED") + " garbageBuildings=" + std::to_string(state.buildings.size()) +
                              " landfillTiles=" + (a.landfillTiles ? std::to_string(*a.landfillTiles) : std::string("unread")) + " produced=" + (a.produced ? "read" : "MISSING") +
                              " uncollected=" + (a.uncollected ? "read" : "MISSING");
    }
    return state;
}

// ---- neighbor deals -----------------------------------------------------------------------------------------
Opt<DateStamp> StampOf(cIGZDate* d) {
    if (!d || !d->IsValid()) return std::nullopt;
    return DateStamp{d->Year(), d->DayOfYear()};
}

// cIGZDate is an interface; a concrete instance is obtained by cloning the simulator's own date.
struct ScratchDate {
    cIGZDate* date = nullptr;
    explicit ScratchDate(cISC4City* city) {
        auto* sim = city ? city->GetSimulator() : nullptr;
        auto* now = sim ? sim->GetSimDate() : nullptr;
        if (now) now->Clone(&date);
    }
    ~ScratchDate() { if (date) date->Release(); }
    ScratchDate(const ScratchDate&) = delete;
    ScratchDate& operator=(const ScratchDate&) = delete;
};


DealState ReadDeals(cISC4City* city, cISC4RegionalCity* regional, bool includeProposals, bool epochMatches, bool probeLatent) {
    DealState state;
    auto* ns = city ? city->GetNeighborsSimulator() : nullptr;
    if (!ns) { state.invalidReason = "NeighborsSimulator unavailable"; state.validity = DeriveValidity(false, false, 0, false, epochMatches); return state; }

    state.uiDisabled = NeighborDealsUiDisabled(ns);
    // connections
    std::vector<std::uint32_t> types;
    std::vector<std::pair<std::uint32_t, cISC4RegionalCity*>> neighborCities;   // borrowed, valid only inside this call
    {
        SC4List<cISC4NeighborConnection*> list;
        ns->GetAllConnections(list);
        for (auto* c : list) {
            if (!c) continue;
            ConnectionRow row;
            row.type = static_cast<std::uint32_t>(c->GetType());
            int x = 0, z = 0;
            c->GetConnectionCell(x, z);
            row.cellX = x;
            row.cellZ = z;
            row.active = c->GetIsActive();
            row.otherCitySerial = SerialOf(c->GetCityOnOtherSideOfConnection());
            row.carries = CommodityOfConnectionType(row.type);
            types.push_back(row.type);
            if (auto* otherCity = c->GetCityOnOtherSideOfConnection()) {
                const auto serial = otherCity->GetCitySerialNumber();
                const bool seen = std::any_of(neighborCities.begin(), neighborCities.end(), [&](const auto& n) { return n.first == serial; });
                if (!seen && row.carries != Commodity::None) neighborCities.emplace_back(serial, otherCity);
            }
            if (probeLatent && includeProposals && row.carries == Commodity::Power) {
                auto* other = c->GetCityOnOtherSideOfConnection();
                double v = 0.0;
                if (other && SafeDealMax(ns, utility_id::kDealSellPower, other, v)) row.sellMax = static_cast<std::int64_t>(v);
                if (other && SafeDealMax(ns, utility_id::kDealBuyPower, other, v)) row.buyMax = static_cast<std::int64_t>(v);
                if (other && SafeDealCost(ns, utility_id::kDealSellPower, 1000.0, other, v)) row.sellCostAtMin = static_cast<std::int64_t>(v);
                if (other && SafeDealCost(ns, utility_id::kDealBuyPower, 1000.0, other, v)) row.buyCostAtMin = static_cast<std::int64_t>(v);
                if (other) {
                    CommodityStats st;
                    st.produced = other->GetProduced(utility_id::kCommodityPower);
                    st.imported = other->GetImported(utility_id::kCommodityPower);
                    st.demanded = other->GetDemanded(utility_id::kCommodityPower);
                    st.exported = other->GetExported(utility_id::kCommodityPower);
                    row.nbStats = st;
                    row.nbBudget = other->GetBudget();
                }
                if (other) {   // GetDealCost quotes any amount (it is not limited by GetDealMaxAmount): needed for LATENT opportunities
                    const std::int64_t grid[] = {1000, 2000, 3000, 4000, 5000, 7500, 10000, 15000, 20000, 30000, 40000, 50000, 75000, 100000, 150000, 200000};
                    for (const auto amount : grid) {
                        if (SafeDealCost(ns, utility_id::kDealSellPower, static_cast<double>(amount), other, v)) row.sellQuotes.emplace_back(amount, static_cast<std::int64_t>(v));
                        if (SafeDealCost(ns, utility_id::kDealBuyPower, static_cast<double>(amount), other, v)) row.buyQuotes.emplace_back(amount, static_cast<std::int64_t>(v));
                    }
                }
            }
            state.connections.push_back(row);
        }
    }
    std::sort(state.connections.begin(), state.connections.end(), [](const auto& x, const auto& y) {
        return std::tie(x.type, x.cellX, x.cellZ) < std::tie(y.type, y.cellX, y.cellZ);
    });
    const auto presence = PresenceFromConnections(types);
    state.connectionPower = presence.power;
    state.connectionWater = presence.water;
    state.connectionGarbage = presence.garbage;

    if (probeLatent && regional) {
        CommodityStats st;
        st.produced = regional->GetProduced(utility_id::kCommodityPower);
        st.imported = regional->GetImported(utility_id::kCommodityPower);
        st.demanded = regional->GetDemanded(utility_id::kCommodityPower);
        st.exported = regional->GetExported(utility_id::kCommodityPower);
        state.localPowerStats = st;
        state.localPowerUnitCost = regional->GetCostPerUnit(utility_id::kCommodityPower);
    }

    // Existing deals come from the typed list RegionalCity::GetLocalDeals. FindDeals(type) is only COUNTED (as the
    // game does for g_nd_count_*) and cross-checked against it; its elements are never dereferenced.
    ScratchDate scratch(city);
    bool dealsOk = true;
    if (regional) {
        SC4List<cISC4NeighborDeal*> local;
        if (regional->GetLocalDeals(local)) {
            for (auto* d : local) {
                if (!d) continue;
                // no AddRef: GetLocalDeals already AddRefs each deal and the SC4List destructor releases it (an extra AddRef would leak a reference per deal per read)
                const std::int32_t t = d->GetType();
                if (!IsKnownDealType(t)) { dealsOk = false; continue; }
                ExistingDealRow row;
                row.pointer = reinterpret_cast<std::uintptr_t>(d);
                row.type = t;
                row.otherCitySerial = SerialOf(d->GetCityOnOtherSideOfConnection());
                row.amount = d->GetAmount();
                row.lastAmount = d->GetLastAmount();
                row.incAmount = d->GetIncAmount();
                row.incAmount2 = d->GetIncAmount2();
                row.amountUsed = d->GetAmountUsed();
                row.flags = d->GetFlags();
                row.cost = d->GetCost();
                if (scratch.date) {
                    d->GetStartDate(*scratch.date);
                    row.start = StampOf(scratch.date);
                    d->GetExpirationDate(*scratch.date);
                    row.expiration = StampOf(scratch.date);
                    d->GetCurrentDate(*scratch.date);
                    row.current = StampOf(scratch.date);
                    d->GetSuspensionDate(*scratch.date);
                    row.suspension = StampOf(scratch.date);
                }
                state.deals.push_back(row);
            }
        } else {
            dealsOk = false;
        }
    } else {
        dealsOk = false;
    }
    for (std::int32_t t = utility_id::kDealBuyPower; t <= utility_id::kDealImportGarbage; ++t) {
        SC4List<cISC4NeighborConnection*> list;
        ns->FindDeals(t, list);
        std::size_t n = 0;
        for (auto* raw : list) if (raw) ++n;
        state.findDealsCount.push_back(n);
    }
    state.monthlyIncome = ns->GetTotalMonthlyNeighborDealIncome();
    state.monthlyExpense = ns->GetTotalMonthlyNeighborDealExpense();

    // effective tuning: the exemplar the neighbor simulator itself uses
    static const std::uint32_t kTuningIds[] = {
        0xCA46931F, 0xEA46932C, 0x6A469337, 0x8A46933D, 0x6A469342, 0xCA469346, 0xAA46934B, 0xEA46934F,
        0x0A469353, 0xEA4AD2FF, 0xEA4AE9EF, 0x6B588FAD, 0x8B58C59E, 0x4B61FFC9};
    cISCResExemplar* tuning = ns->GetTuningExemplar();
    cISCPropertyHolder* holder = tuning ? tuning->AsISCPropertyHolder() : nullptr;
    for (const auto id : kTuningIds) {
        TuningValue v;
        ReadTuning(holder, id, v);
        state.tuning.push_back(v);
    }
    const auto tunedMin = [&](std::uint32_t id) -> Opt<double> {
        for (const auto& t : state.tuning)
            if (t.id == id && t.present && !t.values.empty()) return t.values.front();
        return std::nullopt;
    };

    // opportunities: exactly what the game itself asks for (ProposeDeals(list, false)); nothing fabricated
    if (includeProposals) {
        SC4List<cISC4NeighborConnection*> proposals;
        const bool ok = ns->ProposeDeals(proposals, false);
        std::vector<std::int32_t> proposedTypes;
        if (ok) {
            for (auto* raw : proposals) {
                auto* d = reinterpret_cast<cISC4NeighborDeal*>(raw);
                if (!d) continue;
                const std::int32_t t = d->GetType();
                if (IsKnownDealType(t)) {
                    DealOpportunityRow row;
                    row.type = t;
                    auto* other = d->GetCityOnOtherSideOfConnection();
                    row.otherCitySerial = SerialOf(other);
                    row.proposedAmount = d->GetAmount();
                    row.proposedCost = d->GetCost();
                    if (other) {
                        {
                            auto** vt = *reinterpret_cast<void***>(ns);
                            row.maxAmount = static_cast<std::int64_t>(reinterpret_cast<DealMaxFn>(vt[kNeighborsDealMax])(ns, t, other));
                        }
                        const auto minAmount = tunedMin(t == utility_id::kDealBuyPower || t == utility_id::kDealSellPower ? 0xEA46932Cu
                                                        : t == utility_id::kDealBuyWater || t == utility_id::kDealSellWater ? 0x6A469337u
                                                                                                                              : 0x8A46933Du);
                        if (minAmount) {
                            auto** vt = *reinterpret_cast<void***>(ns);
                            row.costAtMin = static_cast<std::int64_t>(reinterpret_cast<DealCostFn>(vt[kNeighborsDealCost])(ns, t, *minAmount, other));
                        }
                    }
                    if (other && row.maxAmount && *row.maxAmount > 0) {
                        // Ask the game for the total at several amounts: cost is never assumed linear.
                        auto** vt = *reinterpret_cast<void***>(ns);
                        const std::int64_t grid[] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000};
                        for (const auto amount : grid) {
                            if (amount > *row.maxAmount) break;
                            const double q = reinterpret_cast<DealCostFn>(vt[kNeighborsDealCost])(ns, t, static_cast<double>(amount), other);
                            row.quotes.emplace_back(amount, static_cast<std::int64_t>(q));
                        }
                    }
                    proposedTypes.push_back(t);
                    state.opportunities.push_back(row);
                } else {
                    dealsOk = false;
                }
            }
        } else {
            dealsOk = false;
        }
        const auto flags = EligibilityFromProposals(proposedTypes);
        state.canBuyPower = flags.canBuyPower;
        state.canSellPower = flags.canSellPower;
        state.canBuyWater = flags.canBuyWater;
        state.canSellWater = flags.canSellWater;
        state.canExportGarbage = flags.canExportGarbage;
        state.canImportGarbage = flags.canImportGarbage;
    }
    // Commodity cache + per-neighbor market. ProposeDeals above ran UpdateCommodityStats first, so the cache is fresh here; the
    // records are read through the same getters the game's formulas use.
    if (includeProposals && regional) {
        const auto readBook = [](cISC4RegionalCity* city, int commodity, CommodityBookRow& out) {
            out.produced = city->GetProduced(commodity);
            out.imported = city->GetImported(commodity);
            out.demanded = city->GetDemanded(commodity);
            out.exported = city->GetExported(commodity);
            out.costPerUnit = city->GetCostPerUnit(commodity);
        };
        state.localMarket.budget = regional->GetBudget();
        for (int c = 0; c < 3; ++c) readBook(regional, c + 1, state.localMarket.book[c]);
        for (const auto& [serial, other] : neighborCities) {
            NeighborMarketRow m;
            m.serial = serial;
            m.budget = other->GetBudget();
            for (int c = 0; c < 3; ++c) readBook(other, c + 1, m.book[c]);
            for (int t = 0; t < 6; ++t) {
                const std::int32_t type = utility_id::kDealBuyPower + t;
                m.suitable[t] = ns->FindConnectionSuitableForDeal(type, other) != nullptr;
                double v = 0.0;
                if (SafeDealMax(ns, type, other, v) && std::isfinite(v)) m.maxAmount[t] = static_cast<std::int64_t>(v);
            }
            state.neighbors.push_back(std::move(m));
        }
        std::sort(state.neighbors.begin(), state.neighbors.end(), [](const auto& a, const auto& b) { return a.serial < b.serial; });
        state.marketsRead = true;
    }
    if (!dealsOk) {
        state.validity = SnapshotValidity::Partial;
        state.invalidReason = "a deal list returned an unexpected object";
    } else {
        const std::size_t items = state.connections.size() + state.deals.size();
        state.validity = DeriveValidity(true, true, items, false, epochMatches);
    }
    return state;
}

} // namespace

Opt<bool> ReadNeighborDealsUiDisabled(cISC4City* city) {
    return NeighborDealsUiDisabled(city ? city->GetNeighborsSimulator() : nullptr);
}

bool ReadServiceCountersFromCity(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t epoch, ServiceCounters& out) {
    out = ServiceCounters{};
    out.epoch = epoch;
    if (!city) return false;
    if (auto* sim = city->GetPowerSimulator()) {
        out.power = true;
        out.pRequirement = sim->GetMonthlyRequirement();
        out.pProduction = sim->GetMonthlyProduction();
        out.pImport = sim->GetMonthlyImport();
        out.pDemand = sim->GetMonthlyDemand();
        out.pConsumption = sim->GetMonthlyConsumption();
        out.pExport = sim->GetMonthlyExport();
        // effective local capacity: what each plant can deliver at its present funding (maximum output x efficiency), summed
        ilist<cISC4Occupant*> plants;
        sim->GetPowerPlants(plants);
        std::int64_t capacity = 0;
        bool known = true;
        for (auto* occ : plants) {
            if (!occ) continue;
            std::uint32_t maxOut = 0;
            sim->GetOccupantMaxPowerProduced(occ, maxOut);
            const auto eff = PlantPercent(sim, kSlotPlantEfficiency, occ);
            if (!eff) { known = false; continue; }
            capacity += static_cast<std::int64_t>(static_cast<double>(maxOut) * static_cast<double>(*eff) + 0.5);
        }
        if (known) out.pCapacity = capacity;
        if (regional) {
            out.pGameImported = static_cast<std::int64_t>(regional->GetImported(utility_id::kCommodityPower));
            out.pGameExported = static_cast<std::int64_t>(regional->GetExported(utility_id::kCommodityPower));
        }
    }
    if (auto* pol = city->GetPollutionSimulator()) {
        out.garbage = true;
        out.gProduced = static_cast<std::int64_t>(pol->GetGarbageProducedLastMonth());
        out.gUncollected = static_cast<std::int64_t>(pol->GetTotalGarbageUncollected());
        out.gW2e = static_cast<std::int64_t>(pol->GetMonthlyWasteToEnergyCapacity());
        out.gIncinerator = static_cast<std::int64_t>(pol->GetMonthlyIncineratorCapacity());
        if (const auto eff = PollutionFundingEfficiency(pol)) out.gEfficiency = static_cast<std::int64_t>(std::llround(static_cast<double>(*eff) * 10000.0));
        if (regional) {
            out.gGameImported = static_cast<std::int64_t>(regional->GetImported(utility_id::kCommodityGarbage));
            out.gGameExported = static_cast<std::int64_t>(regional->GetExported(utility_id::kCommodityGarbage));
        }
    }
    if (auto* sim = city->GetPlumbingSimulator()) {
        out.water = true;
        out.wCapacity = CallInt32(sim, kPlumbCapacity);
        out.wConsumed = CallInt32(sim, kPlumbConsumed);
        out.wImport = CallInt32(sim, kPlumbImported);
        out.wExport = CallInt32(sim, kPlumbExported);
        out.wDemand = CallInt32(sim, kPlumbDemand);
        out.wWatered = static_cast<std::int64_t>(sim->GetNumWateredBuildings());
        out.wUnwatered = static_cast<std::int64_t>(sim->GetNumUnwateredBuildings());
        if (regional) {
            out.wGameImported = static_cast<std::int64_t>(regional->GetImported(utility_id::kCommodityWater));
            out.wGameExported = static_cast<std::int64_t>(regional->GetExported(utility_id::kCommodityWater));
        }
    }
    return out.power || out.water || out.garbage;
}

UtilitySnapshot ReadUtilitySnapshot(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t epoch, bool epochMatches,
                                    bool includeProposals, bool probeLatent) {
    UtilitySnapshot snap;
    snap.lifecycleEpoch = epoch;
    if (!city) return snap;  // every subsystem stays Invalid
    snap.citySerial = city->GetCitySerialNumber();
    auto* budget = city->GetBudgetSimulator();
    if (auto* sim = city->GetSimulator()) {
        std::uint32_t year = 0, month = 0, day = 0, dayOfYear = 0, weekDay = 0;
        sim->GetSimDate(&year, &month, &day, &dayOfYear, &weekDay);
        snap.simYear = year;
        snap.simDayOfYear = dayOfYear;
    }
    snap.power = ReadPower(city, budget, regional, epochMatches);
    snap.water = ReadWater(city, budget, regional, epochMatches);
    snap.garbage = ReadGarbage(city, budget, regional, snap.power, epochMatches);
    snap.deals = ReadDeals(city, regional, includeProposals, epochMatches, probeLatent);
    snap.deals.proposalsRead = includeProposals;
    {
        const auto& g = snap.garbage.aggregate;
        if (g.landfillCapacity && g.landfillUsage && *g.landfillCapacity >= *g.landfillUsage)
            snap.deals.localMarket.freeLandfillStorage = static_cast<std::int64_t>(*g.landfillCapacity) - *g.landfillUsage;
    }
    return snap;
}

// ---- config ---------------------------------------------------------------------------------------------------
namespace {
std::uint64_t FileWriteTime(const std::string& path) {
    WIN32_FILE_ATTRIBUTE_DATA data{};
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) return 0;
    return (static_cast<std::uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime;
}
} // namespace

void UtilityProbe::LoadConfigIfChanged() {
    if (configPath_.empty()) return;
    const auto stamp = FileWriteTime(configPath_);
    if (stamp == lastWriteTime_) return;
    lastWriteTime_ = stamp;
    config_ = UtilityResearchConfig{};
    if (stamp == 0) return;  // no file: defaults (read-only research on, quiet)
    std::ifstream in(configPath_, std::ios::binary);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string error;
    if (!ParseUtilityResearchConfig(IniSectionText(buffer.str(), kIniSecUtility), config_, error)) {      // only the [UtilityResearch] section
        config_ = UtilityResearchConfig{};
        Log(LogLevel::Warning, "[Utility] DepartmentEfficiency.ini [UtilityResearch] invalid (" + error + "); using defaults");
    } else if (!config_.obsoleteKeys.empty()) {
        std::string names;
        for (const auto& k : config_.obsoleteKeys) names += (names.empty() ? "" : ", ") + k;
        Log(LogLevel::Warning, "[Utility] DepartmentEfficiency.ini [UtilityResearch]: obsolete key(s) ignored: " + names +
                                   "; AutoTradePower / AutoTradeWater / AutoTradeGarbage now control the contracts of each resource");
    }
}

std::string UtilityProbe::Banner() const {
    // What the Department is actually set to manage (read from the configuration, never a fixed statement): each manager is On only when the master switch is Auto AND its own key is true.
    const bool autoOn = config_.enabled && config_.mode == UtilityMode::Auto;
    const auto on = [&](bool key) { return autoOn && key ? "On" : "Off"; };
    return std::string("UtilityResearch=") + (config_.enabled ? "Enabled" : "Disabled") + " UtilityOptimizer=" + UtilityModeName(config_.mode) +
           " PowerAuto=" + on(config_.autoPower) + " WaterAuto=" + on(config_.autoWater) + " GarbageAuto=" + on(config_.autoGarbage) + " DealsAuto=" + on(config_.autoDeals) +
           " " + PolicyText(config_) + " Phase8Runtime=" + (config_.phase8Runtime ? "Passive" : "Off");
}

// ---- logging ------------------------------------------------------------------------------------------------
namespace {

// One-time dump of the vtable entry addresses of the interfaces we call, so the executable can be disassembled
// offline (the SDK headers are not always right for 1.1.641.0; see PlantPercent). Read-only.
void DumpVtable(const char* name, const void* object, int slots) {
    if (!object) { Log(LogLevel::Info, std::string("[Utility][Vtable] ") + name + " null"); return; }
    auto** vtable = *reinterpret_cast<void** const*>(object);
    std::ostringstream s;
    s << "[Utility][Vtable] " << name << " vtable=0x" << std::hex << reinterpret_cast<std::uintptr_t>(vtable) << std::dec << " slots:";
    for (int i = 0; i < slots; ++i)
        s << (i % 8 == 0 ? "\n  " : " ") << i << "=0x" << std::hex << reinterpret_cast<std::uintptr_t>(vtable[i]) << std::dec;
    Log(LogLevel::Info, s.str());
}

std::string PowerSummary(const PowerState& p) {
    const auto& a = p.aggregate;
    std::ostringstream s;
    s << "[Utility][Power] valid=" << ValidityName(p.validity) << " requirement=" << O(a.requirement) << " demand=" << O(a.demand)
      << " production=" << O(a.production) << " consumption=" << O(a.consumption) << " import=" << O(a.import_) << " export=" << O(a.export_)
      << " gameImported=" << O(p.gameImported) << " gameExported=" << O(p.gameExported) << " powered=" << O(a.powered) << " unpowered=" << O(a.unpowered)
      << " plants=" << p.plants.size() << "/" << O(a.plantCount) << " overworked=" << O(a.overworked) << " aged=" << O(a.aged)
      << " distressed=" << O(a.distressed) << " poles=" << O(a.poleCount) << " simFunding=" << OPct(a.simulatorFunding)
      << " deptFunding=" << OPct(p.department.funding) << " deptMax=" << OPct(p.department.maxAllowed)
      << " deptIdeal=" << O(p.department.idealMonthly) << " deptExpenses=" << O(p.department.totalExpenses)
      << " deptSpending=" << O(p.department.totalSpending);
    return s.str();
}

std::string WaterSummary(const WaterState& w) {
    const auto& a = w.aggregate;
    std::ostringstream s;
    s << "[Utility][Water] valid=" << ValidityName(w.validity) << " capacity=" << O(a.productionCapacity) << " demand=" << O(a.demand)
      << " consumed=" << O(a.consumed) << " import=" << O(a.imported) << " export=" << O(a.exported) << " gameImported=" << O(w.gameImported)
      << " gameExported=" << O(w.gameExported) << " sources=" << O(a.sources) << " polluted=" << O(a.pollutedSources)
      << " pipes=" << O(a.pipes) << " distressed=" << O(a.distressedPipes) << " burst=" << O(a.burstPipes)
      << " watered=" << O(a.wateredBuildings) << " unwatered=" << O(a.unwateredBuildings) << " producers=" << w.producers.size()
      << " deptPresent=" << (w.department.present ? "true" : "false") << " deptFunding=" << OPct(w.department.funding)
      << " deptMax=" << OPct(w.department.maxAllowed) << " fixed=" << O(w.department.fixedFunding);
    return s.str();
}

std::string GarbageSummary(const GarbageState& g) {
    const auto& a = g.aggregate;
    std::ostringstream s;
    s << "[Utility][Garbage] valid=" << ValidityName(g.validity) << " produced=" << O(a.produced) << " toLandfill=" << O(a.toLandfill)
      << " recycled=" << O(a.recycled) << " incinerated=" << O(a.incinerated) << " toEnergy=" << O(a.toEnergy) << " imported=" << O(a.imported)
      << " exported=" << O(a.exported) << " gameImported=" << O(g.gameImported) << " gameExported=" << O(g.gameExported)
      << " uncollected=" << O(a.uncollected) << " supply=" << O(a.garbageSupply) << " landfillUsage=" << O(a.landfillUsage)
      << " landfillCapacity=" << O(a.landfillCapacity) << " tiles=" << O(a.landfillTiles) << " landfillEff=" << OPct(a.landfillEfficiency)
      << " landfillMaint=" << O(a.landfillMaintenance) << " incinCap=" << O(a.incineratorCapacityMonthly) << " w2eCap=" << O(a.w2eCapacityMonthly)
      << " recyclingEffect=" << O(a.recyclingEffect) << " abducted=" << O(a.abducted) << " uiScaling=" << O(a.uiScalingFactor)
      << " fundingEfficiency=" << O(a.fundingEfficiency)
      << " buildings=" << g.buildings.size() << " deptFunding=" << OPct(g.department.funding)
      << " deptIdeal=" << O(g.department.idealMonthly) << " deptExpenses=" << O(g.department.totalExpenses)
      << " accountingResidual=UNKNOWN(delta-stock-not-yet-sampled)";
    return s.str();
}

std::string DealsSummary(const DealState& d) {
    std::ostringstream s;
    s << "[Utility][Deals] valid=" << ValidityName(d.validity) << " connections=" << d.connections.size() << " deals=" << d.deals.size()
      << " findDealsCounts=" << [&]() { std::ostringstream c; for (std::size_t i = 0; i < d.findDealsCount.size(); ++i) c << (i ? "/" : "") << d.findDealsCount[i]; return c.str(); }()
      << " opportunities=" << (d.proposalsRead ? std::to_string(d.opportunities.size()) : std::string("NOT_READ_YET")) << " income=" << O(d.monthlyIncome) << " expense=" << O(d.monthlyExpense)
      << " present(power/water/garbage)=" << O(d.connectionPower) << '/' << O(d.connectionWater) << '/' << O(d.connectionGarbage)
      << " can(buyP/sellP/buyW/sellW/expG/impG)=" << O(d.canBuyPower) << '/' << O(d.canSellPower) << '/' << O(d.canBuyWater) << '/'
      << O(d.canSellWater) << '/' << O(d.canExportGarbage) << '/' << O(d.canImportGarbage);
    return s.str();
}

std::string PowerRows(const PowerState& p) {
    std::ostringstream s;
    for (const auto& r : p.plants) {
        s << "  plant cell=" << r.ref.cellX << ',' << r.ref.cellZ << " name=" << r.ref.exemplarName << " groups=" << GroupsText(r.ref.groups)
          << " mechanism=" << MechanismName(r.mechanism) << " w2e=" << (r.wasteToEnergy() ? "true" : "false") << " funding=" << OPct(r.funding)
          << " produced=" << O(r.produced) << " used=" << O(r.used) << " max=" << O(r.maxProduced) << " eff=" << O(r.efficiencyPercent)
          << " cond=" << O(r.conditionPercent) << " cost=" << O(r.monthlyCost) << " powered=" << O(r.powered)
          << " baseAging=" << O(r.baseAgingRate) << " agedThreshold=" << O(r.agedThreshold) << " replacementCost=" << O(r.replacementCost);
        if (r.budgetLine) s << " line(baseCost=" << r.budgetLine->cost << " local=" << r.budgetLine->localFunding << ')';
        s << '\n';
    }
    return s.str();
}

std::string WaterRows(const WaterState& w) {
    std::ostringstream s;
    for (const auto& r : w.producers) {
        s << "  producer cell=" << r.ref.cellX << ',' << r.ref.cellZ << " name=" << r.ref.exemplarName << " groups=" << GroupsText(r.ref.groups)
          << " mechanism=" << MechanismName(r.mechanism) << " infoOk=" << (r.infoOk ? "true" : "false") << " cur=" << O(r.currentCapacity)
          << " max=" << O(r.maxCapacity) << " used=" << O(r.waterUsed) << " production=" << O(r.production) << " eff=" << O(r.efficiencyPercent)
          << " cond=" << O(r.conditionPercent) << " shutDownByPollution=" << O(r.shutDownByPollution) << " upkeep=" << O(r.upkeep);
        if (r.budgetLine) s << " line(baseCost=" << r.budgetLine->cost << " local=" << r.budgetLine->localFunding << ')';
        s << '\n';
    }
    return s.str();
}

std::string GarbageRows(const GarbageState& g) {
    std::ostringstream s;
    for (const auto& r : g.buildings) {
        s << "  building cell=" << r.ref.cellX << ',' << r.ref.cellZ << " name=" << r.ref.exemplarName << " groups=" << GroupsText(r.ref.groups)
          << " alsoPowerPlant=" << (r.alsoPowerPlant ? "true" : "false") << " capacity=" << O(r.capacity) << " cond=" << O(r.conditionPercent)
          << " eff=" << O(r.efficiencyPercent) << " incinUsed=" << O(r.incineratorUsed) << " w2eUsed=" << O(r.w2eUsed)
          << " incinCur=" << O(r.incineratorCurrent) << " w2eCur=" << O(r.w2eCurrent);
        if (r.budgetLine) s << " line(baseCost=" << r.budgetLine->cost << " local=" << r.budgetLine->localFunding << ')';
        s << '\n';
    }
    return s.str();
}

std::string DealRows(const DealState& d) {
    std::ostringstream s;
    for (const auto& c : d.connections)
        s << "  connection type=" << c.type << " cell=" << c.cellX << ',' << c.cellZ << " active=" << (c.active ? "true" : "false")
          << " other=" << Serial(c.otherCitySerial) << " carries=" << (c.carries == Commodity::Power ? "power" : c.carries == Commodity::Water ? "water" : c.carries == Commodity::Garbage ? "garbage" : "none") << '\n';
    for (const auto& x : d.deals)
        s << "  deal " << DealTypeName(x.type) << " other=" << Serial(x.otherCitySerial) << " amount=" << O(x.amount) << " last=" << O(x.lastAmount)
          << " inc=" << O(x.incAmount) << "/" << O(x.incAmount2) << " used=" << O(x.amountUsed) << " cost=" << O(x.cost) << " flags=" << O(x.flags)
          << " start=" << Date(x.start) << " expires=" << Date(x.expiration) << " now=" << Date(x.current) << " suspended=" << Date(x.suspension) << '\n';
    for (const auto& o : d.opportunities) {
        s << "  opportunity " << DealTypeName(o.type) << " other=" << Serial(o.otherCitySerial) << " proposedAmount=" << O(o.proposedAmount)
          << " proposedCost=" << O(o.proposedCost) << " maxAmount=" << O(o.maxAmount) << " costAtMin=" << O(o.costAtMin) << " quotes=";
        for (std::size_t i = 0; i < o.quotes.size(); ++i) s << (i ? "," : "") << o.quotes[i].first << ':' << o.quotes[i].second;
        s << '\n';
    }
    for (const auto& t : d.tuning) {
        s << "  tuning 0x" << std::hex << t.id << std::dec << (t.present ? " " : " ABSENT ") << t.type << " values=";
        for (std::size_t i = 0; i < t.values.size(); ++i) s << (i ? "," : "") << t.values[i];
        s << '\n';
    }
    return s.str();
}

// Logs `line` when the subsystem is non-trivial or changed; empty subsystems are silent after their first mention.
void LogIfChanged(std::map<std::string, std::string>& memory, const std::string& key, const std::string& text, LogLevel level,
                  bool force) {
    auto it = memory.find(key);
    if (!force && it != memory.end() && it->second == text) return;
    memory[key] = text;
    Log(level, text);
}

} // namespace

std::string ReconcileLine(const UtilitySnapshot& s) {
    const auto sums = SumDeals(s.deals.deals);
    const auto pair = [&](int i) {
        std::ostringstream o;
        o << sums.contracted[i] << '/' << sums.used[i] << " cost=" << sums.cost[i] << " n=" << sums.count[i];
        return o.str();
    };
    const auto idx = [](std::int32_t t) { return t - utility_id::kDealBuyPower; };
    std::ostringstream o;
    o << "[Utility][Reconcile] date=" << s.simYear << '/' << s.simDayOfYear
      << " power: dealsBuy(contracted/used)=" << pair(idx(utility_id::kDealBuyPower)) << " simImport=" << O(s.power.aggregate.import_)
      << " gameImported=" << O(s.power.gameImported) << " demand=" << O(s.power.aggregate.demand) << " consumption=" << O(s.power.aggregate.consumption)
      << " | water: dealsBuy=" << pair(idx(utility_id::kDealBuyWater)) << " simImport=" << O(s.water.aggregate.imported)
      << " gameImported=" << O(s.water.gameImported) << " demand=" << O(s.water.aggregate.demand) << " consumed=" << O(s.water.aggregate.consumed)
      << " | garbage: dealsExport=" << pair(idx(utility_id::kDealExportGarbage)) << " simExported=" << O(s.garbage.aggregate.exported)
      << " gameImportedIdx3=" << O(s.garbage.gameImported) << " dealsImport=" << pair(idx(utility_id::kDealImportGarbage))
      << " simImported=" << O(s.garbage.aggregate.imported) << " produced=" << O(s.garbage.aggregate.produced)
      << " recycled=" << O(s.garbage.aggregate.recycled) << " toEnergy=" << O(s.garbage.aggregate.toEnergy)
      << " toLandfill=" << O(s.garbage.aggregate.toLandfill) << " incinerated=" << O(s.garbage.aggregate.incinerated)
      << " uncollected=" << O(s.garbage.aggregate.uncollected);
    return o.str();
}

void UtilityProbe::LogVtables(cISC4City* city, cISC4RegionalCity* regional) {
    DumpVtable("PowerSimulator", city->GetPowerSimulator(), 56);
    DumpVtable("PlumbingSimulator", city->GetPlumbingSimulator(), 30);
    DumpVtable("PollutionSimulator", city->GetPollutionSimulator(), 80);
    DumpVtable("LandfillDeveloper", city->GetLandfillDeveloper(), 14);
    DumpVtable("NeighborsSimulator", city->GetNeighborsSimulator(), 46);
    DumpVtable("BudgetSimulator", city->GetBudgetSimulator(), 72);
    DumpVtable("RegionalCity", regional, 95);
    if (auto* budget = city->GetBudgetSimulator())
        DumpVtable("DepartmentBudget(water)", budget->GetDepartmentBudget(utility_id::kWaterDepartment), 42);
    if (regional) {
        SC4List<cISC4NeighborDeal*> deals;
        if (regional->GetLocalDeals(deals))
            for (auto* d : deals) { DumpVtable("NeighborDeal", d, 44); break; }
    }
}

void UtilityProbe::Observe(cISC4City* city, cISC4RegionalCity* regional, const char* reason, bool includeProposals) {
    LoadConfigIfChanged();
    if (!config_.enabled || !city) return;
    const bool first = !initialDone_;
    if (std::string(reason) == "SimNewMonth") ++monthsSeen_;
    // The deal simulator only populates proposals after simulation progress: before the first month of this
    // city lifecycle they are reported as NOT READ (unknown), never as "no opportunities".
    last_ = ReadUtilitySnapshot(city, regional, epoch_, true, includeProposals && monthsSeen_ > 0 && DealDiscoveryAllowed(config_), config_.latentProbe || config_.mode != UtilityMode::Off);
    if (!vtablesLogged_) { vtablesLogged_ = true; LogVtables(city, regional); }
    initialDone_ = true;
    const auto& s = last_;

    std::ostringstream head;
    head << "[Utility] snapshot reason=" << reason << " epoch=" << s.lifecycleEpoch << " city=" << s.citySerial << " date=" << s.simYear
         << '/' << s.simDayOfYear;
    Log(LogLevel::Info, head.str());

    const auto emit = [&](const char* key, SnapshotValidity validity, const std::string& summary, const std::string& rows) {
        // Validity transitions are always logged; INVALID is a warning, everything else info.
        const std::string v = ValidityName(validity);
        const bool validityChanged = lastValidity_[key] != v;
        lastValidity_[key] = v;
        const LogLevel level = validity == SnapshotValidity::Invalid ? LogLevel::Warning : LogLevel::Info;
        // A steady VALID_EMPTY subsystem is silent after its first mention (normal gameplay, no spam).
        const bool silentEmpty = validity == SnapshotValidity::ValidEmpty && !first && !validityChanged;
        if (!silentEmpty) Log(level, summary);
        if (!silentEmpty && !rows.empty()) {
            if (config_.detail || first || validityChanged) {
                lastDetail_[key] = rows;
                Log(LogLevel::Info, std::string("[Utility][") + key + "][Rows]\n" + rows);
            } else {
                LogIfChanged(lastDetail_, key, rows, LogLevel::Info, false);
            }
        }
    };
    emit("Power", s.power.validity, PowerSummary(s.power), PowerRows(s.power));
    emit("Water", s.water.validity, WaterSummary(s.water), WaterRows(s.water));
    emit("Garbage", s.garbage.validity, GarbageSummary(s.garbage), GarbageRows(s.garbage));
    emit("Deals", s.deals.validity, DealsSummary(s.deals), DealRows(s.deals));
    Log(LogLevel::Info, ReconcileLine(s));
}

} // namespace sc4af
