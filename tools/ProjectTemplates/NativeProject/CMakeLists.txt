cmake_minimum_required(VERSION 3.1.0)
project(HC0)

# for debug
set(LN_TARGET_ARCH "MSVC2017-x86-MD")
set(LUMINO_REPO_ROOT "D:/Proj/LN/Lumino")
set(LN_BUILD_DIRECTORY "D:/Proj/LN/Lumino/build/MSVC2017-x86-MD")
set(LN_EXTERNAL_FIND_PATH_MODE "build")
set(LN_OS_DESKTOP ON)

if (LUMINO_REPO_ROOT)
    include(${LUMINO_REPO_ROOT}/tools/CMake/LuminoSetupForEngineDevelopment.cmake)
else()
    find_package(Lumino)
endif()

add_subdirectory(Projects/Game)
add_subdirectory(Projects/Runtime)
add_subdirectory(Projects/Editor)
