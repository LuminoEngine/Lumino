cmake_minimum_required(VERSION 3.0.0)
project(Runtime)

set(
	LOCAL_SOURCES
	PCH.h
	Main.Windows.cpp
)
source_group("src" FILES ${LOCAL_SOURCES})

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} "PCH.h")
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} Game)
target_precompile_headers(${PROJECT_NAME} PRIVATE "PCH.h")

