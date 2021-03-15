cmake_minimum_required (VERSION 3.0.0)
project(LuminoTranscoder-Test)

file(GLOB LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} ${LN_SOURCES_gtest})
target_include_directories(${PROJECT_NAME} PRIVATE ${LN_INCLUDES_gtest})
target_link_libraries(${PROJECT_NAME} LuminoEngine-static LuminoTranscoder-Core)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
