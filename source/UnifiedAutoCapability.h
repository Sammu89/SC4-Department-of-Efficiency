// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "BuildFeatures.h"

// The compile-time half of the unified controller's capability check. Without it the controller (the only caller of the Deal and
// Sanitation executors) cannot even be constructed: the seam below exists only when the translation unit is compiled with
// SC4AF_ENABLE_UNIFIED_AUTO (cmake options SC4_UNIFIED_AUTO / SC4_FINAL_AUTO_VALIDATION). The runtime half is the INI switches
// documented in UtilityConfig.h (e.g. `EnableUnifiedAuto`).
//
// This is deliberately separate from the low-level macros of NeighborDealWriter.h / SanitationWriter.h, which only cover the
// isolated diagnostic writers.

namespace sc4af {

class UnifiedAutoValidationCapability {
public:
#ifdef SC4AF_ENABLE_UNIFIED_AUTO
    static constexpr bool kBuildEnabled = true;
#else
    static constexpr bool kBuildEnabled = false;
#endif
    UnifiedAutoValidationCapability(const UnifiedAutoValidationCapability&) = default;
    UnifiedAutoValidationCapability& operator=(const UnifiedAutoValidationCapability&) = default;

private:
    UnifiedAutoValidationCapability() = default;
    friend class UnifiedAutoValidationSeam;
};

#ifdef SC4AF_ENABLE_UNIFIED_AUTO
class UnifiedAutoValidationSeam {
public:
    static UnifiedAutoValidationCapability Arm() { return UnifiedAutoValidationCapability(); }
};
#endif

} // namespace sc4af
