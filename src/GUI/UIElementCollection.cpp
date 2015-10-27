
#include "../Internal.h"
#include <Lumino/GUI/UIElementCollection.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// UIElementCollection
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(UIElementCollection, GenericCoreList<UIElement*>);	// TODO: テンプレートはやめるかも

void UIElementCollection::InsertItem(int index, const value_type& item)
{
	ItemAdded.Raise(item);
	GenericCoreList<UIElement*>::InsertItem(index, item);
}
void UIElementCollection::ClearItems()
{
	if (GetCount() > 0) {
		LN_THROW(0, NotImplementedException);
	}

	//for (UIElement* obj : *this) {
	//	m_owner->OnInlineRemoved(obj);
	//}
	GenericCoreList<UIElement*>::ClearItems();
}
void UIElementCollection::RemoveItem(int index)
{
	ItemRemoved.Raise(GetAt(index));
	GenericCoreList<UIElement*>::RemoveItem(index);
}
void UIElementCollection::SetItem(int index, const value_type& item)
{
	LN_THROW(0, NotImplementedException);
	//m_owner->OnInlineAdded(static_cast<UIElement*>(item.GetObject()));
	GenericCoreList<UIElement*>::SetItem(index, item);
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

