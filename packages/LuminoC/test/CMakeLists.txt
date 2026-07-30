project(LuminoC_test)

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
target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoC GTest::gtest)
if(LN_ENABLE_WEBGPU)
    target_copy_webgpu_binaries(${PROJECT_NAME})
endif()

# DLL をテスト実行ファイルのディレクトリへコピー (Windows)
add_custom_command(
    TARGET LuminoC_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LuminoC>
        $<TARGET_FILE_DIR:LuminoC_test>
)

# glfw.dll
add_custom_command(
    TARGET LuminoC_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:glfw>
    $<TARGET_FILE_DIR:LuminoC_test>
)

add_dependencies(${PROJECT_NAME} LuminoC)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
# gtest_discover_tests は POST_BUILD で実行ファイルを --gtest_list_tests 起動するため、
# 依存 DLL (LuminoC.dll, glfw.dll 等) のコピーより後ろに置く必要がある。
# 実 GPU 描画を伴う Test_Graphics.* / Test_DeviceLost.* は CI から除外しやすいよう
# label "gpu" を付ける。
#
# DISCOVERY_TIMEOUT: この実行ファイルは起動時に GPU バックエンドの DLL
# (webgpu_dawn.dll 等) をロードするため、--gtest_list_tests だけでも既定の 5 秒に
# 近い時間がかかる。タイムアウトすると全テストが *_NOT_BUILT として登録され
# ctest が丸ごと失敗するため、余裕を持たせる。
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}
    TEST_FILTER "-Test_Graphics.*:Test_DeviceLost.*"
    DISCOVERY_TIMEOUT 60
)
gtest_discover_tests(${PROJECT_NAME}
    TEST_FILTER "Test_Graphics.*:Test_DeviceLost.*"
    PROPERTIES LABELS "gpu"
    DISCOVERY_TIMEOUT 60
)
