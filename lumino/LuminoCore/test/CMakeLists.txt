cmake_minimum_required (VERSION 3.0.0)
project(LuminoCore-test)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")

source_group("" FILES ${LOCAL_SOURCES})

set(LOCAL_SOURCES
    ${LOCAL_SOURCES}
)


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------


#include_directories(
#	${LN_INCLUDES_gtest}
	#${LUMINO_INCLUDE_DIRECTORIES}
#)


add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore GTest::gtest)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

if (DEFINED EMSCRIPTEN)
	em_link_pre_js(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pre.js)
endif()

#------------------------------------------------------------------------------
# ctest
add_test(NAME LuminoCore-test-all COMMAND $<TARGET_FILE:LuminoCore-test>)
