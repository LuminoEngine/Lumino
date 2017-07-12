
#include "Internal.h"
#include <Lumino/UI/UILayoutPanel.h>
#include <Lumino/UI/UIElementCollection.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// UIElementCollection
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIElementCollection, tr::ReflectionObjectList<UIElement*>);

//------------------------------------------------------------------------------
UIElementCollection::UIElementCollection(tr::IUIElementCollectionOwner* owner)
	: m_owner(owner)
{
}

//------------------------------------------------------------------------------
UIElementCollection::~UIElementCollection()
{
}

//------------------------------------------------------------------------------
void UIElementCollection::insertItem(int index, const value_type& item)
{
	m_newItemsCache.resize(1);
	m_newItemsCache[0] = item;
	m_oldItemsCache.clear();

	tr::ReflectionObjectList<UIElement*>::insertItem(index, item);

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::Add,
		m_newItemsCache, index,
		m_oldItemsCache, 0,
	};
	m_owner->onChildCollectionChanged(e);
	m_newItemsCache.clear();
}

//------------------------------------------------------------------------------
void UIElementCollection::clearItems()
{
	m_newItemsCache.clear();
	m_oldItemsCache.clear();

	tr::ReflectionObjectList<UIElement*>::clearItems();

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::reset,
		m_newItemsCache, 0,
		m_oldItemsCache, 0,
	};
	m_owner->onChildCollectionChanged(e);
}

//------------------------------------------------------------------------------
void UIElementCollection::removeItem(int index)
{
	m_newItemsCache.clear();
	m_oldItemsCache.resize(1);
	m_oldItemsCache[0] = getAt(index);

	tr::ReflectionObjectList<UIElement*>::removeItem(index);

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::Remove,
		m_newItemsCache, 0,
		m_oldItemsCache, index,
	};
	m_owner->onChildCollectionChanged(e);
	m_oldItemsCache.clear();
}

//------------------------------------------------------------------------------
void UIElementCollection::setItem(int index, const value_type& item)
{
	m_newItemsCache.resize(1);
	m_newItemsCache[0] = item;
	m_oldItemsCache.resize(1);
	m_oldItemsCache[0] = getAt(index);

	tr::ReflectionObjectList<UIElement*>::setItem(index, item);

	tr::ChildCollectionChangedArgs e =
	{
		tr::NotifyCollectionChangedAction::replace,
		m_newItemsCache, index,
		m_oldItemsCache, index,
	};
	m_owner->onChildCollectionChanged(e);
	m_newItemsCache.clear();
	m_oldItemsCache.clear();
}

LN_NAMESPACE_END

