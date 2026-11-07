// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "GameVersion.h"

#include <windows.h>
#include <vector>

namespace sc4af {

bool IsSupportedGameVersion(const GameVersion& version) noexcept {
    return version.major == 1 && version.minor == 1 &&
           version.revision == 641 && version.build == 0;
}

std::optional<GameVersion> ReadExecutableVersion() {
    std::wstring path(MAX_PATH, L'\0');
    for (;;) {
        const DWORD copied = GetModuleFileNameW(nullptr, path.data(),
                                                 static_cast<DWORD>(path.size()));
        if (copied == 0) return std::nullopt;
        if (copied < path.size()) {
            path.resize(copied);
            break;
        }
        if (path.size() >= 32768) return std::nullopt;
        path.resize(path.size() * 2);
    }

    DWORD ignored = 0;
    const DWORD bytes = GetFileVersionInfoSizeW(path.c_str(), &ignored);
    if (bytes == 0) return std::nullopt;
    std::vector<BYTE> data(bytes);
    if (!GetFileVersionInfoW(path.c_str(), 0, bytes, data.data())) return std::nullopt;

    VS_FIXEDFILEINFO* fixed = nullptr;
    UINT fixedBytes = 0;
    if (!VerQueryValueW(data.data(), L"\\", reinterpret_cast<void**>(&fixed), &fixedBytes) ||
        fixed == nullptr || fixedBytes < sizeof(VS_FIXEDFILEINFO) ||
        fixed->dwSignature != 0xFEEF04BD) {
        return std::nullopt;
    }
    return GameVersion{
        HIWORD(fixed->dwFileVersionMS), LOWORD(fixed->dwFileVersionMS),
        HIWORD(fixed->dwFileVersionLS), LOWORD(fixed->dwFileVersionLS)};
}

std::string FormatGameVersion(const GameVersion& version) {
    return std::to_string(version.major) + "." + std::to_string(version.minor) +
           "." + std::to_string(version.revision) + "." + std::to_string(version.build);
}

} // namespace sc4af
