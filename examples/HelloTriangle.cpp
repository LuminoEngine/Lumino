/**
 * hello_triangle.cpp
 *
 * RHI API を使って色付き三角形を描画するデモ。
 * Phase 1 の検証マイルストーン: Vulkan バックエンドが正しく動作することを確認する。
 */

#include <lumino_core/platform/Window.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>

#include <cstdio>
#include <cstdlib>

// ── SPIR-V シェーダー ──────────────────────────────────────────────────
// glslangValidator -V で生成 (SPIR-V 1.0)
// vertex shader (GLSL 450):
//   layout(location=0) in vec2 inPos;
//   layout(location=1) in vec3 inColor;
//   layout(location=0) out vec3 fragColor;
//   void main() {
//       gl_Position = vec4(inPos, 0.0, 1.0);
//       fragColor = inColor;
//   }
static const uint32_t s_vertSpirv[] = {
    0x07230203, 0x00010000, 0x0008000b, 0x00000021,
    0x00000000, 0x00020011, 0x00000001, 0x0006000b,
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
    0x00000000, 0x0003000e, 0x00000000, 0x00000001,
    0x0009000f, 0x00000000, 0x00000004, 0x6e69616d,
    0x00000000, 0x0000000d, 0x00000012, 0x0000001d,
    0x0000001f, 0x00030003, 0x00000002, 0x000001c2,
    0x00040005, 0x00000004, 0x6e69616d, 0x00000000,
    0x00060005, 0x0000000b, 0x505f6c67, 0x65567265,
    0x78657472, 0x00000000, 0x00060006, 0x0000000b,
    0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69,
    0x00070006, 0x0000000b, 0x00000001, 0x505f6c67,
    0x746e696f, 0x657a6953, 0x00000000, 0x00070006,
    0x0000000b, 0x00000002, 0x435f6c67, 0x4470696c,
    0x61747369, 0x0065636e, 0x00070006, 0x0000000b,
    0x00000003, 0x435f6c67, 0x446c6c75, 0x61747369,
    0x0065636e, 0x00030005, 0x0000000d, 0x00000000,
    0x00040005, 0x00000012, 0x6f506e69, 0x00000073,
    0x00050005, 0x0000001d, 0x67617266, 0x6f6c6f43,
    0x00000072, 0x00040005, 0x0000001f, 0x6f436e69,
    0x00726f6c, 0x00030047, 0x0000000b, 0x00000002,
    0x00050048, 0x0000000b, 0x00000000, 0x0000000b,
    0x00000000, 0x00050048, 0x0000000b, 0x00000001,
    0x0000000b, 0x00000001, 0x00050048, 0x0000000b,
    0x00000002, 0x0000000b, 0x00000003, 0x00050048,
    0x0000000b, 0x00000003, 0x0000000b, 0x00000004,
    0x00040047, 0x00000012, 0x0000001e, 0x00000000,
    0x00040047, 0x0000001d, 0x0000001e, 0x00000000,
    0x00040047, 0x0000001f, 0x0000001e, 0x00000001,
    0x00020013, 0x00000002, 0x00030021, 0x00000003,
    0x00000002, 0x00030016, 0x00000006, 0x00000020,
    0x00040017, 0x00000007, 0x00000006, 0x00000004,
    0x00040015, 0x00000008, 0x00000020, 0x00000000,
    0x0004002b, 0x00000008, 0x00000009, 0x00000001,
    0x0004001c, 0x0000000a, 0x00000006, 0x00000009,
    0x0006001e, 0x0000000b, 0x00000007, 0x00000006,
    0x0000000a, 0x0000000a, 0x00040020, 0x0000000c,
    0x00000003, 0x0000000b, 0x0004003b, 0x0000000c,
    0x0000000d, 0x00000003, 0x00040015, 0x0000000e,
    0x00000020, 0x00000001, 0x0004002b, 0x0000000e,
    0x0000000f, 0x00000000, 0x00040017, 0x00000010,
    0x00000006, 0x00000002, 0x00040020, 0x00000011,
    0x00000001, 0x00000010, 0x0004003b, 0x00000011,
    0x00000012, 0x00000001, 0x0004002b, 0x00000006,
    0x00000014, 0x00000000, 0x0004002b, 0x00000006,
    0x00000015, 0x3f800000, 0x00040020, 0x00000019,
    0x00000003, 0x00000007, 0x00040017, 0x0000001b,
    0x00000006, 0x00000003, 0x00040020, 0x0000001c,
    0x00000003, 0x0000001b, 0x0004003b, 0x0000001c,
    0x0000001d, 0x00000003, 0x00040020, 0x0000001e,
    0x00000001, 0x0000001b, 0x0004003b, 0x0000001e,
    0x0000001f, 0x00000001, 0x00050036, 0x00000002,
    0x00000004, 0x00000000, 0x00000003, 0x000200f8,
    0x00000005, 0x0004003d, 0x00000010, 0x00000013,
    0x00000012, 0x00050051, 0x00000006, 0x00000016,
    0x00000013, 0x00000000, 0x00050051, 0x00000006,
    0x00000017, 0x00000013, 0x00000001, 0x00070050,
    0x00000007, 0x00000018, 0x00000016, 0x00000017,
    0x00000014, 0x00000015, 0x00050041, 0x00000019,
    0x0000001a, 0x0000000d, 0x0000000f, 0x0003003e,
    0x0000001a, 0x00000018, 0x0004003d, 0x0000001b,
    0x00000020, 0x0000001f, 0x0003003e, 0x0000001d,
    0x00000020, 0x000100fd, 0x00010038,
};

// fragment shader (GLSL 450):
//   layout(location=0) in vec3 fragColor;
//   layout(location=0) out vec4 outColor;
//   void main() { outColor = vec4(fragColor, 1.0); }
static const uint32_t s_fragSpirv[] = {
    0x07230203, 0x00010000, 0x0008000b, 0x00000013,
    0x00000000, 0x00020011, 0x00000001, 0x0006000b,
    0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
    0x00000000, 0x0003000e, 0x00000000, 0x00000001,
    0x0007000f, 0x00000004, 0x00000004, 0x6e69616d,
    0x00000000, 0x00000009, 0x0000000c, 0x00030010,
    0x00000004, 0x00000007, 0x00030003, 0x00000002,
    0x000001c2, 0x00040005, 0x00000004, 0x6e69616d,
    0x00000000, 0x00050005, 0x00000009, 0x4374756f,
    0x726f6c6f, 0x00000000, 0x00050005, 0x0000000c,
    0x67617266, 0x6f6c6f43, 0x00000072, 0x00040047,
    0x00000009, 0x0000001e, 0x00000000, 0x00040047,
    0x0000000c, 0x0000001e, 0x00000000, 0x00020013,
    0x00000002, 0x00030021, 0x00000003, 0x00000002,
    0x00030016, 0x00000006, 0x00000020, 0x00040017,
    0x00000007, 0x00000006, 0x00000004, 0x00040020,
    0x00000008, 0x00000003, 0x00000007, 0x0004003b,
    0x00000008, 0x00000009, 0x00000003, 0x00040017,
    0x0000000a, 0x00000006, 0x00000003, 0x00040020,
    0x0000000b, 0x00000001, 0x0000000a, 0x0004003b,
    0x0000000b, 0x0000000c, 0x00000001, 0x0004002b,
    0x00000006, 0x0000000e, 0x3f800000, 0x00050036,
    0x00000002, 0x00000004, 0x00000000, 0x00000003,
    0x000200f8, 0x00000005, 0x0004003d, 0x0000000a,
    0x0000000d, 0x0000000c, 0x00050051, 0x00000006,
    0x0000000f, 0x0000000d, 0x00000000, 0x00050051,
    0x00000006, 0x00000010, 0x0000000d, 0x00000001,
    0x00050051, 0x00000006, 0x00000011, 0x0000000d,
    0x00000002, 0x00070050, 0x00000007, 0x00000012,
    0x0000000f, 0x00000010, 0x00000011, 0x0000000e,
    0x0003003e, 0x00000009, 0x00000012, 0x000100fd,
    0x00010038,
};

// ── 頂点データ ──
struct Vertex {
    float pos[2];
    float color[3];
};

static const Vertex s_vertices[] = {
    {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},  // top: red
    {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},  // right: green
    {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},  // left: blue
};

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    using namespace lumino;
    using namespace lumino::rhi;
    using namespace lumino::platform;

    // 1. ウィンドウ作成
    WindowDesc winDesc;
    winDesc.title = "Lumino - Hello Triangle";
    winDesc.width = 1280;
    winDesc.height = 720;
    auto* window = PlatformWindow::create(winDesc);
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        return 1;
    }

    // 2. RHI デバイス作成
    DeviceDesc devDesc;
    devDesc.backend = Backend::Vulkan;
    devDesc.enableValidation = true;
    auto deviceResult = Device::create(devDesc);
    if (!deviceResult) {
        fprintf(stderr, "Failed to create device: %s\n", deviceResult.error().message.c_str());
        return 1;
    }
    auto device = std::move(*deviceResult);

    // 3. スワップチェーン作成
    SwapChainDesc scDesc;
    auto handle = window->nativeHandle();
    scDesc.nativeWindowHandle = handle.glfwWindow;
    scDesc.width = winDesc.width;
    scDesc.height = winDesc.height;
    scDesc.format = TextureFormat::BGRA8UnormSrgb;
    scDesc.vsync = true;
    auto swapChainResult = device->createSwapChain(scDesc);
    if (!swapChainResult) {
        fprintf(stderr, "Failed to create swap chain\n");
        return 1;
    }
    auto swapChain = std::move(*swapChainResult);

    // 4. 頂点バッファ
    BufferDesc vbDesc;
    vbDesc.size = sizeof(s_vertices);
    vbDesc.usage = BufferUsage::Vertex;
    vbDesc.initialData = s_vertices;
    auto vbResult = device->createBuffer(vbDesc);
    if (!vbResult) { fprintf(stderr, "Failed to create vertex buffer\n"); return 1; }
    auto vertexBuffer = std::move(*vbResult);

    // 5. シェーダーモジュール
    ShaderModuleDesc vsDesc;
    vsDesc.spirvCode = s_vertSpirv;
    vsDesc.spirvSizeBytes = sizeof(s_vertSpirv);
    auto vsResult = device->createShaderModule(vsDesc);

    ShaderModuleDesc fsDesc;
    fsDesc.spirvCode = s_fragSpirv;
    fsDesc.spirvSizeBytes = sizeof(s_fragSpirv);
    auto fsResult = device->createShaderModule(fsDesc);

    if (!vsResult || !fsResult) { fprintf(stderr, "Failed to create shaders\n"); return 1; }
    auto vertShader = std::move(*vsResult);
    auto fragShader = std::move(*fsResult);

    // 6. パイプラインレイアウト（バインドなし）
    PipelineLayoutDesc plDesc;
    auto plResult = device->createPipelineLayout(plDesc);
    if (!plResult) { fprintf(stderr, "Failed to create pipeline layout\n"); return 1; }
    auto pipelineLayout = std::move(*plResult);

    // 7. レンダーパイプライン
    RenderPipelineDesc rpDesc;
    rpDesc.layout = pipelineLayout.get();
    rpDesc.vertexShader = vertShader.get();
    rpDesc.fragmentShader = fragShader.get();
    rpDesc.topology = PrimitiveTopology::TriangleList;
    rpDesc.cullMode = CullMode::None;
    rpDesc.colorFormats = {swapChain->format()};

    VertexBufferLayout vbl;
    vbl.stride = sizeof(Vertex);
    vbl.attributes = {
        {0, VertexFormat::Float32x2, 0},                     // position
        {1, VertexFormat::Float32x3, sizeof(float) * 2},     // color
    };
    rpDesc.vertexBuffers = {vbl};

    auto pipelineResult = device->createRenderPipeline(rpDesc);
    if (!pipelineResult) { fprintf(stderr, "Failed to create render pipeline\n"); return 1; }
    auto pipeline = std::move(*pipelineResult);

    printf("Lumino RHI initialized. Rendering...\n");

    // 8. メインループ
    while (window->processEvents()) {
        auto* backbuffer = swapChain->acquireNextTexture();

        auto* cmd = device->createCommandBuffer();

        RenderPassDesc passDesc;
        passDesc.colorAttachments.push_back({
            backbuffer,
            LoadOp::Clear,
            StoreOp::Store,
            {0.1f, 0.1f, 0.15f, 1.0f},
        });

        auto* pass = cmd->beginRenderPass(passDesc);
        pass->setPipeline(pipeline.get());
        pass->setVertexBuffer(0, vertexBuffer.get());
        pass->draw(3);
        pass->end();

        cmd->submit();
        swapChain->present();

        delete cmd;
    }

    device->waitIdle();
    delete window;

    printf("Done.\n");
    return 0;
}
