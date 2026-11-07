// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// 1.1.641.0 ABI facts shared by the utility read layer (UtilityProbe.cpp) and the neighbor deal code (NeighborDealWriter.cpp).
// The SDK header declares cISC4NeighborsSimulator::GetDealCost / GetDealMaxAmount as int64, but the real functions return a
// double in ST0, so they are called through their true signature. The calls are wrapped by SEH because the probes may also
// ask about connections that have no opportunity.

#include "UtilityModel.h"

#include "SC4Percentage.h"
#include "cIGZVariant.h"
#include "cISC4RegionalCity.h"
#include "cISCProperty.h"
#include "cISCPropertyHolder.h"

#include <windows.h>
#include <cstdint>
#include <sstream>

namespace sc4af {
namespace gameabi {

using DealCostFn = double(__thiscall*)(void*, std::int32_t, double, const void*);
using DealMaxFn = double(__thiscall*)(void*, std::int32_t, const void*);
constexpr int kNeighborsDealCost = 35, kNeighborsDealMax = 36;

inline Opt<std::uint32_t> SerialOf(cISC4RegionalCity* c) {
    if (!c) return std::nullopt;
    return c->GetCitySerialNumber();
}

inline bool ReadTuning(cISCPropertyHolder* holder, std::uint32_t id, TuningValue& out) {
    out = TuningValue{};
    out.id = id;
    if (!holder) return false;
    cISCProperty* p = holder->GetProperty(id);
    if (!p) return false;
    cIGZVariant* v = p->GetPropertyValue();
    if (!v) return false;
    out.present = true;
    const std::uint16_t type = v->GetType();
    const bool isArray = (type & cIGZVariant::TypeArray) != 0;
    const std::uint16_t base = static_cast<std::uint16_t>(type & ~cIGZVariant::TypeArray);
    const std::uint32_t count = isArray ? v->GetCount() : 1;
    std::ostringstream name;
    name << "type=0x" << std::hex << type << std::dec << " count=" << count;
    out.type = name.str();
    for (std::uint32_t i = 0; i < count && i < 256; ++i) {
        switch (base) {
        case cIGZVariant::Float32:
            out.values.push_back(isArray ? v->RefFloat32()[i] : v->GetValFloat32());
            break;
        case cIGZVariant::Uint32:
            out.values.push_back(isArray ? v->RefUint32()[i] : v->GetValUint32());
            break;
        case cIGZVariant::Sint32:
            out.values.push_back(isArray ? v->RefSint32()[i] : v->GetValSint32());
            break;
        case cIGZVariant::Sint64:
            out.values.push_back(static_cast<double>(isArray ? v->RefSint64()[i] : v->GetValSint64()));
            break;
        case cIGZVariant::Uint16:
            out.values.push_back(isArray ? v->RefUint16()[i] : v->GetValUint16());
            break;
        case cIGZVariant::Uint8:
            out.values.push_back(isArray ? v->RefUint8()[i] : v->GetValUint8());
            break;
        case cIGZVariant::Bool:
            out.values.push_back((isArray ? v->RefBool()[i] : v->GetValBool()) ? 1.0 : 0.0);
            break;
        default:
            out.type += " (unsupported)";
            return true;
        }
    }
    return true;
}

// The game answers with a double in ST0; an unexpected fault while asking about a connection that has no opportunity must not
// take the process down (no C++ objects live in these frames).
inline bool SafeDealMax(void* ns, std::int32_t type, const void* other, double& out) {
    __try {
        auto** vt = *reinterpret_cast<void***>(ns);
        out = reinterpret_cast<DealMaxFn>(vt[kNeighborsDealMax])(ns, type, other);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

inline bool SafeDealCost(void* ns, std::int32_t type, double amount, const void* other, double& out) {
    __try {
        auto** vt = *reinterpret_cast<void***>(ns);
        out = reinterpret_cast<DealCostFn>(vt[kNeighborsDealCost])(ns, type, amount, other);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

} // namespace gameabi
} // namespace sc4af
