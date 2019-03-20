
#include "Internal.hpp"
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>

namespace ln {

//==============================================================================
// UIContainerElement

UIContainerElement::UIContainerElement()
{
}

void UIContainerElement::init()
{
    UIElement::init();
	setHorizontalAlignment(HAlignment::Stretch);
	setVerticalAlignment(VAlignment::Stretch);
}

void UIContainerElement::onDispose(bool explicitDisposing)
{
    removeAllChildren();
	UIElement::onDispose(explicitDisposing);
}

void UIContainerElement::addElement(UIElement* element)
{
	if (LN_REQUIRE(element)) return;
	m_logicalChildren.add(element);
}

void UIContainerElement::removeAllChildren()
{
	m_logicalChildren.clear();
}

void UIContainerElement::setLayoutPanel(UILayoutPanel* panel)
{
	LN_NOTIMPLEMENTED();
	m_logicalChildrenHostPanel = panel;
	// TODO: panel 交換
}

UILayoutPanel* UIContainerElement::layoutPanel() const
{
	return m_logicalChildrenHostPanel;
}

int UIContainerElement::getVisualChildrenCount() const
{
	return m_logicalChildren.size();
}

UIElement* UIContainerElement::getVisualChild(int index) const
{
	return m_logicalChildren[index];
}

Size UIContainerElement::measureOverride(const Size& constraint)
{
	if (m_logicalChildrenHostPanel) {
		return m_logicalChildrenHostPanel->measureOverride(constraint);
	}
	else {
		return UIFrameLayout::staticMeasureOverride(this, constraint);
	}
}

Size UIContainerElement::arrangeOverride(const Size& finalSize)
{
	if (m_logicalChildrenHostPanel) {
		return m_logicalChildrenHostPanel->arrangeOverride(finalSize);
	}
	else {
		return UIFrameLayout::staticArrangeOverride(this, finalSize);
	}
}

} // namespace ln

