cmake_minimum_required(VERSION 3.20)
project(Lumino VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(LUMINO_BUILD_EXAMPLES "Build example applications" ON)
option(LUMINO_BUILD_TESTS    "Build unit tests"           ON)
option(LUMINO_ENABLE_VALIDATION "Enable GPU validation layers" ON)
option(LUMINO_ENABLE_WEBGPU  "Enable WebGPU backend via Dawn" OFF)

#-------------------------------------------------------------------------------
# Platform detection
#-------------------------------------------------------------------------------
if(EMSCRIPTEN)
    set(LUMINO_PLATFORM_WEB TRUE)
else()
    set(LUMINO_PLATFORM_DESKTOP TRUE)
endif()

#-------------------------------------------------------------------------------
# vcpkg packages (found via toolchain)
#-------------------------------------------------------------------------------
find_package(tl-expected CONFIG REQUIRED)

if(LUMINO_PLATFORM_DESKTOP)
    find_package(glfw3 CONFIG REQUIRED)
endif()
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

