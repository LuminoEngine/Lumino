﻿
#include "Internal.hpp"
#include <LuminoEngine/Engine/VMProperty.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>
#include <LuminoEngine/UI/UIListBox.hpp>

namespace ln {

//==============================================================================
// UIListItem

UIListItem::UIListItem()
	: m_ownerListControl(nullptr)
	, m_onSubmitted()
	, m_isSelected(false)
{
	m_objectManagementFlags.unset(detail::ObjectManagementFlags::AutoAddToPrimaryElement);
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

void UIListItem::onSubmitted(UIEventArgs* e)
{
}

void UIListItem::onSelected(UIEventArgs* e)
{
}

void UIListItem::onUnselected(UIEventArgs* e)
{
}

void UIListItem::onRoutedEvent(UIEventArgs* e)
{
	UIControl::onRoutedEvent(e);

	if (e->type() == UIEvents::MouseDownEvent) {
		m_ownerListControl->notifyItemClicked(this);
		e->handled = true;
		return;
	}
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
// UIListItemsControl

UIListItemsControl::UIListItemsControl()
	: m_itemsHostLayout(nullptr)
	, m_selectedItems()
{
}

bool UIListItemsControl::init()
{
	if (!UIControl::init()) return false;

	// TODO: 負荷軽減のため、デフォルトでは作成しない。その場合、StackLayout としてレイアウトする
	auto layout = makeObject<UIStackLayout>();
	layout->setOrientation(Orientation::Vertical);
	setItemsLayoutPanel(layout);

	return true;
}

void UIListItemsControl::setItemsLayoutPanel(UILayoutPanel2* layout)
{
	if (LN_REQUIRE(layout)) return;

	if (m_itemsHostLayout) {
		for (auto& item : *m_logicalChildren) {
			m_itemsHostLayout->removeVisualChild(item);
		}
		removeVisualChild(m_itemsHostLayout);
	}

	m_itemsHostLayout = layout;

	if (m_itemsHostLayout) {
		addVisualChild(m_itemsHostLayout);

		for (const auto& item : *m_logicalChildren) {
			m_itemsHostLayout->addVisualChild(item);
		}
	}
}

void UIListItemsControl::addListItem(UIListItem* item)
{
	if (LN_REQUIRE(item)) return;
	if (LN_REQUIRE(!item->logicalParent())) return;

	item->m_ownerListControl = this;

	m_logicalChildren->add(item);
	item->setLogicalParent(this);

	if (m_itemsHostLayout) {
		m_itemsHostLayout->addVisualChild(item);
	}
}

void UIListItemsControl::removeListItem(UIListItem* item)
{
	if (LN_REQUIRE(item)) return;
	if (LN_REQUIRE(item->m_ownerListControl == this)) return;

	m_selectedItems.remove(item);

	item->m_ownerListControl = nullptr;

	m_logicalChildren->remove(item);
	item->setLogicalParent(nullptr);

	if (m_itemsHostLayout) {
		m_itemsHostLayout->removeVisualChild(item);
	}
}

void UIListItemsControl::removeAllItems()
{
	for (const auto& item : *m_logicalChildren) {
		static_pointer_cast<UIListItem>(item)->m_ownerListControl = nullptr;
		item->setLogicalParent(nullptr);
		m_itemsHostLayout->removeVisualChild(item);
	}
	m_logicalChildren->clear();
}

Size UIListItemsControl::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	if (m_itemsHostLayout) {
		m_itemsHostLayout->measureLayout(layoutContext, constraint);
		return layoutContext->makeDesiredSize(this, m_itemsHostLayout->desiredSize());
	}
	else {
		LN_NOTIMPLEMENTED();
		return UIControl::measureOverride(layoutContext, constraint);
	}
}

Size UIListItemsControl::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
	Rect contentRect = layoutContext->makeContentRect(this, finalSize);

	if (m_itemsHostLayout) {
		m_itemsHostLayout->arrangeLayout(layoutContext, contentRect);
		return finalSize;
	}
	else {
		LN_NOTIMPLEMENTED();
		return UIControl::arrangeOverride(layoutContext, finalSize);
	}
}

void UIListItemsControl::notifyItemClicked(UIListItem* item)
{
	selectItemExclusive(item);
}

void UIListItemsControl::selectItemExclusive(UIListItem* item)
{
	if (LN_REQUIRE(item)) return;

	for (const auto& item : m_selectedItems) {
		item->setSelectedInternal(false);
	}
	m_selectedItems.clear();

	m_selectedItems.add(item);
	item->setSelectedInternal(true);
}

//==============================================================================
// UIListBoxItem

UIListBoxItem::UIListBoxItem()
{
}

bool UIListBoxItem::init()
{
	if (!UIListItem::init()) return false;
	return true;
}

void UIListBoxItem::bind(ObservablePropertyBase* prop)
{
	auto textblock = makeObject<UITextBlock>();
	auto viewProp = textblock->getViewProperty(u"text");
	viewProp->bind(prop);
	addChild(textblock);
}

//==============================================================================
// UIListBox

Ref<UIListBox> UIListBox::create()
{
    return makeObject<UIListBox>();
}

UIListBox::UIListBox()
{
}

bool UIListBox::init()
{
	if (!UIListItemsControl::init()) return false;
	return true;
}

void UIListBox::bind(ObservablePropertyBase* prop)
{
	if (prop->type()->typeClass() == TypeInfoClass::Object) {
		auto obj = prop->getValue()->getObject<Object>();
		if (auto collection = dynamic_pointer_cast<CollectionObject>(obj)) {	// TODO: TypeInfo で Collection かどうか調べたいところ
			int count = collection->getItemCount();
			for (int i = 0; i < count; i++) {
				auto item = ln::makeObject<UIListBoxItem>();
				auto data = collection->getItemAsVariant(i);
				if (auto dataObj = data->getAsObject<Object>()) {
					if (detail::ObjectHelper::isObservableProperty(dataObj)) {
						item->bind(static_pointer_cast<ObservablePropertyBase>(dataObj));
					}
					else {
						LN_NOTIMPLEMENTED();
					}
				}
				else {
					LN_NOTIMPLEMENTED();
				}
				
				addListItem(item);
			}
		}
	}
}

void UIListBox::onAddChild(UIElement* child)
{
    auto item = ln::makeObject<UIListBoxItem>();
    item->addElement(child);

	addListItem(item);
}

} // namespace ln

