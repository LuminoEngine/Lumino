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

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})