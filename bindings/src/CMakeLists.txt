cmake_minimum_required (VERSION 3.0.0)
project (LuminoC)

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
# Options
#------------------------------------------------------------------------------
# Unicode char set.
if (LN_USE_UNICODE_CHAR_SET)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

if (WIN32)
	# ランタイムライブラリ
	if (LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME)
		foreach (flag CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()
	# 出力ファイルのサフィックス
	set(LN_OUTPUT_POSTFIX "_")
	if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "/machine:x64")	# /machine:x64
		set(LN_OUTPUT_POSTFIX "${LN_OUTPUT_POSTFIX}x64")
	else()
		set(LN_OUTPUT_POSTFIX "${LN_OUTPUT_POSTFIX}x86")
	endif()
	if (${LN_USE_UNICODE_CHAR_SET})
		set(LN_OUTPUT_POSTFIX "${LN_OUTPUT_POSTFIX}u")
	endif()
	if (LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME)
		set(LN_OUTPUT_POSTFIX "${LN_OUTPUT_POSTFIX}MT")
	else()
		set(LN_OUTPUT_POSTFIX "${LN_OUTPUT_POSTFIX}MD")
	endif()
endif()

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

#------------------------------
# external deps.
link_libraries(Lumino_Static)

#------------------------------
# Shared library.
add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${LN_OUTPUT_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "LN_BUILD_DLL")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})


add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")

