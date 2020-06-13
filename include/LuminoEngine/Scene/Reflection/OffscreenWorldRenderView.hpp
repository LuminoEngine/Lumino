
#pragma once
#include "../WorldRenderView.hpp"

namespace ln {
namespace detail { class RenderingManager; }

class OffscreenWorldRenderView
	: public WorldRenderView
{
public:
	void setRenderTarget(RenderTargetTexture* renderTarget);
	RenderTargetTexture* renderTarget() const;
    //void render();

LN_CONSTRUCT_ACCESS:
    OffscreenWorldRenderView();
	virtual ~OffscreenWorldRenderView();
	void init();

private:
    detail::RenderingManager* m_renderingManager;
	Ref<RenderTargetTexture> m_renderTarget;
};

} // namespace ln
