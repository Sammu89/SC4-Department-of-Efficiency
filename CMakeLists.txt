# SPDX-License-Identifier: GPL-3.0-only
# Copyright (c) 2026 Sammu89
cmake_minimum_required(VERSION 3.20)
project(DepartmentEfficiency VERSION 1.0.3 LANGUAGES CXX)

# Department of Efficiency: SimCity 4 Deluxe plugin (GZCOM DLL), Windows x86 only.
# This file builds the plugin DLL and nothing else. The DLL is produced inside the build
# directory; nothing is copied into release/ or into the source tree.

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not supported. Use a separate build directory, for example: cmake -S . -B build -A Win32")
endif()

# SimCity 4 is a 32-bit MSVC program and the plugin calls its objects through the MSVC C++ ABI,
# so the only supported target is MSVC producing Win32/x86 binaries.
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows" OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4 OR NOT MSVC)
    message(FATAL_ERROR
        "Unsupported target: system='${CMAKE_SYSTEM_NAME}', pointer size=${CMAKE_SIZEOF_VOID_P} bytes, compiler='${CMAKE_CXX_COMPILER_ID}'.\n"
        "The plugin must be built with MSVC for Windows Win32/x86, for example:\n"
        "  cmake -S . -B build -G \"Visual Studio 17 2022\" -A Win32\n"
        "  cmake -S . -B build -G \"Visual Studio 18 2026\" -A Win32")
endif()

# One static MSVC runtime for the plugin and for gzcom-dll: players do not need the
# Visual C++ redistributable. Must be set before any target is created.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---- gzcom-dll (LGPL-2.1-or-later, Nelson Gomez and contributors) ---------------------------
# Fetched from upstream at one exact commit and linked statically. It is not copied into this
# repository and not modified. Offline or local copy: configure with
#   -DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<path to a checkout of that commit>
# and, once fetched, -DFETCHCONTENT_FULLY_DISCONNECTED=ON.
include(FetchContent)
FetchContent_Declare(gzcom_dll
    GIT_REPOSITORY https://github.com/nsgomez/gzcom-dll.git
    GIT_TAG        779b669bf88112146d58fba6c7ed44f13ac6ce7d)
FetchContent_MakeAvailable(gzcom_dll)

# ---- Build identification ---------------------------------------------------------------------
# The DLL reports "version <X.Y.Z> commit=<id>;" in its log. By default <id> is the short hash
# of the last commit of this repository that touched the sources or this file, with "+dirty"
# when those paths have uncommitted changes, or "unknown" outside a Git checkout (for example a
# source archive). Override with -DDOE_BUILD_ID=<text>.
set(DOE_BUILD_ID "" CACHE STRING "Identification embedded in the DLL (default: repository commit, or 'unknown')")
set(_doe_id "${DOE_BUILD_ID}")
if(_doe_id STREQUAL "")
    set(_doe_id "unknown")
    find_package(Git QUIET)
    if(Git_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h -- source CMakeLists.txt
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT_VARIABLE _doe_sha OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET RESULT_VARIABLE _doe_rc)
        if(_doe_rc EQUAL 0 AND NOT _doe_sha STREQUAL "")
            set(_doe_id "${_doe_sha}")
            execute_process(
                COMMAND "${GIT_EXECUTABLE}" status --porcelain --untracked-files=all -- source CMakeLists.txt
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE _doe_dirty OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
            if(NOT _doe_dirty STREQUAL "")
                set(_doe_id "${_doe_id}+dirty")
            endif()
        endif()
    endif()
endif()
message(STATUS "Build identification: version ${PROJECT_VERSION} commit=${_doe_id}")

# ---- The plugin DLL ---------------------------------------------------------------------------
add_library(DepartmentEfficiency SHARED
    source/Plugin.cpp
    source/GameVersion.cpp
    source/Logging.cpp
    source/CivicProbe.cpp
    source/NativeCivicQueryReader.cpp
    source/OmniProbe.cpp
    source/LotWatch.cpp
    source/FarmWaterZot.cpp
    source/OptionsUI.cpp
    source/MinistryOrdinance.cpp
    source/CivicFacilityIO.cpp
    source/CivicFundingWriter.cpp
    source/AutoFinance.cpp
    source/UtilityProbe.cpp
    source/UtilityWriter.cpp
    source/UtilityActuator.cpp
    source/UtilityShadow.cpp
    source/NeighborDealWriter.cpp
    source/SanitationWriter.cpp
    source/UnifiedActuator.cpp
    source/LandfillZoneReader.cpp
)
set_target_properties(DepartmentEfficiency PROPERTIES OUTPUT_NAME "DepartmentEfficiency")
target_compile_features(DepartmentEfficiency PRIVATE cxx_std_20)
target_include_directories(DepartmentEfficiency PRIVATE source)
target_compile_definitions(DepartmentEfficiency PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    SC4AF_ENABLE_UNIFIED_AUTO)
target_compile_options(DepartmentEfficiency PRIVATE /W4 /EHsc)
target_link_libraries(DepartmentEfficiency PRIVATE gzcom_dll Version)

# Only Plugin.cpp reports the version and the identification.
set_source_files_properties(source/Plugin.cpp PROPERTIES
    COMPILE_DEFINITIONS "SC4AF_GIT_SHA=\"${_doe_id}\";SC4AF_VERSION=\"${PROJECT_VERSION}\"")
