#include "TestEnv.hpp"

class Test_Rendering_Sprite : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        TestEnv::initializeRendering();
    }
};


TEST_F(Test_Rendering_Sprite, Basic) {
    RESET_VIEW();
    auto texture1 = Texture2D::load(LN_ASSETFILE("Rendering/Sprite3.png"));
    auto material1 = Material::create(texture1);

    auto* commandList = TestEnv::swapChain->currentCommandList2();
    commandList->beginCommandRecoding();
    auto* target = TestEnv::swapChain->currentBackbuffer();
    auto* ctx = TestEnv::renderView->begin(commandList, target);

    // 3D (SpriteBaseDirection::ZPlus) では anchor の原点が左下になる
    {
        auto* spriteRenderer = SpriteRenderer::get();
        spriteRenderer->begin(ctx, material1);
        spriteRenderer->drawSprite(Matrix::Identity, Size(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::White, SpriteBaseDirection::ZPlus, BillboardType::None, SpriteFlipFlags::None); 
        spriteRenderer->end();
    }
    
    TestEnv::renderView->end();
    commandList->endCommandRecoding();
    TestEnv::swapChain->present();
    ASSERT_RENDERTARGET(LN_ASSETFILE("Rendering/Expects/Test_Rendering_Sprite.Basic.png"), target);
}

