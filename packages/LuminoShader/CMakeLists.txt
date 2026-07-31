project(LuminoShader)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------
option(LUMINO_USE_SLANG "Enable Slang shader compiler" ON)

# 生成した WGSL を Dawn (Null backend) で検証する。
# Dawn は LN_ENABLE_WEBGPU のデスクトップビルドで既に取得済みなので追加の依存は増えない。
# Emscripten ビルドではシェーダコンパイラ自体が無効なので対象外。
if(LUMINO_USE_SLANG AND TARGET webgpu AND NOT EMSCRIPTEN)
    set(LUMINO_SHADER_VALIDATE_WGSL_DEFAULT ON)
else()
    set(LUMINO_SHADER_VALIDATE_WGSL_DEFAULT OFF)
endif()
option(LUMINO_SHADER_VALIDATE_WGSL
    "Validate generated WGSL with Dawn at shader compile time"
    ${LUMINO_SHADER_VALIDATE_WGSL_DEFAULT})

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
find_package(nlohmann_json CONFIG REQUIRED)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.hpp
)
file(GLOB LN_LOCAL_SOURCES_shaders
    shaders/*.slang
)
source_group("" FILES ${LN_LOCAL_SOURCES})
source_group("shaders" FILES ${LN_LOCAL_SOURCES_shaders})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} STATIC
    ${LN_LOCAL_SOURCES}
    ${LN_LOCAL_SOURCES_shaders}
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC LuminoBase)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

if(LUMINO_USE_SLANG)
    target_link_libraries(${PROJECT_NAME} PRIVATE slang::slang)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LUMINO_USE_SLANG)
endif()

if(LUMINO_SHADER_VALIDATE_WGSL)
    if(NOT TARGET webgpu)
        message(FATAL_ERROR "LUMINO_SHADER_VALIDATE_WGSL requires the 'webgpu' (Dawn) target.")
    endif()
    target_link_libraries(${PROJECT_NAME} PRIVATE webgpu)
    target_compile_definitions(${PROJECT_NAME} PRIVATE LUMINO_SHADER_VALIDATE_WGSL)
    # WgslValidator を持つ実行ファイルは webgpu_dawn.dll を必要とする。
    # 依存側がコピー処理を書けるように、有効かどうかを PUBLIC な定義でも公開する。
    target_compile_definitions(${PROJECT_NAME} PUBLIC LUMINO_SHADER_HAS_WGSL_VALIDATOR)
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_subdirectory(test)
endif()
