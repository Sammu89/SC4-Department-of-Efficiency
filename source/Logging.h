// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <windows.h>
#include <string_view>

namespace sc4af {

enum class LogLevel { Error, Warning, Info, Debug };

// Everything the plugin reads or writes next to its DLL lives in this folder beside DepartmentEfficiency.dll (never in the Plugins root).
inline constexpr const char* kDataFolderName = "Department of Efficiency";

// Opens <dir of the DLL>\Department of Efficiency\DepartmentEfficiency.log, creating the folder if it is not there (the log is the one write that needs it).
bool InitLog(HMODULE module);
void Log(LogLevel level, std::string_view message);
void CloseLog();

} // namespace sc4af
