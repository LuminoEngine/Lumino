cmake_minimum_required (VERSION 3.0.0)
project(Editor)

#--------------------
# Files

file(GLOB_RECURSE LOCAL_SOURCES *.h *.cpp)
source_group("src" FILES ${LOCAL_SOURCES})

#--------------------
# Projects

add_library(${PROJECT_NAME} SHARED ${LOCAL_SOURCES} "PCH.h")
target_link_libraries(${PROJECT_NAME} LuminoEngine LuminoEditor)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/.ln/Plugins"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/.ln/Plugins"
)
target_precompile_headers(${PROJECT_NAME} PRIVATE "PCH.h")
