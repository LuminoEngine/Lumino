cmake_minimum_required (VERSION 3.0.0)
project(EditorCore)

file(GLOB_RECURSE LN_SOURCES_EditorCore "*.hpp" "*.cpp")
source_group("Sources" FILES ${LN_SOURCES_EditorCore})

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_EditorCore})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
