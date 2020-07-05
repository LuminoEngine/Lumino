
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
    Texture* g_viewDepthMap = nullptr;
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

#define USE_TMP 1

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

    m_sceneRenderer_PostEffectPhase = makeRef<detail::UnLigitingSceneRenderer>();
    m_sceneRenderer_PostEffectPhase->init(manager, true);


    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);
#ifndef USE_TMP
    m_viweNormalAndDepthBuffer = RenderTargetTexture::create(640, 480, TextureFormat::RGBA8);
    m_viweNormalAndDepthBuffer->setSamplerState(m_samplerState);
    m_viweDepthBuffer = RenderTargetTexture::create(640, 480, TextureFormat::RGBA32F);
    m_viweDepthBuffer->setSamplerState(m_samplerState);
    m_materialBuffer = RenderTargetTexture::create(640, 480, TextureFormat::RGBA8);
    m_materialBuffer->setSamplerState(m_samplerState);
    g_viewNormalMap = m_viweNormalAndDepthBuffer;
    g_viewDepthMap = m_viweDepthBuffer;
    g_viewMaterialMap = m_materialBuffer;
#endif 

    m_shadowMap = makeObject<RenderTargetTexture>(1024, 1024, TextureFormat::RGBA32F, false);
    m_shadowMap->setSamplerState(m_samplerState);
    m_shadowMapDepthBuffer = makeObject<DepthBuffer>(1024, 1024);

    m_renderPass = makeObject<RenderPass>();
}

void SceneRenderingPipeline::prepare(RenderTargetTexture* renderTarget)
{
    // ポストエフェクトで参照したいテクスチャのインスタンスだけ、先に作っておく

    m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    // Prepare G-Buffers
    {
#ifdef USE_TMP
        m_viweNormalAndDepthBuffer = RenderTargetTexture::realloc(m_viweNormalAndDepthBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA8, false, m_samplerState);
        m_viweDepthBuffer = RenderTargetTexture::realloc(m_viweDepthBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA32F, false, m_samplerState);
        m_materialBuffer = RenderTargetTexture::realloc(m_materialBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA8, false, m_samplerState);
        g_viewNormalMap = m_viweNormalAndDepthBuffer;
        g_viewDepthMap = m_viweDepthBuffer;
        g_viewMaterialMap = m_materialBuffer;
#endif

        m_objectIdBuffer = RenderTargetTexture::realloc(m_objectIdBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::R32S, false, m_samplerState);

        assert(m_viweNormalAndDepthBuffer->format() == TextureFormat::RGBA8);
        assert(m_viweDepthBuffer->format() == TextureFormat::RGBA32F);
        assert(m_materialBuffer->format() == TextureFormat::RGBA8);
        assert(m_objectIdBuffer->format() == TextureFormat::R32S);
    }

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


    // Prepare G-Buffers
    {
        // (0, 0, 0, 0) でクリアすると、G-Buffer 作成時に書き込まれなかったピクセルの法線が nan になる。
        // Vulkan だと PixelShader の出力が nan だと書き込みスキップされるみたいで、前フレームのごみが残ったりした。
        // 他の Backend だとどうだとかあるし、あんまり nan を出すべきではないだろうということで、とりあえず Z+ を入れておく。
        m_renderPass->setRenderTarget(0, m_viweNormalAndDepthBuffer);
        m_renderPass->setRenderTarget(1, m_viweDepthBuffer);
        m_renderPass->setRenderTarget(2, m_materialBuffer);
        m_renderPass->setRenderTarget(3, m_objectIdBuffer);
        m_renderPass->setClearValues(ClearFlags::Color, Color(0, 0, 1, 1), 1.0f, 0);
        graphicsContext->beginRenderPass(m_renderPass);
        graphicsContext->endRenderPass();
    }

    RenderViewInfo renderViewInfo;
    renderViewInfo.cameraInfo = *mainCameraInfo;
    renderViewInfo.mainLightShadowMap = m_shadowMap;
    renderViewInfo.mainLightShadowMapPixelSize = Size(m_shadowMap->width(), m_shadowMap->height());


    auto depthBuffer = DepthBuffer::getTemporary(renderTarget->width(), renderTarget->height());

    //clear(graphicsContext, renderTarget, clearInfo);

    //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };

    //m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPhaseClass::BackgroundSky, nullptr);

    m_sceneRenderer->mainRenderPass()->setClearInfo(mainPassClearInfo);
    m_sceneRenderer->prepare(this, renderViewInfo, RenderPhaseClass::Geometry, sceneGlobalParams);
    m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);


    // TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
    ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
    m_sceneRenderer->mainRenderPass()->setClearInfo(localClearInfo); // 2回目の描画になるので、最初の結果が消えないようにしておく。
    m_sceneRenderer->prepare(this, renderViewInfo, RenderPhaseClass::Gizmo, nullptr);
    m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);

    {
        //CameraInfo camera;
        //camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
		//m_sceneRenderer_PostEffectPhase->lightOcclusionMap = m_sceneRenderer->lightOcclusionPass()->lightOcclusionMap();
        m_sceneRenderer_PostEffectPhase->prepare(this, renderViewInfo, RenderPhaseClass::PostEffect, nullptr);  // TODO: PostEffect なので ZSort 要らないモード追加していいかも
        m_sceneRenderer_PostEffectPhase->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    }

    // Release G-Buffer
    {
#ifdef USE_TMP
        //RenderTargetTexture::releaseTemporary(m_viweNormalAndDepthBuffer);
        //m_viweNormalAndDepthBuffer = nullptr;
        //RenderTargetTexture::releaseTemporary(m_viweDepthBuffer);
        //m_viweDepthBuffer = nullptr;
        //RenderTargetTexture::releaseTemporary(m_materialBuffer);
        //m_materialBuffer = nullptr;

#endif
    }

    // TODO: scoped
    DepthBuffer::releaseTemporary(depthBuffer);

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

    m_sceneRenderer_PostEffectPhase = makeRef<detail::UnLigitingSceneRenderer>();
    m_sceneRenderer_PostEffectPhase->init(manager);
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

    RenderViewInfo renderViewInfo;
    renderViewInfo.cameraInfo = *mainCameraInfo;

    auto depthBuffer = DepthBuffer::getTemporary(renderTarget->width(), renderTarget->height());

    //clear(graphicsContext, renderTarget, clearInfo);
    m_sceneRenderer->mainRenderPass()->setClearInfo(mainPassClearInfo);
    m_sceneRenderer->prepare(this, renderViewInfo, RenderPhaseClass::Geometry, nullptr);
	m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);

	// TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
	//m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPhaseClass::Gizmo, nullptr);

    {
        //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };

        RenderViewInfo renderViewInfo;
        //CameraInfo camera;
        renderViewInfo.cameraInfo.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer_PostEffectPhase->prepare(this, renderViewInfo, RenderPhaseClass::PostEffect, nullptr);
        m_sceneRenderer_PostEffectPhase->render(graphicsContext, this, renderTarget, depthBuffer, renderViewInfo.cameraInfo);
    }

    // TODO: scoped
    DepthBuffer::releaseTemporary(depthBuffer);

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();
    m_elementListCollector = nullptr;
}

} // namespace detail
} // namespace ln

