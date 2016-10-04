
#include "../Internal.h"
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// InputBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(InputBinding, tr::ReflectionObject);

//------------------------------------------------------------------------------
InputBinding::InputBinding()
	: m_minValidMThreshold(0.2f)
{
}

//------------------------------------------------------------------------------
InputBinding::~InputBinding()
{
}

//==============================================================================
// KeyboardBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(KeyboardBinding, InputBinding);

//------------------------------------------------------------------------------
KeyboardBindingPtr KeyboardBinding::Create(Key key, ModifierKeys modifierKeys)
{
	auto ptr = KeyboardBindingPtr::MakeRef(key, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
KeyboardBinding::KeyboardBinding(Key key, ModifierKeys modifierKeys)
	: m_key(key)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
KeyboardBinding::~KeyboardBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType KeyboardBinding::GetType() const
{
	return detail::InputBindingType::Keyboard;
}


//==============================================================================
// MouseBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MouseBinding, InputBinding);

//------------------------------------------------------------------------------
MouseBindingPtr MouseBinding::Create(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	auto ptr = MouseBindingPtr::MakeRef(mouseAction, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
MouseBinding::MouseBinding(MouseAction mouseAction, ModifierKeys modifierKeys)
	: m_mouseAction(mouseAction)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
MouseBinding::~MouseBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType MouseBinding::GetType() const
{
	return detail::InputBindingType::Mouse;
}


//==============================================================================
// GamepadInputBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GamepadInputBinding, InputBinding);

//------------------------------------------------------------------------------
GamepadInputBindingPtr GamepadInputBinding::Create(GamepadInputElement element)
{
	auto ptr = GamepadInputBindingPtr::MakeRef(element);
	return ptr;
}

//------------------------------------------------------------------------------
GamepadInputBinding::GamepadInputBinding(GamepadInputElement element)
	: m_element(element)
{
}

//------------------------------------------------------------------------------
GamepadInputBinding::~GamepadInputBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType GamepadInputBinding::GetType() const
{
	return detail::InputBindingType::Gamepad;
}

LN_NAMESPACE_END
