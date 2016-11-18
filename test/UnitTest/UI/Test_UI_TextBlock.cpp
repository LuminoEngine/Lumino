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
	//auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();
	//auto textBlock1 = UITextBlock::Create();
	//textBlock1->SetText(_T("TextBlock"));
	//uiRoot->SetContent(textBlock1);

	//while (Engine::Update());

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_UI_TextBlock.Basic.png")));
}

