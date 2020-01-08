#include "Common.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>

//==============================================================================
class Test_Rendering_RenderingContext : public LuminoSceneTest
{
public:
    class TestElement : public UIElement
    {
    public:
        std::function<void(UIRenderingContext* context)> render;

        virtual void onRender(UIRenderingContext* context) override { render(context); }
    };
};

//------------------------------------------------------------------------------
//## Basic
TEST_F(Test_Rendering_RenderingContext, Basic)
{
    auto element1 = makeObject<TestElement>();
    element1->setHorizontalAlignment(HAlignment::Stretch);
    element1->setVerticalAlignment(VAlignment::Stretch);
    Engine::mainUIView()->addChild(element1);

    auto tex1 = Texture2D::load(LN_ASSETFILE("Sprite1.png"));
    auto mat1 = makeObject<Material>(tex1);

    auto tex2 = RenderTargetTexture::create(160, 120);
    auto mat2 = makeObject<Material>(tex2);

    auto tex3 = RenderTargetTexture::create(160, 120);
    auto mat3 = makeObject<Material>(tex3);

    //auto shader = makeObject<Shader>(u"D:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/ToneImageEffect.fx");
    //mat2->setShader(shader);

    element1->render = [&](UIRenderingContext* context)
    {
        context->blit(mat1, tex2);


        context->blit(mat2, tex3);
        context->blit(mat3, tex2);

        context->blit(mat2, nullptr);
    };

    TestEnv::updateFrame();
    ASSERT_SCREEN_S(LN_ASSETFILE("Rendering/Expects/RenderingContext-Basic-1.png"));
    LN_TEST_CLEAN_SCENE;
}

