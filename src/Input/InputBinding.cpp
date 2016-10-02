
#include "../Internal.h"
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// InputBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(InputBinding, tr::ReflectionObject);

//------------------------------------------------------------------------------
InputBindingPtr InputBinding::CreateKeyboardBinding(Key key, ModifierKeys modifier)
{
	auto ptr = RefPtr<InputBinding>::MakeRef();
	ptr->InitializeKeyboardBinding(key, modifier);
	return ptr;
}

//------------------------------------------------------------------------------
InputBindingPtr InputBinding::CreateJoystickButtonBinding(int buttonNumber)
{
	auto ptr = RefPtr<InputBinding>::MakeRef();
	ptr->InitializeJoystickButtonBinding(buttonNumber);
	return ptr;
}

//------------------------------------------------------------------------------
InputBindingPtr InputBinding::CreateJoystickAxisBinding(int axisNumber)
{
	auto ptr = RefPtr<InputBinding>::MakeRef();
	ptr->InitializeJoystickAxisBinding(axisNumber);
	return ptr;
}

//------------------------------------------------------------------------------
InputBindingPtr InputBinding::CreateJoystickPovBinding(JoystickPovAxis direction)
{
	auto ptr = RefPtr<InputBinding>::MakeRef();
	ptr->InitializeJoystickPovBinding(direction);
	return ptr;
}

//------------------------------------------------------------------------------
InputBinding::InputBinding()
	: m_source()
	, m_minValidMThreshold(0.2f)
	, m_negativeValue(false)
{
}

//------------------------------------------------------------------------------
InputBinding::~InputBinding()
{
}

//------------------------------------------------------------------------------
void InputBinding::InitializeKeyboardBinding(Key key, ModifierKeys modifier)
{
	m_source.type = detail::DeviceInputSourceType::Keyboard;
	m_source.Keyboard.key = key;
	m_source.Keyboard.modifierKeys = (ModifierKeys::enum_type)(int)modifier;
}

//------------------------------------------------------------------------------
void InputBinding::InitializeJoystickButtonBinding(int buttonNumber)
{
	m_source.type = detail::DeviceInputSourceType::JoystickButton;
	m_source.JoystickButton.buttonNumber = buttonNumber;
}

//------------------------------------------------------------------------------
void InputBinding::InitializeJoystickAxisBinding(int axisNumber)
{
	m_source.type = detail::DeviceInputSourceType::JoystickAxis;
	m_source.JoystickAxis.axizNumber = axisNumber;
}

//------------------------------------------------------------------------------
void InputBinding::InitializeJoystickPovBinding(JoystickPovAxis direction)
{
	m_source.type = detail::DeviceInputSourceType::JoystickPov;
	m_source.JoystickPov.povAxis = direction;
}

//------------------------------------------------------------------------------
bool InputBinding::EqualKeyInput(Key keyCode, ModifierKeys modifier)
{
	if (m_source.type == detail::DeviceInputSourceType::Keyboard)
	{
		if (keyCode != m_source.Keyboard.key) return false;
		if (modifier != m_source.Keyboard.modifierKeys) return false;
		return true;
	}
	return false;
}

LN_NAMESPACE_END
