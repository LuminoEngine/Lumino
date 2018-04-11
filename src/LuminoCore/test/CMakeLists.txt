cmake_minimum_required (VERSION 3.0.0)
project(LuminoCore_Test)

set(LN_SOURCES
	${LN_SOURCES_gtest}
	Common.hpp
	Main.cpp
	Test_Base_EnumFlags.cpp
	Test_Base_Event.cpp
	Test_Base_Formatter.cpp
	Test_Base_Logger.cpp
	Test_Base_String.cpp
	Test_IO_FileSystem.cpp
	Test_IO_Path.cpp
	Test_Text_Encoding.cpp
)

include_directories(
	${LN_INCLUDES_gtest}
	${LUMINO_INCLUDE_DIRECTORIES}
)


add_executable(LuminoCore_Test ${LN_SOURCES})
target_link_libraries(${PROJECT_NAME} LuminoCore)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Lumino")

#em_link_pre_js(LuminoCore_Test ${CMAKE_CURRENT_SOURCE_DIR}/pre.js)


#set_target_properties(${PROJECT_NAME} PROPERTIES
#    CXX_STANDARD 11
#    CXX_STANDARD_REQUIRED YES
#    CXX_EXTENSIONS NO
#)
#message("Supported features = ${CMAKE_CXX_COMPILE_FEATURES}")
