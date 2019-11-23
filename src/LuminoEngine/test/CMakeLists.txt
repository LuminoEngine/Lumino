cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-Test)

file(GLOB LOCAL_SOURCES *.hpp *.cpp)

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} ${LN_SOURCES_gtest})
target_include_directories(${PROJECT_NAME} PRIVATE ${LN_INCLUDES_gtest})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

#------------------------------------------------------------------------------
# ctest

add_test(NAME LuminoEngine-Test-all COMMAND $<TARGET_FILE:LuminoEngine-Test>)
