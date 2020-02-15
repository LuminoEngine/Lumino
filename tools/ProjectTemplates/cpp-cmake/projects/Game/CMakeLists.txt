cmake_minimum_required(VERSION 3.1.0)
project(Game)

file(GLOB LOCAL_SOURCES "../../src/*.h" "../../src/*.cpp")
source_group("src" FILES ${LOCAL_SOURCES})

set(
	LOCAL_SOURCES
	${LOCAL_SOURCES}
)

add_library(${PROJECT_NAME} ${LOCAL_SOURCES} "PCH.h" "PCH.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} LuminoEngine)
ln_add_pch(${PROJECT_NAME} "PCH.h" "PCH.cpp")

