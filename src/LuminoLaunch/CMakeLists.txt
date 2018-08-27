
cmake_minimum_required(VERSION 3.0)
project(LuminoLaunch)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES
	src/LuminoLaunch.PCH.cpp
    src/LuminoLaunch.PCH.h
    src/Main_Win32.cpp
    src/Main_Html5.cpp
)

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoLaunch.PCH.h" "src/LuminoLaunch.PCH.cpp")

if (EMSCRIPTEN)
    #target_compile_definitions(${PROJECT_NAME} PUBLIC -DEMSCRIPTEN=1)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
    #target_compile_definitions(client PUBLIC -DEMSCRIPTEN=1 -DSOCKK=12346)
    #set_target_properties(client PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
    #target_compile_definitions(server PUBLIC -DEMSCRIPTEN=1 -DSOCKK=12346)
    #set_target_properties(server PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

#------------------------------------------------------------------------------
# Install

install(TARGETS ${PROJECT_NAME}
    EXPORT LuminoLaunchTargets
    ARCHIVE     DESTINATION lib			# Library installation location (Copied at "INSTALL" build time)
    INCLUDES    DESTINATION include)	# include directory (File is not copied)

#install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION .)

install(
	EXPORT LuminoLaunchTargets
	FILE LuminoLaunchTargets.cmake
	EXPORT_LINK_INTERFACE_LIBRARIES
	DESTINATION "lib/cmake")

install(
    FILES "LuminoLaunchConfig.cmake"
    DESTINATION "lib/cmake")
