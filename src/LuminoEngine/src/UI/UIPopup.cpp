
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
            m_adorner = makeObject<UIPopupAdorner>(m_placementTarget, this);
        }

        UIFrameRenderView* renderView = getRenderView();
        if (renderView) {
            renderView->adornerLayer()->add(m_adorner);
        }

		activate();
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
    //addVisualChild(m_popup);
}

Size UIPopupAdorner::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    m_popup->measureLayout(layoutContext, constraint);

    return Size::max(m_popup->placementTarget()->desiredSize(), UIElement::measureOverride(layoutContext, constraint));
}

Size UIPopupAdorner::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
    UIElement::arrangeOverride(layoutContext, finalSize);

	switch (m_popup->placementMode())
	{
		case PlacementMode::Bottom:
			// TODO: 簡易 bottom
			m_popup->arrangeLayout(layoutContext, Rect(0, finalSize.height, finalSize));
			break;

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

void UIPopupAdorner::render(UIRenderingContext* context)
{
    if (m_popup) {
        m_popup->render(context);
    }
}

} // namespace ln

