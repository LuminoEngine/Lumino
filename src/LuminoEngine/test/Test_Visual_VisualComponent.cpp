#include "Common.hpp"

//==============================================================================
//# Testing VisualComponent
class Test_Visual_VisualComponent : public LuminoSceneTest {};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Visual_VisualComponent, BuiltinEffects)
{
    // TODO: Builtin shader ごとに行う
	{
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Hide
        auto sprite1 = Sprite::create(tex1, 3, 3);
        //sprite1->setShadingModel(ShadingModel::UnLighting);
        sprite1->setPosition(-6, 0, 0);
        sprite1->setVisible(false);

        //* [ ] Normal
        auto sprite2 = Sprite::create(tex1, 3, 3);
        //sprite2->setShadingModel(ShadingModel::UnLighting);
        sprite2->setPosition(-3, 0, 0);

        //* [ ] Opacity
        auto sprite3 = Sprite::create(tex1, 3, 3);
        //sprite3->setShadingModel(ShadingModel::UnLighting);
        sprite3->setPosition(-6, 3, 0);
        sprite3->setBlendMode(BlendMode::Alpha);
        sprite3->setOpacity(0.5);

        //* [ ] ColorScale
        auto sprite4 = Sprite::create(tex1, 3, 3);
        //sprite4->setShadingModel(ShadingModel::UnLighting);
        sprite4->setPosition(-3, 3, 0);
        sprite4->setColorScale(Color(1, 0, 0, 1));

        //* [ ] BlendColor
        auto sprite5 = Sprite::create(tex1, 3, 3);
        //sprite5->setShadingModel(ShadingModel::UnLighting);
        sprite5->setPosition(0, 3, 0);
        sprite5->setBlendColor(Color(1, 0, 0, 1));

        //* [ ] Tone
        auto sprite6 = Sprite::create(tex1, 3, 3);
        //sprite6->setShadingModel(ShadingModel::UnLighting);
        sprite6->setPosition(3, 3, 0);
        sprite6->setTone(ColorTone(0.5, 0.3, 0.1, 1.0));

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-BuiltinEffects-1.png"));
        LN_TEST_CLEAN_SCENE;
	}
}

//------------------------------------------------------------------------------
//## BlendMode
TEST_F(Test_Visual_VisualComponent, BlendMode)
{
    {
        auto tex1 = Assets::loadTexture(LN_ASSETFILE("Sprite1.png"));

        //* [ ] Normal
        auto sprite1 = Sprite::create(tex1, 3, 3);
        sprite1->setShadingModel(ShadingModel::UnLighting);
        sprite1->setPosition(-6, 0, 0);
        sprite1->setBlendMode(BlendMode::Normal);

        //* [ ] Alpha
        auto sprite2 = Sprite::create(tex1, 3, 3);
        sprite2->setShadingModel(ShadingModel::UnLighting);
        sprite2->setPosition(-3, 0, 0);
        sprite2->setBlendMode(BlendMode::Alpha);

        //* [ ] Add
        auto sprite3 = Sprite::create(tex1, 3, 3);
        sprite3->setShadingModel(ShadingModel::UnLighting);
        sprite3->setPosition(0, 0, 0);
        sprite3->setBlendMode(BlendMode::Add);

        //* [ ] Subtract
        auto sprite4 = Sprite::create(tex1, 3, 3);
        sprite4->setShadingModel(ShadingModel::UnLighting);
        sprite4->setPosition(3, 0, 0);
        sprite4->setBlendMode(BlendMode::Subtract);

        //* [ ] Multiply
        auto sprite5 = Sprite::create(tex1, 3, 3);
        sprite5->setShadingModel(ShadingModel::UnLighting);
        sprite5->setPosition(6, 0, 0);
        sprite5->setBlendMode(BlendMode::Multiply);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-BlendMode-1.png"));
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
        auto sprite1 = Sprite::create(texture1, 5, 5);
        sprite1->setPosition(0, 0, 0);
        sprite1->setEulerAngles(0, -Math::PI / 4, 0);
        sprite1->setShadingModel(ShadingModel::UnLighting);

        auto sprite2 = Sprite::create(texture2, 5, 5);
        sprite2->setPosition(0, 0, 0);
        sprite2->setEulerAngles(0, Math::PI / 4, 0);
        sprite2->setShadingModel(ShadingModel::UnLighting);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-DepthTest-1.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] disable depth test
    {
        auto sprite1 = Sprite::create(texture1, 4, 4);
        sprite1->setPosition(0, 0, 0);
        sprite1->setEulerAngles(0, -Math::PI / 4, 0);
        sprite1->setShadingModel(ShadingModel::UnLighting);

        auto sprite2 = Sprite::create(texture2, 4, 4);
        sprite2->setPosition(0, 0, 0);
        sprite2->setEulerAngles(0, Math::PI / 4, 0);
        sprite2->setShadingModel(ShadingModel::UnLighting);
        sprite2->setDepthTestEnabled(false);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-DepthTest-2.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] disable depth write
    {
        auto sprite1 = Sprite::create(texture1, 4, 4);
        sprite1->setPosition(0, 0, 0);
        sprite1->setEulerAngles(0, -Math::PI / 4, 0);
        sprite1->setShadingModel(ShadingModel::UnLighting);
        sprite1->setDepthWriteEnabled(false);

        auto sprite2 = Sprite::create(texture2, 4, 4);
        sprite2->setPosition(0, 0, 0);
        sprite2->setEulerAngles(0, Math::PI / 4, 0);
        sprite2->setShadingModel(ShadingModel::UnLighting);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-DepthTest-3.png"));
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
        auto sprite1 = Sprite::create(texture1, 3, 3);
        sprite1->setShadingModel(ShadingModel::UnLighting);
        sprite1->setPosition(-1.5, 3, 0);

		// 裏面は Sprite と Mesh で異なるのでここではテストしない

        // Front, FrontFace -> Hide
        auto sprite3 = Sprite::create(texture1, 3, 3);
        sprite3->setShadingModel(ShadingModel::UnLighting);
        sprite3->setPosition(-1.5, 0, 0);
        sprite3->setCullMode(CullMode::Front);

        // Front, BackFace -> Hide
        auto sprite4 = Sprite::create(texture2, 3, 3);
        sprite4->setShadingModel(ShadingModel::UnLighting);
        sprite4->setPosition(1.5, 0, 0);
        sprite4->setCullMode(CullMode::Front);
        sprite4->setEulerAngles(0, Math::PI, 0);

        // None, FrontFace -> Visible
        auto sprite5 = Sprite::create(texture1, 3, 3);
        sprite5->setShadingModel(ShadingModel::UnLighting);
        sprite5->setPosition(-1.5, -3, 0);
        sprite5->setCullMode(CullMode::None);

        // None, BackFace -> Visible
        auto sprite6 = Sprite::create(texture2, 3, 3);
        sprite6->setShadingModel(ShadingModel::UnLighting);
        sprite6->setCullMode(CullMode::None);
        sprite6->setPosition(1.5, -3, 0);
        sprite6->setEulerAngles(0, Math::PI, 0);

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-CullMode-1.png"));
        LN_TEST_CLEAN_SCENE;
    }
}
