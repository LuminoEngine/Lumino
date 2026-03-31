#include <lumino_core/graphics/ForwardRenderer.hpp>
#include <lumino_core/graphics/GraphicsContext.hpp>
#include <cstring>

namespace ln {

Result<Ref<ForwardRenderer>> ForwardRenderer::create(GraphicsContext* ctx) {
    // Create the core Renderer first.
    auto rendererResult = Renderer::create(ctx);
    if (!rendererResult) return tl::make_unexpected(rendererResult.error());

    auto fw   = Ref<ForwardRenderer>::adopt(new ForwardRenderer());
    fw->m_ctx      = ctx;
    fw->m_renderer = std::move(*rendererResult);

    // Determine View UBO size from the Renderer's reflected layout.
    // We create a minimal UBO sized to ViewParamsUBO.
    fw->m_viewUBOSize = sizeof(ViewParamsUBO);

    // ---- Per-view UBO ----
    rhi::BufferDesc viewBufDesc;
    viewBufDesc.size  = fw->m_viewUBOSize;
    viewBufDesc.usage = rhi::BufferUsage::Uniform;
    auto viewBufResult = ctx->device()->createBuffer(viewBufDesc);
    if (!viewBufResult) return tl::make_unexpected(viewBufResult.error());
    fw->m_viewUBO = std::move(*viewBufResult);

    // ---- Per-view BindGroup (set=0) ----
    rhi::BindGroupDesc viewBGDesc;
    viewBGDesc.layout  = fw->m_renderer->viewBindGroupLayout();
    viewBGDesc.entries = {
        {0, fw->m_viewUBO.get(), 0, fw->m_viewUBOSize, nullptr, nullptr},
    };
    auto viewBGResult = ctx->device()->createBindGroup(viewBGDesc);
    if (!viewBGResult) return tl::make_unexpected(viewBGResult.error());
    fw->m_viewBindGroup = std::move(*viewBGResult);

    return fw;
}

Result<void> ForwardRenderer::renderFrame(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const std::vector<RenderObject>& objects,
    const Color& clearColor) {

    // ---- Upload View UBO (camera + lighting) ----
    {
        ViewParamsUBO viewParams{};

        Matrix4x4 vp = camera.viewProjectionMatrix();
        std::memcpy(viewParams.viewProj, vp.m, sizeof(f32) * 16);

        Vector3 camPos = camera.position();
        viewParams.cameraPos[0] = camPos.x;
        viewParams.cameraPos[1] = camPos.y;
        viewParams.cameraPos[2] = camPos.z;
        viewParams.cameraPos[3] = 0.0f;

        Vector3 ld = m_light.direction.normalized();
        viewParams.lightDir[0] = ld.x;
        viewParams.lightDir[1] = ld.y;
        viewParams.lightDir[2] = ld.z;
        viewParams.lightDir[3] = 0.0f;

        viewParams.lightColor[0] = m_light.color.r;
        viewParams.lightColor[1] = m_light.color.g;
        viewParams.lightColor[2] = m_light.color.b;
        viewParams.lightColor[3] = m_light.color.a;

        viewParams.ambientColor[0] = m_light.ambient.r;
        viewParams.ambientColor[1] = m_light.ambient.g;
        viewParams.ambientColor[2] = m_light.ambient.b;
        viewParams.ambientColor[3] = m_light.ambient.a;

        void* mapped = m_viewUBO->map();
        if (mapped) {
            std::memcpy(mapped, &viewParams, sizeof(viewParams));
            m_viewUBO->unmap();
        }
    }

    // ---- Render via the core Renderer ----
    m_renderer->beginFrame();

    m_renderer->beginRenderPass(colorTarget, depthTarget, clearColor);
    m_renderer->setPassBindGroup(0, m_viewBindGroup.get());

    for (const auto& obj : objects) {
        auto result = m_renderer->drawMesh(obj.mesh.get(), obj.transform);
        if (!result) return result;
    }

    m_renderer->endRenderPass();
    m_renderer->endFrame();

    return {};
}

} // namespace ln
