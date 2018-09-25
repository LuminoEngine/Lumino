
cmake_minimum_required(VERSION 3.4.1)
project(LuminoApp)

set(CMAKE_PREFIX_PATH /lib/cmake)

include(${LUMINO}/Engine/Cpp/LuminoSetup.cmake)


enable_language(CXX)
set(CMAKE_CXX_STANDARD 14) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

find_package(LuminoCore REQUIRED)
find_package(LuminoEngine REQUIRED)

set(CMAKE_EXECUTABLE_SUFFIX .html)

add_executable(
    ${PROJECT_NAME}
    ../../Sources/Application.cpp
    PCH.cpp
    PCH.h)


#set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--pre-js ${CMAKE_CURRENT_LIST_DIR}/pre.js -lopenal -s USE_SDL=2  -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES2=1 -s FULL_ES3=1 -s WASM=1")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_SDL=2  -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES2=1 -s FULL_ES3=1 -s WASM=0")
target_link_libraries(${PROJECT_NAME} LuminoEngine LuminoCore)
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")


