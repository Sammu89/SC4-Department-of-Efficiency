// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "SanitationWriter.h"

#include "Logging.h"
#include "UtilityProbe.h"

#include "SC4Percentage.h"
#include "cISC4BudgetSimulator.h"
#include "cISC4City.h"
#include "cISC4DepartmentBudget.h"

#include <windows.h>
#include <cmath>

namespace sc4af {
namespace {

bool SafeSetFunding(cISC4DepartmentBudget* dept, float fraction) {
    __try {
        const SC4Percentage target{fraction};
        dept->SetFundingPercentage(target, 0);   // the return value is not defined by the game (ret 8 straight from the internal setter)
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

bool SafeReadFunding(cISC4DepartmentBudget* dept, float& out) {
    __try {
        const SC4Percentage* p = dept->GetFundingPercentage();
        if (!p) return false;
        out = p->percentage;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

} // namespace

SanitationObservation SanitationFundingTransactor::Observe(cISC4City* city, cISC4RegionalCity* regional, std::uint32_t lifecycleEpoch) {
    if (!city) return SanitationObservation{};
    const UtilitySnapshot snapshot = ReadUtilitySnapshot(city, regional, lifecycleEpoch, true, false);
    return ObserveSanitation(snapshot);
}

SanitationStep SanitationFundingTransactor::Pump(const SanitationWriterResearchToken&, SanitationWriteMachine& machine, cISC4City* city,
                                                 cISC4RegionalCity* regional, std::uint32_t lifecycleEpoch) {
    const SanitationObservation obs = Observe(city, regional, lifecycleEpoch);
    const SanitationStep step = machine.Next(obs);
    Log(LogLevel::Info, std::string("[SanitationWriter][Step] expected=") + std::to_string(machine.request().expectedPercent) + " target=" +
                            std::to_string(machine.request().targetPercent) + " state=" + SanitationWriteStateName(step.state) + " write=" +
                            (step.write ? "yes" : "no") + " (" + step.reason + ")");
    if (!step.write) return step;

    auto* budget = city->GetBudgetSimulator();
    auto* dept = budget ? budget->GetDepartmentBudget(utility_id::kGarbageDepartment) : nullptr;
    if (!dept) { machine.Abort("garbage department vanished at execution"); return step; }

    // Fresh compare-and-set against the department itself (not against the snapshot the plan was made from).
    float fresh = -1.0f;
    if (!SafeReadFunding(dept, fresh) || !FractionNear(fresh, UtilityFractionForPercent(machine.request().expectedPercent))) {
        machine.Abort("funding differs from the planned value at execution (precondition changed)");
        return step;
    }
    if (!SafeSetFunding(dept, step.targetFraction)) { machine.Abort("SetFundingPercentage faulted"); return step; }
    Log(LogLevel::Info, "[SanitationWriter][Write] DepartmentBudget::SetFundingPercentage(target, lineItem=0) done");
    machine.Written();
    return step;
}

} // namespace sc4af
