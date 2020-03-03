cmake_minimum_required(VERSION 3.4.1)
project(LuminoApp)

find_package(Lumino)
set(CMAKE_EXECUTABLE_SUFFIX ".html")

file(GLOB APP_SOURCES "../../src/*.h" "../../src/*.cpp")
 
add_executable(${PROJECT_NAME} ${APP_SOURCES} PCH.cpp PCH.h)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--pre-js ${CMAKE_CURRENT_LIST_DIR}/pre.js -s USE_SDL=2  -s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES2=1 -s FULL_ES3=1")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
target_link_libraries(${PROJECT_NAME} LuminoEngine)
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")

