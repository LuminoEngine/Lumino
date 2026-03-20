cmake_minimum_required(VERSION 3.20)
project(Lumino VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(LUMINO_BUILD_GRAPHICS "Build the Graphics module" ON)
option(LUMINO_BUILD_INPUT    "Build the Input module"    OFF)
option(LUMINO_BUILD_AUDIO    "Build the Audio module"    OFF)
option(LUMINO_BUILD_EXAMPLES "Build example applications" ON)
option(LUMINO_BUILD_TESTS    "Build unit tests"           OFF)
option(LUMINO_ENABLE_VALIDATION "Enable GPU validation layers" ON)

#-------------------------------------------------------------------------------
# Platform detection
#-------------------------------------------------------------------------------
if(EMSCRIPTEN)
    set(LUMINO_PLATFORM_WEB TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "NintendoSwitch" OR DEFINED __NX__)
    set(LUMINO_PLATFORM_NX TRUE)
else()
    set(LUMINO_PLATFORM_DESKTOP TRUE)
endif()

#-------------------------------------------------------------------------------
# vcpkg packages (found via toolchain)
#-------------------------------------------------------------------------------
find_package(tl-expected CONFIG REQUIRED)

if(LUMINO_BUILD_GRAPHICS)
    find_package(Vulkan QUIET)
    if(NOT Vulkan_FOUND)
        find_package(vulkan-headers CONFIG REQUIRED)
    endif()
    if(LUMINO_PLATFORM_DESKTOP)
        find_package(glfw3 CONFIG REQUIRED)
    endif()
endif()

#-------------------------------------------------------------------------------
# packages
#-------------------------------------------------------------------------------
add_subdirectory(packages/base)

#-------------------------------------------------------------------------------
# Platform library
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_GRAPHICS OR LUMINO_BUILD_INPUT)
    add_library(lumino_platform STATIC
        src/lumino/platform/window.cpp
    )
    target_include_directories(lumino_platform PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_libraries(lumino_platform PUBLIC lumino_base)
    if(LUMINO_PLATFORM_DESKTOP)
        target_link_libraries(lumino_platform PRIVATE glfw)
    endif()
endif()

#-------------------------------------------------------------------------------
# Graphics library
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_GRAPHICS)
    add_library(lumino_graphics STATIC
        src/lumino/graphics/rhi/vulkan/vulkan_backend.cpp
    )
    target_include_directories(lumino_graphics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_include_directories(lumino_graphics PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_link_libraries(lumino_graphics PUBLIC lumino_base lumino_platform)

    if(Vulkan_FOUND)
        target_link_libraries(lumino_graphics PRIVATE Vulkan::Vulkan)
    else()
        target_link_libraries(lumino_graphics PRIVATE Vulkan::Headers)
        # Link the Vulkan loader
        if(WIN32)
            target_link_libraries(lumino_graphics PRIVATE vulkan-1)
        else()
            target_link_libraries(lumino_graphics PRIVATE vulkan)
        endif()
    endif()

    if(LUMINO_PLATFORM_DESKTOP)
        target_link_libraries(lumino_graphics PRIVATE glfw)
    endif()
endif()

#-------------------------------------------------------------------------------
# Examples
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_EXAMPLES AND LUMINO_BUILD_GRAPHICS)
    add_subdirectory(examples)
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(tests)
endif()
