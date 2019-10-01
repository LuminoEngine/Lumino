
#include "Internal.hpp"
#include "../Graphics/RenderTargetTextureCache.hpp"
#include "RenderingPipeline.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "UnLigitingSceneRenderer.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderingPipeline

void RenderingPipeline::init()
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

SceneRenderingPipeline::~SceneRenderingPipeline()
{
}

void SceneRenderingPipeline::init()
{
    RenderingPipeline::init();
    RenderingManager* manager = detail::EngineDomain::renderingManager();

    m_sceneRenderer = makeRef<detail::ClusteredShadingSceneRenderer>();
    m_sceneRenderer->init(manager);

    m_sceneRenderer_ImageEffectPhase = makeRef<detail::UnLigitingSceneRenderer>();
    m_sceneRenderer_ImageEffectPhase->init(manager);
}

void SceneRenderingPipeline::render(
    GraphicsContext* graphicsContext,
	RenderTargetTexture* renderTarget,
    //const FrameBuffer& frameBuffer,
    const detail::CameraInfo* mainCameraInfo,
    const List<detail::DrawElementListCollector*>* elementListManagers)
{
    m_elementListManagers = elementListManagers;
    m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    m_sceneRenderer->render(graphicsContext, this, renderTarget, *mainCameraInfo, RendringPhase::Default);


    // TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
    m_sceneRenderer->render(graphicsContext, this, renderTarget, *mainCameraInfo, RendringPhase::BeforeTransparencies);

    {
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, renderTarget, camera, RendringPhase::ImageEffect);
    }

    // 誤用防止
    m_renderingFrameBufferSize = SizeI();
    m_elementListManagers = nullptr;
}


//==============================================================================
// FlatRenderingPipeline

FlatRenderingPipeline::FlatRenderingPipeline()
	: m_sceneRenderer(nullptr)
{
}

void FlatRenderingPipeline::init()
{
	RenderingPipeline::init();
	RenderingManager* manager = detail::EngineDomain::renderingManager();

	m_sceneRenderer = makeRef<detail::UnLigitingSceneRenderer>();
	m_sceneRenderer->init(manager);

    m_sceneRenderer_ImageEffectPhase = makeRef<detail::UnLigitingSceneRenderer>();
    m_sceneRenderer_ImageEffectPhase->init(manager);
}

void FlatRenderingPipeline::render(
	GraphicsContext* graphicsContext,
	RenderTargetTexture* renderTarget,
	//const FrameBuffer& frameBuffer,
	const detail::CameraInfo* mainCameraInfo,
	const List<detail::DrawElementListCollector*>* elementListManagers)
{
	m_elementListManagers = elementListManagers;
	m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

	m_sceneRenderer->render(graphicsContext, this, renderTarget, *mainCameraInfo, RendringPhase::Default);

    {
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, renderTarget, camera, RendringPhase::ImageEffect);
    }

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();
	m_elementListManagers = nullptr;
}

} // namespace detail
} // namespace ln

