
#pragma once

#include <typeinfo>
#include <type_traits>
#include "Common.hpp"

namespace ln {
namespace detail { class VariantHelper; }
class Variant;

enum class VariantType
{
	Null,
	Bool,
	Char,	// ln::Char
	Int8,
	Int16,
	Int32,
	Int64,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Float,
	Double,
	String,
    RefObject,
	List,
};

class Variant
{
public:
	static const Variant Empty;

	Variant();
	Variant(std::nullptr_t);
	Variant(bool value);
	Variant(Char value);
	Variant(int8_t value);
	Variant(int16_t value);
	Variant(int32_t value);
	Variant(int64_t value);
	Variant(uint8_t value);
	Variant(uint16_t value);
	Variant(uint32_t value);
	Variant(uint64_t value);
	Variant(float value);
	Variant(double value);
	Variant(const Char* value);
	Variant(const String& value);
	Variant(const Variant& value);
    Variant(RefObject* value);
    template<class TValue>
    Variant(const Ref<TValue>& value)
        : m_type(VariantType::RefObject)
        , v_RefObject(value)
    {}
	Variant(List<Variant>* value);
	Variant(const List<Variant>& value);
	Variant(const Ref<List<Variant>>& value);
	~Variant();

	template<class T>
	Variant(const List<T>& list)
		: Variant(makeRef<List<Variant>>())
	{
		auto& tl = Variant::list();
		for (auto& item : list) tl.add(item);
	}

	void clear() LN_NOEXCEPT;

	bool hasValue() const { return m_type != VariantType::Null; }

	VariantType type() const { return m_type; }

	bool isNumeric() const;

	template<typename TValue>
	TValue get() const;

    template<typename TValue>
    Ref<TValue> getObject() const
    {
        if (LN_REQUIRE(type() == VariantType::RefObject)) return nullptr;
        return static_pointer_cast<TValue>(v_RefObject);
    }

	/** utility *get<Ref<List<Variantl>>>() */
	List<Variant>& list();
	const List<Variant>& list() const;

	Variant& operator=(const Variant& rhs);

private:
	void assign(bool value);
	void assign(Char value);
	void assign(int8_t value);
	void assign(int16_t value);
	void assign(int32_t value);
	void assign(int64_t value);
	void assign(uint8_t value);
	void assign(uint16_t value);
	void assign(uint32_t value);
	void assign(uint64_t value);
	void assign(float value);
	void assign(double value);
	void assign(const String& value);
    void assign(const Ref<RefObject>& value);
	void assign(const Ref<List<Variant>>& value);
	void copy(const Variant& value);

	VariantType	m_type;

	union
	{
		bool v_Bool;
		Char v_Char;
		int8_t v_Int8;
		int16_t v_Int16;
		int32_t v_Int32;
		int64_t v_Int64;
		uint8_t v_UInt8;
		uint16_t v_UInt16;
		uint32_t v_UInt32;
		uint64_t v_UInt64;
		float v_Float;
		double v_Double;
		String v_String;
        Ref<RefObject> v_RefObject;
		Ref<List<Variant>> v_List;
	};

	friend class detail::VariantHelper;
};

namespace detail
{

class VariantHelper
{
public:
	static bool canConvertToNumeric(const Variant& value)
	{
		switch (value.type())
		{
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
			return true;
		default:
			return false;
		}
	}

	template<typename T>
	static T convertToNumeric(const Variant& value);


	static bool getRawBool(const Variant& value) { return value.v_Bool; }
	//static Char getRawBool(const Variant& value) { return value.v_Char; }
	//static int8_t getRawBool(const Variant& value) { return value.v_Int8; }
	//static int16_t getRawBool(const Variant& value) { return value.v_Int16; }
	//static int32_t getRawBool(const Variant& value) { return value.v_Int32; }
	//static int64_t getRawBool(const Variant& value) { return value.v_Int64; }
	//static uint8_t getRawBool(const Variant& value) { return value.v_UInt8; }
	//static uint16_t getRawBool(const Variant& value) { return value.v_UInt16; }
	//static uint32_t getRawBool(const Variant& value) { return value.v_UInt32; }
	//static uint64_t getRawBool(const Variant& value) { return value.v_UInt64; }
	//static float getRawBool(const Variant& value) { return value.v_Float; }
	//static double getRawBool(const Variant& value) { return value.v_Double; }
	static const String& getRawString(const Variant& value) { return value.v_String; }
};

template<typename T>
T VariantHelper::convertToNumeric(const Variant& value)
{
	switch (value.type())
	{
    case VariantType::Char:
        return static_cast<T>(value.v_Char);
	case VariantType::Int8:
		return static_cast<T>(value.v_Int8);
	case VariantType::Int16:
		return static_cast<T>(value.v_Int16);
	case VariantType::Int32:
		return static_cast<T>(value.v_Int32);
	case VariantType::Int64:
		return static_cast<T>(value.v_Int64);
	case VariantType::UInt8:
		return static_cast<T>(value.v_UInt8);
	case VariantType::UInt16:
		return static_cast<T>(value.v_UInt16);
	case VariantType::UInt32:
		return static_cast<T>(value.v_UInt32);
	case VariantType::UInt64:
		return static_cast<T>(value.v_UInt64);
	case VariantType::Float:
		return static_cast<T>(value.v_Float);
	case VariantType::Double:
		return static_cast<T>(value.v_Double);
	default:
		assert(0);
		return 0;
	}
}

} // namespace detail

inline bool Variant::isNumeric() const
{
	return detail::VariantHelper::canConvertToNumeric(*this);
}

template<typename TValue>
struct VariantValueTraits
{
	static bool canConvertFrom(const Variant& value) { return false; }
	static TValue convert(const Variant& value) { return TValue(); }
};

template<>
struct VariantValueTraits<bool>
{
	static bool canConvertFrom(const Variant& value) { return value.type() == VariantType::Bool; }
	static bool convert(const Variant& value) { return detail::VariantHelper::getRawBool(value); }
};
template<>
struct VariantValueTraits<Char>
{
    static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
    static Char convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<Char>(value); }
};
template<>
struct VariantValueTraits<int8_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int8_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int8_t>(value); }
};
template<>
struct VariantValueTraits<int16_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int16_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int16_t>(value); }
};
template<>
struct VariantValueTraits<int32_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int32_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int32_t>(value); }
};
template<>
struct VariantValueTraits<int64_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static int64_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<int64_t>(value); }
};
template<>
struct VariantValueTraits<uint8_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint8_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint8_t>(value); }
};
template<>
struct VariantValueTraits<uint16_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint16_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint16_t>(value); }
};
template<>
struct VariantValueTraits<uint32_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint32_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint32_t>(value); }
};
template<>
struct VariantValueTraits<uint64_t>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static uint64_t convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<uint64_t>(value); }
};
template<>
struct VariantValueTraits<float>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static float convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<float>(value); }
};
template<>
struct VariantValueTraits<double>
{
	static bool canConvertFrom(const Variant& value) { return detail::VariantHelper::canConvertToNumeric(value); }
	static double convert(const Variant& value) { return detail::VariantHelper::convertToNumeric<double>(value); }
};
template<>
struct VariantValueTraits<String>
{
	static bool canConvertFrom(const Variant& value) { return value.type() == VariantType::String; }
	static String convert(const Variant& value) { return detail::VariantHelper::getRawString(value); }
};

template<typename TValue>
TValue Variant::get() const
{
	return VariantValueTraits<TValue>::convert(*this);
}

//==============================================================================

inline void serialize(Archive& ar, Variant& value)
{
	ArchiveNodeType type;
	ar.makeVariantTag(&type);

	if (ar.isSaving())
	{
		switch (value.type())
		{
		case VariantType::Null:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		case VariantType::Bool:
		{
			auto v = value.get<bool>();
			ar.process(v);
			break;
		}
		case VariantType::Char:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		case VariantType::Int8:
		{
			auto v = value.get<int8_t>();
			ar.process(v);
			break;
		}
		case VariantType::Int16:
		{
			auto v = value.get<int16_t>();
			ar.process(v);
			break;
		}
		case VariantType::Int32:
		{
			auto v = value.get<int32_t>();
			ar.process(v);
			break;
		}
		case VariantType::Int64:
		{
			auto v = value.get<int64_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt8:
		{
			auto v = value.get<uint8_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt16:
		{
			auto v = value.get<uint16_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt32:
		{
			auto v = value.get<uint32_t>();
			ar.process(v);
			break;
		}
		case VariantType::UInt64:
		{
			auto v = value.get<uint64_t>();
			ar.process(v);
			break;
		}
		case VariantType::Float:
		{
			auto v = value.get<float>();
			ar.process(v);
			break;
		}
		case VariantType::Double:
		{
			auto v = value.get<double>();
			ar.process(v);
			break;
		}
		case VariantType::String:
		{
			auto v = value.get<String>();
			ar.process(v);
			break;
		}
		case VariantType::List:
		{
			List<Variant>& v = value.list();
			ar.process(v);
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
		}
	}
	else
	{
		switch (type)
		{
		case ln::ArchiveNodeType::Null:
		{
			value.clear();
			break;
		}
		case ln::ArchiveNodeType::Bool:
		{
			bool v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::Int64:
		{
			int64_t v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::Double:
		{
			double v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::String:
		{
			String v;
			ar.process(v);
			value = v;
			break;
		}
		case ln::ArchiveNodeType::Object:
		{
			LN_NOTIMPLEMENTED();
			break;
		}
		case ln::ArchiveNodeType::Array:
		{
			auto v = makeRef<List<Variant>>();
			ar.process(v);
			value = v;
			break;
		}
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

} // namespace ln
