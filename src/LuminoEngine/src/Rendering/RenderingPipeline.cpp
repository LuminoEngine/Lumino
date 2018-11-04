
#include "Internal.hpp"
#include "RenderingPipeline.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "UnLigitingSceneRenderer.hpp"
#include "RenderTargetTextureCache.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderingPipeline

void RenderingPipeline::initialize()
{
    //RenderingManager* manager = detail::EngineDomain::renderingManager();
    //m_frameBufferCache = makeRef<detail::FrameBufferCache>(
    //    manager->renderTargetTextureCacheManager(), manager->depthBufferCacheManager());
}

//==============================================================================
// SceneRenderingPipeline

SceneRenderingPipeline::SceneRenderingPipeline()
    : m_sceneRenderer(nullptr)
{
}

void SceneRenderingPipeline::initialize()
{
    RenderingPipeline::initialize();
    RenderingManager* manager = detail::EngineDomain::renderingManager();

    m_sceneRenderer = makeRef<detail::ClusteredShadingSceneRenderer>();
    m_sceneRenderer->initialize(manager);

    m_sceneRenderer_ImageEffectPhase = makeRef<detail::UnLigitingSceneRenderer>();
    m_sceneRenderer_ImageEffectPhase->initialize(manager);
}

void SceneRenderingPipeline::render(
    GraphicsContext* graphicsContext,
    const FrameBuffer& frameBuffer,
    const detail::CameraInfo* mainCameraInfo,
    const List<detail::DrawElementListCollector*>* elementListManagers)
{
    m_elementListManagers = elementListManagers;
    m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());

    m_sceneRenderer->render(graphicsContext, this, frameBuffer, *mainCameraInfo, RendringPhase::Default);

    {
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, frameBuffer, camera, RendringPhase::ImageEffect);
    }

    // Œë—p–hŽ~
    m_renderingFrameBufferSize = SizeI();
    m_elementListManagers = nullptr;
}


//==============================================================================
// FlatRenderingPipeline

FlatRenderingPipeline::FlatRenderingPipeline()
	: m_sceneRenderer(nullptr)
{
}

void FlatRenderingPipeline::initialize()
{
	RenderingPipeline::initialize();
	RenderingManager* manager = detail::EngineDomain::renderingManager();

	m_sceneRenderer = makeRef<detail::UnLigitingSceneRenderer>();
	m_sceneRenderer->initialize(manager);

    m_sceneRenderer_ImageEffectPhase = makeRef<detail::UnLigitingSceneRenderer>();
    m_sceneRenderer_ImageEffectPhase->initialize(manager);
}

void FlatRenderingPipeline::render(
	GraphicsContext* graphicsContext,
	const FrameBuffer& frameBuffer,
	const detail::CameraInfo* mainCameraInfo,
	const List<detail::DrawElementListCollector*>* elementListManagers)
{
	m_elementListManagers = elementListManagers;
	m_renderingFrameBufferSize = SizeI(frameBuffer.renderTarget[0]->width(), frameBuffer.renderTarget[0]->height());

	m_sceneRenderer->render(graphicsContext, this, frameBuffer, *mainCameraInfo, RendringPhase::Default);


    {
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, frameBuffer, camera, RendringPhase::ImageEffect);
    }

	// Œë—p–hŽ~
	m_renderingFrameBufferSize = SizeI();
	m_elementListManagers = nullptr;
}

} // namespace detail
} // namespace ln

