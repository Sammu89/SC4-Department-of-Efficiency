// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <array>
#include <cstdint>
#include <optional>

class cISC4Occupant;
class cISC4ResidentialSimulator;

namespace sc4af {

// Read-only reproduction of the data-buffer setup used by vanilla civic
// query helpers in the Steam 1.1.641.0 x86 executable.
std::optional<std::uint32_t> ReadSchoolStudentCount(
    cISC4ResidentialSimulator* residential, cISC4Occupant* occupant);
std::optional<std::uint32_t> ReadHospitalPatientCount(
    cISC4ResidentialSimulator* residential, cISC4Occupant* occupant);

// Raw dwords at buffer offsets 0x14..0x3C (11 dwords) as filled by
// GetSchoolQueryData / GetHospitalQueryData. Read-only diagnostic; the meaning
// of most fields is not fully mapped.
std::optional<std::array<std::uint32_t, 11>> ReadQueryBufferRaw(
    cISC4ResidentialSimulator* residential, cISC4Occupant* occupant, bool school);

} // namespace sc4af
