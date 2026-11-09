// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Where DepartmentEfficiency.log is written: <SimCity 4 user directory>\Logs\DepartmentEfficiency.log (pure, no Windows headers, unit tested).
//
// The user directory is the one the game really uses:
//   1. -UserDir:<path> on the command line of the game (Steam launch options, a shortcut, a test profile), when it is given;
//   2. otherwise the shell's Documents folder (the caller passes the KNOWN FOLDER path, which follows a cloud-sync / other-drive redirection) + "SimCity 4".
// The user directory itself is never created here (the game owns it): if it does not exist, or its Logs folder cannot be created, or the file cannot be opened for append, the
// log falls back to the folder of the DLL (<DLL dir>, the Plugins folder) and says why. Nothing is ever created in any other folder.

#include <cwctype>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <system_error>

namespace sc4af {

inline constexpr const wchar_t* kLogsFolderName = L"Logs";
inline constexpr const wchar_t* kLogFileName = L"DepartmentEfficiency.log";

// The value of -UserDir (or /UserDir) in a Windows command line: case-insensitive name, the value is quoted ("-UserDir:C:\a b\" or -UserDir:"C:\a b") or ends at the next blank.
// std::nullopt when the argument is not there or its value is empty.
inline std::optional<std::wstring> UserDirFromCommandLine(const std::wstring& cmd) {
    static const std::wstring name = L"userdir:";
    for (std::size_t i = 0; i + 1 + name.size() <= cmd.size(); ++i) {
        if (cmd[i] != L'-' && cmd[i] != L'/') continue;
        if (i > 0 && !std::iswspace(cmd[i - 1]) && cmd[i - 1] != L'"') continue;                      // the start of an argument only
        bool same = true;
        for (std::size_t k = 0; k < name.size() && same; ++k) same = std::towlower(cmd[i + 1 + k]) == name[k];
        if (!same) continue;
        std::size_t from = i + 1 + name.size();
        const bool wholeArgQuoted = i > 0 && cmd[i - 1] == L'"';                                      // "-UserDir:C:\a b"
        std::wstring value;
        if (from < cmd.size() && cmd[from] == L'"') {                                                // -UserDir:"C:\a b"
            const auto end = cmd.find(L'"', from + 1);
            value = cmd.substr(from + 1, end == std::wstring::npos ? std::wstring::npos : end - from - 1);
        } else if (wholeArgQuoted) {
            const auto end = cmd.find(L'"', from);
            value = cmd.substr(from, end == std::wstring::npos ? std::wstring::npos : end - from);
        } else {
            std::size_t end = from;
            while (end < cmd.size() && !std::iswspace(cmd[end])) ++end;
            value = cmd.substr(from, end - from);
        }
        if (value.empty()) return std::nullopt;
        return value;
    }
    return std::nullopt;
}

struct LogLocationInputs {
    std::wstring commandLine;                // GetCommandLineW()
    std::filesystem::path documentsDir;      // the Documents KNOWN FOLDER (empty when it could not be determined)
    std::filesystem::path legacyDir;         // <DLL dir>: the fallback
};

struct LogChoice {
    std::filesystem::path path;              // the file to append to
    bool fallback = false;                   // the previous place next to the DLL is used instead of <UserDir>\Logs
    std::filesystem::path userDir;           // the user directory that was tried (empty when none could be determined)
    std::wstring note;                       // why the fallback (empty otherwise)
};

namespace detail {
// The file can be opened for append (it is created when missing, an existing log is never truncated or modified).
inline bool CanAppend(const std::filesystem::path& file) {
    std::ofstream probe(file, std::ios::app);
    return probe.is_open();
}
} // namespace detail

inline LogChoice ChooseLogFile(const LogLocationInputs& in) {
    LogChoice c;
    const auto fallback = [&](std::wstring why) {
        std::error_code ec;
        std::filesystem::create_directories(in.legacyDir, ec);                  // the folder of the DLL: it is there already
        c.path = in.legacyDir / kLogFileName;
        c.fallback = true;
        c.note = std::move(why);
        return c;
    };
    if (const auto explicitDir = UserDirFromCommandLine(in.commandLine)) c.userDir = std::filesystem::path(*explicitDir);
    else if (!in.documentsDir.empty()) c.userDir = in.documentsDir / L"SimCity 4";
    if (c.userDir.empty()) return fallback(L"the SimCity 4 user directory could not be determined");
    std::error_code ec;
    if (!std::filesystem::is_directory(c.userDir, ec)) return fallback(L"the user directory " + c.userDir.wstring() + L" does not exist");
    const auto logs = c.userDir / kLogsFolderName;
    std::filesystem::create_directories(logs, ec);
    if (!std::filesystem::is_directory(logs, ec)) return fallback(L"the folder " + logs.wstring() + L" could not be created");
    const auto file = logs / kLogFileName;
    if (!detail::CanAppend(file)) return fallback(L"the file " + file.wstring() + L" could not be opened for writing");
    c.path = file;
    return c;
}

} // namespace sc4af
