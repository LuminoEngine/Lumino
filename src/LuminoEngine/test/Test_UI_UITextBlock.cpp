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
	text1->setAlignments(HAlignment::Left, VAlignment::Top);
	Engine::mainUIView()->addElement(text1);

    auto text2 = UITextBlock::create();
    text2->setText(u"ABCHIJabchij");
    text2->setTextColor(Color::White);
    text2->setPosition(0, 60, 0);
	text2->setAlignments(HAlignment::Left, VAlignment::Top);
    Engine::mainUIView()->addElement(text2);

	TestEnv::updateFrame();
	ASSERT_SCREEN(LN_ASSETFILE("UI/Expects/Test_UI_UITextBlock-Basic-1.png"));
    // TODO: DrawCall 確認
	LN_TEST_CLEAN_SCENE;
}

