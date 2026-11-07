// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

class cISC4City;

namespace sc4af {

// Read-only civic omniprobe. For every occupant in the
// ResidentialSimulator school/hospital lists it logs group membership,
// runtime/exemplar properties, native query-buffer dwords, funding getters
// (both flag values), capacity and budget items, plus the offline-table
// classification. Full scan at PostCityInit; afterwards only diffs are logged.
// It calls no setter and writes nothing.
// Full scan (fullDump) or diff-only refresh of the school/hospital inventory.
// The previous inventory is kept per city session; OmniReset() clears it.
void RunOmniRefresh(cISC4City* city, const char* reason, bool fullDump);
// Cheap poll: true when the simulator's school/hospital building counts differ
// from the last refresh (false while no baseline exists).
bool OmniCountsChanged(cISC4City* city);
void OmniReset();

} // namespace sc4af
