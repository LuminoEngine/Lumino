#include <LuminoCore/graphics/ForwardRenderer.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <cstring>

namespace ln {

Result<Ref<ForwardRenderer>> ForwardRenderer::create(GraphicsContext* ctx) {
    auto fw   = Ref<ForwardRenderer>::adopt(new ForwardRenderer());
    fw->m_ctx      = ctx;
    fw->m_renderer = ctx->renderer();

    // ---- Dynamic UBO allocator for per-frame scene data (lighting, set=1) ----
    {
        auto r = DynamicUniformAllocator::create(
            ctx->device(), fw->m_renderer->sceneBindGroupLayout(), 0,
            static_cast<u32>(sizeof(SceneParamsUBO)));
        if (!r) return tl::make_unexpected(r.error());
        fw->m_sceneAllocator = std::move(*r);
    }

    return fw;
}

Result<void> ForwardRenderer::renderFrame(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const std::vector<RenderObject>& objects,
    const Color& clearColor) {

    // ---- Render via the core Renderer ----
    // beginFrame resets allocators; scene allocator must also be reset with the same frame counter.
    m_renderer->beginFrame();
    m_sceneAllocator->beginFrame(m_renderer->currentFrameSlot());

    // ---- Allocate and upload Scene UBO (lighting) from the per-frame allocator ----
    auto sceneAlloc = m_sceneAllocator->allocate();
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

        std::memcpy(sceneAlloc.cpuPtr, &sceneParams, sizeof(sceneParams));
    }

    m_renderer->beginRenderPass(colorTarget, depthTarget, camera, clearColor);
    m_renderer->setPassBindGroup(1, sceneAlloc.bindGroup, sceneAlloc.dynamicOffset, 1);

    for (const auto& obj : objects) {
        auto result = m_renderer->drawMesh(obj.mesh.get(), obj.transform);
        if (!result) return result;
    }

    m_renderer->endRenderPass();
    m_renderer->endFrame(colorTarget);

    return {};
}

} // namespace ln
