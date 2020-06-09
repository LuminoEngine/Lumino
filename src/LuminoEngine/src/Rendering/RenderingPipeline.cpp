
#include "Internal.hpp"
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include "../Graphics/RenderTargetTextureCache.hpp"
#include "RenderStage.hpp"
#include "RenderElement.hpp"
#include "RenderingPipeline.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "UnLigitingSceneRenderer.hpp"
#include "RenderingManager.hpp"

namespace ln {
    Texture* g_viewNormalMap = nullptr;
    Texture* g_viewMaterialMap = nullptr;
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

//void RenderingPipeline::clear(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget, const ClearInfo& clearInfo)
//{
//    if (clearInfo.flags != ClearFlags::None) {
//        m_clearRenderPass->setRenderTarget(0, renderTarget);
//        m_clearRenderPass->setClearValues(clearInfo.flags, clearInfo.color, clearInfo.depth, clearInfo.stencil);
//        graphicsContext->beginRenderPass(m_clearRenderPass);
//        graphicsContext->endRenderPass();
//    }
//}

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


    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);
    m_viweNormalAndDepthBuffer = RenderTargetTexture::create(640, 480, TextureFormat::RGBA32F);
    m_viweNormalAndDepthBuffer->setSamplerState(m_samplerState);
    m_materialBuffer = RenderTargetTexture::create(640, 480, TextureFormat::RGBA32F);
    m_materialBuffer->setSamplerState(m_samplerState);
    g_viewNormalMap = m_viweNormalAndDepthBuffer;
    g_viewMaterialMap = m_materialBuffer;
}

void SceneRenderingPipeline::render(
    GraphicsContext* graphicsContext,
	RenderTargetTexture* renderTarget,
    const ClearInfo& mainPassClearInfo,
    const detail::CameraInfo* mainCameraInfo,
    detail::DrawElementListCollector* elementListCollector,
	const detail::SceneGlobalRenderParams* sceneGlobalParams)
{
    m_elementListCollector = elementListCollector;
    m_elementListCollector->classify();

    m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());


    // Prepare G-Buffers
    {

    }


    //clear(graphicsContext, renderTarget, clearInfo);

    //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };

    //m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPhaseClass::BackgroundSky, nullptr);

    m_sceneRenderer->mainRenderPass()->setClearInfo(mainPassClearInfo);

    m_sceneRenderer->render(graphicsContext, this, renderTarget, *mainCameraInfo, RenderPhaseClass::Geometry, sceneGlobalParams);


    // TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
    ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
    m_sceneRenderer->mainRenderPass()->setClearInfo(localClearInfo); // 2回目の描画になるので、最初の結果が消えないようにしておく。
    //m_sceneRenderer->render(graphicsContext, this, renderTarget, *mainCameraInfo, RenderPhaseClass::Gizmo, nullptr);
    // TODO: ↑同じ SceneRenderer を2回 render するのはダメ。GBuffer がクリアされるので、この後のポストエフェクトの処理で利用できなくなる。
    // 今は SSR テスト用に回避したいので、消しておく。

    {
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
		m_sceneRenderer_ImageEffectPhase->lightOcclusionMap = m_sceneRenderer->lightOcclusionPass()->lightOcclusionMap();
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, renderTarget, camera, RenderPhaseClass::ImageEffect, nullptr);
    }

    // 誤用防止
    m_renderingFrameBufferSize = SizeI();
    m_elementListCollector = nullptr;
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
    const ClearInfo& mainPassClearInfo,
	const detail::CameraInfo* mainCameraInfo,
    detail::DrawElementListCollector* elementListCollector)
{
    m_elementListCollector = elementListCollector;
    m_elementListCollector->classify();

	m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    //clear(graphicsContext, renderTarget, clearInfo);
    m_sceneRenderer->mainRenderPass()->setClearInfo(mainPassClearInfo);

	m_sceneRenderer->render(graphicsContext, this, renderTarget, *mainCameraInfo, RenderPhaseClass::Geometry, nullptr);

	// TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
	//m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPhaseClass::Gizmo, nullptr);

    {
        //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
        CameraInfo camera;
        camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_ImageEffectPhase->render(graphicsContext, this, renderTarget, camera, RenderPhaseClass::ImageEffect, nullptr);
    }

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();
    m_elementListCollector = nullptr;
}

} // namespace detail
} // namespace ln

