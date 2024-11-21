project(LuminoFFI-Test)
find_package(glfw3 CONFIG REQUIRED)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${CMAKE_CURRENT_LIST_DIR}/Assets" CMAKE_BINARY_DIR=U"${CMAKE_BINARY_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoFFI-static LuminoCore LuminoEngine glfw GTest::gtest GTest::gtest_main)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.hpp")

add_test(NAME ${PROJECT_NAME}-runtest COMMAND ${PROJECT_NAME})
