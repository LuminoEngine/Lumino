
#include "../Internal.hpp"
#include <LuminoGraphics/GraphicsContext.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/Reflection/OffscreenWorldRenderView.hpp>
#include "../../../../Graphics/src/GraphicsManager.hpp"
#include "../../../../Graphics/src/RenderTargetTextureCache.hpp"
#include "../../Rendering/CommandListServer.hpp"
#include "../../Rendering/RenderingManager.hpp"
#include "../../Rendering/RenderingPipeline.hpp"
#include "../../Rendering/RenderElement.hpp"

namespace ln {

//==============================================================================
// OffscreenWorldRenderView

OffscreenWorldRenderView::OffscreenWorldRenderView()
    : m_renderTarget(nullptr)
{
}

OffscreenWorldRenderView::~OffscreenWorldRenderView()
{
}

void OffscreenWorldRenderView::init()
{
    RenderView::init();
    m_renderingManager = detail::EngineDomain::renderingManager();

    // TODO: SceneRenderingPipeline は WorldRenderView が持っているものとほとんど同じなので、
    // こっちでインスタンスを new するのは無駄が多い。共有したいところ。
    // ただ、GBuffe のように Viewサイズや視点に依存するデータを独立させる必要がある。
    m_sceneRenderingPipeline = makeRef<detail::SceneRenderingPipeline>();
    m_sceneRenderingPipeline->init();
}

void OffscreenWorldRenderView::setRenderTarget(RenderTargetTexture* renderTarget)
{
    m_renderTarget = renderTarget;
	// TODO: dp単位に変換
    setActualSize(Size(m_renderTarget->width(), m_renderTarget->height()));
}

RenderTargetTexture* OffscreenWorldRenderView::renderTarget() const
{
    return m_renderTarget;
}

//void OffscreenWorldRenderView::render()
//{
//	auto depthBuffer = DepthBuffer::getTemporary(m_renderTarget->width(), m_renderTarget->height());
//
//    GraphicsContext* context = m_renderingManager->graphicsManager()->graphicsContext();
//    context->setRenderTarget(0, m_renderTarget);
//    context->setDepthBuffer(depthBuffer);
//    WorldRenderView::render(context);
//
//    // TODO: scoped
//	DepthBuffer::releaseTemporary(depthBuffer);
//}
//

void OffscreenWorldRenderView::render(GraphicsContext* graphicsContext, World* targetWorld)
{
    RenderingContext* context = targetWorld->m_renderingContext;

    setClearMode(context->baseRenderView->clearMode());
    setBackgroundColor(context->baseRenderView->backgroundColor());

    ClearInfo clearInfo;
    clearInfo.color = backgroundColor();
    clearInfo.depth = 1.0f;
    clearInfo.stencil = 0x00;
    if (clearMode() == SceneClearMode::ColorAndDepth) {
        clearInfo.flags = ClearFlags::All;
    }
    else {
        clearInfo.flags = ClearFlags::Depth | ClearFlags::Stencil;
    }

    if (clearMode() == SceneClearMode::ColorAndDepth) {
    }
    else {
        LN_NOTIMPLEMENTED();
    }

    m_sceneRenderingPipeline->prepare(m_renderTarget);


    targetWorld->renderObjects();

    m_sceneRenderingPipeline->render(
        graphicsContext, targetWorld->m_renderingContext,
        m_renderTarget, clearInfo, this,
        targetWorld->m_renderingContext->commandList()->elementList(),
        targetWorld->m_renderingContext->commandListServer(),
        &targetWorld->m_combinedSceneGlobalRenderParams);

}

void OffscreenWorldRenderView::render(GraphicsContext* graphicsContext, RenderTargetTexture* renderTarget)
{
    LN_UNREACHABLE();
}

} // namespace ln

