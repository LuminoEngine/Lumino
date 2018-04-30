cmake_minimum_required (VERSION 3.0.0)
project(LuminoHtml5Runtime_Sandbox)

set(LN_SOURCES
	Common.hpp
	Main.cpp
)

include_directories(
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoHtml5Runtime)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s USE_WEBGL2=1 -s FULL_ES3=1")
