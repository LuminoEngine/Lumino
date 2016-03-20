
#include "Internal.h"
#include "InputManager.h"
#include "VirtualPad.h"
#include <Lumino/Input/Input.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// Input
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Input::IsPressed(const StringRef& bindingName)
{
	return detail::GetInputManager(nullptr)->GetVirtualPad(0)->IsPressed(bindingName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Input::IsOnTriggered(const StringRef& bindingName)
{
	return detail::GetInputManager(nullptr)->GetVirtualPad(0)->IsOnTriggered(bindingName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Input::IsOffTriggered(const StringRef& bindingName)
{
	return detail::GetInputManager(nullptr)->GetVirtualPad(0)->IsOffTriggered(bindingName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Input::IsRepeated(const StringRef& bindingName)
{
	return detail::GetInputManager(nullptr)->GetVirtualPad(0)->IsRepeated(bindingName);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float Input::GetAxisValue(const StringRef& bindingName)
{
	return detail::GetInputManager(nullptr)->GetVirtualPad(0)->GetAxisValue(bindingName);
}

//=============================================================================
// InputButton
//=============================================================================
const String InputButtons::Any = String::GetEmpty();
const String InputButtons::Left = _T("Left");
const String InputButtons::Right = _T("Right");
const String InputButtons::Up = _T("Up");
const String InputButtons::Down = _T("Down");

LN_NAMESPACE_END
