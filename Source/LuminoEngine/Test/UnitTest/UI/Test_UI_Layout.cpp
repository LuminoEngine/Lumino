#include <TestConfig.h>
#include <Lumino/UI/UILayoutPanel.h>

#if 0
class Test_UI_Button : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_Button, Basic)
{
	auto uiRoot = Engine::GetMainWindow();
	auto button1 = UIButton::Create();
	button1->SetText(_T("Button"));
	uiRoot->AddChild(button1);

	while (Engine::Update())
	{
	}

	uiRoot->RemoveChild(button1);
}
#endif

//==============================================================================
class Test_UI_StackPanel : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, HorizontalAlignment)
{
	auto uiRoot = Engine::GetMainWindow();
	auto panel = UIStackPanel::Create();
	panel->SetSize(uiRoot->GetSize());
	panel->SetBackground(Brush::Blue);
	uiRoot->AddChild(panel);

	auto button1 = UIButton::Create();
	auto button2 = UIButton::Create();
	auto button3 = UIButton::Create();
	auto button4 = UIButton::Create();
	button1->SetHAlignment(HAlignment::Stretch);
	button2->SetHAlignment(HAlignment::Left);
	button3->SetHAlignment(HAlignment::Right);
	button4->SetHAlignment(HAlignment::Center);
	button1->SetSize(Size(32, 16));
	button2->SetSize(Size(32, 16));
	button3->SetSize(Size(32, 16));
	button4->SetSize(Size(32, 16));
	panel->AddChild(button1);
	panel->AddChild(button2);
	panel->AddChild(button3);
	panel->AddChild(button4);

	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.HorizontalAlignment1.png"), 99, true));

	uiRoot->RemoveChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, ReverseHorizontal)
{
	auto panel = UIStackPanel::Create();
	auto button1 = UIButton::Create();
	auto button2 = UIButton::Create();
	button1->SetSize(Size(40, 20));
	button1->margin = ThicknessF(5, 5, 5, 5);
	button2->SetSize(Size(40, 30));
	button2->margin = ThicknessF(5, 5, 5, 5);

	panel->padding = ThicknessF(5, 5, 5, 5);
	panel->AddChild(button1);
	panel->AddChild(button2);
	panel->SetOrientation(Orientation::ReverseHorizontal);

	auto uiRoot = Engine::GetMainWindow();
	uiRoot->AddChild(panel);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.ReverseHorizontal1.png"), 99, true));

	uiRoot->RemoveChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, Margin_Padding)
{
	auto uiRoot = Engine::GetMainWindow();
	auto panel = UIStackPanel::Create();
	panel->SetSize(uiRoot->GetSize());
	uiRoot->AddChild(panel);

	auto button1 = UIButton::Create();
	auto button2 = UIButton::Create();
	button1->SetSize(Size(80, 32));
	button2->SetSize(Size(80, 32));
	button1->SetHAlignment(HAlignment::Left);
	button2->SetHAlignment(HAlignment::Right);
	panel->AddChild(button1);
	panel->AddChild(button2);

	panel->padding = ThicknessF(5, 10, 15, 20);
	button2->margin = ThicknessF(5, 10, 15, 20);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.Margin_Padding1.png")));

	uiRoot->RemoveChild(panel);	// 後始末
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
	auto uiRoot = Engine::GetMainWindow();

	auto grid = UIGridLayout::Create();
	auto button = UIButton::Create();
	button->SetSize(Size(32, 32));
	grid->AddChild(button);
	uiRoot->AddChild(grid);

	tr::WeakRefPtr<UIButton> ref = button;

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Basic1.png")));

	// button の参照を切ってもまだ生きている
	button.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// grid の参照を切ってもまだ生きている
	grid.SafeRelease();
	ASSERT_EQ(true, ref.IsAlive());
	// root からの参照を切るとようやく削除される
	uiRoot->GetItems()->Clear();
	ASSERT_EQ(false, ref.IsAlive());
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, DefaultLayout)
{
	auto uiRoot = Engine::GetMainWindow();

	auto grid1 = UIGridLayout::Create();
	auto button1 = UIButton::Create();
	grid1->AddChild(button1);
	uiRoot->AddChild(grid1);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.DefaultLayout1.png")));

	uiRoot->RemoveChild(grid1);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, Layout)
{
	auto uiRoot = Engine::GetMainWindow();

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

	uiRoot->AddChild(grid1);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Layout1.png")));
	uiRoot->RemoveChild(grid1);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, TreeLayout)
{
	auto uiRoot = Engine::GetMainWindow();

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

	uiRoot->AddChild(grid1);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.TreeLayout1.png")));
	uiRoot->RemoveChild(grid1);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, GridLength)
{
	auto uiRoot = Engine::GetMainWindow();

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


	uiRoot->AddChild(grid1);

	Engine::Update();


	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.GridLength1.png")));
	uiRoot->RemoveChild(grid1);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, MinMax)
{
	auto uiRoot = Engine::GetMainWindow();

	auto grid1 = UIGridLayout::Create();

	uiRoot->AddChild(grid1);
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


	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.MinMax1.png")));
	uiRoot->RemoveChild(grid1);
}

