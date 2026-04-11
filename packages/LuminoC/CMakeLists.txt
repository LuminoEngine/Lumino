project(LuminoC)

#-------------------------------------------------------------------------------
# Emscripten (Phase 0) — minimal HelloTest-only build
#-------------------------------------------------------------------------------
# The Phase 0 web build does not link LuminoBase / LuminoCore and only exports
# LNHelloTest to validate the toolchain, CMake guards, MODULARIZE/.mjs output,
# and EMSCRIPTEN_KEEPALIVE symbol export. Everything else will be re-enabled in
# later phases as LuminoBase / LuminoCore are ported to the web target.
if(EMSCRIPTEN)
    add_executable(${PROJECT_NAME}
        src/HelloTest.cpp
    )
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".mjs"
    )

    # Emscripten link flags. Keep the set minimal for Phase 0 — heavier flags
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
    # Debug-friendly output for Phase 0.
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
