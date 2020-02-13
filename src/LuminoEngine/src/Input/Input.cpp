
#include "Internal.hpp"
#include <LuminoEngine/Input/InputController.hpp>
#include <LuminoEngine/Input/Input.hpp>
#include "InputManager.hpp"

namespace ln {

//==============================================================================
// Input
//==============================================================================

static InputController* Input_activeDevice()
{
	return detail::EngineDomain::inputManager()->getVirtualPad(0);
}

//------------------------------------------------------------------------------
bool Input::pressed(const StringRef& buttonName)
{
	return Input_activeDevice()->pressed(buttonName);
}

//------------------------------------------------------------------------------
bool Input::triggered(const StringRef& buttonName)
{
	return Input_activeDevice()->triggered(buttonName);
}

//------------------------------------------------------------------------------
bool Input::triggeredOff(const StringRef& buttonName)
{
	return Input_activeDevice()->triggeredOff(buttonName);
}

//------------------------------------------------------------------------------
bool Input::repeated(const StringRef& buttonName)
{
	return Input_activeDevice()->repeated(buttonName);
}

//------------------------------------------------------------------------------
float Input::getAxisValue(const StringRef& buttonName)
{
	return Input_activeDevice()->getAxisValue(buttonName);
}

//------------------------------------------------------------------------------
void Input::addBinding(const StringRef& buttonName, InputGesture* gesture)
{
	return Input_activeDevice()->addBinding(buttonName, gesture);
}

//------------------------------------------------------------------------------
void Input::removeBinding(InputGesture* gesture)
{
	return Input_activeDevice()->removeBinding(gesture);
}

//------------------------------------------------------------------------------
void Input::clearBindings(const StringRef& buttonName)
{
	return Input_activeDevice()->clearBindings(buttonName);
}

//------------------------------------------------------------------------------
void Input::clearAllBindings()
{
	return Input_activeDevice()->clearAllBindings();
}

//------------------------------------------------------------------------------
InputGesture* Input::getAnyActiveTriggered()
{
	return detail::EngineDomain::inputManager()->getAnyActiveTriggered();
}

//==============================================================================
// InputButton
//==============================================================================
const String InputButtons::Left = _LT("left");
const String InputButtons::Right = _LT("right");
const String InputButtons::Up = _LT("up");
const String InputButtons::Down = _LT("down");
const String InputButtons::Submit = _LT("submit");
const String InputButtons::Cancel = _LT("cancel");
const String InputButtons::Menu = _LT("menu");
const String InputButtons::Shift = _LT("shift");
const String InputButtons::PageUp = _LT("pageup");
const String InputButtons::PageDown = _LT("pagedown");
const String InputButtons::Any = String::Empty;

} // namespace ln
