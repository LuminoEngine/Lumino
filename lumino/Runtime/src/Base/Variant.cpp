
#include "Internal.hpp"
#include <LuminoEngine/Base/Variant.hpp>
#include <LuminoEngine/Reflection/Serializer3.hpp>

namespace ln {

//==============================================================================
// Variant

const Variant Variant::Empty;

Variant::Variant()
	: m_type(VariantType::Null)
{}

Variant::Variant(std::nullptr_t)
	: m_type(VariantType::Null)
{}

Variant::Variant(const Variant& value)
	: m_type(VariantType::Null)
{
	copy(value);
}

Variant::Variant(const List<Ref<Variant>>& value)
	: Variant(Ref<List<Ref<Variant>>>(LN_NEW List<Ref<Variant>>(value), false))
{
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
	case VariantType::Vector2:
		break;
	case VariantType::Vector3:
		break;
	case VariantType::Quaternion:
		break;
	case VariantType::Transform:
		LN_SAFE_DELETE(v_Transform);
		break;
	case VariantType::Rect:
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

List<Ref<Variant>>& Variant::list()
{
	assert(m_type == VariantType::List);
	return *v_List;
}

const List<Ref<Variant>>& Variant::list() const
{
	assert(m_type == VariantType::List);
	return *v_List;
}

void Variant::assign(bool value)
{
	changeType(VariantType::Bool);
	v_Bool = value;
}

void Variant::assign(Char value)
{
	changeType(VariantType::Char);
	v_Char = value;
}

void Variant::assign(int8_t value)
{
	changeType(VariantType::Int8);
	v_Int8 = value;
}

void Variant::assign(int16_t value)
{
	changeType(VariantType::Int16);
	v_Int16 = value;
}

void Variant::assign(int32_t value)
{
	changeType(VariantType::Int32);
	v_Int32 = value;
}

void Variant::assign(int64_t value)
{
	changeType(VariantType::Int64);
	v_Int64 = value;
}

void Variant::assign(uint8_t value)
{
	changeType(VariantType::UInt8);
	v_UInt8 = value;
}

void Variant::assign(uint16_t value)
{
	changeType(VariantType::UInt16);
	v_UInt16 = value;
}

void Variant::assign(uint32_t value)
{
	changeType(VariantType::UInt32);
	v_UInt32 = value;
}

void Variant::assign(uint64_t value)
{
	changeType(VariantType::UInt64);
	v_UInt64 = value;
}

void Variant::assign(float value)
{
	changeType(VariantType::Float);
	v_Float = value;
}

void Variant::assign(double value)
{
	changeType(VariantType::Double);
	v_Double = value;
}

void Variant::assign(const String& value)
{
	changeType(VariantType::String);
	new(&v_String) String(value);
}

void Variant::assign(const Vector2& value)
{
	changeType(VariantType::Vector2);
	v_Vector2 = value;
}

void Variant::assign(const Vector3& value)
{
	changeType(VariantType::Vector3);
	new(&v_Vector3) Vector3(value);
}

void Variant::assign(const Quaternion& value)
{
	changeType(VariantType::Quaternion);
	new(&v_Quaternion) Quaternion(value);
}

void Variant::assign(const AttitudeTransform& value)
{
	changeType(VariantType::Transform);
	v_Transform = LN_NEW AttitudeTransform(value);
}

void Variant::assign(const Rect& value)
{
	changeType(VariantType::Rect);
	v_Rect = value;
}

void Variant::assign(const Ref<RefObject>& value)
{
    if (!changeType(VariantType::RefObject)) {
        v_RefObject.~Ref();
    }

    new(&v_RefObject) Ref<RefObject>(value);
}

void Variant::assign(const Ref<List<Ref<Variant>>>& value)
{
    if (!changeType(VariantType::List)) {
        v_List.~Ref();
    }

    new(&v_List) Ref<List<Ref<Variant>>>(value);
}

int Variant::getInt() const
{
	if (LN_REQUIRE(isNumeric())) return 0;
	return detail::VariantHelper::convertToNumeric<int32_t>(*this);
}

bool Variant::changeType(VariantType newType)
{
	if (m_type == newType) return false;
	clear();
	m_type = newType;
    return true;
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
	case VariantType::Vector3:
		assign(value.v_Vector3);
		break;
	case VariantType::Quaternion:
		assign(value.v_Quaternion);
		break;
	case VariantType::Transform:
		assign(*value.v_Transform);
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

void Variant::serializeInternal3(Serializer3& ar, ArchiveNodeType loadType) {

    Variant& value = *this;

    if (ar.isSaving()) {
        switch (value.type()) {
            case VariantType::Null: {
                // LN_NOTIMPLEMENTED();
                // Ref<Object> null = nullptr;
                // ar.process(null);
                ar.processNull();
                break;
            }
            case VariantType::Bool: {
                auto v = value.get<bool>();
                ar.process(v);
                break;
            }
            case VariantType::Char: {
                LN_NOTIMPLEMENTED();
                break;
            }
            case VariantType::Int8: {
                auto v = value.get<int8_t>();
                ar.process(v);
                break;
            }
            case VariantType::Int16: {
                auto v = value.get<int16_t>();
                ar.process(v);
                break;
            }
            case VariantType::Int32: {
                auto v = value.get<int32_t>();
                ar.process(v);
                break;
            }
            case VariantType::Int64: {
                auto v = value.get<int64_t>();
                ar.process(v);
                break;
            }
            case VariantType::UInt8: {
                auto v = value.get<uint8_t>();
                ar.process(v);
                break;
            }
            case VariantType::UInt16: {
                auto v = value.get<uint16_t>();
                ar.process(v);
                break;
            }
            case VariantType::UInt32: {
                auto v = value.get<uint32_t>();
                ar.process(v);
                break;
            }
            case VariantType::UInt64: {
                auto v = value.get<uint64_t>();
                ar.process(v);
                break;
            }
            case VariantType::Float: {
                auto v = value.get<float>();
                ar.process(v);
                break;
            }
            case VariantType::Double: {
                auto v = value.get<double>();
                ar.process(v);
                break;
            }
            case VariantType::String: {
                auto v = value.get<String>();
                ar.process(v);
                break;
            }
            //case VariantType::List: {
            //    List<Ref<Variant>>& v = value.list();
            //    ar.process(v);
            //    break;
            //}
            default:
                LN_UNREACHABLE();
                break;
        }
    }
    else {
        auto type = loadType;
        switch (type) {
            case ln::ArchiveNodeType::Null: {
                value.clear();
                break;
            }
            case ln::ArchiveNodeType::Bool: {
                bool v;
                ar.process(v);
                value = v;
                break;
            }
            case ln::ArchiveNodeType::Int64: {
                int64_t v;
                ar.process(v);
                value = v;
                break;
            }
            case ln::ArchiveNodeType::Double: {
                double v;
                ar.process(v);
                value = v;
                break;
            }
            case ln::ArchiveNodeType::String: {
                String v;
                ar.process(v);
                value = v;
                break;
            }
            case ln::ArchiveNodeType::Object: {
                LN_NOTIMPLEMENTED();
                break;
            }
            //case ln::ArchiveNodeType::Array: {
            //    auto v = makeRef<List<Ref<Variant>>>();
            //    ar.process(v);
            //    value = v;
            //    break;
            //}
            default:
                LN_UNREACHABLE();
                break;
        }
    }
}

} // namespace ln
