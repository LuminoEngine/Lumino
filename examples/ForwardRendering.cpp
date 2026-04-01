/**
 * ForwardRendering.cpp
 *
 * Phase 2 デモ: ForwardRenderer でプロシージャルメッシュを描画する。
 * Camera, Material, Mesh, ForwardRenderer の動作確認。
 *
 * PlatformWindow でウィンドウを作成し、
 * window->graphicsContext() で Device + SwapChain + DepthBuffer を取得する。
 */

#include <LuminoCore/platform/Window.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <LuminoCore/graphics/ForwardRenderer.hpp>
#include <LuminoCore/graphics/Camera.hpp>
#include <LuminoCore/graphics/Mesh.hpp>
#include <LuminoCore/graphics/Material.hpp>
#include <LuminoCore/graphics/Vertex.hpp>
#include <LuminoCore/graphics/Transform.hpp>
#include <LuminoBase/math/Math.hpp>

#include <cstdio>
#include <cmath>
#include <vector>

using namespace ln;
using namespace ln::rhi;
using namespace ln::platform;

/// Generate a unit cube mesh (24 vertices, 36 indices).
static Result<Ref<Mesh>> createCubeMesh(Device* device) {
    std::vector<Vertex> verts;
    std::vector<u32> indices;

    auto addFace = [&](Vector3 n, Vector3 u, Vector3 v) {
        u32 base = static_cast<u32>(verts.size());
        Vector3 p0 = Vector3{n.x - u.x - v.x, n.y - u.y - v.y, n.z - u.z - v.z};
        Vector3 p1 = Vector3{n.x + u.x - v.x, n.y + u.y - v.y, n.z + u.z - v.z};
        Vector3 p2 = Vector3{n.x + u.x + v.x, n.y + u.y + v.y, n.z + u.z + v.z};
        Vector3 p3 = Vector3{n.x - u.x + v.x, n.y - u.y + v.y, n.z - u.z + v.z};
        Vector3 nn = n.normalized();
        Vertex vtx{};
        vtx.color = Color::white();
        vtx.normal = nn;

        vtx.position = p0; vtx.uv = {0, 0}; verts.push_back(vtx);
        vtx.position = p1; vtx.uv = {1, 0}; verts.push_back(vtx);
        vtx.position = p2; vtx.uv = {1, 1}; verts.push_back(vtx);
        vtx.position = p3; vtx.uv = {0, 1}; verts.push_back(vtx);

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    };

    float h = 0.5f;
    addFace({0,0, h}, { h,0,0}, {0, h,0}); // +Z
    addFace({0,0,-h}, {-h,0,0}, {0, h,0}); // -Z
    addFace({ h,0,0}, {0,0,-h}, {0, h,0}); // +X
    addFace({-h,0,0}, {0,0, h}, {0, h,0}); // -X
    addFace({0, h,0}, { h,0,0}, {0,0, h}); // +Y
    addFace({0,-h,0}, { h,0,0}, {0,0,-h}); // -Y

    SubMesh sub;
    sub.indexOffset = 0;
    sub.indexCount = static_cast<u32>(indices.size());
    sub.materialIndex = 0;

    return Mesh::create(device, verts, indices, {sub});
}

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // 1. Window + GraphicsContext
    WindowDesc winDesc;
    winDesc.title = "Lumino - Forward Rendering";
    winDesc.width = 1280;
    winDesc.height = 720;

    GraphicsContextDesc gfxDesc;
    gfxDesc.preferredBackend = Backend::Vulkan;
    gfxDesc.enableValidation = true;

    auto window = *PlatformWindow::create(winDesc, gfxDesc);
    auto ctx = window->graphicsContext();

    // 2. ForwardRenderer
    auto renderer = *ForwardRenderer::create(ctx);

    // 3. Material (BasicLit)
    auto material = *MaterialFactory::createBasicLit(ctx);
    material->setColor(Color{0.8f, 0.5f, 0.2f, 1.0f});
    auto _ = material->updateBindGroup(ctx->device());

    // 4. Cube mesh
    auto mesh = *createCubeMesh(ctx->device());
    mesh->materials() = {material};

    // 5. Camera
    Camera camera;
    camera.setPerspective(60.0f * 3.14159f / 180.0f,
        static_cast<f32>(ctx->width()) / static_cast<f32>(ctx->height()),
        0.1f, 100.0f);

    // 6. Light
    DirectionalLight light;
    light.direction = Vector3{0.3f, -1.0f, 0.5f};
    light.color = Color::white();
    light.ambient = Color{0.15f, 0.15f, 0.15f, 1.0f};
    renderer->setLight(light);

    printf("Lumino ForwardRenderer initialized. Rendering...\n");

    // 7. Main loop
    f32 time = 0.0f;
    while (window->processEvents()) {
        time += 0.016f; // ~60fps

        camera.setOrbit({0, 0, 0}, 3.0f, time * 0.5f, 0.35f);

        RenderObject obj;
        obj.mesh = mesh;
        obj.transform.rotation = Quaternion::fromEuler(time * 10.0f, time * 15.0f, 0.0f);
        std::vector<RenderObject> objects = {obj};

        auto frame = *ctx->beginFrame();

        auto _ = renderer->renderFrame(
            frame.colorTarget, frame.depthTarget,
            camera, objects, Color{0.1f, 0.1f, 0.15f, 1.0f});

        ctx->endFrame();
    }

    printf("Done.\n");
    return 0;
}
