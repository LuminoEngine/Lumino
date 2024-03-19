#include <LuminoEngine/GPU/SamplerState.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
//#include "../Graphics/RenderTargetTextureCache.hpp"
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/StandardRenderingPipeline.hpp>
#include "../RenderStage.hpp"
#include "../RenderElement.hpp"
#include "../ClusteredShadingSceneRenderer.hpp"
#include "../UnLigitingSceneRenderer.hpp"
#include "../RenderingProfiler.hpp"

namespace ln {

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
    detail::RenderingManager* manager = detail::RenderingManager::instance();

    m_sceneRenderer = makeRef<detail::ClusteredShadingSceneRenderer>();
    m_sceneRenderer->init(manager);

    m_sceneRenderer_PostEffectPhase = makeRef<detail::SceneRenderer>();
    m_sceneRenderer_PostEffectPhase->init();

    m_unlitRendererPass_Normal = makeRef<detail::UnLigitingSceneRendererPass>();
    m_unlitRendererPass_Normal->init(manager, false);

    m_unlitRendererPass_PostEffect = makeRef<detail::UnLigitingSceneRendererPass>();
    m_unlitRendererPass_PostEffect->init(manager, true);


    m_samplerState = makeObject_deprecated<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    m_shadowMap = makeObject_deprecated<RenderTargetTexture>(1024, 1024, TextureFormat::RGBA32F, false, false);
    m_shadowMap->setSamplerState(m_samplerState);
    m_shadowMapDepthBuffer = makeObject_deprecated<DepthBuffer>(1024, 1024);

    m_renderPass = makeObject_deprecated<RenderPass>();
}

void SceneRenderingPipeline::onPrepare(RenderView* renderView, RenderTargetTexture* renderTarget) {
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

        renderView->setBuiltinRenderTexture(BuiltinRenderTextureType::ViewNormalMap, m_viweNormalAndDepthBuffer);
        renderView->setBuiltinRenderTexture(BuiltinRenderTextureType::ViewDepthMap, m_viweDepthBuffer);
        renderView->setBuiltinRenderTexture(BuiltinRenderTextureType::ViewMaterialMap, m_materialBuffer);
        renderView->setBuiltinRenderTexture(BuiltinRenderTextureType::ObjectId, m_objectIdBuffer);
    }


    if (m_hdrEnabled) {
        m_hdrRenderTarget = RenderTargetTexture::realloc(m_hdrRenderTarget, m_renderingFrameBufferSize.width, m_renderingFrameBufferSize.height, TextureFormat::RGBA32F, false, SamplerState::pointClamp());
        // m_hdrRenderTarget->m_msaa = true;
    }

}

void SceneRenderingPipeline::onRender(
    GraphicsCommandList* graphicsContext,
    RenderingContext* renderingContext,
	RenderTargetTexture* renderTarget,
    const ClearInfo& mainPassClearInfo,
    const RenderView* renderView,
    detail::CommandListServer* commandListServer,
	const detail::SceneGlobalRenderParams* sceneGlobalParams)
{
    RenderTargetTexture* actualTarget = (m_hdrRenderTarget) ? m_hdrRenderTarget.get() : renderTarget;
    if (m_hdrRenderTarget) {
        LN_NOTIMPLEMENTED();
    }


    detail::RenderingProfiler* profiler = detail::RenderingManager::instance()->profiler().get();
    profiler->beginSceneRenderer("Standard");


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

    detail::RenderViewInfo renderViewInfo;
    renderView->viewProjection(RenderPart::Geometry)->makeCameraInfo(&renderViewInfo.cameraInfo);

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
#if LN_USE_KANATA
    if (m_sceneRenderer->mainLightInfo()) {
        m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_sceneRenderer->shadowCasterPass());
    }
    m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_sceneRenderer->gbufferPass());
    m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_sceneRenderer->geometryPass());
    m_sceneRenderer->render_Kanata(graphicsContext);
#else
    //m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    //for (SceneRendererPass* pass : m_sceneRenderer->m_renderingPassList) {
    //    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, pass);
    //}
    if (m_sceneRenderer->mainLightInfo()) {
        m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->shadowCasterPass());
    }
    m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->gbufferPass());
    m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->geometryPass());
#endif


    // TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
    ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };
    m_sceneRenderer->mainRenderPass()->setClearInfo(localClearInfo); // 2回目の描画になるので、最初の結果が消えないようにしておく。
    m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Gizmo, nullptr, &m_cullingResult);
#if LN_USE_KANATA
    m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_sceneRenderer->geometryPass());
    m_sceneRenderer->render_Kanata(graphicsContext);
#else
    //m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    //for (SceneRendererPass* pass : m_sceneRenderer->m_renderingPassList) {
    //    m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, pass);
    //}
    //m_sceneRenderer->renderPass(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->gbufferPass());
    m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_sceneRenderer->geometryPass());
#endif



    {
        //CameraInfo camera;
        //camera.makeUnproject(m_renderingFrameBufferSize.toFloatSize());
		//m_sceneRenderer_PostEffectPhase->lightOcclusionMap = m_sceneRenderer->lightOcclusionPass()->lightOcclusionMap();
        m_sceneRenderer_PostEffectPhase->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::PostEffect, nullptr, &m_cullingResult);  // TODO: PostEffect なので ZSort 要らないモード追加していいかも
#if LN_USE_KANATA
        m_sceneRenderer_PostEffectPhase->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_unlitRendererPass_PostEffect);
        m_sceneRenderer_PostEffectPhase->render_Kanata(graphicsContext);
#else
        //m_sceneRenderer_PostEffectPhase->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
        //for (SceneRendererPass* pass : m_sceneRenderer_PostEffectPhase->m_renderingPassList) {
        m_sceneRenderer_PostEffectPhase->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass_PostEffect);
        //}
#endif
    }


    // Gizmo2D
    {
        // Depth だけクリア。そうしないと Gizmo が 3D コンテンツの後ろに隠れてしまう。
        ClearInfo localClearInfo = { ClearFlags::Depth, Color(), 1.0f, 0x00 };
        m_unlitRendererPass_Normal->setClearInfo(localClearInfo);
        renderView->viewProjection(RenderPart::Gizmo2D)->makeCameraInfo(&renderViewInfo.cameraInfo);
        m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Gizmo2D, nullptr, &m_cullingResult);
#if LN_USE_KANATA
        m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_unlitRendererPass_Normal);
        m_sceneRenderer->render_Kanata(graphicsContext);
#else
        m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass_Normal);
#endif
    }



    // TODO: scoped
    DepthBuffer::releaseTemporary(depthBuffer);

    // 誤用防止
    m_renderingFrameBufferSize = SizeI();

    profiler->endSceneRenderer();
}

} // namespace ln

