
#include "../Internal.h"
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/ReflectionArrayObject.h>
#include <Lumino/Reflection/Variant.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Variant
//==============================================================================

const Variant Variant::Empty;

Variant::Variant()
	: m_type(VariantType::Null)
{}

Variant::Variant(bool value)
	: m_type(VariantType::Bool)
	, v_Bool(value)
{
}

Variant::Variant(Char value)
	: m_type(VariantType::Char)
	, v_Char(value)
{
}

Variant::Variant(int8_t value)
	: m_type(VariantType::Int8)
	, v_Int8(value)
{
}

Variant::Variant(int16_t value)
	: m_type(VariantType::Int64)
	, v_Int64(value)
{
}

Variant::Variant(int32_t value)
	: m_type(VariantType::Int32)
	, v_Int32(value)
{
}

Variant::Variant(int64_t value)
	: m_type(VariantType::Int64)
	, v_Int64(value)
{
}

Variant::Variant(uint8_t value)
	: m_type(VariantType::UInt8)
	, v_UInt8(value)
{
}

Variant::Variant(uint16_t value)
	: m_type(VariantType::UInt16)
	, v_UInt16(value)
{
}

Variant::Variant(uint32_t value)
	: m_type(VariantType::UInt32)
	, v_UInt32(value)
{
}

Variant::Variant(uint64_t value)
	: m_type(VariantType::UInt64)
	, v_UInt64(value)
{
}

Variant::Variant(float value)
	: m_type(VariantType::Float)
	, v_Float(value)
{
}

Variant::Variant(double value)
	: m_type(VariantType::Double)
	, v_Double(value)
{
}

Variant::Variant(const Char* value)
	: Variant(String(value))
{
}

Variant::Variant(const String& value)
	: m_type(VariantType::String)
	, v_String(value)
{
}

Variant::Variant(const Variant& value)
	: m_type(VariantType::Null)
{
	copy(value);
}

Variant::~Variant()
{
	clear();
}

void Variant::clear() LN_NOEXCEPT
{
	switch (m_type)
	{
	case VariantType::Null:
	case VariantType::Bool:
	case VariantType::Char:
	case VariantType::Int8:
	case VariantType::Int16:
	case VariantType::Int32:
	case VariantType::Int64:
	case VariantType::UInt8:
	case VariantType::UInt16:
	case VariantType::UInt32:
	case VariantType::UInt64:
	case VariantType::Float:
	case VariantType::Double:
		break;
	case VariantType::String:
		v_String.~String();
		break;
	default:
		assert(0);
		break;
	}

	m_type = VariantType::Null;
}

Variant& Variant::operator=(const Variant& rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}

void Variant::assign(bool value)
{
	m_type = VariantType::Bool;
	v_Bool = value;
}

void Variant::assign(Char value)
{
	m_type = VariantType::Char;
	v_Char = value;
}

void Variant::assign(int8_t value)
{
	m_type = VariantType::Int8;
	v_Int8 = value;
}

void Variant::assign(int16_t value)
{
	m_type = VariantType::Int16;
	v_Int16 = value;
}

void Variant::assign(int32_t value)
{
	m_type = VariantType::Int32;
	v_Int32 = value;
}

void Variant::assign(int64_t value)
{
	m_type = VariantType::Int64;
	v_Int64 = value;
}

void Variant::assign(uint8_t value)
{
	m_type = VariantType::UInt8;
	v_UInt8 = value;
}

void Variant::assign(uint16_t value)
{
	m_type = VariantType::UInt16;
	v_UInt16 = value;
}

void Variant::assign(uint32_t value)
{
	m_type = VariantType::UInt32;
	v_UInt32 = value;
}

void Variant::assign(uint64_t value)
{
	m_type = VariantType::UInt64;
	v_UInt64 = value;
}

void Variant::assign(float value)
{
	m_type = VariantType::Float;
	v_Float = value;
}

void Variant::assign(double value)
{
	m_type = VariantType::Double;
	v_Double = value;
}

void Variant::assign(const String& value)
{
	m_type = VariantType::String;
	v_String = value;
}

void Variant::copy(const Variant& value)
{
	switch (value.m_type)
	{
	case VariantType::Null:
		break;
	case VariantType::Bool:
		assign(value.v_Bool);
		break;
	case VariantType::Char:
		assign(value.v_Char);
		break;
	case VariantType::Int8:
		assign(value.v_Int8);
		break;
	case VariantType::Int16:
		assign(value.v_Int16);
		break;
	case VariantType::Int32:
		assign(value.v_Int32);
		break;
	case VariantType::Int64:
		assign(value.v_Int64);
		break;
	case VariantType::UInt8:
		assign(value.v_UInt8);
		break;
	case VariantType::UInt16:
		assign(value.v_UInt16);
		break;
	case VariantType::UInt32:
		assign(value.v_UInt32);
		break;
	case VariantType::UInt64:
		assign(value.v_UInt64);
		break;
	case VariantType::Float:
		assign(value.v_Float);
		break;
	case VariantType::Double:
		assign(value.v_Double);
		break;
	case VariantType::String:
		assign(value.v_String);
		break;
	default:
		assert(0);
		break;
	}
}

LN_NAMESPACE_END

#ifdef LN_LEGACY_VARIANT_ENABLED
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
void Variant::setNullPtr(std::nullptr_t value)
{
	release();
}
std::nullptr_t Variant::getNullPtr() const
{
	if (LN_REQUIRE(m_type == VariantType::Null)) return nullptr;
	return nullptr;
}
void Variant::setBool(bool value)
{
	release();
	m_type = VariantType::Bool;
	m_bool = value;
}
bool Variant::getBool() const
{
	if (LN_REQUIRE(m_type == VariantType::Bool)) return false;
	return m_bool;
}
void Variant::setArithmetic(int32_t value)
{
	release();
	m_type = VariantType::Int32;
	m_int32 = value;
}
void Variant::setArithmetic(uint32_t value)
{
	release();
	m_type = VariantType::UInt32;
	m_uint32 = value;
}
//void Variant::getArithmetic(int32_t* value) const
//{
//	LN_CHECK_STATE_RETURNV(m_type == VariantType::Int32);
//	*value = m_int32;
//}
void Variant::setArithmetic(float value)
{
	release();
	m_type = VariantType::Float;
	m_float = value;
}
//void Variant::getArithmetic(float* value) const
//{
//	LN_CHECK_STATE_RETURNV(m_type == VariantType::Float);
//	*value = m_float;
//}
void Variant::setArithmetic(double value)
{
	release();
	m_type = VariantType::Double;
	m_double = value;
}
void Variant::setString(const Char* value)
{
	release();
	m_type = VariantType::String;
	m_string = LN_NEW ln::detail::GenericStringCore<Char>();
	m_string->assign(value);
}
void Variant::setString(const String& value)
{
	release();
	m_type = VariantType::String;
	LN_REFOBJ_SET(m_string, value.m_string);
}
String Variant::getString() const
{
	if (LN_REQUIRE(m_type == VariantType::String)) return String::getEmpty();
	String str;
	str.attach(m_string);
	return str;
}
void Variant::setEnumValue(EnumValueType value)
{
	release();
	m_type = VariantType::Enum;
	m_enum = value;
}
EnumValueType Variant::getEnumValue() const
{
	if (LN_REQUIRE(m_type == VariantType::Enum)) return 0;
	return m_enum;
}
void Variant::setStruct(const void* value, size_t size, const std::type_info& typeInfo)
{
	if (LN_REQUIRE(size <= sizeof(m_struct))) return;
	m_type = VariantType::Struct;
	memcpy(m_struct, value, size);
	m_structSize = size;
	m_typeInfo = &typeInfo;
}
const void* Variant::getStruct() const
{
	if (LN_REQUIRE(m_type == VariantType::Struct)) return nullptr;
	return (const void*)m_struct;
}
void Variant::setReflectionObject(Object* obj)
{
	release();
	m_type = VariantType::Object;
	LN_REFOBJ_SET(m_object, obj);
}
Object* Variant::getReflectionObject() const
{
	if (m_type == VariantType::Null) return nullptr;
	if (LN_REQUIRE(m_type == VariantType::Object || m_type == VariantType::ArrayObject)) return nullptr;	// List も Object の一部。
	return m_object;
}
void Variant::setReflectionArrayObject(ReflectionArrayObject* obj)
{
	release();
	m_type = VariantType::ArrayObject;
	LN_REFOBJ_SET(m_arrayObject, obj);
}
ReflectionArrayObject* Variant::getReflectionArrayObject() const
{
	if (m_type == VariantType::Null) return nullptr;
	if (LN_REQUIRE(m_type == VariantType::ArrayObject)) return nullptr;
	return m_arrayObject;
}

//------------------------------------------------------------------------------
bool Variant::equals(const Variant& obj) const
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
void Variant::copy(const Variant& obj)
{
	release();
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
void Variant::release()
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

#endif
