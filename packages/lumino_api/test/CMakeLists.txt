project(lumino_api_test)

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
target_link_libraries(${PROJECT_NAME} PRIVATE lumino_api GTest::gtest)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

# DLL をテスト実行ファイルのディレクトリへコピー (Windows)
add_custom_command(TARGET lumino_api_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:lumino_api>
        $<TARGET_FILE_DIR:lumino_api_test>
)
