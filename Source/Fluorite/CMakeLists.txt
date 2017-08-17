cmake_minimum_required (VERSION 3.0.0)
project (Fluorite)

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

set(LN_FL_SOURCES_Common
	Include/Fluorite/AnalyzerContext.h
	Include/Fluorite/Common.h
	Include/Fluorite/Diagnostics.h
	Include/Fluorite/Entity.h
	Include/Fluorite/EntityDatabase.h
	Include/Fluorite/ParserCombinators.h
	Include/Fluorite/Token.h
	Include/Fluorite/Reference.h
	Include/Fluorite/TranslationUnit.h
	Include/Fluorite/Utilities.h
	Source/Fluorite/AnalyzerContext.cpp
	Source/Fluorite/Diagnostics.cpp
#	Source/Fluorite/Entity.cpp
#	Source/Fluorite/EntityDatabase.cpp
	Source/Fluorite/Internal.h
#	Source/Fluorite/Lexer.cpp
#	Source/Fluorite/Lexer.h
	Source/Fluorite/Preprocessor.cpp
	Source/Fluorite/Preprocessor.h
#	Source/Fluorite/Reference.cpp
#	Source/Fluorite/TranslationUnit.cpp
	Source/Fluorite/Token.cpp
	Source/Fluorite/Rpn/RpnParser.cpp
	Source/Fluorite/Rpn/RpnParser.h
)

set(LN_FL_SOURCES_Lexer
	Source/Fluorite/Lexer/AbstractLexer.cpp
	Source/Fluorite/Lexer/AbstractLexer.h
	Source/Fluorite/Lexer/CppLexer.cpp
	Source/Fluorite/Lexer/CppLexer.h
)

set(LN_FL_SOURCES
	${LN_FL_SOURCES_Common}
	${LN_FL_SOURCES_Lexer}
)

source_group("Fluorite"		FILES ${LN_FL_SOURCES_Common})
source_group("Lexer"		FILES ${LN_FL_SOURCES_Lexer})

set(LN_FL_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/Include
	${PROJECT_SOURCE_DIR}/../LuminoCore/Include
)

#-------------------------------------------------------------------------------
# Configure
#-------------------------------------------------------------------------------
#include(${PROJECT_SOURCE_DIR}/../LuminoCore/CMake/LNCommon.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------------------
link_libraries(LuminoCore)

#-------------------------------------------------------------------------------
# Outputs
#-------------------------------------------------------------------------------
# Fluorite
add_library(Fluorite STATIC ${LN_FL_SOURCES})
ln_add_common_property(Fluorite)
set_target_properties(Fluorite PROPERTIES INCLUDE_DIRECTORIES "${LN_FL_INCLUDE_DIRECTORIES}")

# test
add_subdirectory(test)


