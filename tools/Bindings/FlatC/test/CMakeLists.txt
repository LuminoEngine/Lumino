
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime-Test)

#------------------------------------------------------------------------------
# Files

file(GLOB LOCAL_SOURCES "*.h" "*.hpp" "*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

#------------------------------------------------------------------------------
# Targets

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} ${LOCAL_FLATC_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${LUMINO_INCLUDE_DIRECTORIES}" .)
target_link_libraries(${PROJECT_NAME} LuminoRuntime-Static)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
