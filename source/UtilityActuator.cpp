// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "UtilityActuator.h"

#include "UtilityHealth.h"
#include "UtilityOptimizer.h"
#include "UtilityProbe.h"

#include "SC4Percentage.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4City.h"
#include "cISC4DepartmentBudget.h"
#include "cISC4Occupant.h"
#include "cISC4PowerSimulator.h"
#include "cISC4RegionalCity.h"
#include "cS3DVector3.h"

#include <cmath>
#include <cstdlib>

namespace sc4af {
namespace {

const UtilityTuning& Tuning() {
    static const UtilityTuning t = VanillaUtilityTuning();
    return t;
}

cISC4Occupant* FindPlantAt(cISC4PowerSimulator* sim, int cellX, int cellZ, int& matches) {
    matches = 0;
    cISC4Occupant* found = nullptr;
    ilist<cISC4Occupant*> plants;
    sim->GetPowerPlants(plants);
    for (auto* occ : plants) {
        if (!occ) continue;
        cS3DVector3 position(0.0f, 0.0f, 0.0f);
        if (!occ->GetPosition(&position)) continue;
        if (static_cast<int>(std::floor(position.fX / 16.0f)) == cellX && static_cast<int>(std::floor(position.fZ / 16.0f)) == cellZ) {
            found = occ;
            ++matches;
        }
    }
    return found;
}

UtilityLimits LimitsFor(const UtilityResearchConfig& c, bool power) {
    UtilityLimits l;
    l.reserveFactor = power ? c.powerReserve : c.waterReserve;
    if (!power) l.minPercent = WaterLegalMinPercent(c.protectWaterPipes, c.waterMinPercent);
    l.maxStepPercent = 100;   // policy: no step throttle; the optimizer chose the target, this layer only checks preconditions
    return l;
}

} // namespace

bool GameUtilityActuator::Reread(UtilitySnapshot& out) {
    if (!city_) return false;
    out = ReadUtilitySnapshot(city_, regional_, epoch_, true, false);
    return true;
}

ActuatorResult GameUtilityActuator::WritePlant(int cellX, int cellZ, int fromPercent, int toPercent, UtilitySnapshot& after) {
    ActuatorResult res;
    if (!city_) { res.result = UtilityWriteResult::NotAvailable; return res; }
    const UtilitySnapshot fresh = ReadUtilitySnapshot(city_, regional_, epoch_, true, false);
    auto exports = ContractedExportsOf(fresh.deals.deals, fresh.power.gameExported, std::nullopt, std::nullopt).power;
    std::int64_t importsCredit = 0;
    if (config_.countImports) { importsCredit = ImportCredit(fresh.deals, utility_id::kDealBuyPower, {fresh.simYear, fresh.simDayOfYear}, config_.importSafeDays, nullptr); exports -= importsCredit; }   // dependable imports, same rule as the optimizer
    const PowerWriteRequest req{cellX, cellZ, fromPercent, toPercent};
    auto powerLimits = LimitsFor(config_, true);
    powerLimits.imports = static_cast<double>(importsCredit);
    const auto plan = PlanPowerPlantFunding(fresh.power, req, Tuning().powerEfficiency, powerLimits, exports);
    if (!plan.write) { res.result = plan.result; res.detail = plan.reason; return res; }

    auto* sim = city_->GetPowerSimulator();
    int matches = 0;
    auto* plant = sim ? FindPlantAt(sim, cellX, cellZ, matches) : nullptr;
    if (!plant) { res.result = UtilityWriteResult::TargetMissing; return res; }
    if (matches > 1) { res.result = UtilityWriteResult::TargetAmbiguous; return res; }
    SC4Percentage current{};
    current.percentage = -1.0f;
    if (!sim->GetPowerPlantFundingPercentage(plant, current) || !PercentMatches(current.percentage, fromPercent)) {
        res.result = UtilityWriteResult::PreconditionChanged;
        res.detail = "plant funding differs from the planned input";
        return res;
    }
    const SC4Percentage target{plan.targetFraction};
    sim->SetPowerPlantFundingPercentage(plant, target);

    after = ReadUtilitySnapshot(city_, regional_, epoch_, true, false);
    FundingReadback rb;
    for (const auto& row : after.power.plants) {
        if (row.ref.cellX != cellX || row.ref.cellZ != cellZ) continue;
        rb.getter = row.funding;
        if (row.budgetLine) rb.budgetLine = row.budgetLine->localFunding;
    }
    const auto verdict = VerifyFundingReadback(rb, plan.targetFraction, true);
    if (!verdict.pass) { res.result = UtilityWriteResult::VerificationFailed; res.detail = verdict.reason; return res; }
    res.result = UtilityWriteResult::Applied;
    return res;
}

ActuatorResult GameUtilityActuator::WriteWater(int fromPercent, int toPercent, UtilitySnapshot& after) {
    ActuatorResult res;
    if (!city_) { res.result = UtilityWriteResult::NotAvailable; return res; }
    const UtilitySnapshot fresh = ReadUtilitySnapshot(city_, regional_, epoch_, true, false);
    auto exports = ContractedExportsOf(fresh.deals.deals, std::nullopt, fresh.water.gameExported, std::nullopt).water;
    std::int64_t importsCredit = 0;
    if (config_.countImports) { importsCredit = ImportCredit(fresh.deals, utility_id::kDealBuyWater, {fresh.simYear, fresh.simDayOfYear}, config_.waterImportSafeDays, nullptr); exports -= importsCredit; }
    const WaterWriteRequest req{fromPercent, toPercent};
    auto waterLimits = LimitsFor(config_, false);
    waterLimits.imports = static_cast<double>(importsCredit);
    const auto plan = PlanWaterFunding(fresh.water, req, Tuning().waterEfficiency, waterLimits, exports);
    if (!plan.write) { res.result = plan.result; res.detail = plan.reason; return res; }

    auto* budget = city_->GetBudgetSimulator();
    auto* dept = budget ? budget->GetDepartmentBudget(utility_id::kWaterDepartment) : nullptr;
    if (!dept) { res.result = UtilityWriteResult::NotAvailable; return res; }
    if (!dept->GetFundingPercentage() || !PercentMatches(dept->GetFundingPercentage()->percentage, fromPercent)) {
        res.result = UtilityWriteResult::PreconditionChanged;
        res.detail = "department funding differs from the planned input";
        return res;
    }
    const SC4Percentage target{plan.targetFraction};
    dept->SetFundingPercentage(target, 0);   // the return value is undefined by the game

    after = ReadUtilitySnapshot(city_, regional_, epoch_, true, false);
    FundingReadback rb;
    rb.getter = after.water.department.funding;
    const auto verdict = VerifyFundingReadback(rb, plan.targetFraction, false);
    if (!verdict.pass) { res.result = UtilityWriteResult::VerificationFailed; res.detail = verdict.reason; return res; }
    res.result = UtilityWriteResult::Applied;
    return res;
}

} // namespace sc4af
