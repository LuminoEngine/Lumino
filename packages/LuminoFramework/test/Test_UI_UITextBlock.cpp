#include "Common.hpp"

//==============================================================================
//# Testing Test_UI_UITextBlock
class Test_UI_UITextBlock : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UITextBlock, Basic)
{
	auto text1 = UIText::create();
	text1->setText(_TT("ABCHIJabchij"));
	text1->setTextColor(Color::White);
	text1->setAlignments(UIAlignment::TopLeft);
	Engine::mainUIView()->addElement(text1);

    auto text2 = UIText::create();
    text2->setText(_TT("ABCHIJabchij"));
    text2->setTextColor(Color::White);
    text2->setPosition(0, 60, 0);
    text2->setAlignments(UIAlignment::TopLeft);
    Engine::mainUIView()->addElement(text2);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/Test_UI_UITextBlock-Basic-1.png"));
    // TODO: DrawCall 確認
	LN_TEST_CLEAN_SCENE;
}

