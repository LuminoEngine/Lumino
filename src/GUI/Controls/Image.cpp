
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Image.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// Image
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Image, Control);
LN_UI_ELEMENT_SUBCLASS_IMPL(Image);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image::Image(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Image::~Image()
{
}

} // namespace GUI
} // namespace Lumino

