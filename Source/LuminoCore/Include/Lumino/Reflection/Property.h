
#pragma once
#include <memory>
#include "Common.h"
#include "../Base/RefObject.h"
#include "../Base/String.h"
#include "../Base/Hash.h"
#include "PropertyMetadata.h"
#include "ReflectionObject.h"

#define LN_MEMBER_OFFSETOF(type, member)  \
    (reinterpret_cast<std::size_t>(  \
      &reinterpret_cast<char const volatile&>(  \
        ((type*)0)->member)))

LN_NAMESPACE_BEGIN
namespace tr
{
class ReflectionObject;
class IPropertyChangedListener;
class PropertyBase;
template<typename TValue> class Property;
template<typename TValue> class TypedPropertyInfo;
template<typename TValue> class WeakRefPtr;
template<typename TValue> class PropertyRef;
template<typename TValue> class TypedPropertyInitializer;


/**
	@brief		ReflectionObject のサブクラスが実装できるプロパティを表します。
	@details	プロパティは、メンバ変数に対する簡易的なリフレクションを提供します。		
*/
class PropertyInfo
{
public:
	PropertyInfo(TypeInfo* ownerClassType, PropertyMetadata* metadata, bool stored);
	virtual ~PropertyInfo();

	virtual void SetValue(ReflectionObject* target, Variant value, PropertySetSource source) const { LN_THROW(0, InvalidOperationException); }
	virtual Variant GetValue(const ReflectionObject* target) const { LN_THROW(0, InvalidOperationException); }
	virtual void AddItem(ReflectionObject* target, const Variant& value) const { LN_THROW(0, InvalidOperationException); }

	virtual bool IsReadable() const { return false; }
	virtual bool IsWritable() const { return false; }
	virtual bool IsList() const { return false; }

	TypeInfo* GetOwnerClassType() const { return m_ownerClassType; }
	bool IsStored() const { return m_stored; }

	virtual PropertyBase* GetPropertyBase(ReflectionObject* obj) const = 0;

	const PropertyMetadata& GetMetadata() const { return m_metadata; }

public:

	/**
		@brief		指定したオブジェクトのプロパティの値を設定します。
	*/
	static void SetPropertyValue(ReflectionObject* obj, const PropertyInfo* prop, const Variant& value, PropertySetSource source = PropertySetSource::ByLocal);
	
	/**
		@brief		指定したオブジェクトのプロパティの値を取得します。
	*/
	static Variant GetPropertyValue(ReflectionObject* obj, const PropertyInfo* prop);

	/**
		@brief		指定したオブジェクトのプロパティの値を設定します。あらかじめ型が分かっている場合、SetPropertyValue() よりも少ないオーバーヘッドで設定できます。
	*/
	template<typename TValue>
	static void SetPropertyValueDirect(ReflectionObject* obj, const PropertyInfo* prop, const TValue& value, PropertySetSource source = PropertySetSource::ByLocal);

	/**
		@brief		指定したオブジェクトのプロパティの値を取得します。あらかじめ型が分かっている場合、GetPropertyValue() よりも少ないオーバーヘッドで設定できます。
	*/
	template<typename TValue>
	static const TValue& GetPropertyValueDirect(const ReflectionObject* obj, const PropertyInfo* prop);

	template<typename TValue>
	static PropertyRef<TValue> GetProperty(ReflectionObject* obj, const TypedPropertyInfo<TValue>* prop);

	static void NotifyPropertyChanged(ReflectionObject* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source);



	template<typename TValue>
	static void SetPropertyValueDirect(tr::PropertyRef<TValue>& propRef, const TValue& value, PropertySetSource source = PropertySetSource::ByLocal);




private:
	friend class TypeInfo;
	template<typename TValue> friend class TypedPropertyInitializer;
	TypeInfo*			m_ownerClassType;
	PropertyMetadata	m_metadata;
	size_t				m_memberOffset;
	bool				m_stored;
	bool				m_registerd;
};





template<typename TValue>
class TypedPropertyInfo
	: public PropertyInfo
{
public:
	typedef Property<TValue>*(*GetPrpertyPtrFunc)(ReflectionObject* obj);
	typedef void(*SetterFunc)(ReflectionObject* obj, TValue& value);
	typedef void(*GetterFunc)(ReflectionObject* obj, TValue** valuePtr);
	typedef void(*OnPropertyChangedFunc)(ReflectionObject* obj, PropertyChangedEventArgs* e);
	// ↑※static 関数のポインタでないと、言語バインダを作りにくくなる。

public:
	TypedPropertyInfo(TypeInfo* ownerTypeInfo, const TCHAR* name, PropertyMetadata* metadata)
		: PropertyInfo(ownerTypeInfo, metadata, false)
		, m_name(name)
		, m_setter(nullptr)
		, m_getter(nullptr)
	{}

	virtual ~TypedPropertyInfo() {}

public:
	virtual const String& GetName() const { return m_name; }

	virtual void SetValue(ReflectionObject* target, Variant value, PropertySetSource source) const
	{
		SetValueDirect(target, Variant::Cast<TValue>(value), source);
	}
	virtual Variant GetValue(const ReflectionObject* target) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		TValue* v;
		m_getter(const_cast<ReflectionObject*>(target), &v);
		return Variant(*v);
	}
	//virtual void AddItem(ReflectionObject* target, const Variant& value) const
	//{
	//	LN_THROW(m_getter != NULL, InvalidOperationException);
	//	//VariantList* list = m_getter(target);
	//	// TODO: 間違えて UIElementFractory のまま追加してしまうことがあった。型チェックできると良い。
	//	//list->AddVariant(value);
	//	auto list = m_getter(target);
	//	AddItemInternal(list, value);
	//}
	virtual bool IsReadable() const { return m_getter != nullptr; }	// TODO: virtual やめたほうが高速化できる。
	virtual bool IsWritable() const { return m_getter != nullptr; }

	void SetValueDirect(ReflectionObject* target, const TValue& value, PropertySetSource source) const
	{
		if (LN_CHECK_STATE(m_setter != nullptr)) return;
		m_setter(target, const_cast<TValue&>(value));
		//NotifyPropertyChanged(target, this, source);
	}
	const TValue& GetValueDirect(const ReflectionObject* target) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		TValue* v;
		m_getter(const_cast<ReflectionObject*>(target), &v);
		return *v;
	}

	virtual PropertyBase* GetPropertyBase(ReflectionObject* obj) const override { return m_getPropPtr(obj); }

	template<typename T, typename TIsList> struct ListOperationSelector
	{
		static bool IsList() { return false; }
		static void AddItem(T& list, const Variant& item) { LN_THROW(0, InvalidOperationException); }
	};
	template<typename T> struct ListOperationSelector<T, std::true_type>
	{
		static bool IsList() { return true; }
		static void AddItem(T& list, const Variant& item) { list.AddVariant(item); }
	};


	//template<typename T> struct ListOperationSelector2
	//{
	//	static bool IsList() { return false; }
	//};
	//template<typename T> struct ListOperationSelector2<T*>
	//{
	//	static bool IsList() { return ListOperationSelector<T, std::is_base_of<ReflectionObjectList, T>::type>::IsList(); }
	//};

	//virtual void NotifyPropertyChange(ReflectionObject* target, PropertyChangedEventArgs* e) const
	//{
	//	if (m_propChanged != NULL) {
	//		m_propChanged(target, e);
	//	}
	//}

public:	// TODO
	template<typename T> friend class TypedPropertyInitializer;
	String					m_name;
	GetPrpertyPtrFunc		m_getPropPtr;
	SetterFunc				m_setter;
	GetterFunc				m_getter;
};


// TODO; Internal
template<typename TValue>
class TypedPropertyInitializer
{
	// Initializer は .h 側に不必要な型を書きたくないから用意したもの。
	// 現在 TypedProperty はメンバ static ではなく、グローバル static を前提としている。
	// となると、private メンバには当然アクセスできないが、このクラス経由で各メンバへの参照経路を設定する。

	// ・LN_PROPERTY を書く宣言(.h)側に、TypedProperty<> を公開したくない。基底のポインタ Property* として公開したい。
	//		→	TypedProperty<> 以外のプロパティを作りたいとき、マクロ名を変えないとならなくなる。
	//			「宣言は LN_PROPERTY だけ書けばOK」にして、考えることは極力減らしたい。
	// ・とすると、TypedProperty<> の実体はメンバではなくグローバル変数にしなければならない。
	//		→	マクロで生成される各種アクセサにアクセスできなくなる。

public:
	typedef Property<TValue>*(*GetPrpertyPtrFunc)(ReflectionObject* obj);
	typedef void(*SetterFunc)(ReflectionObject* obj, TValue& value);
	typedef void(*GetterFunc)(ReflectionObject* obj, TValue** outValuePtr);

	//template<typename TPropertyChangedCallbackCaster>
	TypedPropertyInitializer(
		TypedPropertyInfo<TValue>* prop,
		size_t memberOffset,
		GetPrpertyPtrFunc getPropPtr,
		SetterFunc setter,
		GetterFunc getter,
		const PropertyMetadata& metadata)
	{
		prop->m_getPropPtr = getPropPtr;
		prop->m_memberOffset = memberOffset;
		prop->m_setter = setter;
		prop->m_getter = getter;
		prop->m_metadata = metadata;
	}
};



//------------------------------------------------------------------------------
template<typename TValue>
void PropertyInfo::SetPropertyValueDirect(ReflectionObject* obj, const PropertyInfo* prop, const TValue& value, PropertySetSource source)
{
	if (LN_CHECK_ARG(obj != nullptr)) return;
	if (LN_CHECK_ARG(prop != nullptr)) return;
	auto t = static_cast<const TypedPropertyInfo<TValue>*>(prop);
	t->SetValueDirect(obj, value, source);
}

//------------------------------------------------------------------------------
template<typename TValue>
const TValue& PropertyInfo::GetPropertyValueDirect(const ReflectionObject* obj, const PropertyInfo* prop)
{
	LN_VERIFY_ARG(obj != nullptr);
	LN_VERIFY_ARG(prop != nullptr);
	auto t = static_cast<const TypedPropertyInfo<TValue>*>(prop);
	return t->GetValueDirect(obj);
}


class PropertyHelper
{
public:

	template<typename TValue, typename TProperty>
	static void SetValue(TProperty& field, TValue& value)
	{
		field.Set(value);
	}
	template<typename TValue, typename TProperty>
	static void GetValue(TProperty& field, TValue** outValuePtr)
	{
		*outValuePtr = &field.m_value;
	}

	template<class T, class C>
	static LN_CONSTEXPR std::size_t MemberOffsetOf(T(C::*pm))
	{
		return reinterpret_cast<std::size_t>(
			&reinterpret_cast<const volatile char&>(((C*)0)->*pm)
			);
	}
};




#define LN_TR_PROPERTY(valueType, propVar) \
	private: static ::ln::tr::Property<valueType>*					get_Ptr##propVar(ln::tr::ReflectionObject* obj); \
	private: static void											set_##propVar(ln::tr::ReflectionObject* obj, valueType& value); \
	private: static void											get_##propVar(ln::tr::ReflectionObject* obj, valueType** outValuePtr); \
	private: static ln::tr::TypedPropertyInitializer<valueType>		init_##propVar; \
	public:  static const ln::tr::TypedPropertyInfo<valueType>*		propVar##Id;

#define LN_TR_PROPERTY_IMPLEMENT(ownerClass, valueType, propVar, metadata) \
	static ln::tr::TypedPropertyInfo<valueType>		_##propVar##Id(ln::tr::TypeInfo::GetTypeInfo<ownerClass>(), _T(#propVar), nullptr); \
	const ln::tr::TypedPropertyInfo<valueType>*		ownerClass::propVar##Id = &_##propVar##Id; \
	::ln::tr::Property<valueType>*					ownerClass::get_Ptr##propVar(ln::tr::ReflectionObject* obj) { return &static_cast<ownerClass*>(obj)->propVar; } \
	void											ownerClass::set_##propVar(ln::tr::ReflectionObject* obj, valueType& value) { tr::PropertyHelper::SetValue(static_cast<ownerClass*>(obj)->propVar, value); } \
	void											ownerClass::get_##propVar(ln::tr::ReflectionObject* obj, valueType** outValuePtr) { tr::PropertyHelper::GetValue<valueType>(static_cast<ownerClass*>(obj)->propVar, outValuePtr); } \
	ln::tr::TypedPropertyInitializer<valueType>		ownerClass::init_##propVar(&_##propVar##Id, LN_MEMBER_OFFSETOF(ownerClass, propVar), &ownerClass::get_Ptr##propVar, &ownerClass::set_##propVar, &ownerClass::get_##propVar, metadata); \





//class IPropertyChangedListener
//{
//protected:
//	virtual ~IPropertyChangedListener() = default;
//
//	virtual void OnPropertyChanged(PropertyChangedEventArgs* e) = 0;
//
//	friend class PropertyBase;
//};

class PropertyBase
{
public:
	PropertyBase(/*ReflectionObject* owner*/)
		: m_owner(nullptr)
		, m_propId(nullptr)
		, m_staticListenerOwner(nullptr)
		, m_staticListener(nullptr)
	{
	}

	ReflectionObject* GetOwnerObject() const { return m_owner; }
	//void AddListener(IPropertyChangedListener* listener) { m_listeners.Add(listener); }
	//void RemoveListener(IPropertyChangedListener* listener) { m_listeners.Remove(listener); }
	//void CallListener(PropertyChangedEventArgs* e) const;

	// TODO: internal
	typedef void(*OnPropertyChangedFunc)(ReflectionObject* obj, PropertyChangedEventArgs* e);
	void SetStaticListener(ReflectionObject* owner, OnPropertyChangedFunc callback)
	{
		m_staticListenerOwner = owner;
		m_staticListener = callback;
	}

public:	// TODO
	
	List<IPropertyChangedListener*>	m_listeners;
	
	// set from TypeInfo::InitializeProperties
	ReflectionObject*		m_owner;
	const PropertyInfo*		m_propId;

	ReflectionObject*		m_staticListenerOwner;
	OnPropertyChangedFunc	m_staticListener;
};






template<typename TValue>
class Property
	: public PropertyBase
{
public:
	Property(/*ReflectionObject* owner*/)
		: PropertyBase(/*owner*/)
		, m_defaultValue()
		, m_value()
		, m_valueSource(PropertySetSource::Default)
	{
	}

	Property(/*ReflectionObject* owner, */const TValue& value)
		: PropertyBase(/*owner*/)
		, m_defaultValue(value)
		, m_value(value)
		, m_valueSource(PropertySetSource::Default)
	{}

	~Property()
	{}

	void Set(const TValue& value)
	{
		SetInternal(value, PropertySetSource::ByLocal);
	}

	const TValue& Get() const { return m_value; }



	/** プロパティのローカル値を消去します。*/
	void ClearValue()
	{
		if (m_value != m_defaultValue || m_valueSource != PropertySetSource::Default)
		{
			m_value = m_defaultValue;
			m_valueSource = PropertySetSource::Default;
			PropertyInfo::NotifyPropertyChanged(GetOwnerObject(), this, m_propId, m_valueSource);
		}
	}

	Property& operator = (const TValue& value) { Set(value); return *this; }
	operator const TValue&() const { return m_value; }

	const PropertyInfo* GetPropertyInfo() const { return m_propId; }

LN_INTERNAL_ACCESS:
	uint32_t GetHashCode() const
	{
		return
			Hash::CalcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value)) +
			Hash::CalcHash(reinterpret_cast<const char*>(&m_valueSource), sizeof(m_valueSource));
	}

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Property);

	void SetInternal(const TValue& value, PropertySetSource source)
	{
		if (m_value != value || m_valueSource != source)
		{
			m_value = value;
			m_valueSource = source;
			PropertyInfo::NotifyPropertyChanged(GetOwnerObject(), this, m_propId, m_valueSource);
		}
	}

	TValue				m_defaultValue;
	TValue				m_value;
	PropertySetSource	m_valueSource;

	friend class PropertyHelper;
	friend class PropertyInfo;
};


template<typename TValue>
inline bool operator == (const Property<TValue>& lhs, const TValue& rhs)
{
	return lhs.Get() == rhs;
}
template<typename TValue>
inline bool operator == (const TValue& lhs, const Property<TValue>& rhs)
{
	return lhs == rhs.Get();
}
template<typename TValue>
inline bool operator != (const Property<TValue>& lhs, const TValue& rhs)
{
	return lhs.Get() != rhs;
}
template<typename TValue>
inline bool operator != (const TValue& lhs, const Property<TValue>& rhs)
{
	return lhs != rhs.Get();
}







template<typename TValue>
class PropertyRef
{
public:
	PropertyRef(ReflectionObject* propOwner, const PropertyInfo* propInfo)
		: PropertyRef(propOwner, *static_cast<const TypedPropertyInfo<TValue>*>(propInfo)->m_getPropPtr(propOwner))
	{
	}

	PropertyRef(ReflectionObject* propOwner, Property<TValue>& prop)
		: m_propOwner(propOwner)
		, m_prop(prop)
	{
	}

	void SetValue(const TValue& value)
	{
		auto ptr = m_propOwner.Resolve();
		if (ptr != nullptr) m_prop.Set(value);
	}

	const TValue& GetValue(const TValue& defaultValue) const
	{
		auto ptr = m_propOwner.Resolve();
		if (ptr != nullptr) return m_prop.Get();
		return defaultValue;
	}

	void ClearValue()
	{
		auto ptr = m_propOwner.Resolve();
		if (ptr != nullptr) return m_prop.ClearValue();
	}

	RefPtr<ReflectionObject> GetOwenr()
	{
		return m_propOwner.Resolve();
	}

	const PropertyInfo* GetPropertyInfo() const
	{
		return m_prop.GetPropertyInfo();
	}

	Property<TValue>* GetProperty()
	{
		return &m_prop;
	}

private:
	WeakRefPtr<ReflectionObject>	m_propOwner;
	Property<TValue>&				m_prop;
};




//------------------------------------------------------------------------------
template<typename TValue>
PropertyRef<TValue> PropertyInfo::GetProperty(ReflectionObject* obj, const TypedPropertyInfo<TValue>* prop)
{
	return tr::PropertyRef<TValue>(obj, prop);
}

//------------------------------------------------------------------------------
template<typename TValue>
void PropertyInfo::SetPropertyValueDirect(tr::PropertyRef<TValue>& propRef, const TValue& value, PropertySetSource source)
{
	propRef.GetProperty()->SetInternal(value, source);
}


} // namespace tr
LN_NAMESPACE_END


