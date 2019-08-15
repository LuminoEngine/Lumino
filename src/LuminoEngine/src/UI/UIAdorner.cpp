
#include "Internal.hpp"
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

void UIAdorner::setContent(UIElement* element)
{
    m_content = element;
}

UIElement* UIAdorner::content() const
{
    return m_content;
}

Size UIAdorner::measureOverride(const Size& constraint)
{
    m_content->measureLayout(constraint);
    return UIElement::measureOverride(constraint);
}

Size UIAdorner::arrangeOverride(const Size& finalSize)
{
    m_content->arrangeLayout(Rect(0, 0, finalSize));
    return UIElement::arrangeOverride(finalSize);
}

void UIAdorner::render(UIRenderingContext* context)
{
    if (m_content) {
        m_content->render(context);
    }
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

void UIAdornerLayer::measureLayout(const Size& availableSize)
{
    for (auto& adorner : m_adorners) {
        adorner->measureLayout(availableSize);
    }
}

void UIAdornerLayer::arrangeLayout(const Rect& localSlotRect)
{
    for (auto& adorner : m_adorners) {
        adorner->arrangeLayout(localSlotRect);
        // TODO: 以下、adorner の中にまとめたほうがいい気がする。というか、measure->arrange->updateFinal 一連をひとつのメソッドでまとめていいと思う。adornder 同士は相互作用しないし
        adorner->updateFinalRects(adorner->adornedElement()->m_finalGlobalRect);
        adorner->content()->updateFinalRects(adorner->adornedElement()->m_finalGlobalRect);
    }
}

void UIAdornerLayer::render(UIRenderingContext* context)
{
    for (auto& adorner : m_adorners) {
        adorner->render(context);
    }
}

} // namespace ln

