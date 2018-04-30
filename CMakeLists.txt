
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

#------------------------------------------------------------------------------
# options

# build unit test projects
option(LN_BUILD_TESTS "build unit test projects." OFF)

# MSVC Runtime library.
option(LN_MSVC_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." OFF)

#------------------------------------------------------------------------------

include(src/LuminoCommon.cmake)

set(LUMINO_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(LUMINO_INCLUDE_DIRECTORIES
    "${LUMINO_ROOT_PATH}/src/LuminoCore/include"
    "${LUMINO_ROOT_PATH}/src/LuminoEngine/include")

if (DEFINED EMSCRIPTEN)
    include_directories("${EMSCRIPTEN_ROOT_PATH}/system/lib/libcxxabi/include")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()

#------------------------------------------------------------------------------

include(external/External.cmake)
add_subdirectory(src/LuminoCore)
add_subdirectory(src/LuminoEngine)
#add_subdirectory(src/LuminoRuntime)

if (DEFINED EMSCRIPTEN)
    add_subdirectory(src/LuminoHtml5Runtime)
endif()
