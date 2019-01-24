
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Scene/OffscreenWorldRenderView.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../Rendering/RenderTargetTextureCache.hpp"
#include "../Rendering/RenderingManager.hpp"

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
    m_renderingManager = detail::EngineDomain::renderingManager();
}

void OffscreenWorldRenderView::setRenderTarget(RenderTargetTexture* renderTarget)
{
    m_renderTarget = renderTarget;
    setActualPixelSize(Size(m_renderTarget->width(), m_renderTarget->height()));
}

RenderTargetTexture* OffscreenWorldRenderView::renderTarget() const
{
    return m_renderTarget;
}

void OffscreenWorldRenderView::render()
{
    auto depthBuffer = m_renderingManager->depthBufferCacheManager()->requestObject(SizeI(m_renderTarget->width(), m_renderTarget->height()));
    WorldRenderView::render(m_renderingManager->graphicsManager()->graphicsContext(), m_renderTarget, depthBuffer);
    // TODO: scoped
    m_renderingManager->depthBufferCacheManager()->releaseObject(depthBuffer);
}

} // namespace ln

