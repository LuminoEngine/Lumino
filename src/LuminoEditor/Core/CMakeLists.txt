cmake_minimum_required (VERSION 3.0.0)
project(EditorCore)

set(LN_ROOT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../include)

file(GLOB_RECURSE LOCAL_HEADERS ${LN_ROOT_INCLUDE_DIR}/LuminoEditor/*.hpp)
source_group("Sources" FILES ${LOCAL_HEADERS})

file(GLOB_RECURSE LN_SOURCES_EditorCore "*.hpp" "*.cpp")
source_group("Sources" FILES ${LN_SOURCES_EditorCore})

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES_EditorCore} ${LOCAL_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Editor")
ln_add_pch(${PROJECT_NAME} "PCH.hpp" "PCH.cpp")
