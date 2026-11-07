// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "NativeCivicQueryReader.h"

#include "SC4String.h"
#include "cISC4ResidentialSimulator.h"

#include <array>
#include <cstddef>
#include <cstring>

namespace sc4af {
namespace {

// In 1.1.641.0, vanilla's 0x101C and 0x1025 handlers construct a cRZString
// at offset 0, pass a stack buffer to the simulator, then read a dword at
// offset 0x24. The simulator implementations at 0x006F91C0 and 0x006F7E80
// only access offsets 0x14..0x38; the vanilla stack leaves 0x40 bytes from
// the buffer start to the pre-call stack top. SC4String is the SDK's ABI-
// compatible cRZString, including constructor and destructor.
struct CivicQueryBuffer641 {
    SC4String text;
    std::array<std::byte, 0x2C> fields{};
};
static_assert(sizeof(SC4String) == 0x14);
static_assert(sizeof(CivicQueryBuffer641) == 0x40);

constexpr std::size_t kUsageOffset = 0x24;

std::optional<std::uint32_t> ReadUsage(cISC4ResidentialSimulator* residential,
                                       cISC4Occupant* occupant, bool school) {
    if (!residential || !occupant) return std::nullopt;
    CivicQueryBuffer641 buffer{};
    const auto address = reinterpret_cast<intptr_t>(&buffer);
    const bool ok = school ? residential->GetSchoolQueryData(occupant, address)
                           : residential->GetHospitalQueryData(occupant, address);
    if (!ok) return std::nullopt;
    std::uint32_t value = 0;
    std::memcpy(&value, reinterpret_cast<const std::byte*>(&buffer) + kUsageOffset,
                sizeof(value));
    // Vanilla normalizes this stored "not sampled yet" sentinel to zero in
    // GetSchool/HospitalQueryData, but keep an explicit guard at our boundary.
    if (value == 0xFFFFFFFFu) return std::nullopt;
    return value;
}

} // namespace

std::optional<std::uint32_t> ReadSchoolStudentCount(
    cISC4ResidentialSimulator* residential, cISC4Occupant* occupant) {
    return ReadUsage(residential, occupant, true);
}

std::optional<std::uint32_t> ReadHospitalPatientCount(
    cISC4ResidentialSimulator* residential, cISC4Occupant* occupant) {
    return ReadUsage(residential, occupant, false);
}

std::optional<std::array<std::uint32_t, 11>> ReadQueryBufferRaw(
    cISC4ResidentialSimulator* residential, cISC4Occupant* occupant, bool school) {
    if (!residential || !occupant) return std::nullopt;
    CivicQueryBuffer641 buffer{};
    const auto address = reinterpret_cast<intptr_t>(&buffer);
    const bool ok = school ? residential->GetSchoolQueryData(occupant, address)
                           : residential->GetHospitalQueryData(occupant, address);
    if (!ok) return std::nullopt;
    std::array<std::uint32_t, 11> out{};
    std::memcpy(out.data(), reinterpret_cast<const std::byte*>(&buffer) + 0x14, sizeof(out));
    return out;
}

} // namespace sc4af
