// Generated from the vanilla SC4 UI query layouts. Do not edit by hand.
#pragma once

#include <cstdint>

namespace sc4af {

enum class QueryUnit : std::uint8_t { None, Students, Patients, Books, Visits, Patrons };

struct QueryLayoutSemantics {
    std::uint32_t layout;       // occupant property 0x2A499F85
    QueryUnit usageUnit;        // unit of the control served by the usage handlers (0x101C/0x101E/0x1025)
    QueryUnit capacityUnit;     // unit of the control served by the capacity handlers (0x101B/0x101D/0x101F/0x1024)
    bool hasStaffSlider;        // building funding slider (0x1018 / 0x1021)
    bool hasSecondarySlider;    // bus / ambulance slider (0x1019 / 0x1022)
};

inline constexpr QueryLayoutSemantics kQueryLayouts[] = {
    {0x2A564884, QueryUnit::Students, QueryUnit::Students, true, false},  // CV44x44_CityCollege_031C
    {0x2A8B7E1C, QueryUnit::Students, QueryUnit::Students, true, false},  // RW44x28_UnivAdminTower_033D
    {0x4A8B7FE7, QueryUnit::Books, QueryUnit::Books, true, false},  // RW30x44_MainCityLibrary_031B
    {0x6A555A84, QueryUnit::Visits, QueryUnit::Visits, true, false},  // CV24x22_ScienceHistoryArtMuseum_031D
    {0x8A554483, QueryUnit::Students, QueryUnit::Students, true, true},  // CV28x28_K-8SmallSchool_0314
    {0xAA554AEA, QueryUnit::Patients, QueryUnit::Patients, true, true},  // CV12x14_urgentcareclinic_0311
    {0xAA565036, QueryUnit::Books, QueryUnit::Books, true, false},  // CV10x24_SmallLibrary_0319
    {0xCA8B8408, QueryUnit::Patients, QueryUnit::Patients, true, false},  // RW36x50_DiseaseCtr_033F
    {0xCA8B8564, QueryUnit::Students, QueryUnit::Students, false, false},  // RW24x30_PrivateSchool2_0379
    {0xEA8B82DB, QueryUnit::Visits, QueryUnit::Visits, true, false},  // RW42x38_MajorArtMuseum_033C
};

inline const QueryLayoutSemantics* FindQueryLayout(std::uint32_t layout) {
    for (const auto& entry : kQueryLayouts)
        if (entry.layout == layout) return &entry;
    return nullptr;
}

} // namespace sc4af
