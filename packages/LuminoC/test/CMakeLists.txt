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
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest)

# LuminoC.dll とその実行時依存 DLL (glfw3, slang-compiler, webgpu_dawn, dxc) の
# コピーは C-API クライアント共通の設定にまとめてある。個別にコピーを書くと
# slang-compiler.dll のような間接依存を取りこぼす。
ln_setup_c_api_client_configurations(${PROJECT_NAME})

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
# gtest_discover_tests は実行ファイルを --gtest_list_tests 起動するため、
# 依存 DLL のコピーより後ろに置く必要がある。
#
# DISCOVERY_TIMEOUT: この実行ファイルは起動時に GPU バックエンドの DLL
# (webgpu_dawn.dll 等) をロードするため、--gtest_list_tests だけでも既定の 5 秒に
# 近い時間がかかる。タイムアウトすると全テストが *_NOT_BUILT として登録され
# ctest が丸ごと失敗するため、余裕を持たせる。
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}
    DISCOVERY_TIMEOUT 60
)
