
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Thumb.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Thumb
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Thumb);
LN_UI_ELEMENT_SUBCLASS_IMPL(Thumb);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::Thumb(GUIManager* manager)
	: Control(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Thumb::~Thumb()
{
}

} // namespace GUI
} // namespace Lumino

