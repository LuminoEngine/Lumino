cmake_minimum_required (VERSION 3.0.0)
project(LuminoEngine_Test)

set(LN_SOURCES
	${LN_SOURCES_gtest}
	Common.hpp
	Main.cpp
)

include_directories(
	${LN_INCLUDES_gtest}
	${LUMINO_INCLUDE_DIRECTORIES}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoCore)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
