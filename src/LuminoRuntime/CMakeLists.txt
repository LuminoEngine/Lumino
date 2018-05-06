
cmake_minimum_required(VERSION 3.0)
project(LuminoRuntime)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES
	src/LuminoRuntime.PCH.cpp
	src/LuminoRuntime.PCH.h
)

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")
