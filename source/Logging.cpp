// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#include "Logging.h"
#include "LogTrim.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

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
} // namespace

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
    p += std::wstring(kDataFolderName, kDataFolderName + std::char_traits<char>::length(kDataFolderName)) + L"\\";
    std::error_code ec;
    std::filesystem::create_directories(std::filesystem::path(p), ec);
    p += L"DepartmentEfficiency.log";
    std::lock_guard lock(mutex);
    logPath = std::filesystem::path(p);
    logSize = std::filesystem::file_size(logPath, ec);
    if (ec) logSize = 0;
    TrimIfNeeded();                                                          // a log left large by a previous session is trimmed at once
    if (!file.is_open()) file.open(logPath, std::ios::app);
    return file.is_open();
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
