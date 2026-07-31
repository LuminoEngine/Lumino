project(LuminoShader_test)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES *.cpp *.hpp)
source_group("" FILES ${LN_LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(
    ${PROJECT_NAME}
    ${LN_LOCAL_SOURCES}
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoShader GTest::gtest)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

# WGSL 検証は Dawn を使うため、実行時に webgpu_dawn.dll が必要になる。
if(LUMINO_SHADER_VALIDATE_WGSL)
    target_copy_webgpu_binaries(${PROJECT_NAME})
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
include(GoogleTest)
# webgpu_dawn.dll (WGSL 検証用) のロードが入るため、既定の 5 秒では
# 初回起動時にテスト列挙がタイムアウトすることがある。
gtest_discover_tests(${PROJECT_NAME} DISCOVERY_TIMEOUT 60)