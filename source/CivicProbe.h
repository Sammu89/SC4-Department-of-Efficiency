// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "CivicSnapshot.h"

class cISC4City;

namespace sc4af {

// Reads a single city synchronously. The returned object owns only copied plain values.
CivicSnapshot ReadCivicSnapshot(cISC4City* city);
void LogCivicSnapshot(const CivicSnapshot& snapshot, bool detailed);

} // namespace sc4af
