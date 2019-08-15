
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/UIPopup.hpp>

namespace ln {
    
//==============================================================================
// UIPopup

UIPopup::UIPopup()
    : m_placementTarget(nullptr)
    , m_placementMode(PlacementMode::Bottom)
    , m_adorner(nullptr)
    , m_opend(false)
{
}

void UIPopup::init()
{
	UIElement::init();
    specialElementFlags().set(detail::UISpecialElementFlags::Popup);

    // UIAdorner で左上を PlacementTarget と合わせてもらう
    setHorizontalAlignment(HAlignment::Left);
    setVerticalAlignment(VAlignment::Top);
}

void UIPopup::setPlacementTarget(UIElement* target)
{
    m_placementTarget = target;
}

void UIPopup::setPlacementMode(PlacementMode mode)
{
    m_placementMode = mode;
}

void UIPopup::open()
{
    if (!m_placementTarget) return;

    if (!m_opend)
    {
        if (!m_adorner) {
            m_adorner = makeObject<UIPopupAdorner>(m_placementTarget, this);
        }

        UIRenderView* renderView = getRenderView();
        if (renderView) {
            renderView->adornerLayer()->add(m_adorner);
        }
        m_opend = true;
    }
}

void UIPopup::close()
{
    if (m_opend)
    {
        if (m_adorner)
        {
            UIRenderView* renderView = getRenderView();
            if (renderView) {
                renderView->adornerLayer()->remove(m_adorner);
            }
            m_adorner = nullptr;
        }
        m_opend = false;
    }
}

//==============================================================================
// UIPopupAdorner

UIPopupAdorner::UIPopupAdorner()
{
}

void UIPopupAdorner::init(UIElement* adornedElement, UIPopup* popup)
{
    UIAdorner::init(adornedElement);
    m_popup = popup;
    addVisualChild(m_popup);
}

Size UIPopupAdorner::measureOverride(const Size& constraint)
{
    m_popup->measureLayout(constraint);
    return Size::max(m_popup->desiredSize(), UIElement::measureOverride(constraint));
}

Size UIPopupAdorner::arrangeOverride(const Size& finalSize)
{
    UIElement::arrangeOverride(finalSize);

    // TODO: 簡易 bottom
    m_popup->arrangeLayout(Rect(0, finalSize.height, finalSize));
    return finalSize;
}

void UIPopupAdorner::onUpdateLayout(const Rect& finalGlobalRect)
{
    m_popup->updateFinalLayoutHierarchical(finalGlobalRect);
}

void UIPopupAdorner::render(UIRenderingContext* context)
{
    if (m_popup) {
        m_popup->render(context);
    }
}

} // namespace ln

