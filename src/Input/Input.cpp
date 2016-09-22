
#include "Internal.h"
#include "InputManager.h"
#include <Lumino/Input/InputController.h>
#include <Lumino/Input/Input.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Input
//==============================================================================

//------------------------------------------------------------------------------
bool Input::IsPressed(const StringRef& bindingName)
{
	return detail::InputManager::GetInstance()->GetVirtualPad(0)->IsPressed(bindingName);
}

//------------------------------------------------------------------------------
bool Input::IsTriggered(const StringRef& bindingName)
{
	return detail::InputManager::GetInstance()->GetVirtualPad(0)->IsTriggered(bindingName);
}

//------------------------------------------------------------------------------
bool Input::IsOffTriggered(const StringRef& bindingName)
{
	return detail::InputManager::GetInstance()->GetVirtualPad(0)->IsOffTriggered(bindingName);
}

//------------------------------------------------------------------------------
bool Input::IsRepeated(const StringRef& bindingName)
{
	return detail::InputManager::GetInstance()->GetVirtualPad(0)->IsRepeated(bindingName);
}

//------------------------------------------------------------------------------
float Input::GetAxisValue(const StringRef& bindingName)
{
	return detail::InputManager::GetInstance()->GetVirtualPad(0)->GetAxisValue(bindingName);
}

//==============================================================================
// InputButton
//==============================================================================
const String InputButtons::Any = String::GetEmpty();
const String InputButtons::Left = _T("left");
const String InputButtons::Right = _T("right");
const String InputButtons::Up = _T("up");
const String InputButtons::Down = _T("down");
const String InputButtons::Ok = _T("ok");
const String InputButtons::Cancel = _T("cancel");

LN_NAMESPACE_END
