
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/EventArgs.h>

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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyEventArgs::KeyEventArgs()
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


//=============================================================================
// EventArgsPool
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EventArgsPool::EventArgsPool()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EventArgsPool::~EventArgsPool()
{
	for (auto list : m_pool)
	{
		for (auto e : (*list.second))
		{
			e->Release();
		}
		delete list.second;
	}




	LN_FOREACH(auto obj, m_mouseEventArgsPool) {
		obj->Release();
	}
	LN_FOREACH(auto obj, m_keyEventArgsPool) {
		obj->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MouseEventArgs* EventArgsPool::CreateMouseEventArgs(MouseButton button, int wheel, float x, float y, int clickCount)
{
	MouseEventArgs* args = FindFreeObject(m_mouseEventArgsPool);
	if (args == NULL) {
		args = LN_NEW MouseEventArgs();
		m_mouseEventArgsPool.Add(args);
	}

	args->Handled = false;

	args->Button = button;
	args->Wheel = wheel;
	args->X = x;
	args->Y = y;
	args->ClickCount = clickCount;

	args->AddRef();
	return args;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyEventArgs* EventArgsPool::CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	KeyEventArgs* args = FindFreeObject(m_keyEventArgsPool);
	if (args == NULL) {
		args = LN_NEW KeyEventArgs();
		m_keyEventArgsPool.Add(args);
	}

	args->Handled = false;

	args->KeyCode = keyCode;
	args->IsAlt = isAlt;
	args->IsShift = isShift;
	args->IsControl = isControl;

	args->AddRef();
	return args;
}


LN_NAMESPACE_GUI_END
} // namespace Lumino

