
#include "Internal.h"
#include <Lumino/UI/UIPanel.h>
#include <Lumino/UI/UIElementCollection.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIElementCollection
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIElementCollection, tr::ReflectionObjectList<UIElement*>);

//------------------------------------------------------------------------------
UIElementCollection::UIElementCollection(UIPanel* owner)
	: m_owner(owner)
{
}

//------------------------------------------------------------------------------
UIElementCollection::~UIElementCollection()
{
}

//------------------------------------------------------------------------------
void UIElementCollection::InsertItem(int index, const value_type& item)
{
	m_owner->OnChildElementAdd(item);
	tr::ReflectionObjectList<UIElement*>::InsertItem(index, item);
}

//------------------------------------------------------------------------------
void UIElementCollection::ClearItems()
{
	for (UIElement* obj : *this) {
		m_owner->OnChildElementRemove(obj);
	}
	tr::ReflectionObjectList<UIElement*>::ClearItems();
}

//------------------------------------------------------------------------------
void UIElementCollection::RemoveItem(int index)
{
	m_owner->OnChildElementRemove(GetAt(index));
	tr::ReflectionObjectList<UIElement*>::RemoveItem(index);
}

//------------------------------------------------------------------------------
void UIElementCollection::SetItem(int index, const value_type& item)
{
	m_owner->OnChildElementRemove(GetAt(index));
	m_owner->OnChildElementAdd(item);
	tr::ReflectionObjectList<UIElement*>::SetItem(index, item);
}

LN_NAMESPACE_END

