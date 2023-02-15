cmake_minimum_required (VERSION 3.20.0)
project(FromVcpkg)

find_package(Lumino CONFIG REQUIRED)

add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE lumino::LuminoCore)

