#include <LuminoCore/Graphics/ForwardRenderer.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <cstring>

namespace ln {

Result<Ref<ForwardRenderer>> ForwardRenderer::create(GraphicsContext* ctx) {
    auto fw   = Ref<ForwardRenderer>::adopt(new ForwardRenderer());
    fw->m_ctx      = ctx;
    fw->m_renderer = ctx->renderer();

    // シーンアロケータの参照用として BasicLit の PipelineLayout を使う
    const auto& refShaderPass = ctx->module()->builtinShader(BuiltinShader::BasicLit);
    auto* refPipelineLayout = refShaderPass->pipelineLayout();
    int16_t sceneSetIndex = refShaderPass->sceneSetIndex();

    // ---- フレームごとのシーンデータ (ライティング) 用の動的 UBO アロケータ ----
    {
        auto r = DynamicUniformAllocator::create(
            ctx->device(), refPipelineLayout,
            static_cast<uint32_t>(sceneSetIndex), 0,
            static_cast<uint32_t>(sizeof(SceneParamsUBO)),
            ctx->maxFramesInFlight());
        if (!r) return LN_FORWARD_ERROR(r);
        fw->m_sceneAllocator = std::move(*r);
    }

    fw->m_sceneSetIndex = sceneSetIndex;
    return fw;
}

Result<void> ForwardRenderer::renderFrame(
    rhi::TextureView* colorTarget,
    rhi::TextureView* depthTarget,
    const Camera& camera,
    const RenderObject* objects,
    size_t objectCount,
    const Color& clearColor) {

    // ---- コアの Renderer で描画する ----
    // beginFrame はアロケータをリセットする。シーンアロケータも同じフレームカウンタでリセットすること。
    m_renderer->beginFrame();
    m_sceneAllocator->beginFrame(m_renderer->currentFrameSlot());

    // ---- フレームごとのアロケータから Scene UBO (ライティング) を確保してアップロードする ----
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
    m_renderer->setPassBindGroup(
        static_cast<uint32_t>(m_sceneSetIndex), sceneAlloc.bindGroup, sceneAlloc.dynamicOffset, 1);

    for (size_t i = 0; i < objectCount; ++i) {
        const RenderObject& obj = objects[i];
        auto result = m_renderer->drawMeshImmediate(obj.mesh.get(), obj.transform);
        if (!result) return result;
    }

    m_renderer->endRenderPass();
    m_renderer->endFrame();

    return {};
}

} // namespace ln
