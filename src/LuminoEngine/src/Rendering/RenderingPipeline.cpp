
#include "Internal.hpp"
#include "RenderingPipeline.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "RenderTargetTextureCache.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderingPipeline

void RenderingPipeline::initialize()
{
    RenderingManager* manager = detail::EngineDomain::renderingManager();
    m_frameBufferCache = makeRef<detail::FrameBufferCache>(
        manager->renderTargetTextureCacheManager(), manager->depthBufferCacheManager());
}

//==============================================================================
// SceneRenderingPipeline

void SceneRenderingPipeline::initialize()
{
    RenderingPipeline::initialize();
    RenderingManager* manager = detail::EngineDomain::renderingManager();

    m_sceneRenderer = makeRef<detail::ClusteredShadingSceneRenderer>();
    m_sceneRenderer->initialize(manager);
}

void SceneRenderingPipeline::render(
    GraphicsContext* graphicsContext,
    const FrameBuffer& frameBuffer,
    const detail::CameraInfo* mainCameraInfo,
    const List<detail::DrawElementListCollector*>* elementListManagers)
{
    m_mainCameraInfo = mainCameraInfo;
    m_elementListManagers = elementListManagers;
    m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());

    m_sceneRenderer->render(graphicsContext, this, frameBuffer);

    // Œë—p–hŽ~
    m_renderingFrameBufferSize = SizeI();
    m_mainCameraInfo = nullptr;
    m_elementListManagers = nullptr;
}

} // namespace detail
} // namespace ln

