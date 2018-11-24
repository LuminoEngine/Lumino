#include "Common.hpp"

//==============================================================================
//# Testing UISprite
class Test_UI_UISprite : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_UI_UISprite, Basic)
{
	{
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Hide
        auto sprite1 = newObject<UISprite>();
        sprite1->setTexture(tex1);
        //sprite1->setPosition(-6, 0, 0);
        //sprite1->setVisible(false);
        TestEnv::updateFrame();
        ASSERT_SCREEN_S(LN_ASSETFILE("Result/Test_UI_UISprite-Basic-1.png"));
	}
}

//------------------------------------------------------------------------------
//## BlendMode
TEST_F(Test_UI_UISprite, BlendMode)
{
}
