
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/UIDialog.hpp>

namespace ln {
    
//==============================================================================
// UIDialog

UIDialog::UIDialog()
    : m_adorner(nullptr)
    , m_opend(false)
{
}

void UIDialog::init()
{
	UIElement::init();
    specialElementFlags().set(detail::UISpecialElementFlags::Popup);

    // UIAdorner で左上を PlacementTarget と合わせてもらう
    setHorizontalAlignment(HAlignment::Left);
    setVerticalAlignment(VAlignment::Top);
}

void UIDialog::open()
{
    if (!m_opend)
    {
        if (!m_adorner) {
            m_adorner = makeObject<UIDialogAdorner>(this);
        }

        UIFrameRenderView* renderView = getRenderView();
        if (renderView) {
            renderView->adornerLayer()->add(m_adorner);
        }
        m_opend = true;
    }
}

void UIDialog::close()
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
// UIDialogAdorner

UIDialogAdorner::UIDialogAdorner()
{
}

void UIDialogAdorner::init(UIDialog* popup)
{
    UIAdorner::init(nullptr);
	setBackgroundColor(Color(0, 0, 0, 0.5f));
    m_popup = popup;
    //addVisualChild(m_popup);
}

void UIDialogAdorner::onRoutedEvent(UIEventArgs* e)
{
	if (e->type() == UIEvents::MouseDownEvent) {
		m_popup->close();
		e->handled = true;
		return;
	}

	return UIAdorner::onRoutedEvent(e);
}

Size UIDialogAdorner::measureOverride(const Size& constraint)
{
    m_popup->measureLayout(constraint);
    return Size::max(m_popup->desiredSize(), UIElement::measureOverride(constraint));
}

Size UIDialogAdorner::arrangeOverride(const Size& finalSize)
{
    UIElement::arrangeOverride(finalSize);

	auto desiredSize = m_popup->desiredSize();
	Rect rect;
	rect.x = (finalSize.width - desiredSize.width) / 2;
	rect.y = (finalSize.height - desiredSize.height) / 2;
	rect.width = desiredSize.width;
	rect.height = desiredSize.height;
	m_popup->arrangeLayout(rect);

    return finalSize;
}

void UIDialogAdorner::onUpdateLayout(const Rect& finalGlobalRect)
{
    m_popup->updateFinalLayoutHierarchical(finalGlobalRect);
}

UIElement* UIDialogAdorner::lookupMouseHoverElement(const Point& frameClientPosition)
{
	// popup は UIAdorner の VisualChildren ではないため、直接呼び出す必要がある
	if (m_popup) {
		UIElement* element = m_popup->lookupMouseHoverElement(frameClientPosition);
		if (element) {
			return element;
		}
	}

	UIAdorner::lookupMouseHoverElement(frameClientPosition);
}

void UIDialogAdorner::render(UIRenderingContext* context)
{
	UIAdorner::render(context);

	// popup は UIAdorner の VisualChildren ではないため、直接呼び出す必要がある
    if (m_popup) {
        m_popup->render(context);
    }
}

} // namespace ln

