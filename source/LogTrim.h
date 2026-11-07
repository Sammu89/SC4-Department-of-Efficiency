// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// The log never grows past a limit: when DepartmentEfficiency.log reaches 5 MB, its oldest lines are removed and only the most recent 1 MB is kept, cut at a line
// boundary and headed by one line that says so. Pure string / file helpers with no game dependency; Logging.cpp applies them.
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <system_error>

namespace sc4af {

inline constexpr std::uintmax_t kLogTrimAtBytes = 5u * 1024u * 1024u;      // the size that triggers a trim
inline constexpr std::size_t kLogKeepBytes = 1u * 1024u * 1024u;           // what is kept: the most recent lines, about 1 MB

// The most recent `keep` bytes of `content`, starting at the first complete line, after `note`. Content that is already small enough is returned unchanged.
inline std::string TrimLogTail(std::string_view content, std::size_t keep, std::string_view note) {
    if (content.size() <= keep) return std::string(content);
    std::size_t start = content.size() - keep;
    const std::size_t newline = content.find('\n', start == 0 ? 0 : start - 1);
    start = newline == std::string_view::npos ? content.size() : newline + 1;
    std::string out(note);
    out.append(content.substr(start));
    return out;
}

// Rewrites the file at `path` with TrimLogTail when it holds at least `trimAt` bytes. Returns the size of the file afterwards (0 when it cannot be read). The caller has
// closed its own stream on the file and opens it again afterwards. A failure leaves the file as it was.
inline std::uintmax_t TrimLogFile(const std::filesystem::path& path, std::uintmax_t trimAt, std::size_t keep, std::string_view note) {
    std::error_code ec;
    const std::uintmax_t size = std::filesystem::file_size(path, ec);
    if (ec) return 0;
    if (size < trimAt) return size;
    std::string content;
    {
        std::ifstream in(path, std::ios::binary);
        if (!in) return size;
        content.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    }
    const std::string kept = TrimLogTail(content, keep, note);
    std::filesystem::path tmp = path;
    tmp += L".trim";
    {
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        if (!out) return size;
        out.write(kept.data(), static_cast<std::streamsize>(kept.size()));
        if (!out) { out.close(); std::filesystem::remove(tmp, ec); return size; }
    }
    std::filesystem::rename(tmp, path, ec);                                 // replaces the log in one step
    if (ec) { std::filesystem::remove(tmp, ec); return size; }
    return kept.size();
}

} // namespace sc4af
