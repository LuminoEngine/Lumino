
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollViewer.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// ScrollViewer
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollViewer);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollViewer);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::ScrollViewer(GUIManager* manager)
	: ContentControl(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollViewer::~ScrollViewer()
{
}

} // namespace GUI
} // namespace Lumino

