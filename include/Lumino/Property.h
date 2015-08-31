
#pragma once
#include <map>
#include <memory>
#include <functional>


namespace Lumino
{
class Property;
class CoreObject;
class PropertyMetadata;


// TODO; Internal
class PropertyInstanceData
	: public RefObject
{
public:
	Property*		InheritanceKey;		// プロパティを親から継承するとき、this またはこの値をキーとして検索する。
	CoreObject*		InheritanceParent;	// ↑ので見つかった親あるいは祖先オブジェクト
	const Property*	InheritanceTarget;	// ↑のオブジェクトのどのプロパティから受け継ぐか (走査の結果継承元が見つからなかった場合、この PropertyInstanceData が表す Propery を指す。このとき InheritanceParent は NULL)
	uint16_t		RevisionCount;
	uint16_t		PathRevisionCount;
	bool			IsDefault;

	PropertyInstanceData()
		: InheritanceTarget(NULL)
		, InheritanceParent(NULL)
		, RevisionCount(0)
		, PathRevisionCount(0)
		, IsDefault(true)
	{}
};

typedef void(*PropertyChangedCallback)(CoreObject* obj, PropertyChangedEventArgs* e);
typedef RefPtr<PropertyInstanceData>*(*InstanceDataGetterFunc)(CoreObject* obj);

/**
	@brief		CoreObject のサブクラスが実装できるプロパティを表します。
	@details	プロパティは、メンバ変数に対する簡易的なリフレクションを提供します。
				
*/
class Property
{
public:
	Property(TypeInfo* ownerClassType, PropertyMetadata* metadata, bool stored)
		: m_ownerClassType(ownerClassType)
		//, m_defaultValue(defaultValue)
		, m_metadata(metadata)
		, m_stored(stored)
		, m_registerd(false)
	{
	}
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

	//const Variant& GetDefaultValue() const { return m_defaultValue; }
	PropertyMetadata* GetMetadata() const { return m_metadata; }
	PropertyInstanceData* GetPropertyInstanceData(CoreObject* obj) const;

	/// TypeInfo 内でこのプロパティを一意に識別するインデックス値
	int GetLocalIndex() const { return m_localIndex; }

	/// TODO: Meadata の機能
	virtual void NotifyPropertyChange(CoreObject* target, PropertyChangedEventArgs* e) const {}

private:
	template<typename T> friend class TypedPropertyInitializer;
	TypeInfo*	m_ownerClassType;
	PropertyMetadata*	m_metadata;	// グローバルインスタンスへのポインタ
	InstanceDataGetterFunc	m_instanceDataGetterFunc;
	//Variant	m_defaultValue;
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









template<typename TValue>
class TypedProperty
	: public Property
{
public:
	typedef void(*SetterFunc)(CoreObject* obj, TValue value);
	typedef TValue(*GetterFunc)(const CoreObject* obj);
	typedef void(*OnPropertyChangedFunc)(CoreObject* obj, PropertyChangedEventArgs* e);
	// ↑※static 関数のポインタでないと、言語バインダを作りにくくなる。

public:
	TypedProperty(TypeInfo* ownerTypeInfo, const TCHAR* name, PropertyMetadata* metadata/* TValue defaultValue*/)
		: Property(ownerTypeInfo, metadata, false)
		, m_name(name)
		, m_setter(NULL)
		, m_getter(NULL)
		, m_propChanged(NULL)
	{}

	virtual ~TypedProperty() {}

public:
	virtual const String& GetName() const { return m_name; }

	virtual void SetValue(CoreObject* target, Variant value) const
	{
		SetValueDirect(target, Variant::Cast<TValue>(value));
	}
	virtual Variant GetValue(const CoreObject* target) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		return Variant(m_getter(target));
	}
	virtual void AddItem(CoreObject* target, const Variant& value) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		//VariantList* list = m_getter(target);
		// TODO: 間違えて UIElementFractory のまま追加してしまうことがあった。型チェックできると良い。
		//list->AddVariant(value);
		auto list = m_getter(target);
		AddItemInternal(list, value);
	}
	virtual bool IsReadable() const { return m_getter != NULL; }	// TODO: virtual やめたほうが高速化できる。
	virtual bool IsWritable() const { return m_getter != NULL; }
	virtual bool IsList() const { return ListOperationSelector2<TValue>::IsList(); }

	void SetValueDirect(CoreObject* target, TValue value) const
	{
		LN_THROW(m_setter != NULL, InvalidOperationException);

		if (m_getter != NULL)
		{
			Variant oldValue = m_getter(target);
			m_setter(target, value);
			target->NotifyPropertyChange(this, value, oldValue);
		}
		else {
			m_setter(target, value);
			target->NotifyPropertyChange(this, value, Variant::Null);
		}

	}
	TValue GetValueDirect(const CoreObject* target) const
	{
		LN_THROW(m_getter != NULL, InvalidOperationException);
		return m_getter(target);
	}

	template<typename T>
	void AddItemInternal(T& list, const Variant& item) const	// 値型
	{
		LN_THROW(0, InvalidOperationException);
	}
	template<typename T>
	void AddItemInternal(T* list, const Variant& item) const	// ポインタ型
	{
		ListOperationSelector<T, std::is_base_of<VariantList, T>::type >::AddItem(*list, item);
	}


	template<typename T, typename TIsList> struct ListOperationSelector
	{ 
		static bool IsList() { return false; }
		static void AddItem(T& list, const Variant& item) { LN_THROW(0, InvalidOperationException); }
	};
	template<typename T> struct ListOperationSelector<T, std::true_type>
	{
		static bool IsList() { return true; }
		static void AddItem(T& list, const Variant& item) { list.VariantList::Add(item); }
	};


	template<typename T> struct ListOperationSelector2
	{
		static bool IsList() { return false; }
	};
	template<typename T> struct ListOperationSelector2<T*>
	{
		static bool IsList() { return ListOperationSelector<T, std::is_base_of<VariantList, T>::type>::IsList(); }
	};
	

	//template<typename T, typename TIsList>
	//void ListAddFunc(T* list) {}

	//template<typename T>
	//void ListAddFunc<T, std::true_type>(T* list, const Variant& item) { list->AddVariant(item); }

	virtual void NotifyPropertyChange(CoreObject* target, PropertyChangedEventArgs* e) const
	{
		if (m_propChanged != NULL) {
			m_propChanged(target, e);
		}
	}

private:
	template<typename T> friend class TypedPropertyInitializer;
	String					m_name;
	SetterFunc				m_setter;
	GetterFunc				m_getter;
	OnPropertyChangedFunc	m_propChanged;
};




/* プロパティの動作オプションです。*/
LN_ENUM_FLAGS(PropertyOptions)
{
	None = 0x0000,			/**< オプション指定無し。*/
	Inherits = 0x0001,		/**< 子要素によって継承されます。*/
};
LN_ENUM_FLAGS_DECLARE(PropertyOptions);

/**
	@brief		
*/
class PropertyMetadata
{
	// メンバ関数ポインタは void* に変換できない。
	// また、プロパティ定義側に TypedPropertyMetadata<Owner> みたいにテンプレートで書きたくない。
	// なので、メンバ関数ポインタをラップするクラスを用意した。
	class CallbackWrapper
	{
	public:
		virtual ~CallbackWrapper() {}
	};

	template<typename TCallback>
	class SimpleCallbackWrapper : public CallbackWrapper
	{
	public:
		SimpleCallbackWrapper(TCallback callback) : m_callback(callback) {}
		TCallback GetCallback() { return m_callback; }
		TCallback m_callback;
	};

public:
	PropertyMetadata(const Variant& defaultValue)
	{
		Init();
		m_defaultValue = defaultValue;
	}
	template<typename TOwnerClass>
	PropertyMetadata(const Variant& defaultValue, void(TOwnerClass::*propertyChanged)(PropertyChangedEventArgs*))
	{
		Init();
		m_defaultValue = defaultValue;
		m_propertyChangedCallback = LN_NEW SimpleCallbackWrapper< void(TOwnerClass::*)(PropertyChangedEventArgs*) >(propertyChanged);
	}
	template<typename TOwnerClass>
	PropertyMetadata(const Variant& defaultValue, PropertyOptions options, void(TOwnerClass::*propertyChanged)(PropertyChangedEventArgs*))
	{
		Init();
		m_defaultValue = defaultValue;
		m_propertyChangedCallback = LN_NEW SimpleCallbackWrapper< void(TOwnerClass::*)(PropertyChangedEventArgs*) >(propertyChanged);
		m_options = options;
	}
	template<typename TOwnerClass>
	PropertyMetadata(const Variant& defaultValue, PropertyOptions options, Property* inheritanceKey, void(TOwnerClass::*propertyChanged)(PropertyChangedEventArgs*))
	{
		Init();
		m_defaultValue = defaultValue;
		m_propertyChangedCallback = LN_NEW SimpleCallbackWrapper< void(TOwnerClass::*)(PropertyChangedEventArgs*) >(propertyChanged);
		m_options = options;
		m_inheritanceKey = inheritanceKey;
	}

	~PropertyMetadata()
	{
		LN_SAFE_DELETE(m_propertyChangedCallback);
	}

public:
	const Variant& GetDefaultValue() const { return m_defaultValue; }
	template<typename TCallback>
	TCallback GetPropertyChangedCallback()
	{
		if (m_propertyChangedCallback == NULL) { return NULL; }
		return static_cast< SimpleCallbackWrapper<TCallback>* >(m_propertyChangedCallback)->GetCallback();
	}
	PropertyOptions GetPropertyOptions() const { return m_options; }
	Property* GetInheritanceTarget() const { return m_inheritanceTarget; }

private:
	void Init()
	{
		m_defaultValue = Variant::Null;
		m_propertyChangedCallback = NULL;
		m_options = PropertyOptions::None;
		m_inheritanceTarget = NULL;
	}

private:
	Variant					m_defaultValue;
	CallbackWrapper*		m_propertyChangedCallback;
	//void*					m_propertyChangedCallback;	// 実体は void(ownerClass::*func)(PropertyChangedEventArgs*)
	PropertyOptions			m_options;
	Property*				m_inheritanceTarget;
};




template<typename TValue>
class TypedPropertyInitializer
{
	// Initializer は .h 側に不必要な型を書きたくないから用意したもの。
	// 現在 TypedProperty はメンバ static ではなく、グローバル static を前提としている。
	// となると、private メンバには当然アクセスできないが、このクラス経由で各メンバへの参照経路を設定する。

public:
	typedef void(*SetterFunc)(CoreObject* obj, TValue value);
	typedef TValue(*GetterFunc)(const CoreObject* obj);

	// Obsolete
	TypedPropertyInitializer(TypedProperty<TValue>* prop, SetterFunc setter, GetterFunc getter, PropertyChangedCallback propChanged)
	{
		prop->m_setter = setter;
		prop->m_getter = getter;
		prop->m_propChanged = propChanged;
	}

	template<typename TPropertyChangedCallbackCaster>
	TypedPropertyInitializer(
		TypedProperty<TValue>* prop,
		SetterFunc setter,
		GetterFunc getter,
		TPropertyChangedCallbackCaster propertyChangedCallbackCaster,
		PropertyMetadata* metadata,
		InstanceDataGetterFunc instanceDataGetter)
	{
		prop->m_setter = setter;
		prop->m_getter = getter;
		prop->m_propChanged = propertyChangedCallbackCaster;
		prop->m_metadata = metadata;
		prop->m_instanceDataGetterFunc = instanceDataGetter;
		assert(metadata != NULL);
	}
};


#define LN_PROPERTY(valueType, propVar) \
	public:  static const Lumino::Property*	propVar##; \
	private: static void  set_##propVar(CoreObject* obj, valueType value); \
	private: static valueType get_##propVar(const CoreObject* obj); \
	private: static void  changed_##propVar(CoreObject* obj, PropertyChangedEventArgs* e); \
	private: static TypedPropertyInitializer<valueType> init_##propVar; \
	private: static PropertyMetadata metadata_##propVar; \
	private: static RefPtr<PropertyInstanceData>* getInstanceData_##propVar(CoreObject* obj); \
	private: RefPtr<PropertyInstanceData> instanceData_##propVar = NULL;

#define LN_PROPERTY_IMPLEMENT(ownerClass, valueType, propVar, propName, memberVar, metadata) \
	typedef void(ownerClass::*PropertyChangedCallback_##propVar)(PropertyChangedEventArgs*); \
	static TypedProperty<valueType>		_##propVar(ownerClass::GetClassTypeInfo(), _T(propName), NULL); \
	const Lumino::Property*				ownerClass::propVar = PropertyManager::RegisterProperty(ownerClass::GetClassTypeInfo(), &_##propVar); \
	void								ownerClass::set_##propVar(CoreObject* obj, valueType value) { static_cast<ownerClass*>(obj)->memberVar = value; } \
	valueType							ownerClass::get_##propVar(const CoreObject* obj) { return static_cast<const ownerClass*>(obj)->memberVar; } \
	void								ownerClass::changed_##propVar(CoreObject* obj, PropertyChangedEventArgs* e) { auto func = metadata_##propVar.GetPropertyChangedCallback<PropertyChangedCallback_##propVar>(); if (func != NULL) { (static_cast<ownerClass*>(obj)->*func)(e); } } \
	TypedPropertyInitializer<valueType>	ownerClass::init_##propVar(&_##propVar, &ownerClass::set_##propVar, &ownerClass::get_##propVar, changed_##propVar, &ownerClass::metadata_##propVar, ownerClass::getInstanceData_##propVar); \
	RefPtr<PropertyInstanceData>*		ownerClass::getInstanceData_##propVar(CoreObject* obj) { return &(static_cast<ownerClass*>(obj)->instanceData_##propVar); } \
	PropertyMetadata					ownerClass::metadata_##propVar = metadata;

#define LN_PROPERTY_IMPLEMENT_GETTER_SETTER(ownerClass, valueType, propVar, propName, getter, setter, metadata) \
	typedef void(ownerClass::*PropertyChangedCallback_##propVar)(PropertyChangedEventArgs*); \
	static TypedProperty<valueType>		_##propVar(ownerClass::GetClassTypeInfo(), _T(propName), NULL); \
	const Lumino::Property*				ownerClass::propVar = PropertyManager::RegisterProperty(ownerClass::GetClassTypeInfo(), &_##propVar); \
	void								ownerClass::set_##propVar(CoreObject* obj, valueType value) { static_cast<ownerClass*>(obj)->setter(value); } \
	valueType							ownerClass::get_##propVar(const CoreObject* obj) { return static_cast<const ownerClass*>(obj)->getter(); } \
	void								ownerClass::changed_##propVar(CoreObject* obj, PropertyChangedEventArgs* e) { auto func = metadata_##propVar.GetPropertyChangedCallback<PropertyChangedCallback_##propVar>(); if (func != NULL) { (static_cast<ownerClass*>(obj)->*func)(e); } } \
	TypedPropertyInitializer<valueType>	ownerClass::init_##propVar(&_##propVar, &ownerClass::set_##propVar, &ownerClass::get_##propVar, changed_##propVar, &ownerClass::metadata_##propVar, ownerClass::getInstanceData_##propVar); \
	RefPtr<PropertyInstanceData>*		ownerClass::getInstanceData_##propVar(CoreObject* obj) { return &(static_cast<ownerClass*>(obj)->instanceData_##propVar); } \
	PropertyMetadata					ownerClass::metadata_##propVar = metadata;





class AttachedProperty
	: public Property
{
public:
	AttachedProperty(TypeInfo* ownerClassType, const String& name, PropertyMetadata* metadata/*const Variant& defaultValue*/)
		: Property(ownerClassType, metadata, true)
		, m_name(name)
		//, m_defaultValue(defaultValue)
	{
		assert(metadata != NULL);
	}

	virtual const String& GetName() const { return m_name; }
	virtual void SetValue(CoreObject* target, Variant value) const { LN_THROW(0, InvalidOperationException); }
	virtual Variant GetValue(const CoreObject* target) const  { LN_THROW(0, InvalidOperationException); }

private:
	String	m_name;
	//Variant	m_defaultValue;
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
	StaticAttachedProperty(TypeInfo* ownerClassInfo, const String& name, PropertyMetadata* metadata/*, const Variant& defaultValue*/)
		: AttachedProperty(ownerClassInfo, name, metadata)
	{
		//PropertyManager::RegisterAttachedProperty(ownerClassInfo, name, defaultValue);
	}
};

#define LN_DEFINE_ATTACHED_PROPERTY(ownerClassType, propVar, name, defaultValue) \
	PropertyMetadata		metadata_##propVar(defaultValue); \
	static StaticAttachedProperty _##propVar(ownerClassType::GetClassTypeInfo(), _T(name), &metadata_##propVar); \
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
