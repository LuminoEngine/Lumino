
#pragma once
#include <map>
#include <memory>
#include <functional>

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
	Property(TypeInfo* ownerClassType, const Variant& defaultValue, bool stored)
		: m_ownerClassType(ownerClassType)
		, m_defaultValue(defaultValue)
		, m_stored(stored)
		, m_registerd(false)
	{}
	~Property() {}

public:
	virtual const String& GetName() const = 0;

	virtual void SetValue(CoreObject* target, Variant value) const { LN_THROW(0, InvalidOperationException); }
	virtual Variant GetValue(const CoreObject* target) const { LN_THROW(0, InvalidOperationException); }
	virtual void AddItem(CoreObject* target, const Variant& value) const { LN_THROW(0, InvalidOperationException); }

	virtual bool IsReadable() const { return false; }
	virtual bool IsWritable() const { return false; }
	virtual bool IsList() const { return false; }

	/// 値を CoreObject の map に Variant として持つかどうか。
	/// (よい名前が思いつかないのでとりあえずこれで)
	/// (virtual にしてもいいが、割とクリティカルなところで呼び出されると思われるのでそうしないでおく)
	bool IsStored() const { return m_stored; }

	TypeInfo* GetOwnerClassType() const { return m_ownerClassType; }

	const Variant& GetDefaultValue() const { return m_defaultValue; }

	int GetLocalIndex() const { return m_localIndex; }

private:
	TypeInfo*	m_ownerClassType;
	Variant	m_defaultValue;
	bool	m_stored;

	friend class TypeInfo;
	bool	m_registerd;
	int		m_localIndex;

	// このクラスのインスタンスは基本的に static にする。
	// あくまで「名前をキーにしてどのgetter/setterを呼び出すか？」が目的なので、状態は持たない。
	// 余計な new はせずに共有する。
};

/* Note:
		
		メンバ変数へのアクセスは関数ポインタ経由で行う。
		メンバ変数のポインタを返すことでもアクセスは可能だが、言語バインダを作り辛くなる。
*/


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
		: Property(TClass::GetClassTypeInfo(), defaultValue, false)
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
		m_setter.Call(instance, static_cast<TCast>(Variant::Cast<TInternalValue>(value)));
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

/// リスト型のプロパティ。
/// 非 const な リストポインタを返す getter のみあれば良い。
template<class TOwnerClass, typename TList, typename TItem>
class ListProperty : public Property
{
public:
	typedef std::function< TList*(TOwnerClass*) >	GetterFunctor;

public:
	ListProperty(const String& name, GetterFunctor getter)
		: Property(TOwnerClass::GetClassTypeInfo(), Variant::Null, false)
		, m_name(name)
		, m_getter(getter)
	{
		TOwnerClass::GetClassTypeInfo()->RegisterProperty(this);
	}

	virtual ~ListProperty()
	{}

	virtual const String& GetName() const { return m_name; }


	virtual void SetValue(CoreObject* target, Variant value) const
	{
		LN_VERIFY_RETURN(value.GetType() == VariantType_List);
		TOwnerClass* instance = static_cast<TOwnerClass*>(target);
		TList* list = m_getter(instance);

		list->Clear();

		VariantList* srcList = value.GetList();
		for (Variant& v : *srcList) {
			// TODO: 間違えて UIElementFractory のまま追加してしまうことがあった。型チェックできると良い。
			list->Add(Variant::Cast<TItem*>(value));
		}
	}

	virtual void AddItem(CoreObject* target, const Variant& value) const
	{
		TOwnerClass* instance = static_cast<TOwnerClass*>(target);
		TList* list = m_getter(instance);
		// TODO: 間違えて UIElementFractory のまま追加してしまうことがあった。型チェックできると良い。
		list->Add(Variant::Cast<TItem*>(value));
	}

	virtual bool IsList() const { return true; }

private:
	String			m_name;
	GetterFunctor	m_getter;
};

//// 削除予定
//#define LN_DEFINE_PROPERTY(classType, nativeType, name, setterFuncPtr, getterFuncPtr, defaultValue) \
//{ \
//	static ::Lumino::CoreObjectProperty<classType, nativeType> prop( \
//		name, setterFuncPtr, getterFuncPtr, defaultValue); \
//	classType::GetClassTypeInfo()->RegisterProperty(&prop); \
//}
//
//// 削除予定
//#define LN_DEFINE_PROPERTY_ENUM(classType, nativeType, name, setterFuncPtr, getterFuncPtr, defaultValue) \
//{ \
//	static ::Lumino::CoreObjectProperty<classType, nativeType, int, nativeType::enum_type> prop( \
//		name, setterFuncPtr, getterFuncPtr, defaultValue); \
//	classType::GetClassTypeInfo()->RegisterProperty(&prop); \
//}

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

#define LN_DEFINE_PROPERTY_LIST(ownerClass, listType, itemType, var, name, getter) \
	static ListProperty<ownerClass, listType, itemType> _##var(_T(name), getter); \
	const Property* ownerClass::var = &_##var;










template<typename TValue>
class TypedProperty
	: public Property
{
public:
	typedef void(*SetterFunc)(CoreObject* obj, const TValue& value);
	typedef const TValue&(*GetterFunc)(const CoreObject* obj);

public:
	TypedProperty(TypeInfo* ownerTypeInfo, const TCHAR* name, TValue defaultValue)
		: Property(ownerTypeInfo, defaultValue, false)
		, m_name(name)
		, m_setter(NULL)
		, m_getter(NULL)
	{}

	virtual ~TypedProperty() {}

public:
	virtual const String& GetName() const { return m_name; }

	virtual void SetValue(CoreObject* target, Variant value) const
	{
		LN_THROW(m_setter != NULL, InvalidOperationException);
		m_setter(target, Variant::Cast<TValue>(value));
	}
	virtual Variant GetValue(const CoreObject* target) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		return Variant(m_getter(target));
	}
	virtual bool IsReadable() const { return m_getter != NULL; }
	virtual bool IsWritable() const { return m_getter != NULL; }

	void SetValueDirect(CoreObject* target, TValue value) const
	{
		LN_THROW(m_setter != NULL, InvalidOperationException);
		m_setter(target, value);
	}
	const TValue& GetValueDirect(const CoreObject* target) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		return m_getter(target);
	}

private:
	template<typename TValue> friend class TypedPropertyInitializer;
	String		m_name;
	SetterFunc	m_setter;
	GetterFunc	m_getter;
};

template<typename TValue>
class TypedPropertyInitializer
{
public:
	typedef void(*SetterFunc)(CoreObject* obj, const TValue& value);
	typedef const TValue&(*GetterFunc)(const CoreObject* obj);

	SetterFunc	m_setter;
	GetterFunc	m_getter;

	TypedPropertyInitializer(TypedProperty<TValue>* prop, SetterFunc setter, GetterFunc getter)
	{
		prop->m_setter = setter;
		prop->m_getter = getter;
	}


};


#define LN_PROPERTY_BEGIN	struct Properties {
#define LN_PROPERTY_END		};

#define LN_PROPERTY(valueType, propVar) \
	public:  static const Lumino::Property*	propVar##; \
	private: static void  set_##propVar(CoreObject* obj, const valueType& value); \
	private: static const valueType& get_##propVar(const CoreObject* obj); \
	private: static TypedPropertyInitializer<valueType> init_##propVar;

#define LN_PROPERTY_IMPLEMENT(ownerClass, valueType, propVar, memberVar, defaultValue) \
	static TypedProperty<valueType>		_##propVar(ownerClass::GetClassTypeInfo(), _T(#propVar), defaultValue); \
	const Lumino::Property*				ownerClass::Properties::propVar## = PropertyManager::RegisterProperty(ownerClass::GetClassTypeInfo(), &_##propVar); \
	void								ownerClass::Properties::set_##propVar(CoreObject* obj, const valueType& value) { static_cast<ownerClass*>(obj)->memberVar = value; } \
	const valueType&					ownerClass::Properties::get_##propVar(const CoreObject* obj) { return static_cast<const ownerClass*>(obj)->memberVar; } \
	TypedPropertyInitializer<valueType>	ownerClass::Properties::init_##propVar(&_##propVar, &ownerClass::Properties::set_##propVar, &ownerClass::Properties::get_##propVar);






class AttachedProperty
	: public Property
{
public:
	AttachedProperty(TypeInfo* ownerClassType, const String& name, const Variant& defaultValue)
		: Property(ownerClassType, defaultValue, true)
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
	static /*AttachedProperty**/void  RegisterAttachedProperty(TypeInfo* ownerClass, const String& propertyName, const Variant& defaultValue);

	/// グローバル空間に定義された static 変数を初期化するのが目的
	static Property* RegisterProperty(TypeInfo* ownerClass, Property* prop)
	{
		ownerClass->RegisterProperty(prop);
		return prop;
	}


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


class StaticAttachedProperty
	: public AttachedProperty
{
public:
	StaticAttachedProperty(TypeInfo* ownerClassInfo, const String& name, const Variant& defaultValue)
		: AttachedProperty(ownerClassInfo, name, defaultValue)
	{
		//PropertyManager::RegisterAttachedProperty(ownerClassInfo, name, defaultValue);
	}
};

#define LN_DEFINE_ATTACHED_PROPERTY(ownerClassType, propVar, name, defaultValue) \
	static StaticAttachedProperty _##propVar(ownerClassType::GetClassTypeInfo(), _T(name), defaultValue); \
	const AttachedProperty* ownerClassType::propVar = &_##propVar;
//{ \
//	if (prop == NULL) { \
//		prop = PropertyManager::RegisterAttachedProperty(ownerClassType::GetClassTypeInfo(), _T(name), defaultValue); \
//	} \
//}

/// GUI 用 Set ユーティリティ
#define LN_SET_ATTACHED_PROPERTY(element, prop, value) \
	LN_VERIFY(element != NULL); \
	element->SetPropertyValue(prop, value);

/// GUI 用 Get ユーティリティ
#define LN_GET_ATTACHED_PROPERTY(element, prop, type) \
	LN_VERIFY(element != NULL); \
	return Variant::Cast<type>(element->GetPropertyValue(prop));

} // namespace Lumino
