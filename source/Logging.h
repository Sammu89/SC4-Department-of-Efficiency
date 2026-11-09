// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include <windows.h>
#include <string_view>

namespace sc4af {

enum class LogLevel { Error, Warning, Info, Debug };

// Opens <SimCity 4 user directory>\Logs\DepartmentEfficiency.log (LogLocation.h: -UserDir, else the Documents known folder; the Logs folder is created), and falls back to
// <dir of the DLL>\DepartmentEfficiency.log (the DLL, the DAT and the INI are installed together in Plugins) when the new place cannot be used.
bool InitLog(HMODULE module);
void Log(LogLevel level, std::string_view message);
void CloseLog();

} // namespace sc4af
