# mkdir build
# cd build
# cmake .. -G"Visual Studio 16 2019" -A x64
# cmake --build

cmake_minimum_required(VERSION 3.15)
project(TestProject)
include(../../../ImportLumino.cmake)    # Path to repository root.

add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoEngine)
