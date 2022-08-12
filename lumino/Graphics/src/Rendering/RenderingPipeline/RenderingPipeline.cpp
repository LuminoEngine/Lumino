#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/RenderingPipeline/RenderingPipeline.hpp>
#include "../CommandListServer.hpp"

namespace ln {

//==============================================================================
// RenderingPipeline

void RenderingPipeline::init()
{
    //RenderingManager* manager = detail::RenderingManager::instance();
    //m_frameBufferCache = makeRef<detail::FrameBufferCache>(
    //    manager->renderTargetTextureCacheManager(), manager->depthBufferCacheManager());
    //m_clearRenderPass = makeObject_deprecated<RenderPass>();
}

//void RenderingPipeline::clear(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget, const ClearInfo& clearInfo)
//{
//    if (clearInfo.flags != ClearFlags::None) {
//        m_clearRenderPass->setRenderTarget(0, renderTarget);
//        m_clearRenderPass->setClearValues(clearInfo.flags, clearInfo.color, clearInfo.depth, clearInfo.stencil);
//        graphicsContext->beginRenderPass(m_clearRenderPass);
//        graphicsContext->endRenderPass();
//    }
//}

void RenderingPipeline::prepare(RenderView* renderView, RenderTargetTexture* renderTarget) {
    onPrepare(renderView, renderTarget);
}

void RenderingPipeline::onPrepare(RenderView* renderView, RenderTargetTexture* renderTarget) {
}

void RenderingPipeline::render(
    GraphicsCommandList* graphicsContext,
    RenderingContext* renderingContext,
    RenderTargetTexture* renderTarget,
    const RenderView* renderView,
    const detail::SceneGlobalRenderParams* sceneGlobalParams) {

    ClearInfo mainPassClearInfo;
    mainPassClearInfo.color = renderView->backgroundColor();
    mainPassClearInfo.depth = 1.0f;
    mainPassClearInfo.stencil = 0x00;
    if (renderView->clearMode() == SceneClearMode::ColorAndDepth) {
        mainPassClearInfo.flags = ClearFlags::All;
    }
    else {
        mainPassClearInfo.flags = ClearFlags::Depth | ClearFlags::Stencil;
    }
    onRender(graphicsContext, renderingContext, renderTarget, mainPassClearInfo, renderView, renderingContext->commandListServer(), sceneGlobalParams);
}

} // namespace ln

