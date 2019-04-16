#include "Common.hpp"

#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# Testing Test_UI_UILayout
class Test_UI_UILayout : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UILayout, Default)
{
	//- [ ] Default
	{
		auto e1 = newObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		e1->setWidth(80);
		e1->setHeight(60);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-Default-1.png"));
		LN_TEST_CLEAN_SCENE;
	}

	{
		//auto uiRoot = GetUIRoot();
		//auto panel = UIStackPanel::create();
		//panel->setOrientation(Orientation::Vertical);
		//panel->setSize(uiRoot->getSize());
		//panel->setBackground(Brush::Blue);
		//uiRoot->addChild(panel);

		//auto button1 = UIButton::create();
		//auto button2 = UIButton::create();
		//auto button3 = UIButton::create();
		//auto button4 = UIButton::create();
		//auto button5 = UIButton::create();
		//button1->setHAlignment(HAlignment::Stretch);
		//button2->setHAlignment(HAlignment::Left);
		//button3->setHAlignment(HAlignment::Right);
		//button4->setHAlignment(HAlignment::Center);
		//button5->setHAlignment(HAlignment::Stretch);
		//button1->setHeight(16);
		//button2->setSize(Size(32, 16));
		//button3->setSize(Size(32, 16));
		//button4->setSize(Size(32, 16));
		//button5->setSize(Size(32, 16));
		//panel->addChild(button1);
		//panel->addChild(button2);
		//panel->addChild(button3);
		//panel->addChild(button4);
		//panel->addChild(button5);

		//Engine::update();
		//ASSERT_TRUE(TestEnv::CheckScreenShot(LN_LOCALFILE("Result/Test_UI_FlowLayout.HorizontalAlignment1.png")));

		//uiRoot->removeChild(panel);	// 後始末
	}
}

//------------------------------------------------------------------------------
//## ParentChildRelation
TEST_F(Test_UI_UILayout, ParentChildRelation)
{
	auto e1 = newObject<UIContainerElement>();
	e1->setBackgroundColor(Color::Red);
	e1->setMargin(Thickness(10, 20, 0, 0));
	e1->setWidth(100);
	e1->setHeight(80);
	e1->setHorizontalAlignment(HAlignment::Left);
	e1->setVerticalAlignment(VAlignment::Top);

	auto e2 = newObject<UIContainerElement>();
	e2->setBackgroundColor(Color::Green);
	e1->setMargin(Thickness(10, 5, 0, 0));
	e2->setWidth(80);
	e2->setHeight(20);
	e2->setHorizontalAlignment(HAlignment::Left);
	e2->setVerticalAlignment(VAlignment::Top);
	e1->addElement(e2);

	TestEnv::updateFrame();
	ASSERT_SCREEN_S(LN_ASSETFILE("UI/Result/Test_UI_UILayout-ParentChildRelation-1.png"));
	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//## RenderTransform
TEST_F(Test_UI_UILayout, RenderTransform)
{
	//- [ ] position
	{
		auto e1 = newObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		e1->setWidth(80);
		e1->setHeight(60);
		e1->setPosition(10, 20);

		TestEnv::updateFrame();
		ASSERT_SCREEN_S(LN_ASSETFILE("UI/Result/Test_UI_UILayout-RenderTransform-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## BorderLayout
TEST_F(Test_UI_UILayout, BorderLayout)
{
	//- [ ] 8 方向境界と中央への配置
	{
		auto e11 = newObject<UIElement>();
		e11->setBackgroundColor(Color::Red);
		e11->setWidth(20);
		e11->setHeight(20);
		e11->setHorizontalAlignment(HAlignment::Left);
		e11->setVerticalAlignment(VAlignment::Top);

		auto e12 = newObject<UIElement>();
		e12->setBackgroundColor(Color::Green);
		e12->setWidth(20);
		e12->setHeight(20);
		e12->setHorizontalAlignment(HAlignment::Center);
		e12->setVerticalAlignment(VAlignment::Top);

		auto e13 = newObject<UIElement>();
		e13->setBackgroundColor(Color::Blue);
		e13->setWidth(20);
		e13->setHeight(20);
		e13->setHorizontalAlignment(HAlignment::Right);
		e13->setVerticalAlignment(VAlignment::Top);

		auto e21 = newObject<UIElement>();
		e21->setBackgroundColor(Color::Red);
		e21->setWidth(20);
		e21->setHeight(20);
		e21->setHorizontalAlignment(HAlignment::Left);
		e21->setVerticalAlignment(VAlignment::Center);

		auto e22 = newObject<UIElement>();
		e22->setBackgroundColor(Color::Green);
		e22->setWidth(20);
		e22->setHeight(20);
		e22->setHorizontalAlignment(HAlignment::Center);
		e22->setVerticalAlignment(VAlignment::Center);

		auto e23 = newObject<UIElement>();
		e23->setBackgroundColor(Color::Blue);
		e23->setWidth(20);
		e23->setHeight(20);
		e23->setHorizontalAlignment(HAlignment::Right);
		e23->setVerticalAlignment(VAlignment::Center);

		auto e31 = newObject<UIElement>();
		e31->setBackgroundColor(Color::Red);
		e31->setWidth(20);
		e31->setHeight(20);
		e31->setHorizontalAlignment(HAlignment::Left);
		e31->setVerticalAlignment(VAlignment::Bottom);

		auto e32 = newObject<UIElement>();
		e32->setBackgroundColor(Color::Green);
		e32->setWidth(20);
		e32->setHeight(20);
		e32->setHorizontalAlignment(HAlignment::Center);
		e32->setVerticalAlignment(VAlignment::Bottom);

		auto e33 = newObject<UIElement>();
		e33->setBackgroundColor(Color::Blue);
		e33->setWidth(20);
		e33->setHeight(20);
		e33->setHorizontalAlignment(HAlignment::Right);
		e33->setVerticalAlignment(VAlignment::Bottom);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-BorderLayout-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## StackLayout
TEST_F(Test_UI_UILayout, StackLayout)
{
	UIContainerElement* uiRoot = Engine::mainUIRoot();

	auto l1 = UIStackLayout::create();
	uiRoot->setLayoutPanel(l1);

	auto e1 = newObject<UIElement>();
	e1->setBackgroundColor(Color::Red);
	e1->setWidth(20);
	e1->setHeight(20);
	e1->setHorizontalAlignment(HAlignment::Left);
	e1->setVerticalAlignment(VAlignment::Top);

	auto e2 = newObject<UIElement>();
	e2->setBackgroundColor(Color::Green);
	e2->setWidth(20);
	e2->setHeight(20);
	e2->setHorizontalAlignment(HAlignment::Center);
	e2->setVerticalAlignment(VAlignment::Center);

	auto e3 = newObject<UIElement>();
	e3->setBackgroundColor(Color::Blue);
	e3->setWidth(20);
	e3->setHeight(20);
	e3->setHorizontalAlignment(HAlignment::Right);
	e3->setVerticalAlignment(VAlignment::Bottom);

	//- [ ] default (Vertical)
	{
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-1.png"));
	}

	//- [ ] Horizontal
	{
		auto layout = UIStackLayout::create();
		layout->setOrientation(Orientation::Horizontal);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-2.png"));
	}

	//- [ ] ReverseVertical
	{
		auto layout = UIStackLayout::create();
		layout->setOrientation(Orientation::ReverseVertical);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-3.png"));
	}

	//- [ ] ReverseHorizontal
	{
		auto layout = UIStackLayout::create();
		layout->setOrientation(Orientation::ReverseHorizontal);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-4.png"));
	}

	LN_TEST_CLEAN_SCENE;
}

#endif // LN_UNIT_TEST_EXPERIMENTAL
