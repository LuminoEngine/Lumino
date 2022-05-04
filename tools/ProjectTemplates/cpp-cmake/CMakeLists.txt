cmake_minimum_required(VERSION 3.1.0)
project(LuminoApp)
set(CMAKE_CXX_STANDARD 17)

if (LUMINO_REPO_ROOT)
    # for engine development
    include(${LUMINO_REPO_ROOT}/tools/CMake/LuminoSetupForEngineDevelopment.cmake)
else()
    find_package(Lumino)
endif()
ln_use_msvc_static_runtime_library()

add_subdirectory(projects/Game)
add_subdirectory(projects/Runtime)
#add_subdirectory(projects/Editor)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Runtime)
