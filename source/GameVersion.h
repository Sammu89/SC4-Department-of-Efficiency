// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace sc4af {

struct GameVersion {
    std::uint16_t major;
    std::uint16_t minor;
    std::uint16_t revision;
    std::uint16_t build;
};

bool IsSupportedGameVersion(const GameVersion& version) noexcept;
std::optional<GameVersion> ReadExecutableVersion();
std::string FormatGameVersion(const GameVersion& version);

} // namespace sc4af
