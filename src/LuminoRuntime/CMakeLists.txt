
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES
    src/Entry.cpp
	src/LuminoRuntime.PCH.cpp
	src/LuminoRuntime.PCH.h
)

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${LUMINO_INCLUDE_DIRECTORIES}" "src")
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoRuntime.PCH.h" "src/LuminoRuntime.PCH.cpp")

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME     DESTINATION lib
    LIBRARY     DESTINATION lib)

install(
	EXPORT ${PROJECT_NAME}Targets
	FILE ${PROJECT_NAME}Targets.cmake
	EXPORT_LINK_INTERFACE_LIBRARIES
	DESTINATION "lib/cmake")

install(
    FILES "${PROJECT_NAME}Config.cmake"
    DESTINATION "lib/cmake")
