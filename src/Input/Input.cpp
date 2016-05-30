
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

//------------------------------------------------------------------------------
InputController* Input::GetController(int index)
{
	return detail::InputManager::GetInstance()->GetVirtualPad(index);
}

//==============================================================================
// InputButton
//==============================================================================
const String InputButtons::Any = String::GetEmpty();
const String InputButtons::Left = _T("Left");
const String InputButtons::Right = _T("Right");
const String InputButtons::Up = _T("Up");
const String InputButtons::Down = _T("Down");
const String InputButtons::Ok = _T("Ok");
const String InputButtons::Cancel = _T("Cancel");

LN_NAMESPACE_END
