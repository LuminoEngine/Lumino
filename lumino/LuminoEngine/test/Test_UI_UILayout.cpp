#include "Common.hpp"

//==============================================================================
//# Testing Test_UI_UILayout
class Test_UI_UILayout : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UILayout, Basic)
{
	////- [ ] Default
	//{
	//	auto e1 = makeObject<UIElement>();
	//	e1->setHAlignment(UIHAlignment::Left);
	//	e1->setVAlignment(UIVAlignment::Top);
	//	e1->setBackgroundColor(Color::Red);
	//	e1->setWidth(80);
	//	e1->setHeight(60);
	//	Engine::mainUIView()->addElement(e1);

	//	TestEnv::updateFrame();
	//	ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-Basic-1.png"));
	//	LN_TEST_CLEAN_SCENE;
	//}

	{
        auto parent1 = makeObject<UIContainerElement>();
        parent1->setBackgroundColor(Color::Red);
        parent1->setWidth(80);
        parent1->setHeight(60);
		parent1->setHAlignment(UIHAlignment::Left);
		parent1->setVAlignment(UIVAlignment::Top);
        //parent1->setLayoutPanel(UIStackLayout_Obsolete::create());
		Engine::mainUIView()->addElement(parent1);

        auto layout1 = makeObject<UIStackLayout2_Obsolete>();
        parent1->addElement(layout1);
        
        auto child1 = makeObject<UIElement>();
        child1->setWidth(32);
        child1->setHeight(16);
		child1->setHAlignment(UIHAlignment::Left);
        child1->setBackgroundColor(Color::Green);
		//Engine::mainUIView()->addElement(child1);
        layout1->addChild(child1);

        auto child2 = makeObject<UIElement>();
        child2->setWidth(32);
        child2->setHeight(16);
        child2->setHAlignment(UIHAlignment::Left);
        child2->setBackgroundColor(Color::Blue);
		//Engine::mainUIView()->addElement(child2);
        layout1->addChild(child2);

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
		e1->setHAlignment(UIHAlignment::Left);
		e1->setVAlignment(UIVAlignment::Top);
		e1->setBackgroundColor(Color::Red);
		e1->setWidth(80);
		e1->setHeight(60);
		e1->setPosition(10, 20);
		Engine::mainUIView()->addElement(e1);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-RenderTransform-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
}



TEST_F(Test_UI_UILayout, Size)
{
	// size
	{
		auto e1 = makeObject<UIControl>();
		e1->setAlignments(UIHAlignment::Center, UIVAlignment::Center);
		e1->setBackgroundColor(Color::Red);
		e1->setSize(100, 80);
		Engine::ui()->addElement(e1);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UILayout-Size-1.png"));
		LN_TEST_CLEAN_SCENE;
	}
	// size & margin
	{
		auto e1 = makeObject<UIControl>();
		e1->setAlignments(UIHAlignment::Center, UIVAlignment::Center);
		e1->setBackgroundColor(Color::Red);
		Engine::ui()->addElement(e1);

		auto e2 = makeObject<UIElement>();
		e2->setAlignments(UIHAlignment::Center, UIVAlignment::Center);
		e2->setBackgroundColor(Color::Green);
		e2->setSize(30, 20);
		e2->setMargin(Thickness(10));
		e1->addElement(e2);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UILayout-Size-2.png"));
		LN_TEST_CLEAN_SCENE;
	}
	// size & padding
	{
		auto e1 = makeObject<UIControl>();
		e1->setAlignments(UIHAlignment::Center, UIVAlignment::Center);
		e1->setBackgroundColor(Color::Red);
		e1->setPadding(Thickness(10));
		Engine::ui()->addElement(e1);

		auto e2 = makeObject<UIElement>();
		e2->setAlignments(UIHAlignment::Center, UIVAlignment::Center);
		e2->setBackgroundColor(Color::Green);
		e2->setSize(30, 20);
		e1->addElement(e2);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UILayout-Size-3.png"));
		LN_TEST_CLEAN_SCENE;
	}
	// 親子両方でサイズ固定されている & 子の方がサイズ大きい場合 → はみだす
	{
		auto e1 = makeObject<UIControl>();
		e1->setAlignments(UIHAlignment::Center, UIVAlignment::Center);
		e1->setBackgroundColor(Color::Red);
		e1->setSize(50, 50);
		Engine::mainUIView()->addElement(e1);
		
		auto e2 = makeObject<UIElement>();
		e2->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
		e2->setBackgroundColor(Color::Green);
		e2->setSize(100, 20);
		e1->addElement(e2);
		
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UILayout-Size-5.png"));
		LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## BorderLayout
TEST_F(Test_UI_UILayout, BorderLayout)
{
    auto owner1 = Engine::mainUIView();

    auto e11 = makeObject<UIElement>();
    e11->setBackgroundColor(Color::Red);
    e11->setWidth(20);
    e11->setHeight(20);
    e11->setHAlignment(UIHAlignment::Left);
    e11->setVAlignment(UIVAlignment::Top);
	Engine::mainUIView()->addElement(e11);

    auto e12 = makeObject<UIElement>();
    e12->setBackgroundColor(Color::Green);
    e12->setWidth(20);
    e12->setHeight(20);
    e12->setHAlignment(UIHAlignment::Center);
    e12->setVAlignment(UIVAlignment::Top);
	Engine::mainUIView()->addElement(e12);

    auto e13 = makeObject<UIElement>();
    e13->setBackgroundColor(Color::Blue);
    e13->setWidth(20);
    e13->setHeight(20);
    e13->setHAlignment(UIHAlignment::Right);
    e13->setVAlignment(UIVAlignment::Top);
	Engine::mainUIView()->addElement(e13);

    auto e21 = makeObject<UIElement>();
    e21->setBackgroundColor(Color::Red);
    e21->setWidth(20);
    e21->setHeight(20);
    e21->setHAlignment(UIHAlignment::Left);
    e21->setVAlignment(UIVAlignment::Center);
	Engine::mainUIView()->addElement(e21);

    auto e22 = makeObject<UIElement>();
    e22->setBackgroundColor(Color::Green);
    e22->setWidth(20);
    e22->setHeight(20);
    e22->setHAlignment(UIHAlignment::Center);
    e22->setVAlignment(UIVAlignment::Center);
	Engine::mainUIView()->addElement(e22);

    auto e23 = makeObject<UIElement>();
    e23->setBackgroundColor(Color::Blue);
    e23->setWidth(20);
    e23->setHeight(20);
    e23->setHAlignment(UIHAlignment::Right);
    e23->setVAlignment(UIVAlignment::Center);
	Engine::mainUIView()->addElement(e23);

    auto e31 = makeObject<UIElement>();
    e31->setBackgroundColor(Color::Red);
    e31->setWidth(20);
    e31->setHeight(20);
    e31->setHAlignment(UIHAlignment::Left);
    e31->setVAlignment(UIVAlignment::Bottom);
	Engine::mainUIView()->addElement(e31);

    auto e32 = makeObject<UIElement>();
    e32->setBackgroundColor(Color::Green);
    e32->setWidth(20);
    e32->setHeight(20);
    e32->setHAlignment(UIHAlignment::Center);
    e32->setVAlignment(UIVAlignment::Bottom);
	Engine::mainUIView()->addElement(e32);

    auto e33 = makeObject<UIElement>();
    e33->setBackgroundColor(Color::Blue);
    e33->setWidth(20);
    e33->setHeight(20);
    e33->setHAlignment(UIHAlignment::Right);
    e33->setVAlignment(UIVAlignment::Bottom);
	Engine::mainUIView()->addElement(e33);

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
	UIControl* uiRoot = Engine::mainUIView();

	auto l1 = UIStackLayout2_Obsolete::create();
	//uiRoot->setLayoutPanel(l1);
    Engine::mainUIView()->addElement(l1);

	auto e1 = makeObject<UIElement>();
	e1->setBackgroundColor(Color::Red);
	e1->setWidth(20);
	e1->setHeight(20);
	e1->setHAlignment(UIHAlignment::Left);
	e1->setVAlignment(UIVAlignment::Top);
	//Engine::mainUIView()->addElement(e1);
    l1->addChild(e1);

	auto e2 = makeObject<UIElement>();
	e2->setBackgroundColor(Color::Green);
	e2->setWidth(20);
	e2->setHeight(20);
	e2->setHAlignment(UIHAlignment::Center);
	e2->setVAlignment(UIVAlignment::Center);
	//Engine::mainUIView()->addElement(e2);
    l1->addChild(e2);

	auto e3 = makeObject<UIElement>();
	e3->setBackgroundColor(Color::Blue);
	e3->setWidth(20);
	e3->setHeight(20);
	e3->setHAlignment(UIHAlignment::Right);
	e3->setVAlignment(UIVAlignment::Bottom);
	//Engine::mainUIView()->addElement(e3);
    l1->addChild(e3);

	//- [ ] default (Vertical)
	{
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-1.png"));
        Engine::mainUIView()->removeElement(l1);
	}

#if 0 // TODO:
	//- [ ] Horizontal
	{

		auto layout = UIStackLayout2_Obsolete::create();
		layout->setOrientation(UILayoutOrientation::Horizontal);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-2.png"));
	}

	//- [ ] ReverseVertical
	{
		auto layout = UIStackLayout2_Obsolete::create();
		layout->setOrientation(UILayoutOrientation::ReverseVertical);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-3.png"));
        Engine::mainUIView()->removeElement(layout);
	}

	//- [ ] ReverseHorizontal
	{
		auto layout = UIStackLayout2_Obsolete::create();
		layout->setOrientation(UILayoutOrientation::ReverseHorizontal);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-4.png"));
	}

    uiRoot->setLayoutPanel(nullptr);
#endif

	LN_TEST_CLEAN_SCENE;
}

TEST_F(Test_UI_UILayout, BoxLayout)
{
	UIControl* uiRoot = Engine::mainUIView();

	// Horizontal
	{
		auto l1 = makeObject<UIBoxLayout>();
		l1->setOrientation(UILayoutOrientation::Horizontal);
		Engine::mainUIView()->addElement(l1);

		auto e1 = makeObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		e1->setHAlignment(UIHAlignment::Stretch);
		e1->setVAlignment(UIVAlignment::Stretch);
		l1->addChild(e1);

		auto e2 = makeObject<UIElement>();
		e2->setBackgroundColor(Color::Green);
		e2->setHAlignment(UIHAlignment::Stretch);
		e2->setVAlignment(UIVAlignment::Stretch);
		l1->addChild(e2);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/BoxLayout-1.png"));
		LN_TEST_CLEAN_SCENE;
	}

	// Vertical
	{
		auto l1 = makeObject<UIBoxLayout>();
		l1->setOrientation(UILayoutOrientation::Vertical);
		Engine::mainUIView()->addElement(l1);

		auto e1 = makeObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		e1->setHAlignment(UIHAlignment::Stretch);
		e1->setVAlignment(UIVAlignment::Stretch);
		l1->addChild(e1);

		auto e2 = makeObject<UIElement>();
		e2->setBackgroundColor(Color::Green);
		e2->setHAlignment(UIHAlignment::Stretch);
		e2->setVAlignment(UIVAlignment::Stretch);
		l1->addChild(e2);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/BoxLayout-2.png"));
		LN_TEST_CLEAN_SCENE;
	}

	// Horizontal -> Vertical
	{
		auto l1 = makeObject<UIBoxLayout>();
		l1->setOrientation(UILayoutOrientation::Horizontal);
		Engine::mainUIView()->addElement(l1);

		auto e1 = makeObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		e1->setHAlignment(UIHAlignment::Stretch);
		e1->setVAlignment(UIVAlignment::Stretch);
		l1->addChild(e1);

		auto l2 = makeObject<UIBoxLayout>();
		l2->setOrientation(UILayoutOrientation::Vertical);
		l1->addChild(l2);

		auto e2 = makeObject<UIElement>();
		e2->setBackgroundColor(Color::Green);
		e2->setHAlignment(UIHAlignment::Stretch);
		e2->setVAlignment(UIVAlignment::Stretch);
		l2->addChild(e2);

		auto e3 = makeObject<UIElement>();
		e3->setBackgroundColor(Color::Blue);
		e3->setHAlignment(UIHAlignment::Stretch);
		e3->setVAlignment(UIVAlignment::Stretch);
		l2->addChild(e3);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/BoxLayout-3.png"));
		LN_TEST_CLEAN_SCENE;
	}

#if 0 // TODO:
	//- [ ] Horizontal
	{

		auto layout = UIStackLayout2_Obsolete::create();
		layout->setOrientation(UILayoutOrientation::Horizontal);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-2.png"));
	}

	//- [ ] ReverseVertical
	{
		auto layout = UIStackLayout2_Obsolete::create();
		layout->setOrientation(UILayoutOrientation::ReverseVertical);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-3.png"));
		Engine::mainUIView()->removeElement(layout);
	}

	//- [ ] ReverseHorizontal
	{
		auto layout = UIStackLayout2_Obsolete::create();
		layout->setOrientation(UILayoutOrientation::ReverseHorizontal);
		uiRoot->setLayoutPanel(layout);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-StackLayout-4.png"));
	}

	uiRoot->setLayoutPanel(nullptr);
#endif

	LN_TEST_CLEAN_SCENE;
}
