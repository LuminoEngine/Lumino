
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIElement.hpp>
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/Controls/UIDialog.hpp>
#include <LuminoEngine/UI/UIFocusNavigator.hpp>
#include "../../Graphics/src/Rendering/CommandListServer.hpp"
#include "../../Graphics/src/Rendering/RenderStage.hpp"
#include "../../Graphics/src/Rendering/RenderElement.hpp"
#include <LuminoGraphics/Rendering/RenderingPipeline/FlatRenderingPipeline.hpp>

namespace ln {

UIFrameRenderView::UIFrameRenderView()
    : m_ownerViewport(nullptr)
    , m_rootElement(nullptr)
{
}

void UIFrameRenderView::init()
{
    m_renderingContext = makeObject_deprecated<UIRenderingContext>();

    RoutingRenderView::init(m_renderingContext);

    setRenderingPipeline(makeObject_deprecated<FlatRenderingPipeline>());

    //m_clearRenderPass = makeObject_deprecated<RenderPass>();

    m_adornerLayer = makeObject_deprecated<UIAdornerLayer>(this);

    //setClearMode(SceneClearMode::ColorAndDepth);
}

void UIFrameRenderView::setRootElement(UIDomainProvidor* element)
{
    if (m_rootElement) {
        m_rootElement->m_parentRenderView = nullptr;
        m_rootElement->m_renderView = nullptr;
    }

    m_rootElement = element;

    if (m_rootElement) {
        m_rootElement->m_parentRenderView = this;
        m_rootElement->m_renderView = this;
    }
}

UIDomainProvidor* UIFrameRenderView::rootElement() const
{
    return m_rootElement;
}

UIAdornerLayer* UIFrameRenderView::adornerLayer() const
{
    return m_adornerLayer;
}

void UIFrameRenderView::render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) {
    renderPipeline(graphicsContext, renderTarget);
}

void UIFrameRenderView::onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) {
    viewPoint->viewPosition = Vector3::Zero;
    viewPoint->viewDirection = Vector3::UnitZ;
#ifdef LN_COORD_RH
    viewPoint->viewMatrix = Matrix::makeLookAtRH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    viewPoint->projMatrix = Matrix::makePerspective2DRH(viewPoint->viewPixelSize.width, viewPoint->viewPixelSize.height, 0, 1000);
#else
    m_viewPoint->viewMatrix = Matrix::makeLookAtLH(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    m_viewPoint->projMatrix = Matrix::makePerspective2DLH(m_viewPoint->viewPixelSize.width, m_viewPoint->viewPixelSize.height, 0, 1000);
#endif
    viewPoint->viewProjMatrix = viewPoint->viewMatrix * viewPoint->projMatrix;
    viewPoint->viewFrustum = ViewFrustum(viewPoint->viewProjMatrix);
    viewPoint->fovY = 1.0f;
    viewPoint->nearClip = 0.0f;
    viewPoint->farClip = 1000.0f;
    viewPoint->dpiScale = 1.0f; // TODO: dpiscale

    // detail::CameraInfo camera;
    // camera.viewPixelSize = m_viewPoint->viewPixelSize;
    // camera.viewPosition = m_viewPoint->viewPosition;
    // camera.viewDirection = m_viewPoint->viewDirection;
    // camera.viewMatrix = m_viewPoint->viewMatrix;
    // camera.projMatrix = m_viewPoint->projMatrix;
    // camera.viewProjMatrix = m_viewPoint->viewProjMatrix;
    // camera.viewFrustum = m_viewPoint->viewFrustum;
    // camera.nearClip = m_viewPoint->nearClip;
    // camera.farClip = m_viewPoint->farClip;

    // auto p1 = Vector3::transformCoord(Vector3(0, 0, 0), m_viewPoint->viewMatrix);
    // auto p2 = Vector3::transformCoord(Vector3(0, 100, 0), m_viewPoint->viewMatrix);
    // auto p3 = Vector3::transformCoord(Vector3(100, 0, 0), m_viewPoint->viewMatrix);

    // auto p11 = Vector3::transformCoord(Vector3(0, 0, 0), m_viewPoint->viewProjMatrix);
    // auto p22 = Vector3::transformCoord(Vector3(0, 100, 0), m_viewPoint->viewProjMatrix);
    // auto p33 = Vector3::transformCoord(Vector3(100, 0, 0), m_viewPoint->viewProjMatrix);

    // camera.makePerspective(
    //     Vector3::Zero, Vector3::UnitZ,
    //     Math::PI / 4, m_viewPoint->viewPixelSize,
    //     0.0f, 1000.0f);
}

void UIFrameRenderView::onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) {

    if (m_rootElement)
    {


        

        // build draw elements
        {
            m_renderingContext->m_frameWindowRenderingGraphicsContext = graphicsContext;
            m_renderingContext->baseRenderView = this;
            m_renderingContext->clearPostEffects();

            //if (clearMode() == SceneClearMode::ColorAndDepth) {
            //    m_renderingContext->clear(ClearFlags::All, backgroundColor(), 1.0f, 0x00);
            //}
            //else {
            //    m_renderingContext->clear(ClearFlags::Depth, backgroundColor(), 1.0f, 0x00);
            //}

            m_rootElement->render(m_renderingContext, Matrix());


            m_renderingContext->m_frameWindowRenderingGraphicsContext = nullptr;

            m_adornerLayer->render(m_renderingContext, Matrix());



            //if (m_dialog) {
            //    m_dialog->render(m_renderingContext);
            //}
        }
    }
}

void UIFrameRenderView::invalidate(detail::UIElementDirtyFlags flags)
{
    // TODO: 今は AdonerLayer のを FrameWindow に伝えるために使用しているが、RenderView をルートとするべきかもしれない
    rootElement()->invalidate(flags, false);
}

//==============================================================================
// UIRenderView

UIRenderView::UIRenderView()
{
}

void UIRenderView::init()
{
    UIFrameRenderView::init();
}

//void UIRenderView::setDialog(UIDialog* dialog)
//{
//    m_dialog = dialog;
//    m_rootElement->invalidateStyle();  // TODO: 仮。とりあえずの動作テスト用
//}

void UIRenderView::onUpdateFrame(float elapsedSeconds)
{
    rootElement()->updateFrame(elapsedSeconds);
    //if (m_dialog) { // TODO: このあたりは VisualTree に任せたい
    //    m_dialog->updateFrame(elapsedSeconds);
    //}
}

void UIRenderView::onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    rootElement()->updateStyleHierarchical(styleContext, finalStyle);
	adornerLayer()->updateStyleHierarchical(styleContext, finalStyle);
    //if (m_dialog) { // TODO: このあたりは VisualTree に任せたい
    //    m_dialog->updateStyleHierarchical(styleContext, finalStyle);
    //}
}

void UIRenderView::onUpdateUILayout(UILayoutContext* layoutContext)
{
    Rect finalGlobalRect(0, 0, actualSize());
    rootElement()->updateLayout(layoutContext, finalGlobalRect);
    rootElement()->updateFinalLayoutHierarchical(layoutContext, Matrix());
    adornerLayer()->measureLayout(layoutContext, finalGlobalRect.getSize());
    adornerLayer()->arrangeLayout(layoutContext, finalGlobalRect);

    //if (m_dialog) {
    //    m_dialog->updateLayout(Rect(0, 0, finalGlobalRect.getSize()));
    //    m_dialog->updateFinalLayoutHierarchical(finalGlobalRect);
    //}
}

UIElement* UIRenderView::onLookupMouseHoverElement(const Point& frameClientPosition)
{


	UIElement* element = adornerLayer()->lookupMouseHoverElement(frameClientPosition);
	if (element) {
		return element;
	}

    //if (m_dialog) {
    //    return m_dialog->lookupMouseHoverElement(frameClientPosition);
    //}
    //else {
        return rootElement()->lookupMouseHoverElement(frameClientPosition);
    //}
}

//==============================================================================
// UIDomainProvidor

UIDomainProvidor::UIDomainProvidor()
    : m_parentRenderView(nullptr)
{
}

bool UIDomainProvidor::init()
{
    if (!UIContainerElement::init()) return false;
    return true;
}

void UIDomainProvidor::setupNavigator()
{
    if (!m_focusNavigator) {
        m_focusNavigator = makeObject_deprecated<UIFocusNavigator>();
    }
}

void UIDomainProvidor::onRoutedEvent(UIEventArgs* e)
{
    if (LN_REQUIRE(!m_visualParent)) return;


    if (m_focusNavigator) {
        if (UIElement* element = m_focusNavigator->focusedElement()) {
            if (e->type() == UIEvents::ExecuteCommandEvent) {
                if (static_cast<UICommandEventArgs*>(e)->command() == UICommonInputCommands::cancel()) {
                    m_focusNavigator->popFocus();
                    e->handled = true;
                    return;
                }
            }
        }
    }

    UIContainerElement::onRoutedEvent(e);

    if (m_parentRenderView && m_parentRenderView->m_ownerViewport) {
        m_parentRenderView->m_ownerViewport->raiseEvent(e);
    }
}

UIElement* UIDomainProvidor::lookupMouseHoverElement(const Point& frameClientPosition)
{
    if (m_focusNavigator) {
        if (UIElement* element = m_focusNavigator->focusedElement()) {
            return element->lookupMouseHoverElement(frameClientPosition);
        }
    }

    return UIContainerElement::lookupMouseHoverElement(frameClientPosition);
}

} // namespace ln

