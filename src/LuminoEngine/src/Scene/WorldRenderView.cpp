
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
#include <LuminoEngine/Scene/Camera.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderingPipeline.hpp"

namespace ln {

//==============================================================================
// WorldRenderView

WorldRenderView::WorldRenderView()
{
}

WorldRenderView::~WorldRenderView()
{
}

void WorldRenderView::initialize()
{
    RenderView::initialize();
    m_sceneRenderingPipeline = makeRef<detail::SceneRenderingPipeline>();
    m_sceneRenderingPipeline->initialize();
    m_drawElementListCollector = makeRef<detail::DrawElementListCollector>();
    m_viewPoint = newObject<RenderViewPoint>();
}

void WorldRenderView::setTargetWorld(World* world)
{
    m_targetWorld = world;

    m_drawElementListCollector->addDrawElementList(detail::RendringPhase::Default, m_targetWorld->m_renderingContext->m_elementList);

    addDrawElementListManager(m_drawElementListCollector);
}

void WorldRenderView::setCamera(Camera* camera)
{
    if (camera) {
        if (LN_REQUIRE(!camera->m_ownerRenderView)) return;
    }

    if (m_camera) {
        m_camera->m_ownerRenderView = nullptr;
    }

	m_camera = camera;

    if (m_camera) {
        m_camera->m_ownerRenderView = this;
    }
}

void WorldRenderView::render(GraphicsContext* graphicsContext)
{
	if (m_camera)
	{

        FrameBuffer fb;
        fb.renderTarget[0] = graphicsContext->colorBuffer(0);
        fb.depthBuffer = graphicsContext->depthBuffer();

        // TODO:
        detail::CameraInfo camera;
        {
            CameraComponent* cc = m_camera->cameraComponent();
            
            m_viewPoint->worldMatrix = m_camera->worldMatrix();
            m_viewPoint->viewPixelSize = camera.viewPixelSize = Size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());	// TODO: 必要？
            m_viewPoint->viewPosition = camera.viewPosition = m_camera->position();
            m_viewPoint->viewDirection = camera.viewDirection = cc->getDirectionInternal().xyz();
            m_viewPoint->viewMatrix = camera.viewMatrix = cc->getViewMatrix();
            m_viewPoint->projMatrix = camera.projMatrix = cc->getProjectionMatrix();
            m_viewPoint->viewProjMatrix = camera.viewProjMatrix = cc->getViewProjectionMatrix();
            m_viewPoint->viewFrustum = camera.viewFrustum = cc->getViewFrustum();

            m_viewPoint->nearClip = camera.nearClip = cc->getNearClip();
            m_viewPoint->farClip = camera.farClip = cc->getFarClip();

            //Size size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());
            //Vector3 pos = Vector3(5, 5, -5);
            //camera.makePerspective(pos, Vector3::normalize(Vector3::Zero - pos), Math::PI / 3.0f, size, 0.1f, 100.0f);

        }




        // DrawList 構築
        if (m_targetWorld) {
			detail::WorldSceneGraphRenderingContext* renderingContext = m_targetWorld->prepareRender(m_viewPoint);

			if (clearMode() == RenderViewClearMode::ColorAndDepth) {
				renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
			}

            m_targetWorld->renderObjects();
        }



		m_sceneRenderingPipeline->render(graphicsContext, fb, &camera, &elementListManagers());
	}
}

} // namespace ln

