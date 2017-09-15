cmake_minimum_required (VERSION 3.7.0)
project (Lumino)

include(CMake/LuminoCommon.cmake NO_POLICY_SCOPE)


set(LN_STATIC_LIB_OUTPUT_NAME "Lumino")
set(LN_LINK_LIBS "")
set(LN_UNIT_TEST_LINK_LIBS "")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "C:/Proj/Lumino/lib/MSVC140")

#-------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------

# Unicode char set.
option(LN_USE_UNICODE_CHAR_SET "Use unicode char set." ON)

# MSVC Runtime library.
option(LN_MSVC_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

# Exceprion Backtrace.
option(LN_EXCEPTION_BACKTRACE "In some unix environment there is a possibility that can not be compiled." ON)

option(LN_BUILD_C_API "Build 'C API' module." OFF)
option(LN_USE_EXTERNAL_Effekseer "" ON)

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)

#-------------------------------------------------------------------------------
set(LN_DEPENDENCIES_DEBUG_OUTPUT_DIR ${LN_LIB_DEBUG_OUTPUT_ROOT_DIR})
set(LN_DEPENDENCIES_RELEASE_OUTPUT_DIR ${LN_LIB_RELEASE_OUTPUT_ROOT_DIR})
set(LN_DEPENDENCIES_POSTFIX "")
set(LN_DEPENDENCIES_INCLUDE_DIRECTORIES "")
add_subdirectory(External)

#-------------------------------------------------------------------------------
# 
#-------------------------------------------------------------------------------
macro(ln_add_common_property projectName)
	set_target_properties(${projectName} PROPERTIES OUTPUT_NAME "${projectName}")
	set_target_properties(${projectName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${LN_LIB_OUTPUT_DIR}")
	set_target_properties(${projectName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${LN_LIB_DEBUG_OUTPUT_ROOT_DIR}")
	set_target_properties(${projectName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${LN_LIB_RELEASE_OUTPUT_ROOT_DIR}")
	set_target_properties(${projectName} PROPERTIES PREFIX "")
endmacro()

#-------------------------------------------------------------------------------
# External
#-------------------------------------------------------------------------------

add_subdirectory(Source/LuminoCore)

add_subdirectory(Source/Fluorite)

add_subdirectory(Source/LuminoEngine)

add_subdirectory(Source/LuminoExperimental)

add_subdirectory(Source/LuminoLaunch)

add_subdirectory(Samples)

#-------------------------------------------------------------------------------
# Output
#-------------------------------------------------------------------------------
add_definitions(-DLUMINO_ROOT_DIR="${CMAKE_SOURCE_DIR}")

if (LN_EXCEPTION_BACKTRACE)
	add_definitions(-DLN_EXCEPTION_BACKTRACE)
endif()

#------------------------------
# library. (Unicode)
if (LN_USE_EXTERNAL_Effekseer)
	add_definitions(-DLN_USE_EXTERNAL_Effekseer)
endif()

#------------------------------
#
add_subdirectory(Bindings/Runtime)
#add_subdirectory(Bindings/C/Test)
if (MSVC)
	#add_subdirectory(tools/ArchiveMaker)
endif()

if (NOT LN_USE_UNICODE_CHAR_SET)
	#add_subdirectory(tools/LNSL)
	add_subdirectory(tools/LWIG)
endif()

