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

option(LN_BUILD_AUDIO_MODULE "Build 'Audio' module." ON)
option(LN_BUILD_SCENE_MODULE "Build 'Scene' module." ON)
option(LN_BUILD_C_API "Build 'C API' module." OFF)
option(LN_USE_EXTERNAL_Effekseer "" ON)

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)

option(LN_DxLib_Demo "DX Library demo." OFF)


#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
# Unicode char set.
if (LN_USE_UNICODE_CHAR_SET)
	#add_definitions(-DUNICODE)
	#add_definitions(-D_UNICODE)
endif()

find_package(OpenGL REQUIRED)

if (WIN32)
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

	set(CMAKE_DEBUG_POSTFIX "d")

	# platform include dirs.
	set(LN_PLATFORM_INCLUDE_DIRS "$ENV{DXSDK_DIR}Include")

	# link librarys.
	list(APPEND LN_LINK_LIBS
		"imagehlp.lib"
		"msacm32.lib"
		"Opengl32.lib")


	# effekseer
	link_directories("$ENV{DXSDK_DIR}Lib/x86")

	add_definitions("/EHsc")	# for VS2015 error

elseif (APPLE)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

	find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	list(APPEND LN_LINK_LIBS "${COCOA_FRAMEWORK}"
			  "${OPENGL_gl_LIBRARY}"
			  "${IOKIT_FRAMEWORK}"
			  "${CORE_FOUNDATION_FRAMEWORK}"
			  "${CORE_VIDEO_FRAMEWORK}")

elseif (UNIX)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")

	# link librarys.
	list(APPEND LN_LINK_LIBS "dl")
	list(APPEND LN_LINK_LIBS "bfd")
    find_package(X11 REQUIRED)

    #list(APPEND glfw_INCLUDE_DIRS ${X11_X11_INCLUDE_PATH})
    list(APPEND LN_LINK_LIBS ${X11_X11_LIB})

	#list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND LN_LINK_LIBS ${OPENGL_gl_LIBRARY})

else()
	# Not support.
    message(FATAL_ERROR "No supported platform was detected.")
endif()

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/Source/LuminoEngine/Include
	${PROJECT_SOURCE_DIR}/external
	${PROJECT_SOURCE_DIR}/Source/LuminoCore/Include
	${PROJECT_SOURCE_DIR}/external/libpng
	${PROJECT_SOURCE_DIR}/external/bullet-2.82
	${PROJECT_SOURCE_DIR}/external/freetype-2.6/include
	${PROJECT_SOURCE_DIR}/external/libogg/include
	${PROJECT_SOURCE_DIR}/external/glfw-3.1.2/include
	${PROJECT_SOURCE_DIR}/external/EffekseerRuntime101/Source/LuminoEngine/Source/Effekseer
)
include_directories(${LN_INCLUDE_DIRECTORIES} ${LN_PLATFORM_INCLUDE_DIRS})


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

add_definitions(-DLUMINO_ROOT_DIR="${PROJECT_SOURCE_DIR}")

#set(CMAKE_CXX_FLAGS "")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -pg -O3")
#set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "")

if (LN_EXCEPTION_BACKTRACE)
	add_definitions(-DLN_EXCEPTION_BACKTRACE)
endif()

#------------------------------
# library. (Unicode)

if (LN_USE_UNICODE_CHAR_SET)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()
if (LN_BUILD_AUDIO_MODULE)
	add_definitions(-DLN_BUILD_AUDIO_MODULE)
endif()
if (LN_BUILD_SCENE_MODULE)
	add_definitions(-DLN_BUILD_SCENE_MODULE)
endif()
if (LN_USE_EXTERNAL_Effekseer)
	add_definitions(-DLN_USE_EXTERNAL_Effekseer)
endif()


#------------------------------
# Demo.
if (LN_DxLib_Demo)
	add_subdirectory(demo/DxLib)
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

