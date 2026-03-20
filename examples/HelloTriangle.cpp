/**
 * hello_triangle.cpp
 *
 * RHI API を使って色付き三角形を描画するデモ。
 * Phase 1 の検証マイルストーン: Vulkan バックエンドが正しく動作することを確認する。
 */

#include <lumino_core/platform/Window.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>
#include <lumino_shader/ShaderCompiler.hpp>
#include <lumino_shader/UnifiedShader.hpp>

#include <cstdio>
#include <cstdlib>

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

    // 5. シェーダーコンパイル (lumino_shader)
    auto compilerResult = lumino::shader::ShaderCompiler::create();
    if (!compilerResult) {
        fprintf(stderr, "Failed to create shader compiler: %s\n", compilerResult.error().message.c_str());
        return 1;
    }
    auto compiler = std::move(*compilerResult);

    auto buildResult = compiler->build(SHADER_FILE);
    if (!buildResult) {
        fprintf(stderr, "Failed to compile shader: %s\n", buildResult.error().message.c_str());
        return 1;
    }

    lumino::shader::UnifiedShader* unifiedShader = compiler->shader();
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        fprintf(stderr, "No shader passes found\n");
        return 1;
    }

    auto* globalPass = globalPasses[0].get();
    auto targetPassId = globalPass->getTargetShaderPassId(lumino::shader::ShaderTarget_SPIRV);
    auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        fprintf(stderr, "No SPIRV target pass found\n");
        return 1;
    }

    auto* vertEP = unifiedShader->targetEntryPoint(targetPass->vertEntryPointId);
    auto* fragEP = unifiedShader->targetEntryPoint(targetPass->fragEntryPointId);
    if (!vertEP || !fragEP) {
        fprintf(stderr, "Missing vertex or fragment entry point\n");
        return 1;
    }

    auto* vertBlob = unifiedShader->blob(vertEP->codeBlobId);
    auto* fragBlob = unifiedShader->blob(fragEP->codeBlobId);

    ShaderModuleDesc vsDesc;
    vsDesc.spirvCode = reinterpret_cast<const lumino::u32*>(vertBlob->data.data());
    vsDesc.spirvSizeBytes = vertBlob->data.size();
    auto vsResult = device->createShaderModule(vsDesc);

    ShaderModuleDesc fsDesc;
    fsDesc.spirvCode = reinterpret_cast<const lumino::u32*>(fragBlob->data.data());
    fsDesc.spirvSizeBytes = fragBlob->data.size();
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
    rpDesc.vertexEntry = vertEP->name;
    rpDesc.fragmentEntry = fragEP->name;
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
