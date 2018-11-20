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
        auto sprite1 = Sprite::create(3, 3, tex1);
        sprite1->setPosition(-6, 0, 0);
        sprite1->setVisible(false);

        //* [ ] Normal
        auto sprite2 = Sprite::create(3, 3, tex1);
        sprite2->setPosition(-3, 0, 0);

        //* [ ] Opacity
        auto sprite3 = Sprite::create(3, 3, tex1);
        sprite3->setPosition(-6, 3, 0);
        sprite3->setBlendMode(BlendMode::Alpha);
        sprite3->setOpacity(0.5);

        //* [ ] ColorScale
        auto sprite4 = Sprite::create(3, 3, tex1);
        sprite4->setPosition(-3, 3, 0);
        sprite4->setColorScale(Color(1, 0, 0, 1));

        //* [ ] BlendColor
        auto sprite5 = Sprite::create(3, 3, tex1);
        sprite5->setPosition(0, 3, 0);
        sprite5->setBlendColor(Color(1, 0, 0, 1));

        //* [ ] Tone
        auto sprite6 = Sprite::create(3, 3, tex1);
        sprite6->setPosition(3, 3, 0);
        sprite6->setTone(ToneF(0.5, 0.3, 0.1, 1.0));

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Visual_VisualComponent-BuiltinEffects-1.png"));
	}
}

//------------------------------------------------------------------------------
//## BlendMode
TEST_F(Test_Visual_VisualComponent, BlendMode)
{
    {
        auto light1 = AmbientLight::create();

        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Normal (Default)
        auto sprite1 = Sprite::create(3, 3, tex1);
        sprite1->setPosition(-6, 0, 0);

        //* [ ] Alpha
        auto sprite2 = Sprite::create(3, 3, tex1);
        sprite2->setPosition(-3, 0, 0);
        sprite2->setBlendMode(BlendMode::Alpha);

        //* [ ] Add
        auto sprite3 = Sprite::create(3, 3, tex1);
        sprite3->setPosition(0, 0, 0);
        sprite3->setBlendMode(BlendMode::Add);

        //* [ ] Subtract
        auto sprite4 = Sprite::create(3, 3, tex1);
        sprite4->setPosition(3, 0, 0);
        sprite4->setBlendMode(BlendMode::Subtract);

        //* [ ] Multiply
        auto sprite5 = Sprite::create(3, 3, tex1);
        sprite5->setPosition(6, 0, 0);
        sprite5->setBlendMode(BlendMode::Multiply);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Visual_VisualComponent-BlendMode-1.png"));
    }
}
