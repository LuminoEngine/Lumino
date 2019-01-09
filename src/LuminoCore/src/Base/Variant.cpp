
#include "Internal.hpp"
#include <LuminoCore/Base/Variant.hpp>

namespace ln {

//==============================================================================
// Variant

const Variant Variant::Empty;

Variant::Variant()
	: m_type(VariantType::Null)
{}

Variant::Variant(nullptr_t)
	: Variant()
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
	: m_type(VariantType::Int16)
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

Variant::Variant(RefObject* value)
    : m_type(VariantType::RefObject)
    , v_RefObject(value)
{
}

Variant::Variant(List<Variant>* value)
	: m_type(VariantType::List)
	, v_List(value)
{
}

Variant::Variant(const Ref<List<Variant>>& value)
	: m_type(VariantType::List)
	, v_List(value)
{
}

Variant::Variant(const List<Variant>& value)
	: Variant(Ref<List<Variant>>(LN_NEW List<Variant>(value), false))
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
    case VariantType::RefObject:
        v_RefObject.~Ref();
        break;
	case VariantType::List:
		v_List.~Ref();
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

List<Variant>& Variant::list()
{
	assert(m_type == VariantType::List);
	return *v_List;
}

const List<Variant>& Variant::list() const
{
	assert(m_type == VariantType::List);
	return *v_List;
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
	new(&v_String) String(value);
}

void Variant::assign(const Ref<RefObject>& value)
{
    m_type = VariantType::RefObject;
    new(&v_RefObject) Ref<RefObject>(value);
}

void Variant::assign(const Ref<List<Variant>>& value)
{
	m_type = VariantType::List;
	new(&v_List) Ref<List<Variant>>(value);
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
    case VariantType::RefObject:
        assign(value.v_RefObject);
        break;
	case VariantType::List:
		assign(value.v_List);
		break;
	default:
		assert(0);
		break;
	}
}

} // namespace ln
