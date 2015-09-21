cmake_minimum_required (VERSION 3.0.0)
project (LuminoC_Test)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES
	Main.cpp
)

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/../../src/C_API
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Core/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Math/include)

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------
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
endif()

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

#------------------------------
# external deps.
link_libraries(LuminoC)

#------------------------------
# 
add_executable(${PROJECT_NAME} ${LN_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${LN_OUTPUT_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")

