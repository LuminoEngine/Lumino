
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES_Engine
	include/Lumino/Engine/Common.hpp
	include/Lumino/Engine/Engine.hpp
	src/Engine/Internal.hpp
	src/Engine/Engine.cpp
)

set(LN_SOURCES_Platform
	src/Platform/Internal.hpp
	src/Platform/GLFWWindowManager.cpp
	src/Platform/GLFWWindowManager.hpp
	src/Platform/PlatformManager.cpp
	src/Platform/PlatformManager.hpp
	src/Platform/PlatformWindowManager.cpp
	src/Platform/PlatformWindowManager.hpp
)

# Grouping.
source_group("Engine"				FILES ${LN_SOURCES_Engine})
source_group("Platform"				FILES ${LN_SOURCES_Platform})

set(LN_SOURCES
	src/LuminoEngine.PCH.cpp
	src/LuminoEngine.PCH.h
	${LN_SOURCES_Engine}
	${LN_SOURCES_Platform}
)

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

# Unit test.
if (LN_BUILD_TESTS)
	add_subdirectory(test)
	add_subdirectory(sandbox)
endif()

