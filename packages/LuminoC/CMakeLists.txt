project(LuminoC)

#-------------------------------------------------------------------------------
# Emscripten — WebAssembly build
#-------------------------------------------------------------------------------
# Sources are pulled directly into the wasm executable (not separate libraries).
# Dawn is configured in the root CMakeLists.txt and provides emdawnwebgpu
# (WebGPU C-API shim for Emscripten — same webgpu.h as desktop Dawn).
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
    # Sources
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
        # LuminoCore essentials
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/CoreInstance.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Object.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Runtime/ObjectRegistry.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Platform/Window.cpp
        # WebGPU RHI backend (Phase 2)
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/DeviceFactory_Web.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUDevice.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUSwapChain.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUTexture.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUTextureView.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUBuffer.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUShaderModule.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUPipelineLayout.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUSampler.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPURenderPipeline.cpp
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUCommandBuffer.cpp
    )

    add_executable(${PROJECT_NAME} ${LN_WEB_SOURCES})
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/packages/LuminoBase/include
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/include
        # WebGPU RHI hpp files live in the src tree (private headers).
        ${CMAKE_SOURCE_DIR}/packages/LuminoCore/src
        ${LN_TL_EXPECTED_INCLUDE_DIR}
    )
    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".mjs"
    )

    # Link Dawn's emdawnwebgpu — provides webgpu.h C-API implementation
    # for Emscripten (bridges to the browser's native WebGPU).
    target_link_libraries(${PROJECT_NAME} PRIVATE emdawnwebgpu_c)

    # Emscripten link flags
    set(LN_EM_LINK_FLAGS "")
    string(APPEND LN_EM_LINK_FLAGS " -sWASM=1")
    string(APPEND LN_EM_LINK_FLAGS " -sMODULARIZE=1")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORT_NAME=\"${PROJECT_NAME}\"")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORT_ES6=1")
    string(APPEND LN_EM_LINK_FLAGS " -sENVIRONMENT=web")
    string(APPEND LN_EM_LINK_FLAGS " -sNO_FILESYSTEM=1")
    string(APPEND LN_EM_LINK_FLAGS " -sALLOW_MEMORY_GROWTH=1")
    string(APPEND LN_EM_LINK_FLAGS " -sNO_EXIT_RUNTIME=1")
    string(APPEND LN_EM_LINK_FLAGS " -sDISABLE_EXCEPTION_CATCHING=1")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORTED_RUNTIME_METHODS=\"['ccall','cwrap']\"")
    string(APPEND LN_EM_LINK_FLAGS " -sEXPORTED_FUNCTIONS=\"['_malloc','_free']\"")
    # ASYNCIFY is required for WebGPU async operations (RequestAdapter,
    # RequestDevice, etc.) to yield to the browser event loop.
    string(APPEND LN_EM_LINK_FLAGS " -sASYNCIFY=1")
    # Debug-friendly output for development.
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
