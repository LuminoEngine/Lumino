
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "RenderingManager.hpp"
#include "RenderStage.hpp"
#include "SceneRenderer.hpp"
#include "RenderTargetTextureCache.hpp"

namespace ln {

//==============================================================================
// RenderView

void RenderView::initialize()
{
	Object::initialize();
	m_manager = detail::EngineDomain::renderingManager();

	m_frameBufferCache = makeRef<detail::FrameBufferCache>(
		m_manager->renderTargetTextureCacheManager(), m_manager->depthBufferCacheManager());
}

void RenderView::clearDrawElementListManagers()
{
	m_elementListManagers.clear();
}

void RenderView::addDrawElementListManager(detail::DrawElementListCollector* elementListManager)
{
	m_elementListManagers.add(elementListManager);
}

void RenderView::render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
{
	m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());

	sceneRenderer->render(graphicsContext, this, frameBuffer);

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();
}

} // namespace ln

