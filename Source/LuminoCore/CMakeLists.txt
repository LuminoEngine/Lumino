# e.g.) $ cmake -DLN_EXCEPTION_BACKTRACE=OFF ..

cmake_minimum_required (VERSION 3.0.0)
project (LuminoCore)

include(cmake/LNCommon.cmake NO_POLICY_SCOPE)

set(LN_LINK_LIBS "")
set(LN_UNIT_TEST_LINK_LIBS "")

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------

# Unicode char set.
if (WIN32)
option(LN_USE_UNICODE_CHAR_SET "Use unicode char set." ON)
else()
option(LN_USE_UNICODE_CHAR_SET "Use unicode char set." OFF)
endif()

# MSVC Runtime library.
option(LN_MSVC_STATIC_RUNTIME "static runtime (/MT, /MTd) or DLL runtime (/MD, /MDd) or to link either." ON)

# Exceprion Backtrace.
option(LN_EXCEPTION_BACKTRACE "In some unix environment there is a possibility that can not be compiled." ON)

# Architecture.
#option(LN_BUILD_X64 "Architecture." OFF)

#------------------------------------------------------------------------------
# Files
#------------------------------------------------------------------------------
#--------------------------------------
# Source files.
set(LN_SOURCES_Base
	Include/Lumino/Base/ByteBuffer.h
	Include/Lumino/Base/Cache.h
	Include/Lumino/Base/Collection.h
	Include/Lumino/Base/Common.h
	Include/Lumino/Base/CRT.h
	Include/Lumino/Base/Delegate.h
	Include/Lumino/Base/Delegate.inl
	Include/Lumino/Base/Diff.h
	Include/Lumino/Base/DllLoader.h
	Include/Lumino/Base/ElapsedTimer.h
	Include/Lumino/Base/Enumerable.h
	Include/Lumino/Base/EnumExtension.h
	Include/Lumino/Base/Environment.h
	Include/Lumino/Base/Event.h
	Include/Lumino/Base/Event.inl
	Include/Lumino/Base/Exception.h
	Include/Lumino/Base/Formatter.h
	Include/Lumino/Base/Hash.h
	Include/Lumino/Base/LinkedNodeList.h
	Include/Lumino/Base/List.h
	Include/Lumino/Base/List.inl
	Include/Lumino/Base/Locale.h
	Include/Lumino/Base/Logger.h
	Include/Lumino/Base/Memory.h
	Include/Lumino/Base/Nullable.h
	Include/Lumino/Base/RefObject.h
	Include/Lumino/Base/Regex.h
	Include/Lumino/Base/Resource.h
	Include/Lumino/Base/Resource.h
	Include/Lumino/Base/Serialization.h
	Include/Lumino/Base/Stack.h
	Include/Lumino/Base/StlHelper.h
	Include/Lumino/Base/String.h
	Include/Lumino/Base/StringArray.h
	Include/Lumino/Base/StringBuilder.h
	Include/Lumino/Base/StringRef.h
	Include/Lumino/Base/StringHelper.h
	Include/Lumino/Base/Typedef.h
	Source/Base/ByteBuffer.cpp
	Source/Base/Cache.cpp
	Source/Base/Diff.cpp
	Source/Base/DllLoader.cpp
	Source/Base/ElapsedTimer.cpp
	Source/Base/Environment.cpp
	Source/Base/Exception.cpp
	Source/Base/Hash.cpp
	Source/Base/Locale.cpp
	Source/Base/Logger.cpp
	Source/Base/Memory.cpp
	Source/Base/RefObject.cpp
	Source/Base/Resource.cpp
	Source/Base/Serialization.cpp
	Source/Base/String.cpp
	Source/Base/StringBuilder.cpp
	Source/Base/StringRef.cpp
	Source/Base/StringHelper.cpp
)

set(LN_SOURCES_Text
	Include/Lumino/Text/Encoding.h
	Include/Lumino/Text/EncodingConverter.h
	Include/Lumino/Text/EncodingDetector.h
	Include/Lumino/Text/UnicodeUtils.h
	Source/Text/ASCIIEncoding.cpp
	Source/Text/ASCIIEncoding.h
	Source/Text/ConvertTable_Big5.c
	Source/Text/ConvertTable_EUCJP_EXT.c
	Source/Text/ConvertTable_EUCKR.c
	Source/Text/ConvertTable_GB2312.c
	Source/Text/ConvertTable_SJIS.c
	Source/Text/DBCSEncoding.cpp
	Source/Text/DBCSEncoding.h
	Source/Text/Encoding.cpp
	Source/Text/EncodingConverter.cpp
	Source/Text/EncodingDetector.cpp
	Source/Text/EUCJPEncoding.cpp
	Source/Text/EUCJPEncoding.h
	Source/Text/UnicodeUtils.cpp
	Source/Text/UTF8Encoding.cpp
	Source/Text/UTF8Encoding.h
	Source/Text/UTF16Encoding.cpp
	Source/Text/UTF16Encoding.h
	Source/Text/UTF32Encoding.cpp
	Source/Text/UTF32Encoding.h
)

set(LN_SOURCES_IO
	Include/Lumino/IO/BinaryReader.h
	Include/Lumino/IO/BinaryWriter.h
	Include/Lumino/IO/CommandLine.h
	Include/Lumino/IO/Common.h
	Include/Lumino/IO/Console.h
	Include/Lumino/IO/DirectoryUtils.h
	Include/Lumino/IO/File.h
	Include/Lumino/IO/FileStream.h
	Include/Lumino/IO/FileSystem.h
	Include/Lumino/IO/MemoryStream.h
	Include/Lumino/IO/PathName.h
	Include/Lumino/IO/PathTraits.h
	Include/Lumino/IO/Process.h
	Include/Lumino/IO/Stream.h
	Include/Lumino/IO/StreamReader.h
	Include/Lumino/IO/StreamWriter.h
	Include/Lumino/IO/StringReader.h
	Include/Lumino/IO/StringWriter.h
	Include/Lumino/IO/TextReader.h
	Include/Lumino/IO/TextWriter.h
	Source/IO/BinaryReader.cpp
	Source/IO/BinaryWriter.cpp
	Source/IO/CommandLine.cpp
	Source/IO/Console.cpp
	Source/IO/DirectoryUtils.cpp
	Source/IO/File.cpp
	Source/IO/FileFinder_UNIX.h
	Source/IO/FileFinder_Win32.h
	Source/IO/FileStream.cpp
	Source/IO/FileSystem.cpp
	Source/IO/FileSystem_Unix.h
	Source/IO/FileSystem_Win32.h
	Source/IO/MemoryStream.cpp
	Source/IO/PathName.cpp
	Source/IO/PathTraits.cpp
	Source/IO/Process.cpp
	Source/IO/Process_Unix.h
	Source/IO/Process_Win32.h
	Source/IO/StreamReader.cpp
	Source/IO/StreamWriter.cpp
	Source/IO/StringReader.cpp
	Source/IO/StringWriter.cpp
	Source/IO/TextWriter.cpp
)

set(LN_SOURCES_Json
	Include/Lumino/Json/Common.h
	Include/Lumino/Json/JsonDiagnostics.h
	Include/Lumino/Json/JsonDocument.h
	Include/Lumino/Json/JsonHandler.h
	Include/Lumino/Json/JsonReader.h
	Include/Lumino/Json/JsonValue.h
	Include/Lumino/Json/JsonWriter.h
	Source/Json/JsonDiagnostics.cpp
	Source/Json/JsonDocument.cpp
	Source/Json/JsonHandler.cpp
	Source/Json/JsonReader.cpp
	Source/Json/JsonValue.cpp
	Source/Json/JsonWriter.cpp
)

set(LN_SOURCES_Threading
	Include/Lumino/Threading/AtomicCounter.h
	Include/Lumino/Threading/ConditionFlag.h
	Include/Lumino/Threading/Mutex.h
	Include/Lumino/Threading/ReadWriteMutex.h
	Include/Lumino/Threading/Semaphore.h
	Include/Lumino/Threading/Task.h
	Include/Lumino/Threading/TaskScheduler.h
	Include/Lumino/Threading/Thread.h
	Include/Lumino/Threading/ThreadingExceptions.h
	Source/Threading/AtomicCounter.cpp
	Source/Threading/ConditionFlag.cpp
	Source/Threading/ConditionFlag_POSIX.h
	Source/Threading/ConditionFlag_Win32.h
	Source/Threading/Mutex.cpp
	Source/Threading/Mutex_POSIX.h
	Source/Threading/Mutex_Win32.h
	Source/Threading/ReadWriteMutex.cpp
	Source/Threading/ReadWriteMutex_POSIX.h
	Source/Threading/ReadWriteMutex_Win32.h
	Source/Threading/Semaphore.cpp
	Source/Threading/Semaphore_POSIX.h
	Source/Threading/Semaphore_Win32.h
	Source/Threading/Task.cpp
	Source/Threading/TaskScheduler.cpp
	Source/Threading/Thread.cpp
	Source/Threading/Thread_POSIX.h
	Source/Threading/Thread_Win32.h
	Source/Threading/ThreadingExceptions.cpp
)

set(LN_SOURCES_Xml
	Include/Lumino/Xml/XmlDocument.h
	Include/Lumino/Xml/XmlException.h
	Include/Lumino/Xml/XmlReader.h
	Include/Lumino/Xml/XmlWriter.h
	Source/Xml/XmlDocument.cpp
	Source/Xml/XmlException.cpp
	Source/Xml/XmlReader.cpp
	Source/Xml/XmlWriter.cpp
)

set(LN_SOURCES_Reflection
	Include/Lumino/Reflection/Common.h
	Include/Lumino/Reflection/EventArgsPool.h
	Include/Lumino/Reflection/Notify.h
	Include/Lumino/Reflection/Property.h
	Include/Lumino/Reflection/PropertyMetadata.h
	Include/Lumino/Reflection/ReflectionArrayObject.h
	Include/Lumino/Reflection/ReflectionEventArgs.h
	Include/Lumino/Reflection/ReflectionObject.h
	Include/Lumino/Reflection/ReflectionStruct.h
	Include/Lumino/Reflection/TypeInfo.h
	Include/Lumino/Reflection/Variant.h
	Source/Reflection/Notify.cpp
	Source/Reflection/Property.cpp
	Source/Reflection/ReflectionEventArgs.cpp
	Source/Reflection/ReflectionObject.cpp
	Source/Reflection/TypeInfo.cpp
	Source/Reflection/Variant.cpp
)

set(LN_SOURCES_Math
	Include/LuminoMath.h
	Include/Lumino/Math/Geometries.h
	Include/Lumino/Math/Quaternion.inl
	Include/Lumino/Math/Random.h
	Include/Lumino/Math/AttitudeTransform.h
	Include/Lumino/Math/Vector2.h
	Include/Lumino/Math/Vector2.inl
	Include/Lumino/Math/Vector3.h
	Include/Lumino/Math/Vector3.inl
	Include/Lumino/Math/Vector4.h
	Include/Lumino/Math/Vector4.inl
	Include/Lumino/Math/Common.h
	Include/Lumino/Math/MathUtils.h
	Include/Lumino/Math/Matrix.h
	Include/Lumino/Math/Matrix.inl
	Include/Lumino/Math/Plane.h
	Include/Lumino/Math/Plane.inl
	Include/Lumino/Math/Quaternion.h
	Include/Lumino/Math/Version.h
	Include/Lumino/Math/ViewFrustum.h
	Source/Math/Geometries.cpp
	Source/Math/Random.cpp
	Source/Math/AttitudeTransform.cpp
	Source/Math/Vector2.cpp
	Source/Math/Vector3.cpp
	Source/Math/Vector4.cpp
	Source/Math/MathUtils.cpp
	Source/Math/Matrix.cpp
	Source/Math/Plane.cpp
	Source/Math/Quaternion.cpp
	Source/Math/ViewFrustum.cpp
)

set(LN_SOURCES_Testing
	Include/Lumino/Testing/TestHelper.h
	Source/Testing/TestHelper.cpp
)

#--------------------------------------
# Platform source files.
if (WIN32)
	set(LN_SOURCES_Text
		${LN_SOURCES_Text}
		Source/Text/Win32CodePageEncoding.cpp
		Source/Text/Win32CodePageEncoding.h
	)

elseif (APPLE)

elseif (UNIX)

endif()

#--------------------------------------
# Grouping.
source_group("Base"			FILES ${LN_SOURCES_Base})
source_group("Text"			FILES ${LN_SOURCES_Text})
source_group("IO"			FILES ${LN_SOURCES_IO})
source_group("Json"			FILES ${LN_SOURCES_Json})
source_group("Threading"	FILES ${LN_SOURCES_Threading})
source_group("Xml"			FILES ${LN_SOURCES_Xml})
source_group("Reflection"	FILES ${LN_SOURCES_Reflection})
source_group("Math"			FILES ${LN_SOURCES_Math})
source_group("Testing"		FILES ${LN_SOURCES_Testing})

set(LN_SOURCES
	Include/LuminoCore.h
	Include/LuminoMath.h
	Source/LuminoCore.PCH.cpp
	Source/LuminoCore.PCH.h
	${LN_SOURCES_Base}
	${LN_SOURCES_Text}
	${LN_SOURCES_IO}
	${LN_SOURCES_Json}
	${LN_SOURCES_Threading}
	${LN_SOURCES_Xml}
	${LN_SOURCES_Reflection}
	${LN_SOURCES_Math}
	${LN_SOURCES_Testing})

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
	if (LN_MSVC_STATIC_RUNTIME)
		foreach (flag CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
			if (${flag} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
			endif()
			if (${flag} MATCHES "/MDd")
				string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
			endif()
		endforeach()
	endif()

	#set(CMAKE_DEBUG_POSTFIX "d")
	
	add_definitions("/EHsc")	# VS2015, cmake

	# link librarys.
	list(APPEND LN_LINK_LIBS "imagehlp.lib")

elseif (APPLE)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
	FIND_LIBRARY(CARBON_LIBRARY Carbon)
	list(APPEND LN_LINK_LIBS ${CARBON_LIBRARY})

elseif (UNIX)
	set(LN_UNIT_TEST_LINK_LIBS "pthread")
	add_definitions("-std=c++11")

	# link librarys.
	list(APPEND LN_LINK_LIBS "dl")
	list(APPEND LN_LINK_LIBS "bfd")
	list(APPEND LN_LINK_LIBS "rt")
    find_package(X11 REQUIRED)

    #list(APPEND glfw_INCLUDE_DIRS ${X11_X11_INCLUDE_PATH})
    list(APPEND LN_LINK_LIBS ${X11_X11_LIB})

	#list(APPEND glfw_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list(APPEND LN_LINK_LIBS ${OPENGL_gl_LIBRARY})

else()
	# Not support.
    message(FATAL_ERROR "No supported platform was detected.")
endif()

#------------------------------------------------------------------------------
# Output
#------------------------------------------------------------------------------
if (LN_EXCEPTION_BACKTRACE)
	add_definitions(-DLN_EXCEPTION_BACKTRACE)
endif()

#--------------------------------------
# Static library.
# Core
add_library(${PROJECT_NAME} STATIC ${LN_SOURCES})
ln_add_common_property(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${LN_LINK_LIBS})
ln_add_pch(${PROJECT_NAME} "LuminoCore.PCH.h" "Source/LuminoCore.PCH.cpp")

# Unit test.
add_subdirectory(test)
