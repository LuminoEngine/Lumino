
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
	//LN_TR_REFLECTION_TYPEINFO_DECLARE();	// TODO
public:
	typedef tr::ReflectionObjectList<UIElement*>::value_type value_type;

protected:
	virtual void InsertItem(int index, const value_type& item) override;
	virtual void ClearItems() override;
	virtual void RemoveItem(int index) override;
	virtual void SetItem(int index, const value_type& item) override;

LN_INTERNAL_ACCESS:
	UIElementCollection(UIPanel* owner);
	virtual ~UIElementCollection();

private:
	UIPanel*	m_owner;
};

LN_NAMESPACE_END
