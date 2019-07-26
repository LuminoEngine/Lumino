#include "Common.hpp"

//==============================================================================
//# Testing Test_UI_UILayout
class Test_UI_UILayout : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UILayout, Basic)
{
	//- [ ] Default
	{
		auto e1 = makeObject<UIElement>();
		e1->setHorizontalAlignment(HAlignment::Left);
		e1->setVerticalAlignment(VAlignment::Top);
		e1->setBackgroundColor(Color::Red);
		e1->setWidth(80);
		e1->setHeight(60);
		Engine::mainUIRoot()->addElement(e1);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-Basic-1.png"));
		LN_TEST_CLEAN_SCENE;
	}

	{
        auto parent1 = makeObject<UIContainerElement>();
        parent1->setBackgroundColor(Color::Red);
        parent1->setWidth(80);
        parent1->setHeight(60);
		parent1->setHorizontalAlignment(HAlignment::Left);
		parent1->setVerticalAlignment(VAlignment::Top);
        parent1->setLayoutPanel(UIStackLayout::create());
		Engine::mainUIRoot()->addElement(parent1);
        
        auto child1 = makeObject<UIElement>();
        child1->setWidth(32);
        child1->setHeight(16);
		child1->setHorizontalAlignment(HAlignment::Left);
        child1->setBackgroundColor(Color::Green);
		Engine::mainUIRoot()->addElement(child1);
        parent1->addElement(child1);

        auto child2 = makeObject<UIElement>();
        child2->setWidth(32);
        child2->setHeight(16);
		child1->setHorizontalAlignment(HAlignment::Left);
        child2->setBackgroundColor(Color::Blue);
		Engine::mainUIRoot()->addElement(child2);
        parent1->addElement(child2);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-Basic-2.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## RenderTransform
TEST_F(Test_UI_UILayout, RenderTransform)
{
	//- [ ] position
	{
		auto e1 = makeObject<UIElement>();
		e1->setHorizontalAlignment(HAlignment::Left);
		e1->setVerticalAlignment(VAlignment::Top);
		e1->setBackgroundColor(Color::Red);
		e1->setWidth(80);
		e1->setHeight(60);
		e1->setPosition(10, 20);
		Engine::mainUIRoot()->addElement(e1);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-RenderTransform-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## BorderLayout
TEST_F(Test_UI_UILayout, BorderLayout)
{
    auto owner1 = Engine::mainUIRoot();

    auto e11 = makeObject<UIElement>();
    e11->setBackgroundColor(Color::Red);
    e11->setWidth(20);
    e11->setHeight(20);
    e11->setHorizontalAlignment(HAlignment::Left);
    e11->setVerticalAlignment(VAlignment::Top);
	Engine::mainUIRoot()->addElement(e11);

    auto e12 = makeObject<UIElement>();
    e12->setBackgroundColor(Color::Green);
    e12->setWidth(20);
    e12->setHeight(20);
    e12->setHorizontalAlignment(HAlignment::Center);
    e12->setVerticalAlignment(VAlignment::Top);
	Engine::mainUIRoot()->addElement(e12);

    auto e13 = makeObject<UIElement>();
    e13->setBackgroundColor(Color::Blue);
    e13->setWidth(20);
    e13->setHeight(20);
    e13->setHorizontalAlignment(HAlignment::Right);
    e13->setVerticalAlignment(VAlignment::Top);
	Engine::mainUIRoot()->addElement(e13);

    auto e21 = makeObject<UIElement>();
    e21->setBackgroundColor(Color::Red);
    e21->setWidth(20);
    e21->setHeight(20);
    e21->setHorizontalAlignment(HAlignment::Left);
    e21->setVerticalAlignment(VAlignment::Center);
	Engine::mainUIRoot()->addElement(e21);

    auto e22 = makeObject<UIElement>();
    e22->setBackgroundColor(Color::Green);
    e22->setWidth(20);
    e22->setHeight(20);
    e22->setHorizontalAlignment(HAlignment::Center);
    e22->setVerticalAlignment(VAlignment::Center);
	Engine::mainUIRoot()->addElement(e22);

    auto e23 = makeObject<UIElement>();
    e23->setBackgroundColor(Color::Blue);
    e23->setWidth(20);
    e23->setHeight(20);
    e23->setHorizontalAlignment(HAlignment::Right);
    e23->setVerticalAlignment(VAlignment::Center);
	Engine::mainUIRoot()->addElement(e23);

    auto e31 = makeObject<UIElement>();
    e31->setBackgroundColor(Color::Red);
    e31->setWidth(20);
    e31->setHeight(20);
    e31->setHorizontalAlignment(HAlignment::Left);
    e31->setVerticalAlignment(VAlignment::Bottom);
	Engine::mainUIRoot()->addElement(e31);

    auto e32 = makeObject<UIElement>();
    e32->setBackgroundColor(Color::Green);
    e32->setWidth(20);
    e32->setHeight(20);
    e32->setHorizontalAlignment(HAlignment::Center);
    e32->setVerticalAlignment(VAlignment::Bottom);
	Engine::mainUIRoot()->addElement(e32);

    auto e33 = makeObject<UIElement>();
    e33->setBackgroundColor(Color::Blue);
    e33->setWidth(20);
    e33->setHeight(20);
    e33->setHorizontalAlignment(HAlignment::Right);
    e33->setVerticalAlignment(VAlignment::Bottom);
	Engine::mainUIRoot()->addElement(e33);

	//- [ ] 8 方向境界と中央への配置
	{
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-BorderLayout-1.png"));
	}

    //- [ ] 親要素に padding をつける
    {
        owner1->setPadding(Thickness(20));

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-BorderLayout-2.png"));

		owner1->setPadding(Thickness(0));
    }

    LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//## StackLayout
TEST_F(Test_UI_UILayout, StackLayout)
{
	UIContainerElement* uiRoot = Engine::mainUIRoot();

	auto l1 = UIStackLayout::create();
	uiRoot->setLayoutPanel(l1);

	auto e1 = makeObject<UIElement>();
	e1->setBackgroundColor(Color::Red);
	e1->setWidth(20);
	e1->setHeight(20);
	e1->setHorizontalAlignment(HAlignment::Left);
	e1->setVerticalAlignment(VAlignment::Top);
	Engine::mainUIRoot()->addElement(e1);

	auto e2 = makeObject<UIElement>();
	e2->setBackgroundColor(Color::Green);
	e2->setWidth(20);
	e2->setHeight(20);
	e2->setHorizontalAlignment(HAlignment::Center);
	e2->setVerticalAlignment(VAlignment::Center);
	Engine::mainUIRoot()->addElement(e2);

	auto e3 = makeObject<UIElement>();
	e3->setBackgroundColor(Color::Blue);
	e3->setWidth(20);
	e3->setHeight(20);
	e3->setHorizontalAlignment(HAlignment::Right);
	e3->setVerticalAlignment(VAlignment::Bottom);
	Engine::mainUIRoot()->addElement(e3);

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

    uiRoot->setLayoutPanel(nullptr);
	LN_TEST_CLEAN_SCENE;
}
