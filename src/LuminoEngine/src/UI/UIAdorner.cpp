
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>

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

void UIAdornerLayer::init()
{
    Object::init();
}

void UIAdornerLayer::add(UIAdorner* adorner)
{
    if (LN_REQUIRE(adorner)) return;
	m_adorners.add(adorner);
}

void UIAdornerLayer::remove(UIAdorner* adorner)
{
	m_adorners.remove(adorner);
}

void UIAdornerLayer::updateStyleHierarchical(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle)
{
    for (auto& adorner : m_adorners) {
        adorner->updateStyleHierarchical(styleContext, parentFinalStyle);
    }
}

void UIAdornerLayer::measureLayout(const Size& availableSize)
{
    for (auto& adorner : m_adorners) {
        adorner->measureLayout(availableSize);
    }
}

void UIAdornerLayer::arrangeLayout(const Rect& localSlotRect)
{
    for (auto& adorner : m_adorners) {
        auto adornedRect = adorner->adornedElement()->m_finalGlobalRect;

        adorner->arrangeLayout(Rect(0, 0, adornedRect.width, adornedRect.height));
        // TODO: 以下、adorner の中にまとめたほうがいい気がする。というか、measure->arrange->updateFinal 一連をひとつのメソッドでまとめていいと思う。adornder 同士は相互作用しないし
        //adorner->updateFinalRects(adorner->adornedElement()->m_finalGlobalRect);
        //adorner->content()->updateFinalLayoutHierarchical(adorner->adornedElement()->m_finalGlobalRect);
        adorner->updateFinalLayoutHierarchical(adornedRect);
    }
}

void UIAdornerLayer::render(UIRenderingContext* context)
{
    context->m_adornerRendering = true;
    for (auto& adorner : m_adorners) {
        adorner->render(context);
    }
    context->m_adornerRendering = false;
}

} // namespace ln

