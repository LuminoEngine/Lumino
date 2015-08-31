
#include "../Internal.h"
#include <Lumino/GUI/UIElementCollection.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// UIElementCollection
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(UIElementCollection, GenericVariantList<UIElement*>);	// TODO: テンプレートはやめるかも

void UIElementCollection::InsertItem(int index, const Variant& item)
{
	ItemAdded.Raise(Variant::Cast<UIElement*>(item));
	GenericVariantList<UIElement*>::InsertItem(index, item);
}
void UIElementCollection::ClearItems()
{
	if (GetCount() > 0) {
		LN_THROW(0, NotImplementedException);
	}

	//for (UIElement* obj : *this) {
	//	m_owner->OnInlineRemoved(obj);
	//}
	GenericVariantList<UIElement*>::ClearItems();
}
void UIElementCollection::RemoveItem(int index)
{
	const Variant& v = VariantList::GetAt(index);
	ItemRemoved.Raise(Variant::Cast<UIElement*>(v));
	GenericVariantList<UIElement*>::RemoveItem(index);
}
void UIElementCollection::SetItem(int index, const Variant& item)
{
	LN_THROW(0, NotImplementedException);
	//m_owner->OnInlineAdded(static_cast<UIElement*>(item.GetObject()));
	GenericVariantList<UIElement*>::SetItem(index, item);
}

} // namespace GUI
} // namespace Lumino

