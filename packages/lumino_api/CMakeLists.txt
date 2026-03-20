project(lumino_api)

#-------------------------------------------------------------------------------
# Sources
#-------------------------------------------------------------------------------
file(GLOB LN_LOCAL_SOURCES
    src/*.cpp
    src/*.hpp
    include/${PROJECT_NAME}/*.h
)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_library(
    ${PROJECT_NAME} SHARED
    ${LN_LOCAL_SOURCES}
)
target_compile_definitions(${PROJECT_NAME} PRIVATE LUMINO_BUILD_DLL)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
if(LUMINO_BUILD_TESTS)
    add_executable(lumino_api_test
        test/TestMain.cpp
    )
    target_precompile_headers(lumino_api_test PRIVATE test/pch.hpp)
    target_link_libraries(lumino_api_test PRIVATE lumino_api GTest::gtest)
    add_test(NAME lumino_api_test COMMAND lumino_api_test)

    # DLL をテスト実行ファイルのディレクトリへコピー (Windows)
    add_custom_command(TARGET lumino_api_test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:lumino_api>
            $<TARGET_FILE_DIR:lumino_api_test>
    )
endif()
