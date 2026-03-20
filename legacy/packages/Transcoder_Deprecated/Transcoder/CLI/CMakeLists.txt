cmake_minimum_required (VERSION 3.0.0)
project(LuminoTranscoder-CLI)

file(GLOB_RECURSE LOCAL_SOURCES "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE . ${LN_ENGINE_INCLUDE_DIR} ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine LuminoTranscoder-Core)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Tools")
target_precompile_headers(${PROJECT_NAME} PRIVATE "PCH.hpp")
