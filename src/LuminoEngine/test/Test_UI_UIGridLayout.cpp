﻿#include "Common.hpp"
#include <LuminoEngine/UI/Layout/UIGridLayout.hpp>

//==============================================================================
//# Testing Test_UI_UIGridLayout
class Test_UI_UIGridLayout : public LuminoSceneTest {};

TEST_F(Test_UI_UIGridLayout, Basic)
{
	//- [ ] Default. Filled screen.
	{
		auto layout1 = makeObject<UIGridLayout>();
		Engine::mainUIView()->addElement(layout1);


		auto child1 = makeObject<UIElement>();
		//child1->setWidth(32);
		//child1->setHeight(16);
		//child1->setHAlignment(UIHAlignment::Left);
		child1->setBackgroundColor(Color::Green);
		layout1->addChild(child1);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIGridLayout-Basic-1.png"));
		LN_TEST_CLEAN_SCENE;
	}

	//- [ ] 3 columns.
	{
		auto layout1 = makeObject<UIGridLayout>();
		layout1->setColumnCount(3);
		Engine::mainUIView()->addElement(layout1);

		auto e1 = makeObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		layout1->addChild(e1);

		auto e2 = makeObject<UIElement>();
		e2->setBackgroundColor(Color::Green);
		layout1->addChild(e2);

		auto e3 = makeObject<UIElement>();
		e3->setBackgroundColor(Color::Blue);
		layout1->addChild(e3);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIGridLayout-Basic-2.png"));
		LN_TEST_CLEAN_SCENE;
	}

#if 0
	//- [ ] 2x2 cells.
	{
		auto layout1 = makeDecl<UIGridLayout>()
			.columnCount(2)
			.children(
			{
				makeDecl<UIElement>()
					.backgroundColor(Color::Red),
				makeDecl<UIElement>()
					.backgroundColor(Color::Green),
				makeDecl<UIElement>()
					.backgroundColor(Color::Blue),
				makeDecl<UIElement>()
					.backgroundColor(Color::Yellow)

			}).build();
		Engine::mainUIView()->addElement(layout1);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIGridLayout-Basic-3.png"));
		LN_TEST_CLEAN_SCENE;
	}
#endif
}

TEST_F(Test_UI_UIGridLayout, VerticalFlow)
{
	//- [ ] Basic form style.
	{
		auto layout1 = makeObject<UIGridLayout>();
		layout1->setRule(UILayoutRule::VerticalFlow);
		layout1->setColumnCount(2);
		Engine::mainUIView()->addElement(layout1);

		auto e1 = makeObject<UIElement>();
		e1->setBackgroundColor(Color::Red);
		e1->setHeight(20);
		layout1->addChild(e1);

		auto e2 = makeObject<UIElement>();
		e2->setBackgroundColor(Color::Green);
		e2->setHeight(20);
		layout1->addChild(e2);

		auto s = makeObject<UIElement>();
		s->setBackgroundColor(Color::White);
		s->setHeight(20);
		s->getGridLayoutInfo()->layoutColumnSpan = 2;
		layout1->addChild(s);

		auto e3 = makeObject<UIElement>();
		e3->setBackgroundColor(Color::Blue);
		e3->setHeight(20);
		layout1->addChild(e3);

		auto e4 = makeObject<UIElement>();
		e4->setBackgroundColor(Color::Yellow);
		e4->setHeight(20);
		layout1->addChild(e4);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIGridLayout-Basic-4.png"));
		LN_TEST_CLEAN_SCENE;
	}
}
