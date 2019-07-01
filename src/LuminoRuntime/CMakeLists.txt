
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime)

#------------------------------------------------------------------------------
# Files

file(GLOB LOCAL_SOURCES "src/*.h" "src/*.hpp" "src/*.cpp")
source_group("Sources" FILES ${LOCAL_SOURCES})

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} SHARED ${LOCAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${LUMINO_INCLUDE_DIRECTORIES}" "src")
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/PCH.hpp" "src/PCH.cpp")

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME     DESTINATION lib
    ARCHIVE     DESTINATION lib
    LIBRARY     DESTINATION lib)

install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	EXPORT_LINK_INTERFACE_LIBRARIES
	DESTINATION "lib/cmake")

install(
    FILES "${PROJECT_NAME}Config.cmake"
    DESTINATION "lib/cmake")
