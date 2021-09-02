#include "Common.hpp"
#include <LuminoCore/Base/Assertion.hpp>

class Test_Base_ErrorHandling : public ::testing::Test {};

static std::u16string g_lastMessage;

static bool TestHandler(Exception& e)
{
	g_lastMessage = e.message();
	return true;
}

//------------------------------------------------------------------------------
TEST_F(Test_Base_ErrorHandling, Message)
{
	auto oldHandler = Exception::notificationHandler();
	Exception::setNotificationHandler(TestHandler);

	//* [ ] LN_ERROR
	{
		LN_ERROR("error!");	// TestHandler は true を返すので継続する
		ASSERT_EQ(u"error!", g_lastMessage);

		// char16_t でも OK
		LN_ERROR(u"error2!");
		ASSERT_EQ(u"error2!", g_lastMessage);

		// fmt が使える
		LN_ERROR(u"error{}!", 100);
		ASSERT_EQ(u"error100!", g_lastMessage);
	}

	Exception::setNotificationHandler(oldHandler);
}
