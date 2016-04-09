
#include "../Internal.h"
#include "InputDriver.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// InputDriver
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputDriver::InputDriver()
	: m_keyStatus{}
	, m_mouseStatus{}
	, m_mousePoint()
	, m_mouseWheel(0)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputDriver::~InputDriver()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputDriver::UpdateFrame()
{
	m_keyStatus.fill(false);
	m_mouseStatus.fill(false);
	m_mousePoint.x = 0;
	m_mousePoint.y = 0;
	m_mouseWheel = 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void InputDriver::OnEvent(const PlatformEventArgs& e)
{
	switch (e.Type)
	{
	case EventType_MouseDown:
		m_mouseStatus[e.Mouse.Button] = true;
		break;
	case EventType_MouseUp:
		m_mouseStatus[e.Mouse.Button] = false;
		break;
	case EventType_MouseMove:
		m_mousePoint.x = e.Mouse.X;
		m_mousePoint.y = e.Mouse.Y;
		break;
	case EventType_MouseWheel:
		m_mouseWheel = e.Mouse.WheelDelta;
		break;
	case EventType_KeyDown:
		m_keyStatus[(int)e.Key.KeyCode] = true;
		break;
	case EventType_KeyUp:
		m_keyStatus[(int)e.Key.KeyCode] = false;
		break;
	default:
		break;
	}
}

LN_NAMESPACE_END
