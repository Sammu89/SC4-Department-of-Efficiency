// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
// No "no water" zot over agricultural lots. See FarmWaterZot.h.

#include "FarmWaterZot.h"

#include "Logging.h"

#include "cISC4Lot.h"
#include "cISC4ZoneManager.h"

#include <windows.h>

#include <cstring>
#include <string>

namespace sc4af::farm_water_zot {
namespace {

using Addr = std::uintptr_t;

constexpr Addr kTest = 0x6CAF27;          // mov al,[esp+1C] / test al,al / je 0x6CAF5C
constexpr std::uint8_t kTestBytes[] = {0x8A, 0x44, 0x24, 0x1C, 0x84, 0xC0, 0x74, 0x2D};
constexpr std::size_t kStolen = 6;        // the je stays
constexpr Addr kZotPush = 0x6CAF3E;       // push 0x1C430000 (Zot_NoWater) a few bytes further: the right routine
constexpr std::uint8_t kZotPushBytes[] = {0x68, 0x00, 0x00, 0x43, 0x1C};

bool installed = false;
std::uint32_t suppressed = 0;

bool Same(Addr va, const std::uint8_t* expect, std::size_t n) {
    __try {
        return std::memcmp(reinterpret_cast<const void*>(va), expect, n) == 0;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

// The game's "needs water" for this lot, without agricultural lots. Called on the simulation thread from the stub below; a lot that cannot be read keeps the
// game's answer.
extern "C" std::uint32_t __cdecl NeedsWaterZot(cISC4Lot* lot) {
    bool farm = false;
    __try {
        farm = lot && lot->GetZoneType() == cISC4ZoneManager::ZoneType::Agriculture;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        farm = false;
    }
    if (!farm) return 1;
    ++suppressed;
    return 0;
}

// Replaces `mov al,[esp+1C]; test al,al` (esi = the lot). Inside the call [esp] is the return address, so the game's byte is at [esp+0x20]. eax, ecx and edx
// are free at this point of the routine (both paths after the je load them again). Leaves al and the flags as the game's test would.
__declspec(naked) void Stub() {
    __asm {
        mov al, byte ptr [esp + 0x20]
        test al, al
        jz done
        push esi
        call NeedsWaterZot
        add esp, 4
        test al, al
    done:
        ret
    }
}

} // namespace

bool Install() {
    if (installed) return true;
    if (reinterpret_cast<Addr>(GetModuleHandleW(nullptr)) != 0x400000) {
        Log(LogLevel::Error, "[FarmWaterZot] REFUSED: SimCity 4.exe is not at the expected image base 0x400000; the game's water icons are unchanged");
        return false;
    }
    if (!Same(kTest, kTestBytes, sizeof kTestBytes) || !Same(kZotPush, kZotPushBytes, sizeof kZotPushBytes)) {
        Log(LogLevel::Error, "[FarmWaterZot] REFUSED (nothing patched): the water-zot routine does not match the 1.1.641.0 bytes at 0x6CAF27 / 0x6CAF3E");
        return false;
    }
    DWORD old = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(kTest), kStolen, PAGE_EXECUTE_READWRITE, &old)) {
        Log(LogLevel::Error, "[FarmWaterZot] REFUSED: the code page could not be made writable; the game's water icons are unchanged");
        return false;
    }
    std::uint8_t patch[kStolen];
    patch[0] = 0xE8;                                                                       // call Stub
    const std::int32_t rel = static_cast<std::int32_t>(reinterpret_cast<Addr>(&Stub) - (kTest + 5));
    std::memcpy(patch + 1, &rel, 4);
    patch[5] = 0x90;
    std::memcpy(reinterpret_cast<void*>(kTest), patch, kStolen);                           // one copy, on the simulation thread at load (no city is running yet)
    DWORD ignored = 0;
    VirtualProtect(reinterpret_cast<void*>(kTest), kStolen, old, &ignored);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(kTest), kStolen);
    installed = true;
    Log(LogLevel::Info, "[FarmWaterZot] OK: agricultural lots never get the no-water icon (patch at 0x6CAF27); every other icon is the game's own");
    return true;
}

bool Installed() { return installed; }

std::uint32_t TakeSuppressedCount() {
    const std::uint32_t n = suppressed;
    suppressed = 0;
    return n;
}

} // namespace sc4af::farm_water_zot
