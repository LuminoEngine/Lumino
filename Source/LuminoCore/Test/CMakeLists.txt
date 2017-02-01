set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	UnitTest/Main.cpp
	UnitTest/TestConfig.h
	UnitTest/Base/Test_Base.cpp
	UnitTest/Base/Test_Base_Array.cpp
	UnitTest/Base/Test_Base_ByteBuffer.cpp
	UnitTest/Base/Test_Base_Cache.cpp
	UnitTest/Base/Test_Base_Delegate.cpp
	UnitTest/Base/Test_Base_Diff.cpp
	UnitTest/Base/Test_Base_Encoding.cpp
	UnitTest/Base/Test_Base_Encoding_UTF8.cpp
	UnitTest/Base/Test_Base_Enum.cpp
	UnitTest/Base/Test_Base_Enumerable.cpp
	UnitTest/Base/Test_Base_Environment.cpp
	UnitTest/Base/Test_Base_Event.cpp
	UnitTest/Base/Test_Base_Exception.cpp
	UnitTest/Base/Test_Base_Formatter.cpp
	UnitTest/Base/Test_Base_Locale.cpp
	UnitTest/Base/Test_Base_Regex.cpp
	UnitTest/Base/Test_Base_Resource.cpp
	UnitTest/Base/Test_Base_String.cpp
	UnitTest/Base/Test_Base_StringArray.cpp
	UnitTest/Base/Test_Base_StringRef.cpp
	UnitTest/Base/Test_Base_StringTraits.cpp
	UnitTest/Base/Test_Base_UnicodeUtils.cpp
	UnitTest/Base/Test_Base_UTF32Encoding.cpp
	UnitTest/Base/Test_Base_WeakRefPtr.cpp
	UnitTest/Base/Test_Serialization.cpp
	UnitTest/IO/Test_IO_BinaryReader.cpp
	UnitTest/IO/Test_IO_File.cpp
	UnitTest/IO/Test_IO_FileStream.cpp
	UnitTest/IO/Test_IO_FileSystem.cpp
	UnitTest/IO/Test_IO_MemoryStream.cpp
	UnitTest/IO/Test_IO_PathName.cpp
	UnitTest/IO/Test_IO_PathTraits.cpp
	UnitTest/IO/Test_IO_Process.cpp
	UnitTest/IO/Test_IO_StreamReader.cpp
	UnitTest/IO/Test_IO_TextWriter.cpp
	UnitTest/Json/Test_Json_JsonDocument.cpp
	UnitTest/Json/Test_Json_JsonReader.cpp
	UnitTest/Json/Test_Json_JsonValue.cpp
	UnitTest/Json/Test_Json_JsonWriter.cpp
	UnitTest/Text/Test_Text_DBCSEncoding.cpp
	UnitTest/Text/Test_Text_EncodingConverter.cpp
	UnitTest/Text/Test_Text_EncodingDetector.cpp
	UnitTest/Text/Test_Text_EUCJPEncoding.cpp
	UnitTest/Text/Test_Text_UTF8Encoding.cpp
	UnitTest/Text/Test_Text_UTF16Encoding.cpp
	UnitTest/Text/Test_Text_UTF32Encoding.cpp
	UnitTest/Threading/Test_Threading_Atomic.cpp
	UnitTest/Threading/Test_Threading_EventFlag.cpp
	UnitTest/Threading/Test_Threading_Semaphore.cpp
	UnitTest/Threading/Test_Threading_Task.cpp
	UnitTest/Threading/Test_Threading_Thread.cpp
	UnitTest/Xml/Test_Xml.cpp
	UnitTest/Reflection/Test_Reflection_ReflectionObject.cpp
	UnitTest/Reflection/Test_Reflection_Variant.cpp
	UnitTest/Math/Test_Math.cpp
	UnitTest/Math/Test_Matrix.cpp
	UnitTest/Math/Test_Plane.cpp
	UnitTest/Math/Test_Quaternion.cpp
	UnitTest/Math/Test_AttitudeTransform.cpp
	UnitTest/Math/Test_Vector2.cpp
	UnitTest/Math/Test_Vector3.cpp
	UnitTest/Math/Test_Vector4.cpp
	UnitTest/Math/Test_ViewFrustum.cpp
)

include_directories(
	"${PROJECT_SOURCE_DIR}/Include"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest"
	"${PROJECT_SOURCE_DIR}/test/gtest/src"
	"${PROJECT_SOURCE_DIR}/test/UnitTest"
)

set(LN_TEST_LIBS LuminoCore)
if (WIN32)
elseif (APPLE)
	list(APPEND LN_TEST_LIBS "pthread")	# for gtest.
elseif (UNIX)
	list(APPEND LN_TEST_LIBS "pthread")	# for gtest.
else()
endif()
link_libraries(${LN_TEST_LIBS})

add_definitions(-DGTEST_HAS_TR1_TUPLE=0)	# vs2012

add_executable(LuminoCore_Test ${LN_TEST_SOURCES})
add_dependencies(LuminoCore_Test LuminoCore)

