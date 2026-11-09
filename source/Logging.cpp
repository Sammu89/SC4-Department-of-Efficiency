// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "Logging.h"
#include "LogLocation.h"
#include "LogTrim.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

#include "KnownFolders.h"

namespace sc4af {
namespace {
std::ofstream file;
std::mutex mutex;
std::filesystem::path logPath;
std::uintmax_t logSize = 0;          // bytes in the file (counted as they are written; the file is checked again when it is trimmed)

const char* Label(LogLevel level) {
    switch (level) {
    case LogLevel::Error: return "ERROR";
    case LogLevel::Warning: return "WARN";
    case LogLevel::Info: return "INFO";
    case LogLevel::Debug: return "DEBUG";
    }
    return "UNKNOWN";
}

std::string Stamp() {
    SYSTEMTIME now{};
    GetLocalTime(&now);
    std::ostringstream line;
    line << '[' << std::setfill('0') << std::setw(4) << now.wYear << '-'
         << std::setw(2) << now.wMonth << '-' << std::setw(2) << now.wDay << ' '
         << std::setw(2) << now.wHour << ':' << std::setw(2) << now.wMinute << ':'
         << std::setw(2) << now.wSecond << "] [DepartmentEfficiency] [";
    return line.str();
}

// The log reached kLogTrimAtBytes: keep its most recent lines (mutex held). The stream is closed while the file is rewritten and opened again after.
void TrimIfNeeded() {
    if (logSize < kLogTrimAtBytes || logPath.empty()) return;
    if (file.is_open()) file.close();
    const std::string note = Stamp() + "INFO] [Log] the log reached 5 MB: its oldest lines were removed, the most recent 1 MB is kept\r\n";
    logSize = TrimLogFile(logPath, kLogTrimAtBytes, kLogKeepBytes, note);
    file.open(logPath, std::ios::app);
}

std::string Utf8(const std::wstring& w) {
    if (w.empty()) return {};
    const int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<std::size_t>(n > 0 ? n : 0), ' ');
    if (n > 0) WideCharToMultiByte(CP_UTF8, 0, w.data(), static_cast<int>(w.size()), s.data(), n, nullptr, nullptr);
    return s;
}

} // namespace

// The log goes to <SimCity 4 user directory>\Logs\DepartmentEfficiency.log (LogLocation.h); the folder of the DLL (Plugins) is the fallback.
bool InitLog(HMODULE module) {
    if (!module) return false;
    std::array<wchar_t, 32768> path{};
    const DWORD copied = GetModuleFileNameW(module, path.data(),
                                             static_cast<DWORD>(path.size()));
    if (copied == 0 || copied >= path.size()) return false;
    std::wstring p(path.data(), copied);
    const auto slash = p.find_last_of(L"\\/");
    if (slash == std::wstring::npos) return false;
    p.resize(slash + 1);
    LogLocationInputs in;
    in.commandLine = GetCommandLineW();
    in.documentsDir = DocumentsKnownFolder();
    in.legacyDir = std::filesystem::path(p);
    LogChoice choice = ChooseLogFile(in);
    if (choice.fallback) OutputDebugStringW((L"[DepartmentEfficiency] log: using the plugin folder: " + choice.note + L"\n").c_str());
    std::error_code ec;
    std::lock_guard lock(mutex);
    logPath = choice.path;
    logSize = std::filesystem::file_size(logPath, ec);
    if (ec) logSize = 0;
    TrimIfNeeded();                                                          // a log left large by a previous session is trimmed at once
    if (!file.is_open()) file.open(logPath, std::ios::app);
    if (!file.is_open() && !choice.fallback) {                               // the chosen file turned out not to be writable after all: the previous place
        choice.fallback = true;
        choice.note = L"the file " + logPath.wstring() + L" could not be opened";
        OutputDebugStringW((L"[DepartmentEfficiency] log: using the plugin folder: " + choice.note + L"\n").c_str());
        std::filesystem::create_directories(in.legacyDir, ec);
        logPath = in.legacyDir / kLogFileName;
        logSize = std::filesystem::file_size(logPath, ec);
        if (ec) logSize = 0;
        file.open(logPath, std::ios::app);
    }
    const bool ok = file.is_open();
    if (ok) {
        std::string line = Stamp() + (choice.fallback ? "WARN] " : "INFO] ") + "[Log] writing to " + Utf8(logPath.wstring());
        if (choice.fallback) line += " (the previous place next to the plugin: " + Utf8(choice.note) + ")";
        line += '\n';
        file << line;
        file.flush();
        logSize += line.size() + 1;
        OutputDebugStringA(line.c_str());
    }
    return ok;
}

void Log(LogLevel level, std::string_view message) {
    std::string output = Stamp() + Label(level) + "] ";
    output.append(message);
    output += '\n';
    std::lock_guard lock(mutex);
    if (file.is_open()) {
        file << output;
        file.flush();
        logSize += output.size() + 1;                                        // + the '\r' the text stream adds before '\n'
        TrimIfNeeded();
    }
    OutputDebugStringA(output.c_str());
}

void CloseLog() {
    std::lock_guard lock(mutex);
    if (file.is_open()) file.close();
}

} // namespace sc4af
