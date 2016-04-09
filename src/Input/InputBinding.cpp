
#include "../Internal.h"
#include <Lumino/Input/InputBinding.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// InputBinding
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(InputBinding, tr::ReflectionObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputBindingPtr InputBinding::Create(const String& bindingName, Key key, ModifierKeys modifier)
{
	return RefPtr<InputBinding>::Construct(bindingName, key, modifier);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputBinding::InputBinding()
	: m_negativeValue(false)
	, m_minValidMThreshold(0.2f)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputBinding::InputBinding(const String& bindingName, Key key, ModifierKeys modifier)
	: InputBinding()
{
	m_bindingName = bindingName;
	m_source.id = detail::DeviceInputSource::KeyboardFlag | ((uint32_t)key) | (modifier << 24);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputBinding::~InputBinding()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool InputBinding::EqualKeyInput(Key keyCode, bool isAlt, bool isShift, bool isControl)
{
	if (m_source.id & detail::DeviceInputSource::KeyboardFlag)
	{
		uint32_t k = (m_source.id & detail::DeviceInputSource::ValueMask) & 0x0FFF;
		uint32_t m = ((m_source.id & detail::DeviceInputSource::ValueMask) & 0xF000) >> 24;
		if ((uint32_t)keyCode != k) return false;
		if ((m & ModifierKeys::Alt) && !isAlt) return false;
		if ((m & ModifierKeys::Shift) && !isShift) return false;
		if ((m & ModifierKeys::Control) && !isControl) return false;
		return true;
	}
	return false;
}

LN_NAMESPACE_END
