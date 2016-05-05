
#pragma once
#include "UIElement.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class UIItemsControl
	: public UIElement
{
	LN_UI_TYPEINFO_DECLARE();
public:


protected:
	UIItemsControl();
	virtual ~UIItemsControl();
	void Initialize(detail::UIManager* manager);

private:
};

LN_NAMESPACE_END
