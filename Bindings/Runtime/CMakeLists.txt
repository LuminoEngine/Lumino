cmake_minimum_required (VERSION 3.0.0)
project (LuminoRuntime)

include(../../Source/LuminoCore/CMake/LNCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_Runtime
	include/Common.h
	src/Internal.h
	src/WrapperIF.generated.h
	src/BindingManager.cpp
	src/BindingManager.h
	src/InternalAPI.cpp
	src/LuminoC.generated.h
	src/WrapperIF.generated.cpp
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
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Lumino${LN_LIB_POSTFIX}")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "LUMINO_BUILD_DLL")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")			# .dll
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${LN_LIB_OUTPUT_DIR}")		# .dll
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${LN_LIB_OUTPUT_DIR}")	# .dll
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${LN_LIB_OUTPUT_DIR}")		# .lib (linker)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")			# .lib (linker)
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${LN_LIB_OUTPUT_DIR}")	# .lib (linker)

