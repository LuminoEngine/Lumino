
#include "Internal.h"
#include <Lumino/Variant.h>

namespace Lumino
{
	
//=============================================================================
// CoreObject
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject::CoreObject()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject::~CoreObject()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
String CoreObject::ToString()
{
	return String::GetEmpty();
}
	
//=============================================================================
// Variant
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant()
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const Variant& obj)
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
	Copy(obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(float value)
	: m_type(VariantType_Float)
	, m_float(value)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(CoreObject* obj)
	: m_type(VariantType_Unknown)
	, m_uint(0)	// union 全てを初期化
{
	Set(obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::Variant(const SizeF& value)
	: m_type(VariantType_Unknown)
	, m_uint(0)
{
	SetSizeF(value);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Variant::~Variant()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::Set(CoreObject* obj)
{
	Release();
	m_type = VariantType_Object;
	LN_REFOBJ_SET(m_object, obj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CoreObject* Variant::GetObject()
{
	LN_VERIFY(m_type == VariantType_Object) { return NULL; }
	return m_object;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::SetSizeF(const SizeF& value)
{
	Release();
	m_type = VariantType_SizeF;
	*((SizeF*)m_sizeF) = value;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const SizeF& Variant::GetSizeF() const
{
	LN_VERIFY(m_type == VariantType_SizeF) { return SizeF(); }
	return *((SizeF*)m_sizeF);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::Copy(const Variant& obj)
{
	Release();
	m_type = obj.m_type;
	switch (m_type)
	{
	case Lumino::VariantType_Unknown:
		break;
	case Lumino::VariantType_Bool:
		m_bool = obj.m_bool;
		break;
	case Lumino::VariantType_Int:
		m_int = obj.m_int;
		break;
	case Lumino::VariantType_Float:
		m_float = obj.m_float;
		break;
	case Lumino::VariantType_String:
		m_string = obj.m_string;
		break;
	case Lumino::VariantType_List:
		LN_REFOBJ_SET(m_valueList, obj.m_valueList);
		break;
	case Lumino::VariantType_Object:
		LN_REFOBJ_SET(m_object, obj.m_object);
		break;
	case Lumino::VariantType_SizeF:
		memcpy(m_sizeF, obj.m_sizeF, sizeof(m_sizeF));
		break;
	default:
		LN_VERIFY(0);
		break;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Variant::Release()
{
	if (m_type == VariantType_List) {
		LN_SAFE_RELEASE(m_valueList);
	}
	else if (m_type == VariantType_Object) {
		LN_SAFE_RELEASE(m_object);
	}
	m_uint = 0;	// union 全てを初期化
}

} // namespace Lumino
