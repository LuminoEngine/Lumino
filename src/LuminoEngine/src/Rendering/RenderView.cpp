
#include "Internal.hpp"
#include <Lumino/Rendering/RenderView.hpp>
#include "RenderStage.hpp"
#include "SceneRenderer.hpp"

namespace ln {

//==============================================================================
// RenderView

void RenderView::attachDrawElementList(detail::DrawElementList* elementList)
{
	m_elementLists.add(elementList);
}

void RenderView::render(GraphicsContext* graphicsContext, const FrameBuffer& frameBuffer, detail::SceneRenderer* sceneRenderer)
{
	sceneRenderer->render(graphicsContext, this, frameBuffer);
}

} // namespace ln

