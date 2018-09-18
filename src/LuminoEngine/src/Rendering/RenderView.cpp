
#include "Internal.hpp"
#include <Lumino/Rendering/RenderView.hpp>
#include "RenderStage.hpp"
#include "SceneRenderer.hpp"

namespace ln {

//==============================================================================
// RenderView

void RenderView::clearDrawElementListManagers()
{
	m_elementListManagers.clear();
}

void RenderView::addDrawElementListManager(detail::DrawElementListManager* elementListManager)
{
	m_elementListManagers.add(elementListManager);
}

void RenderView::render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
{
	sceneRenderer->render(graphicsContext, this, frameBuffer);
}

} // namespace ln

