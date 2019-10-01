
#include "Internal.hpp"
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include "../Rendering/RenderStage.hpp"
#include "../Rendering/RenderingPipeline.hpp"

namespace ln {

//==============================================================================
// UIRenderView

UIRenderView::UIRenderView()
    : m_rootElement(nullptr)
{
}

void UIRenderView::init()
{
	RenderView::init();
	m_renderingContext = makeRef<UIRenderingContext>();
	m_sceneRenderingPipeline = makeRef<detail::FlatRenderingPipeline>();
	m_sceneRenderingPipeline->init();
	m_drawElementListCollector = makeRef<detail::DrawElementListCollector>();
    m_viewPoint = makeObject<RenderViewPoint>();

    m_adornerLayer = makeObject<UIAdornerLayer>();

	m_drawElementListCollector->addDrawElementList(/*RendringPhase::Default, */m_renderingContext->m_elementList);
	addDrawElementListManager(m_drawElementListCollector);
}

UIAdornerLayer* UIRenderView::adornerLayer() const
{
    return m_adornerLayer;
}

void UIRenderView::setRootElement(UIElement* element)
{
    if (m_rootElement) {
        m_rootElement->m_renderView = nullptr;
    }

    m_rootElement = element;

    if (m_rootElement) {
        m_rootElement->m_renderView = this;
    }
}

void UIRenderView::onUpdateFrame(float elapsedSeconds)
{
	m_rootElement->updateFrame(elapsedSeconds);
}

void UIRenderView::onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
	m_rootElement->updateStyleHierarchical(styleContext, finalStyle);
}

void UIRenderView::onUpdateUILayout(const Rect& finalGlobalRect)
{
	m_rootElement->updateLayout(Rect(0, 0, finalGlobalRect.getSize()));
    m_rootElement->updateFinalLayoutHierarchical(finalGlobalRect);
    m_adornerLayer->measureLayout(finalGlobalRect.getSize());
    m_adornerLayer->arrangeLayout(finalGlobalRect);
}

UIElement* UIRenderView::onLookupMouseHoverElement(const Point& frameClientPosition)
{
    return m_rootElement->lookupMouseHoverElement(frameClientPosition);
}

void UIRenderView::render(GraphicsContext* graphicsContext)
{
    if (m_rootElement)
    {



        FrameBuffer fb;
        fb.renderTarget[0] = graphicsContext->renderPass()->renderTarget(0);
        fb.depthBuffer = graphicsContext->renderPass()->depthBuffer();

        // TODO:
        detail::CameraInfo camera;
        {
            m_viewPoint->viewPixelSize = camera.viewPixelSize = Size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());	// TODO: 必要？
            m_viewPoint->viewPosition = camera.viewPosition = Vector3::Zero;
            m_viewPoint->viewDirection = camera.viewDirection = Vector3::UnitZ;
            m_viewPoint->viewMatrix = camera.viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);//Matrix();// 
            m_viewPoint->projMatrix = camera.projMatrix = Matrix::makePerspective2DLH(camera.viewPixelSize.width, camera.viewPixelSize.height, 0, 1000);
            m_viewPoint->viewProjMatrix = camera.viewProjMatrix = camera.viewMatrix * camera.projMatrix;
            m_viewPoint->viewFrustum = camera.viewFrustum = ViewFrustum(camera.viewProjMatrix);
            m_viewPoint->nearClip = camera.nearClip = 0;
            m_viewPoint->farClip = camera.farClip = 1000;
        }


        // build draw elements
        {
            m_renderingContext->resetForBeginRendering();
            m_renderingContext->setRenderTarget(0, fb.renderTarget[0]);
            m_renderingContext->setDepthBuffer(fb.depthBuffer);
            m_renderingContext->setViewPoint(m_viewPoint);
            m_renderingContext->m_frameWindowRenderingGraphicsContext = graphicsContext;
            if (clearMode() == RenderViewClearMode::ColorAndDepth) {
                m_renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
            }

            m_rootElement->render(m_renderingContext);


            m_renderingContext->m_frameWindowRenderingGraphicsContext = nullptr;

            m_adornerLayer->render(m_renderingContext);
        }





        m_sceneRenderingPipeline->render(graphicsContext, fb, &camera, &elementListManagers());
    }
}

} // namespace ln

