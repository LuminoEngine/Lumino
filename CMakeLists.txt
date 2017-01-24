cmake_minimum_required (VERSION 2.8.7)
project (LuminoEngine)

include(Source/LuminoCore/CMake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_DEPENDENCIES_ROOT_DIR "${PROJECT_SOURCE_DIR}/external/LuminoDependencies/")
include(external/LuminoDependencies/LuminoDependencies.cmake)

set(LN_STATIC_LIB_OUTPUT_NAME "Lumino")
set(LN_LINK_LIBS "")
set(LN_UNIT_TEST_LINK_LIBS "")
set(LN_LIB_POSTFIX "")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "C:/Proj/Lumino/lib/MSVC140")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

# Unicode char set.
option(LN_USE_UNICODE_CHAR_SET "Use unicode char set." ON)

# MSVC Runtime library.
option(LN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

# Exceprion Backtrace.
option(LN_EXCEPTION_BACKTRACE "In some unix environment there is a possibility that can not be compiled." ON)

option(LN_BUILD_C_API "Build 'C API' module." OFF)
option(LN_USE_EXTERNAL_Effekseer "" ON)

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)

#------------------------------------------------------------------------------
# External
#------------------------------------------------------------------------------

add_subdirectory(Source/LuminoCore)
link_libraries(LuminoCore_Static)
link_libraries(LuminoMath)

add_subdirectory(Source/Fluorite)

add_subdirectory(Source/LuminoEngine)

# Make postfix.
ln_make_postfix(LN_LIB_POSTFIX)



#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------
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
add_subdirectory(bindings/Runtime)
#add_subdirectory(bindings/C/Test)
if (MSVC)
	#add_subdirectory(tools/ArchiveMaker)
endif()

if (NOT LN_USE_UNICODE_CHAR_SET)
	#add_subdirectory(tools/LNSL)
	add_subdirectory(tools/LWIG)
endif()

