#include "Common.hpp"

//==============================================================================
//# Testing Test_UI_UITextBlock
class Test_UI_UITextBlock : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UITextBlock, Basic)
{
	auto text1 = UITextBlock::create();
	text1->setText(u"ABCHIJabchij");
	text1->setTextColor(Color::White);
	Engine::mainUIRoot()->addElement(text1);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("UI/Result/Test_UI_UITextBlock-Basic-1.png"));
	LN_TEST_CLEAN_SCENE;
}

