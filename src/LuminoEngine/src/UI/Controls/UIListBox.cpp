
#include "Internal.hpp"
#include <LuminoEngine/Engine/VMProperty.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Layout/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>
#include <LuminoEngine/UI/Controls/UIListBox.hpp>

namespace ln {

//==============================================================================
// UIListItem

UIListItem::UIListItem()
	: m_ownerListControl(nullptr)
	, m_onSubmit()
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

Ref<EventConnection> UIListItem::connectOnSubmit(Ref<UIEventHandler> handler)
{
	return m_onSubmit.connect(handler);
}

void UIListItem::onSubmit()
{
	m_onSubmit.raise();
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
	, m_selectionMoveMode(UIListSelectionMoveMode::Cyclic)
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

void UIListItemsControl::selectItem(UIListItem* item)
{
	if (LN_REQUIRE(m_logicalChildren->contains(item))) return;
	selectItemExclusive(item);
}

UIListItem* UIListItemsControl::selectedItem() const
{
	if (m_selectedItems.isEmpty()) return nullptr;
	return m_selectedItems.front();
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
	if (!m_logicalChildren->isEmpty()) {
		for (const auto& item : *m_logicalChildren) {
			static_pointer_cast<UIListItem>(item)->m_ownerListControl = nullptr;
			item->setLogicalParent(nullptr);
			m_itemsHostLayout->removeVisualChild(item);
		}
		m_logicalChildren->clear();
	}
}

void UIListItemsControl::onRoutedEvent(UIEventArgs* e)
{
	UIControl::onRoutedEvent(e);

	if (e->type() == UIEvents::KeyDownEvent) {

		if (!m_logicalChildren->isEmpty()) {
			auto* k = static_cast<UIKeyEventArgs*>(e);
			Vector2 dir;
			if (k->getKey() == Keys::Left)
				dir.x = -1;
			else if (k->getKey() == Keys::Up)
				dir.y = -1;
			else if (k->getKey() == Keys::Right)
				dir.x = 1;
			else if (k->getKey() == Keys::Down)
				dir.y = 1;

			if (dir != Vector2::Zero) {
				if (m_selectedItems.isEmpty()) {
					selectItemExclusive(static_pointer_cast<UIListItem>(m_logicalChildren->at(0)));
				}
				else {

					UIElement* selected = m_selectedItems.front();
					UIElement* next = nullptr;
					float distance = std::numeric_limits<double>::max();
					for (const auto& item : *m_logicalChildren) {
						auto diff = item->localPosition() - selected->localPosition();
						auto f = diff * dir;
						if (f.x >= 1.0f || f.y >= 1.0f) {
							float len = diff.lengthSquared();
							if (len < distance) {
								distance = len;
								next = item;
							}
						}
					}

					if (!next && m_selectionMoveMode == UIListSelectionMoveMode::Cyclic) {
						dir *= -1.0f;
						distance = 0.0f;
						for (const auto& item : *m_logicalChildren) {
							auto diff = item->localPosition() - selected->localPosition();
							auto f = diff * dir;
							if (f.x >= 1.0f || f.y >= 1.0f) {
								float len = diff.lengthSquared();
								if (len > distance) {
									distance = len;
									next = item;
								}
							}
						}
					}

					if (next) {
						selectItemExclusive(static_cast<UIListItem*>(next));
					}
				}

				e->handled = true;
			}
		}





		//m_ownerListControl->notifyItemClicked(this);
		return;
	}
	else if (e->type() == UIEvents::ExecuteCommandEvent) {
		if (static_cast<UICommandEventArgs*>(e)->command() == UICommonInputCommands::submit()) {
			if (UIListItem* item = selectedItem()) {
				item->onSubmit();
				//e->handled = true;
			}
			//return;
		}
	}

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

Ref<UIListBoxItem> UIListBoxItem::create(StringRef text)
{
	return makeObject<UIListBoxItem>(text);
}

UIListBoxItem::UIListBoxItem()
{
}

bool UIListBoxItem::init()
{
	if (!UIListItem::init()) return false;
	return true;
}

bool UIListBoxItem::init(StringRef text)
{
	if (!init()) return false;

	addChild(makeObject<UITextBlock>(text));

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

UIListBoxItem* UIListBox::addItem(const ln::String& text)
{
	auto item = UIListBoxItem::create(text);
	addChild(item);
	return item;
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
	// TODO: flag で判定
	if (auto* item1 = dynamic_cast<UIListBoxItem*>(child)) {
		addListItem(item1);
	}
	else {
		auto item = ln::makeObject<UIListBoxItem>();
		item->addElement(child);
		addListItem(item);
	}
}

} // namespace ln

