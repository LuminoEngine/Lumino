cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine-Sandbox)

set(LN_SOURCES
	Common.hpp
	SandboxMain.cpp
)

include_directories(
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
