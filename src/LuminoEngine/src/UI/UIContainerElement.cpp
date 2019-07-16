
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
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
	//setHorizontalAlignment(HAlignment::Stretch);
	//setVerticalAlignment(VAlignment::Stretch);
}

void UIContainerElement::onDispose(bool explicitDisposing)
{
    removeAllChildren();
	UIElement::onDispose(explicitDisposing);
}

void UIContainerElement::addElement(UIElement* element)
{
	if (LN_REQUIRE(element)) return;

    // 通常は作成直後、デフォルトの Container に追加されている。
    // WPF のように別の親に追加済みであれば例外するのもありだけど、いちいち removeFromParent するのは面倒。
    if (element->logicalParent()) {
        element->removeFromLogicalParent();
    }
	
    m_logicalChildren.add(element);
    element->setLogicalParent(this);

	if (m_logicalChildrenHost) {
		m_logicalChildrenHost->addLayoutOwnerLogicalChild(element);
	}
}

void UIContainerElement::removeElement(UIElement* element)
{
    if (LN_REQUIRE(element)) return;
    if (LN_REQUIRE(element->logicalParent() == this)) return;

    if (m_logicalChildrenHost) {
        m_logicalChildrenHost->removeLayoutOwnerLogicalChild(element);
    }

    m_logicalChildren.remove(element);
    element->setLogicalParent(nullptr);
}

void UIContainerElement::removeAllChildren()
{
	m_logicalChildren.clear();
	if (m_logicalChildrenHost) {
		m_logicalChildrenHost->clearLayoutOwnerLogicalChildren();
	}
}

void UIContainerElement::setLayoutPanel(UILayoutPanel* panel)
{
	setLogicalChildrenHost(panel);
}

UILayoutPanel* UIContainerElement::layoutPanel() const
{
	return m_logicalChildrenHost;
}

int UIContainerElement::getVisualChildrenCount() const
{
	if (m_logicalChildrenHost) {
		return 1;
	}
	else {
		return m_logicalChildren.size();
	}
}

UIElement* UIContainerElement::getVisualChild(int index) const
{
	if (m_logicalChildrenHost) {
		return m_logicalChildrenHost;
	}
	else {
		return m_logicalChildren[index];
	}
}

Size UIContainerElement::measureOverride(const Size& constraint)
{
	if (m_logicalChildrenHost) {
        m_logicalChildrenHost->measureLayout(constraint);
        Size layoutSize = m_logicalChildrenHost->desiredSize();
        Size localSize = UIElement::measureOverride(constraint);
        return Size::max(layoutSize, localSize);
	}
	else {
		return UIFrameLayout::staticMeasureOverride(this, constraint);
	}
}

Size UIContainerElement::arrangeOverride(const Size& finalSize)
{
    Rect contentSlotRect(0, 0, finalSize);
    //detail::LayoutHelper::adjustAlignment(finalSize, desiredSize(), HAlignment::Center, VAlignment::Center, &contentSlotRect);

	if (m_logicalChildrenHost) {
        m_logicalChildrenHost->arrangeLayout(contentSlotRect.makeDeflate(finalStyle()->padding));
		return finalSize;
	}
	else {
		return UIFrameLayout::staticArrangeOverride(this, contentSlotRect);
	}
}

void UIContainerElement::setLogicalChildrenHost(UILayoutPanel* panel)
{
	if (panel != m_logicalChildrenHost)
	{
		// 既に持っていれば取り除いておく
		if (m_logicalChildrenHost)
		{
			m_logicalChildrenHost->clearLayoutOwnerLogicalChildren();
			//removeVisualChild(m_logicalChildrenPresenter);
			m_logicalChildrenHost = nullptr;
		}

		// 新しく保持する
		if (panel)
		{
			//if (readCoreFlag(detail::UICoreFlags_LogicalChildrenPresenterAutoManagement))
			//{
			//	addVisualChild(presenter);
			//}

			m_logicalChildrenHost = panel;

			for (auto& element : m_logicalChildren) {
				m_logicalChildrenHost->addLayoutOwnerLogicalChild(element);
			}
		}

		// 変更通知
		//onLogicalChildrenPresenterChanged(m_logicalChildrenPresenter);

		//m_invalidateItemsHostPanel = true;
	}
}


//==============================================================================
// UIFrame

UIFrame::UIFrame()
{
}

void UIFrame::init()
{
    UIContainerElement::init();
}

} // namespace ln

