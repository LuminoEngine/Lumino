cmake_minimum_required(VERSION 3.0.0)
project(hlsl2glslfork)

# files
set(LN_LOCAL_SOURCES
	hlslang/Include/BaseTypes.h
	hlslang/Include/Common.h
	hlslang/Include/InfoSink.h
	hlslang/Include/InitializeGlobals.h
	hlslang/Include/InitializeParseContext.h
	hlslang/Include/intermediate.h
	hlslang/Include/PoolAlloc.h
	hlslang/Include/Types.h
	hlslang/MachineIndependent/SymbolTable.h
	hlslang/GLSLCodeGen/glslCommon.cpp
	hlslang/GLSLCodeGen/glslCommon.h
	hlslang/GLSLCodeGen/glslFunction.cpp
	hlslang/GLSLCodeGen/glslFunction.h
	hlslang/GLSLCodeGen/glslOutput.cpp
	hlslang/GLSLCodeGen/glslOutput.h
	hlslang/GLSLCodeGen/glslStruct.cpp
	hlslang/GLSLCodeGen/glslStruct.h
	hlslang/GLSLCodeGen/glslSymbol.cpp
	hlslang/GLSLCodeGen/glslSymbol.h
	hlslang/GLSLCodeGen/hlslCrossCompiler.cpp
	hlslang/GLSLCodeGen/hlslCrossCompiler.h
	hlslang/GLSLCodeGen/hlslLinker.cpp
	hlslang/GLSLCodeGen/hlslLinker.h
	hlslang/GLSLCodeGen/hlslSupportLib.cpp
	hlslang/GLSLCodeGen/hlslSupportLib.h
	hlslang/GLSLCodeGen/propagateMutable.cpp
	hlslang/GLSLCodeGen/propagateMutable.h
	hlslang/GLSLCodeGen/typeSamplers.cpp
	hlslang/GLSLCodeGen/typeSamplers.h
	hlslang/MachineIndependent/HLSL2GLSL.cpp
	hlslang/MachineIndependent/hlslang.l
	hlslang/MachineIndependent/hlslang.y
	hlslang/MachineIndependent/InfoSink.cpp
	hlslang/MachineIndependent/Initialize.cpp
	hlslang/MachineIndependent/Initialize.h
	hlslang/MachineIndependent/Intermediate.cpp
	hlslang/MachineIndependent/intermOut.cpp
	hlslang/MachineIndependent/IntermTraverse.cpp
	hlslang/MachineIndependent/localintermediate.h
	#hlslang/MachineIndependent/parseConst.cpp
	hlslang/MachineIndependent/ParseHelper.cpp
	hlslang/MachineIndependent/ParseHelper.h
	hlslang/MachineIndependent/PoolAlloc.cpp
	hlslang/MachineIndependent/RemoveTree.cpp
	hlslang/MachineIndependent/RemoveTree.h
	hlslang/MachineIndependent/SymbolTable.cpp
	hlslang/MachineIndependent/SymbolTable.h
	hlslang/MachineIndependent/ConstantFolding.cpp
	hlslang/MachineIndependent/preprocessor/mojoshader_common.cpp
	hlslang/MachineIndependent/preprocessor/mojoshader_internal.h
	hlslang/MachineIndependent/preprocessor/mojoshader_lexer.cpp
	hlslang/MachineIndependent/preprocessor/mojoshader_preprocessor.cpp
	hlslang/MachineIndependent/preprocessor/mojoshader.cpp
	hlslang/MachineIndependent/preprocessor/mojoshader.h
	hlslang/MachineIndependent/preprocessor/sourceloc.cpp
	hlslang/MachineIndependent/preprocessor/sourceloc.h
	hlslang/MachineIndependent/Gen_hlslang.cpp
	hlslang/MachineIndependent/Gen_hlslang_tab.cpp
	hlslang/MachineIndependent/hlslang_tab.h
)

if (WIN32)
	set(OSDEPENDENT_FILES
		#hlslang/OSDependent/Windows/main.cpp
		hlslang/OSDependent/Windows/osinclude.h
		hlslang/OSDependent/Windows/ossource.cpp
	)
	include_directories("hlslang/OSDependent/Windows")

elseif (APPLE)
	set(OSDEPENDENT_FILES
		hlslang/OSDependent/Mac/osinclude.h
		hlslang/OSDependent/Mac/ossource.cpp
	)
	include_directories("hlslang/OSDependent/Mac")
	
elseif (UNIX)
	set(OSDEPENDENT_FILES
		hlslang/OSDependent/Linux/osinclude.h
		hlslang/OSDependent/Linux/ossource.cpp
	)
	include_directories("hlslang/OSDependent/Linux")
	
endif()

# create project
add_library(${PROJECT_NAME} STATIC ${LN_LOCAL_SOURCES} ${OSDEPENDENT_FILES})
include_directories("hlslang/Include")
include_directories("hlslang/MachineIndependent")

# public include directory
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES ${LN_DEPENDENCIES_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)

