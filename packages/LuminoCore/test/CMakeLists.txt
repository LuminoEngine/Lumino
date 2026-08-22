project(LuminoCore_test)

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
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../LuminoC/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore GTest::gtest)
if(LN_ENABLE_WEBGPU)
    target_copy_webgpu_binaries(${PROJECT_NAME})
endif()

#-------------------------------------------------------------------------------
# Tests
#-------------------------------------------------------------------------------
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME} DISCOVERY_TIMEOUT 60)
