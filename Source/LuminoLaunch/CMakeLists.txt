cmake_minimum_required (VERSION 3.0.0)
project (LuminoLaunch)

include(../LuminoCore/CMake/LNCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#--------------------------------------
# Source files.
set(LN_SOURCES_Launch
	Source/EntryPoint.cpp
	Source/EntryPoint.h
)

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Launch
		${LN_SOURCES_Launch}
		Source/Main_Win32.cpp
	)

elseif (APPLE)
	set(LN_SOURCES_Launch
		${Main_Mac}
		Source/Main_Win32.cpp
	)

elseif (UNIX)

endif()

#--------------------------------------
# Grouping.
source_group("Launch"		FILES ${LN_SOURCES_Launch})

set(LN_SOURCES
	Source/LuminoLaunch.PCH.cpp
	Source/LuminoLaunch.PCH.h
	${LN_SOURCES_Launch}
)

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/include)
include_directories(${LN_INCLUDE_DIRECTORIES})

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
# Unicode char set.
if (LN_USE_UNICODE_CHAR_SET)
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

if (WIN32)

elseif (APPLE)
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

elseif (UNIX)
	add_definitions("-std=c++11")

else()
	# Not support.
    message(FATAL_ERROR "No supported platform was detected.")
endif()

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------

include_directories(
	${PROJECT_SOURCE_DIR}/../LuminoCore/Include
	${PROJECT_SOURCE_DIR}/../LuminoEngine/Include
)

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
ln_add_common_property(LuminoEngine)
ln_add_pch(${PROJECT_NAME} "LuminoLaunch.PCH.h" "Source/LuminoLaunch.PCH.cpp")

