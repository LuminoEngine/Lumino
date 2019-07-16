#include "Common.hpp"

//#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# Testing Test_UI_UIStyle
class Test_UI_UIStyle : public LuminoSceneTest {};

class Test_UI_UIStyle_Element : public UIElement
{
public:

protected:
	virtual const String& className() const  override { static String name = u"TestElement"; return name; }
};

//------------------------------------------------------------------------------
//## Background
TEST_F(Test_UI_UIStyle, Background)
{
	auto element1 = makeObject<Test_UI_UIStyle_Element>();
	element1->setHorizontalAlignment(HAlignment::Center);
	element1->setVerticalAlignment(VAlignment::Center);
	element1->setWidth(80);
	element1->setHeight(60);

	element1->setBackgroundColor(Color::Red);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/Background-1.png"));
	LN_TEST_CLEAN_SCENE;
}

//------------------------------------------------------------------------------
//## Border
TEST_F(Test_UI_UIStyle, Border)
{
	auto element1 = makeObject<Test_UI_UIStyle_Element>();
	element1->setHorizontalAlignment(HAlignment::Center);
	element1->setVerticalAlignment(VAlignment::Center);
	element1->setWidth(80);
	element1->setHeight(60);

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
	auto element1 = makeObject<Test_UI_UIStyle_Element>();
	element1->setHorizontalAlignment(HAlignment::Center);
	element1->setVerticalAlignment(VAlignment::Center);
	element1->setWidth(80);
	element1->setHeight(60);

	element1->setBackgroundColor(Color::Red);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UIStyle/BoxShadow-1.png"));
	LN_TEST_CLEAN_SCENE;
}

//#endif // LN_UNIT_TEST_EXPERIMENTAL
