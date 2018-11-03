
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderingPipeline.hpp"

namespace ln {

//==============================================================================
// UIRenderView

UIRenderView::UIRenderView()
{
}

void UIRenderView::initialize()
{
	RenderView::initialize();
	m_renderingContext = makeRef<UIRenderingContext>();
	m_sceneRenderingPipeline = makeRef<detail::FlatRenderingPipeline>();
	m_sceneRenderingPipeline->initialize();
	m_drawElementListCollector = makeRef<detail::DrawElementListCollector>();

	m_drawElementListCollector->addDrawElementList(detail::RendringPhase::Default, m_renderingContext->m_elementList);
	addDrawElementListManager(m_drawElementListCollector);
}

void UIRenderView::renderTree(GraphicsContext* graphicsContext, UIElement* element)
{
	// build draw elements
	{
		m_renderingContext->resetForBeginRendering();
		element->render(m_renderingContext);
	}






	FrameBuffer fb;
	fb.renderTarget[0] = graphicsContext->colorBuffer(0);
	fb.depthBuffer = graphicsContext->depthBuffer();

	// TODO:
	detail::CameraInfo camera;
	{
		camera.viewPixelSize = Size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());	// TODO: 必要？
		camera.viewPosition = Vector3::Zero;
		camera.viewDirection = Vector3::UnitZ;
		camera.viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);//Matrix();// 
		camera.projMatrix = Matrix::makePerspective2DLH(camera.viewPixelSize.width, camera.viewPixelSize.height, 0, 1000);
		camera.viewProjMatrix = camera.viewMatrix * camera.projMatrix;
		camera.viewFrustum = ViewFrustum(camera.viewProjMatrix);
		camera.nearClip = 0;
		camera.farClip = 1000;
	}

	m_sceneRenderingPipeline->render(graphicsContext, fb, &camera, &elementListManagers());
}

} // namespace ln

