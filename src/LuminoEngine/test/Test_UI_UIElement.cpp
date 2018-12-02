﻿#include "Common.hpp"

//==============================================================================
//# Testing UIElement
class Test_UI_UIElement : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UIElement, RenderEffects)
{
	{
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Visible
        auto sprite1 = newObject<UISprite>();
        sprite1->setTexture(tex1);
        sprite1->setPosition(0, 0, 0);
        sprite1->setVisible(false);

        //* [ ] BlendMode
        auto sprite2 = newObject<UISprite>();
        sprite2->setTexture(tex1);
        sprite2->setBlendMode(BlendMode::Add);
        sprite2->setPosition(32, 0, 0);

        //* [ ] Opacity
        auto sprite3 = newObject<UISprite>();
        sprite3->setTexture(tex1);
        sprite3->setPosition(64, 0, 0);
        sprite3->setOpacity(0.5);

        //* [ ] ColorScale
        auto sprite4 = newObject<UISprite>();
        sprite4->setTexture(tex1);
        sprite4->setPosition(96, 0, 0);
        sprite4->setColorScale(Color(1, 0, 0, 1));

        //* [ ] BlendColor
        auto sprite5 = newObject<UISprite>();
        sprite5->setTexture(tex1);
        sprite5->setPosition(0, 32, 0);
        sprite5->setBlendColor(Color(1, 0, 0, 1));

        //* [ ] Tone
        auto sprite6 = newObject<UISprite>();
        sprite6->setTexture(tex1);
        sprite6->setPosition(32, 32, 0);
        sprite6->setTone(ToneF(0.5, 0.3, 0.1, 1.0));

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_UI_UIElement-RenderEffects-1.png"));
	}
}