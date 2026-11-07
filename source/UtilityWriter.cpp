// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "UtilityWriter.h"

#include "Logging.h"
#include "UtilityHealth.h"
#include "UtilityProbe.h"

#include "SC4Percentage.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4City.h"
#include "cISC4DepartmentBudget.h"
#include "cISC4Occupant.h"
#include "cISC4PowerSimulator.h"
#include "cISC4RegionalCity.h"
#include "cS3DVector3.h"

#include <windows.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace sc4af {
namespace {

// Vanilla funding->efficiency tables feed only the SAFETY RESERVE estimate; the game's own efficiency getter
// is logged next to the model curve for comparison. See UtilityHealth.h.
const UtilityTuning& Tuning() {
    static const UtilityTuning t = VanillaUtilityTuning();
    return t;
}

std::uint64_t FileWriteTime(const std::string& path) {
    WIN32_FILE_ATTRIBUTE_DATA data{};
    if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &data)) return 0;
    return (static_cast<std::uint64_t>(data.ftLastWriteTime.dwHighDateTime) << 32) | data.ftLastWriteTime.dwLowDateTime;
}

template <class T> std::string Num(const Opt<T>& v) {
    if (!v) return "?";
    std::ostringstream s;
    s << std::setprecision(9) << +*v;
    return s.str();
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

const PowerPlantRow* PlantRow(const PowerState& s, int x, int z) {
    for (const auto& p : s.plants)
        if (p.ref.cellX == x && p.ref.cellZ == z) return &p;
    return nullptr;
}

std::string PlantMetrics(const PowerPlantRow* p) {
    if (!p) return "plant=MISSING";
    std::ostringstream s;
    s << "plant funding=" << Num(p->funding) << " line=" << (p->budgetLine ? Num(std::optional<float>(p->budgetLine->localFunding)) : "?")
      << " nominalCost=" << (p->budgetLine ? std::to_string(p->budgetLine->cost) : "?") << " costGetter=" << Num(p->monthlyCost)
      << " eff=" << Num(p->efficiencyPercent) << " cond=" << Num(p->conditionPercent) << " max=" << Num(p->maxProduced)
      << " produced=" << Num(p->produced) << " used=" << Num(p->used) << " baseAging=" << Num(p->baseAgingRate);
    return s.str();
}

std::string PowerAggregateText(const PowerState& s) {
    std::ostringstream o;
    o << "city demand=" << Num(s.aggregate.demand) << " consumption=" << Num(s.aggregate.consumption) << " import=" << Num(s.aggregate.import_)
      << " production=" << Num(s.aggregate.production) << " unpowered=" << Num(s.aggregate.unpowered)
      << " | powerDept funding=" << Num(s.department.funding) << " ideal=" << Num(s.department.idealMonthly) << " expenses=" << Num(s.department.totalExpenses)
      << " spending=" << Num(s.department.totalSpending);
    return o.str();
}

std::string WaterMetrics(const WaterState& w) {
    std::ostringstream s;
    s << "waterDept funding=" << Num(w.department.funding) << " ideal=" << Num(w.department.idealMonthly) << " expenses=" << Num(w.department.totalExpenses)
      << " spending=" << Num(w.department.totalSpending) << " capacity=" << Num(w.aggregate.productionCapacity) << " demand=" << Num(w.aggregate.demand)
      << " consumed=" << Num(w.aggregate.consumed) << " import=" << Num(w.aggregate.imported) << " export=" << Num(w.aggregate.exported)
      << " gameExported=" << Num(w.gameExported) << " watered=" << Num(w.aggregate.wateredBuildings) << " unwatered=" << Num(w.aggregate.unwateredBuildings)
      << " pipes=" << Num(w.aggregate.pipes) << " distressed=" << Num(w.aggregate.distressedPipes) << " burst=" << Num(w.aggregate.burstPipes);
    for (const auto& p : w.producers)
        s << "\n  producer cell=" << p.ref.cellX << ',' << p.ref.cellZ << " cur=" << Num(p.currentCapacity) << " max=" << Num(p.maxCapacity)
          << " used=" << Num(p.waterUsed) << " eff=" << Num(p.efficiencyPercent) << " cond=" << Num(p.conditionPercent)
          << " shutDownByPollution=" << Num(p.shutDownByPollution) << " upkeep=" << Num(p.upkeep);
    return s.str();
}

// Sell/export contracts read from the snapshot: they consume the same local capacity.
std::int64_t ExportsOf(const UtilitySnapshot& s, bool power) {
    const auto c = ContractedExportsOf(s.deals.deals, s.power.gameExported, s.water.gameExported, std::nullopt);
    return power ? c.power : c.water;
}

} // namespace

void UtilityWriter::ResetCity() {
    monthsInCity_ = 0;
    if (anyWatch()) {
        ++reloadsSinceApply_;
        monthsSinceReload_ = 0;
        Log(LogLevel::Info, "[UtilityWriter][Reload] city lifecycle changed while watching nonce=" + appliedNonce_ +
                                " reloadsSinceApply=" + std::to_string(reloadsSinceApply_) + "; the next months verify persistence");
    }
}

void UtilityWriter::LoadConfigIfChanged() {
    if (configPath_.empty()) return;
    const auto stamp = FileWriteTime(configPath_);
    if (stamp == lastWriteTime_) return;
    lastWriteTime_ = stamp;
    config_ = UtilityWriterConfig{};
    if (stamp == 0) return;  // no file: disarmed, silent
    std::ifstream in(configPath_, std::ios::binary);
    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string error;
    if (!ParseUtilityWriterConfig(buffer.str(), config_, error)) {
        config_ = UtilityWriterConfig{};
        Log(LogLevel::Warning, "[UtilityWriter] config invalid (" + error + "); disarmed");
        return;
    }
    Log(LogLevel::Info, std::string("[UtilityWriter] config loaded enabled=") + (config_.enabled ? "true" : "false") + " nonce=" + config_.nonce);
}

bool UtilityWriter::AlreadyApplied(const std::string& nonce) {
    std::ifstream in(appliedPath_);
    std::string line;
    while (std::getline(in, line)) {
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ')) line.pop_back();
        if (line == nonce) return true;
    }
    return false;
}

void UtilityWriter::MarkApplied(const std::string& nonce) {
    std::ofstream out(appliedPath_, std::ios::app);
    out << nonce << '\n';
}

void UtilityWriter::Observe(const UtilitySnapshot& s) {
    if (!anyWatch()) return;
    ++monthsSinceReload_;
    std::ostringstream o;
    o << "[UtilityWriter][Observe] nonce=" << appliedNonce_ << " month=" << monthsInCity_ << " date=" << s.simYear << '/' << s.simDayOfYear
      << " reloads=" << reloadsSinceApply_ << " monthsSinceReload=" << monthsSinceReload_ << ' ';
    if (power_.active) {
        const auto* row = PlantRow(s.power, power_.cellX, power_.cellZ);
        o << PlantMetrics(row) << " | " << PowerAggregateText(s.power);
        if (!row) o << " | MISSING plant";
        else if (row->funding && !FractionNear(*row->funding, power_.fraction)) o << " | DRIFT power expected=" << power_.fraction;
    }
    if (water_.active) {
        if (power_.active) o << "\n  ";
        o << WaterMetrics(s.water);
        if (s.water.department.funding && !FractionNear(*s.water.department.funding, water_.fraction)) o << " | DRIFT water expected=" << water_.fraction;
    }
    Log(LogLevel::Info, o.str());
}

UtilityWriteResult UtilityWriter::ApplyPower(cISC4City* city, cISC4RegionalCity* regional, const UtilitySnapshot& snapshot) {
    const UtilityLimits limits;
    const PowerWriteRequest req{config_.cellX, config_.cellZ, config_.expectedPercent, config_.targetPercent};
    const auto plan = PlanPowerPlantFunding(snapshot.power, req, Tuning().powerEfficiency, limits, ExportsOf(snapshot, true));
    Log(LogLevel::Info, "[UtilityWriter][Plan] Power cell=" + std::to_string(req.cellX) + ',' + std::to_string(req.cellZ) + " " +
                            std::to_string(req.expectedPercent) + "->" + std::to_string(req.targetPercent) + " result=" +
                            UtilityWriteResultName(plan.result) + " (" + plan.reason + ")");
    Log(LogLevel::Info, "[UtilityWriter][Before] " + PlantMetrics(PlantRow(snapshot.power, req.cellX, req.cellZ)) + " | " + PowerAggregateText(snapshot.power));
    if (!plan.write) return plan.result;

    auto* sim = city->GetPowerSimulator();
    int matches = 0;
    auto* plant = sim ? FindPlantAt(sim, req.cellX, req.cellZ, matches) : nullptr;
    if (!plant) return UtilityWriteResult::TargetMissing;
    if (matches > 1) return UtilityWriteResult::TargetAmbiguous;
    SC4Percentage fresh{};
    fresh.percentage = -1.0f;
    if (!sim->GetPowerPlantFundingPercentage(plant, fresh) || !FractionNear(fresh.percentage, UtilityFractionForPercent(req.expectedPercent)))
        return UtilityWriteResult::PreconditionChanged;
    const SC4Percentage target{plan.targetFraction};
    const bool ret = sim->SetPowerPlantFundingPercentage(plant, target);
    Log(LogLevel::Info, std::string("[UtilityWriter][Write] SetPowerPlantFundingPercentage returned ") + (ret ? "true" : "false"));

    const auto after = ReadUtilitySnapshot(city, regional, snapshot.lifecycleEpoch, true, false);
    FundingReadback rb;
    const auto* row = PlantRow(after.power, req.cellX, req.cellZ);
    if (row) {
        rb.getter = row->funding;
        if (row->budgetLine) rb.budgetLine = row->budgetLine->localFunding;
    }
    Log(LogLevel::Info, "[UtilityWriter][After] " + PlantMetrics(row) + " | " + PowerAggregateText(after.power));
    if (row && row->efficiencyPercent && row->conditionPercent) {
        const double model = Tuning().powerEfficiency.Eval(plan.targetPercent) / 100.0 * *row->conditionPercent;
        Log(LogLevel::Info, "[UtilityWriter][Curve] game efficiency=" + Num(row->efficiencyPercent) + " model curve(" + std::to_string(plan.targetPercent) +
                                ")*condition=" + std::to_string(model));
    }
    const auto verdict = VerifyFundingReadback(rb, plan.targetFraction, true);
    if (verdict.pass) {
        power_ = {true, req.cellX, req.cellZ, plan.targetFraction};
        return UtilityWriteResult::Applied;
    }
    Log(LogLevel::Error, "[UtilityWriter] power verification failed: " + verdict.reason);
    return UtilityWriteResult::VerificationFailed;
}

UtilityWriteResult UtilityWriter::ApplyWater(cISC4City* city, cISC4RegionalCity* regional, const UtilitySnapshot& snapshot) {
    const UtilityLimits limits;
    const WaterWriteRequest req{config_.target == UtilityWriterConfig::Target::Both ? config_.waterExpectedPercent : config_.expectedPercent,
                                config_.target == UtilityWriterConfig::Target::Both ? config_.waterTargetPercent : config_.targetPercent};
    const auto plan = PlanWaterFunding(snapshot.water, req, Tuning().waterEfficiency, limits, ExportsOf(snapshot, false));
    Log(LogLevel::Info, "[UtilityWriter][Plan] Water " + std::to_string(req.expectedPercent) + "->" + std::to_string(req.targetPercent) +
                            " result=" + UtilityWriteResultName(plan.result) + " (" + plan.reason + ")");
    Log(LogLevel::Info, "[UtilityWriter][Before] " + WaterMetrics(snapshot.water));
    if (!plan.write) return plan.result;

    auto* budget = city->GetBudgetSimulator();
    auto* dept = budget ? budget->GetDepartmentBudget(utility_id::kWaterDepartment) : nullptr;
    if (!dept) return UtilityWriteResult::NotAvailable;
    if (!dept->GetFundingPercentage() || !FractionNear(dept->GetFundingPercentage()->percentage, UtilityFractionForPercent(req.expectedPercent)))
        return UtilityWriteResult::PreconditionChanged;
    const SC4Percentage target{plan.targetFraction};
    dept->SetFundingPercentage(target, 0);  // the return value is not defined by the game (ret 8 straight from the internal setter)
    Log(LogLevel::Info, "[UtilityWriter][Write] DepartmentBudget::SetFundingPercentage(target, lineItem=0) done");

    const auto after = ReadUtilitySnapshot(city, regional, snapshot.lifecycleEpoch, true, false);
    FundingReadback rb;
    rb.getter = after.water.department.funding;
    Log(LogLevel::Info, "[UtilityWriter][After] " + WaterMetrics(after.water));
    const auto verdict = VerifyFundingReadback(rb, plan.targetFraction, false);
    if (verdict.pass) {
        water_ = {true, 0, 0, plan.targetFraction};
        return UtilityWriteResult::Applied;
    }
    Log(LogLevel::Error, "[UtilityWriter] water verification failed: " + verdict.reason);
    return UtilityWriteResult::VerificationFailed;
}

void UtilityWriter::OnMonth(cISC4City* city, cISC4RegionalCity* regional, const UtilitySnapshot& snapshot) {
    ++monthsInCity_;
    LoadConfigIfChanged();
    Observe(snapshot);
    if (!config_.enabled || !city) return;
    if (AlreadyApplied(config_.nonce)) return;
    if (static_cast<int>(monthsInCity_) < config_.minMonths) return;  // let the simulation populate efficiency/counts first

    MarkApplied(config_.nonce);  // one attempt per nonce, whatever the outcome
    power_ = Watch{};            // a new nonce replaces what was being watched
    water_ = Watch{};
    appliedNonce_ = config_.nonce;
    reloadsSinceApply_ = 0;
    monthsSinceReload_ = 0;

    const bool doPower = config_.target == UtilityWriterConfig::Target::Power || config_.target == UtilityWriterConfig::Target::Both;
    const bool doWater = config_.target == UtilityWriterConfig::Target::Water || config_.target == UtilityWriterConfig::Target::Both;
    if (doPower) {
        const auto r = ApplyPower(city, regional, snapshot);
        Log(r == UtilityWriteResult::Applied ? LogLevel::Info : LogLevel::Warning,
            std::string("[UtilityWriter][Result] Power ") + UtilityWriteResultName(r));
    }
    if (doWater) {
        const auto r = ApplyWater(city, regional, snapshot);
        Log(r == UtilityWriteResult::Applied ? LogLevel::Info : LogLevel::Warning,
            std::string("[UtilityWriter][Result] Water ") + UtilityWriteResultName(r));
    }
}

} // namespace sc4af
