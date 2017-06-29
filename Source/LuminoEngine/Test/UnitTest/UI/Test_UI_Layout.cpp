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
	auto uiRoot = Application::getMainWindow();
	auto panel = UIStackPanel::create();
	panel->setOrientation(Orientation::Vertical);
	panel->setSize(uiRoot->getSize());
	panel->setBackground(Brush::Blue);
	uiRoot->addChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	auto button3 = UIButton::create();
	auto button4 = UIButton::create();
	auto button5 = UIButton::create();
	button1->setHAlignment(HAlignment::Stretch);
	button2->setHAlignment(HAlignment::Left);
	button3->setHAlignment(HAlignment::Right);
	button4->setHAlignment(HAlignment::Center);
	button5->setHAlignment(HAlignment::Stretch);
	button1->setHeight(16);
	button2->setSize(Size(32, 16));
	button3->setSize(Size(32, 16));
	button4->setSize(Size(32, 16));
	button5->setSize(Size(32, 16));
	panel->addChild(button1);
	panel->addChild(button2);
	panel->addChild(button3);
	panel->addChild(button4);
	panel->addChild(button5);

	Engine::update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.HorizontalAlignment1.png")));

	uiRoot->removeChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, VerticalAlignment)
{
	auto uiRoot = Application::getMainWindow();
	auto panel = UIStackPanel::create();
	panel->setOrientation(Orientation::Horizontal);
	panel->setSize(uiRoot->getSize());
	panel->setBackground(Brush::Blue);
	uiRoot->addChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	auto button3 = UIButton::create();
	auto button4 = UIButton::create();
	auto button5 = UIButton::create();
	button1->setVAlignment(VAlignment::Stretch);
	button2->setVAlignment(VAlignment::Top);
	button3->setVAlignment(VAlignment::Bottom);
	button4->setVAlignment(VAlignment::Center);
	button5->setVAlignment(VAlignment::Stretch);
	button1->setWidth(16);
	button2->setSize(Size(16, 32));
	button3->setSize(Size(16, 32));
	button4->setSize(Size(16, 32));
	button5->setSize(Size(16, 32));
	panel->addChild(button1);
	panel->addChild(button2);
	panel->addChild(button3);
	panel->addChild(button4);
	panel->addChild(button5);

	Engine::update();
	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.VerticalAlignment1.png")));

	uiRoot->removeChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, ReverseHorizontal)
{
	auto uiRoot = Application::getMainWindow();
	auto panel = UIStackPanel::create();
	panel->setSize(uiRoot->getSize());
	panel->setBackground(Brush::Blue);
	panel->setOrientation(Orientation::ReverseHorizontal);
	uiRoot->addChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	button1->setSize(Size(20, 20));
	button2->setSize(Size(30, 30));
	panel->addChild(button1);
	panel->addChild(button2);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.ReverseHorizontal1.png")));

	uiRoot->removeChild(panel);	// 後始末
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_StackPanel, Margin_Padding)
{
	auto uiRoot = Application::getMainWindow();
	auto panel = UIStackPanel::create();
	panel->setSize(uiRoot->getSize());
	panel->setBackground(Brush::Blue);
	uiRoot->addChild(panel);

	auto button1 = UIButton::create();
	auto button2 = UIButton::create();
	button1->setHeight(16);
	button2->setHeight(16);
	panel->addChild(button1);
	panel->addChild(button2);

	panel->padding = ThicknessF(5, 10, 15, 20);
	button2->margin = ThicknessF(5, 10, 15, 20);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.Margin_Padding1.png")));

	uiRoot->removeChild(panel);	// 後始末
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
		auto uiRoot = Application::getMainWindow();

		auto grid = UIGridLayout::create();
		grid->setBackground(Brush::Blue);
		uiRoot->addChild(grid);

		auto button = UIButton::create();
		button->setSize(Size(32, 32));
		grid->addChild(button);

		tr::WeakRefPtr<UIButton> ref = button;
		UIGridLayout* gridPtr = grid;

		Engine::update();
		TestEnv::WaitRendering();

		// button の参照を切ってもまだ生きている
		button.safeRelease();
		ASSERT_EQ(true, ref.isAlive());
		// grid の参照を切ってもまだ生きている
		grid.safeRelease();
		ASSERT_EQ(true, ref.isAlive());
		// root からの参照を切るとようやく削除される
		uiRoot->removeChild(gridPtr);
		ASSERT_EQ(false, ref.isAlive());
	}

	// <Test> GridLayout を setLayoutPanel() で追加する場合、デフォルトでは画面いっぱいに広がる。
	//        これは、オーナーの UIControl からそのサイズ (MainWindow ならクライアント領域サイズ) が与えられ、
	//        また GridLayout(UIElement) のデフォルトの Alignment が Stretch であるため。
	{
		auto uiRoot = Application::getMainWindow();
		RefPtr<UILayoutPanel> oldLayot = uiRoot->getLayoutPanel();

		auto grid = UIGridLayout::create();
		grid->setBackground(Brush::Blue);
		uiRoot->setLayoutPanel(grid);

		auto button = UIButton::create();
		button->setSize(Size(32, 32));
		uiRoot->addChild(button);

		Engine::update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Basic1.png")));
		uiRoot->setLayoutPanel(oldLayot);
	}

	// <Test> GridLayout を addChild() で追加する場合、サイズは 子要素の DesirdSize となり、左上に配置される。
	//        これは、オーナーが AnchorLayout であり、デフォルト(アンカーなし)左上詰めであるため。
	//        なお、AnchorLayout はアンカーなし子要素のサイズ調整は行わない。そのため Alignment が Stretch であっても DesirdSize となる。
	{
		auto uiRoot = Application::getMainWindow();

		auto grid = UIGridLayout::create();
		grid->setBackground(Brush::Blue);
		uiRoot->addChild(grid);

		auto button = UIButton::create();
		button->setSize(Size(32, 32));
		grid->addChild(button);

		Engine::update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Basic2.png"), 99, true));
		uiRoot->clearChildren();
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, DefaultLayout)
{
	// <Test> GridLayout へサイズ指定無しの子要素を追加する場合、UIElement のデフォルトの Alignment(Stretch) に従い引き伸ばされる。
	{
		auto uiRoot = Application::getMainWindow();
		RefPtr<UILayoutPanel> oldLayot = uiRoot->getLayoutPanel();

		auto grid1 = UIGridLayout::create();
		uiRoot->setLayoutPanel(grid1);

		auto button1 = UIButton::create();
		uiRoot->addChild(button1);

		Engine::update();

		ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.DefaultLayout1.png")));
		uiRoot->clearChildren();
		uiRoot->setLayoutPanel(oldLayot);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, layout)
{
	auto uiRoot = Application::getMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->getLayoutPanel();

	auto grid1 = UIGridLayout::create(4, 4);
	uiRoot->setLayoutPanel(grid1);

	auto button1 = UIButton::create();
	uiRoot->addChild(button1);

	auto button2 = UIButton::create();
	button2->setLayoutColumn(1);
	uiRoot->addChild(button2);

	auto button3 = UIButton::create();
	button3->setLayoutRow(1);
	uiRoot->addChild(button3);

	auto button4 = UIButton::create();
	button4->setLayoutColumn(1);
	button4->setLayoutRow(1);
	uiRoot->addChild(button4);

	// column span
	auto button5 = UIButton::create();
	button5->setLayoutColumn(2);
	button5->setLayoutColumnSpan(2);
	uiRoot->addChild(button5);

	auto button6 = UIButton::create();
	button6->setLayoutColumn(2);
	button6->setLayoutRow(1);
	button6->setLayoutColumnSpan(2);
	uiRoot->addChild(button6);

	// row span
	auto button7 = UIButton::create();
	button7->setLayoutRow(2);
	button7->setLayoutRowSpan(2);
	uiRoot->addChild(button7);

	auto button8 = UIButton::create();
	button8->setLayoutColumn(1);
	button8->setLayoutRow(2);
	button8->setLayoutRowSpan(2);
	uiRoot->addChild(button8);

	// column and row span
	auto button9 = UIButton::create();
	button9->setLayoutColumn(2);
	button9->setLayoutColumnSpan(2);
	button9->setLayoutRow(2);
	button9->setLayoutRowSpan(2);
	uiRoot->addChild(button9);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.Layout1.png")));
	uiRoot->clearChildren();
	uiRoot->setLayoutPanel(oldLayot);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, TreeLayout)
{
	auto uiRoot = Application::getMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->getLayoutPanel();

	auto grid1 = UIGridLayout::create(2, 2);
	uiRoot->setLayoutPanel(grid1);

	UIGridLayoutPtr grids[3];
	for (int i = 0; i < 3; ++i)
	{
		grids[i] = UIGridLayout::create(2, 2);

		auto button1 = UIButton::create();
		grids[i]->addChild(button1);

		auto button2 = UIButton::create();
		button2->setLayoutColumn(1);
		grids[i]->addChild(button2);

		auto button3 = UIButton::create();
		button3->setLayoutRow(1);
		grids[i]->addChild(button3);

		auto button4 = UIButton::create();
		button4->setLayoutColumn(1);
		button4->setLayoutRow(1);
		grids[i]->addChild(button4);

		uiRoot->addChild(grids[i]);
	}
	grids[1]->setLayoutRow(1);
	grids[2]->setLayoutColumn(1);
	grids[2]->setLayoutRowSpan(2);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.TreeLayout1.png")));
	uiRoot->clearChildren();
	uiRoot->setLayoutPanel(oldLayot);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, GridLength)
{
	auto uiRoot = Application::getMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->getLayoutPanel();

	auto grid1 = UIGridLayout::create();
	uiRoot->setLayoutPanel(grid1);
	grid1->addColumnDefinition();	// default
	grid1->addColumnDefinition(GridLengthType::Pixel, 50);
	grid1->addColumnDefinition(GridLengthType::Auto);
	grid1->addColumnDefinition(GridLengthType::Ratio, 2.0f);
	grid1->addRowDefinition();		// default
	grid1->addRowDefinition(GridLengthType::Pixel, 50);
	grid1->addRowDefinition(GridLengthType::Auto);
	grid1->addRowDefinition(GridLengthType::Ratio, 2.0f);

	auto button1 = UIButton::create();
	button1->setLayoutColumn(0);
	button1->setLayoutRow(0);
	uiRoot->addChild(button1);

	auto button2 = UIButton::create();
	button2->setLayoutColumn(1);
	button2->setLayoutRow(1);
	uiRoot->addChild(button2);

	auto button3 = UIButton::create();
	button3->setSize(Size(20, 20));
	button3->setLayoutColumn(2);
	button3->setLayoutRow(2);
	uiRoot->addChild(button3);

	auto button4 = UIButton::create();
	button4->setLayoutColumn(3);
	button4->setLayoutRow(3);
	uiRoot->addChild(button4);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.GridLength1.png")));
	uiRoot->clearChildren();
	uiRoot->setLayoutPanel(oldLayot);
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_GridLayout, MinMax)
{
	auto uiRoot = Application::getMainWindow();
	RefPtr<UILayoutPanel> oldLayot = uiRoot->getLayoutPanel();

	auto grid1 = UIGridLayout::create();
	uiRoot->setLayoutPanel(grid1);
	grid1->addColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->addColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->addColumnDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->addRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->addRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);
	grid1->addRowDefinition(GridLengthType::Auto, 1.0f, 20.0f, 30.0f);

	auto button1 = UIButton::create();
	button1->setLayoutColumn(0);
	button1->setLayoutRow(0);
	button1->setSize(Size(10, 10));
	button1->setHAlignment(HAlignment::Left);
	button1->setVAlignment(VAlignment::Top);
	uiRoot->addChild(button1);

	auto button2 = UIButton::create();
	button2->setLayoutColumn(1);
	button2->setLayoutRow(1);
	button2->setSize(Size(40, 40));
	button2->setHAlignment(HAlignment::Left);
	button2->setVAlignment(VAlignment::Top);
	uiRoot->addChild(button2);

	auto button3 = UIButton::create();
	button3->setLayoutColumn(2);
	button3->setLayoutRow(2);
	button3->setSize(Size(10, 10));
	button3->setHAlignment(HAlignment::Left);
	button3->setVAlignment(VAlignment::Top);
	uiRoot->addChild(button3);

	Engine::update();

	ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_GridLayout.MinMax1.png")));
	uiRoot->clearChildren();
	uiRoot->setLayoutPanel(oldLayot);
}

