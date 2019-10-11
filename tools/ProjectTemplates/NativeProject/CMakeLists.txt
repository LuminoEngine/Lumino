cmake_minimum_required(VERSION 3.1.0)
project(HC0)

# for debug
set(LUMINO_REPO_ROOT "D:/Proj/LN/Lumino")

if (LUMINO_REPO_ROOT)
    include(${LUMINO_REPO_ROOT}/tools/CMake/LuminoSetupForEngineDevelopment.cmake)
else()
    find_package(Lumino)
endif()

add_subdirectory(Projects/Game)
add_subdirectory(Projects/Runtime)
add_subdirectory(Projects/Editor)
