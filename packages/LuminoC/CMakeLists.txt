project(LuminoC)

#-------------------------------------------------------------------------------
# Emscripten (Phase 0 + Phase 1) — HelloTest + CoreInstance init/terminate
#-------------------------------------------------------------------------------
# Phase 0 exported only LNHelloTest to validate the Emscripten toolchain.
# Phase 1 additionally pulls a minimal slice of LuminoBase + LuminoCore into
# the wasm executable so that LNInstance_Initialize / LNInstance_Terminate can
# drive the real ln::CoreInstance and surface its log output in the browser.
#
# GraphicsModule / WebGPU backend are still *not* linked here — that lands in
# Phase 2/4. The CoreInstance::init path is gated by __EMSCRIPTEN__ and skips
# glfwInit / GraphicsModule creation entirely.
if(EMSCRIPTEN)
    #-------------------------------------------------------------------------
    # Dependency: tl::expected (header-only). LuminoBase's Result.hpp uses it.
    # Pulled in via FetchContent so the web build does not depend on vcpkg.
    #-------------------------------------------------------------------------
    include(FetchContent)
    FetchContent_Declare(
        tl_expected
        GIT_REPOSITORY https://github.com/TartanLlama/expected.git
        GIT_TAG        v1.1.0
        GIT_SHALLOW    TRUE
    )
    set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(tl_expected)
    # tl::expected is header-only; just need the include dir.
    set(LN_TL_EXPECTED_INCLUDE_DIR "${tl_expected_SOURCE_DIR}/include")

    #-------------------------------------------------------------------------
    # Sources: HelloTest + Phase 1 LuminoBase/LuminoCore minimal slice.
    #-------------------------------------------------------------------------
    set(LN_WEB_SOURCES
        # LuminoC entry points (web-specific)
        src/HelloTest.cpp
        src/WasmAPI.cpp
        # LuminoBase essentials
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/src/Logger.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/src/Result.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/src/RefObject.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/src/math/Math.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/src/math/Matrix4x4.cpp
        # LuminoCore essentials (enough for CoreInstance::init / dispose)
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/CoreInstance.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Object.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Runtime/ObjectRegistry.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Platform/Window.cpp
    )

    add_executable(${PROJECT_NAME} ${LN_WEB_SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/include
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/include
        ${LN_TL_EXPECTED_INCLUDE_DIR}
    )
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".mjs"
    )

    # Emscripten link flags. Keep the set minimal for Phase 1 — heavier flags
    # (USE_WEBGPU, ASYNCIFY, etc.) are introduced in Phase 4.
    set(LN_EM_LINK_FLAGS "")
    string(APPEND LN_EM_LINK_FLAGS " -s WASM=1")
    string(APPEND LN_EM_LINK_FLAGS " -s MODULARIZE=1")
    string(APPEND LN_EM_LINK_FLAGS " -s EXPORT_NAME=\"${PROJECT_NAME}\"")
    string(APPEND LN_EM_LINK_FLAGS " -s EXPORT_ES6=1")
    string(APPEND LN_EM_LINK_FLAGS " -s ENVIRONMENT=web")
    string(APPEND LN_EM_LINK_FLAGS " -s NO_FILESYSTEM=1")
    string(APPEND LN_EM_LINK_FLAGS " -s ALLOW_MEMORY_GROWTH=1")
    string(APPEND LN_EM_LINK_FLAGS " -s NO_EXIT_RUNTIME=1")
    string(APPEND LN_EM_LINK_FLAGS " -s DISABLE_EXCEPTION_CATCHING=1")
    string(APPEND LN_EM_LINK_FLAGS " -s EXPORTED_RUNTIME_METHODS=\"['ccall','cwrap']\"")
    string(APPEND LN_EM_LINK_FLAGS " -s EXPORTED_FUNCTIONS=\"['_malloc','_free']\"")
    # Debug-friendly output for Phase 1.
    string(APPEND LN_EM_LINK_FLAGS " -O0")
    string(APPEND LN_EM_LINK_FLAGS " -g3")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LN_EM_LINK_FLAGS}")

    return()
endif()

#-------------------------------------------------------------------------------
# Desktop build
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/pch.hpp
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.h
)
source_group("" FILES ${LN_LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} SHARED
    ${LN_LOCAL_SOURCES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE LUMINO_BUILD_DLL)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    LuminoBase
    LuminoCore)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

# WebGPU (copy dll)
if(LUMINO_ENABLE_WEBGPU)
    target_link_libraries(${PROJECT_NAME} PRIVATE dawn::webgpu_dawn)
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()

#-------------------------------------------------------------------------------
# Examples
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
