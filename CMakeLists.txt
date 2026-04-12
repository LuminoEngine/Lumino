cmake_minimum_required(VERSION 3.20)
project(Lumino VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-------------------------------------------------------------------------------
# Platform detection
#-------------------------------------------------------------------------------
if(EMSCRIPTEN)
    set(LUMINO_PLATFORM_WEB TRUE)
else()
    set(LUMINO_PLATFORM_DESKTOP TRUE)
endif()

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(LUMINO_BUILD_EXAMPLES "Build example applications" ON)
# Tests require GTest which is not set up for the web build, so default OFF on Emscripten.
if(LUMINO_PLATFORM_WEB)
    option(LUMINO_BUILD_TESTS "Build unit tests" OFF)
else()
    option(LUMINO_BUILD_TESTS "Build unit tests" ON)
endif()
option(LUMINO_ENABLE_VALIDATION "Enable GPU validation layers" ON)
option(LUMINO_ENABLE_WEBGPU  "Enable WebGPU backend via Dawn" OFF)

#-------------------------------------------------------------------------------
# Web (Emscripten) build
#-------------------------------------------------------------------------------
# Sources are pulled directly into the LuminoC executable (see
# packages/LuminoC/CMakeLists.txt). Dawn provides webgpu.h and emdawnwebgpu
# for a unified WebGPU C-API across desktop and web.
if(LUMINO_PLATFORM_WEB)
    # Dawn — provides emdawnwebgpu (WebGPU C-API shim for Emscripten).
    # DAWN_SOURCE_DIR is configurable; defaults to <repo>/third_party/dawn.
    if(NOT DEFINED DAWN_SOURCE_DIR)
        set(DAWN_SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/dawn")
    endif()
    if(NOT EXISTS "${DAWN_SOURCE_DIR}/CMakeLists.txt")
        message(FATAL_ERROR
            "Dawn source not found at ${DAWN_SOURCE_DIR}.\n"
            "Clone it with:\n"
            "  git clone https://dawn.googlesource.com/dawn ${DAWN_SOURCE_DIR}\n"
            "Or set -DDAWN_SOURCE_DIR=<path> to point to an existing checkout.")
    endif()
    set(DAWN_FETCH_DEPENDENCIES ON CACHE BOOL "" FORCE)
    #set(DAWN_FETCH_DEPENDENCIES OFF CACHE BOOL "" FORCE)
    set(DAWN_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(DAWN_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory("${DAWN_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/dawn" EXCLUDE_FROM_ALL)

    add_subdirectory(packages/LuminoC)
    return()
endif()

#-------------------------------------------------------------------------------
# vcpkg packages (found via toolchain) — desktop only
#-------------------------------------------------------------------------------
find_package(tl-expected CONFIG REQUIRED)
find_package(directx-dxc CONFIG) # optional, used for shader compilation on Windows

find_package(glfw3 CONFIG REQUIRED)

if(LUMINO_ENABLE_WEBGPU)
    find_package(dawn CONFIG REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Tests (GTest は packages より前に find_package する)
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
endif()

#-------------------------------------------------------------------------------
# packages
#-------------------------------------------------------------------------------
add_subdirectory(packages/LuminoBase)
add_subdirectory(packages/LuminoShader)
add_subdirectory(packages/LuminoCore)
add_subdirectory(packages/LuminoC)
add_subdirectory(packages/luminosc)
