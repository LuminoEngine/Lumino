#include "Common.hpp"

#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# Testing Test_UI_UITextBlock
class Test_UI_UITextBlock : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UITextBlock, Basic)
{
	auto text1 = UITextBlock::create();
	text1->setText("Test");
	text1->setTextColor(Color::White);

	TestEnv::updateFrame();
	ASSERT_SCREEN_S(LN_ASSETFILE("UI/Result/Test_UI_UITextBlock-Basic-1.png"));
	LN_TEST_CLEAN_SCENE;
}

#endif // LN_UNIT_TEST_EXPERIMENTAL
