
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
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
    m_clearRenderPass = makeObject<RenderPass>();
}

void RenderingPipeline::clear(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, const ClearInfo& clearInfo)
{
    if (clearInfo.flags != ClearFlags::None) {
        m_clearRenderPass->setRenderTarget(0, renderTarget);
        m_clearRenderPass->setClearValues(clearInfo.flags, clearInfo.color, clearInfo.depth, clearInfo.stencil);
        graphicsContext->beginRenderPass(m_clearRenderPass);
        graphicsContext->endRenderPass();
    }
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
    const ClearInfo& clearInfo,
    const detail::CameraInfo* mainCameraInfo,
    const List<detail::DrawElementListCollector*>* elementListManagers)
{
    m_elementListManagers = elementListManagers;
    m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    clear(graphicsContext, renderTarget, clearInfo);

    ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
    m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RendringPhase::Default);


    // TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
    m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RendringPhase::BeforeTransparencies);

    {
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, renderTarget, localClearInfo, camera, RendringPhase::ImageEffect);
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
    const ClearInfo& clearInfo,
	const detail::CameraInfo* mainCameraInfo,
	const List<detail::DrawElementListCollector*>* elementListManagers)
{
	m_elementListManagers = elementListManagers;
	m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    clear(graphicsContext, renderTarget, clearInfo);

    ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
	m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RendringPhase::Default);

    {
        ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, renderTarget, localClearInfo, camera, RendringPhase::ImageEffect);
    }

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();
	m_elementListManagers = nullptr;
}

} // namespace detail
} // namespace ln

