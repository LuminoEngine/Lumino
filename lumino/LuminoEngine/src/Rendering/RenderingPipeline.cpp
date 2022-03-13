
#include "Internal.hpp"
#include <LuminoGraphics/SamplerState.hpp>
#include <LuminoGraphics/RenderPass.hpp>
//#include "../Graphics/RenderTargetTextureCache.hpp"
#include "RenderStage.hpp"
#include "RenderElement.hpp"
#include "RenderingPipeline.hpp"
#include "ClusteredShadingSceneRenderer.hpp"
#include "UnLigitingSceneRenderer.hpp"
#include "RenderingManager.hpp"
#include "RenderingProfiler.hpp"

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

    m_sceneRenderer_PostEffectPhase = makeRef<detail::SceneRenderer>();
    m_sceneRenderer_PostEffectPhase->init();

    m_unlitRendererPass_Normal = makeRef<UnLigitingSceneRendererPass>();
    m_unlitRendererPass_Normal->init(manager, false);

    m_unlitRendererPass_PostEffect = makeRef<UnLigitingSceneRendererPass>();
    m_unlitRendererPass_PostEffect->init(manager, true);


    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    m_shadowMap = makeObject<RenderTargetTexture>(1024, 1024, TextureFormat::RGBA32F, false, false);
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
        m_viweNormalAndDepthBuffer = RenderTargetTexture::realloc(m_viweNormalAndDepthBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA8, false, m_samplerState);
        m_viweDepthBuffer = RenderTargetTexture::realloc(m_viweDepthBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA32F, false, m_samplerState);
        m_materialBuffer = RenderTargetTexture::realloc(m_materialBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA8, false, m_samplerState);

        m_objectIdBuffer = RenderTargetTexture::realloc(m_objectIdBuffer, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::R32S, false, m_samplerState);

        assert(m_viweNormalAndDepthBuffer->format() == TextureFormat::RGBA8);
        assert(m_viweDepthBuffer->format() == TextureFormat::RGBA32F);
        assert(m_materialBuffer->format() == TextureFormat::RGBA8);
        assert(m_objectIdBuffer->format() == TextureFormat::R32S);
    }
}

void SceneRenderingPipeline::render(
    GraphicsContext* graphicsContext,
    RenderingContext* renderingContext,
	RenderTargetTexture* renderTarget,
    const ClearInfo& mainPassClearInfo,
    const RenderView* renderView,
    detail::DrawElementList* elementList,
    detail::CommandListServer* commandListServer,
	const detail::SceneGlobalRenderParams* sceneGlobalParams)
{

    RenderingProfiler* profiler = detail::EngineDomain::renderingManager()->profiler().get();
    profiler->beginSceneRenderer("Standard");

    m_elementList = elementList;
    //m_commandListServer = commandListServer;

    // Culling
    {
        m_cullingResult.cull(renderView, renderingContext, commandListServer);
    }

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
    renderViewInfo.cameraInfo = renderView->viewProjection(RenderPart::Geometry);

    {
        const auto* light = m_sceneRenderer->mainLightInfo();

        if (light && light->shadowEnabled()) {
            const auto& camera = renderViewInfo.cameraInfo;

            // ↑のパラメータで指定された範囲を Ortho に収めるような視点情報を作る
            const auto lookAt = camera.viewPosition + camera.viewDirection * (light->shadowCameraZFar / 2);
            const auto lightPos = lookAt - light->m_direction * (light->shadowLightZFar / 2);
#ifdef LN_COORD_RH
            const auto view = Matrix::makeLookAtRH(lightPos, lookAt, Vector3::UnitY);
            const auto proj = Matrix::makeOrthoRH(light->shadowCameraZFar, light->shadowCameraZFar, 0.5, light->shadowLightZFar);
#else
            const auto view = Matrix::makeLookAtLH(lightPos, lookAt, Vector3::UnitY);
            const auto proj = Matrix::makeOrthoLH(light->shadowCameraZFar, light->shadowCameraZFar, 0.5, light->shadowLightZFar);
#endif
            renderViewInfo.mainLightViewProjection = Matrix::multiply(view, proj);
            renderViewInfo.mainLightShadowMap = m_shadowMap;
            renderViewInfo.mainLightShadowMapPixelSize = Size(renderViewInfo.mainLightShadowMap->width(), renderViewInfo.mainLightShadowMap->height());
            renderViewInfo.mainLightShadowDensity = 0.5f;
        }
        else {
            // TODO: 今はダミーテクスチャを使うことで影を生成しないようにしているが、
            // ソフトシャドウの処理は動いているので無駄がある。シャドウの濃さみたいなパラメータを用意して、
            // 0 なら PS でシャドウ作らないみたいな対応が欲しい。
            renderViewInfo.mainLightShadowMap = Texture2D::whiteTexture();
            renderViewInfo.mainLightShadowMapPixelSize = Size(renderViewInfo.mainLightShadowMap->width(), renderViewInfo.mainLightShadowMap->height());
            renderViewInfo.mainLightShadowDensity = 0.0f;
        }
    }

    auto depthBuffer = DepthBuffer::getTemporary(renderTarget->width(), renderTarget->height());

    //clear(graphicsContext, renderTarget, clearInfo);

    //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };

    //m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPart::BackgroundSky, nullptr);

    m_sceneRenderer->mainRenderPass()->setClearInfo(mainPassClearInfo);
    m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Geometry, sceneGlobalParams, &m_cullingResult);
    //m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    //for (SceneRendererPass* pass : m_sceneRenderer->m_renderingPassList) {
    //    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, pass);
    //}
    if (m_sceneRenderer->mainLightInfo()) {
        m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->shadowCasterPass());
    }
    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->gbufferPass());
    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->geometryPass());


    // TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
    ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
    m_sceneRenderer->mainRenderPass()->setClearInfo(localClearInfo); // 2回目の描画になるので、最初の結果が消えないようにしておく。
    m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Gizmo, nullptr, &m_cullingResult);
    //m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    //for (SceneRendererPass* pass : m_sceneRenderer->m_renderingPassList) {
    //    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, pass);
    //}
    //m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->gbufferPass());
    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->geometryPass());



    {
        //CameraInfo camera;
        //camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
		//m_sceneRenderer_PostEffectPhase->lightOcclusionMap = m_sceneRenderer->lightOcclusionPass()->lightOcclusionMap();
        m_sceneRenderer_PostEffectPhase->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::PostEffect, nullptr, &m_cullingResult);  // TODO: PostEffect なので ZSort 要らないモード追加していいかも
        //m_sceneRenderer_PostEffectPhase->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
        //for (SceneRendererPass* pass : m_sceneRenderer_PostEffectPhase->m_renderingPassList) {
            m_sceneRenderer_PostEffectPhase->renderPass(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass_PostEffect);
        //}
    }


    // Gizmo2D
    {
        // Depth だけクリア。そうしないと Gizmo が 3D コンテンツの後ろに隠れてしまう。
        ClearInfo localClearInfo = { ClearFlags::Depth, Color(), 1.0f, 0x00 };
        m_unlitRendererPass_Normal->setClearInfo(localClearInfo);
        renderViewInfo.cameraInfo = renderView->viewProjection(RenderPart::Gizmo2D);
        m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Gizmo2D, nullptr, &m_cullingResult);
        m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass_Normal);
    }



    // TODO: scoped
    DepthBuffer::releaseTemporary(depthBuffer);

    // 誤用防止
    m_renderingFrameBufferSize = SizeI();
    m_elementList = nullptr;
    //m_commandListServer = nullptr;

    profiler->endSceneRenderer();
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

	m_sceneRenderer = makeRef<detail::SceneRenderer>();
	m_sceneRenderer->init();

    //m_sceneRenderer_PostEffectPhase = makeRef<detail::SceneRenderer>();
    //m_sceneRenderer_PostEffectPhase->init();

    m_unlitRendererPass = makeRef<UnLigitingSceneRendererPass>();
    m_unlitRendererPass->init(manager, false);

    m_unlitRendererPass_PostEffect = makeRef<UnLigitingSceneRendererPass>();
    m_unlitRendererPass_PostEffect->init(manager, true);
}

void FlatRenderingPipeline::render(
	GraphicsContext* graphicsContext,
    RenderingContext* renderingContext,
	RenderTargetTexture* renderTarget,
    const ClearInfo& mainPassClearInfo,
    const RenderView* renderView,
    detail::DrawElementList* elementList,
    detail::CommandListServer* commandListServer)
{
    RenderingProfiler* profiler = detail::EngineDomain::renderingManager()->profiler().get();
    profiler->beginSceneRenderer("Flat");

    // Culling
    {
        m_cullingResult.cull(renderView, renderingContext, commandListServer);
    }

    m_elementList = elementList;
    //m_commandListServer = commandListServer;

	m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    RenderViewInfo renderViewInfo;
    renderViewInfo.cameraInfo = renderView->viewProjection(RenderPart::Geometry);//*mainCameraInfo;

    auto depthBuffer = DepthBuffer::getTemporary(renderTarget->width(), renderTarget->height());

    //clear(graphicsContext, renderTarget, clearInfo);
    m_unlitRendererPass->setClearInfo(mainPassClearInfo);
    m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Geometry, nullptr, &m_cullingResult);
	//m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    //for (SceneRendererPass* pass : m_sceneRenderer->m_renderingPassList) {
        m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass);
    //}

	// TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
	//m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPart::Gizmo, nullptr);

    {
        //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };

        RenderViewInfo renderViewInfo;
        //CameraInfo camera;
        renderViewInfo.cameraInfo = renderView->viewProjection(RenderPart::PostEffect);//.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
        m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::PostEffect, nullptr, &m_cullingResult);
        //m_sceneRenderer_PostEffectPhase->render(graphicsContext, this, renderTarget, depthBuffer, renderViewInfo.cameraInfo);
        //for (SceneRendererPass* pass : m_sceneRenderer_PostEffectPhase->m_renderingPassList) {
        m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass_PostEffect);
        //}
    }

    // TODO: scoped
    DepthBuffer::releaseTemporary(depthBuffer);

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();
    m_elementList = nullptr;
    //m_commandListServer = nullptr;

    profiler->endSceneRenderer();
}

} // namespace detail
} // namespace ln

