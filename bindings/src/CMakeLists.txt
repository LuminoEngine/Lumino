cmake_minimum_required (VERSION 2.8.7)
project (LNote)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_C_API
	C_API/LNCommon.h
	C_API/LNAudio.cpp
	C_API/LNAudio.h
	C_API/LNGUI.cpp
	C_API/LNGUI.h
	C_API/LNInternal.cpp
	C_API/LNInternal.h
	C_API/LNApplication.cpp
	C_API/LNApplication.h
	C_API/LNBase.cpp
	C_API/LNBase.h
	C_API/LNManager.cpp
	C_API/LNManager.h
	C_API/LNMath.cpp
	C_API/LNMath.h
	C_API/LNTypedef.cpp
	C_API/LNTypedef.h
)

source_group("C_API"	FILES ${LN_SOURCES_C_API})

set(LN_SOURCES
	${LN_SOURCES_C_API})

set(LN_INCLUDE_DIRECTORIES
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Core/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Math/include)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

#------------------------------
# external deps.
link_libraries(Lumino_Static)

#------------------------------
# Shared library.
add_library(LNote SHARED ${LN_SOURCES})
set_target_properties(LNote PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(LNote PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(LNote PROPERTIES COMPILE_DEFINITIONS "LN_BUILD_DLL")
set_target_properties(LNote PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
target_link_libraries(LNote ${LN_LINK_LIBS})
add_custom_command(
	TARGET LNote
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:LNote> "${PROJECT_SOURCE_DIR}")
add_custom_command(
	TARGET LNote
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:LNote> "${PROJECT_SOURCE_DIR}")

