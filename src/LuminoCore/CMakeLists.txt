cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)

include(${PROJECT_SOURCE_DIR}/../LuminoCommon.cmake NO_POLICY_SCOPE)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#--------------------------------------
# Source files.
set(LN_SOURCES_Base
	include/Lumino/Base/Common.hpp
	include/Lumino/Base/Assertion.hpp
	include/Lumino/Base/Buffer.hpp
	include/Lumino/Base/CRCHash.hpp
	include/Lumino/Base/EnumFlags.hpp
	include/Lumino/Base/Environment.hpp
	include/Lumino/Base/Event.hpp
	include/Lumino/Base/List.hpp
	include/Lumino/Base/List.inl
	include/Lumino/Base/Locale.hpp
	include/Lumino/Base/Logger.hpp
	include/Lumino/Base/Platform.hpp
	include/Lumino/Base/RefObject.hpp
	include/Lumino/Base/StlHelper.hpp
	include/Lumino/Base/String.hpp
	include/Lumino/Base/StringFormat.inl
	include/Lumino/Base/StringHelper.hpp
	src/Base/Internal.hpp
	src/Base/Assertion.cpp
	src/Base/Buffer.cpp
	src/Base/CRCHash.cpp
	src/Base/CRTHelper.h
	src/Base/Environment.cpp
	src/Base/Environment_Linux.hpp
	src/Base/Environment_Mac.hpp
	src/Base/Environment_Win32.hpp
	src/Base/Locale.cpp
	src/Base/Logger.cpp
	src/Base/Platform.cpp
	src/Base/RefObject.cpp
	src/Base/StlHelper.cpp
	src/Base/String.cpp
	src/Base/StringHelper.cpp
)

set(LN_SOURCES_IO
	include/Lumino/IO/Common.hpp
	include/Lumino/IO/FileStream.hpp
	include/Lumino/IO/FileSystem.hpp
	include/Lumino/IO/Path.hpp
	include/Lumino/IO/Stream.hpp
	include/Lumino/IO/StreamReader.hpp
	include/Lumino/IO/StreamWriter.hpp
	include/Lumino/IO/StringReader.hpp
	include/Lumino/IO/StringWriter.hpp
	include/Lumino/IO/TextReader.hpp
	include/Lumino/IO/TextWriter.hpp
	src/IO/Internal.hpp
	src/IO/FileStream.cpp
	src/IO/Path.cpp
	src/IO/FileSystem.cpp
	src/IO/FileSystem_Unix.hpp
	src/IO/FileSystem_Win32.hpp
	src/IO/PathHelper.cpp
	src/IO/PathHelper.hpp
	src/IO/Stream.cpp
	src/IO/StreamReader.cpp
	src/IO/StreamWriter.cpp
	src/IO/StringReader.cpp
	src/IO/StringWriter.cpp
	src/IO/TextReader.cpp
	src/IO/TextWriter.cpp
)

set(LN_SOURCES_Json
	include/Lumino/Json/Common.hpp
	include/Lumino/Json/JsonDiagnostics.h
	include/Lumino/Json/JsonDocument.h
	include/Lumino/Json/JsonReader.h
	include/Lumino/Json/JsonValue.h
	include/Lumino/Json/JsonWriter.hpp
	src/Json/JsonDiagnostics.cpp
	src/Json/JsonDocument.cpp
	src/Json/JsonReader.cpp
	src/Json/JsonValue.cpp
	src/Json/JsonWriter.cpp
)

set(LN_SOURCES_Math
	include/Lumino/Math/Common.hpp
	include/Lumino/Math/Geometries.hpp
	include/Lumino/Math/Random.hpp
	include/Lumino/Math/AttitudeTransform.hpp
	include/Lumino/Math/Vector2.hpp
	include/Lumino/Math/Vector3.hpp
	include/Lumino/Math/Vector4.hpp
	include/Lumino/Math/Math.hpp
	include/Lumino/Math/Matrix.hpp
	include/Lumino/Math/Plane.hpp
	include/Lumino/Math/Quaternion.hpp
	include/Lumino/Math/ViewFrustum.hpp
	src/Math/Internal.hpp
	src/Math/Asm.h
	src/Math/Geometries.cpp
	src/Math/Random.cpp
	src/Math/AttitudeTransform.cpp
	src/Math/Vector2.cpp
	src/Math/Vector3.cpp
	src/Math/Vector4.cpp
	src/Math/Math.cpp
	src/Math/Matrix.cpp
	src/Math/Plane.cpp
	src/Math/Quaternion.cpp
	src/Math/ViewFrustum.cpp
)

set(LN_SOURCES_Serialization
	include/Lumino/Serialization/Common.hpp
	include/Lumino/Serialization/ArchiveStore.hpp
	include/Lumino/Serialization/Serialization.hpp
	include/Lumino/Serialization/SerializeTypes.inl
	src/Serialization/Internal.hpp
	src/Serialization/ArchiveStore.cpp
	src/Serialization/Serialization.cpp
)

set(LN_SOURCES_Testing
	include/Lumino/Testing/Common.hpp
	include/Lumino/Testing/TestHelper.hpp
	src/Testing/Internal.hpp
	src/Testing/TestHelper.cpp
)

set(LN_SOURCES_Text
	include/Lumino/Text/Common.hpp
	include/Lumino/Text/Encoding.hpp
	include/Lumino/Text/EncodingConverter.hpp
	src/Text/Encoding.cpp
	src/Text/EncodingConverter.cpp
	src/Text/UnicodeUtils.cpp
	src/Text/UnicodeUtils.hpp
	src/Text/ASCIIEncoding.cpp
	src/Text/ASCIIEncoding.hpp
	src/Text/UTF8Encoding.cpp
	src/Text/UTF8Encoding.hpp
	src/Text/UTF16Encoding.cpp
	src/Text/UTF16Encoding.hpp
	src/Text/UTF32Encoding.cpp
	src/Text/UTF32Encoding.hpp
)

set(LN_SOURCES_Threading
	include/Lumino/Threading/Common.hpp
)

set(LN_SOURCES_Xml
	include/Lumino/Xml/Common.hpp
)

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Text
		${LN_SOURCES_Text}
		src/Text/Win32CodePageEncoding.cpp
		src/Text/Win32CodePageEncoding.hpp
	)

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
	${PROJECT_SOURCE_DIR}/include)
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
#------------------------------------------------------------------------------

#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)


add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")
ln_add_pch(${PROJECT_NAME} "src/LuminoCore.PCH.h" "src/LuminoCore.PCH.cpp")

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    CXX_STANDARD 11
#    CXX_STANDARD_REQUIRED YES
#    CXX_EXTENSIONS NO
#)

# Unit test.
#if (LN_BUILD_TESTS)
	add_subdirectory(test)
#endif()
