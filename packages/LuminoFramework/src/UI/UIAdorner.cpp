#include "Internal.hpp"
#include <LuminoFramework/UI/UIRenderingContext.hpp>
#include <LuminoFramework/UI/UIRenderView.hpp>
#include <LuminoFramework/UI/UIAdorner.hpp>

namespace ln {
    
//==============================================================================
// UIAdorner

UIAdorner::UIAdorner()
{
}

void UIAdorner::init(UIElement* adornedElement)
{
	UIElement::init();
    m_adornedElement = adornedElement;
}

UIElement* UIAdorner::adornedElement() const
{
    return m_adornedElement;
}

//==============================================================================
// UIAdornerLayer

UIAdornerLayer::UIAdornerLayer()
{
}

void UIAdornerLayer::init(UIFrameRenderView* owner)
{
    Object::init();
    m_owner = owner;
}

void UIAdornerLayer::add(UIAdorner* adorner)
{
    if (LN_REQUIRE(adorner)) return;
	m_adorners.add(adorner);
    m_owner->invalidate(detail::UIElementDirtyFlags::Layout);
}

void UIAdornerLayer::remove(UIAdorner* adorner)
{
    if (LN_REQUIRE(adorner)) return;
    m_adorners.remove(adorner);
    adorner->handleDetachFromUITree();
    m_owner->invalidate(detail::UIElementDirtyFlags::Layout);
}

void UIAdornerLayer::updateStyleHierarchical(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle)
{
    for (auto& adorner : m_adorners) {
        adorner->updateStyleHierarchical(styleContext, parentFinalStyle);
    }
}

void UIAdornerLayer::measureLayout(UILayoutContext* layoutContext, const Size& availableSize)
{
    for (auto& adorner : m_adorners) {
        adorner->measureLayout(layoutContext, availableSize);
    }
}

void UIAdornerLayer::arrangeLayout(UILayoutContext* layoutContext, const Rect& localSlotRect)
{
    for (auto& adorner : m_adorners) {
		Rect adornedRect = localSlotRect;
		if (adorner->adornedElement()) {
            adornedRect = Rect(adorner->adornedElement()->localPosition(), adorner->adornedElement()->actualSize());
            // = adorner->adornedElement()->m_finalGlobalRect;
		}

		//adornedRect = Rect(0, 0, 100, 100);

        //auto adornedRect = adorner->adornedElement()->m_finalGlobalRect;

        adorner->arrangeLayout(layoutContext, Rect(0, 0, adornedRect.width, adornedRect.height));
        // TODO: 以下、adorner の中にまとめたほうがいい気がする。というか、measure->arrange->updateFinal 一連をひとつのメソッドでまとめていいと思う。adornder 同士は相互作用しないし
        //adorner->updateFinalRects(adorner->adornedElement()->m_finalGlobalRect);
        //adorner->content()->updateFinalLayoutHierarchical(adorner->adornedElement()->m_finalGlobalRect);

        auto transform = Matrix::makeTranslation(adornedRect.x, adornedRect.y, 0);
        adorner->updateFinalLayoutHierarchical(layoutContext, transform);
    }
}

void UIAdornerLayer::render(UIRenderingContext* context, const Matrix& parentTransform)
{
    context->m_adornerRendering = true;
    for (auto& adorner : m_adorners) {

        //adorner->render(context, parentTransform * adorner->m_localTransform);
		adorner->render(context, adorner->m_combinedFinalRenderTransform);
    }
    context->m_adornerRendering = false;
}

UIElement* UIAdornerLayer::lookupMouseHoverElement(const Point& frameClientPosition)
{
	for (auto& adorner : m_adorners) {
		UIElement* element = adorner->lookupMouseHoverElement(frameClientPosition);
		if (element) {
			return element;
		}
	}
	return nullptr;
}

} // namespace ln

