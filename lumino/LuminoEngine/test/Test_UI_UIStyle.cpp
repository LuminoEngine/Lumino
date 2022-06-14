#include "Common.hpp"

//#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# Testing Test_UI_UIStyle
class Test_UI_UIStyle : public LuminoSceneTest {};

class Test_UI_UIStyle_Element : public UIElement
{
public:

protected:
	virtual const String& elementName() const  override { static String name = _TT("TestElement"); return name; }
};

//------------------------------------------------------------------------------
//## Background
TEST_F(Test_UI_UIStyle, Background)
{
	auto element1 = makeObject<Test_UI_UIStyle_Element>();
	element1->setAlignments(UIAlignment::Center);
	element1->setWidth(80);
	element1->setHeight(60);
	Engine::mainUIView()->addElement(element1);

	element1->setBackgroundColor(Color::Red);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/Background-1.png"));
	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
TEST_F(Test_UI_UIStyle, BackgroundImage)
{
	auto element1 = makeObject<UIElement>();
	element1->setAlignments(UIAlignment::Center);
	element1->setSize(80, 60);
	Engine::ui()->addElement(element1);

	element1->setBackgroundImage(Texture2D::load(_TT("Sprite1.png")));
	{
		element1->setBackgroundDrawMode(Sprite9DrawMode::StretchedSingleImage);
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIStyle-BackgroundImage-1.png"));
	}
	{
		element1->setBackgroundDrawMode(Sprite9DrawMode::RepeatedSingleImage);
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIStyle-BackgroundImage-2.png"));
	}

	element1->setBackgroundImage(Texture2D::load(_TT("UI/Window2.png")));
	element1->setBackgroundImageRect(Rect(0, 32, 32, 32));
	element1->setBackgroundImageBorder(Thickness(8));
	{
		element1->setBackgroundDrawMode(Sprite9DrawMode::StretchedBoxFrame);
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIStyle-BackgroundImage-3.png"));
	}
	{
		element1->setBackgroundDrawMode(Sprite9DrawMode::RepeatedBoxFrame);
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIStyle-BackgroundImage-4.png"));
	}
	{
		element1->setBackgroundDrawMode(Sprite9DrawMode::StretchedBorderFrame);
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIStyle-BackgroundImage-5.png"));
	}
	{
		element1->setBackgroundDrawMode(Sprite9DrawMode::RepeatedBorderFrame);
		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/UIStyle-BackgroundImage-6.png"));
	}

	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//## Border
TEST_F(Test_UI_UIStyle, Border)
{
	auto element1 = makeObject<Test_UI_UIStyle_Element>();
	element1->setAlignments(UIAlignment::Center);
	element1->setWidth(80);
	element1->setHeight(60);
	Engine::mainUIView()->addElement(element1);

	//* [ ] Basic border
	{
		element1->setBorderThickness(5);
		element1->setBorderColor(Color::Red);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/Border-1.png"));
	}

	//* [ ] Border and Background
	{
		element1->setBackgroundColor(Color::Red);
		element1->setBorderThickness(5);
		element1->setBorderColor(Color::Green);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/Border-2.png"));
	}

	//* [ ] partially border
	{
		element1->setBackgroundColor(Color::White);
		element1->setBorderThickness(Thickness(0, 0, 0, 5));
		element1->setBorderColor(Color::Green);

		TestEnv::updateFrame();
		ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/Border-3.png"));
	}

	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//## BoxShadow
TEST_F(Test_UI_UIStyle, BoxShadow)
{
	struct Test
	{
		int a;
		Test() {}
		Test(std::initializer_list<int> list) {}
	};

	Test t;
	t = { 10 };

	//auto element1 = makeObject<Test_UI_UIStyle_Element>();
	//element1->setHAlignment(UIHAlignment::Center);
	//element1->setVAlignment(UIVAlignment::Center);
	//element1->setWidth(80);
	//element1->setHeight(60);
	//Engine::mainUIView()->addElement(element1);

	//element1->setBackgroundColor(Color::Red);

	//TestEnv::updateFrame();
	//ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/BoxShadow-1.png"));
	//LN_TEST_CLEAN_SCENE;
}

//#endif // LN_UNIT_TEST_EXPERIMENTAL
