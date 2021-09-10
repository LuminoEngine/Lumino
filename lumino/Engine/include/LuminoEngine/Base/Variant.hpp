
#pragma once

#include <typeinfo>
#include <type_traits>
#include "Common.hpp"
#include "../Reflection/Object.hpp"
#include "../Reflection/TypeInfo.hpp"
#include <LuminoCore/Geometries/GeometryStructs.hpp>

namespace ln {
namespace detail { class VariantHelper; }
class Variant;

enum class VariantType
{
	// primitive
	Null,
	Bool,
	Char,	// ln::Char
	Int,
	Int64,
	Float,
	Double,
	String,

	// struct
	Vector2,
	Vector3,
	Quaternion,
	Transform,
	Rect,

	// object
    RefObject,
	List,

	Int8,	// obsolete
	Int16,	// obsolete
	//Int64,
	UInt8,	// obsolete
	UInt16,	// obsolete
	UInt32,	// obsolete
	UInt64,	// obsolete
	Int32 = Int,
};

void serialize(Archive& ar, Variant& value);




/** RefObject 用 スマートポインタ */
template<>
class Ref<Variant>
{
public:
	Ref(bool value) noexcept;
	Ref(int32_t value) noexcept;
	Ref(const String& value) noexcept;

#define LN_SPECIALIZED_TYPE Variant
#include <LuminoCore/Base/Ref1.inl>
#undef LN_SPECIALIZED_TYPE
};




/** Variant */
LN_CLASS()
class Variant
	: public Object
{
public:
	static const Variant Empty;

	template<class T> friend class Ref;

LN_CONSTRUCT_ACCESS:
	Variant();

	// primitive
	Variant(std::nullptr_t);
	Variant(const Variant& value);
	Variant(bool value) : Variant() { assign(value); }
	Variant(Char value) : Variant() { assign(value); }
	Variant(int8_t value) : Variant() { assign(value); }
	Variant(int16_t value) : Variant() { assign(value); }
	Variant(int32_t value) : Variant() { assign(value); }
	Variant(int64_t value) : Variant() { assign(value); }
	Variant(uint8_t value) : Variant() { assign(value); }
	Variant(uint16_t value) : Variant() { assign(value); }
	Variant(uint32_t value) : Variant() { assign(value); }
	Variant(uint64_t value) : Variant() { assign(value); }
	Variant(float value) : Variant() { assign(value); }
	Variant(double value) : Variant() { assign(value); }
	Variant(const Char* value) : Variant(String(value)) { }
	Variant(const String& value) : Variant() { assign(value); }

	// struct
	Variant(const Vector2& value) : Variant() { assign(value); }
	Variant(const Vector3& value) : Variant() { assign(value); }
	Variant(const Quaternion& value) : Variant() { assign(value); }
	Variant(const AttitudeTransform& value) : Variant() { assign(value); }
	Variant(const Rect& value) : Variant() { assign(value); }

	// object
    Variant(RefObject* value) : Variant() { assign(value); }
    template<class TValue>
    Variant(const Ref<TValue>& value) : Variant() { assign(value.get()); }
	Variant(List<Ref<Variant>>* value) : Variant() { assign(value); }
	Variant(const List<Ref<Variant>>& value);
	Variant(const Ref<List<Ref<Variant>>>& value) : Variant() { assign(value); }
	virtual ~Variant();

	template<class T>
	Variant(const List<T>& list);
	
	/** init. */
	LN_METHOD()
	bool init() { return Object::init(); }

public:
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

	/** Object として取得。Object ではない場合は nullptr */
	template<typename TValue>
	Ref<TValue> getAsObject() const
	{
		if (type() != VariantType::RefObject) return nullptr;
		return static_pointer_cast<TValue>(v_RefObject);
	}

	/** utility *get<Ref<List<Variantl>>>() */
	List<Ref<Variant>>& list();
	const List<Ref<Variant>>& list() const;

	Variant& operator=(const Variant& rhs);


	//LN_SERIALIZE_CLASS_VERSION(1);
	virtual void serializeInternal(Archive& ar, ArchiveNodeType loadType)
	{
		Variant& value = *this;

		//ArchiveNodeType type;
		//ar.makeVariantTag(&type);

		if (ar.isSaving())
		{
			switch (value.type())
			{
			case VariantType::Null:
			{
				//LN_NOTIMPLEMENTED();
				//Ref<Object> null = nullptr;
				//ar.process(null);
				ar.processNull();
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
				List<Ref<Variant>>& v = value.list();
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
            auto type = loadType;
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
				auto v = makeRef<List<Ref<Variant>>>();
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
	void assign(const Vector2& value);
	void assign(const Vector3& value);
	void assign(const Quaternion& value);
	void assign(const AttitudeTransform& value);
	void assign(const Rect& value);
    void assign(const Ref<RefObject>& value);
	void assign(RefObject* value) { assign(Ref<RefObject>(value)); }
	template<class TValue> void assign(const Ref<TValue>& value) { assign(value.get()); }
	void assign(const Ref<List<Ref<Variant>>>& value);

	//const void* getValueRawPtr() const { return reinterpret_cast<const void*>(&v_Bool); }
	
	/** setInt */
	LN_METHOD()
	void setInt(int value) { return assign(value); }
	
	/** getInt */
	LN_METHOD()
	int getInt() const;

private:
	bool changeType(VariantType newType);
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
		Vector2 v_Vector2;
		Vector3 v_Vector3;
		Quaternion v_Quaternion;
		AttitudeTransform* v_Transform;
		Rect v_Rect;
        Ref<RefObject> v_RefObject;
		Ref<List<Ref<Variant>>> v_List;
	};

	friend class detail::VariantHelper;
	friend void serialize(Archive& ar, Variant& value);
	template<class T, class... TArgs> friend Ref<T> makeRef(TArgs&&... args);
	template<typename... TArgs> friend Ref<Variant> makeVariant(TArgs&&... args);
};

template<class T>
Variant::Variant(const List<T>& list)
	: Variant(makeRef<List<Ref<Variant>>>())
{
	auto& tl = Variant::list();
	for (auto& item : list) {
		tl.add(Ref<Variant>(LN_NEW Variant(item), false));
	}
}


inline Ref<Variant>::Ref(bool value) noexcept
	: Ref(LN_NEW Variant(value), false)
{}

inline Ref<Variant>::Ref(int32_t value) noexcept
	: Ref(LN_NEW Variant(value), false)
{}

inline Ref<Variant>::Ref(const String& value) noexcept
	: Ref(LN_NEW Variant(value), false)
{}


#define LN_SPECIALIZED_TYPE Variant
#include <LuminoCore/Base/Ref2.inl>
#undef LN_SPECIALIZED_TYPE

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

//inline void serialize(Archive& ar, Variant& value)
//{
//	ArchiveNodeType type;
//	ar.makeVariantTag(&type);
//
//	if (ar.isSaving())
//	{
//		switch (value.type())
//		{
//		case VariantType::Null:
//		{
//			LN_NOTIMPLEMENTED();
//			break;
//		}
//		case VariantType::Bool:
//		{
//			auto v = value.get<bool>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::Char:
//		{
//			LN_NOTIMPLEMENTED();
//			break;
//		}
//		case VariantType::Int8:
//		{
//			auto v = value.get<int8_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::Int16:
//		{
//			auto v = value.get<int16_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::Int32:
//		{
//			auto v = value.get<int32_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::Int64:
//		{
//			auto v = value.get<int64_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::UInt8:
//		{
//			auto v = value.get<uint8_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::UInt16:
//		{
//			auto v = value.get<uint16_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::UInt32:
//		{
//			auto v = value.get<uint32_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::UInt64:
//		{
//			auto v = value.get<uint64_t>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::Float:
//		{
//			auto v = value.get<float>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::Double:
//		{
//			auto v = value.get<double>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::String:
//		{
//			auto v = value.get<String>();
//			ar.process(v);
//			break;
//		}
//		case VariantType::List:
//		{
//			List<Ref<Variant>>& v = value.list();
//			ar.process(v);
//			break;
//		}
//		default:
//			LN_UNREACHABLE();
//			break;
//		}
//	}
//	else
//	{
//		switch (type)
//		{
//		case ln::ArchiveNodeType::Null:
//		{
//			value.clear();
//			break;
//		}
//		case ln::ArchiveNodeType::Bool:
//		{
//			bool v;
//			ar.process(v);
//			value = v;
//			break;
//		}
//		case ln::ArchiveNodeType::Int64:
//		{
//			int64_t v;
//			ar.process(v);
//			value = v;
//			break;
//		}
//		case ln::ArchiveNodeType::Double:
//		{
//			double v;
//			ar.process(v);
//			value = v;
//			break;
//		}
//		case ln::ArchiveNodeType::String:
//		{
//			String v;
//			ar.process(v);
//			value = v;
//			break;
//		}
//		case ln::ArchiveNodeType::Object:
//		{
//			LN_NOTIMPLEMENTED();
//			break;
//		}
//		case ln::ArchiveNodeType::Array:
//		{
//			auto v = makeRef<List<Ref<Variant>>>();
//			ar.process(v);
//			value = v;
//			break;
//		}
//		default:
//			LN_UNREACHABLE();
//			break;
//		}
//	}
//}



template<>
inline void serialize(Archive& ar, Ref<Variant>& value)
{
	ArchiveNodeType type;
	ar.makeVariantTag(&type);

	if (ar.isSaving()) {
		if (!value) {
			ar.processNull();
		}
		else {
			value->serializeInternal(ar, type);
		}
	}
	else {
		if (type != ArchiveNodeType::Null) {

			// TODO: いまのところ Variant 用
			if (!value) {
				value = makeObject<Variant>();
			}
			value->serializeInternal(ar, type);
		}
		else {
			value = nullptr;
		}
	}
}



template<typename... TArgs>
Ref<Variant> makeVariant(TArgs&&... args)
{
	auto ptr = Ref<Variant>(new Variant(std::forward<TArgs>(args)...), false);
	ptr->init();
	return ptr;
}


} // namespace ln
