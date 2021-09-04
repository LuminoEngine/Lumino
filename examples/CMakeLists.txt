cmake_minimum_required(VERSION 3.4.1)
project(LuminoApp)

find_package(Lumino)
#set(CMAKE_EXECUTABLE_SUFFIX ".html")
set(CMAKE_EXECUTABLE_SUFFIX ".js")

set(VERBOSE 1)

file(GLOB APP_SOURCES "src/*.h" "src/*.cpp")
 
set(_LINK_FLAGS "--pre-js ${CMAKE_CURRENT_LIST_DIR}/pre.js")
set(_LINK_FLAGS "${_LINK_FLAGS} -s WASM=1")
set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_SDL=2")
set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_GLFW=3")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_WEBGL2=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s FULL_ES2=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s FULL_ES3=1")
set(_LINK_FLAGS "${_LINK_FLAGS} -s ASSERTIONS=1")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_PTHREADS=1")
set(_LINK_FLAGS "${_LINK_FLAGS} -s PTHREAD_POOL_SIZE=8")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
set(_LINK_FLAGS "${_LINK_FLAGS} -s WASM_MEM_MAX=512MB")
set(_LINK_FLAGS "${_LINK_FLAGS} -lopenal")
#set(_LINK_FLAGS "${_LINK_FLAGS} --shared-memory")
set(_LINK_FLAGS "${_LINK_FLAGS} --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell_minimal.html")
set(_LINK_FLAGS "${_LINK_FLAGS} --minify 0")
set(_LINK_FLAGS "${_LINK_FLAGS} -s EXTRA_EXPORTED_RUNTIME_METHODS=\"['ccall']\"")
#set(_LINK_FLAGS "${_LINK_FLAGS} -s \"EXPORTED_FUNCTIONS=['_gogo_test']\"")
set(_LINK_FLAGS "${_LINK_FLAGS} --js-library ${CMAKE_CURRENT_LIST_DIR}/import_funcs.js")




set(CMAKE_CXX_FLAGS_DEBUG "-g4")
#set(CMAKE_CXX_FLAGS "-g4")

add_executable(${PROJECT_NAME} ${APP_SOURCES} PCH.cpp PCH.h)
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${_LINK_FLAGS} -g -gsource-map")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
#target_link_libraries(${PROJECT_NAME} LuminoEngine-static LuminoCore)
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")
