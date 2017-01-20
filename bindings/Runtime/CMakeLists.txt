cmake_minimum_required (VERSION 3.0.0)
project (LuminoRuntime)

include(../../external/Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_Runtime
	include/Common.h
	src/Internal.h
	src/WrapperIF.generated.h
	src/BindingManager.cpp
	src/BindingManager.h
	src/LNBase.cpp
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
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Core/include
	${CMAKE_SOURCE_DIR}/external/Lumino.Math/include)
	
# make postfix.
ln_make_postfix(LN_STATIC_OUTPUT_POSTFIX)
ln_make_postfix_for_shared_lib(LN_SHARED_OUTPUT_POSTFIX)
ln_make_output_dir(LN_LIB_OUTPUT_DIR)

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

#------------------------------
# external deps.
link_libraries(LuminoEngine)
link_libraries(LuminoFoundation)

#------------------------------
# Shared library.
add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${LN_SHARED_OUTPUT_POSTFIX})
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "LUMINO_BUILD_DLL")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")	# .dll
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")	# .lib (linker)

if (LN_USE_UNICODE_CHAR_SET)
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "LuminoU")
else()
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Lumino")
endif()



