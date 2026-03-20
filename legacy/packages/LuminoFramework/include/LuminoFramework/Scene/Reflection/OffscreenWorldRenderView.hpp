
#pragma once
#include "../WorldRenderView.hpp"

namespace ln {
namespace detail {
class RenderingManager;
}

// PostEffect はつかない。
class OffscreenWorldRenderView : public RenderView {
public:
    void setRenderTarget(RenderTargetTexture* renderTarget);
    RenderTargetTexture* renderTarget() const;
    void render(GraphicsCommandList* graphicsContext, World* targetWorld);

    Ref<RenderViewPoint> m_viewPoint;

LN_CONSTRUCT_ACCESS:
    OffscreenWorldRenderView();
    virtual ~OffscreenWorldRenderView();
    void init();

protected:
    void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) override;
    void onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) override;
    void onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) override;

private:
    detail::RenderingManager* m_renderingManager;
    Ref<RenderTargetTexture> m_renderTarget;
    World* m_targetWorld;
    // Ref<SceneRenderingPipeline> m_sceneRenderingPipeline;
};

} // namespace ln
