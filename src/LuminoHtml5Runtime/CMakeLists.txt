cmake_minimum_required(VERSION 3.0)
project(LuminoHtml5Runtime)

set(LN_SOURCES
	src/LuminoHtml5Runtime.PCH.cpp
	src/LuminoHtml5Runtime.PCH.h
	src/Main.cpp
)
source_group("Sources" FILES ${LN_SOURCES})

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoHtml5Runtime.PCH.h" "src/LuminoHtml5Runtime.PCH.cpp")

# test
add_subdirectory(sandbox)

