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
	auto button1 = UIButton::create();
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
	auto panel = UIStackPanel::create();
	panel->SetOrientation(Orientation::Vertical);
	panel->SetSize(uiRoot->getSize());
	panel->SetBackground(Brush::Blue);
	uiRoot->AddChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	auto button3 = UIButton::create();
	auto button4 = UIButton::create();
	auto button5 = UIButton::create();
	button1->SetHAlignment(HAlignment::Stretch);
	button2->SetHAlignment(HAlignment::Left);
	button3->SetHAlignment(HAlignment::Right);
	button4->SetHAlignment(HAlignment::Center);
	button5->SetHAlignment(HAlignment::Stretch);
	button1->SetHeight(16);
	button2->SetSize(Size(32, 16));
	button3->SetSize(Size(32, 16));
	button4->SetSize(Size(32, 16));
	button5->SetSize(Size(32, 16));
	panel->AddChild(button1);
	panel->AddChild(button2);
	panel->AddChild(button3);
	panel->AddChild(button4);
	panel->AddChild(button5);

	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.HorizontalAlignment1.png")));

	uiRoot->RemoveChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, VerticalAlignment)
{
	auto uiRoot = Engine::GetMainWindow();
	auto panel = UIStackPanel::create();
	panel->SetOrientation(Orientation::Horizontal);
	panel->SetSize(uiRoot->getSize());
	panel->SetBackground(Brush::Blue);
	uiRoot->AddChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	auto button3 = UIButton::create();
	auto button4 = UIButton::create();
	auto button5 = UIButton::create();
	button1->SetVAlignment(VAlignment::Stretch);
	button2->SetVAlignment(VAlignment::Top);
	button3->SetVAlignment(VAlignment::Bottom);
	button4->SetVAlignment(VAlignment::Center);
	button5->SetVAlignment(VAlignment::Stretch);
	button1->SetWidth(16);
	button2->SetSize(Size(16, 32));
	button3->SetSize(Size(16, 32));
	button4->SetSize(Size(16, 32));
	button5->SetSize(Size(16, 32));
	panel->AddChild(button1);
	panel->AddChild(button2);
	panel->AddChild(button3);
	panel->AddChild(button4);
	panel->AddChild(button5);

	Engine::Update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.VerticalAlignment1.png")));

	uiRoot->RemoveChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, ReverseHorizontal)
{
	auto uiRoot = Engine::GetMainWindow();
	auto panel = UIStackPanel::create();
	panel->SetSize(uiRoot->getSize());
	panel->SetBackground(Brush::Blue);
	panel->SetOrientation(Orientation::ReverseHorizontal);
	uiRoot->AddChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	button1->SetSize(Size(20, 20));
	button2->SetSize(Size(30, 30));
	panel->AddChild(button1);
	panel->AddChild(button2);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.ReverseHorizontal1.png")));

	uiRoot->RemoveChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, Margin_Padding)
{
	auto uiRoot = Engine::GetMainWindow();
	auto panel = UIStackPanel::create();
	panel->SetSize(uiRoot->getSize());
	panel->SetBackground(Brush::Blue);
	uiRoot->AddChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	button1->SetHeight(16);
	button2->SetHeight(16);
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
	// <Test> インスタンス参照の関係
	{
		auto uiRoot = Engine::GetMainWindow();

		auto grid = UIGridLayout::create();
		grid->SetBackground(Brush::Blue);
		uiRoot->AddChild(grid);

		auto button = UIButton::create();
		button->SetSize(Size(32, 32));
		grid->AddChild(button);

		tr::WeakRefPtr<UIButton> ref = button;
		UIGridLayout* gridPtr = grid;

		Engine::Update();
		TestEnv::WaitRendering();

		// button の参照を切ってもまだ生きている
		button.safeRelease();
		ASSERT_EQ(true, ref.IsAlive());
		// grid の参照を切ってもまだ生きている
		grid.safeRelease();
		ASSERT_EQ(true, ref.IsAlive());
		// root からの参照を切るとようやく削除される
		uiRoot->RemoveChild(gridPtr);
		ASSERT_EQ(false, ref.IsAlive());
	}

	// <Test> GridLayout を SetLayoutPanel() で追加する場合、デフォルトでは画面いっぱいに広がる。
	//        これは、オーナーの UIControl からそのサイズ (MainWindow ならクライアント領域サイズ) が与えられ、
	//        また GridLayout(UIElement) のデフォルトの Alignment が Stretch であるため。
	{
		auto uiRoot = Engine::GetMainWindow();
		RefPtr<UILayoutPanel> oldLayot = uiRoot->GetLayoutPanel();

		auto grid = UIGridLayout::create();
		grid->SetBackground(Brush::Blue);
		uiRoot->SetLayoutPanel(grid);

		auto button = UIButton::create();
		button->SetSize(Size(32, 32));
		uiRoot->AddChild(button);

		Engine::Update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Basic1.png")));
		uiRoot->SetLayoutPanel(oldLayot);
	}

	// <Test> GridLayout を AddChild() で追加する場合、サイズは 子要素の DesirdSize となり、左上に配置される。
	//        これは、オーナーが AnchorLayout であり、デフォルト(アンカーなし)左上詰めであるため。
	//        なお、AnchorLayout はアンカーなし子要素のサイズ調整は行わない。そのため Alignment が Stretch であっても DesirdSize となる。
	{
		auto uiRoot = Engine::GetMainWindow();

		auto grid = UIGridLayout::create();
		grid->SetBackground(Brush::Blue);
		uiRoot->AddChild(grid);

		auto button = UIButton::create();
		button->SetSize(Size(32, 32));
		grid->AddChild(button);

		Engine::Update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Basic2.png"), 99, true));
		uiRoot->ClearChildren();
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, DefaultLayout)
{
	// <Test> GridLayout へサイズ指定無しの子要素を追加する場合、UIElement のデフォルトの Alignment(Stretch) に従い引き伸ばされる。
	{
		auto uiRoot = Engine::GetMainWindow();
		RefPtr<UILayoutPanel> oldLayot = uiRoot->GetLayoutPanel();

		auto grid1 = UIGridLayout::create();
		uiRoot->SetLayoutPanel(grid1);

		auto button1 = UIButton::create();
		uiRoot->AddChild(button1);

		Engine::Update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.DefaultLayout1.png")));
		uiRoot->ClearChildren();
		uiRoot->SetLayoutPanel(oldLayot);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, Layout)
{
	auto uiRoot = Engine::GetMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->GetLayoutPanel();

	auto grid1 = UIGridLayout::create(4, 4);
	uiRoot->SetLayoutPanel(grid1);

	auto button1 = UIButton::create();
	uiRoot->AddChild(button1);

	auto button2 = UIButton::create();
	button2->SetLayoutColumn(1);
	uiRoot->AddChild(button2);

	auto button3 = UIButton::create();
	button3->SetLayoutRow(1);
	uiRoot->AddChild(button3);

	auto button4 = UIButton::create();
	button4->SetLayoutColumn(1);
	button4->SetLayoutRow(1);
	uiRoot->AddChild(button4);

	// column span
	auto button5 = UIButton::create();
	button5->SetLayoutColumn(2);
	button5->SetLayoutColumnSpan(2);
	uiRoot->AddChild(button5);

	auto button6 = UIButton::create();
	button6->SetLayoutColumn(2);
	button6->SetLayoutRow(1);
	button6->SetLayoutColumnSpan(2);
	uiRoot->AddChild(button6);

	// row span
	auto button7 = UIButton::create();
	button7->SetLayoutRow(2);
	button7->SetLayoutRowSpan(2);
	uiRoot->AddChild(button7);

	auto button8 = UIButton::create();
	button8->SetLayoutColumn(1);
	button8->SetLayoutRow(2);
	button8->SetLayoutRowSpan(2);
	uiRoot->AddChild(button8);

	// column and row span
	auto button9 = UIButton::create();
	button9->SetLayoutColumn(2);
	button9->SetLayoutColumnSpan(2);
	button9->SetLayoutRow(2);
	button9->SetLayoutRowSpan(2);
	uiRoot->AddChild(button9);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Layout1.png")));
	uiRoot->ClearChildren();
	uiRoot->SetLayoutPanel(oldLayot);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, TreeLayout)
{
	auto uiRoot = Engine::GetMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->GetLayoutPanel();

	auto grid1 = UIGridLayout::create(2, 2);
	uiRoot->SetLayoutPanel(grid1);

	UIGridLayoutPtr grids[3];
	for (int i = 0; i < 3; ++i)
	{
		grids[i] = UIGridLayout::create(2, 2);

		auto button1 = UIButton::create();
		grids[i]->AddChild(button1);

		auto button2 = UIButton::create();
		button2->SetLayoutColumn(1);
		grids[i]->AddChild(button2);

		auto button3 = UIButton::create();
		button3->SetLayoutRow(1);
		grids[i]->AddChild(button3);

		auto button4 = UIButton::create();
		button4->SetLayoutColumn(1);
		button4->SetLayoutRow(1);
		grids[i]->AddChild(button4);

		uiRoot->AddChild(grids[i]);
	}
	grids[1]->SetLayoutRow(1);
	grids[2]->SetLayoutColumn(1);
	grids[2]->SetLayoutRowSpan(2);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.TreeLayout1.png")));
	uiRoot->ClearChildren();
	uiRoot->SetLayoutPanel(oldLayot);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, GridLength)
{
	auto uiRoot = Engine::GetMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->GetLayoutPanel();

	auto grid1 = UIGridLayout::create();
	uiRoot->SetLayoutPanel(grid1);
	grid1->AddColumnDefinition();	// default
	grid1->AddColumnDefinition(GridLengthType::Pixel, 50);
	grid1->AddColumnDefinition(GridLengthType::Auto);
	grid1->AddColumnDefinition(GridLengthType::Ratio, 2.0f);
	grid1->AddRowDefinition();		// default
	grid1->AddRowDefinition(GridLengthType::Pixel, 50);
	grid1->AddRowDefinition(GridLengthType::Auto);
	grid1->AddRowDefinition(GridLengthType::Ratio, 2.0f);

	auto button1 = UIButton::create();
	button1->SetLayoutColumn(0);
	button1->SetLayoutRow(0);
	uiRoot->AddChild(button1);

	auto button2 = UIButton::create();
	button2->SetLayoutColumn(1);
	button2->SetLayoutRow(1);
	uiRoot->AddChild(button2);

	auto button3 = UIButton::create();
	button3->SetSize(Size(20, 20));
	button3->SetLayoutColumn(2);
	button3->SetLayoutRow(2);
	uiRoot->AddChild(button3);

	auto button4 = UIButton::create();
	button4->SetLayoutColumn(3);
	button4->SetLayoutRow(3);
	uiRoot->AddChild(button4);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.GridLength1.png")));
	uiRoot->ClearChildren();
	uiRoot->SetLayoutPanel(oldLayot);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, MinMax)
{
	auto uiRoot = Engine::GetMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->GetLayoutPanel();

	auto grid1 = UIGridLayout::create();
	uiRoot->SetLayoutPanel(grid1);
	grid1->AddColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->AddRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);

	auto button1 = UIButton::create();
	button1->SetLayoutColumn(0);
	button1->SetLayoutRow(0);
	button1->SetSize(Size(10, 10));
	button1->SetHAlignment(HAlignment::Left);
	button1->SetVAlignment(VAlignment::Top);
	uiRoot->AddChild(button1);

	auto button2 = UIButton::create();
	button2->SetLayoutColumn(1);
	button2->SetLayoutRow(1);
	button2->SetSize(Size(40, 40));
	button2->SetHAlignment(HAlignment::Left);
	button2->SetVAlignment(VAlignment::Top);
	uiRoot->AddChild(button2);

	auto button3 = UIButton::create();
	button3->SetLayoutColumn(2);
	button3->SetLayoutRow(2);
	button3->SetSize(Size(10, 10));
	button3->SetHAlignment(HAlignment::Left);
	button3->SetVAlignment(VAlignment::Top);
	uiRoot->AddChild(button3);

	Engine::Update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.MinMax1.png")));
	uiRoot->ClearChildren();
	uiRoot->SetLayoutPanel(oldLayot);
}

