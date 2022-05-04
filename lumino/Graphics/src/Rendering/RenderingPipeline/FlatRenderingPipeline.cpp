#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoGraphics/RHI/RenderPass.hpp>
//#include "../Graphics/RenderTargetTextureCache.hpp"
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include <LuminoGraphics/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>
#include "../RenderStage.hpp"
#include "../RenderElement.hpp"
#include "../UnLigitingSceneRenderer.hpp"
#include "../RenderingProfiler.hpp"

namespace ln {

//==============================================================================
// FlatRenderingPipeline

FlatRenderingPipeline::FlatRenderingPipeline()
	: m_sceneRenderer(nullptr)
{
}

void FlatRenderingPipeline::init()
{
	RenderingPipeline::init();
	detail::RenderingManager* manager = detail::RenderingManager::instance();

	m_sceneRenderer = makeRef<detail::SceneRenderer>();
	m_sceneRenderer->init();

    //m_sceneRenderer_PostEffectPhase = makeRef<detail::SceneRenderer>();
    //m_sceneRenderer_PostEffectPhase->init();

    m_unlitRendererPass = makeRef<detail::UnLigitingSceneRendererPass>();
    m_unlitRendererPass->init(manager, false);

    m_unlitRendererPass_PostEffect = makeRef<detail::UnLigitingSceneRendererPass>();
    m_unlitRendererPass_PostEffect->init(manager, true);
}

void FlatRenderingPipeline::onRender(
    GraphicsCommandList* graphicsContext,
    RenderingContext* renderingContext,
    RenderTargetTexture* renderTarget,
    const ClearInfo& mainPassClearInfo,
    const RenderView* renderView,
    detail::CommandListServer* commandListServer,
    const detail::SceneGlobalRenderParams* sceneGlobalParams) {
    //detail::RenderingProfiler* profiler = detail::RenderingManager::instance()->profiler().get();
    //profiler->beginSceneRenderer("Flat");

    // Culling
    {
        m_cullingResult.cull(renderView, renderingContext, commandListServer);
    }


	m_renderingFrameBufferSize = SizeI(renderTarget->width(), renderTarget->height());

    detail::RenderViewInfo renderViewInfo;
    renderView->viewProjection(RenderPart::Geometry)->makeCameraInfo(&renderViewInfo.cameraInfo);

    auto depthBuffer = DepthBuffer::getTemporary(renderTarget->width(), renderTarget->height());

    //clear(graphicsContext, renderTarget, clearInfo);
    m_unlitRendererPass->setClearInfo(mainPassClearInfo);

    m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Geometry, nullptr, &m_cullingResult);
#if LN_USE_KANATA
    m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_unlitRendererPass);
    m_sceneRenderer->render_Kanata(graphicsContext);
#else
    //m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::Geometry, nullptr, &m_cullingResult);
	//m_sceneRenderer->render(graphicsContext, this, renderTarget, depthBuffer, *mainCameraInfo);
    //for (SceneRendererPass* pass : m_sceneRenderer->m_renderingPassList) {
    m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass);
    //}
#endif

	// TODO: ひとまずテストとしてデバッグ用グリッドを描画したいため、効率は悪いけどここで BeforeTransparencies をやっておく。
	//m_sceneRenderer->render(graphicsContext, this, renderTarget, localClearInfo, *mainCameraInfo, RenderPart::Gizmo, nullptr);

    {
        //ClearInfo localClearInfo = { ClearFlags::None, Color(), 1.0f, 0x00 };

        detail::RenderViewInfo renderViewInfo;
        //CameraInfo camera;
        renderView->viewProjection(RenderPart::PostEffect)->makeCameraInfo(&renderViewInfo.cameraInfo);
        m_sceneRenderer->prepare(graphicsContext, this, renderingContext, renderViewInfo, RenderPart::PostEffect, nullptr, &m_cullingResult);
#if LN_USE_KANATA
        m_sceneRenderer->buildBatchList_Kanata(graphicsContext, &m_cullingResult, renderTarget, depthBuffer, m_unlitRendererPass_PostEffect);
        m_sceneRenderer->render_Kanata(graphicsContext);
#else
        //m_sceneRenderer_PostEffectPhase->render(graphicsContext, this, renderTarget, depthBuffer, renderViewInfo.cameraInfo);
        //for (SceneRendererPass* pass : m_sceneRenderer_PostEffectPhase->m_renderingPassList) {
        m_sceneRenderer->renderPass_Legacy(graphicsContext, renderTarget, depthBuffer, m_unlitRendererPass_PostEffect);
        //}
#endif
    }

    // TODO: scoped
    DepthBuffer::releaseTemporary(depthBuffer);

	// 誤用防止
	m_renderingFrameBufferSize = SizeI();

    //profiler->endSceneRenderer();
}

} // namespace ln

