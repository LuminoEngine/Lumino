project(Example-Graphics-WebGL)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
set(CMAKE_EXECUTABLE_SUFFIX ".html")
#string(APPEND LOCAL_LINK_FLAGS " -o index.html")
string(APPEND LOCAL_LINK_FLAGS " -s WASM=2")
string(APPEND LOCAL_LINK_FLAGS " -s USE_GLFW=3")
string(APPEND LOCAL_LINK_FLAGS " -s USE_WEBGL2=1")
string(APPEND LOCAL_LINK_FLAGS " -s FULL_ES3=1")
string(APPEND LOCAL_LINK_FLAGS "--pre-js ${CMAKE_CURRENT_LIST_DIR}/pre.js")

# for Debug
string(APPEND LOCAL_LINK_FLAGS " --minify 0")
string(APPEND LOCAL_LINK_FLAGS " -gsource-map")
string(APPEND LOCAL_LINK_FLAGS " -g3")
string(APPEND LOCAL_LINK_FLAGS " --emit-symbol-map")

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSETS_DIR=U"${ASSETS_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoPlatform LuminoGraphics LuminoShaderCompiler LuminoBitmap)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Examples")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${LOCAL_LINK_FLAGS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
