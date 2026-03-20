
#include "Internal.hpp"
#include <LuminoEngine/Reflection/VMProperty.hpp>
#include <LuminoFramework/UI/UICommand.hpp>
//#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/Layout/UILayoutPanel.hpp>
//#include <LuminoFramework/UI/UIText.hpp>
#include <LuminoFramework/UI/Controls/UIListBox.hpp>

namespace ln {

//==============================================================================
// UIListItemsControl

LN_OBJECT_IMPLEMENT(UIListItemsControl, UIControl) {}

UIListItemsControl::UIListItemsControl()
	: m_itemsHostLayout(nullptr)
	, m_selectedItems()
	, m_selectionMoveMode(UIListSelectionMoveMode::Cyclic)
	, m_submitMode(UIListSubmitMode::Single)
{
}

bool UIListItemsControl::init()
{
	if (!UIControl::init()) return false;

	// TODO: 負荷軽減のため、デフォルトでは作成しない。その場合、StackLayout としてレイアウトする
	auto layout = makeObject_deprecated<UIStackLayout>();
	layout->setOrientation(UILayoutOrientation::Vertical);
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

void UIListItemsControl::setItemsLayoutPanel(UILayoutPanel* layout)
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

UILayoutPanel* UIListItemsControl::itemsLayoutPanel() const
{
	return m_itemsHostLayout;
}

void UIListItemsControl::setSubmitMode(UIListSubmitMode value)
{
	m_submitMode = value;
}

UIListSubmitMode UIListItemsControl::submitMode() const
{
	return m_submitMode;
}

Ref<EventConnection> UIListItemsControl::connectOnSubmit(Ref<UIGeneralEventHandler> handler)
{
	return m_onSubmit.connect(handler);
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

void UIListItemsControl::submitItem(UIListItem* item)
{
	m_onSubmit.raise(UIEventArgs::create(item, UIEvents::Submitted));
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
					float distance = std::numeric_limits<float>::max();
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
				submitItem(item);
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

Size UIListItemsControl::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const auto finalSize = finalArea.getSize();
	Rect contentRect = layoutContext->makeContentRect(this, finalSize);

	if (m_itemsHostLayout) {
		m_itemsHostLayout->arrangeLayout(layoutContext, contentRect);
		return finalSize;
	}
	else {
		LN_NOTIMPLEMENTED();
		return UIControl::arrangeOverride(layoutContext, finalArea);
	}
}

void UIListItemsControl::notifyItemClicked(UIListItem* item, int clickCount)
{
	if (submitMode() == UIListSubmitMode::Single) {
		submitItem(item);
		item->onSubmit();
	}
	else {
		selectItemExclusive(item);
	}
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
// UIListItemsControl::BuilderDetails

void UIListItemsControl::BuilderDetails::apply(UIListItemsControl* p) const
{
	UIControl::BuilderDetails::apply(p);
	if (onSubmit) p->connectOnSubmit(onSubmit);
}

//==============================================================================
// UIListBox

LN_OBJECT_IMPLEMENT(UIListBox, UIListItemsControl) {}

Ref<UIListBox> UIListBox::create()
{
    return makeObject_deprecated<UIListBox>();
}

UIListBox::UIListBox()
	: m_scrollViewHelper()
{
}

bool UIListBox::init()
{
	if (!UIListItemsControl::init()) return false;

	m_scrollViewHelper = makeRef<UIScrollViewHelper>(this);
	m_scrollViewHelper->setScrollTarget(itemsLayoutPanel());
	m_scrollViewHelper->setVScrollbarVisible(true);

	return true;
}

UIListBoxItem* UIListBox::addItem(const ln::String& text, Ref<Variant> data)
{
	auto item = UIListBoxItem::create(text);
	item->setData(data);
	addChild(item);
	return item;
}

UIListBoxItem* UIListBox::addItem(UIElement* content)
{
	if (content->m_specialElementFlags.hasFlag(detail::UISpecialElementFlags::ListItem)) {
		addChild(content);
		return static_cast<UIListBoxItem*>(content);
	}
	else {
		auto item = makeObject_deprecated<UIListBoxItem>(content);
		addChild(item);
		return item;
	}
}

//void UIListBox::bind(ObservablePropertyBase* prop)
//{
//	if (prop->type()->typeClass() == TypeInfoClass::Object) {
//		auto obj = prop->getValue()->getObject<Object>();
//		if (auto collection = dynamic_pointer_cast<CollectionObject>(obj)) {	// TODO: TypeInfo で Collection かどうか調べたいところ
//			int count = collection->getItemCount();
//			for (int i = 0; i < count; i++) {
//				auto item = ln::makeObject_deprecated<UIListBoxItem>();
//				auto data = collection->getItemAsVariant(i);
//				if (auto dataObj = data->getAsObject<Object>()) {
//					if (detail::ObjectHelper::isObservableProperty(dataObj)) {
//						item->bind(static_pointer_cast<ObservablePropertyBase>(dataObj));
//					}
//					else {
//						LN_NOTIMPLEMENTED();
//					}
//				}
//				else {
//					LN_NOTIMPLEMENTED();
//				}
//				
//				addListItem(item);
//			}
//		}
//	}
//}

void UIListBox::onAddChild(UIElement* child)
{
	// TODO: flag で判定
	if (auto* item1 = dynamic_cast<UIListBoxItem*>(child)) {
		addListItem(item1);
	}
	else {
		auto item = ln::makeObject_deprecated<UIListBoxItem>();
		item->addElement(child);
		addListItem(item);
	}
}

Size UIListBox::measureOverride(UILayoutContext* layoutContext, const Size& constraint)
{
	const Size scrollBarDesiredSize = m_scrollViewHelper->measure(layoutContext, constraint);
	const Size baseSize = UIListItemsControl::measureOverride(layoutContext, constraint);
	return Size(baseSize.width + scrollBarDesiredSize.width, baseSize.height + scrollBarDesiredSize.height);
}

Size UIListBox::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea)
{
	const Size finalSize = finalArea.getSize();
	const Rect clientArea = m_scrollViewHelper->calculateClientArea(finalSize);

	UIListItemsControl::arrangeOverride(layoutContext, clientArea);

	m_scrollViewHelper->arrange(layoutContext, finalSize);

	return finalSize;
}

void UIListBox::onRoutedEvent(UIEventArgs* e)
{
	UIListItemsControl::onRoutedEvent(e);
	m_scrollViewHelper->handleRoutedEvent(e);
}

//==============================================================================
// UIListBox::BuilderDetails

void UIListBox::BuilderDetails::apply(UIListBox* p) const
{
	UIListItemsControl::BuilderDetails::apply(p);
}

} // namespace ln

