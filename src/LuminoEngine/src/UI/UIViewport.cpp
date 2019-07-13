
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../ImageEffect/ImageEffectRenderer.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIViewport

UIViewport::UIViewport()
    : m_manager(detail::EngineDomain::uiManager())
{
}

UIViewport::~UIViewport()
{
}

void UIViewport::init()
{
	UIContainerElement::init();
	setHorizontalAlignment(HAlignment::Stretch);
	setVerticalAlignment(VAlignment::Stretch);

    m_imageEffectRenderer = makeRef<detail::ImageEffectRenderer>();
}

void UIViewport::onDispose(bool explicitDisposing)
{
    m_renderViews.clear();
	UIContainerElement::onDispose(explicitDisposing);
}

void UIViewport::addRenderView(RenderView* view)
{
    m_renderViews.add(view);
}

void UIViewport::removeRenderView(RenderView* view)
{
    m_renderViews.remove(view);
}

void UIViewport::addImageEffect(ImageEffect* effect)
{
    m_imageEffectRenderer->addImageEffect(effect);
}

void UIViewport::removeImageEffect(ImageEffect* effect)
{
    m_imageEffectRenderer->removeImageEffect(effect);
}

UIElement* UIViewport::lookupMouseHoverElement(const Point& globalPt)
{
    for (auto& view : m_renderViews) {
        UIElement* e = view->onLookupMouseHoverElement(globalPt);
        if (e) return e;
    }
    return UIContainerElement::lookupMouseHoverElement(globalPt);
}

void UIViewport::onUpdateFrame(float elapsedTimer)
{
	for (auto& view : m_renderViews) {
		view->updateFrame(elapsedTimer);
	}

    m_imageEffectRenderer->updateFrame(elapsedTimer);
}

void UIViewport::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
	for (auto& view : m_renderViews) {
		view->updateUIStyle(styleContext, finalStyle);
	}
}

void UIViewport::onUpdateLayout(const Rect& finalGlobalRect)
{
	for (auto& view : m_renderViews) {
		view->updateUILayout(finalGlobalRect);
	}
}

Size UIViewport::arrangeOverride(const Size& finalSize)
{
    // TODO: tmp
    for (auto& rv : m_renderViews) {
        rv->setActualPixelSize(finalSize);
    }
    m_actualViewboxSize = finalSize;
    return UIContainerElement::arrangeOverride(finalSize);
}

//void UIViewport::render(UIRenderingContext* context)
//{
//}

void UIViewport::onRender(UIRenderingContext* context)
{
    // TODO: ViewBoxTransform

    GraphicsContext* graphicsContext = context->m_frameWindowRenderingGraphicsContext;


    for (auto& view : m_renderViews)
    {
        view->render(graphicsContext);
    }

    m_imageEffectRenderer->render(context, graphicsContext->renderTarget(0));
}

void UIViewport::onRoutedEvent(UIEventArgs* e)
{
    for (auto& view : m_renderViews)
    {
        view->onRoutedEvent(e);
        if (e->handled) return;
    }

    UIContainerElement::onRoutedEvent(e);
}

} // namespace ln

