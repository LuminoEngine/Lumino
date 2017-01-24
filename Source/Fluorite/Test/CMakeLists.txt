
set(LN_TEST_SOURCES
	gtest/src/gtest-all.cc
	Main.cpp
	TestConfig.h
	Test_CppLexer.cpp
	#Test_Parser_Preprocessor.cpp
	Test_ParserCombinators.cpp
	Test_Rpn.cpp
)

include_directories(
	"${PROJECT_SOURCE_DIR}"
	"${PROJECT_SOURCE_DIR}/Include"
	"${PROJECT_SOURCE_DIR}/test"
	"${PROJECT_SOURCE_DIR}/test/gtest"
	"${PROJECT_SOURCE_DIR}/test/gtest/include"
	"${PROJECT_SOURCE_DIR}/test/gtest/src"
	"${PROJECT_SOURCE_DIR}/../LuminoCore/Include"
)
	
link_libraries(Fluorite)
add_executable(Fluorite_Test ${LN_TEST_SOURCES})
