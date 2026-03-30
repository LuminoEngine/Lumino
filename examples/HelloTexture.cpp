/**
 * HelloTexture.cpp
 *
 * PNG 画像を TextureLoader で読み込み、四角形 Mesh に貼り付けて描画するデモ。
 */

#include <lumino_core/platform/Window.hpp>
#include <lumino_core/graphics/GraphicsContext.hpp>
#include <lumino_core/graphics/ForwardRenderer.hpp>
#include <lumino_core/graphics/Camera.hpp>
#include <lumino_core/graphics/Mesh.hpp>
#include <lumino_core/graphics/Material.hpp>
#include <lumino_core/graphics/Vertex.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <lumino_core/graphics/TextureLoader.hpp>

#include <cstdio>
#include <vector>

using namespace ln;
using namespace ln::rhi;
using namespace ln::platform;

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // 1. Window + GraphicsContext
    WindowDesc winDesc;
    winDesc.title = "Lumino - Hello Texture";
    winDesc.width = 1280;
    winDesc.height = 720;

    GraphicsContextDesc gfxDesc;
    gfxDesc.preferredBackend = Backend::Vulkan;
    gfxDesc.enableValidation = true;

    auto window = *PlatformWindow::create(winDesc, gfxDesc);
    auto* ctx = window->graphicsContext();

    // 2. ForwardRenderer
    auto renderer = *ForwardRenderer::create(ctx);

    // 3. Unlit Material
    auto material = *MaterialFactory::createUnlit(ctx, renderer->pipelineLayout());

    // 4. PNG テクスチャ読み込み
    auto texture = *TextureLoader::loadFromFile(ctx->device(), ASSETS_DIR "/picture1.png");
    material->setTexture(texture.get());

    // 5. Pipeline & BindGroup ビルド
    material->buildPipeline(
        ctx->device(),
        renderer->pipelineLayout(),
        ctx->colorFormat(),
        ctx->depthFormat());
    material->updateBindGroup(ctx->device());

    // 6. UV付き四角形メッシュ (2枚の三角形)
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
    std::vector<u32> indices = {0, 2, 1, 1, 2, 3};
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
        static_cast<f32>(ctx->width()) / static_cast<f32>(ctx->height()),
        0.1f,
        100.0f);
    camera.setLookAt({0.0f, 0.0f, 3.0f}, {0.0f, 0.0f, 0.0f});

    printf("Lumino HelloTexture initialized. Rendering...\n");

    // 8. Main loop
    while (window->processEvents()) {
        RenderObject obj;
        obj.mesh = mesh;

        std::vector<RenderObject> objects = {obj};

        auto frame = *ctx->beginFrame();

        renderer->renderFrame(
            ctx->device(), frame.colorTarget, frame.depthTarget,
            camera, objects, Color{0, 0, 0, 1.0f});

        ctx->endFrame();
    }

    ctx->waitIdle();

    printf("Done.\n");
    return 0;
}
