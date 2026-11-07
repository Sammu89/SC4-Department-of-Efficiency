// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace sc4af {

// Shared validity vocabulary for every read model (civic and utilities).
// Validity comes from the read operation itself, never from a previous count:
// a successful enumeration that returns zero items is VALID_EMPTY, not a failure.
enum class SnapshotValidity {
    Valid,       // authoritative read succeeded and returned items
    ValidEmpty,  // authoritative read succeeded and there is genuinely nothing to manage
    Partial,     // read succeeded but some items could not be read; those items are absent
    Invalid,     // the read itself failed or the lifecycle changed: nothing may be concluded
};

inline const char* ValidityName(SnapshotValidity v) {
    switch (v) {
    case SnapshotValidity::Valid: return "VALID";
    case SnapshotValidity::ValidEmpty: return "VALID_EMPTY";
    case SnapshotValidity::Partial: return "PARTIAL";
    case SnapshotValidity::Invalid: return "INVALID_SNAPSHOT";
    }
    return "?";
}

enum class LogSeverityHint { None, Info, Warning };

// Outcome of asking the game for a list (e.g. GetSchoolBuildings).
struct ListAcquisition {
    bool available = false;  // the owning simulator existed and the call returned success
    std::size_t count = 0;   // raw items returned (only meaningful when available)
};

} // namespace sc4af
