#pragma once
#include <LuminoGraphics/Rendering/Kanata/KUnlitRenderPass.hpp>
#include "SceneRenderer.hpp"

namespace ln {
namespace detail {

class UnLigitingSceneRendererPass
    : public SceneRendererPass {
public:
    UnLigitingSceneRendererPass();

    void init(RenderingManager* manager, bool forPostEffect);

    virtual void onBeginRender(SceneRenderer* sceneRenderer, GraphicsCommandList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) override;
    virtual RenderPass* renderPass() const;

    virtual ShaderTechnique* selectShaderTechnique(
        const ShaderTechniqueRequestClasses& requester,
        Shader* requestedShader,
        ShadingModel requestedShadingModel) override;

    virtual kanata::SceneRenderPass* internalSceneRenderPass() const { return m_internalSceneRenderPass; }

private:
    Ref<Shader> m_defaultShader;
    Ref<RenderPass> m_renderPass;
    URef<kanata::UnlitRenderPass> m_internalSceneRenderPass;
};

} // namespace detail
} // namespace ln
