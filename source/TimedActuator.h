// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// Diagnostics only: times every call the unified controller makes into the game, so a SLOW simulated day can say where its time went ([Perf][SlowDay] in Plugin.cpp).
// It forwards each call unchanged to the real actuator and never decides anything.
#pragma once

#include "UnifiedAuto.h"

#include <chrono>
#include <cstdio>
#include <string>

namespace sc4af {

class TimedUnifiedActuator final : public IUnifiedActuator {
public:
    explicit TimedUnifiedActuator(IUnifiedActuator& inner) : inner_(inner) {}

    bool Reread(UtilitySnapshot& out) override { Scope s(*this, kReread); return inner_.Reread(out); }
    ActuatorResult WritePlant(int x, int z, int from, int to, UtilitySnapshot& after) override { Scope s(*this, kWritePlant); return inner_.WritePlant(x, z, from, to, after); }
    ActuatorResult WriteWater(int from, int to, UtilitySnapshot& after) override { Scope s(*this, kWriteWater); return inner_.WriteWater(from, to, after); }
    bool ReadForPlan(UtilitySnapshot& out) override { Scope s(*this, kReadForPlan); return inner_.ReadForPlan(out); }
    bool ReadServiceCounters(ServiceCounters& out) override { Scope s(*this, kCounters); return inner_.ReadServiceCounters(out); }
    bool RefreshCommodityStats() override { Scope s(*this, kRefresh); return inner_.RefreshCommodityStats(); }
    Opt<std::int64_t> Quote(const DealKey& key, std::int64_t amount) override { Scope s(*this, kQuote); return inner_.Quote(key, amount); }
    DealStep PumpDeal(DealWriteMachine& m, std::uint32_t epoch) override { Scope s(*this, kPumpDeal); return inner_.PumpDeal(m, epoch); }
    SanitationStep PumpSanitation(SanitationWriteMachine& m, std::uint32_t epoch) override { Scope s(*this, kPumpSanitation); return inner_.PumpSanitation(m, epoch); }

    double totalMs() const { double t = 0; for (const double v : ms_) t += v; return t; }
    // "ReadForPlan=12.3(2) RefreshCommodityStats=301.0(1) ..." (only the calls that happened)
    std::string Text() const {
        static const char* names[kCount] = {"Reread", "WritePlant", "WriteWater", "ReadForPlan", "ReadServiceCounters", "RefreshCommodityStats", "Quote", "PumpDeal", "PumpSanitation"};
        std::string out;
        char buf[96];
        for (int i = 0; i < kCount; ++i) {
            if (!calls_[i]) continue;
            std::snprintf(buf, sizeof buf, "%s%s=%.1f(%d)", out.empty() ? "" : " ", names[i], ms_[i], calls_[i]);
            out += buf;
        }
        return out.empty() ? std::string("none") : out;
    }

private:
    enum { kReread, kWritePlant, kWriteWater, kReadForPlan, kCounters, kRefresh, kQuote, kPumpDeal, kPumpSanitation, kCount };
    struct Scope {
        TimedUnifiedActuator& a;
        int i;
        std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
        Scope(TimedUnifiedActuator& owner, int index) : a(owner), i(index) {}
        ~Scope() { a.ms_[i] += std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count(); ++a.calls_[i]; }
    };
    IUnifiedActuator& inner_;
    double ms_[kCount] = {};
    int calls_[kCount] = {};
};

}  // namespace sc4af
