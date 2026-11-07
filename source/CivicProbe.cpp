// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "CivicProbe.h"
#include "CivicClassifier.h"
#include "Logging.h"
#include "NativeCivicQueryReader.h"

#include "SC4List.h"
#include "SC4Percentage.h"
#include "SC4Vector.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4BuildingOccupant.h"
#include "cISC4City.h"
#include "cISC4Occupant.h"
#include "cISC4ResidentialSimulator.h"
#include "cRZAutoRefCount.h"

#include <array>
#include <cmath>
#include <exception>
#include <iomanip>
#include <sstream>

namespace sc4af {
namespace {

// Group numbers from the SDK's BuildingCategoryFlags comments.
// An IsOccupantGroup match hints at, but does not prove, a finer building subtype.
constexpr std::array<std::uint32_t, 5> kCandidateGroups{
    0x1503, 0x1504, 0x1505, 0x1506, 0x1507
};

// Budget purposes in game version 1.1.641.0.
constexpr std::uint32_t kSchoolStaffPurpose = 0xEA5654B6;
constexpr std::uint32_t kSchoolBusPurpose = 0x4A5654BA;
constexpr std::uint32_t kHospitalStaffPurpose = 0xCA565486;
constexpr std::uint32_t kHospitalAmbulancePurpose = 0xEA56549E;

const char* EvidenceText(Evidence evidence) {
    switch (evidence) {
    case Evidence::HeaderCandidate: return "VERIFIED_HEADER/SEMANTIC_UNKNOWN";
    case Evidence::StaticTrace: return "VERIFIED_STATIC/NEEDS_UI_COMPARISON";
    case Evidence::RuntimeVerified: return "VERIFIED_RUNTIME";
    case Evidence::Unknown: return "UNKNOWN";
    case Evidence::Unavailable: return "UNAVAILABLE";
    case Evidence::Error: return "ERROR";
    }
    return "UNKNOWN";
}

template <class T> void WriteField(std::ostringstream& out, const char* name,
                                   const Observed<T>& field) {
    out << name << '=';
    if (field.value) out << *field.value;
    else out << "UNKNOWN";
    out << " source=" << field.source << " status=" << EvidenceText(field.evidence) << '\n';
}

CivicProbeRecord ReadRecord(CivicKind kind, cISC4Occupant* occupant,
                            cISC4ResidentialSimulator* residential,
                            cISC4BudgetSimulator* budget) {
    CivicProbeRecord record{};
    record.kind = kind;
    record.runtimeAddress = reinterpret_cast<std::uintptr_t>(occupant);
    const auto usage = kind == CivicKind::School
        ? ReadSchoolStudentCount(residential, occupant)
        : ReadHospitalPatientCount(residential, occupant);
    record.usage = {usage, kind == CivicKind::School
        ? "ResidentialSimulator::GetSchoolQueryData / vanilla 0x101C +0x24"
        : "ResidentialSimulator::GetHospitalQueryData / vanilla 0x1025 +0x24 (city-wide patient pool, same value for every hospital)",
        usage ? Evidence::RuntimeVerified : Evidence::Unavailable};
    record.monthlyCost = {std::nullopt, "budget item not found", Evidence::Unknown};
    if (!occupant) return record;

    std::uint32_t lotTag = 0;
    if (occupant->GetLotTag(lotTag))
        record.lotTagRaw = {lotTag, "Occupant::GetLotTag", Evidence::HeaderCandidate};
    else
        record.lotTagRaw = {std::nullopt, "Occupant::GetLotTag", Evidence::Unavailable};

    for (const auto group : kCandidateGroups) {
        if (occupant->IsOccupantGroup(group)) record.matchingKnownGroups.push_back(group);
    }

    cRZAutoRefCount<cISC4BuildingOccupant> building;
    if (occupant->QueryInterface(GZIID_cISC4BuildingOccupant, building.AsPPVoid())) {
        record.buildingTypeRaw = {building->GetBuildingType(),
                                  "BuildingOccupant::GetBuildingType", Evidence::HeaderCandidate};
    } else {
        record.buildingTypeRaw = {std::nullopt, "BuildingOccupant::GetBuildingType",
                                  Evidence::Unavailable};
    }

    // Vanilla's capacity query calls this vtable slot and ignores its return value.
    // Keep the initialized outputs as diagnostics even when it returns false.
    std::uint32_t rawA = 0xFFFFFFFF, rawB = 0xFFFFFFFF;
    record.capacityCallReturned = residential->EstimateCurrentOccupantCapacity(occupant, rawA, rawB);
    if (record.capacityCallReturned) {
        record.capacityRawA = {rawA, "ResidentialSimulator::EstimateCurrentOccupantCapacity/out1",
                               Evidence::HeaderCandidate};
        record.capacityRawB = {rawB, "ResidentialSimulator::EstimateCurrentOccupantCapacity/out2",
                               Evidence::HeaderCandidate};
    } else {
        record.capacityRawA = {rawA, "EstimateCurrentOccupantCapacity/out1 (return=false; diagnostic)",
                               Evidence::Unknown};
        record.capacityRawB = {rawB, "EstimateCurrentOccupantCapacity/out2 (return=false; diagnostic)",
                               Evidence::Unknown};
    }
    record.capacity = {rawA != 0xFFFFFFFFu ? std::optional<std::uint32_t>(rawA) : std::nullopt,
        "ResidentialSimulator::EstimateCurrentOccupantCapacity/out1; vanilla 0x101B/0x1024",
        rawA != 0xFFFFFFFFu ? Evidence::RuntimeVerified : Evidence::Unavailable};
    record.staffCount = {rawB != 0xFFFFFFFFu ? std::optional<std::uint32_t>(rawB) : std::nullopt,
        "EstimateCurrentOccupantCapacity/out2 (teachers/doctors; hospitals UI-matched 7 and 16)",
        rawB != 0xFFFFFFFFu ? (kind == CivicKind::Hospital ? Evidence::RuntimeVerified : Evidence::StaticTrace)
                            : Evidence::Unavailable};

    SC4Percentage fundingA{}, fundingB{};
    // SetOccupantFundingPercentages / GetOccupantFundingPercentages (1.1.641.0:
    // 0x006F8290 / 0x006F8360): the boolean selects the purpose pair.
    // true = school staff + school bus, false = hospital staff + ambulance. Passing
    // false for a school reads absent hospital purposes (default 1).
    const bool isSchool = kind == CivicKind::School;
    if (residential->GetOccupantFundingPercentages(occupant, fundingA, fundingB, isSchool)) {
        record.fundingRawA = {fundingA.percentage,
                              "GetOccupantFundingPercentages/out1 (staff; flag=isSchool) static trace",
                              Evidence::StaticTrace};
        record.fundingRawB = {fundingB.percentage,
                              "GetOccupantFundingPercentages/out2 (bus/ambulance; flag=isSchool) static trace",
                              Evidence::StaticTrace};
    } else {
        record.fundingRawA = {std::nullopt, "GetOccupantFundingPercentages/out1",
                              Evidence::Unavailable};
        record.fundingRawB = {std::nullopt, "GetOccupantFundingPercentages/out2",
                              Evidence::Unavailable};
    }

    if (budget) {
        if (auto* properties = occupant->AsPropertyHolder()) {
            SC4Vector<cISC4BudgetSimulator::BudgetItem> items;
            if (budget->GetBudgetItemInfo(properties, items)) {
                for (const auto& item : items) {
                    record.budgetItems.push_back({item.departmentID, item.lineItem,
                                                  item.purpose, item.cost,
                                                  item.localFundingPercentage});
                }
            }
        }
    }

    // Building funding / bus / ambulance come from the budget items, not from
    // GetOccupantFundingPercentages (which returns 1/1 for schools funded at 31%/25%).
    const auto staffPurpose = kind == CivicKind::School ? kSchoolStaffPurpose : kHospitalStaffPurpose;
    const auto secondaryPurpose = kind == CivicKind::School ? kSchoolBusPurpose : kHospitalAmbulancePurpose;
    for (const auto& item : record.budgetItems) {
        if (item.purposeId == staffPurpose) {
            record.buildingFunding = {item.rawLocalFunding, "BudgetItem.localFundingPercentage (staff purpose)",
                                      Evidence::RuntimeVerified};
            // Rounded to nearest: 187*0.31=57.97->58, 187*0.25=46.75->47, 600*0.13=78, 600*0.34=204.
            record.monthlyCost = {RoundedFundingCost(item.rawCost, item.rawLocalFunding),
                                  "round(BudgetItem.cost * localFunding); building funding line only",
                                  Evidence::RuntimeVerified};
        } else if (item.purposeId == secondaryPurpose) {
            record.secondaryFunding = {item.rawLocalFunding, "BudgetItem.localFundingPercentage (bus/ambulance purpose)",
                                       Evidence::StaticTrace};
        }
    }
    return record;
}

void LogRecord(const CivicProbeRecord& record) {
    std::ostringstream out;
    out << "[CivicProbe][" << (record.kind == CivicKind::School ? "School" : "Hospital")
        << "] RuntimeAddress=0x" << std::hex << std::uppercase << record.runtimeAddress
        << std::dec << " (ephemeral, not a persistent ID)\n";
    WriteField(out, "BuildingTypeRaw", record.buildingTypeRaw);
    WriteField(out, "LotTagRaw", record.lotTagRaw);
    out << "KnownGroupMatches=";
    if (record.matchingKnownGroups.empty()) out << "none";
    for (auto group : record.matchingKnownGroups) out << " 0x" << std::hex << group << std::dec;
    out << " source=Occupant::IsOccupantGroup status=HEADER_CANDIDATE; full group list UNKNOWN\n";
    out << "Classification=" << (record.kind == CivicKind::School ? "School" : "Hospital")
        << " source=ResidentialSimulator membership status=VERIFIED_HEADER/NEEDS_UI\n";
    out << "ExemplarTGI=UNKNOWN source=no validated occupant TGI getter status=UNKNOWN\n";
    out << "ActiveState=UNKNOWN source=no validated active-state getter status=UNKNOWN\n";
    WriteField(out, "Usage", record.usage);
    WriteField(out, "Capacity", record.capacity);
    WriteField(out, "CapacityRawA", record.capacityRawA);
    WriteField(out, "CapacityRawB", record.capacityRawB);
    out << "CapacityCallReturned=" << (record.capacityCallReturned ? "true" : "false") << '\n';
    WriteField(out, "FundingRawA", record.fundingRawA);
    WriteField(out, "FundingRawB", record.fundingRawB);
    WriteField(out, "StaffCount", record.staffCount);
    WriteField(out, "BuildingFunding", record.buildingFunding);
    WriteField(out, "SecondaryFunding(bus/ambulance)", record.secondaryFunding);
    WriteField(out, "CurrentMonthlyCost", record.monthlyCost);
    out << "BudgetItemsRaw=" << record.budgetItems.size()
        << " source=BudgetSimulator::GetBudgetItemInfo; NOT current monthly cost\n";
    std::size_t shown = 0;
    for (const auto& item : record.budgetItems) {
        if (shown++ == 64) {
            out << "  further budget items omitted from log (limit 64)\n";
            break;
        }
        out << "  department=0x" << std::hex << item.departmentId
            << " line=0x" << item.lineId << " purpose=0x" << item.purposeId
            << std::dec << " costRaw=" << item.rawCost
            << " localFundingRaw=" << item.rawLocalFunding << '\n';
    }
    Log(LogLevel::Info, out.str());
}

} // namespace

CivicSnapshot ReadCivicSnapshot(cISC4City* city) {
    CivicSnapshot result{};
    if (!city) {
        result.errors++;
        Log(LogLevel::Error, "Civic probe skipped: city pointer unavailable");
        return result;
    }
    result.citySerial = city->GetCitySerialNumber();
    auto* residential = city->GetResidentialSimulator();
    if (!residential) {
        result.errors++;
        Log(LogLevel::Error, "Civic probe disabled for this city: ResidentialSimulator unavailable");
        return result;
    }
    // Read implementation addresses only while this city interface is live.
    // No runtime pointer is retained after this snapshot.
    {
        const auto* const vtable = *reinterpret_cast<void* const* const*>(residential);
        std::ostringstream addresses;
        addresses << "[CivicProbe][RE] ResidentialSimulator methods: "
                  << "GetSchoolQueryData=0x" << std::hex << reinterpret_cast<std::uintptr_t>(vtable[0x68 / 4])
                  << " GetHospitalQueryData=0x" << reinterpret_cast<std::uintptr_t>(vtable[0x6C / 4])
                  << " EstimateCurrentOccupantCapacity=0x" << reinterpret_cast<std::uintptr_t>(vtable[0x70 / 4]);
        Log(LogLevel::Info, addresses.str());
    }
    auto* budget = city->GetBudgetSimulator();
    if (!budget) Log(LogLevel::Warning, "BudgetSimulator unavailable; raw budget items omitted");

    {
        SC4List<cISC4Occupant*> schools;
        result.schoolsRead = residential->GetSchoolBuildings(schools);
        if (!result.schoolsRead) {
            result.errors++;
            Log(LogLevel::Warning, "GetSchoolBuildings failed");
        } else {
            for (auto* occupant : schools) {
                if (!occupant) { result.errors++; continue; }
                try {
                    result.schools.push_back(ReadRecord(CivicKind::School, occupant,
                                                        residential, budget));
                } catch (const std::exception& error) {
                    result.errors++;
                    Log(LogLevel::Warning, std::string("School record skipped: ") + error.what());
                }
            }
        }
    }
    {
        SC4List<cISC4Occupant*> hospitals;
        result.hospitalsRead = residential->GetHospitalBuildings(hospitals);
        if (!result.hospitalsRead) {
            result.errors++;
            Log(LogLevel::Warning, "GetHospitalBuildings failed");
        } else {
            for (auto* occupant : hospitals) {
                if (!occupant) { result.errors++; continue; }
                try {
                    result.hospitals.push_back(ReadRecord(CivicKind::Hospital, occupant,
                                                          residential, budget));
                } catch (const std::exception& error) {
                    result.errors++;
                    Log(LogLevel::Warning, std::string("Hospital record skipped: ") + error.what());
                }
            }
        }
    }
    return result;
}

void LogCivicSnapshot(const CivicSnapshot& snapshot, bool detailed) {
    std::ostringstream summary;
    summary << "[CivicProbe] CitySerial=" << snapshot.citySerial
            << " SchoolsFound=" << snapshot.schools.size()
            << " SchoolsRead=" << (snapshot.schoolsRead ? "YES" : "NO")
            << " HospitalsFound=" << snapshot.hospitals.size()
            << " HospitalsRead=" << (snapshot.hospitalsRead ? "YES" : "NO")
            << " Errors=" << snapshot.errors
            << " Usage/Capacity=VERIFIED_RUNTIME(school,clinic) CurrentMonthlyCost=VERIFIED_RUNTIME(building line)";
    Log(LogLevel::Info, summary.str());
    if (!detailed) {
        const auto logSamples = [](const char* kind, const std::vector<CivicProbeRecord>& records) {
            for (const auto& sample : records) {
                std::ostringstream line;
                line << "[CivicProbe][MonthlySample][" << kind << "] RuntimeAddress=0x"
                     << std::hex << std::uppercase << sample.runtimeAddress << std::dec;
                line << " Usage=";
                if (sample.usage.value) line << *sample.usage.value;
                else line << "UNKNOWN";
                line << " Capacity=";
                if (sample.capacity.value) line << *sample.capacity.value;
                else line << "UNKNOWN";
                line << " CapacityRawA=";
                if (sample.capacityRawA.value) line << *sample.capacityRawA.value;
                else line << "UNKNOWN";
                line << " CapacityRawB=";
                if (sample.capacityRawB.value) line << *sample.capacityRawB.value;
                else line << "UNKNOWN";
                line << " StaffCount=";
                if (sample.staffCount.value) line << *sample.staffCount.value;
                else line << "UNKNOWN";
                line << " BuildingFunding=";
                if (sample.buildingFunding.value) line << *sample.buildingFunding.value;
                else line << "UNKNOWN";
                line << " SecondaryFunding=";
                if (sample.secondaryFunding.value) line << *sample.secondaryFunding.value;
                else line << "UNKNOWN";
                line << " CurrentMonthlyCost=";
                if (sample.monthlyCost.value) line << *sample.monthlyCost.value;
                else line << "UNKNOWN";
                line << " FundingRawA=";
                if (sample.fundingRawA.value) line << *sample.fundingRawA.value;
                else line << "UNKNOWN";
                line << " FundingRawB=";
                if (sample.fundingRawB.value) line << *sample.fundingRawB.value;
                else line << "UNKNOWN";
                for (const auto& item : sample.budgetItems) {
                    line << " BudgetPurpose0x" << std::hex << std::uppercase << item.purposeId
                         << std::dec << "={costRaw:" << item.rawCost
                         << ",localFundingRaw:" << item.rawLocalFunding << '}';
                }
                Log(LogLevel::Info, line.str());
            }
        };
        logSamples("School", snapshot.schools);
        logSamples("Hospital", snapshot.hospitals);
        return;
    }
    for (const auto& school : snapshot.schools) LogRecord(school);
    for (const auto& hospital : snapshot.hospitals) LogRecord(hospital);
}

} // namespace sc4af
