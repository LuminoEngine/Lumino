
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/RepeatButton.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// RepeatButton
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(RepeatButton, ButtonBase);
LN_UI_ELEMENT_SUBCLASS_IMPL(RepeatButton);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RepeatButton::RepeatButton(GUIManager* manager)
	: ButtonBase(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RepeatButton::~RepeatButton()
{
}

} // namespace GUI
} // namespace Lumino

