cmake_minimum_required (VERSION 3.0.0)
project (Lumino)

include(../../CMake/LuminoCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_Runtime
	Include/Common.h
	Source/Internal.h
	Source/BindingManager.cpp
	Source/BindingManager.h
	Source/InternalAPI.cpp
	Source/Lumino.FlatC.generated.cpp
	Source/Lumino.FlatC.generated.h
	Source/Lumino.Wrapper.generated.h
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

add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "LUMINO_BUILD_DLL")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
