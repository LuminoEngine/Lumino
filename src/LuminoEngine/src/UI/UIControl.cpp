
#include "Internal.hpp"
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIControl.hpp>
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIControl

UIControl::UIControl()
    : m_enabledDirectChildrenContentAlignment(true)
{
}

void UIControl::init()
{
    UIElement::init();
	//setHorizontalAlignment(HAlignment::Stretch);
	//setVerticalAlignment(VAlignment::Stretch);

    //setLayoutPanel(makeObject<UIFrameLayout>());
}

void UIControl::onDispose(bool explicitDisposing)
{
    removeAllChildren();
	UIElement::onDispose(explicitDisposing);
}

void UIControl::setHorizontalContentAlignment(HAlignment value)
{
    m_localStyle->mainStyle()->horizontalContentAlignment = value;
}

HAlignment UIControl::horizontalContentAlignment() const
{
    return m_localStyle->mainStyle()->horizontalContentAlignment;
}

void UIControl::setVerticalContentAlignment(VAlignment value)
{
    m_localStyle->mainStyle()->verticalContentAlignment = value;
}

VAlignment UIControl::verticalContentAlignment() const
{
    return m_localStyle->mainStyle()->verticalContentAlignment;
}

void UIControl::addElement(UIElement* element)
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

void UIControl::removeElement(UIElement* element)
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

void UIControl::removeAllChildren()
{
	for (auto& e : m_logicalChildren) {
		removeVisualChild(e);
	}

	m_logicalChildren.clear();
	//if (m_logicalChildrenHost) {
	//	m_logicalChildrenHost->clearLayoutOwnerLogicalChildren();
	//}
}

//void UIControl::setLayoutPanel(UILayoutPanel* panel)
//{
//	if (m_layout != panel) {
//		m_layout = panel;
//		if (!m_layout) {
//			m_layout = makeObject<UIFrameLayout>();
//		}
//		onLayoutPanelChanged(m_layout);
//		//setLogicalChildrenHost(panel);
//	}
//}
//
//UILayoutPanel* UIControl::layoutPanel() const
//{
//	//if (m_layout)
//		return m_layout;
//	//else
//	//	return m_manager->defaultLayout();
//}
//
//UILayoutPanel* UIControl::layoutPanel() const
//{
//    return m_layout;
//    //LN_UNREACHABLE();
//    //return nullptr;//m_logicalChildrenHost;
//}

//int UIControl::getVisualChildrenCount() const
//{
//	//if (m_logicalChildrenHost) {
//	//	return 1;
//	//}
//	//else {
//		return m_logicalChildren.size();
//	//}
//}
//
//UIElement* UIControl::getVisualChild(int index) const
//{
//	//if (m_logicalChildrenHost) {
//	//	return m_logicalChildrenHost;
//	//}
//	//else {
//		return m_logicalChildren[index];
//	//}
//}
//
Size UIControl::measureOverride(const Size& constraint)
{
 //   struct ElementList : public IUIElementList {
 //       List<Ref<UIElement>>* list;
 //       virtual int getElementCount() const { return list->size(); }
 //       virtual UIElement* getElement(int i) const { return list->at(i); }
 //   } list;
 //   list.list = &m_logicalChildren;

	//UILayoutPanel* layout = layoutPanel();
	//layout->measureLayout(&list, constraint);
 //   Size desiredSize = layout->desiredSize();
 //   Size localSize = UIElement::measureOverride(constraint);
 //   return Size::max(desiredSize, localSize);

	//if (m_logicalChildrenHost) {
 //       m_logicalChildrenHost->measureLayout(constraint);
 //       Size layoutSize = m_logicalChildrenHost->desiredSize();
 //       Size localSize = UIElement::measureOverride(constraint);
 //       return Size::max(layoutSize, localSize);
	//}
	//else {
    return UIFrameLayout2::staticMeasureOverride(this, constraint);

	//}
}

Size UIControl::arrangeOverride(const Size& finalSize)
{
	//UILayoutPanel* layout = layoutPanel();

 //   Rect contentSlotRect;
 //   if (m_enabledDirectChildrenContentAlignment) {
 //       detail::LayoutHelper::adjustAlignment(Rect(0, 0, finalSize), layout->desiredSize(), m_finalStyle->horizontalContentAlignment, m_finalStyle->verticalContentAlignment, &contentSlotRect);
 //   }
 //   else {
 //       contentSlotRect = Rect(0, 0, finalSize);
 //   }

 //   contentSlotRect = contentSlotRect.makeDeflate(m_finalStyle->padding);
 //   

 //   struct ElementList : public IUIElementList {
 //       List<Ref<UIElement>>* list;
 //       virtual int getElementCount() const { return list->size(); }
 //       virtual UIElement* getElement(int i) const { return list->at(i); }
 //   } list;
 //   list.list = &m_logicalChildren;

	//layout->arrangeLayout(&list, contentSlotRect);
 //   return finalSize;

 //   Rect contentSlotRect(0, 0, finalSize);
 //   detail::LayoutHelper::adjustAlignment(Rect(0, 0, finalSize), desiredSize(), m_finalStyle->horizontalContentAlignment, m_finalStyle->verticalContentAlignment, &contentSlotRect);

	////if (m_logicalChildrenHost) {
 ////       m_logicalChildrenHost->arrangeLayout(contentSlotRect.makeDeflate(finalStyle()->padding));
	////	return finalSize;
	////}
	////else {
	//	return UIFrameLayout2::staticArrangeOverride(this, contentSlotRect.getSize());
	////}
    return UIFrameLayout2::staticArrangeOverride(this, finalSize);
}

void UIControl::onLayoutPanelChanged(UILayoutPanel* newPanel)
{
}



} // namespace ln

