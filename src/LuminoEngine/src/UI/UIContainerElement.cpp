
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIContainerElement.hpp>
#include "UIManager.hpp"

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

    setLayoutPanel(makeObject<UIFrameLayout>());
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
	addVisualChild(element);

	//if (m_logicalChildrenHost) {
	//	m_logicalChildrenHost->addLayoutOwnerLogicalChild(element);
	//}
}

void UIContainerElement::removeElement(UIElement* element)
{
    if (LN_REQUIRE(element)) return;
    if (LN_REQUIRE(element->logicalParent() == this)) return;

    //if (m_logicalChildrenHost) {
    //    m_logicalChildrenHost->removeLayoutOwnerLogicalChild(element);
    //}

    m_logicalChildren.remove(element);
    element->setLogicalParent(nullptr);
	removeVisualChild(element);
}

void UIContainerElement::removeAllChildren()
{
	for (auto& e : m_logicalChildren) {
		removeVisualChild(e);
	}

	m_logicalChildren.clear();
	//if (m_logicalChildrenHost) {
	//	m_logicalChildrenHost->clearLayoutOwnerLogicalChildren();
	//}
}

void UIContainerElement::setLayoutPanel(UILayoutPanel* panel)
{
	if (m_layout != panel) {
		m_layout = panel;
		if (!m_layout) {
			m_layout = makeObject<UIFrameLayout>();
		}
		onLayoutPanelChanged(m_layout);
		//setLogicalChildrenHost(panel);
	}
}

UILayoutPanel* UIContainerElement::layoutPanel() const
{
	//if (m_layout)
		return m_layout;
	//else
	//	return m_manager->defaultLayout();
}
//
//UILayoutPanel* UIContainerElement::layoutPanel() const
//{
//    return m_layout;
//    //LN_UNREACHABLE();
//    //return nullptr;//m_logicalChildrenHost;
//}

//int UIContainerElement::getVisualChildrenCount() const
//{
//	//if (m_logicalChildrenHost) {
//	//	return 1;
//	//}
//	//else {
//		return m_logicalChildren.size();
//	//}
//}
//
//UIElement* UIContainerElement::getVisualChild(int index) const
//{
//	//if (m_logicalChildrenHost) {
//	//	return m_logicalChildrenHost;
//	//}
//	//else {
//		return m_logicalChildren[index];
//	//}
//}
//
Size UIContainerElement::measureOverride(const Size& constraint)
{
    struct ElementList : public IUIElementList {
        List<Ref<UIElement>>* list;
        virtual int getElementCount() const { return list->size(); }
        virtual UIElement* getElement(int i) const { return list->at(i); }
    } list;
    list.list = &m_logicalChildren;

	UILayoutPanel* layout = layoutPanel();
	layout->measureLayout(&list, constraint);
    Size desiredSize = layout->desiredSize();
    Size localSize = UIElement::measureOverride(constraint);
    return Size::max(desiredSize, localSize);

	//if (m_logicalChildrenHost) {
 //       m_logicalChildrenHost->measureLayout(constraint);
 //       Size layoutSize = m_logicalChildrenHost->desiredSize();
 //       Size localSize = UIElement::measureOverride(constraint);
 //       return Size::max(layoutSize, localSize);
	//}
	//else {
	//	return UIFrameLayout::staticMeasureOverride(this, constraint);
	//}
}

Size UIContainerElement::arrangeOverride(const Size& finalSize)
{
	UILayoutPanel* layout = layoutPanel();

    Rect contentSlotRect;
    detail::LayoutHelper::adjustAlignment(finalSize, layout->desiredSize(), m_finalStyle->horizontalContentAlignment, m_finalStyle->verticalContentAlignment, &contentSlotRect);

    contentSlotRect = contentSlotRect.makeDeflate(m_finalStyle->padding);
    

    struct ElementList : public IUIElementList {
        List<Ref<UIElement>>* list;
        virtual int getElementCount() const { return list->size(); }
        virtual UIElement* getElement(int i) const { return list->at(i); }
    } list;
    list.list = &m_logicalChildren;

	layout->arrangeLayout(&list, contentSlotRect);
    return finalSize;

 //   Rect contentSlotRect(0, 0, finalSize);
 //   //detail::LayoutHelper::adjustAlignment(finalSize, desiredSize(), HAlignment::Center, VAlignment::Center, &contentSlotRect);

	//if (m_logicalChildrenHost) {
 //       m_logicalChildrenHost->arrangeLayout(contentSlotRect.makeDeflate(finalStyle()->padding));
	//	return finalSize;
	//}
	//else {
	//	return UIFrameLayout::staticArrangeOverride(this, contentSlotRect);
	//}
}

void UIContainerElement::onLayoutPanelChanged(UILayoutPanel* newPanel)
{
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

