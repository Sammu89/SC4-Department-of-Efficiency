// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// The Windows "Documents" known folder (follows a redirection to a cloud-synchronised folder or to another drive). shell32 and ole32 are looked up at run time, not imported: the release DLL may import
// nothing but KERNEL32 / USER32 / VERSION (checked when the release is built).

#include <windows.h>
#include <shlobj.h>        // types only

#include <filesystem>

namespace sc4af {

inline // The Documents folder as the shell knows it (it follows a redirection to a cloud-synchronised folder or to another drive); empty when it cannot be determined. shell32 and ole32 are already loaded by the game;
// SHGetKnownFolderPath / CoTaskMemFree are looked up at run time instead of being imported.
std::filesystem::path DocumentsKnownFolder() {
    using GetFolderFn = HRESULT(WINAPI*)(const GUID&, DWORD, HANDLE, PWSTR*);
    using FreeFn = void(WINAPI*)(LPVOID);
    static const GUID kDocuments = {0xFDD39AD0, 0x238F, 0x46AF, {0xAD, 0xB4, 0x6C, 0x85, 0x48, 0x03, 0x69, 0xC7}};      // FOLDERID_Documents
    std::filesystem::path result;
    HMODULE shell = LoadLibraryW(L"shell32.dll");
    HMODULE ole = LoadLibraryW(L"ole32.dll");
    const auto getFolder = shell ? reinterpret_cast<GetFolderFn>(reinterpret_cast<void*>(GetProcAddress(shell, "SHGetKnownFolderPath"))) : nullptr;
    const auto freeMem = ole ? reinterpret_cast<FreeFn>(reinterpret_cast<void*>(GetProcAddress(ole, "CoTaskMemFree"))) : nullptr;
    PWSTR raw = nullptr;
    if (getFolder && SUCCEEDED(getFolder(kDocuments, 0, nullptr, &raw)) && raw) result = raw;
    if (raw && freeMem) freeMem(raw);
    if (shell) FreeLibrary(shell);
    if (ole) FreeLibrary(ole);
    return result;
}

} // namespace sc4af
