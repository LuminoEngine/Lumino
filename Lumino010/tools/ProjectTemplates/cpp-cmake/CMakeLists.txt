cmake_minimum_required(VERSION 3.1.0)
project(LuminoApp)
set(LN_MSVC_STATIC_RUNTIME ON)

if (LUMINO_REPO_ROOT)
    # for engine development
    include(${LUMINO_REPO_ROOT}/tools/CMake/LuminoSetupForEngineDevelopment.cmake)
else()
    find_package(Lumino)
endif()

add_subdirectory(projects/Game)
add_subdirectory(projects/Runtime)
add_subdirectory(projects/Editor)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Runtime)
