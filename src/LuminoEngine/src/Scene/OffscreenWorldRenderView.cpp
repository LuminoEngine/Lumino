
#include "Internal.hpp"
#include <LuminoEngine/Scene/OffscreenWorldRenderView.hpp>

namespace ln {

//==============================================================================
// OffscreenWorldRenderView

OffscreenWorldRenderView::OffscreenWorldRenderView()
    : m_renderTarget(nullptr)
{
}

OffscreenWorldRenderView::~OffscreenWorldRenderView()
{
}

void OffscreenWorldRenderView::init()
{
    WorldRenderView::init();
}

void OffscreenWorldRenderView::setRenderTarget(RenderTargetTexture* renderTarget)
{
    m_renderTarget = renderTarget;
}

RenderTargetTexture* OffscreenWorldRenderView::renderTarget() const
{
    return m_renderTarget;
}

void OffscreenWorldRenderView::render()
{
}

} // namespace ln

