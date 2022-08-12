
#include "Internal.hpp"
#include <LuminoEngine/Reflection/VMProperty.hpp>
//#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
//#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UIText.hpp>
#include <LuminoEngine/UI/Controls/UIListBox.hpp>
#include <LuminoEngine/UI/Controls/UIListBoxItem.hpp>

namespace ln {

//==============================================================================
// UIListItem

LN_OBJECT_IMPLEMENT(UIListItem, UIControl) {}

UIListItem::UIListItem()
	: m_ownerListControl(nullptr)
	, m_onSubmit()
	, m_isSelected(false)
{
	m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
	m_specialElementFlags.set(detail::UISpecialElementFlags::ListItem);
}

bool UIListItem::init()
{
	if (!UIControl::init()) return false;
	auto vsm = getVisualStateManager();
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Unselected);
	vsm->registerState(UIVisualStates::SelectionStates, UIVisualStates::Selected);
	vsm->gotoState(UIVisualStates::Unselected);
	return true;
}

Ref<EventConnection> UIListItem::connectOnSubmit(Ref<UIGeneralEventHandler> handler)
{
	return m_onSubmit.connect(handler);
}

void UIListItem::onSubmit()
{
	m_onSubmit.raise(UIEventArgs::create(this, UIEvents::Submitted));
}

void UIListItem::onSelected(UIEventArgs* e)
{
}

void UIListItem::onUnselected(UIEventArgs* e)
{
}

void UIListItem::onRoutedEvent(UIEventArgs* e)
{

	if (e->type() == UIEvents::MouseDownEvent) {
		const auto* me = static_cast<const UIMouseEventArgs*>(e);
		m_ownerListControl->notifyItemClicked(this, me->getClickCount());
		e->handled = true;
		return;
	}
	else if (e->type() == UIEvents::MouseMoveEvent) {
		if (m_ownerListControl->submitMode() == UIListSubmitMode::Single) {
			m_ownerListControl->selectItemExclusive(this);
			e->handled = true;
			return;
		}
	}
	//if (e->type() == UIEvents::MouseEnterEvent) {
	//	if (m_ownerListControl->submitMode() == UIListSubmitMode::Single) {
	//		return;
	//	}
	//}
	//else if (e->type() == UIEvents::MouseLeaveEvent) {
	//	if (m_ownerListControl->submitMode() == UIListSubmitMode::Single) {
	//		return;
	//	}
	//}

	UIControl::onRoutedEvent(e);

}

void UIListItem::setSelectedInternal(bool selected)
{
	if (m_isSelected != selected) {
		m_isSelected = selected;

		if (m_isSelected) {
			onSelected(UIEventArgs::create(this, UIEvents::Selected));
			getVisualStateManager()->gotoState(UIVisualStates::Selected);
		}
		else {
			onUnselected(UIEventArgs::create(this, UIEvents::Selected));
			getVisualStateManager()->gotoState(UIVisualStates::Unselected);
		}
	}
}

//==============================================================================
// UIListBoxItem

LN_OBJECT_IMPLEMENT(UIListBoxItem, UIListItem) {}

Ref<UIListBoxItem> UIListBoxItem::create(StringView text)
{
	return makeObject_deprecated<UIListBoxItem>(text);
}

UIListBoxItem::UIListBoxItem()
{
}

bool UIListBoxItem::init()
{
	if (!UIListItem::init()) return false;
	return true;
}

bool UIListBoxItem::init(StringView text)
{
	if (!init()) return false;
	addChild(makeObject_deprecated<UIText>(text));
	return true;
}

bool UIListBoxItem::init(UIElement* content)
{
	if (!init()) return false;
	addChild(content);
	return true;
}

//void UIListBoxItem::bind(ObservablePropertyBase* prop)
//{
//	auto textblock = makeObject_deprecated<UIText>();
//	auto viewProp = textblock->getViewProperty(_TT("text"));
//	viewProp->bind(prop);
//	addChild(textblock);
//}

//==============================================================================
// UIListBox::BuilderDetails

void UIListBoxItem::BuilderDetails::apply(UIListBoxItem* p) const
{
	UIListItem::BuilderDetails::apply(p);

	if (!text.isEmpty()) {
		auto textblock = makeObject_deprecated<UIText>(text);
		p->addChild(textblock);
	}
	if (onSubmit) p->connectOnSubmit(onSubmit);
}


} // namespace ln

