
#include "Internal.hpp"
#include <LuminoEngine/UI/UIRenderView.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIAdorner.hpp>
#include <LuminoEngine/UI/Controls/UIDialog.hpp>

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
    setHAlignment(UIHAlignment::Left);
    setVAlignment(UIVAlignment::Top);
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

void UIDialog::setupDialogButtons(UIDialogButtons buttons)
{
	m_dialogButtons.clear();
	switch (buttons)
	{
	case UIDialogButtons::OK:
		addDialogButton(UIDialogButtonRole::Accept, _TT("OK"));
		break;
	case UIDialogButtons::OKCancel:
		addDialogButton(UIDialogButtonRole::Accept, _TT("OK"));
		addDialogButton(UIDialogButtonRole::Reject, _TT("Cancel"));
		break;
	case UIDialogButtons::YesNo:
		addDialogButton(UIDialogButtonRole::Accept, _TT("Yes"));
		addDialogButton(UIDialogButtonRole::Discard, _TT("No"));
		break;
	case UIDialogButtons::YesNoCancel:
		addDialogButton(UIDialogButtonRole::Accept, _TT("Yes"));
		addDialogButton(UIDialogButtonRole::Discard, _TT("No"));
		addDialogButton(UIDialogButtonRole::Reject, _TT("Cancel"));
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

void UIDialog::addDialogButton(UIDialogButtonRole role, const String& text)
{
	if (!m_dialogButtonsLayout) {
		m_dialogButtonsLayout = ln::makeObject<UIBoxLayout>();
		m_dialogButtonsLayout->setOrientation(UILayoutOrientation::Horizontal);
		addVisualChild(m_dialogButtonsLayout);
	}

	auto button = ln::makeObject<ln::UIButton>();
	button->setText(text);
	button->connectOnClicked(bind(this, &UIDialog::handleDialogButtonClicked));
	m_dialogButtonsLayout->addChild(button);
}

void UIDialog::handleDialogButtonClicked()
{
	close();
}

Size UIDialog::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	Size baseSize = UIContainerElement::measureOverride(layoutContext, constraint);


	if (m_dialogButtonsLayout) {
		m_dialogButtonsLayout->measureLayout(layoutContext, constraint);
		Size buttonsSize = m_dialogButtonsLayout->desiredSize();

		return baseSize;
		//return Size(
		//	std::max(baseSize.width, buttonsSize.height),
		//	baseSize.height + buttonsSize.height);
	}
	else {
		return baseSize;
	}
}

Size UIDialog::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();
	Size baseSize = UIContainerElement::arrangeOverride(layoutContext, finalArea);

	if (m_dialogButtonsLayout) {
		Size buttonsSize = m_dialogButtonsLayout->desiredSize();
		Rect buttonsRect(
			std::max(0.0f, finalSize.width - buttonsSize.width),
			std::max(0.0f, finalSize.height - buttonsSize.height),
			std::min(buttonsSize.width, finalSize.width),
			std::min(buttonsSize.height, finalSize.height));
		m_dialogButtonsLayout->arrangeLayout(layoutContext, buttonsRect);
	}

	return baseSize;
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

Size UIDialogAdorner::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
    m_popup->measureLayout(layoutContext, constraint);
    return Size::max(m_popup->desiredSize(), UIElement::measureOverride(layoutContext, constraint));
}

Size UIDialogAdorner::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();
    UIElement::arrangeOverride(layoutContext, finalArea);

	auto desiredSize = m_popup->desiredSize();
	Rect rect;
	rect.x = (finalSize.width - desiredSize.width) / 2;
	rect.y = (finalSize.height - desiredSize.height) / 2;
	rect.width = desiredSize.width;
	rect.height = desiredSize.height;
	m_popup->arrangeLayout(layoutContext, rect);

    return finalSize;
}

void UIDialogAdorner::onUpdateLayout(UILayoutContext* layoutContext)
{
    m_popup->updateFinalLayoutHierarchical(layoutContext, m_combinedFinalRenderTransform);
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

	return UIAdorner::lookupMouseHoverElement(frameClientPosition);
}

void UIDialogAdorner::render(UIRenderingContext* context, const Matrix& parentTransform)
{
	UIAdorner::render(context, parentTransform);

	// popup は UIAdorner の VisualChildren ではないため、直接呼び出す必要がある
    if (m_popup) {
        m_popup->render(context, parentTransform);
    }
}

} // namespace ln

