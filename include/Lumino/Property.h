
#pragma once

namespace Lumino
{

/**
	@brief		
*/
class Property
{
public:
	Property() {}
	~Property() {}

public:
	virtual const String& GetName() const = 0;

	virtual void SetValue(CoreObject* target, Variant value) = 0;
	virtual Variant GetValue(const CoreObject* target) const = 0;

	virtual bool IsReadable() const { return false; }
	virtual bool IsWritable() const { return false; }
	
	// Property は C++ 側でだけ使用する。
	// getter setter の登録と GetValue SetValue 実装のユーティリティと考える。
	// DependencyProperty のように static で公開したりはしない。

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
template<class TClass, typename TValue, typename TInternalValue = TValue>	// TInternalValue は Enum のばあい一律 int。それいがいは TValue と一緒
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

		void Call(TClass* instance, const TValue& value)
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
		: m_name(name)
		, m_setter(setter)
		, m_getter(getter)
	{}

	virtual ~CoreObjectProperty() {}

	virtual const String& GetName() const { return m_name; }

	virtual void SetValue(CoreObject* target, Variant value)
	{
		LN_THROW(!m_setter.IsEmpty(), InvalidOperationException);
		TClass* instance = static_cast<TClass*>(target);
		m_setter.Call(instance, value.Cast<TInternalValue>());
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

#define LN_DEFINE_PROPERTY(classType, nativeType, name, setterFuncPtr, getterFuncPtr, defaultValue) \
{ \
	static ::Lumino::CoreObjectProperty<classType, nativeType> prop( \
		name, setterFuncPtr, getterFuncPtr, defaultValue); \
	RegisterProperty(&prop); \
}

#define LN_DEFINE_PROPERTY_ENUM(classType, nativeType, name, setterFuncPtr, getterFuncPtr, defaultValue) \
{ \
	static ::Lumino::CoreObjectProperty<classType, nativeType, int> prop( \
		name, setterFuncPtr, getterFuncPtr, defaultValue); \
	RegisterProperty(&prop); \
}

} // namespace Lumino
