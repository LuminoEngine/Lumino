cmake_minimum_required(VERSION 3.20)
project(Lumino VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_definitions(-DLN_REPO_ROOT_DIR="${CMAKE_SOURCE_DIR}")

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(LUMINO_BUILD_EXAMPLES "Build example applications" ON)
option(LUMINO_BUILD_TESTS "Build unit tests" ON)
option(LUMINO_BUILD_TOOLS "Build tools" ON)
option(LUMINO_USE_WEBGPU  "Enable WebGPU backend" ON)

#-------------------------------------------------------------------------------
# Common packages (found via vcpkg toolchain on all platforms)
#-------------------------------------------------------------------------------
find_package(tl-expected CONFIG REQUIRED)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
# 警告をエラーとして扱う
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(EMSCRIPTEN)
    set(LN_PLATFORM_WEB ON)
    set(LN_ENABLE_VULKAN OFF)
    set(LN_ENABLE_WEBGPU ON)
else()
    set(LN_PLATFORM_DESKTOP ON)
    set(LN_ENABLE_VULKAN ON)
    set(LUMINO_USE_SLANG ON)
    if(LUMINO_USE_WEBGPU)
        set(LN_ENABLE_WEBGPU ON)
    else()
        set(LN_ENABLE_WEBGPU OFF)
    endif()
endif()

#-------------------------------------------------------------------------------
# Web (Emscripten) build
#-------------------------------------------------------------------------------
# LuminoBase and LuminoCore are built as static libraries, then linked by
# LuminoC. WebGPU-distribution provides a unified webgpu target that
# automatically selects the emdawnwebgpu backend on Emscripten.
if(LN_PLATFORM_WEB)
    # WebGPU-distribution — automatically selects EMDAWNWEBGPU on Emscripten.
    add_subdirectory(build/webgpu)

    # WebGPU is always enabled for Emscripten
    #set(LN_ENABLE_WEBGPU ON CACHE BOOL "" FORCE)

    # Disable Slang compiler for WASM (only shader deserialization is needed)
    set(LUMINO_USE_SLANG OFF CACHE BOOL "" FORCE)

    add_subdirectory(packages/LuminoBase)
    add_subdirectory(packages/LuminoShader)
    add_subdirectory(packages/LuminoCore)
    add_subdirectory(packages/LuminoC)
    return()
endif()

#-------------------------------------------------------------------------------
# vcpkg packages (found via toolchain) — desktop only
#-------------------------------------------------------------------------------

# WebGPU-distribution — use DAWN backend by default on desktop.
# Override with -DWEBGPU_BACKEND=WGPU to switch to wgpu-native.
if(LN_ENABLE_WEBGPU)
    set(WEBGPU_BACKEND "DAWN" CACHE STRING "WebGPU backend implementation")
    add_subdirectory(build/webgpu)
    find_package(glfw3 CONFIG REQUIRED)
else()
    find_package(glfw3 CONFIG REQUIRED)
endif()

if(LN_PLATFORM_DESKTOP)
    find_package(directx-dxc CONFIG) # dawn で必要になる
endif()

if(LUMINO_USE_SLANG)
    find_package(slang CONFIG REQUIRED)
endif()
#-------------------------------------------------------------------------------
# Tests (GTest は packages より前に find_package する)
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
endif()

#-------------------------------------------------------------------------------
# Utils
#-------------------------------------------------------------------------------
function(setup_c_api_client_configurations TARGET)
    target_link_libraries(${TARGET} PRIVATE LuminoC glfw)
    add_dependencies(${TARGET} LuminoC)

    # WebGPU (copy dll)
    if(LN_ENABLE_WEBGPU)
        target_copy_webgpu_binaries(${TARGET})
    endif()

    if(LUMINO_USE_SLANG)
        target_link_libraries(${TARGET} PRIVATE slang::slang)
    endif()

    # LuminoC.dll
    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoC>
        $<TARGET_FILE_DIR:${TARGET}>
    )

    # glfw.dll
    add_custom_command(
        TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:glfw>
        $<TARGET_FILE_DIR:${TARGET}>
    )

    # DXC libraries required by Dawn's D3D12 backend
    if(LN_ENABLE_WEBGPU AND TARGET Microsoft::DirectXShaderCompiler)
        add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Microsoft::DirectXShaderCompiler>
                $<TARGET_FILE_DIR:${TARGET}>
        )
        add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Microsoft::DXIL>
                $<TARGET_FILE_DIR:${TARGET}>
        )
    endif()
endfunction()

#-------------------------------------------------------------------------------
# packages
#-------------------------------------------------------------------------------
add_subdirectory(packages/LuminoBase)
add_subdirectory(packages/LuminoShader)
add_subdirectory(packages/LuminoCore)
add_subdirectory(packages/LuminoC)
if (LUMINO_BUILD_TOOLS)
    add_subdirectory(packages/luminosc)
endif()
