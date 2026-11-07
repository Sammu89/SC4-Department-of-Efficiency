// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// The game's own neighbor-deal notices are silenced by DepartmentEfficiencyDealAdvice.dat ONLY while the plugin manages the deals itself.
// If the plugin does not manage the deals (AutoDeals=false, Mode != Auto, plugin switched off), the notices belong to the game again,
// so that DAT must not be present. The game reads its DATs at start and keeps them open, so the plugin cannot switch the file at run time: it judges the pair
// (configuration, file) and reports the verdict in the log. Pure decision here; the file test is done by Plugin.cpp.
#pragma once

#include "UtilityConfig.h"

namespace sc4af {

inline bool DealNoticesShouldBeSilenced(const UtilityResearchConfig& c) {
    return UnifiedGateOpenRuntime(c) && c.autoDeals && (c.tradePower || c.tradeWater || c.tradeGarbage);   // with all three resources in manual mode nobody manages a deal, so the game's notices stay
}

enum class DealNoticeVerdict { Consistent, RemoveDat, DatMissing };

inline DealNoticeVerdict JudgeDealNoticeDat(bool wantSilenced, bool datPresent) {
    if (wantSilenced) return datPresent ? DealNoticeVerdict::Consistent : DealNoticeVerdict::DatMissing;
    return datPresent ? DealNoticeVerdict::RemoveDat : DealNoticeVerdict::Consistent;
}

inline const char* DealNoticeVerdictText(DealNoticeVerdict v) {
    switch (v) {
        case DealNoticeVerdict::RemoveDat: return "the plugin does not manage the neighbor deals but DepartmentEfficiencyDealAdvice.dat is installed: the game's deal notices stay silenced; delete that file (the game must be closed) to get them back";
        case DealNoticeVerdict::DatMissing: return "the plugin manages the neighbor deals but DepartmentEfficiencyDealAdvice.dat is not installed: the game's own deal notices will still appear (build it with tools/deal_advice_patch.py)";
        default: return "the game's neighbor-deal notices follow the configuration (silenced only while the plugin manages the deals)";
    }
}

}  // namespace sc4af
