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
        auto tex1 = Texture2D::load(LN_ASSETFILE("Sprite1.png"));

        // Hide
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(-6, 0, 0)
            .visible(false)
            .buildInto();

        // Normal
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(-3, 0, 0)
            .buildInto();

        // Opacity
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(-6, 3, 0)
            .blendMode(BlendMode::Alpha)
            .opacity(0.5)
            .buildInto();

        // ColorScale
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(-3, 3, 0)
            .colorScale(Color(1, 0, 0, 1))
            .buildInto();

        // BlendColor
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(0, 3, 0)
            .blendColor(Color(1, 0, 0, 1))
            .buildInto();

        // Tone
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(3, 3, 0)
            .colorTone(ColorTone(0.5, 0.3, 0.1, 1.0))
            .buildInto();

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
        auto tex1 = Texture2D::load(LN_ASSETFILE("Sprite1.png"));

        // Normal
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(-6, 0, 0)
            .shadingModel(ShadingModel::Unlit)
            .blendMode(BlendMode::Normal)
            .buildInto();

        // Alpha
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(-3, 0, 0)
            .shadingModel(ShadingModel::Unlit)
            .blendMode(BlendMode::Alpha)
            .buildInto();

        // Add
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(0, 0, 0)
            .shadingModel(ShadingModel::Unlit)
            .blendMode(BlendMode::Add)
            .buildInto();

        // Subtract
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(3, 0, 0)
            .shadingModel(ShadingModel::Unlit)
            .blendMode(BlendMode::Subtract)
            .buildInto();

        // Multiply
        Sprite::With()
            .texture(tex1)
            .size(3, 3)
            .position(6, 0, 0)
            .shadingModel(ShadingModel::Unlit)
            .blendMode(BlendMode::Multiply)
            .buildInto();

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
        Sprite::With()
            .texture(texture1)
            .size(5, 5)
            .position(0, 0, 0)
#ifdef LN_COORD_RH
            .rotation(0, Math::PI / 4, 0)
#else
            .rotation(0, -Math::PI / 4, 0)
#endif
            .shadingModel(ShadingModel::Unlit)
            .buildInto();

        Sprite::With()
            .texture(texture2)
            .size(5, 5)
            .position(0, 0, 0)
#ifdef LN_COORD_RH
            .rotation(0, -Math::PI / 4, 0)
#else
            .rotation(0, Math::PI / 4, 0)
#endif
            .shadingModel(ShadingModel::Unlit)
            .buildInto();

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-DepthTest-1.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] disable depth test
    {
        Sprite::With()
            .texture(texture1)
            .size(4, 4)
            .position(0, 0, 0)
#ifdef LN_COORD_RH
            .rotation(0, Math::PI / 4, 0)
#else
            .rotation(0, -Math::PI / 4, 0)
#endif
            .shadingModel(ShadingModel::Unlit)
            .buildInto();

        Sprite::With()
            .texture(texture2)
            .size(4, 4)
            .position(0, 0, 0)
#ifdef LN_COORD_RH
            .rotation(0, -Math::PI / 4, 0)
#else
            .rotation(0, Math::PI / 4, 0)
#endif
            .shadingModel(ShadingModel::Unlit)
            .depthTest(false)
            .buildInto();

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-DepthTest-2.png"));
        LN_TEST_CLEAN_SCENE;
    }

    //* [ ] disable depth write
    {
        Sprite::With()
            .texture(texture1)
            .size(4, 4)
            .position(0, 0, 0)
#ifdef LN_COORD_RH
            .rotation(0, Math::PI / 4, 0)
#else
            .rotation(0, -Math::PI / 4, 0)
#endif
            .shadingModel(ShadingModel::Unlit)
            .depthWrite(false)
            .buildInto();

        Sprite::With()
            .texture(texture2)
            .size(4, 4)
            .position(0, 0, 0)
#ifdef LN_COORD_RH
            .rotation(0, -Math::PI / 4, 0)
#else
            .rotation(0, Math::PI / 4, 0)
#endif
            .shadingModel(ShadingModel::Unlit)
            .buildInto();

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
        Sprite::With()
            .texture(texture1)
            .size(3, 3)
            .position(-1.5, 3, 0)
            .shadingModel(ShadingModel::Unlit)
            .buildInto();

		// 裏面は Sprite と Mesh で異なるのでここではテストしない

        // Front, FrontFace -> Hide
        Sprite::With()
            .texture(texture1)
            .size(3, 3)
            .position(-1.5, 3, 0)
            .shadingModel(ShadingModel::Unlit)
            .cullMode(CullMode::Front)
            .buildInto();

        // Front, BackFace -> Hide
        Sprite::With()
            .texture(texture2)
            .size(3, 3)
            .position(1.5, 0, 0)
            .rotation(0, Math::PI, 0)
            .shadingModel(ShadingModel::Unlit)
            .cullMode(CullMode::Front)
            .buildInto();

        // None, FrontFace -> Visible
        Sprite::With()
            .texture(texture1)
            .size(3, 3)
            .position(-1.5, -3, 0)
            .shadingModel(ShadingModel::Unlit)
            .cullMode(CullMode::None)
            .buildInto();

        // None, BackFace -> Visible
        Sprite::With()
            .texture(texture2)
            .size(3, 3)
            .position(1.5, -3, 0)
            .rotation(0, Math::PI, 0)
            .shadingModel(ShadingModel::Unlit)
            .cullMode(CullMode::None)
            .buildInto();

        TestEnv::updateFrame();
        ASSERT_SCREEN(LN_ASSETFILE("Visual/Result/Test_Visual_VisualComponent-CullMode-1.png"));
        LN_TEST_CLEAN_SCENE;
    }
}
