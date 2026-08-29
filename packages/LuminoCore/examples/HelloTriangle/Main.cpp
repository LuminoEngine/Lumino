/**
 * hello_triangle.cpp
 *
 * RHI API を使って色付き三角形を描画するデモ。
 * Phase 1 の検証マイルストーン: Vulkan バックエンドが正しく動作することを確認する。
 *
 * PlatformWindow でウィンドウを作成し、
 * window->graphicsContext() でデバイスとスワップチェーンを取得する。
 * 低レベル RHI (Buffer, Pipeline 等) は ctx->device() 経由で使用する。
 */

#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoShader/ShaderCompiler2.hpp>
#include <LuminoShader/UnifiedShader2.hpp>

#include <cstdio>
#include <cstdlib>

// -- 頂点データ --
struct Vertex {
    float pos[2];
    float color[3];
};

static const Vertex s_vertices[] = {
    {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},  // 上: 赤
    {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},  // 右: 緑
    {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},  // 左: 青
};

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    using namespace ln;
    using namespace ln::rhi;
    using namespace ln::platform;

    // 1. CoreInstance + Window + GraphicsContext
    CoreInstance::Settings coreSettings;
    coreSettings.preferredBackend = Backend::Vulkan;
    coreSettings.enableValidation = true;
    auto _ = CoreInstance::initialize(coreSettings);
    {
        WindowDesc winDesc;
        winDesc.title = "Lumino - Hello Triangle";
        winDesc.width = 1280;
        winDesc.height = 720;

        GraphicsContextDesc gfxDesc;

        auto window = *PlatformWindow::create(CoreInstance::instance()->graphicsModule(), winDesc, gfxDesc);
        auto* ctx = window->graphicsContext();
        auto* device = ctx->device();

        // 2. 頂点バッファ
        BufferDesc vbDesc;
        vbDesc.size = sizeof(s_vertices);
        vbDesc.usage = BufferUsage::Vertex;
        vbDesc.initialData = s_vertices;
        auto vertexBuffer = *device->createBuffer(vbDesc);

        // 3. シェーダーコンパイル
        auto compiler = *ln::shader::ShaderCompiler2::create();
        auto _ = compiler->build(SHADER_FILE);

        ln::shader::UnifiedShader2* unifiedShader = compiler->shader();
        auto& globalPasses = unifiedShader->globalShaderPasses();
        if (globalPasses.empty()) {
            fprintf(stderr, "No shader passes found\n");
            return 1;
        }

        auto* globalPass = globalPasses[0].get();

        // デバイスのバックエンドに基づいてシェーダターゲットを選択する。
        ln::shader::ShaderTarget shaderTarget;
        ShaderCodeFormat codeFormat;
        switch (device->backend()) {
            case Backend::WebGPU:
                shaderTarget = ln::shader::ShaderTarget_WGSL;
                codeFormat = ShaderCodeFormat::WGSL;
                break;
            default:
                shaderTarget = ln::shader::ShaderTarget_SPIRV;
                codeFormat = ShaderCodeFormat::SPIRV;
                break;
        }

        auto targetPassId = globalPass->getTargetShaderPassId(shaderTarget);
        auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
        if (!targetPass) {
            fprintf(stderr, "No target pass found for the current backend\n");
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
        vsDesc.format = codeFormat;
        vsDesc.code = vertBlob->data.data();
        vsDesc.codeSizeBytes = vertBlob->data.size();
        auto vertShader = *device->createShaderModule(vsDesc);

        ShaderModuleDesc fsDesc;
        fsDesc.format = codeFormat;
        fsDesc.code = fragBlob->data.data();
        fsDesc.codeSizeBytes = fragBlob->data.size();
        auto fragShader = *device->createShaderModule(fsDesc);

        // 4. パイプラインレイアウト（バインドなし）
        PipelineLayoutDesc plDesc;
        auto pipelineLayout = *device->createPipelineLayout(plDesc);

        // 5. レンダーパイプライン（RenderPass は beginRenderPass 時に取得）
        RenderPipelineDesc rpDesc;
        rpDesc.layout = pipelineLayout.get();
        rpDesc.vertexShader = vertShader.get();
        rpDesc.fragmentShader = fragShader.get();
        rpDesc.vertexEntry = vertEP->name;
        rpDesc.fragmentEntry = fragEP->name;
        rpDesc.topology = PrimitiveTopology::TriangleList;
        rpDesc.cullMode = CullMode::None;

        VertexBufferLayout vbl;
        vbl.stride = sizeof(::Vertex);
        vbl.attributes = {
            {0, VertexFormat::Float32x2, 0},                     // position
            {1, VertexFormat::Float32x3, sizeof(float) * 2},     // color
        };
        rpDesc.vertexBuffers = {vbl};

        Ref<RenderPipeline> pipeline;

        // 6. メインループ
        while (window->processEvents()) {
            const FramebufferInfo* fb = *ctx->beginFrame(ctx->width(), ctx->height());

            auto* cmd = ctx->currentCommandBuffer();

            RenderPassDesc passDesc;
            passDesc.colorAttachments.push_back({
                fb->colorTexture->rhiTextureView(),
                LoadOp::Clear,
                StoreOp::Store,
                {0.1f, 0.1f, 0.15f, 1.0f},
            });

            auto* pass = cmd->beginRenderPass(passDesc);

            if (!pipeline) {
                rpDesc.renderPassLayout = pass->layoutDesc();
                pipeline = *device->createRenderPipeline(rpDesc);
            }

            pass->setPipeline(pipeline.get());
            pass->setVertexBuffer(0, vertexBuffer.get());
            pass->draw(3);
            pass->end();

            cmd->submit();
            ctx->endFrame();

            delete cmd;
        }

        printf("Done.\n");
    }
    CoreInstance::terminate();
    return 0;
}
