project(LuminoPlatform-Example-Web)

#-------------------------------------------------------------------------------
# Configuration
#-------------------------------------------------------------------------------
set(CMAKE_EXECUTABLE_SUFFIX ".html")
set(_LINK_FLAGS "${_LINK_FLAGS} -s WASM=2")
set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_GLFW=3")
set(_LINK_FLAGS "${_LINK_FLAGS} -s USE_PTHREADS=1")


#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore LuminoRuntime pthread)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${_LINK_FLAGS} -g")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")

