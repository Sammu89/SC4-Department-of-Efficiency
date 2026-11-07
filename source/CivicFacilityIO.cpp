// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "CivicFacilityIO.h"

#include "Logging.h"
#include "QuerySemantics.h"
#include "NativeCivicQueryReader.h"

#include "SC4List.h"
#include "SC4Percentage.h"
#include "SC4Vector.h"
#include "cIGZString.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4BuildingOccupant.h"
#include "cISC4City.h"
#include "cISC4Occupant.h"
#include "cISC4ResidentialSimulator.h"
#include "cIGZVariant.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"
#include "cRZAutoRefCount.h"
#include "cS3DVector3.h"

#include <cmath>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace sc4af {
namespace {

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

std::string Fmt(float v) {
    std::ostringstream s;
    s << std::setprecision(9) << v;
    return s.str();
}

// Does the building carry a local staff funding line with a cost (the only thing the civic controller can fund)? nullopt: the budget cannot be read (the full read decides).
// A vanilla Private School has none (it is run privately): it is not a facility to fund and must stay out of the logic, readable or not.
std::optional<bool> HasLocalStaffLine(FundingFamily family, cISC4Occupant* occupant, cISC4BudgetSimulator* budget) {
    auto* holder = occupant->AsPropertyHolder();
    if (!budget || !holder) return std::nullopt;
    SC4Vector<cISC4BudgetSimulator::BudgetItem> items;
    if (!budget->GetBudgetItemInfo(holder, items)) return std::nullopt;
    const std::uint32_t staffPurpose = FundingSelector(family) ? purpose::SchoolStaff : purpose::HospitalStaff;
    for (const auto& item : items)
        if (item.purpose == staffPurpose && item.cost > 0) return true;
    return false;
}

const char* gLastFail = "";
#define FAIL(reason) do { gLastFail = reason; return false; } while (0)

bool ReadOne(FundingFamily family, cISC4Occupant* occupant, cISC4ResidentialSimulator* residential,
             cISC4BudgetSimulator* budget, bool specialSignalsVerified, LiveFacility& out) {
    (void)specialSignalsVerified;  // unused; kept for API stability
    const bool selector = FundingSelector(family);
    out = LiveFacility{};
    out.occupant = occupant;
    auto& s = out.state;
    auto& r = out.readback;
    s.family = family;

    const auto groups = ScanGroups(occupant);
    auto* holder = occupant->AsPropertyHolder();
    if (!holder) FAIL("no property holder");
    s.subtype = ClassifyCivic(groups).subtype;
    if (s.subtype.empty()) s.subtype = "unclassified";

    cRZAutoRefCount<cISC4BuildingOccupant> building;
    if (occupant->QueryInterface(GZIID_cISC4BuildingOccupant, building.AsPPVoid()))
        if (cIGZString* name = building->GetExemplarName()) out.exemplarName = name->ToChar() ? name->ToChar() : "";

    cS3DVector3 position(0.0f, 0.0f, 0.0f);
    if (!occupant->GetPosition(&position)) FAIL("GetPosition");
    s.posX = position.fX;
    s.posZ = position.fZ;
    s.hasPos = std::isfinite(position.fX) && std::isfinite(position.fZ);
    s.cellX = static_cast<int>(std::floor(position.fX / 16.0f));
    s.cellZ = static_cast<int>(std::floor(position.fZ / 16.0f));

    // Capacity: initialise the outputs with a sentinel and ignore the boolean result.
    std::uint32_t capA = 0xFFFFFFFF, capB = 0xFFFFFFFF;
    residential->EstimateCurrentOccupantCapacity(occupant, capA, capB);
    if (capA == 0xFFFFFFFFu) FAIL("capacity unset");
    s.capacity = capA;

    const auto raw = ReadQueryBufferRaw(residential, occupant, family == FundingFamily::School);
    if (!raw) FAIL("query buffer");
    s.baseCapacity = (*raw)[0];
    r.baseCapacity = s.baseCapacity;
    r.capacity = capA;
    std::memcpy(&r.bufferBuilding, &(*raw)[(0x2C - 0x14) / 4], sizeof(float));
    std::memcpy(&r.bufferSecondary, &(*raw)[(0x34 - 0x14) / 4], sizeof(float));
    const std::uint32_t bufferUsage = (*raw)[(0x24 - 0x14) / 4];

    // Usage is only trusted once the runtime SGProp holds a sample; 0xFFFFFFFF = never sampled.
    std::uint32_t sgUsage = 0xFFFFFFFF;
    holder->GetProperty(family == FundingFamily::School ? 0x09220161u : 0xC922015Cu, sgUsage);
    out.usageSampled = sgUsage != 0xFFFFFFFFu;
    if (out.usageSampled) s.usage = static_cast<std::int64_t>(bufferUsage);

    // The budget lines decide the writer shape.
    const std::uint32_t staffPurpose = selector ? purpose::SchoolStaff : purpose::HospitalStaff;
    const std::uint32_t secondaryPurpose = selector ? purpose::SchoolBus : purpose::HospitalAmbulance;
    bool haveStaff = false, haveSecondary = false;
    if (budget) {
        SC4Vector<cISC4BudgetSimulator::BudgetItem> items;
        if (budget->GetBudgetItemInfo(holder, items)) {
            for (const auto& item : items) {
                if (item.purpose == staffPurpose) {
                    r.budgetBuilding = item.localFundingPercentage;
                    r.baseCost = item.cost;
                    haveStaff = true;
                } else if (item.purpose == secondaryPurpose) {
                    r.budgetSecondary = item.localFundingPercentage;
                    haveSecondary = true;
                }
            }
        }
    }
    s.shape = ClassifyShape(family, haveStaff, haveSecondary);
    // Policy signal from the building's query layout, vanilla layouts only. An unreadable or custom
    // layout leaves vanillaLayout=false and the facility is reported as NON_VANILLA_MECHANISM.
    std::uint32_t layout = 0;
    if (holder->GetProperty(0x2A499F85u, layout) && layout != 0) s.queryLayout = layout;
    const auto* semantics = s.queryLayout ? LookupQueryLayout(s.queryLayout) : nullptr;
    s.vanillaLayout = semantics != nullptr;
    s.layoutMatchesFamily = LayoutMatchesFamily(semantics, family);
    s.kind = semantics ? PolicyKindForLayout(semantics) : PolicyKind::UnknownSignal;
    s.hasSecondary = haveSecondary;
    r.hasSecondary = haveSecondary;
    if (!haveStaff) return true;  // listed but without a local funding line: census only

    // Getter with the family selector. For a staff-only facility it returns false while still
    // filling out1 (staff) and leaving out2 at the fallback, so the boolean is not a validity flag.
    SC4Percentage a{}, b{};
    a.percentage = -1.0f;
    b.percentage = -1.0f;
    residential->GetOccupantFundingPercentages(occupant, a, b, selector);
    if (a.percentage < 0.0f) FAIL("funding getter produced no out1");
    r.getterBuilding = a.percentage;
    r.getterSecondary = b.percentage;
    s.buildingFunding = r.budgetBuilding;
    s.secondaryFunding = haveSecondary ? b.percentage : 0.0f;
    s.baseCost = r.baseCost;
    const auto coherence = AssessFundingCoherence(r);
    s.fundingCoherent = coherence.coherent;
    s.coherenceReason = coherence.reason;
    r.monthlyCost = RoundedFundingCost(r.baseCost, r.budgetBuilding);
    s.monthlyCost = r.monthlyCost;
    return true;
}

} // namespace

CivicEnumeration EnumerateFacilities(cISC4City* city, bool epochMatches) {
    CivicEnumeration result;
    auto& in = result.input;
    in.cityPresent = city != nullptr;
    in.epochMatches = epochMatches;
    if (!city) return result;
    auto* residential = city->GetResidentialSimulator();
    auto* budget = city->GetBudgetSimulator();
    in.residentialPresent = residential != nullptr;
    if (!residential) {
        result.validity = ClassifyEnumeration(in);
        return result;
    }
    const auto collect = [&](SC4List<cISC4Occupant*>& list, FundingFamily family, ListAcquisition& acq) {
        for (auto* occupant : list) {
            if (!occupant) continue;
            const bool foreign = !BelongsToList(family == FundingFamily::School, ClassifyCivic(ScanGroups(occupant)));
            if (foreign || HasLocalStaffLine(family, occupant, budget) == std::optional<bool>(false)) {
                ++in.notOfFamily;                     // e.g. a reward building in the hospital list, a private school: not a facility to fund
                std::string name = foreign ? "" : "no-local-funding:";
                cRZAutoRefCount<cISC4BuildingOccupant> building;
                if (occupant->QueryInterface(GZIID_cISC4BuildingOccupant, building.AsPPVoid()))
                    if (cIGZString* n = building->GetExemplarName()) name += n->ToChar() ? n->ToChar() : "?";
                if (result.notOfFamily.find(name) == std::string::npos) result.notOfFamily += (result.notOfFamily.empty() ? "" : ",") + name;
                continue;
            }
            ++acq.count;
            LiveFacility live;
            if (ReadOne(family, occupant, residential, budget, true, live)) {
                ++in.readOk;
                result.facilities.push_back(live);
            } else {
                ++in.readFailed;
                if (result.skipReasons.find(gLastFail) == std::string::npos) {
                    if (!result.skipReasons.empty()) result.skipReasons += "; ";
                    result.skipReasons += gLastFail;
                }
            }
        }
    };
    {
        SC4List<cISC4Occupant*> schools;
        in.schools.available = residential->GetSchoolBuildings(schools);
        if (in.schools.available) collect(schools, FundingFamily::School, in.schools);
    }
    {
        SC4List<cISC4Occupant*> hospitals;
        in.hospitals.available = residential->GetHospitalBuildings(hospitals);
        if (in.hospitals.available) collect(hospitals, FundingFamily::Health, in.hospitals);
    }
    result.validity = ClassifyEnumeration(in);
    return result;
}

std::vector<LiveFacility> ReadFacilities(cISC4City* city, bool specialSignalsVerified) {
    (void)specialSignalsVerified;
    return EnumerateFacilities(city, true).facilities;
}

bool ReadFacility(cISC4City* city, cISC4Occupant* occupant, FundingFamily family,
                  bool specialSignalsVerified, LiveFacility& out) {
    if (!city || !occupant) return false;
    auto* residential = city->GetResidentialSimulator();
    if (!residential) return false;
    return ReadOne(family, occupant, residential, city->GetBudgetSimulator(), specialSignalsVerified, out);
}

bool WriteBuildingFunding(cISC4City* city, const LiveFacility& target, float newBuildingFraction) {
    auto* residential = city ? city->GetResidentialSimulator() : nullptr;
    if (!residential || !target.occupant) return false;
    const SC4Percentage building{newBuildingFraction};
    // Live value from the getter (the vanilla fallback for staff-only facilities).
    const SC4Percentage secondary{target.readback.getterSecondary};
    return residential->SetOccupantFundingPercentages(target.occupant, building, secondary,
                                                      FundingSelector(target.state.family));
}

StrikeSignals ReadStrikeSignals(cISC4Occupant* occupant, FundingFamily family) {
    StrikeSignals out;
    auto* holder = occupant ? occupant->AsPropertyHolder() : nullptr;
    if (!holder) return out;
    const std::uint32_t coverageId = family == FundingFamily::School ? 0x292453C5u : 0x092453CBu;
    if (holder->HasProperty(coverageId)) {
        if (cISCProperty* property = holder->GetProperty(coverageId)) {
            if (cIGZVariant* value = property->GetPropertyValue()) {
                if (value->GetType() == cIGZVariant::Float32) out.coverage = value->GetValFloat32();
            }
        }
    }
    std::uint32_t since = 0;
    if (holder->GetProperty(0xEAA69F83u, since)) out.strikeSince = since;
    return out;
}

std::string DescribeFacility(const LiveFacility& f) {
    const auto& s = f.state;
    const auto& r = f.readback;
    std::ostringstream o;
    o << (s.family == FundingFamily::School ? "School" : "Health") << '/' << s.subtype << " cell=" << s.cellX << ','
      << s.cellZ << " shape=" << ShapeName(s.shape) << " signal=" << PolicyKindName(s.kind) << " layout=0x" << std::hex << s.queryLayout << std::dec << '('
      << [&]() -> std::string {
             const auto* l = LookupQueryLayout(s.queryLayout);
             return l ? std::string(QueryUnitName(l->usageUnit)) + '/' + QueryUnitName(l->capacityUnit) : "non-vanilla";
         }() << ')'
      << " funding(getter/buffer/budget)=" << Fmt(r.getterBuilding) << '/' << Fmt(r.bufferBuilding) << '/'
      << Fmt(r.budgetBuilding) << " secondary=";
    if (s.hasSecondary) o << Fmt(r.getterSecondary) << '/' << Fmt(r.bufferSecondary) << '/' << Fmt(r.budgetSecondary);
    else o << "none";
    o << " coherent=" << (s.fundingCoherent ? "YES" : "NO") << " baseCost=" << r.baseCost << " cost=" << r.monthlyCost << " capacity=" << r.capacity
      << " baseCapacity=" << r.baseCapacity << " usage=";
    if (s.usage) o << *s.usage; else o << "UNSAMPLED";
    return o.str();
}

} // namespace sc4af
