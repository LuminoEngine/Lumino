cmake_minimum_required(VERSION 3.0)
project(LuminoHtml5Runtime)

set(LN_SOURCES
	src/LuminoHtml5Runtime.PCH.cpp
	src/LuminoHtml5Runtime.PCH.h
	src/Main.cpp
)
source_group("Sources" FILES ${LN_SOURCES})

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include" ${LUMINO_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoHtml5Runtime.PCH.h" "src/LuminoHtml5Runtime.PCH.cpp")


if (EMSCRIPTEN)
    #target_compile_definitions(${PROJECT_NAME} PUBLIC -DEMSCRIPTEN=1)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
    #target_compile_definitions(client PUBLIC -DEMSCRIPTEN=1 -DSOCKK=12346)
    #set_target_properties(client PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
    #target_compile_definitions(server PUBLIC -DEMSCRIPTEN=1 -DSOCKK=12346)
    #set_target_properties(server PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
endif()

# test
add_subdirectory(sandbox)

