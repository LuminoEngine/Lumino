#include "Common.hpp"

#ifdef LN_UNIT_TEST_EXPERIMENTAL

//==============================================================================
//# Testing UISprite
class Test_UI_UISprite : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UISprite, Basic)
{
	{
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        auto sprite1 = makeObject<UISprite>();
        sprite1->setTexture(tex1);
        sprite1->setPosition(20, 0, 0);
		Engine::mainUIRoot()->addElement(sprite1);

        auto sprite2 = makeObject<UISprite>();
        sprite2->setTexture(tex1);
        sprite2->setPosition(0, 32, 0);
        sprite2->setSourceRect(Rect(0, 0, 16, 16));
		Engine::mainUIRoot()->addElement(sprite2);

        auto sprite3 = makeObject<UISprite>();
        sprite3->setTexture(tex1);
        sprite3->setPosition(32, 32, 0);
        sprite3->setSourceRect(16, 16, 16, 16);
		Engine::mainUIRoot()->addElement(sprite3);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_UI_UISprite-Basic-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## BlendMode
// https://developer.mozilla.org/ja/docs/Web/CSS/CSS_Backgrounds_and_Borders
TEST_F(Test_UI_UISprite, BackgroundsAndBorders)
{
}

#endif // LN_UNIT_TEST_EXPERIMENTAL
