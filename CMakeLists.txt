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
# Tests require GTest which is not set up for the web build, so default OFF on Emscripten.
if(LUMINO_PLATFORM_WEB)
    option(LUMINO_BUILD_EXAMPLES "Build example applications" OFF)
    option(LUMINO_BUILD_TESTS "Build unit tests" OFF)
else()
    option(LUMINO_BUILD_EXAMPLES "Build example applications" ON)
    option(LUMINO_BUILD_TESTS "Build unit tests" ON)
endif()
option(LUMINO_ENABLE_VALIDATION "Enable GPU validation layers" ON)
option(LUMINO_ENABLE_WEBGPU  "Enable WebGPU backend via WebGPU-distribution" OFF)
if(LUMINO_PLATFORM_WEB)
    option(LUMINO_ENABLE_VULKAN "Enable Vulkan backend" OFF)
else()
    option(LUMINO_ENABLE_VULKAN "Enable Vulkan backend" ON)
endif()

#-------------------------------------------------------------------------------
# Web (Emscripten) build
#-------------------------------------------------------------------------------
# LuminoBase and LuminoCore are built as static libraries, then linked by
# LuminoC. WebGPU-distribution provides a unified webgpu target that
# automatically selects the emdawnwebgpu backend on Emscripten.
if(LUMINO_PLATFORM_WEB)
    # tl::expected via FetchContent (no vcpkg on web).
    # Must be available before LuminoBase (which links tl::expected).
    include(FetchContent)
    FetchContent_Declare(
        tl_expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG        v1.1.0
        GIT_SHALLOW    TRUE
    )
    set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(tl_expected)

    # WebGPU-distribution — automatically selects EMDAWNWEBGPU on Emscripten.
    add_subdirectory(third_party/webgpu)

    # WebGPU is always enabled for Emscripten
    set(LUMINO_ENABLE_WEBGPU ON CACHE BOOL "" FORCE)

    add_subdirectory(packages/LuminoBase)
    add_subdirectory(packages/LuminoCore)
    add_subdirectory(packages/LuminoC)
    return()
endif()

#-------------------------------------------------------------------------------
# vcpkg packages (found via toolchain) — desktop only
#-------------------------------------------------------------------------------
find_package(tl-expected CONFIG REQUIRED)
find_package(directx-dxc CONFIG) # optional, used for shader compilation on Windows

find_package(glfw3 CONFIG REQUIRED)

# WebGPU-distribution — use DAWN backend by default on desktop.
# Override with -DWEBGPU_BACKEND=WGPU to switch to wgpu-native.
if(LUMINO_ENABLE_WEBGPU)
    set(WEBGPU_BACKEND "DAWN" CACHE STRING "WebGPU backend implementation")
    add_subdirectory(third_party/webgpu)
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
