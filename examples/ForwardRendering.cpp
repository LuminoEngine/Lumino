/**
 * ForwardRendering.cpp
 *
 * Phase 2 デモ: ForwardRenderer でプロシージャルメッシュを描画する。
 * Camera, Material, Mesh, ForwardRenderer の動作確認。
 */

#include <lumino_core/platform/Window.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <lumino_core/graphics/ForwardRenderer.hpp>
#include <lumino_core/graphics/Camera.hpp>
#include <lumino_core/graphics/Mesh.hpp>
#include <lumino_core/graphics/Material.hpp>
#include <lumino_core/graphics/Vertex.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <lumino_base/Math.hpp>

#include <cstdio>
#include <cmath>
#include <vector>

using namespace lumino;
using namespace lumino::rhi;
using namespace lumino::platform;

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

    // 1. Window
    WindowDesc winDesc;
    winDesc.title = "Lumino - Forward Rendering";
    winDesc.width = 1280;
    winDesc.height = 720;
    auto* window = PlatformWindow::create(winDesc);
    if (!window) { fprintf(stderr, "Failed to create window\n"); return 1; }

    // 2. Device
    DeviceDesc devDesc;
    devDesc.backend = Backend::Vulkan;
    devDesc.enableValidation = true;
    auto deviceResult = Device::create(devDesc);
    if (!deviceResult) { fprintf(stderr, "Device: %s\n", deviceResult.error().message.c_str()); return 1; }
    auto device = std::move(*deviceResult);

    // 3. SwapChain
    SwapChainDesc scDesc;
    scDesc.nativeWindowHandle = window->nativeHandle().glfwWindow;
    scDesc.width = winDesc.width;
    scDesc.height = winDesc.height;
    scDesc.format = TextureFormat::BGRA8UnormSrgb;
    scDesc.vsync = true;
    auto swapChainResult = device->createSwapChain(scDesc);
    if (!swapChainResult) { fprintf(stderr, "SwapChain failed\n"); return 1; }
    auto swapChain = std::move(*swapChainResult);

    // 4. Depth texture
    TextureDesc depthTexDesc;
    depthTexDesc.width = winDesc.width;
    depthTexDesc.height = winDesc.height;
    depthTexDesc.format = TextureFormat::Depth32Float;
    depthTexDesc.usage = TextureUsage::DepthStencil;
    auto depthTexResult = device->createTexture(depthTexDesc);
    if (!depthTexResult) { fprintf(stderr, "Depth texture failed\n"); return 1; }
    auto depthTex = std::move(*depthTexResult);
    auto depthViewResult = device->createTextureView(depthTex.get());
    if (!depthViewResult) { fprintf(stderr, "Depth view failed\n"); return 1; }
    auto depthView = std::move(*depthViewResult);

    // 5. ForwardRenderer
    auto rendererResult = ForwardRenderer::create(
        device.get(), swapChain->format(), TextureFormat::Depth32Float);
    if (!rendererResult) { fprintf(stderr, "Renderer failed\n"); return 1; }
    auto renderer = std::move(*rendererResult);

    // 6. Material (BasicLit)
    auto matResult = MaterialFactory::createBasicLit(
        device.get(), renderer->pipelineLayout(),
        swapChain->format(), TextureFormat::Depth32Float);
    if (!matResult) { fprintf(stderr, "Material: %s\n", matResult.error().message.c_str()); return 1; }
    auto material = std::move(*matResult);
    material->setColor(Color{0.8f, 0.5f, 0.2f, 1.0f});
    auto ubgResult = material->updateBindGroup(device.get());
    if (!ubgResult) { fprintf(stderr, "Material bind group: %s\n", ubgResult.error().message.c_str()); return 1; }

    // 7. Cube mesh
    auto meshResult = createCubeMesh(device.get());
    if (!meshResult) { fprintf(stderr, "Mesh failed\n"); return 1; }
    auto mesh = std::move(*meshResult);
    mesh->materials() = {material};

    // 8. Camera
    Camera camera;
    camera.setPerspective(60.0f * 3.14159f / 180.0f,
        static_cast<f32>(winDesc.width) / static_cast<f32>(winDesc.height),
        0.1f, 100.0f);

    // 9. Light
    DirectionalLight light;
    light.direction = Vector3{0.3f, -1.0f, 0.5f};
    light.color = Color::white();
    light.ambient = Color{0.15f, 0.15f, 0.15f, 1.0f};
    renderer->setLight(light);

    printf("Lumino ForwardRenderer initialized. Rendering...\n");

    // 10. Main loop
    f32 time = 0.0f;
    while (window->processEvents()) {
        time += 0.016f; // ~60fps

        // Orbit camera (angles in radians)
        camera.setOrbit({0, 0, 0}, 3.0f, time * 0.5f, 0.35f);

        // Build render objects
        RenderObject obj;
        obj.mesh = mesh;
        obj.transform.rotation = Quaternion::fromEuler(time * 20.0f, time * 30.0f, 0.0f);
        std::vector<RenderObject> objects = {obj};

        auto* backbuffer = swapChain->acquireNextTexture();

        auto renderResult = renderer->renderFrame(
            device.get(), backbuffer, depthView.get(),
            camera, objects, Color{0.1f, 0.1f, 0.15f, 1.0f});

        if (!renderResult) {
            fprintf(stderr, "Render error\n");
        }

        swapChain->present();
    }

    device->waitIdle();
    delete window;

    printf("Done.\n");
    return 0;
}
