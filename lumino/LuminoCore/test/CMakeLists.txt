cmake_minimum_required (VERSION 3.0.0)
project(LuminoCore-test)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LN_SOURCES *.cpp *.hpp)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${LN_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore GTest::gtest GTest::gtest_main "-framework Foundation")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

if (DEFINED EMSCRIPTEN)
	em_link_pre_js(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pre.js)
endif()

add_test(NAME ${PROJECT_NAME}-runtest COMMAND ${PROJECT_NAME})
