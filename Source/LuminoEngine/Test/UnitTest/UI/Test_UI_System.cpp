#include <TestConfig.h>

//namespace {

//class TestControl
//	: public UIControl
//{
//public:
//
//};
//
//}
//

//==============================================================================
class Test_UI_System : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

static std::string g_eventLog;

//------------------------------------------------------------------------------
TEST_F(Test_UI_System, focus)
{
	auto uiRoot = Application::getMainWindow();
	uiRoot->setLayoutPanel(UIStackPanel::create());

	auto c1 = newObject<UIControl>();
	c1->setSize(Size(64, 32));
	uiRoot->addChild(c1);

	auto c1_1 = newObject<UIControl>();
	c1_1->setSize(Size(32, 32));
	c1->addChild(c1_1);

	auto c2 = newObject<UIControl>();
	c2->setSize(Size(64, 32));
	uiRoot->addChild(c2);

	auto c2_1 = newObject<UIControl>();
	c2_1->setSize(Size(16, 16));
	c2->addChild(c2_1);

	auto c2_2 = newObject<UIControl>();
	c2_2->setSize(Size(32, 32));
	c2->addChild(c2_2);

	auto c2_3 = newObject<UIControl>();
	c2_3->setSize(Size(48, 32));
	c2_2->addChild(c2_3);

	/*
		c1
			c1_1
		c2
			c2_1
			c2_2
				c2_3
	*/

	c1->connectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c1.got,"); });
	c1->connectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c1.lost,"); });
	c1_1->connectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c1_1.got,"); });
	c1_1->connectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c1_1.lost,"); });

	c2->connectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2.got,"); });
	c2->connectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2.lost,"); });
	c2_1->connectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2_1.got,"); });
	c2_1->connectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2_1.lost,"); });
	c2_2->connectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2_2.got,"); });
	c2_2->connectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2_2.lost,"); });
	c2_3->connectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2_3.got,"); });
	c2_3->connectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2_3.lost,"); });

	g_eventLog.clear();
	c1_1->focus();
	ASSERT_EQ("c1_1.got,c1.got,", g_eventLog);
	ASSERT_EQ(true, uiRoot->hasFocus());	// ルートもついでにチェック

	g_eventLog.clear();
	c2_1->focus();
	ASSERT_EQ("c1_1.lost,c1.lost,c2_1.got,c2.got,", g_eventLog);

	g_eventLog.clear();
	c2_3->focus();
	ASSERT_EQ("c2_1.lost,c2_3.got,c2_2.got,", g_eventLog);

	
	//std::cout << g_eventLog << std::endl;

	Engine::update();

	uiRoot->removeChild(c1);
	uiRoot->removeChild(c2);
}

