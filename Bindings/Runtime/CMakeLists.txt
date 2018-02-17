cmake_minimum_required (VERSION 3.0.0)
project (LuminoRuntime)

include(../../CMake/LuminoCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_Runtime
	include/Common.h
	src/Internal.h
	src/BindingManager.cpp
	src/BindingManager.h
	src/InternalAPI.cpp
	src/Lumino.FlatC.generated.cpp
	src/Lumino.FlatC.generated.h
	src/Lumino.Wrapper.generated.h
)

source_group("Runtime"	FILES ${LN_SOURCES_Runtime})

set(LN_SOURCES
	${LN_SOURCES_Runtime})

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

# include directories.
set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/../../Source/LuminoEngine/Include
	${PROJECT_SOURCE_DIR}/../../Source/LuminoCore/Include)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

#------------------------------
# external deps.
link_libraries(LuminoEngine)

#------------------------------
# Shared library.
add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Lumino")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "LUMINO_BUILD_DLL")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")

