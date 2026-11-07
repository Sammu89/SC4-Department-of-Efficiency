// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// One RegionalCity commodity record as the Neighbor Deal formulas see it (five doubles at map entry +0 .. +0x20).
// Kept in its own header so the read model (UtilityModel.h) and the pricing model (DealQuote.h) can share it.

namespace sc4af {

struct CommodityBook {
    double produced = 0.0, imported = 0.0, demanded = 0.0, exported = 0.0, costPerUnit = 0.0;
};

} // namespace sc4af
