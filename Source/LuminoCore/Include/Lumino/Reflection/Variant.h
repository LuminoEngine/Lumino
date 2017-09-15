
#pragma once
#include <typeinfo>
#include <type_traits>
#include "../Base/Common.h"

#ifdef LN_LEGACY_VARIANT_ENABLED
#include "../Base/RefObject.h"
#include "../Base/String.h"
#include "../Base/EnumExtension.h"
#include "../Base/StlHelper.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class ReflectionObject;
class ReflectionArrayObject;
struct ReflectionStruct;

enum class VariantType
{
	Null,
	Bool,
	Int32,
	UInt32,
	Float,
	Double,
	String,
	Enum,
	Struct,
	Pointer,
	Object,
	ArrayObject,
};

namespace detail
{
	extern void* enabler;

	class KindPrimitive {};
	class KindArithmetic {};	// 算術型。相互キャスト可能
	class KindEnum {};
	class KindStruct {};
	class KindPointer {};
	class KindReflectionObject {};
	class KindReflectionArrayObject {};
	class KindRefPtr {};
}

/**
	@brief		さまざまなデータ型を格納できる動的型変数です。
	@details	一般的な Variant や any 型と比べて、格納できるデータ型をある程度制限することで高速化を図っています。
				格納できるデータ型のサイズは 32byte までです。

	@section	一般的な使い方
	~~~~~~~~~~~~~~~{.cpp}
	// 格納
	Variant v1 = 10;
	Variant v2 = Point(5, 10);
	Variant v3 = "str";

	// 取得
	int a1 = Variant::cast<int>(v1);
	Point a2 = Variant::cast<Point>(v2);
	String a3 = Variant::cast<String>(v3);
	~~~~~~~~~~~~~~~

	@note		Windows7 64Bit Corei7 - 格納と取得を 1000000 回行った速度比較。
				- Variant		27,625us
				- boost::any	54,852us
*/
class Variant
{
public:
	static const Variant Null;





	template<typename T, typename TKind> struct AccessorSelector {};

	

	// 値型または参照型用の AccessorSelectorHelper
	template<typename T> struct AccessorSelectorHelper
	{
		using typeKind = ::ln::detail::StlHelper::first_enabled_t<
			std::enable_if<std::is_same<T, std::nullptr_t>::value, detail::KindPrimitive>,
			std::enable_if<std::is_same<T, bool>::value, detail::KindPrimitive>,
			std::enable_if<std::is_arithmetic<T>::value, detail::KindArithmetic>,
			std::enable_if<std::is_same<T, String>::value, detail::KindPrimitive>,
			std::enable_if<std::is_base_of<Enum, T>::value, detail::KindEnum>,
			std::enable_if<std::is_enum<T>::value, detail::KindEnum>,
			std::enable_if<std::is_base_of<RefPtrBase, T>::value, detail::KindRefPtr>,
			std::enable_if<std::is_class<std::remove_reference<T>>::value, detail::KindStruct>,		// 非POD構造体 or クラス実体 (remove_reference で、T=const Struct& に備える)
			std::enable_if<std::is_pod<std::remove_reference<T>>::value, detail::KindStruct>,		// POD構造体
			std::false_type>;

		static void setValue(Variant* variant, const T& value)
		{
			AccessorSelector<T, typeKind>::setValue(variant, value);
		}
		static T getValue(const Variant& value)
		{
			static_assert(std::is_same<T, const String&>::value == false, "Reference string type is not supported.");
			return AccessorSelector<T, typeKind>::getValue(&value);
		}
	};
	// ポインタ型用の AccessorSelectorHelper
	template<typename T> struct AccessorSelectorHelper<T*>
	{
		using typeKind = ::ln::detail::StlHelper::first_enabled_t<
			std::enable_if<std::is_base_of<ReflectionArrayObject, T>::value, detail::KindReflectionArrayObject>,
			std::enable_if<std::is_base_of<ReflectionObject, T>::value, detail::KindReflectionObject>,
			std::false_type>;

		static void setValue(Variant* variant, T* value)
		{
			AccessorSelector<T*, typeKind>::setValue(variant, value);
		}
		static T* getValue(const Variant& value)
		{
			return AccessorSelector<T*, typeKind>::getValue(&value);
		}
	};
	/* ↑struct 型の判別に is_pod を使用しているが、is_pod はポインタ型も true とみなしてしまうため、前段で分ける必要がある。
	 * また、ポインタはその実体の型に対して is_base_of したいので、これもまた前段で分ける必要がある。
	 *
	 * 文字列リテラルはポインタ型ではなく配列型となる。
	 * コレだけに限って特殊化するのも面倒なので、コンストラクタオーバーロードで解決している。
	 *
	 * 値側は remove_reference で、T=const Struct& に備えている。
	 * VisualC++ ではコレ相当のことを勝手にやってくれるが、GCC では自分で外さなければならない。
	 *
	 * ポインタ型用の AccessorSelectorHelper は必ず必要になる。
	 * ReflectionObject と ReflectionArrayObject だけであればコンストラクタオーバーロードで解決できるが、
	 * それらの派生クラスのポインタを渡した場合はテンプレートコンストラクタの方が呼ばれる。
	 * このときは特殊化しないとダメ。
	 */



public:
	Variant();
	Variant(const Variant& value) : Variant() { copy(value); }
	//Variant(std::nullptr_t value);
	//Variant(bool value) : Variant() { SetBool(value); }
	//Variant(int32_t value) : Variant() {}
	//Variant(float value);
	Variant(const Char* value) : Variant() { setString(value); }
	//Variant(const String& value);
	//Variant(const Enum& value) : Variant() { setEnumValue(value.GetValue()); }
	////Variant(const ReflectionStruct& value) : Variant() { }
	//Variant(ReflectionObject* value) : Variant() { setReflectionObject(value); }
	//Variant(ReflectionArrayObject* value) : Variant() { setReflectionArrayObject(value); }

	~Variant() { release(); }
	Variant& operator = (const Variant& obj) { copy(obj); return (*this); }





	//template<typename T, typename std::enable_if<std::is_pointer<T>::value>::type*& = detail::enabler>
	//Variant(const T& value)
	//{
	//	//using typeKind = first_enabled_t<
	//	//	std::enable_if<std::is_enum<T>::value, detail::KindEnum>,
	//	//	std::enable_if<std::is_pointer<T>::value, std::false_type>,
	//	//	detail::KindStruct>;
	//	//PodSetterSelector<T, typeKind>::setValue(this, value);
	//}

	//// enum メンバ or struct
	//template<typename T, typename std::enable_if<std::is_pod<T>::value>::type*& = detail::enabler>
	//Variant(const T& value)
	//{
	//	using typeKind = first_enabled_t<
	//		std::enable_if<std::is_enum<T>::value, detail::KindEnum>,
	//		std::enable_if<std::is_pointer<T>::value, std::false_type>,
	//		detail::KindStruct>;
	//	PodSetterSelector<T, typeKind>::setValue(this, value);
	//}

	template<typename T>
	Variant(const Ref<T>& value)
		: Variant()
	{
		AccessorSelectorHelper<T*>::setValue(this, value.get());
	}

	template<typename T>
	Variant(const T& value)
		: Variant()
	{
		AccessorSelectorHelper<T>::setValue(this, value);
	}


	//template<typename T, typename std::enable_if<std::is_enum<T>::value>::type*& = detail::enabler>
	//Variant(T value) { setEnumValue(value); }	// T が enum メンバの場合はこのコンストラクタが呼ばれる。


	//template<typename T, typename std::enable_if<std::is_base_of<T, ReflectionStruct>::value>::type*& = detail::enabler>
	//Variant(const T& value) { }

	//template<typename T, typename std::enable_if<std::is_pod<T>::value>::type*& = detail::enabler>
	//Variant(T value) { }	// T が POD 型の場合はこのコンストラクタが呼ばれる。


	//template<typename T, EnumSubClass>
	//Variant(Ref<T>& obj)
	//{
	//}

	//template<class T>
	//Variant(Ref<T>& obj);

public:
	VariantType getType() const { return m_type; }



	/**
		@brief		指定した Variant の値を指定した型にキャストする。
		@code
					UIElement* item = Variant::cast<UIElement*>(value);
		@endcode
	*/
	template<typename T>
	static T cast(const Variant& value)
	{
		return AccessorSelectorHelper<T>::getValue(value);
	}

private:
	void setNullPtr(std::nullptr_t value);
	std::nullptr_t getNullPtr() const;
	void setBool(bool value);
	bool getBool() const;
	void setArithmetic(int32_t value);
	void setArithmetic(uint32_t value);
	void setArithmetic(float value);
	void setArithmetic(double value);
	void setString(const Char* value);
	void setString(const String& value);
	String getString() const;
	void setEnumValue(EnumValueType value);
	EnumValueType getEnumValue() const;
	void setStruct(const void* value, size_t size, const std::type_info& typeInfo);
	const void* getStruct() const;
	void setReflectionObject(ReflectionObject* obj);
	ReflectionObject* getReflectionObject() const;
	void setReflectionArrayObject(ReflectionArrayObject* obj);
	ReflectionArrayObject* getReflectionArrayObject() const;

	template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type*& = detail::enabler>
	T getArithmetic() const
	{
		switch (m_type)
		{
		case VariantType::Int32: return static_cast<T>(m_int32);
		case VariantType::UInt32: return static_cast<T>(m_uint32);
		case VariantType::Float: return static_cast<T>(m_float);
		case VariantType::Double: return static_cast<T>(m_double);
		default: return 0;
		}
	}

	//void setValue(bool value) { SetBool(value); }
	//void setValue(ReflectionObject* value) { setReflectionObject(value); }
	//void setValue(ReflectionArrayObject* value) { setReflectionArrayObject(value); }

	

	//template<typename T> struct CastValueOrPointerSelector
	//{
	//	static T GetValue(const Variant& value)
	//	{
	//		using typeKind = first_enabled_t<
	//			std::enable_if<std::is_same<T, bool>::value, detail::KindPrimitive>,
	//			std::enable_if<std::is_base_of<Enum, T>::value, detail::KindEnum>,
	//			std::enable_if<std::is_enum<T>::value, detail::KindEnum>,
	//			std::enable_if<std::is_pod<T>::value, detail::KindPodStruct>,
	//			std::false_type>;
	//		return AccessorSelector<T, typeKind>::GetValue(&value);
	//	}
	//};
	//template<typename T> struct CastValueOrPointerSelector<T*>
	//{
	//	static T* GetValue(const Variant& value)
	//	{
	//		// cast<T>() の T はポインタ型である場合ここに来る。
	//		// KindReflectionArrayObject または KindReflectionObject のサブクラスであるかを確認する。
	//		using typeKind = first_enabled_t<
	//			std::enable_if<std::is_base_of<ReflectionArrayObject, T>::value, detail::KindReflectionArrayObject>,
	//			std::enable_if<std::is_base_of<ReflectionObject, T>::value, detail::KindReflectionObject>,
	//			std::false_type>;
	//		return AccessorSelector<T*, typeKind>::GetValue(&value);
	//	}
	//};


#if 0
	template<typename T, typename TIsEnum> struct CastSelector { static T getValue(const Variant& v) { return static_cast<T>(v.GetObject()); } };
	//template<typename TRefPtr, typename TIsEnum> struct CastSelector { static TRefPtr GetValue(const Variant& v) { return TRefPtr(static_cast<TRefPtr::PtrType>(v.GetObject())); } };

	template<> struct CastSelector < bool, std::false_type >		{ static bool getValue(const Variant& v) { return v.GetBool(); } };
	template<> struct CastSelector < int, std::false_type >		{ static int getValue(const Variant& v) { return v.getInt(); } };
	template<> struct CastSelector < float, std::false_type >		{ static float getValue(const Variant& v) { return v.GetFloat(); } };
	template<> struct CastSelector < String, std::false_type >		{ static String getValue(const Variant& v) { return v.getString(); } };
	template<> struct CastSelector < Rect, std::false_type >		{ static Rect getValue(const Variant& v) { return v.GetRect(); } };
	template<> struct CastSelector < SizeF, std::false_type >		{ static SizeF getValue(const Variant& v) { return v.GetSizeF(); } };
	template<> struct CastSelector < Thickness, std::false_type > { static const Thickness& getValue(const Variant& v) { return v.GetThicknessF(); } };
	template<typename T> struct CastSelector < T, std::true_type > { static T getValue(const Variant& v) { return *((T*)(&v.m_enum)); } };	// TODO: 型チェック
//	template<typename TRefPtr, typename U> struct CastSelector < TRefPtr<U>, std::true_type > { static TRefPtr<U> GetValue(const Variant& v) { return TRefPtr<U>(static_cast<U>(v.GetObject())); } };	// TODO: 型チェック
#endif

public:
	//bool operator == (const Variant& right) const;

	bool equals(const Variant& obj) const;

private:
	void copy(const Variant& obj);
	void release();

private:
	VariantType	m_type;
	union
	{
		bool					m_bool;
		int32_t					m_int32;
		uint32_t				m_uint32;
		float					m_float;
		double					m_double;
		ln::detail::GenericStringCore<Char>*	m_string;
		EnumValueType			m_enum;
		ReflectionObject*		m_object;
		ReflectionArrayObject*	m_arrayObject;
		byte_t					m_struct[32];
	};
	size_t			m_structSize;
	//String			m_string;
	const std::type_info*		m_typeInfo;
};

//------------------------------------------------------------------------------
template<> struct Variant::AccessorSelector<std::nullptr_t, detail::KindPrimitive>
{
	static void setValue(Variant* v, std::nullptr_t value) { v->setNullPtr(value); }
	static std::nullptr_t getValue(const Variant* v) { return v->getNullPtr(); }
};
template<> struct Variant::AccessorSelector<bool, detail::KindPrimitive>
{
	static void setValue(Variant* v, bool value) { v->setBool(value); }
	static bool getValue(const Variant* v) { return v->getBool(); }
};
template<typename T> struct Variant::AccessorSelector<T, detail::KindArithmetic>
{
	static void setValue(Variant* v, T value) { v->setArithmetic(value); }
	static T getValue(const Variant* v) { return v->getArithmetic<T>(); }
};
template<> struct Variant::AccessorSelector<String, detail::KindPrimitive>
{
	static void setValue(Variant* v, const String& value) { v->setString(value); }
	static String getValue(const Variant* v) { return v->getString(); }		// 戻り値は参照ではダメ
};
template<typename T> struct Variant::AccessorSelector<T, detail::KindEnum>
{
	static void setValue(Variant* v, T value) { v->setEnumValue((int)value); }
	static T getValue(const Variant* v) { return static_cast<typename T::enum_type>(v->getEnumValue()); }
};
template<typename T> struct Variant::AccessorSelector<T, detail::KindReflectionObject>
{
	static void setValue(Variant* v, T value) { v->setReflectionObject(value); }
	static T getValue(const Variant* v) { return static_cast<T>(v->getReflectionObject()); }
};
template<typename T> struct Variant::AccessorSelector<T, detail::KindReflectionArrayObject>
{
	static void setValue(Variant* v, T value) { v->setReflectionArrayObject(value); }
	static T getValue(const Variant* v) { return static_cast<T>(v->getReflectionArrayObject()); }
};
template<typename T> struct Variant::AccessorSelector<T, detail::KindStruct>
{
	static void setValue(Variant* v, T value) { v->setStruct(&value, sizeof(T), typeid(T)); }
	static T getValue(const Variant* v) { return *((T*)v->getStruct()); }
};
template<typename T> struct Variant::AccessorSelector<const T&, detail::KindStruct>	// const Struct& のような参照での get をサポートする
{
	static const T& getValue(const Variant* v) { return *((T*)v->getStruct()); }
};
template<typename T> struct Variant::AccessorSelector<T, detail::KindRefPtr>
{
	static void setValue(Variant* v, T value) { v->setReflectionObject(value); }
	static T getValue(const Variant* v) { return T(static_cast<typename T::PtrType>(v->getReflectionObject())); }
};

} // namespace tr
LN_NAMESPACE_END

#endif
