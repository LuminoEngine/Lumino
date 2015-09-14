
#pragma once
#include "ButtonBase.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief
*/
class RepeatButton
	: public ButtonBase
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RepeatButton);
public:
	RepeatButton(GUIManagerImpl* manager);
	virtual ~RepeatButton();
};


LN_NAMESPACE_GUI_END
} // namespace Lumino
