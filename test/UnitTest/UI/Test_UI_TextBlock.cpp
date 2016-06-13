#include <TestConfig.h>

class Test_UI_TextBlock : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_TextBlock, Basic)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();
	auto textBlock1 = UITextBlock::Create();
	textBlock1->SetText(_T("TextBlock"));
	uiRoot->SetContent(textBlock1);

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::EqualsScreenShot(LN_LOCALFILE("TestData/Test_UI_TextBlock.Basic.png")));
	//TestEnv::SaveScreenShot(LN_LOCALFILE("TestData/Test_UI_TextBlock.Basic.png"));
}

