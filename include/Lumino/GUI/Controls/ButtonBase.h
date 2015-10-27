
#pragma once
#include "../ContentControl.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

/**
	@brief
*/
class ButtonBase
	: public ContentControl
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ButtonBase);
public:
	ButtonBase(GUIManagerImpl* manager);
	virtual ~ButtonBase();
};


LN_NAMESPACE_GUI_END
LN_NAMESPACE_END
