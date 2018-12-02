﻿#include "Common.hpp"

//==============================================================================
//# Testing UISprite
class Test_UI_UISprite : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UISprite, Basic)
{
	{
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        auto sprite1 = newObject<UISprite>();
        sprite1->setTexture(tex1);
        sprite1->setPosition(20, 0, 0);

        auto sprite2 = newObject<UISprite>();
        sprite2->setTexture(tex1);
        sprite2->setPosition(0, 32, 0);
        sprite2->setSourceRect(Rect(0, 0, 16, 16));

        auto sprite3 = newObject<UISprite>();
        sprite3->setTexture(tex1);
        sprite3->setPosition(32, 32, 0);
        sprite3->setSourceRect(16, 16, 16, 16);

        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Result/Test_UI_UISprite-Basic-1.png"));
	}
}

//------------------------------------------------------------------------------
//## BlendMode
TEST_F(Test_UI_UISprite, BlendMode)
{
}