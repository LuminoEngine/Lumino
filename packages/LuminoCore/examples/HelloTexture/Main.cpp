/**
 * HelloTexture.cpp
 *
 * PNG 画像を TextureLoader で読み込み、四角形 Mesh に貼り付けて描画するデモ。
 */

#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/ForwardRenderer.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <LuminoCore/Graphics/Transform.hpp>
#include <LuminoCore/Graphics/TextureLoader.hpp>

#include <cstdio>
#include <vector>

using namespace ln;
using namespace ln::rhi;
using namespace ln::platform;

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // 1. CoreInstance + Window + GraphicsContext
    CoreInstance::Settings coreSettings;
    coreSettings.preferredBackend = Backend::Vulkan;
    coreSettings.enableValidation = true;
    (void)CoreInstance::initialize(coreSettings);
    {
        WindowDesc winDesc;
        winDesc.title = "Lumino - Hello Texture";
        winDesc.width = 1280;
        winDesc.height = 720;

        GraphicsContextDesc gfxDesc;

        auto window = *PlatformWindow::create(CoreInstance::instance()->graphicsModule(), winDesc, gfxDesc);
        auto* ctx = window->graphicsContext();

        // 2. ForwardRenderer
        auto renderer = *ForwardRenderer::create(ctx);

        // 3. Unlit マテリアル
        auto material = *MaterialFactory::createUnlit(ctx);

        // 4. PNG テクスチャ読み込み
        auto texture = *TextureLoader::loadFromFile(ctx->device(), ASSETS_DIR "/picture1.png");
        material->setTexture(texture.get());

        // 5. UV付き四角形メッシュ (2枚の三角形)
        //   v0(-0.5, 0.5) --- v1(0.5, 0.5)
        //      |           /      |
        //   v2(-0.5,-0.5) --- v3(0.5,-0.5)
        Vertex v0{};
        v0.position = {-0.5f,  0.5f, 0.0f};
        v0.uv       = {0.0f, 0.0f};
        v0.color    = {1.0f, 1.0f, 1.0f, 1.0f};

        Vertex v1{};
        v1.position = { 0.5f,  0.5f, 0.0f};
        v1.uv       = {1.0f, 0.0f};
        v1.color    = {1.0f, 1.0f, 1.0f, 1.0f};

        Vertex v2{};
        v2.position = {-0.5f, -0.5f, 0.0f};
        v2.uv       = {0.0f, 1.0f};
        v2.color    = {1.0f, 1.0f, 1.0f, 1.0f};

        Vertex v3{};
        v3.position = { 0.5f, -0.5f, 0.0f};
        v3.uv       = {1.0f, 1.0f};
        v3.color    = {1.0f, 1.0f, 1.0f, 1.0f};

        std::vector<Vertex> vertices = {v0, v1, v2, v3};
        // CCW: 上三角 (v0,v2,v1) + 下三角 (v1,v2,v3)
        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};
        SubMesh sub;
        sub.indexOffset = 0;
        sub.indexCount = 6;
        sub.materialIndex = 0;

        auto mesh = *Mesh::create(ctx->device(), vertices, indices, {sub});
        mesh->materials() = {material};

        // 7. Perspective カメラ
        Camera camera;
        camera.setPerspective(
            60.0f * 3.14159f / 180.0f,
            static_cast<float>(ctx->width()) / static_cast<float>(ctx->height()),
            0.1f,
            100.0f);
        camera.setLookAt({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f});

        // 8. メインループ
        while (window->processEvents()) {
            RenderObject obj;
            obj.mesh = mesh;


            const FramebufferInfo* fb = *ctx->beginFrame(ctx->width(), ctx->height());

            auto _ = renderer->renderFrame(
                fb->colorTexture->rhiTextureView(),
                fb->depthTexture->rhiTextureView(),
                camera, &obj, 1, Color{0, 0, 0, 1.0f});

            ctx->endFrame();
        }

        ctx->waitIdle();

    }
    printf("Done.\n");
    CoreInstance::terminate();
    return 0;
}
