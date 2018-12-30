
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "RenderingManager.hpp"
#include "RenderStage.hpp"
#include "SceneRenderer.hpp"

namespace ln {

//==============================================================================
// RenderView

RenderView::RenderView()
	: m_clearMode(RenderViewClearMode::ColorAndDepth)
	, m_backgroundColor(Color::White)
{
}

RenderView::~RenderView()
{
}

void RenderView::initialize()
{
	Object::initialize();
	m_manager = detail::EngineDomain::renderingManager();
}

void RenderView::clearDrawElementListManagers()
{
	m_elementListManagers.clear();
}

void RenderView::addDrawElementListManager(detail::DrawElementListCollector* elementListManager)
{
	m_elementListManagers.add(elementListManager);
}

EventConnection RenderView::connectOnUIEvent(UIEventHandler handler)
{
    return m_onUIEvent.connect(handler);
}

void RenderView::onRoutedEvent(UIEventArgs* e)
{
    m_onUIEvent.raise(e);
}

//void RenderView::render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
//{
//	m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());
//
//	sceneRenderer->render(graphicsContext, this, frameBuffer);
//
//	// 誤用防止
//	m_renderingFrameBufferSize = SizeI();
//}

} // namespace ln

