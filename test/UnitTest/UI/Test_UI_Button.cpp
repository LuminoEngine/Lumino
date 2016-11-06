#include <TestConfig.h>
#include <Lumino/UI/UILayoutPanel.h>

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

//==============================================================================
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
	button->SetSize(Size(32, 32));
	grid->AddChild(button);
	uiRoot->SetContent(grid);

	tr::WeakRefPtr<UIButton> ref = button;

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Basic1.png")));

	// button ‚ÌŽQÆ‚ðØ‚Á‚Ä‚à‚Ü‚¾¶‚«‚Ä‚¢‚é
	button.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// grid ‚ÌŽQÆ‚ðØ‚Á‚Ä‚à‚Ü‚¾¶‚«‚Ä‚¢‚é
	grid.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// root ‚©‚ç‚ÌŽQÆ‚ðØ‚é‚Æ‚æ‚¤‚â‚­íœ‚³‚ê‚é
	uiRoot->SetContent(nullptr);
	ASSERT_EQ(false, ref.IsAlive());
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, DefaultLayout)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid1 = UIGridLayout::Create();
	auto button1 = UIButton::Create();
	grid1->AddChild(button1);
	uiRoot->SetContent(grid1);

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.DefaultLayout1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, Layout)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid1 = UIGridLayout::Create(4, 4);

	auto button1 = UIButton::Create();
	grid1->AddChild(button1);

	auto button2 = UIButton::Create();
	button2->SetLayoutColumn(1);
	grid1->AddChild(button2);

	auto button3 = UIButton::Create();
	button3->SetLayoutRow(1);
	grid1->AddChild(button3);

	auto button4 = UIButton::Create();
	button4->SetLayoutColumn(1);
	button4->SetLayoutRow(1);
	grid1->AddChild(button4);

	// column span
	auto button5 = UIButton::Create();
	button5->SetLayoutColumn(2);
	button5->SetLayoutColumnSpan(2);
	grid1->AddChild(button5);

	auto button6 = UIButton::Create();
	button6->SetLayoutColumn(2);
	button6->SetLayoutRow(1);
	button6->SetLayoutColumnSpan(2);
	grid1->AddChild(button6);

	// row span
	auto button7 = UIButton::Create();
	button7->SetLayoutRow(2);
	button7->SetLayoutRowSpan(2);
	grid1->AddChild(button7);

	auto button8 = UIButton::Create();
	button8->SetLayoutColumn(1);
	button8->SetLayoutRow(2);
	button8->SetLayoutRowSpan(2);
	grid1->AddChild(button8);

	// column and row span
	auto button9 = UIButton::Create();
	button9->SetLayoutColumn(2);
	button9->SetLayoutColumnSpan(2);
	button9->SetLayoutRow(2);
	button9->SetLayoutRowSpan(2);
	grid1->AddChild(button9);

	uiRoot->SetContent(grid1);

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Layout1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, TreeLayout)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid1 = UIGridLayout::Create(2, 2);

	UIGridLayoutPtr grids[3];
	for (int i = 0; i < 3; ++i)
	{
		grids[i] = UIGridLayout::Create(2, 2);

		auto button1 = UIButton::Create();
		grids[i]->AddChild(button1);

		auto button2 = UIButton::Create();
		button2->SetLayoutColumn(1);
		grids[i]->AddChild(button2);

		auto button3 = UIButton::Create();
		button3->SetLayoutRow(1);
		grids[i]->AddChild(button3);

		auto button4 = UIButton::Create();
		button4->SetLayoutColumn(1);
		button4->SetLayoutRow(1);
		grids[i]->AddChild(button4);

		grid1->AddChild(grids[i]);
	}
	grids[1]->SetLayoutRow(1);
	grids[2]->SetLayoutColumn(1);
	grids[2]->SetLayoutRowSpan(2);

	uiRoot->SetContent(grid1);

	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.TreeLayout1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, GridLength)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid1 = UIGridLayout::Create();
	grid1->AddColumnDefinition();	// default
	grid1->AddColumnDefinition(GridLengthType::Pixel, 50);
	grid1->AddColumnDefinition(GridLengthType::Auto);
	grid1->AddColumnDefinition(GridLengthType::Ratio, 2.0f);
	grid1->AddRowDefinition();		// default
	grid1->AddRowDefinition(GridLengthType::Pixel, 50);
	grid1->AddRowDefinition(GridLengthType::Auto);
	grid1->AddRowDefinition(GridLengthType::Ratio, 2.0f);

	auto button1 = UIButton::Create();
	button1->SetLayoutColumn(0);
	button1->SetLayoutRow(0);
	grid1->AddChild(button1);

	auto button2 = UIButton::Create();
	button2->SetLayoutColumn(1);
	button2->SetLayoutRow(1);
	grid1->AddChild(button2);

	auto button3 = UIButton::Create();
	button3->SetSize(Size(20, 20));
	button3->SetLayoutColumn(2);
	button3->SetLayoutRow(2);
	grid1->AddChild(button3);

	auto button4 = UIButton::Create();
	button4->SetLayoutColumn(3);
	button4->SetLayoutRow(3);
	grid1->AddChild(button4);


	uiRoot->SetContent(grid1);

	Engine::UpdateFrame();


	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.GridLength1.png")));
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, MinMax)
{
	auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();

	auto grid1 = UIGridLayout::Create();

	uiRoot->SetContent(grid1);
	grid1->AddColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);


	auto button1 = UIButton::Create();
	button1->SetLayoutColumn(0);
	button1->SetLayoutRow(0);
	button1->SetSize(Size(10, 10));
	button1->SetHAlignment(HAlignment::Left);
	button1->SetVAlignment(VAlignment::Top);
	grid1->AddChild(button1);

	auto button2 = UIButton::Create();
	button2->SetLayoutColumn(1);
	button2->SetLayoutRow(1);
	button2->SetSize(Size(40, 40));
	button2->SetHAlignment(HAlignment::Left);
	button2->SetVAlignment(VAlignment::Top);
	grid1->AddChild(button2);

	auto button3 = UIButton::Create();
	button3->SetLayoutColumn(2);
	button3->SetLayoutRow(2);
	button3->SetSize(Size(10, 10));
	button3->SetHAlignment(HAlignment::Left);
	button3->SetVAlignment(VAlignment::Top);
	grid1->AddChild(button3);


	Engine::UpdateFrame();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.MinMax1.png"), 99, true));
}

