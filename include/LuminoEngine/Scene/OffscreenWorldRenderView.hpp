
#pragma once
#include "WorldRenderView.hpp"

namespace ln {

class OffscreenWorldRenderView
	: public WorldRenderView
{
public:
	void setRenderTarget(RenderTargetTexture* renderTarget);
	RenderTargetTexture* renderTarget() const;
    void render();

LN_CONSTRUCT_ACCESS:
    OffscreenWorldRenderView();
	virtual ~OffscreenWorldRenderView();
	void init();

private:
	Ref<RenderTargetTexture> m_renderTarget;
};

} // namespace ln
