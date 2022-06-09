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

#if LN_USE_YOGA
#else
TEST_F(Test_Rendering_RenderingContext, RenderTargetCrossDrawing)
{
    auto element1 = makeObject<TestElement>();
    element1->setHAlignment(UIHAlignment::Stretch);
    element1->setVAlignment(UIVAlignment::Stretch);
    Engine::mainUIView()->addChild(element1);

	auto tex1 = Texture2D::load(LN_ASSETFILE("Sprite2.png"));
    auto mat1 = makeObject<Material>(tex1);

    auto tex2 = RenderTargetTexture::create(160, 120);
    auto mat2 = makeObject<Material>(tex2);

    auto tex3 = RenderTargetTexture::create(160, 120);
    auto mat3 = makeObject<Material>(tex3);

    element1->render = [&](UIRenderingContext* context)
    {
        CommandList* commandList = context->getCommandList(RenderPart::PostEffect);
        commandList->blit(mat1, tex2);
        commandList->blit(mat2, tex3);	// corss-drawing
        commandList->blit(mat3, tex2);	// corss-drawing
        commandList->blit(mat2, nullptr);
    };

    TestEnv::updateFrame();
    ASSERT_SCREEN(LN_ASSETFILE("Rendering/Expects/RenderingContext-RenderTargetCrossDrawing-1.png"));
    LN_TEST_CLEAN_SCENE;
}

#endif
