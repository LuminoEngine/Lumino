
#include "../Internal.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/ReflectionArrayObject.h>
#include <Lumino/Reflection/Variant.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// Variant
//==============================================================================

const Variant Variant::Null;

//------------------------------------------------------------------------------
Variant::Variant()
	: m_type(VariantType::Null)
	, m_object(nullptr)
	, m_structSize(0)
{
}

//------------------------------------------------------------------------------
void Variant::SetNullPtr(std::nullptr_t value)
{
	Release();
}
std::nullptr_t Variant::GetNullPtr() const
{
	if (LN_CHECK_STATE(m_type == VariantType::Null)) return nullptr;
	return nullptr;
}
void Variant::SetBool(bool value)
{
	Release();
	m_type = VariantType::Bool;
	m_bool = value;
}
bool Variant::GetBool() const
{
	if (LN_CHECK_STATE(m_type == VariantType::Bool)) return false;
	return m_bool;
}
void Variant::SetArithmetic(int32_t value)
{
	Release();
	m_type = VariantType::Int32;
	m_int32 = value;
}
void Variant::SetArithmetic(uint32_t value)
{
	Release();
	m_type = VariantType::UInt32;
	m_uint32 = value;
}
//void Variant::GetArithmetic(int32_t* value) const
//{
//	LN_CHECK_STATE_RETURNV(m_type == VariantType::Int32);
//	*value = m_int32;
//}
void Variant::SetArithmetic(float value)
{
	Release();
	m_type = VariantType::Float;
	m_float = value;
}
//void Variant::GetArithmetic(float* value) const
//{
//	LN_CHECK_STATE_RETURNV(m_type == VariantType::Float);
//	*value = m_float;
//}
void Variant::SetArithmetic(double value)
{
	Release();
	m_type = VariantType::Double;
	m_double = value;
}
void Variant::SetString(const TCHAR* value)
{
	Release();
	m_type = VariantType::String;
	m_string = LN_NEW ln::detail::GenericStringCore<TCHAR>();
	m_string->assign(value);
}
void Variant::SetString(const String& value)
{
	Release();
	m_type = VariantType::String;
	LN_REFOBJ_SET(m_string, value.m_string);
}
String Variant::GetString() const
{
	if (LN_CHECK_STATE(m_type == VariantType::String)) return String::GetEmpty();
	String str;
	str.Attach(m_string);
	return str;
}
void Variant::SetEnumValue(EnumValueType value)
{
	Release();
	m_type = VariantType::Enum;
	m_enum = value;
}
EnumValueType Variant::GetEnumValue() const
{
	if (LN_CHECK_STATE(m_type == VariantType::Enum)) return 0;
	return m_enum;
}
void Variant::SetStruct(const void* value, size_t size, const std::type_info& typeInfo)
{
	if (LN_CHECK_ARG(size <= sizeof(m_struct))) return;
	m_type = VariantType::Struct;
	memcpy(m_struct, value, size);
	m_structSize = size;
	m_typeInfo = &typeInfo;
}
const void* Variant::GetStruct() const
{
	if (LN_CHECK_STATE(m_type == VariantType::Struct)) return nullptr;
	return (const void*)m_struct;
}
void Variant::SetReflectionObject(ReflectionObject* obj)
{
	Release();
	m_type = VariantType::Object;
	LN_REFOBJ_SET(m_object, obj);
}
ReflectionObject* Variant::GetReflectionObject() const
{
	if (m_type == VariantType::Null) return nullptr;
	if (LN_CHECK_STATE(m_type == VariantType::Object || m_type == VariantType::ArrayObject)) return nullptr;	// List も Object の一部。
	return m_object;
}
void Variant::SetReflectionArrayObject(ReflectionArrayObject* obj)
{
	Release();
	m_type = VariantType::ArrayObject;
	LN_REFOBJ_SET(m_arrayObject, obj);
}
ReflectionArrayObject* Variant::GetReflectionArrayObject() const
{
	if (m_type == VariantType::Null) return nullptr;
	if (LN_CHECK_STATE(m_type == VariantType::ArrayObject)) return nullptr;
	return m_arrayObject;
}

//------------------------------------------------------------------------------
bool Variant::Equals(const Variant& obj) const
{
	if (m_type != obj.m_type) return false;
	switch (m_type)
	{
	case VariantType::Null:
		return true;
	case VariantType::Bool:
		return m_bool == obj.m_bool;
	case VariantType::Int32:
		return m_int32 == obj.m_int32;
	case VariantType::UInt32:
		return m_uint32 == obj.m_uint32;
	case VariantType::Float:
		return m_float == obj.m_float;
	case VariantType::Double:
		return m_double == obj.m_double;
	case VariantType::String:
		return m_string == obj.m_string;
	case VariantType::Enum:
		return m_enum == obj.m_enum;
	case VariantType::Struct:
		if (m_structSize != obj.m_structSize) return false;
		return memcmp(m_struct, obj.m_struct, m_structSize) != 0;
	case VariantType::Object:
		return m_object == obj.m_object;
	case VariantType::ArrayObject:
		return m_arrayObject == obj.m_arrayObject;
	default:
		LN_ASSERT(0);
		break;
	}
	return false;
}

//------------------------------------------------------------------------------
void Variant::Copy(const Variant& obj)
{
	Release();
	m_type = obj.m_type;
	switch (m_type)
	{
	case VariantType::Null:
		break;
	case VariantType::Bool:
		m_bool = obj.m_bool;
		break;
	case VariantType::Int32:
		m_int32 = obj.m_int32;
		break;
	case VariantType::UInt32:
		m_uint32 = obj.m_uint32;
		break;
	case VariantType::Float:
		m_float = obj.m_float;
		break;
	case VariantType::Double:
		m_double = obj.m_double;
		break;
	case VariantType::String:
		LN_REFOBJ_SET(m_string, obj.m_string);
		break;
	case VariantType::Enum:
		m_enum = obj.m_enum;
		break;
	case VariantType::Struct:
		m_structSize = obj.m_structSize;
		memcpy(m_struct, obj.m_struct, m_structSize);
		break;
	case VariantType::Object:
		LN_REFOBJ_SET(m_object, obj.m_object);
		break;
	case VariantType::ArrayObject:
		LN_REFOBJ_SET(m_arrayObject, obj.m_arrayObject);
		break;
	default:
		LN_ASSERT(0);
		break;
	}
}

//------------------------------------------------------------------------------
void Variant::Release()
{
	if (m_type == VariantType::String) {
		LN_SAFE_RELEASE(m_string);
	}
	else if (m_type == VariantType::Object) {
		LN_SAFE_RELEASE(m_object);
	}
	else if (m_type == VariantType::ArrayObject) {
		LN_SAFE_RELEASE(m_arrayObject);
	}
	m_type = VariantType::Null;
	m_structSize = 0;
}

} // namespace tr
LN_NAMESPACE_END
