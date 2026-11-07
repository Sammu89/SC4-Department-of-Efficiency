// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "NeighborDealWriter.h"

#include "Logging.h"
#include "NeighborGameAbi.h"

#include "SC4List.h"
#include "cISC4City.h"
#include "cISC4NeighborConnection.h"
#include "cISC4NeighborDeal.h"
#include "cISC4NeighborsSimulator.h"
#include "cISC4RegionalCity.h"
#include "cISCPropertyHolder.h"
#include "cISCResExemplar.h"

#include <windows.h>
#include <cmath>

namespace sc4af {
namespace {

using namespace gameabi;

// Tuned minimum deal amount per commodity (NDb_MinDealAmount*), the M of the quantity ladder.
std::uint32_t MinAmountProperty(std::int32_t dealType) {
    switch (CommodityOfDealType(dealType)) {
    case Commodity::Power: return 0xEA46932Cu;
    case Commodity::Water: return 0x6A469337u;
    case Commodity::Garbage: return 0x8A46933Du;
    default: return 0;
    }
}

cISC4RegionalCity* NeighborBySerial(cISC4NeighborsSimulator* ns, std::uint32_t serial) {
    SC4List<cISC4NeighborConnection*> connections;
    ns->GetAllConnections(connections);
    for (auto* c : connections) {
        if (!c) continue;
        auto* other = c->GetCityOnOtherSideOfConnection();
        if (other && other->GetCitySerialNumber() == serial) return other;
    }
    return nullptr;
}

// The deal primitives are called through tiny SEH frames (no C++ objects live in them): a fault is a technical failure of the
// transaction, never a silent success.
bool SafeActivate(cISC4NeighborDeal* d, bool& result) {
    __try {
        result = d->Activate();
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

bool SafeSetAmount(cISC4NeighborDeal* d, std::int32_t amount) {
    __try {
        d->SetAmount(amount);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

bool SafeCancel(cISC4NeighborDeal* d) {
    __try {
        d->Cancel();
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

bool SafeUpdateCommodityStats(cISC4NeighborsSimulator* ns, cISC4City* city) {
    __try {
        ns->UpdateCommodityStats(city);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

Opt<std::int64_t> TruncFlow(float v) {
    if (!std::isfinite(v)) return std::nullopt;
    return static_cast<std::int64_t>(std::llround(v));
}

} // namespace

DealObservation NeighborDealTransactor::Observe(cISC4City* city, cISC4RegionalCity* regional, const DealKey& key, std::uint32_t lifecycleEpoch) {
    DealObservation o;
    o.lifecycleEpoch = lifecycleEpoch;
    auto* ns = city ? city->GetNeighborsSimulator() : nullptr;
    if (!ns || !regional) return o;

    // Rows: the list owns one reference per element (GetLocalDeals AddRefs, the list destructor releases): no extra AddRef (it would leak one reference per deal per read).
    SC4List<cISC4NeighborDeal*> local;
    if (!regional->GetLocalDeals(local)) return o;
    for (auto* d : local) {
        if (!d) continue;
        const std::int32_t t = d->GetType();
        if (!IsKnownDealType(t)) return o;           // an object we cannot classify: the observation is not trustworthy
        ExistingDealRow row;
        row.pointer = reinterpret_cast<std::uintptr_t>(d);   // diagnostics only
        row.type = t;
        row.otherCitySerial = SerialOf(d->GetCityOnOtherSideOfConnection());
        row.amount = d->GetAmount();
        row.lastAmount = d->GetLastAmount();
        row.incAmount = d->GetIncAmount();
        row.incAmount2 = d->GetIncAmount2();
        row.amountUsed = d->GetAmountUsed();
        row.flags = d->GetFlags();
        row.cost = d->GetCost();
        o.deals.push_back(row);
    }

    for (int c = 0; c < 3; ++c) {
        o.regionalImported[c] = TruncFlow(regional->GetImported(c + 1));
        o.regionalExported[c] = TruncFlow(regional->GetExported(c + 1));
    }
    o.monthlyIncome = ns->GetTotalMonthlyNeighborDealIncome();
    o.monthlyExpense = ns->GetTotalMonthlyNeighborDealExpense();

    if (auto* other = NeighborBySerial(ns, key.neighbor)) {
        double v = 0.0;
        if (SafeDealMax(ns, key.type, other, v) && std::isfinite(v)) o.maxAmountTruncated = static_cast<std::int64_t>(v);
    }
    if (const auto prop = MinAmountProperty(key.type)) {
        cISCResExemplar* tuning = ns->GetTuningExemplar();
        cISCPropertyHolder* holder = tuning ? tuning->AsISCPropertyHolder() : nullptr;
        TuningValue tv;
        if (ReadTuning(holder, prop, tv) && tv.present && !tv.values.empty()) o.minAmount = static_cast<std::int32_t>(tv.values.front());
    }
    o.valid = true;
    return o;
}

bool NeighborDealTransactor::RefreshCommodityStats(cISC4City* city) {
    auto* ns = city ? city->GetNeighborsSimulator() : nullptr;
    return ns && SafeUpdateCommodityStats(ns, city);
}

Opt<std::int64_t> NeighborDealTransactor::Quote(cISC4City* city, const DealKey& key, std::int64_t amount) {
    auto* ns = city ? city->GetNeighborsSimulator() : nullptr;
    if (!ns) return std::nullopt;
    auto* other = NeighborBySerial(ns, key.neighbor);
    double v = 0.0;
    if (!other || !SafeDealCost(ns, key.type, static_cast<double>(amount), other, v) || !std::isfinite(v)) return std::nullopt;
    return static_cast<std::int64_t>(v);
}

DealStep NeighborDealTransactor::Pump(const DealWriterResearchToken&, DealWriteMachine& machine, cISC4City* city,
                                      cISC4RegionalCity* regional, std::uint32_t lifecycleEpoch) {
    const DealKey key = machine.request().key;
    const DealObservation obs = Observe(city, regional, key, lifecycleEpoch);
    const DealStep step = machine.Next(obs);
    Log(LogLevel::Info, std::string("[DealWriter][Step] op=") + DealWriteOpName(machine.request().op) + " type=" + std::to_string(key.type) +
                            " neighbor=" + std::to_string(key.neighbor) + " state=" + DealWriteStateName(step.state) + " primitive=" +
                            DealPrimitiveName(step.primitive) + " amount=" + std::to_string(step.amount) + " (" + step.reason + ")");
    if (step.primitive == DealPrimitive::None) return step;

    auto* ns = city ? city->GetNeighborsSimulator() : nullptr;
    if (!ns) { machine.Abort("NeighborsSimulator unavailable at execution"); return step; }

    if (step.primitive == DealPrimitive::ProposeDeals) {
        // Exactly the window builder's call: standalone list, add flag true (the proposals enter the simulator list).
        SC4List<cISC4NeighborConnection*> proposals;
        const bool ok = ns->ProposeDeals(proposals, true);
        Log(LogLevel::Info, std::string("[DealWriter][Primitive] ProposeDeals(list,true) returned ") + (ok ? "true" : "false"));
        machine.Executed(true);   // the proposal list is only a scratch return; the next observation proves what exists
        return step;
    }

    // Structural re-acquire of the live deal by (type, neighbor): never a retained pointer.
    SC4List<cISC4NeighborDeal*> local;
    cISC4NeighborDeal* target = nullptr;
    int matches = 0;
    if (regional && regional->GetLocalDeals(local)) {
        for (auto* d : local) {
            if (!d) continue;
            if (d->GetType() == key.type) {
                auto* other = d->GetCityOnOtherSideOfConnection();
                if (other && other->GetCitySerialNumber() == key.neighbor) { target = d; ++matches; }
            }
        }
    }
    if (matches != 1) { machine.Abort(matches == 0 ? "target vanished between observation and execution" : "target became ambiguous at execution"); return step; }

    bool ok = true;
    switch (step.primitive) {
    case DealPrimitive::Activate: {
        bool result = false;
        if (!SafeActivate(target, result)) { machine.Abort("Activate() faulted"); return step; }
        ok = result;
        break;
    }
    case DealPrimitive::SetAmount:
        if (!SafeSetAmount(target, step.amount)) { machine.Abort("SetAmount() faulted"); return step; }
        break;
    case DealPrimitive::Cancel:
        if (!SafeCancel(target)) { machine.Abort("Cancel() faulted"); return step; }
        break;
    default:
        machine.Abort("unknown primitive");
        return step;
    }
    Log(LogLevel::Info, std::string("[DealWriter][Primitive] ") + DealPrimitiveName(step.primitive) + " returned " + (ok ? "true" : "false"));
    machine.Executed(ok);
    return step;
}

} // namespace sc4af
