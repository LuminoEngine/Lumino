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
TEST_F(Test_UI_System, Focus)
{
	auto uiRoot = Engine::GetMainWindow();
	uiRoot->SetLayoutPanel(UIStackPanel::Create());

	auto c1 = NewObject<UIControl>();
	c1->SetSize(Size(64, 32));
	uiRoot->AddChild(c1);

	auto c1_1 = NewObject<UIControl>();
	c1_1->SetSize(Size(32, 32));
	c1->AddChild(c1_1);

	auto c2 = NewObject<UIControl>();
	c2->SetSize(Size(64, 32));
	uiRoot->AddChild(c2);

	auto c2_1 = NewObject<UIControl>();
	c2_1->SetSize(Size(16, 16));
	c2->AddChild(c2_1);

	auto c2_2 = NewObject<UIControl>();
	c2_2->SetSize(Size(32, 32));
	c2->AddChild(c2_2);

	auto c2_3 = NewObject<UIControl>();
	c2_3->SetSize(Size(48, 32));
	c2_2->AddChild(c2_3);

	/*
		c1
			c1_1
		c2
			c2_1
			c2_2
				c2_3
	*/

	c1->ConnectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c1.got,"); });
	c1->ConnectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c1.lost,"); });
	c1_1->ConnectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c1_1.got,"); });
	c1_1->ConnectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c1_1.lost,"); });

	c2->ConnectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2.got,"); });
	c2->ConnectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2.lost,"); });
	c2_1->ConnectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2_1.got,"); });
	c2_1->ConnectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2_1.lost,"); });
	c2_2->ConnectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2_2.got,"); });
	c2_2->ConnectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2_2.lost,"); });
	c2_3->ConnectOnGotFocus([](UIEventArgs* e) { g_eventLog += ("c2_3.got,"); });
	c2_3->ConnectOnLostFocus([](UIEventArgs* e) { g_eventLog += ("c2_3.lost,"); });

	g_eventLog.clear();
	c1_1->Focus();
	ASSERT_EQ("c1_1.got,c1.got,", g_eventLog);
	ASSERT_EQ(true, uiRoot->HasFocus());	// ルートもついでにチェック

	g_eventLog.clear();
	c2_1->Focus();
	ASSERT_EQ("c1_1.lost,c1.lost,c2_1.got,c2.got,", g_eventLog);

	g_eventLog.clear();
	c2_3->Focus();
	ASSERT_EQ("c2_1.lost,c2_3.got,c2_2.got,", g_eventLog);

	
	//std::cout << g_eventLog << std::endl;

	Engine::Update();

	uiRoot->RemoveChild(c1);
	uiRoot->RemoveChild(c2);
}

