// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "UtilityShadow.h"

#include "Logging.h"
#include "UnifiedPlan.h"
#include "UtilityAuto.h"
#include "UtilityOptimizer.h"

namespace sc4af {

void UtilityShadow::OnPowerCycle(const UtilitySnapshot& snapshot, const UtilityResearchConfig& config, const SubsystemPlan& plan) {
    if (config.mode == UtilityMode::Off || !config.power) return;
    Log(LogLevel::Info, "[UtilityOptimizer] PowerCycle date=" + std::to_string(snapshot.simYear) + '/' + std::to_string(snapshot.simDayOfYear) +
                            (config.mode == UtilityMode::Shadow ? " NO WRITES" : " AUTO (executed changes are logged as [UtilityAuto])"));
    Log(LogLevel::Info, FormatSubsystem("Power", plan));
    Log(LogLevel::Info, FormatPowerTarget(plan));
    Log(LogLevel::Info, FormatTransition("Power", SummarizePowerTransition(snapshot, plan)));
    for (const auto& c : plan.cards) Log(LogLevel::Info, "[DealCard] " + FormatDealCard(c, config.debug));
}

void UtilityShadow::OnMonth(const UtilitySnapshot& snapshot, const UtilityResearchConfig& config) {
    if (config.mode == UtilityMode::Off) return;
    ++months_;
    const OptimizerConfig cfg = MakeOptimizerConfig(config);
    const UtilityTuning tuning = VanillaUtilityTuning();
    UtilityPlan plan = PlanUtilities(snapshot, cfg, tuning);
    if (!config.water) plan.water = SubsystemPlan{};

    Log(LogLevel::Info, "[UtilityOptimizer] Shadow month=" + std::to_string(months_) + " date=" + std::to_string(snapshot.simYear) + '/' +
                            std::to_string(snapshot.simDayOfYear) + " epoch=" + std::to_string(snapshot.lifecycleEpoch) +
                            (config.mode == UtilityMode::Shadow ? " NO WRITES; plan is replanned from the real state every month"
                                                                                                 : " AUTO: this is the optimizer plan; executed changes are logged as [UtilityAuto]"));
    if (config.water) {
        Log(LogLevel::Info, FormatSubsystem("Water", plan.water));
        Log(LogLevel::Info, FormatTransition("Water", SummarizeTransition(plan.water)));
        for (const auto& c : plan.water.cards) Log(LogLevel::Info, "[DealCard] " + FormatDealCard(c, config.debug));
    }
    Log(LogLevel::Info, FormatGarbage(plan));
    Log(LogLevel::Info, FormatGarbageHandling(plan, cfg.garbageReserve));
    Log(LogLevel::Info, FormatGarbageFunding(plan));
    Log(LogLevel::Info, FormatDeals(plan, snapshot.deals.opportunities.size()));
    {
        // The whole-system plan: exactly the pass the unified Auto controller executes from. Shadow stops before execution.
        UnifiedOptions uo;
        uo.includeDeals = DealDiscoveryAllowed(config);   // without the proposals the portfolio cannot be evaluated: the plan then uses the active contracts only (as the unified Auto does)
        uo.dealScope.commodity[0] = config.tradePower; uo.dealScope.commodity[1] = config.tradeWater; uo.dealScope.commodity[2] = config.tradeGarbage;   // a resource whose contracts the player manages by hand is not planned (its contracts are counted as they are)
        const UnifiedPlan unified = BuildUnifiedPlan(snapshot, cfg, tuning, uo);
        Log(LogLevel::Info, "[UtilityOptimizer][Unified] READ_ONLY (Shadow: same planning pass as unified Auto, no execution)");
        for (const auto& l : FormatUnifiedPlan(unified)) Log(LogLevel::Info, l);
        for (const auto& l : DescribePowerEconomics(snapshot, cfg, tuning, unified)) Log(LogLevel::Info, l);
        for (const auto& l : DescribeGarbageDecision(snapshot, cfg, tuning, unified, 0).extra) Log(LogLevel::Info, l);
        for (const auto& c : unified.portfolio.commodities)
            Log(LogLevel::Info, std::string("[UtilityOptimizer][Portfolio][") + CommodityLabel(c.commodity) + "] evaluated=" + (c.evaluated ? "yes" : "no") + " sells=" +
                                    std::to_string(c.sellTotal) + " buys=" + std::to_string(c.buyTotal) + " baselineNet=" + std::to_string(c.baselineNet) + " keepNet=" +
                                    (c.keepFeasible ? std::to_string(c.keepNet) : std::string("N/A(current portfolio infeasible)")) + " optimumNet=" + std::to_string(c.optimumNet) + " mode=" + c.mode + (c.note.empty() ? "" : " note=\"" + c.note + "\"") +
                                    (c.arbitrage.empty() ? "" : " arbitrage=\"" + c.arbitrage + "\""));
        if (config.debug)
            for (const auto& n : unified.portfolio.notes) Log(LogLevel::Info, "[UtilityOptimizer][Portfolio][Debug] " + n);
    }
}

} // namespace sc4af
