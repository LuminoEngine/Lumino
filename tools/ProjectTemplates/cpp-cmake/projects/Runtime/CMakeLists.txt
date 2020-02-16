cmake_minimum_required(VERSION 3.0.0)
project(Runtime)

set(
	LOCAL_SOURCES
	PCH.cpp
	PCH.h
	Main.Windows.cpp
)
source_group("src" FILES ${LOCAL_SOURCES})

add_executable(${PROJECT_NAME} ${LOCAL_SOURCES} "PCH.h" "PCH.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} Game)
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")
