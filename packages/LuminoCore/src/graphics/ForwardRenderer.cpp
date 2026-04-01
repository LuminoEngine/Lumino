#include <LuminoCore/graphics/ForwardRenderer.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <cstring>

namespace ln {

Result<Ref<ForwardRenderer>> ForwardRenderer::create(GraphicsContext* ctx) {
    // Create the core Renderer first (also creates the view UBO internally).
    auto rendererResult = Renderer::create(ctx);
    if (!rendererResult) return tl::make_unexpected(rendererResult.error());

    auto fw   = Ref<ForwardRenderer>::adopt(new ForwardRenderer());
    fw->m_ctx      = ctx;
    fw->m_renderer = std::move(*rendererResult);

    // ---- Per-scene UBO (lighting, set=1) ----
    fw->m_sceneUBOSize = sizeof(SceneParamsUBO);

    rhi::BufferDesc sceneBufDesc;
    sceneBufDesc.size  = fw->m_sceneUBOSize;
    sceneBufDesc.usage = rhi::BufferUsage::Uniform;
    auto sceneBufResult = ctx->device()->createBuffer(sceneBufDesc);
    if (!sceneBufResult) return tl::make_unexpected(sceneBufResult.error());
    fw->m_sceneUBO = std::move(*sceneBufResult);

    // ---- Per-scene BindGroup (set=1) ----
    rhi::BindGroupDesc sceneBGDesc;
    sceneBGDesc.layout  = fw->m_renderer->sceneBindGroupLayout();
    sceneBGDesc.entries = {
        {0, fw->m_sceneUBO.get(), 0, fw->m_sceneUBOSize, nullptr, nullptr},
    };
    auto sceneBGResult = ctx->device()->createBindGroup(sceneBGDesc);
    if (!sceneBGResult) return tl::make_unexpected(sceneBGResult.error());
    fw->m_sceneBindGroup = std::move(*sceneBGResult);

    return fw;
}

Result<void> ForwardRenderer::renderFrame(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const std::vector<RenderObject>& objects,
    const Color& clearColor) {

    // ---- Upload Scene UBO (lighting) ----
    {
        SceneParamsUBO sceneParams{};

        Vector3 ld = m_light.direction.normalized();
        sceneParams.lightDir[0] = ld.x;
        sceneParams.lightDir[1] = ld.y;
        sceneParams.lightDir[2] = ld.z;
        sceneParams.lightDir[3] = 0.0f;

        sceneParams.lightColor[0] = m_light.color.r;
        sceneParams.lightColor[1] = m_light.color.g;
        sceneParams.lightColor[2] = m_light.color.b;
        sceneParams.lightColor[3] = m_light.color.a;

        sceneParams.ambientColor[0] = m_light.ambient.r;
        sceneParams.ambientColor[1] = m_light.ambient.g;
        sceneParams.ambientColor[2] = m_light.ambient.b;
        sceneParams.ambientColor[3] = m_light.ambient.a;

        void* mapped = m_sceneUBO->map();
        if (mapped) {
            std::memcpy(mapped, &sceneParams, sizeof(sceneParams));
            m_sceneUBO->unmap();
        }
    }

    // ---- Render via the core Renderer ----
    // beginRenderPass with camera auto-uploads the View UBO and binds set=0.
    m_renderer->beginFrame();

    m_renderer->beginRenderPass(colorTarget, depthTarget, camera, clearColor);
    m_renderer->setPassBindGroup(1, m_sceneBindGroup.get());

    for (const auto& obj : objects) {
        auto result = m_renderer->drawMesh(obj.mesh.get(), obj.transform);
        if (!result) return result;
    }

    m_renderer->endRenderPass();
    m_renderer->endFrame();

    return {};
}

} // namespace ln
