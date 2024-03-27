
#include "../Internal.hpp"
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoFramework/Scene/World.hpp>
#include <LuminoFramework/Scene/Scene.hpp>
#include <LuminoFramework/Scene/Reflection/OffscreenWorldRenderView.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderingPipeline/StandardRenderingPipeline.hpp>
#include "../SceneManager.hpp"

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
    RenderView::init(nullptr);
    m_renderingManager = detail::RenderingManager::instance();

    // TODO: SceneRenderingPipeline は WorldRenderView が持っているものとほとんど同じなので、
    // こっちでインスタンスを new するのは無駄が多い。共有したいところ。
    // ただ、GBuffe のように Viewサイズや視点に依存するデータを独立させる必要がある。
    setRenderingPipeline(makeObject_deprecated<SceneRenderingPipeline>());

    m_viewPoint = makeObject_deprecated<RenderViewPoint>();
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
//    GraphicsCommandList* context = m_renderingManager->graphicsManager()->graphicsContext();
//    context->setRenderTarget(0, m_renderTarget);
//    context->setDepthBuffer(depthBuffer);
//    WorldRenderView::render(context);
//
//    // TODO: scoped
//	DepthBuffer::releaseTemporary(depthBuffer);
//}
//

void OffscreenWorldRenderView::render(GraphicsCommandList* graphicsContext, World* targetWorld) {
    m_targetWorld = targetWorld;
    auto* context = getContext();
    setClearMode(context->baseRenderView->clearMode());
    setBackgroundColor(context->baseRenderView->backgroundColor());
    RenderView::render(graphicsContext, m_renderTarget);


    //LN_NOTIMPLEMENTED();
    //renderingPipeline()->prepare(this, m_renderTarget);


     //renderingPipeline()->render(
     //   graphicsContext, targetWorld->m_renderingContext,
     //   m_renderTarget, this,
     //   &targetWorld->m_combinedSceneGlobalRenderParams);

}

void OffscreenWorldRenderView::render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) {
    LN_UNREACHABLE();
}

void OffscreenWorldRenderView::onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) {
    LN_UNREACHABLE();
}

void OffscreenWorldRenderView::onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) {
    m_targetWorld->renderObjects(renderingContext);
}

} // namespace ln

