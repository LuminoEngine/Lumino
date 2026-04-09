/**
 * HelloMesh.cpp
 *
 * Mesh API を使って色付き三角形を描画するデモ。
 * VertexBuffer や IndexBuffer を直接操作せず、Mesh にカプセル化された
 * 高レベル API のみで描画を行う。
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
    CoreInstance::initialize(coreSettings);
    {
        WindowDesc winDesc;
        winDesc.title = "Lumino - Hello Mesh";
        winDesc.width = 1280;
        winDesc.height = 720;

        GraphicsContextDesc gfxDesc;

        auto window = *PlatformWindow::create(CoreInstance::instance()->graphicsModule(), winDesc, gfxDesc);
        auto* ctx = window->graphicsContext();

        // 2. ForwardRenderer
        auto renderer = *ForwardRenderer::create(ctx);

        // 3. Unlit Material (white color so vertex colors show through)
        auto material = *MaterialFactory::createUnlit(ctx);
        //material->setCullMode(CullMode::None);

        // 4. Triangle mesh with per-vertex colors
        // 反時計回り (CCW) が正面。右手座標系ということで。godot と同じ。
        Vertex v0{};
        v0.position = {0.0f, 0.5f, 0.0f};
        v0.color = {1.0f, 0.0f, 0.0f, 1.0f};
        Vertex v1{};
        v1.position = {-0.5f, -0.5f, 0.0f};
        v1.color = {0.0f, 1.0f, 0.0f, 1.0f};
        Vertex v2{};
        v2.position ={0.5f, -0.5f, 0.0f};
        v2.color = {0.0f, 0.0f, 1.0f, 1.0f};

        std::vector<Vertex> vertices = {v0, v1, v2};
        std::vector<u32> indices = {0, 1, 2};
        SubMesh sub;
        sub.indexOffset = 0;
        sub.indexCount = 3;
        sub.materialIndex = 0;

        auto mesh = *Mesh::create(ctx->device(), vertices, indices, {sub});
        mesh->materials() = {material};

        // 5. Orthographic camera looking at the origin
        Camera camera;
        camera.setPerspective(
            //0.3f,
            60.0f * 3.14159f / 180.0f,
            static_cast<f32>(ctx->width()) / static_cast<f32>(ctx->height()),
            0.1f,
            100.0f);
        camera.setLookAt({0.0f, 0.0f, 5.f}, {0.0f, 0.0f, 0.0f});

        // 6. Main loop
        int frameCount = 0;
        while (window->processEvents()) {
            RenderObject obj;
            obj.mesh = mesh;
            obj.transform.position.x = 0.5;
            obj.transform.rotation = Quaternion::fromAxisAngle(Vector3::unitY(), (float)frameCount * 0.1f);
            std::vector<RenderObject> objects = {obj};

            const FramebufferInfo* fb = *ctx->beginFrame();

            auto _ = renderer->renderFrame(
                fb->colorTexture->rhiTextureView(),
                fb->depthTexture->rhiTextureView(),
                camera, objects, Color{0.1f, 0.1f, 0.15f, 1.0f});

            ctx->endFrame();
            frameCount++;
        }

        // 現状、wait しないと実行中 Command が使っているリソースを解放しようとしてしまうので、必要。
        ctx->waitIdle();

        printf("Done.\n");
    }
    CoreInstance::terminate();
    return 0;
}
