
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class InputBinding
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		InputBinding オブジェクトを作成します。
		@details	作成されたオブジェクトは使い終えたら Release() を呼び出して参照を解放する必要があります。
	*/
	static InputBinding* Create(const String& bindingName, Key key, int joyNumber);

protected:
	InputBinding();
	virtual ~InputBinding();

private:
	String						m_bindingName;
	detail::DeviceInputSource	m_source;
};

LN_NAMESPACE_END
