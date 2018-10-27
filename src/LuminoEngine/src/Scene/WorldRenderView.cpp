
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldRenderView.hpp>
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
}

void WorldRenderView::setTargetWorld(World* world)
{
    m_targetWorld = world;

    m_drawElementListCollector->addDrawElementList(detail::RendringPhase::Default, m_targetWorld->m_renderingContext->m_elementList);

    addDrawElementListManager(m_drawElementListCollector);
}

void WorldRenderView::render(GraphicsContext* graphicsContext)
{
    FrameBuffer fb;
    fb.renderTarget[0] = graphicsContext->colorBuffer(0);
    fb.depthBuffer = graphicsContext->depthBuffer();

    // TODO:
    detail::CameraInfo camera;
    {
        Size size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());
        Vector3 pos = Vector3(5, 5, -5);
        camera.makePerspective(pos, Vector3::normalize(Vector3::Zero - pos), Math::PI / 3.0f, size, 0.1f, 100.0f);

    }

    m_sceneRenderingPipeline->render(graphicsContext, fb, &camera, &elementListManagers());
}

} // namespace ln

