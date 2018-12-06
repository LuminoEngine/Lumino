cmake_minimum_required (VERSION 3.0.0)
project(lumino-cli-Test)

set(LN_SOURCES
	${LN_SOURCES_gtest}
	Common.hpp
	Main.cpp
	Test_Commands.cpp
	TestEnv.cpp
	TestEnv.hpp
)

include_directories(
	${LN_INCLUDES_gtest}
)

add_executable(${PROJECT_NAME} ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
