#include "Common.hpp"

//==============================================================================
//# Testing VisualComponent
class Test_Visual_VisualComponent : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Visual_VisualComponent, BuiltinEffects)
{
	{
        auto light1 = AmbientLight::create();

        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Hide
        auto sprite1 = Sprite::create(2, 2, tex1);
        sprite1->setPosition(-4, 0, 0);
        sprite1->setVisible(false);

        //* [ ] Normal
        auto sprite2 = Sprite::create(2, 2, tex1);
        sprite2->setPosition(-2, 0, 0);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Visual_VisualComponent-BuiltinEffects-1.png"));
	}
}
