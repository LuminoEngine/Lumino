
cmake_minimum_required(VERSION 3.0)
project(LuminoEngine)

#------------------------------------------------------------------------------
# Files

set(LN_SOURCES_Engine
	include/Lumino/Engine/Common.hpp
	include/Lumino/Engine/Engine.hpp
	include/Lumino/Engine/EngineSettings.hpp
	src/Engine/Internal.hpp
	src/Engine/Engine.cpp
	src/Engine/EngineDomain.hpp
	src/Engine/EngineManager.cpp
	src/Engine/EngineManager.hpp
)

set(LN_SOURCES_Platform
	include/Lumino/Platform/PlatformEvent.hpp
	src/Platform/Internal.hpp
	src/Platform/GLFWPlatformWindowManager.cpp
	src/Platform/GLFWPlatformWindowManager.hpp
	src/Platform/PlatformEvent.cpp
	src/Platform/PlatformManager.cpp
	src/Platform/PlatformManager.hpp
	src/Platform/PlatformWindowManager.cpp
	src/Platform/PlatformWindowManager.hpp
)

set(LN_SOURCES_Graphics
	include/Lumino/Graphics/GeometryStructs.hpp
	src/Graphics/Internal.hpp
	src/Graphics/GeometryStructs.cpp
	src/Graphics/GraphicsManager.cpp
	src/Graphics/GraphicsManager.hpp
)

# Grouping.
source_group("Engine"				FILES ${LN_SOURCES_Engine})
source_group("Platform"				FILES ${LN_SOURCES_Platform})
source_group("Graphics"				FILES ${LN_SOURCES_Graphics})

set(LN_SOURCES
	src/LuminoEngine.PCH.cpp
	src/LuminoEngine.PCH.h
	include/LuminoEngine.hpp
	${LN_SOURCES_Engine}
	${LN_SOURCES_Platform}
	${LN_SOURCES_Graphics}
)

#------------------------------------------------------------------------------
# Targets


add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${LUMINO_INCLUDE_DIRECTORIES}" "src" ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} LuminoCore glfw)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoEngine.PCH.h" "src/LuminoEngine.PCH.cpp")

# Unit test.
if (LN_BUILD_TESTS)
	add_subdirectory(test)
	add_subdirectory(sandbox)
endif()

