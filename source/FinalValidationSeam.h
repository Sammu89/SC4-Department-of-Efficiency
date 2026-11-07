// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "BuildFeatures.h"

// The ONLY bridge from the unified controller to the low-level Deal / Sanitation executors. Everything here exists only when
// SC4AF_ENABLE_UNIFIED_AUTO is defined; otherwise this header is empty and the executors' tokens (NeighborDealWriter.h /
// SanitationWriter.h) stay unreachable from the rest of the code.

#ifdef SC4AF_ENABLE_UNIFIED_AUTO

#include "NeighborDealWriter.h"
#include "SanitationWriter.h"
#include "UnifiedAutoCapability.h"

namespace sc4af {

class FinalValidationExecutorSeam {
public:
    static DealWriterResearchToken DealToken(const UnifiedAutoValidationCapability&) { return DealWriterResearchToken(); }
    static SanitationWriterResearchToken SanitationToken(const UnifiedAutoValidationCapability&) { return SanitationWriterResearchToken(); }
};

} // namespace sc4af

#endif
