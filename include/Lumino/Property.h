
#pragma once
#include <map>
#include <memory>

/*
	プロパティの種類は
	- 実体が C++ ネイティブのメンバ変数
	- 実体が外部(C#とか) のメンバ変数	→ 高速化を狙うときに使う。必須ではない
	- 実体が map に登録される Variant
	と、それぞれ
	- 普通のプロパティ
	- 添付プロパティ
*/

namespace Lumino
{

/**
	@brief		
*/
class Property
{
public:
	Property(const Variant& defaultValue, bool stored)
		: m_defaultValue(defaultValue)
		, m_stored(stored)
		, m_registerd(false)
	{}
	~Property() {}

public:
	virtual const String& GetName() const = 0;

	virtual void SetValue(CoreObject* target, Variant value) const = 0;
	virtual Variant GetValue(const CoreObject* target) const = 0;

	virtual bool IsReadable() const { return false; }
	virtual bool IsWritable() const { return false; }

	/// 値を CoreObject の map に Variant として持つかどうか。
	/// (よい名前が思いつかないのでとりあえずこれで)
	/// (virtual にしてもいいが、割とクリティカルなところで呼び出されると思われるのでそうしないでおく)
	bool IsStored() const { return m_stored; }

	const Variant& GetDefaultValue() const { return m_defaultValue; }

private:
	Variant	m_defaultValue;
	bool	m_stored;

	friend class TypeInfo;
	bool	m_registerd;

	// このクラスのインスタンスは基本的に static にする。
	// あくまで「名前をキーにしてどのgetter/setterを呼び出すか？」が目的なので、状態は持たない。
	// 余計な new はせずに共有する。
};


///**
//	@brief		
//*/
//template<typename T>
//class TypedProperty
//{
//public:
//	TypedProperty();
//	~TypedProperty();
//
//public:
//	
//};

/**
	@brief		
*/
template<class TClass, typename TValue, typename TInternalValue = TValue, typename TCast = TValue>
class CoreObjectProperty : public Property
{
public:
	//typedef void (TClass::*Setter)(T);
	//typedef const T& (TClass::*Getter)() const;

	struct SetterFunctor
	{
		typedef void (TClass::*PlainSetter)(TValue value);
		typedef void (TClass::*ConstRefSetter)(const TValue& value);

		SetterFunctor(intptr_t null)
			: m_plainSetter(NULL)
			, m_constRefSetter(NULL)
		{}

		SetterFunctor(PlainSetter setter)
			: m_plainSetter(setter)
			, m_constRefSetter(NULL)
		{}

		SetterFunctor(ConstRefSetter setter)
			: m_plainSetter(NULL)
			, m_constRefSetter(setter)
		{}

		bool IsEmpty() const
		{
			return m_plainSetter == NULL && m_constRefSetter == NULL;
		}

		void Call(TClass* instance, const TValue& value) const
		{
			if (m_plainSetter) {
				(instance->*m_plainSetter)(value);
			}
			if (m_constRefSetter) {
				(instance->*m_constRefSetter)(value);
			}
		}

		PlainSetter		m_plainSetter;
		ConstRefSetter	m_constRefSetter;
	};

	struct GetterFunctor
	{
		template<typename DT> struct EnsurePlain { typedef DT result; };
		template<typename DT> struct EnsurePlain<DT&> { typedef DT result; };
		template<typename DT> struct EnsurePlain<const DT&> { typedef DT result; };

		template<typename DT> struct EnsureConstRef { typedef const DT& result; };
		template<typename DT> struct EnsureConstRef<DT&> { typedef const DT& result; };
		template<typename DT> struct EnsureConstRef<const DT&> { typedef const DT& result; };

		template<typename DT> struct EnsureRef { typedef DT& result; };
		template<typename DT> struct EnsureRef<DT&> { typedef DT& result; };
		template<typename DT> struct EnsureRef<const DT&> { typedef DT& result; };

		typedef typename EnsurePlain<typename TValue>::result(TClass::*PlainGetter)() const;
		typedef typename EnsureConstRef<typename TValue>::result(TClass::*ConstRefGetter)() const;
		typedef typename EnsureRef<typename TValue>::result(TClass::*RefGetter)() const;

		GetterFunctor(PlainGetter getter) :
			d_plainGetter(getter)
			//d_constRefGetter(0), no need to initialise these, we will never use them
			//d_refGetter(0)
		{}

		GetterFunctor(ConstRefGetter getter) :
			d_plainGetter(0),
			d_constRefGetter(getter)
			//d_refGetter(0) // no need to initialise this, we will never use it
		{}

		GetterFunctor(RefGetter getter) :
			d_plainGetter(0),
			d_constRefGetter(0),
			d_refGetter(getter)
		{}
		// to set 0 as func
		GetterFunctor(int /*val*/) :
			d_plainGetter(0),
			d_constRefGetter(0),
			d_refGetter(0)
		{}
		operator bool(void) const
		{
			return d_plainGetter || d_constRefGetter || d_refGetter;
		}
		//typename const T& /*Helper::safe_method_return_type*/ operator()(const TClass* instance) const
		typename TValue Call(const TClass* instance) const	// ※実体
		{
			// FIXME: Ideally we want this to be done during compilation, not runtime

			if (d_plainGetter)
				return (instance->*d_plainGetter)();
			if (d_constRefGetter)
				return (instance->*d_constRefGetter)();
			if (d_refGetter)
				return (instance->*d_refGetter)();

			assert(false);
			return (instance->*d_plainGetter)();
		}

		PlainGetter d_plainGetter;
		ConstRefGetter d_constRefGetter;
		RefGetter d_refGetter;
	};



public:
	CoreObjectProperty(const String& name, SetterFunctor setter, GetterFunctor getter, TValue defaultValue)
		: Property( defaultValue, false)
		, m_name(name)
		, m_setter(setter)
		, m_getter(getter)
	{}

	virtual ~CoreObjectProperty() {}

	virtual const String& GetName() const { return m_name; }

	virtual void SetValue(CoreObject* target, Variant value) const
	{
		LN_THROW(!m_setter.IsEmpty(), InvalidOperationException);
		TClass* instance = static_cast<TClass*>(target);
		m_setter.Call(instance, static_cast<TCast>(value.Cast<TInternalValue>()));
		//(instance->*m_setter)(value.Cast<T>());
	}
	virtual Variant GetValue(const CoreObject* target) const
	{
		LN_THROW((m_getter != NULL), InvalidOperationException);
		const TClass* instance = static_cast<const TClass*>(target);
		return Variant(m_getter.Call(instance));
		//const TClass* instance = static_cast<const TClass*>(target);
		//return Variant((instance->*m_getter)());
	}
	virtual bool IsReadable() const { return m_getter != NULL; }
	virtual bool IsWritable() const { return !m_setter.IsEmpty(); }

private:
	String	m_name;
	SetterFunctor	m_setter;
	GetterFunctor	m_getter;
	//Getter	m_getter;
};

// 削除予定
#define LN_DEFINE_PROPERTY(classType, nativeType, name, setterFuncPtr, getterFuncPtr, defaultValue) \
{ \
	static ::Lumino::CoreObjectProperty<classType, nativeType> prop( \
		name, setterFuncPtr, getterFuncPtr, defaultValue); \
	classType::GetClassTypeInfo()->RegisterProperty(&prop); \
}

// 削除予定
#define LN_DEFINE_PROPERTY_ENUM(classType, nativeType, name, setterFuncPtr, getterFuncPtr, defaultValue) \
{ \
	static ::Lumino::CoreObjectProperty<classType, nativeType, int, nativeType::enum_type> prop( \
		name, setterFuncPtr, getterFuncPtr, defaultValue); \
	classType::GetClassTypeInfo()->RegisterProperty(&prop); \
}

// static としてグローバルスコープに定義したプロパティを登録する機能を持ったプロパティ定義ユーティリティ
template<class TOwnerClass, typename TValue, typename TInternalValue = TValue, typename TCast = TValue>
struct StaticProperty
	: public CoreObjectProperty<TOwnerClass, TValue, TInternalValue, TCast>
{
	StaticProperty(const String& name, SetterFunctor setter, GetterFunctor getter, TValue defaultValue)
		: CoreObjectProperty<TOwnerClass, TValue, TInternalValue, TCast>(name, setter, getter, defaultValue)
	{
		TOwnerClass::GetClassTypeInfo()->RegisterProperty(this);
	}
};

#define LN_DEFINE_PROPERTY_2(ownerClass, valueType, var, name, defaultValue, setter, getter) \
	static StaticProperty<ownerClass, valueType> _##var(_T(name), setter, getter, defaultValue); \
	const Property* ownerClass::var = &_##var;

#define LN_DEFINE_PROPERTY_ENUM_2(ownerClass, valueType, var, name, defaultValue, setter, getter) \
	static StaticProperty<ownerClass, valueType, int, valueType::enum_type> _##var(_T(name), setter, getter, defaultValue); \
	const Property* ownerClass::var = &_##var;




class AttachedProperty
	: public Property
{
public:
	AttachedProperty(const String& name, const Variant& defaultValue)
		: Property(defaultValue, true)
		, m_name(name)
		, m_defaultValue(defaultValue)
	{}

	virtual const String& GetName() const { return m_name; }
	virtual void SetValue(CoreObject* target, Variant value) const { LN_THROW(0, InvalidOperationException); }
	virtual Variant GetValue(const CoreObject* target) const  { LN_THROW(0, InvalidOperationException); }

private:
	String	m_name;
	Variant	m_defaultValue;
};


class PropertyManager
{
public:
	static AttachedProperty* RegisterAttachedProperty(TypeInfo* ownerClass, const String& propertyName, const Variant& defaultValue);

private:
	class TypedNameKey
	{
	public:
		TypedNameKey(TypeInfo* type, const String& propertyName)
			: m_type(type)
			, m_propertyName(propertyName)
		{}

		bool operator < (const TypedNameKey& key) const
		{
			if ((*m_type) < (*key.m_type)) return true;
			if (m_propertyName < key.m_propertyName) return true;
			return false;
		}

	private:
		TypeInfo*	m_type;
		String		m_propertyName;
	};

	typedef std::map<TypedNameKey, std::shared_ptr<AttachedProperty> >	PropertyMap;	//TODO: map だと重くない？
	static PropertyMap	m_propertyMap;
};


#define LN_DEFINE_ATTACHED_PROPERTY(prop, name, nativeType, ownerClassType, defaultValue) \
{ \
	if (prop == NULL) { \
		prop = PropertyManager::RegisterAttachedProperty(ownerClassType::GetClassTypeInfo(), _T(name), defaultValue); \
	} \
}

/// GUI 用 Set ユーティリティ
#define LN_SET_ATTACHED_PROPERTY(element, prop, value) \
	LN_VERIFY(element != NULL); \
	element->SetPropertyValue(prop, value);

/// GUI 用 Get ユーティリティ
#define LN_GET_ATTACHED_PROPERTY(element, prop, type) \
	LN_VERIFY(element != NULL); \
	return element->GetPropertyValue(prop).Cast<type>();

} // namespace Lumino
