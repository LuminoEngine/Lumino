
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

#set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/_build/vcpkg/installed/x64-windows")
find_package(GTest CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

add_subdirectory(lumino/LuminoCore)

