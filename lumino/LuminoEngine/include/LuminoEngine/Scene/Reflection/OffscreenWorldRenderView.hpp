
#pragma once
#include "../WorldRenderView.hpp"

namespace ln {
namespace detail { class RenderingManager; }

// PostEffect はつかない。
class OffscreenWorldRenderView
	: public RenderView
{
public:
	void setRenderTarget(RenderTargetTexture* renderTarget);
	RenderTargetTexture* renderTarget() const;
    void render(GraphicsCommandList* graphicsContext, World* targetWorld);

	void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) override;


	//detail::CameraInfo m_cameraInfo;

LN_CONSTRUCT_ACCESS:
    OffscreenWorldRenderView();
	virtual ~OffscreenWorldRenderView();
	void init();

private:
    detail::RenderingManager* m_renderingManager;
	Ref<RenderTargetTexture> m_renderTarget;

	Ref<detail::SceneRenderingPipeline> m_sceneRenderingPipeline;
};

} // namespace ln
