
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

set(LN_SOURCES
	src/LuminoEngine.PCH.cpp
	src/LuminoEngine.PCH.h
	include/Lumino/Engine/Common.hpp
	src/Engine/Internal.hpp
)
source_group("Sources" FILES ${FL_SOURCES})

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

# Unit test.
if (LN_BUILD_TESTS)
	add_subdirectory(test)
endif()

