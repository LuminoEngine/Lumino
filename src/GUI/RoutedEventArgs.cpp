
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/RoutedEventArgs.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// MouseEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(MouseEventArgs, RoutedEventArgs);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MouseEventArgs::MouseEventArgs()
	: Button(MouseButton::None)
	, Wheel(0)
	, X(0)
	, Y(0)
	, ClickCount(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MouseEventArgs::~MouseEventArgs()
{
}


//=============================================================================
// KeyEventArgs
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(KeyEventArgs, RoutedEventArgs);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyEventArgs::KeyEventArgs()
	: KeyCode(Key::Unknown)
	, IsAlt(false)
	, IsShift(false)
	, IsControl(false)
	, Char(0x00)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyEventArgs::~KeyEventArgs()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//char KeyEventArgs::GetChar() const
//{
//	if (Key_A <= KeyCode && KeyCode <= Key_Z)
//	{
//		if (IsShift) {
//			return 'A' + KeyCode;
//		}
//		else {
//			return 'a' + KeyCode;
//		}
//	}
//	if (Key_0 <= KeyCode && KeyCode <= Key_9) { return '0' + KeyCode; }
//
//	switch (KeyCode)
//	{
//	case Key_Space: return ' ';
//	case Key_Tab: return '\t';
//
//	case Key_Colon: return ':';
//	case Key_SemiColon: return ';';
//	case Key_Comma: return ',';
//	case Key_Period: return '.';
//	case Key_Slash: return '/';
//	case Key_Minus: return '-';
//	case Key_BackSlash: return '\\';
//	case Key_Yen: return '\\';
//	case Key_Caret: return '\'';
//	case Key_LBracket: return ',';
//	case Key_RBracket: return ',';
//
//	case Key_Android_Plus,		///< Android '+'
//	}
//}

LN_NAMESPACE_GUI_END
} // namespace Lumino

