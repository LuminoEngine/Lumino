cmake_minimum_required (VERSION 3.0.0)
project (LuminoC)

include(../../external/Lumino.Core/cmake/LNCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

set(LN_SOURCES_C_API
	include/LuminoC.h
	include/LNAudio.h
	include/LNBase.h
	include/LNCommon.h
	include/LNEngine.h
	include/LNGraphics.h
	include/LNMath.h
	include/LNPhysics.h
	include/LNScene.h
	include/LNTypedef.h
	include/LNUI.h
	include/LNGenericInstance.generated.h
	include/LuminoC.h
	src/LNAudio.cpp
	src/LNBase.cpp
	src/LNEngine.cpp
	src/LNGraphics.cpp
	src/LNInternal.cpp
	src/LNInternal.h
	src/LNManager.cpp
	src/LNManager.h
	src/LNMath.cpp
	src/LNOverride_src.cpp
	src/LNPhysics.cpp
	src/LNScene.cpp
	src/LNTypedef.cpp
	src/LNUI.cpp
	src/LNGenericInstance.generated.cpp
	src/LNWrapperImplement.h
)

source_group("C_API"	FILES ${LN_SOURCES_C_API})

set(LN_SOURCES
	${LN_SOURCES_C_API})

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
link_libraries(Lumino_Static)

#------------------------------
# Shared library.
add_library(${PROJECT_NAME} SHARED ${LN_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${LN_SHARED_OUTPUT_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "LN_BUILD_DLL")
set_target_properties(${PROJECT_NAME} PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")	# .dll
set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")	# .lib (linker)
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})

#------------------------------
# Static library.
add_library(${PROJECT_NAME}_Static STATIC ${LN_SOURCES})
set_target_properties(${PROJECT_NAME}_Static PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${LN_STATIC_OUTPUT_POSTFIX})
set_target_properties(${PROJECT_NAME}_Static PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${PROJECT_NAME}_Static PROPERTIES COMPILE_DEFINITIONS "LN_BUILD_DLL")
set_target_properties(${PROJECT_NAME}_Static PROPERTIES INCLUDE_DIRECTORIES "${LN_INCLUDE_DIRECTORIES}")
set_target_properties(${PROJECT_NAME}_Static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")
target_link_libraries(${PROJECT_NAME}_Static ${LN_LINK_LIBS})




