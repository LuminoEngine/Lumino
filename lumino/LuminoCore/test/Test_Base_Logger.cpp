#include "Common.hpp"
#include <LuminoCore/Base/String.hpp>
#include <LuminoCore/Base/Logger.hpp>
#include <LuminoCore/IO/Path.hpp>

//==============================================================================
//# Logger
class Test_Base_Logger : public ::testing::Test { };

//## usage
TEST_F(Test_Base_Logger, Basic)
{
	LN_LOG_INFO("test1");
	LN_LOG_INFO("test1 {}", 1);
	LN_LOG_INFO(_TT("test2"));
	LN_LOG_INFO(String(_TT("test2")));
	LN_LOG_INFO(StringRef(_TT("test3")));
	LN_LOG_INFO(Path(_TT("test4")));
}
