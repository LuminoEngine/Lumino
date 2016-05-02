
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIElementCollection
	: public tr::ReflectionObjectList<UIElement*>
{
public:
	typedef tr::ReflectionObjectList<UIElement*>::value_type value_type;

LN_INTERNAL_ACCESS:
	UIElementCollection() = default;
	virtual ~UIElementCollection() = default;

//	Event<void(UIElement*)>	ItemAdded;
//	Event<void(UIElement*)>	ItemRemoved;
//
//protected:
//	virtual void InsertItem(int index, const value_type& item);
//	virtual void ClearItems();
//	virtual void RemoveItem(int index);
//	virtual void SetItem(int index, const value_type& item);
};

LN_NAMESPACE_END
