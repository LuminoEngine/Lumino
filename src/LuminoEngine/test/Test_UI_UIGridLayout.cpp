#include "Common.hpp"
#include <LuminoEngine/UI/UIGridLayout.hpp>

//==============================================================================
//# Testing Test_UI_UIGridLayout
class Test_UI_UIGridLayout : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UIGridLayout, Basic)
{
	auto layout1 = makeObject<UIGridLayout>();
	Engine::mainUIView()->addElement(layout1);


	auto child1 = makeObject<UIElement>();
	child1->setWidth(32);
	child1->setHeight(16);
	child1->setHorizontalAlignment(HAlignment::Left);
	child1->setBackgroundColor(Color::Green);
	layout1->addChild(child1);

	//auto child2 = makeObject<UIElement>();
	//child2->setWidth(32);
	//child2->setHeight(16);
	//child2->setHorizontalAlignment(HAlignment::Left);
	//child2->setBackgroundColor(Color::Blue);
	////Engine::mainUIView()->addElement(child2);
	//layout1->addChild(child2);

	////- [ ] Default
	//{
	//	auto e1 = makeObject<UIElement>();
	//	e1->setHorizontalAlignment(HAlignment::Left);
	//	e1->setVerticalAlignment(VAlignment::Top);
	//	e1->setBackgroundColor(Color::Red);
	//	e1->setWidth(80);
	//	e1->setHeight(60);
	//	Engine::mainUIView()->addElement(e1);

	//	TestEnv::updateFrame();
	//	ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UILayout-Basic-1.png"));
	//	LN_TEST_CLEAN_SCENE;
	//}

	{
  //      auto parent1 = makeObject<UIContainerElement>();
  //      parent1->setBackgroundColor(Color::Red);
  //      parent1->setWidth(80);
  //      parent1->setHeight(60);
		//parent1->setHorizontalAlignment(HAlignment::Left);
		//parent1->setVerticalAlignment(VAlignment::Top);
  //      //parent1->setLayoutPanel(UIStackLayout_Obsolete::create());
		//Engine::mainUIView()->addElement(parent1);

  //      auto layout1 = makeObject<UIStackLayout2>();
  //      parent1->addElement(layout1);
  //      
  //      auto child1 = makeObject<UIElement>();
  //      child1->setWidth(32);
  //      child1->setHeight(16);
		//child1->setHorizontalAlignment(HAlignment::Left);
  //      child1->setBackgroundColor(Color::Green);
		////Engine::mainUIView()->addElement(child1);
  //      layout1->addChild(child1);

  //      auto child2 = makeObject<UIElement>();
  //      child2->setWidth(32);
  //      child2->setHeight(16);
  //      child2->setHorizontalAlignment(HAlignment::Left);
  //      child2->setBackgroundColor(Color::Blue);
		////Engine::mainUIView()->addElement(child2);
  //      layout1->addChild(child2);

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("UI/Result/UIGridLayout-Basic-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}
