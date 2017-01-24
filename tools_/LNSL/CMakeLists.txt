cmake_minimum_required (VERSION 3.0.0)
project (lnsl)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_lnsl
	Main.cpp
	DXDevice.cpp
	DXDevice.h
)

set(LN_SOURCES_hlsl2glslfork
	hlsl2glslfork/hlslang/GLSLCodeGen/typeSamplers.h
	hlsl2glslfork/hlslang/GLSLCodeGen/glslCommon.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/glslCommon.h
	hlsl2glslfork/hlslang/GLSLCodeGen/glslFunction.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/glslFunction.h
	hlsl2glslfork/hlslang/GLSLCodeGen/glslOutput.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/glslOutput.h
	hlsl2glslfork/hlslang/GLSLCodeGen/glslStruct.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/glslStruct.h
	hlsl2glslfork/hlslang/GLSLCodeGen/glslSymbol.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/glslSymbol.h
	hlsl2glslfork/hlslang/GLSLCodeGen/hlslCrossCompiler.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/hlslCrossCompiler.h
	hlsl2glslfork/hlslang/GLSLCodeGen/hlslLinker.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/hlslLinker.h
	hlsl2glslfork/hlslang/GLSLCodeGen/hlslSupportLib.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/hlslSupportLib.h
	hlsl2glslfork/hlslang/GLSLCodeGen/propagateMutable.cpp
	hlsl2glslfork/hlslang/GLSLCodeGen/propagateMutable.h
	hlsl2glslfork/hlslang/GLSLCodeGen/typeSamplers.cpp
	hlsl2glslfork/hlslang/Include/PoolAlloc.h
	hlsl2glslfork/hlslang/Include/Types.h
	hlsl2glslfork/hlslang/Include/BaseTypes.h
	hlsl2glslfork/hlslang/Include/Common.h
	hlsl2glslfork/hlslang/Include/InfoSink.h
	hlsl2glslfork/hlslang/Include/InitializeGlobals.h
	hlsl2glslfork/hlslang/Include/InitializeParseContext.h
	hlsl2glslfork/hlslang/Include/intermediate.h
	hlsl2glslfork/hlslang/MachineIndependent/SymbolTable.h
	hlsl2glslfork/hlslang/MachineIndependent/ConstantFolding.cpp
	hlsl2glslfork/hlslang/MachineIndependent/HLSL2GLSL.cpp
	hlsl2glslfork/hlslang/MachineIndependent/hlslang.l
	hlsl2glslfork/hlslang/MachineIndependent/hlslang.y
	hlsl2glslfork/hlslang/MachineIndependent/InfoSink.cpp
	hlsl2glslfork/hlslang/MachineIndependent/Initialize.cpp
	hlsl2glslfork/hlslang/MachineIndependent/Initialize.h
	hlsl2glslfork/hlslang/MachineIndependent/Intermediate.cpp
	hlsl2glslfork/hlslang/MachineIndependent/intermOut.cpp
	hlsl2glslfork/hlslang/MachineIndependent/IntermTraverse.cpp
	hlsl2glslfork/hlslang/MachineIndependent/localintermediate.h
	hlsl2glslfork/hlslang/MachineIndependent/ParseHelper.cpp
	hlsl2glslfork/hlslang/MachineIndependent/ParseHelper.h
	hlsl2glslfork/hlslang/MachineIndependent/PoolAlloc.cpp
	hlsl2glslfork/hlslang/MachineIndependent/RemoveTree.cpp
	hlsl2glslfork/hlslang/MachineIndependent/RemoveTree.h
	hlsl2glslfork/hlslang/MachineIndependent/SymbolTable.cpp
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/sourceloc.h
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader.cpp
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader.h
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader_common.cpp
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader_internal.h
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader_lexer.cpp
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader_lexer.re
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/mojoshader_preprocessor.cpp
	hlsl2glslfork/hlslang/MachineIndependent/preprocessor/sourceloc.cpp
	hlsl2glslfork/hlslang/MachineIndependent/hlslang_tab.h
	hlsl2glslfork/hlslang/MachineIndependent/Gen_hlslang.cpp
	hlsl2glslfork/hlslang/MachineIndependent/Gen_hlslang_tab.cpp
	hlsl2glslfork/hlslang/OSDependent/Windows/unistd.h
	hlsl2glslfork/hlslang/OSDependent/Windows/osinclude.h
	hlsl2glslfork/hlslang/OSDependent/Windows/ossource.cpp
)

source_group("lnsl"				FILES ${LN_SOURCES_lnsl})
source_group("hlsl2glslfork"	FILES ${LN_SOURCES_hlsl2glslfork})

set(LN_SOURCES
	${LN_SOURCES_lnsl}
	${LN_SOURCES_hlsl2glslfork}
)

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/hlsl2glslfork/include
	${PROJECT_SOURCE_DIR}/hlsl2glslfork/hlslang/GLSLCodeGen
	${PROJECT_SOURCE_DIR}/hlsl2glslfork/hlslang/Include
	${PROJECT_SOURCE_DIR}/hlsl2glslfork/hlslang/MachineIndependent
	${PROJECT_SOURCE_DIR}/hlsl2glslfork/hlslang/OSDependent/Windows
	${CMAKE_SOURCE_DIR}/external/Lumino.Core/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Math/include
)

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
endif()

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

#------------------------------
# external deps.
link_libraries(LuminoCore_Static)

#------------------------------
# 
add_executable(${PROJECT_NAME} ${LN_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${LN_OUTPUT_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${CMAKE_SOURCE_DIR}/lib")

