
#include "../../Internal.h"
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/RepeatButton.h>
#include "../UIManagerImpl.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// RepeatButton
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(RepeatButton, ButtonBase);
LN_UI_ELEMENT_SUBCLASS_IMPL(RepeatButton);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RepeatButton::RepeatButton(GUIManagerImpl* manager)
	: ButtonBase(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RepeatButton::~RepeatButton()
{
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

