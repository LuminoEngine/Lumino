
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderingPipeline.hpp"
#include "UIRenderView.hpp"

namespace ln {
namespace detail {

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
}

void UIRenderView::render(GraphicsContext* graphicsContext)
{
	FrameBuffer fb;
	fb.renderTarget[0] = graphicsContext->colorBuffer(0);
	fb.depthBuffer = graphicsContext->depthBuffer();

	// TODO:
	detail::CameraInfo camera;
	{
		camera.viewPixelSize = Size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());	// TODO: 必要？
		camera.viewPosition = Vector3::Zero;
		camera.viewDirection = Vector3::UnitZ;
		camera.viewMatrix = Matrix();// ::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
		camera.projMatrix = Matrix::makePerspective2DLH(camera.viewPixelSize.width, camera.viewPixelSize.height, 0, 10000);
		camera.viewProjMatrix = camera.viewMatrix * camera.viewProjMatrix;
		camera.viewFrustum = ViewFrustum(camera.viewProjMatrix);
		camera.nearClip = 0;
		camera.farClip = 1000;
	}

	m_sceneRenderingPipeline->render(graphicsContext, fb, &camera, &elementListManagers());
}

} // namespace detail
} // namespace ln

