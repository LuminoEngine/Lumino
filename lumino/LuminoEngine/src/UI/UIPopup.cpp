
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
    setAlignments(UIAlignment::TopLeft);

	m_hitTestMode = detail::UIHitTestMode::InvisibleControl;
}

void UIPopup::setPlacementTarget(UIElement* target)
{
    m_placementTarget = target;
}

UIElement* UIPopup::placementTarget() const
{
	return m_placementTarget;
}

void UIPopup::setPlacementMode(PlacementMode mode)
{
    m_placementMode = mode;
}

void UIPopup::open()
{
    if (!m_opend)
    {
        if (!m_adorner) {
            m_adorner = makeObject_deprecated<UIPopupAdorner>(m_placementTarget, this);
        }

        UIFrameRenderView* renderView = getRenderView();
        if (renderView) {
            renderView->adornerLayer()->add(m_adorner);
        }

		focus();
		m_hitTestMode = detail::UIHitTestMode::Visible;
        m_opend = true;
    }
}

void UIPopup::close()
{
    if (m_opend)
    {
        if (m_adorner)
        {
            UIFrameRenderView* renderView = getRenderView();
            if (renderView) {
                renderView->adornerLayer()->remove(m_adorner);
            }
        }
		m_hitTestMode = detail::UIHitTestMode::Visible;
        m_opend = false;
    }
}

void UIPopup::onRoutedEvent(UIEventArgs* e)
{
    if (e->type() == UIEvents::LostFocusEvent) {
        close();
        e->handled = true;
        return;
    }
    //else {
    //    if (m_placementTarget && e->strategy == UIEventRoutingStrategy::Bubble) {
    //        m_placementTarget->raiseEvent(e, UIEventRoutingStrategy::Bubble);
    //        if (e->handled) return;
    //    }
    //}

    UIContainerElement::onRoutedEvent(e);
}

//bool UIPopup::onHitTest(const Point& frameClientPosition)
//{
//	if (!m_opend) return false;
//
//	UIContainerElement::onHitTest(frameClientPosition);
//}

//==============================================================================
// UIPopupAdorner

UIPopupAdorner::UIPopupAdorner()
{
}

void UIPopupAdorner::init(UIElement* adornedElement, UIPopup* popup)
{
    UIAdorner::init(adornedElement);
    m_popup = popup;
    //addVisualChild(m_popup);
}

Size UIPopupAdorner::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    m_popup->measureLayout(layoutContext, constraint);

    Size contentSize;
    if (m_popup->placementTarget()) {
        contentSize = m_popup->placementTarget()->desiredSize();
    }

    return Size::max(contentSize, UIElement::measureOverride(layoutContext, constraint));
}

Size UIPopupAdorner::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
    const auto finalSize = finalArea.getSize();
    UIElement::arrangeOverride(layoutContext, finalArea);

	switch (m_popup->placementMode())
	{
		case PlacementMode::Bottom:
        {
            // TODO: 簡易 bottom
            const auto& p1 = m_combinedFinalRenderTransform.position();
            const auto& p2 = m_popup->placementTarget()->m_combinedFinalRenderTransform.position();
            const auto& pos = p2 - p1;

            m_popup->arrangeLayout(layoutContext, Rect(pos.x, pos.y + finalSize.height, finalSize));
            break;
        }

		default:	// overray
		{
			auto desiredSize = m_popup->desiredSize();
			Rect rect;
			rect.x = (finalSize.width - desiredSize.width) / 2;
			rect.y = (finalSize.height - desiredSize.height) / 2;
			rect.width = desiredSize.width;
			rect.height = desiredSize.height;
			m_popup->arrangeLayout(layoutContext, rect);
			break;
		}
	}

    return finalSize;
}

void UIPopupAdorner::onUpdateLayout(UILayoutContext* layoutContext)
{
    m_popup->updateFinalLayoutHierarchical(layoutContext, m_combinedFinalRenderTransform);
}

void UIPopupAdorner::render(UIRenderingContext* context, const Matrix& parentTransform)
{
    if (m_popup) {
        m_popup->render(context, parentTransform);
    }
}

} // namespace ln

