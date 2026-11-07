// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Build features. Two separate switches:
//
//  SC4AF_ENABLE_UNIFIED_AUTO           Product: compiles in the unified controller (power / water / sanitation / neighbor deals), the fast service check and the native
//                                      advisor notices. Defined by the product DLL (cmake option SC4_UNIFIED_AUTO, default ON). The safeguards are the same in every build:
//                                      runtime gate (Mode=Auto + EnableUnifiedAuto), per-manager opt-ins, fail-closed behaviour and verified transactions.
//  SC4AF_ENABLE_FINAL_AUTO_VALIDATION  Validation overlay, never shipped: session grading, the city scout, window caption / beep cues and the readiness log. It implies the product
//                                      feature and adds nothing that writes to the game.
#if defined(SC4AF_ENABLE_FINAL_AUTO_VALIDATION) && !defined(SC4AF_ENABLE_UNIFIED_AUTO)
#define SC4AF_ENABLE_UNIFIED_AUTO 1
#endif
