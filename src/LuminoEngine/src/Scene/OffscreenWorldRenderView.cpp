
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Scene/OffscreenWorldRenderView.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../Graphics/RenderTargetTextureCache.hpp"
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
	// TODO: dp単位に変換
    setActualSize(Size(m_renderTarget->width(), m_renderTarget->height()));
}

RenderTargetTexture* OffscreenWorldRenderView::renderTarget() const
{
    return m_renderTarget;
}

//void OffscreenWorldRenderView::render()
//{
//	auto depthBuffer = DepthBuffer::getTemporary(m_renderTarget->width(), m_renderTarget->height());
//
//    GraphicsContext* context = m_renderingManager->graphicsManager()->graphicsContext();
//    context->setRenderTarget(0, m_renderTarget);
//    context->setDepthBuffer(depthBuffer);
//    WorldRenderView::render(context);
//
//    // TODO: scoped
//	DepthBuffer::releaseTemporary(depthBuffer);
//}
//
} // namespace ln

