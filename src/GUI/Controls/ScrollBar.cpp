/*
	・WPF
		ScrollBar
			Grid
				RepeatButton
				Track
				RepeatButton

		Track
			RepeatButton	上の余白
			Thumb
				ScrollChrome
			RepeatButton	下の余白

	つまみのサイズは Track.ArrangeOverride で決めていた。
*/
#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/ScrollBar.h>

namespace Lumino
{
namespace GUI
{
//=============================================================================
// ScrollBar
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(ScrollBar);
LN_UI_ELEMENT_SUBCLASS_IMPL(ScrollBar);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::ScrollBar(GUIManager* manager)
	: Control(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ScrollBar::~ScrollBar()
{
}

} // namespace GUI
} // namespace Lumino

