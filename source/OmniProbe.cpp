// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "OmniProbe.h"

#include "CivicClassifier.h"
#include "CivicInventory.h"
#include "Logging.h"
#include "NativeCivicQueryReader.h"

#include "SC4List.h"
#include "SC4Percentage.h"
#include "SC4Vector.h"
#include "cIGZString.h"
#include "cIGZVariant.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4BuildingOccupant.h"
#include "cISC4City.h"
#include "cISC4Occupant.h"
#include "cISC4ResidentialSimulator.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cRZAutoRefCount.h"

#include <cstdint>
#include <cstring>
#include <exception>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {
namespace {

// Exemplar/runtime property IDs (SC4Devotion building subfile table + exemplar inventory).
constexpr std::uint32_t kPropOccupantGroups = 0xAA1DD396;
constexpr std::uint32_t kPropBudgetDepartments = 0xEA54D283;
constexpr std::uint32_t kPropBudgetPurposes = 0xEA54D285;
constexpr std::uint32_t kPropBudgetCosts = 0xEA54D286;
constexpr std::uint32_t kPropSchoolUsageRuntime = 0x09220161;
constexpr std::uint32_t kPropHospitalUsageRuntime = 0xC922015C;
constexpr std::uint32_t kPropSchoolCapacityRuntime = 0xE9244B2E;
constexpr std::uint32_t kPropHospitalCapacityRuntime = 0x49244B21;

constexpr std::uint32_t kMaxArray = 64;

// Bounded scan of occupant groups: IsOccupantGroup is a scalar, side-effect-free query.
std::vector<std::uint32_t> ScanGroups(cISC4Occupant* occupant) {
    std::vector<std::uint32_t> groups;
    const auto scan = [&](std::uint32_t lo, std::uint32_t hi) {
        for (std::uint32_t id = lo; id <= hi; ++id)
            if (occupant->IsOccupantGroup(id)) groups.push_back(id);
    };
    scan(0x1000, 0x100F);
    scan(0x1500, 0x153F);
    scan(0x1900, 0x193F);
    scan(0x1A00, 0x1A0F);
    return groups;
}

// Reads a UINT32 property (scalar or array) through the property holder; nullopt if absent.
std::optional<std::vector<std::uint32_t>> ReadU32Property(cISCPropertyHolder* holder,
                                                         std::uint32_t id) {
    if (!holder || !holder->HasProperty(id)) return std::nullopt;
    cISCProperty* property = holder->GetProperty(id);
    if (!property) return std::nullopt;
    cIGZVariant* value = property->GetPropertyValue();
    if (!value) return std::nullopt;
    std::vector<std::uint32_t> out;
    const auto type = value->GetType();
    if (type == cIGZVariant::Uint32Array) {
        const std::uint32_t count = value->GetCount();
        const std::uint32_t* data = value->RefUint32();
        if (!data) return std::nullopt;
        for (std::uint32_t i = 0; i < count && i < kMaxArray; ++i) out.push_back(data[i]);
    } else if (type == cIGZVariant::Uint32) {
        out.push_back(value->GetValUint32());
    } else {
        return std::nullopt;
    }
    return out;
}

std::optional<std::uint32_t> ReadU32Scalar(cISCPropertyHolder* holder, std::uint32_t id) {
    if (!holder) return std::nullopt;
    std::uint32_t value = 0;
    if (!holder->GetProperty(id, value)) return std::nullopt;
    return value;
}

void AppendHexList(std::ostringstream& out, const std::vector<std::uint32_t>& values) {
    out << '[';
    for (std::size_t i = 0; i < values.size(); ++i)
        out << (i ? "," : "") << "0x" << std::hex << std::uppercase << values[i] << std::dec;
    out << ']';
}

void AppendOptional(std::ostringstream& out, const std::optional<std::uint32_t>& v) {
    if (v) out << *v; else out << "UNAVAILABLE";
}

float AsFloat(std::uint32_t bits) {
    float f;
    std::memcpy(&f, &bits, sizeof(f));
    return f;
}

struct Probed {
    InventoryEntry entry;
    std::string text;
};

Probed ProbeOccupant(const char* listName, std::size_t index, cISC4Occupant* occupant,
                     cISC4ResidentialSimulator* residential, cISC4BudgetSimulator* budget) {
    std::ostringstream sig;
    std::string exemplarName;
    const bool school = std::strcmp(listName, "School") == 0;
    std::ostringstream out;
    out << "[Omni][" << listName << "#" << index << "] RuntimeAddress=0x" << std::hex
        << std::uppercase << reinterpret_cast<std::uintptr_t>(occupant) << std::dec
        << " (ephemeral)\n";

    // Identity: names and building type (read-only getters).
    cRZAutoRefCount<cISC4BuildingOccupant> building;
    if (occupant->QueryInterface(GZIID_cISC4BuildingOccupant, building.AsPPVoid())) {
        out << "  BuildingType=0x" << std::hex << building->GetBuildingType() << std::dec;
        if (cIGZString* name = building->GetExemplarName()) {
            exemplarName = name->ToChar() ? name->ToChar() : "";
            out << " ExemplarName=\"" << exemplarName << '"';
        }
        out << '\n';
    }

    auto* holder = occupant->AsPropertyHolder();
    const auto scanned = ScanGroups(occupant);
    const auto propertyGroups = ReadU32Property(holder, kPropOccupantGroups);
    const auto purposes = ReadU32Property(holder, kPropBudgetPurposes);
    out << "  GroupsByScan=";
    AppendHexList(out, scanned);
    out << " GroupsByProperty=";
    if (propertyGroups) AppendHexList(out, *propertyGroups); else out << "UNAVAILABLE";
    out << '\n';
    if (const auto costs = ReadU32Property(holder, kPropBudgetCosts)) {
        out << "  ExemplarBudgetCosts=[";
        for (std::size_t i = 0; i < costs->size(); ++i) out << (i ? "," : "") << (*costs)[i];
        out << "] Purposes=";
        AppendHexList(out, purposes ? *purposes : std::vector<std::uint32_t>{});
        out << " Departments=";
        const auto departments = ReadU32Property(holder, kPropBudgetDepartments);
        AppendHexList(out, departments ? *departments : std::vector<std::uint32_t>{});
        out << '\n';
    }
    const auto klass = ClassifyCivic(scanned, purposes ? *purposes : std::vector<std::uint32_t>{});
    const bool matchesList = school ? ListedBySchoolSimulator(klass) : ListedByHospitalSimulator(klass);
    sig << exemplarName << '|' << FamilyName(klass.family) << '/' << klass.subtype << '|';
    out << "  Classification=" << FamilyName(klass.family) << '/' << klass.subtype
        << " listedBy=" << listName << "List" << (matchesList ? "" : " (MISMATCH family vs list)") << '\n';

    // Runtime SGProps mirrored by the native query handlers.
    out << "  RuntimeProps usage=";
    AppendOptional(out, ReadU32Scalar(holder, school ? kPropSchoolUsageRuntime : kPropHospitalUsageRuntime));
    out << " capacity=";
    AppendOptional(out, ReadU32Scalar(holder, school ? kPropSchoolCapacityRuntime : kPropHospitalCapacityRuntime));
    out << '\n';

    // Native query path: usage dword (+0x24) and all buffer dwords.
    if (const auto raw = ReadQueryBufferRaw(residential, occupant, school)) {
        out << "  QueryBuffer dwords:";
        for (std::size_t i = 0; i < raw->size(); ++i)
            out << " +0x" << std::hex << (0x14 + i * 4) << std::dec << "=" << (*raw)[i];
        out << "\n  QueryBuffer floats: +0x2C=" << AsFloat((*raw)[(0x2C - 0x14) / 4])
            << " +0x34=" << AsFloat((*raw)[(0x34 - 0x14) / 4]) << " usage(+0x24)="
            << (*raw)[(0x24 - 0x14) / 4] << '\n';
    } else {
        out << "  QueryBuffer UNAVAILABLE (getter returned false)\n";
    }

    // Capacity: sentinel-initialised, boolean ignored (as vanilla does).
    std::uint32_t capA = 0xFFFFFFFF, capB = 0xFFFFFFFF;
    const bool capRet = residential->EstimateCurrentOccupantCapacity(occupant, capA, capB);
    out << "  Capacity out1=" << (capA == 0xFFFFFFFFu ? std::string("UNSET") : std::to_string(capA))
        << " out2(staff)=" << (capB == 0xFFFFFFFFu ? std::string("UNSET") : std::to_string(capB))
        << " (return=" << (capRet ? "true" : "false") << ")\n";
    // Capacity is deliberately NOT part of the change signature: the estimate jitters by
    // +-1 between calls; it follows funding, which is in the signature.

    // Funding getter with both values of the flag. Vanilla's writer passes true
    // for schools and false for hospitals; only getters are called here.
    for (const bool flag : {true, false}) {
        SC4Percentage a{}, b{};
        const bool ok = residential->GetOccupantFundingPercentages(occupant, a, b, flag);
        out << "  GetOccupantFundingPercentages(flag=" << (flag ? "true" : "false") << ") ret="
            << (ok ? "true" : "false") << " out1=" << a.percentage << " out2=" << b.percentage << '\n';
        if (flag == school) sig << "fund=" << a.percentage << '/' << b.percentage << '|';
    }

    // Budget items (per-line cost and local funding).
    if (budget && holder) {
        SC4Vector<cISC4BudgetSimulator::BudgetItem> items;
        if (budget->GetBudgetItemInfo(holder, items)) {
            for (const auto& item : items) {
                out << "  BudgetItem purpose=0x" << std::hex << std::uppercase << item.purpose
                    << " department=0x" << item.departmentID << std::dec << " cost=" << item.cost
                    << " localFunding=" << item.localFundingPercentage << " monthlyCost="
                    << RoundedFundingCost(item.cost, item.localFundingPercentage) << '\n';
                sig << "item=" << item.purpose << ':' << item.cost << ':' << item.localFundingPercentage << '|';
            }
        }
    }
    Probed result;
    result.entry = {reinterpret_cast<std::uintptr_t>(occupant), listName,
                    exemplarName.empty() ? std::string("?") : exemplarName, sig.str()};
    result.text = out.str();
    return result;
}

template <class List>
void ProbeList(const char* name, List& list, cISC4ResidentialSimulator* residential,
               cISC4BudgetSimulator* budget, std::vector<Probed>& out) {
    std::size_t index = 0;
    for (auto* occupant : list) {
        if (!occupant) continue;
        try {
            out.push_back(ProbeOccupant(name, index, occupant, residential, budget));
        } catch (const std::exception& error) {
            Log(LogLevel::Warning, std::string("[Omni] occupant skipped: ") + error.what());
        }
        ++index;
    }
}

// Previous inventory, valid only for the current city session.
std::vector<InventoryEntry> gPrevious;
bool gHaveBaseline = false;
std::int32_t gLastSchoolCount = -1;
std::int32_t gLastHospitalCount = -1;

} // namespace

void OmniReset() {
    gPrevious.clear();
    gHaveBaseline = false;
    gLastSchoolCount = -1;
    gLastHospitalCount = -1;
}

bool OmniCountsChanged(cISC4City* city) {
    if (!city || !gHaveBaseline) return false;
    auto* residential = city->GetResidentialSimulator();
    if (!residential) return false;
    return residential->GetSchoolBuildingCount() != gLastSchoolCount ||
           residential->GetHospitalBuildingCount() != gLastHospitalCount;
}

void RunOmniRefresh(cISC4City* city, const char* reason, bool fullDump) {
    if (!city) return;
    auto* residential = city->GetResidentialSimulator();
    if (!residential) {
        Log(LogLevel::Warning, "[Omni] ResidentialSimulator unavailable");
        return;
    }
    auto* budget = city->GetBudgetSimulator();
    std::vector<Probed> probed;
    {
        SC4List<cISC4Occupant*> schools;
        if (!residential->GetSchoolBuildings(schools)) {
            Log(LogLevel::Warning, "[Omni] GetSchoolBuildings failed; refresh skipped");
            return; // never diff against a partial read
        }
        ProbeList("School", schools, residential, budget, probed);
    }
    {
        SC4List<cISC4Occupant*> hospitals;
        if (!residential->GetHospitalBuildings(hospitals)) {
            Log(LogLevel::Warning, "[Omni] GetHospitalBuildings failed; refresh skipped");
            return;
        }
        ProbeList("Hospital", hospitals, residential, budget, probed);
    }

    std::vector<InventoryEntry> current;
    for (const auto& p : probed) current.push_back(p.entry);
    const auto diff = DiffInventory(gPrevious, current);
    gLastSchoolCount = residential->GetSchoolBuildingCount();
    gLastHospitalCount = residential->GetHospitalBuildingCount();

    std::size_t schoolCount = 0, hospitalCount = 0;
    for (const auto& e : current) (e.list == "School" ? schoolCount : hospitalCount)++;

    const auto textFor = [&](const InventoryEntry& entry) -> std::string {
        for (const auto& p : probed)
            if (p.entry.key == entry.key && p.entry.list == entry.list) return p.text;
        return std::string();
    };

    if (fullDump || !gHaveBaseline) {
        Log(LogLevel::Info, std::string("[Omni] full scan reason=") + reason +
            " schools=" + std::to_string(schoolCount) + " hospitals=" + std::to_string(hospitalCount) +
            " (read-only; no setter is called)");
        for (const auto& p : probed) Log(LogLevel::Info, p.text);
    } else if (!diff.empty()) {
        Log(LogLevel::Info, std::string("[Omni][Diff] reason=") + reason +
            " +" + std::to_string(diff.added.size()) + " -" + std::to_string(diff.removed.size()) +
            " ~" + std::to_string(diff.changed.size()) + " now schools=" + std::to_string(schoolCount) +
            " hospitals=" + std::to_string(hospitalCount));
        for (const auto& e : diff.added) {
            Log(LogLevel::Info, "[Omni][Diff] ADDED " + e.list + " " + e.label);
            Log(LogLevel::Info, textFor(e));
        }
        for (const auto& e : diff.removed)
            Log(LogLevel::Info, "[Omni][Diff] REMOVED " + e.list + " " + e.label);
        for (const auto& c : diff.changed) {
            Log(LogLevel::Info, "[Omni][Diff] CHANGED " + c.second.list + " " + c.second.label +
                "\n  before: " + c.first.signature + "\n  after:  " + c.second.signature);
            Log(LogLevel::Info, textFor(c.second));
        }
    }
    gPrevious = std::move(current);
    gHaveBaseline = true;
}

} // namespace sc4af
