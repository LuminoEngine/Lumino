
#include "Internal.hpp"
#include <LuminoEngine/Engine/VMProperty.hpp>
#include <LuminoEngine/UI/UILayoutPanel.hpp>
#include <LuminoEngine/UI/UITextBlock.hpp>
#include <LuminoEngine/UI/UIListBox.hpp>

namespace ln {

//==============================================================================
// UIListBoxItem

UIListBoxItem::UIListBoxItem()
{
}

void UIListBoxItem::init()
{
    UICollectionItem::init();
}

void UIListBoxItem::bind(ObservablePropertyBase* prop)
{
	auto textblock = makeObject<UITextBlock>();
	auto viewProp = textblock->getViewProperty(u"text");
	viewProp->bind(prop);
	//textblock->setText(child);
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

void UIListBox::init()
{
    UIItemsControl::init();

    auto layout = makeObject<UIStackLayout2_Obsolete>();
    layout->setOrientation(Orientation::Vertical);
    setItemsLayoutPanel(layout);
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

				
				UIItemsControl::addItem(item);
			}
		}
	}
}

void UIListBox::onAddChild(UIElement* child)
{
    auto item = ln::makeObject<UIListBoxItem>();
    item->addElement(child);

    UIItemsControl::addItem(item);
}

} // namespace ln

