﻿#include "Common.hpp"

//==============================================================================
//# Testing Test_UI_UITextBlock
class Test_UI_UITextBlock : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UITextBlock, Basic)
{
	auto text1 = UITextBlock::create();
	text1->setText("Test");

	TestEnv::updateFrame();
	ASSERT_SCREEN_S(LN_ASSETFILE("UI/Result/Test_UI_UITextBlock-Basic-1.png"));
	LN_TEST_CLEAN_SCENE;
}