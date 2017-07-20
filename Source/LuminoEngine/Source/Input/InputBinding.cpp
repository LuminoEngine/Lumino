
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

static const String& getKeyName(Keys key)
{
	struct KeyName
	{
		Keys	key;	// for verify
		String	name;
	};
	static const KeyName table[] =
	{
		{ Keys::Unknown,	_T("Unknown") },
		{ Keys::A,			_T("A") },
		{ Keys::B,			_T("B") },
		{ Keys::C,			_T("C") },
		{ Keys::D,			_T("D") },
		{ Keys::E,			_T("E") },
		{ Keys::F,			_T("F") },
		{ Keys::G,			_T("G") },
		{ Keys::H,			_T("H") },
		{ Keys::I,			_T("I") },
		{ Keys::J,			_T("J") },
		{ Keys::K,			_T("K") },
		{ Keys::L,			_T("L") },
		{ Keys::M,			_T("M") },
		{ Keys::N,			_T("N") },
		{ Keys::O,			_T("O") },
		{ Keys::P,			_T("P") },
		{ Keys::Q,			_T("Q") },
		{ Keys::R,			_T("R") },
		{ Keys::S,			_T("S") },
		{ Keys::T,			_T("T") },
		{ Keys::U,			_T("U") },
		{ Keys::V,			_T("V") },
		{ Keys::W,			_T("W") },
		{ Keys::X,			_T("X") },
		{ Keys::Y,			_T("Y") },
		{ Keys::Z,			_T("Z") },

		{ Keys::D0,			_T("0") },
		{ Keys::D1,			_T("1") },
		{ Keys::D2,			_T("2") },
		{ Keys::D3,			_T("3") },
		{ Keys::D4,			_T("4") },
		{ Keys::D5,			_T("5") },
		{ Keys::D6,			_T("6") },
		{ Keys::D7,			_T("7") },
		{ Keys::D8,			_T("8") },
		{ Keys::D9,			_T("9") },

		{ Keys::F1,			_T("F1") },
		{ Keys::F2,			_T("F2") },
		{ Keys::F3,			_T("F3") },
		{ Keys::F4,			_T("F4") },
		{ Keys::F5,			_T("F5") },
		{ Keys::F6,			_T("F6") },
		{ Keys::F7,			_T("F7") },
		{ Keys::F8,			_T("F8") },
		{ Keys::F9,			_T("F9") },
		{ Keys::F10,		_T("F10") },
		{ Keys::F11,		_T("F11") },
		{ Keys::F12,		_T("F12") },

		{ Keys::Space,		_T("Space") },
		{ Keys::Escape,		_T("Esc") },
		{ Keys::Up,			_T("Up") },
		{ Keys::Down,		_T("Down") },
		{ Keys::Left,		_T("Left") },
		{ Keys::Right,		_T("Right") },
		{ Keys::LShift,		_T("LeftShift") },
		{ Keys::RShift,		_T("RightShift") },
		{ Keys::LCtrl,		_T("LeftCtrl") },
		{ Keys::RCtrl,		_T("RightCtrl") },
		{ Keys::LAlt,		_T("LeftAlt") },
		{ Keys::RAlt,		_T("RightAlt") },
		{ Keys::Tab,		_T("Tab") },
		{ Keys::Enter,		_T("Enter") },
		{ Keys::BackSpace,	_T("BackSpace") },
		{ Keys::Insert,		_T("Insert") },
		{ Keys::Delete,		_T("Delete") },
		{ Keys::PageUp,		_T("PageUp") },
		{ Keys::PageDown,	_T("PageDown") },
		{ Keys::Home,		_T("Home") },
		{ Keys::End,		_T("End") },

		{ Keys::Colon,		_T(":") },
		{ Keys::Semicolon,	_T(";") },
		{ Keys::Comma,		_T(",") },
		{ Keys::Period,		_T(".") },
		{ Keys::Slash,		_T("/") },
		{ Keys::Minus,		_T("-") },
		{ Keys::Backslash,	_T("\\") },
		{ Keys::Yen,		_T("\\") },
		{ Keys::Caret,		_T("'") },
		{ Keys::LBracket,	_T("[") },
		{ Keys::RBracket,	_T("]") },

		{ Keys::AndroidPlus,	_T("+") },
	};

	int ki = (int)key;
	LN_ASSERT(ki < LN_ARRAY_SIZE_OF(table));
	LN_ASSERT(key == table[ki].key);
	return table[ki].name;
}

//------------------------------------------------------------------------------
KeyboardBindingPtr KeyboardBinding::create(Keys key, ModifierKeys modifierKeys)
{
	auto ptr = KeyboardBindingPtr::makeRef(key, modifierKeys);
	return ptr;
}

//------------------------------------------------------------------------------
KeyboardBinding::KeyboardBinding(Keys key, ModifierKeys modifierKeys)
	: m_key(key)
	, m_modifierKeys(modifierKeys)
{
}

//------------------------------------------------------------------------------
KeyboardBinding::~KeyboardBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType KeyboardBinding::getType() const
{
	return detail::InputBindingType::Keyboard;
}


//==============================================================================
// MouseBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(MouseBinding, InputBinding);

//------------------------------------------------------------------------------
MouseBindingPtr MouseBinding::create(MouseAction mouseAction, ModifierKeys modifierKeys)
{
	auto ptr = MouseBindingPtr::makeRef(mouseAction, modifierKeys);
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
detail::InputBindingType MouseBinding::getType() const
{
	return detail::InputBindingType::Mouse;
}


//==============================================================================
// GamepadBinding
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GamepadBinding, InputBinding);

//------------------------------------------------------------------------------
GamepadBindingPtr GamepadBinding::create(GamepadElement element)
{
	auto ptr = GamepadBindingPtr::makeRef(element);
	return ptr;
}

//------------------------------------------------------------------------------
GamepadBinding::GamepadBinding(GamepadElement element)
	: m_element(element)
{
}

//------------------------------------------------------------------------------
GamepadBinding::~GamepadBinding()
{
}

//------------------------------------------------------------------------------
detail::InputBindingType GamepadBinding::getType() const
{
	return detail::InputBindingType::Gamepad;
}

LN_NAMESPACE_END
