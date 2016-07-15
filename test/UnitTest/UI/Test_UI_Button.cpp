#include <TestConfig.h>

class Test_UI_Button : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_Button, Basic)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();
	auto button1 = UIButton::Create();
	button1->SetText(_T("Button"));
	uiRoot->SetContent(button1);

	while (Engine::UpdateFrame());


}

