
#pragma once
#include "../Internal.h"
#include <Lumino/GUI/EventArgs.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// EventArgs
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(EventArgs);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EventArgs::EventArgs()
	: HandlerOwner(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EventArgs::~EventArgs()
{
}


//=============================================================================
// MouseEventArgs
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(MouseEventArgs);

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
MouseEventArgs* EventArgsPool::CreateMouseEventArgs(MouseButton button, short wheel, short x, short y)
{
	MouseEventArgs* args = FindFreeObject(m_mouseEventArgsPool);
	if (args == NULL) {
		args = LN_NEW MouseEventArgs();
		m_mouseEventArgsPool.Add(args);
	}

	args->Button = button;
	args->Wheel = wheel;
	args->X = x;
	args->Y = y;

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

	args->KeyCode = keyCode;
	args->IsAlt = isAlt;
	args->IsShift = isShift;
	args->IsControl = isControl;

	args->AddRef();
	return args;
}


} // namespace GUI
} // namespace Lumino

