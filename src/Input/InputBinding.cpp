
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
InputBinding::InputBinding()
	: m_negativeValue(false)
	, m_minValidMThreshold(0.2f)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputBinding::InputBinding(const String& bindingName, Key key)
	: InputBinding()
{
	m_bindingName = bindingName;
	m_source.id = detail::DeviceInputSource::KeyboardFlag | ((uint32_t)key);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
InputBinding::~InputBinding()
{
}

LN_NAMESPACE_END
