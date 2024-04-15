#pragma once
#include "RenderingPipeline.hpp"

namespace ln {

// ライティングしない Pipeline。UI で使う。
class FlatRenderingPipeline : public RenderingPipeline {
public:
    FlatRenderingPipeline();
    void init();

protected:
    void onRender(
        GraphicsCommandList* graphicsContext,
        RenderingContext* renderingContext,
        RenderTargetTexture* renderTarget,
        const ClearInfo& mainPassClearInfo,
        const RenderView* renderView,
        detail::CommandListServer* commandListServer,
        const detail::SceneGlobalRenderParams* sceneGlobalParams) override;

private:
    // Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;
    Ref<detail::SceneRenderer> m_sceneRenderer;
    Ref<detail::UnLigitingSceneRendererPass> m_unlitRendererPass;
    Ref<detail::UnLigitingSceneRendererPass> m_unlitRendererPass_PostEffect;
};

} // namespace ln
