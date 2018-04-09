
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#------------------------------------------------------------------------------
# options

# build unit test projects
option(LN_BUILD_TESTS "build unit test projects." OFF)

#------------------------------------------------------------------------------

include(src/LuminoCommon.cmake)

set(LUMINO_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(LUMINO_INCLUDE_DIRECTORIES
    "${LUMINO_ROOT_PATH}/src/LuminoCore/include"
    "${LUMINO_ROOT_PATH}/src/LuminoEngine/include")

add_subdirectory(src/LuminoCore)
add_subdirectory(src/LuminoEngine)

