#include <TestConfig.h>
#include <Lumino/UI/UIPanel.h>

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

class Test_UI_GridLayout : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, Basic)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid = UIGridLayout::Create();
	auto button = UIButton::Create();
	button->SetSize(SizeF(32, 32));
	grid->AddChild(button);
	uiRoot->SetContent(grid);

	tr::WeakRefPtr<UIButton> ref = button;

	while (Engine::UpdateFrame());

	// button ‚ÌŽQÆ‚ðØ‚Á‚Ä‚à‚Ü‚¾¶‚«‚Ä‚¢‚é
	button.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// grid ‚ÌŽQÆ‚ðØ‚Á‚Ä‚à‚Ü‚¾¶‚«‚Ä‚¢‚é
	grid.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// root ‚©‚ç‚ÌŽQÆ‚ðØ‚é‚Æ‚æ‚¤‚â‚­íœ‚³‚ê‚é
	uiRoot->SetContent(nullptr);
	ASSERT_EQ(false, ref.IsAlive());

	//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("TestData/Test_UI_TextBlock.Basic.png")));
}
