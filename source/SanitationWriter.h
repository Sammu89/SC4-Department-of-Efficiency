// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Game-facing executor of the Sanitation funding transaction (SanitationWriterPlan.h). Disabled in the product, like the
// Neighbor Deal executor: there is no production caller, and executing needs a research token that only exists when the
// translation unit defines SC4AF_ENABLE_SANITATION_WRITER_RESEARCH, which no build defines.
//
// The one vanilla call is cISC4DepartmentBudget::SetFundingPercentage(SC4Percentage{target}, lineItem = 0) on the garbage
// department 0x6A42A8F5 - the same call path that is validated for the Water department.

#include "SanitationWriterPlan.h"

#include <cstdint>

class cISC4City;
class cISC4RegionalCity;

namespace sc4af {

class SanitationWriterResearchToken {
public:
    SanitationWriterResearchToken(const SanitationWriterResearchToken&) = default;
    SanitationWriterResearchToken& operator=(const SanitationWriterResearchToken&) = default;

private:
    SanitationWriterResearchToken() = default;
    friend class SanitationWriterResearchSeam;
    friend class FinalValidationExecutorSeam;   // FinalValidationSeam.h, validation builds only
};

#ifdef SC4AF_ENABLE_SANITATION_WRITER_RESEARCH
class SanitationWriterResearchSeam {
public:
    static SanitationWriterResearchToken Arm() { return SanitationWriterResearchToken(); }
};
#endif

class SanitationFundingTransactor {
public:
    // Read-only snapshot of the garbage department and the delayed efficiency / capacity readbacks.
    static SanitationObservation Observe(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t lifecycleEpoch);

    // One host iteration: Observe -> machine.Next -> at most ONE vanilla write (after a fresh compare-and-set against the
    // department itself) -> machine.Written(). Call it again on later snapshots/events until the machine is terminal.
    static SanitationStep Pump(const SanitationWriterResearchToken& token, SanitationWriteMachine& machine, cISC4City* city,
                               cISC4RegionalCity* regional, std::uint32_t lifecycleEpoch);
};

} // namespace sc4af
