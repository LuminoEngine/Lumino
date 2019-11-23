
cmake_minimum_required(VERSION 3.0.0)
project (Lumino)

# TODO: fix full path
set(LN_QT_ROOT "D:/Qt/5.12.3/msvc2017")
set(CMAKE_PREFIX_PATH ${LN_QT_ROOT})

add_subdirectory(Core)
add_subdirectory(GUI)

