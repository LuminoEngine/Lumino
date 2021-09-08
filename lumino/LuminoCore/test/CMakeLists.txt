cmake_minimum_required (VERSION 3.0.0)
project(LuminoCore-test)

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------
set(LN_SOURCES
	Common.hpp
	Main.cpp
	Test_Base_Assertion.cpp
	Test_Base_ByteBuffer.cpp
	Test_Base_EnumFlags.cpp
	Test_Base_Environment.cpp
	Test_Base_Formatter.cpp
	Test_Base_List.cpp
	Test_Base_Logger.cpp
	Test_Base_Optional.cpp
	Test_Base_RefObject.cpp
	Test_Base_String.cpp
	Test_Base_StringRef.cpp
	Test_IO_BinaryReader.cpp
	Test_IO_CommandLineParser.cpp
	Test_IO_FileStream.cpp
	Test_IO_FileSystem.cpp
	Test_IO_Path.cpp
	Test_IO_Process.cpp
	Test_IO_StreamReader.cpp
	Test_IO_TextWriter.cpp
	Test_Json_JsonReader.cpp
	Test_Json_JsonWriter.cpp
	Test_Math_Common.hpp
	Test_Math.cpp
	Test_Math_Matrix.cpp
	Test_Math_Plane.cpp
	Test_Math_Quaternion.cpp
	Test_Math_AttitudeTransform.cpp
	Test_Math_Vector2.cpp
	Test_Math_Vector3.cpp
	Test_Math_Vector4.cpp
	Test_Math_ViewFrustum.cpp
	Test_Serialization_Archive.cpp
	Test_Text_Encoding.cpp
	Test_Text_EncodingConverter.cpp
)

#-------------------------------------------------------------------------------
# Targets
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${LN_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE LuminoCore GTest::gtest GTest::gtest_main "-framework Foundation")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

if (DEFINED EMSCRIPTEN)
	em_link_pre_js(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/pre.js)
endif()

add_test(NAME LuminoCore-test-all COMMAND $<TARGET_FILE:LuminoCore-test>)
