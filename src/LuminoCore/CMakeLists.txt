cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)

#------------------------------------------------------------------------------
# Files

#--------------------------------------
# Source files.
set(LN_SOURCES_Base
	include/Lumino/Base/Common.hpp
	src/Base/Internal.hpp
)

set(LN_SOURCES_IO
	include/Lumino/IO/Common.hpp
	src/IO/Internal.hpp
)

set(LN_SOURCES_Json
	include/Lumino/Json/Common.hpp
	src/Json/Internal.hpp
)

set(LN_SOURCES_Math
	include/Lumino/Math/Common.hpp
	src/Math/Internal.hpp
)

set(LN_SOURCES_Serialization
	include/Lumino/Serialization/Common.hpp
	src/Serialization/Internal.hpp
)

set(LN_SOURCES_Testing
	include/Lumino/Testing/Common.hpp
	src/Testing/Internal.hpp
)

set(LN_SOURCES_Text
	include/Lumino/Text/Common.hpp
	src/Text/Internal.hpp
)

set(LN_SOURCES_Threading
	include/Lumino/Threading/Common.hpp
	src/Threading/Internal.hpp
)

set(LN_SOURCES_Xml
	include/Lumino/Xml/Common.hpp
	src/Xml/Internal.hpp
)

#--------------------------------------
# Platform source files.
if (WIN32)

elseif (APPLE)

elseif (UNIX)

endif()

#--------------------------------------
# Grouping.
source_group("Base"				FILES ${LN_SOURCES_Base})
source_group("Text"				FILES ${LN_SOURCES_Text})
source_group("IO"				FILES ${LN_SOURCES_IO})
source_group("Json"				FILES ${LN_SOURCES_Json})
source_group("Threading"		FILES ${LN_SOURCES_Threading})
source_group("Xml"				FILES ${LN_SOURCES_Xml})
source_group("Math"				FILES ${LN_SOURCES_Math})
source_group("Serialization"	FILES ${LN_SOURCES_Serialization})
source_group("Testing"			FILES ${LN_SOURCES_Testing})

set(LN_SOURCES
	src/LuminoCore.PCH.cpp
	src/LuminoCore.PCH.h
	${LN_SOURCES_Base}
	${LN_SOURCES_Text}
	${LN_SOURCES_IO}
	${LN_SOURCES_Json}
	${LN_SOURCES_Threading}
	${LN_SOURCES_Xml}
	${LN_SOURCES_Math}
	${LN_SOURCES_Serialization}
	${LN_SOURCES_Testing})

set(LN_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/Include)
include_directories(${LN_INCLUDE_DIRECTORIES})

#------------------------------------------------------------------------------
# Apply options
#------------------------------------------------------------------------------
set(LN_LINK_LIBS "")

if (WIN32)
	list(APPEND LN_LINK_LIBS "imagehlp.lib")
	list(APPEND LN_LINK_LIBS "Shlwapi.lib")

elseif (APPLE)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	
elseif (UNIX)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	
endif()

#------------------------------------------------------------------------------
# Targets

add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoCore.PCH.h" "src/LuminoCore.PCH.cpp")

# Unit test.
if (LN_BUILD_TESTS)
	add_subdirectory(test)
endif()
