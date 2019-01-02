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
        LN_TEST_CLEAN_SCENE;
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
        LN_TEST_CLEAN_SCENE;
    }
}

//------------------------------------------------------------------------------
//## DepthTest, DepthWrite
TEST_F(Test_Visual_VisualComponent, DepthTest)
{
    auto texture1 = Texture2D::create(32, 32);
    auto texture2 = Texture2D::create(32, 32);
    texture1->clear(Color::Red);
    texture2->clear(Color::Green);

    //* [ ] default (enabled depth test and write)
    {
        auto sprite1 = Sprite::create(5, 5, texture1);
        sprite1->setPosition(0, 0, 0);
        sprite1->setEulerAngles(0, -Math::PI / 4, 0);
        sprite1->setShadingModel(ShadingModel::UnLighting);

        auto sprite2 = Sprite::create(5, 5, texture2);
        sprite2->setPosition(0, 0, 0);
        sprite2->setEulerAngles(0, Math::PI / 4, 0);
        sprite2->setShadingModel(ShadingModel::UnLighting);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Visual/Test_Visual_VisualComponent-DepthTest-1.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] disable depth test
    {
        auto sprite1 = Sprite::create(4, 4, texture1);
        sprite1->setPosition(0, 0, 0);
        sprite1->setEulerAngles(0, -Math::PI / 4, 0);
        sprite1->setShadingModel(ShadingModel::UnLighting);

        auto sprite2 = Sprite::create(4, 4, texture2);
        sprite2->setPosition(0, 0, 0);
        sprite2->setEulerAngles(0, Math::PI / 4, 0);
        sprite2->setShadingModel(ShadingModel::UnLighting);
        sprite2->setDepthTestEnabled(false);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Visual/Test_Visual_VisualComponent-DepthTest-2.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] disable depth write
    {
        auto sprite1 = Sprite::create(4, 4, texture1);
        sprite1->setPosition(0, 0, 0);
        sprite1->setEulerAngles(0, -Math::PI / 4, 0);
        sprite1->setShadingModel(ShadingModel::UnLighting);
        sprite1->setDepthWriteEnabled(false);

        auto sprite2 = Sprite::create(4, 4, texture2);
        sprite2->setPosition(0, 0, 0);
        sprite2->setEulerAngles(0, Math::PI / 4, 0);
        sprite2->setShadingModel(ShadingModel::UnLighting);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Visual/Test_Visual_VisualComponent-DepthTest-3.png"));
        LN_TEST_CLEAN_SCENE;
    }
}

//------------------------------------------------------------------------------
//## CullMode
TEST_F(Test_Visual_VisualComponent, CullMode)
{
    auto texture1 = Texture2D::create(32, 32);
    auto texture2 = Texture2D::create(32, 32);
    texture1->clear(Color::Red);
    texture2->clear(Color::Green);

    //* [ ] CullMode
    {
        // Back(default), FrontFace -> Visible
        auto sprite1 = Sprite::create(3, 3, texture1);
        sprite1->setShadingModel(ShadingModel::UnLighting);
        sprite1->setPosition(-1.5, 3, 0);

        // Back(default), BackFace -> Hide
        auto sprite2 = Sprite::create(3, 3, texture2);
        sprite2->setShadingModel(ShadingModel::UnLighting);
        sprite2->setPosition(1.5, 3, 0);
        sprite2->setEulerAngles(0, Math::PI, 0);

        // Front, FrontFace -> Hide
        auto sprite3 = Sprite::create(3, 3, texture1);
        sprite3->setShadingModel(ShadingModel::UnLighting);
        sprite3->setPosition(-1.5, 0, 0);
        sprite3->setCullMode(CullMode::Front);

        // Front, BackFace -> Hide
        auto sprite4 = Sprite::create(3, 3, texture2);
        sprite4->setShadingModel(ShadingModel::UnLighting);
        sprite4->setPosition(1.5, 0, 0);
        sprite4->setCullMode(CullMode::Front);
        sprite4->setEulerAngles(0, Math::PI, 0);

        // None, FrontFace -> Visible
        auto sprite5 = Sprite::create(3, 3, texture1);
        sprite5->setShadingModel(ShadingModel::UnLighting);
        sprite5->setPosition(-1.5, -3, 0);
        sprite5->setCullMode(CullMode::None);

        // None, BackFace -> Visible
        auto sprite6 = Sprite::create(3, 3, texture2);
        sprite6->setShadingModel(ShadingModel::UnLighting);
        sprite6->setCullMode(CullMode::None);
        sprite6->setPosition(1.5, -3, 0);
        sprite6->setEulerAngles(0, Math::PI, 0);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Result/Visual/Test_Visual_VisualComponent-CullMode-1.png"));
        LN_TEST_CLEAN_SCENE;
    }
}

