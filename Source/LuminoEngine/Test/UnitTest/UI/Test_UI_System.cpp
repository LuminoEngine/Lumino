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
	uiRoot->AddChild(c2_1);

	auto c2_2 = NewObject<UIControl>();
	c2_2->SetSize(Size(32, 32));
	uiRoot->AddChild(c2_2);

	Engine::Update();

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_Image.Basic1.png")));
	uiRoot->RemoveChild(c1);
	uiRoot->RemoveChild(c2);
}

