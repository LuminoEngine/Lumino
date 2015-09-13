
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ButtonBase.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// ButtonBase
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ButtonBase, ContentControl);
LN_UI_ELEMENT_SUBCLASS_IMPL(ButtonBase);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonBase::ButtonBase(GUIManager* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ButtonBase::~ButtonBase()
{
}

LN_NAMESPACE_GUI_END
} // namespace Lumino

