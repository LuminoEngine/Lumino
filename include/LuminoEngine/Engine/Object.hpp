#pragma once
#include <mutex>

namespace ln {
class EngineContext;
class TypeInfo;
class PropertyInfo;
class ReflectionObjectVisitor;
namespace detail {
class WeakRefInfo; 
class ObjectHelper;
class EngineDomain;
}

#define LN_OBJECT \
    friend class ::ln::TypeInfo; \
    friend class ::ln::detail::EngineDomain; \
    static TypeInfo* _lnref_getTypeInfo(); \
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override; \
	static void _lnref_registerTypeInfo(EngineContext* context);

#define LN_OBJECT_IMPLEMENT(classType, baseclassType) \
    TypeInfo* classType::_lnref_getTypeInfo() \
    { \
        static TypeInfo typeInfo(#classType, ::ln::TypeInfo::getTypeInfo<baseclassType>()); \
        return &typeInfo; \
    } \
    ::ln::TypeInfo* classType::_lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); } \
	void classType::_lnref_registerTypeInfo(EngineContext* context)

#define LN_INTERNAL_NEW_OBJECT \
    template<class T, typename... TArgs> friend ln::Ref<T> ln::makeObject(TArgs&&... args); \
    template<class T, typename... TArgs> friend void ln::placementNewObject(void* ptr, TArgs&&... args); 

#ifndef LN_CONSTRUCT_ACCESS
#define LN_CONSTRUCT_ACCESS \
		template<class T, typename... TArgs> friend ln::Ref<T> ln::makeObject(TArgs&&... args); \
		template<class T, typename... TArgs> friend void ln::placementNewObject(void* ptr, TArgs&&... args); \
		protected
#endif

template<class T, typename... TArgs>
Ref<T> makeObject(TArgs&&... args)
{
	auto ptr = Ref<T>(new T(), false);
	ptr->init(std::forward<TArgs>(args)...);
	return ptr;
}

template<class T, typename... TArgs>
void placementNewObject(void* ptr, TArgs&&... args)
{
	new (ptr)T();
	static_cast<T*>(ptr)->init(std::forward<TArgs>(args)...);
}

class LN_API Object
	: public RefObject
{
LN_CONSTRUCT_ACCESS:
	Object();
	void init();

protected:
	virtual ~Object();
	virtual void finalize() override;
	virtual void onDispose(bool explicitDisposing);

	LN_SERIALIZE_CLASS_VERSION(1);
    virtual void serialize(Archive& ar);

public:
	/**
	 * このオブジェクトが保持しているリソースを開放します。
	 *
	 * このメソッドは通常、明示的に呼び出す必要はありません。オブジェクトが保持しているリソースの開放の処理は、
	 * オブジェクトの参照が無くなった時点でフレームワークにより呼び出されます。
	 *
	 * 派生クラスで何らかのリソースを開放する必要がある場合は onDispose を実装して開放してください。(デストラクタや finalize、dispose ではな)
	 *
	 * @attention このメソッドは virtual です。RAII の実装を目的としてデストラクタで呼び出すことはできません。代わりに finalize() からコールされます。
	 */
	virtual void dispose();

	// TODO: internal
	virtual bool traverseRefrection(ReflectionObjectVisitor* visitor);

    void setAssetId(const Uuid& id) { m_assetId = id; }
    const Uuid& assetId() const { return m_assetId; }

private:
    detail::WeakRefInfo* requestWeakRefInfo();
    static TypeInfo* _lnref_getTypeInfo();
    virtual TypeInfo* _lnref_getThisTypeInfo() const;

    detail::WeakRefInfo* m_weakRefInfo;
    std::mutex m_weakRefInfoMutex;
	intptr_t m_runtimeData;
    Uuid m_assetId;

    friend class TypeInfo;
    friend class detail::ObjectHelper;
};

class ObjectHelper
{
public:
	static void dispose(Object* obj)
    {
        if (obj) obj->dispose();
    }
};


namespace detail {

class ObjectHelper
{
public:
    template<class T>
    static detail::WeakRefInfo* requestWeakRefInfo(T* obj) { return obj->requestWeakRefInfo(); }
    static void destructObject(Object* obj) { obj->~Object(); }
	static void setRuntimeData(Object* obj, intptr_t data) { obj->m_runtimeData = data; }
	static intptr_t getRuntimeData(Object* obj) { return obj->m_runtimeData; }
};

class WeakRefInfo final
{
public:
    Object* owner;
    std::atomic<int> weakRefCount;
    WeakRefInfo();
    void addRef();
    void release();
};

} // namespace detail


/**
    @brief
    @details
        監視しているオブジェクトにアクセスする場合は isAlive() と resolve() を併用しないでください。
        マルチスレッドプログラムで不正アクセスの危険があります。
        次のコードは間違いです。
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        WeakRefPtr<MyClass> weak(obj);
        if (weak.isAlive())
            weak->resolve()->Func();
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        正しいコードは次の通りです。
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        WeakRefPtr<MyClass> weak(obj);
        Ref<MyClass> ptr = weak.resolve();
        if (ptr != nullptr)
            ptr->Func();
        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
template <class T>
class WeakRefPtr
{
public:

    /** コンストラクタ */
    WeakRefPtr()
        : m_weakRefInfo(nullptr)
    {}

    /** コンストラクタ */
    WeakRefPtr(T* obj)
        : m_weakRefInfo(nullptr)
    {
        set(detail::ObjectHelper::requestWeakRefInfo(obj));
    }

    /** コピーコンストラクタ */
    WeakRefPtr(const WeakRefPtr<T>& obj)
        : m_weakRefInfo(nullptr)
    {
        set(obj.m_weakRefInfo);
    }

    /** デストラクタ */
    virtual ~WeakRefPtr()
    {
        release();
    }

    /** 監視しているオブジェクトが削除されておらず、使用できるかを確認します。*/
    bool isAlive() const
    {
        return (m_weakRefInfo != nullptr && m_weakRefInfo->owner != nullptr);
    }

    /** 監視しているオブジェクトへの Ref を取得します。*/
    Ref<T> resolve() const
    {
        if (!isAlive())
        {
            return Ref<T>();
        }
        return Ref<T>(static_cast<T*>(m_weakRefInfo->owner));
    }

    /** */
    WeakRefPtr<T>& operator =(const WeakRefPtr<T>& obj)
    {
        set(obj.m_weakRefInfo);
        return *this;
    }

    /** */
    WeakRefPtr<T>& operator =(T* obj)
    {
        set(detail::ObjectHelper::requestWeakRefInfo(obj));
        return *this;
    }

private:

    void set(detail::WeakRefInfo* info)
    {
        release();
        m_weakRefInfo = info;
        if (m_weakRefInfo != nullptr)
        {
            m_weakRefInfo->addRef();
        }
    }

    void release()
    {
        if (m_weakRefInfo != nullptr)
        {
            m_weakRefInfo->release();
            m_weakRefInfo = nullptr;
        }
    }

    detail::WeakRefInfo*	m_weakRefInfo;
};
namespace detail { struct TypeInfoInternal; }

class TypeInfo
    : public RefObject
{
public:
    TypeInfo(const char* className, TypeInfo* baseType)
        : m_name(className)
        , m_baseType(baseType)
		, m_managedTypeInfoId(-1)
    {}

    /** クラス名を取得します。 */
    const String& name() const { return m_name; }

    /** ベースクラスの型情報を取得します。 */
    TypeInfo* baseType() const { return m_baseType; }

    void registerProperty(PropertyInfo* prop);
    const List<Ref<PropertyInfo>>& properties() const { return m_properties; }

	Ref<Object> createInstance() const;
	static Ref<Object> createInstance(const String& typeName);	// TODO: EngineContext へ


    /** 型引数に指定したクラス型の型情報を取得します。 */
    template<class T>
    static TypeInfo* getTypeInfo()
    {
        return T::_lnref_getTypeInfo();
    }

    static TypeInfo* getTypeInfo(const Object* obj)
    {
        return obj->_lnref_getThisTypeInfo();
    }

    static void initializeObjectProperties(Object* obj);

	// TODO: internal
	std::function<Ref<Object>()> m_factory;

private:
    String m_name;
    TypeInfo* m_baseType;
    List<Ref<PropertyInfo>> m_properties;
	int64_t m_managedTypeInfoId;

	friend class detail::TypeInfoInternal;
};

namespace detail {
struct TypeInfoInternal
{
	static void setManagedTypeInfoId(TypeInfo* typeInfo, int64_t id) { typeInfo->m_managedTypeInfoId = id; }
	static int64_t getManagedTypeInfoId(const TypeInfo* typeInfo) { return typeInfo->m_managedTypeInfoId; }
};
} // namespace detail

class Serializer
{
public:
	template<class T>
	static String serialize(const Ref<T>&& value)
	{
		JsonTextOutputArchive ar;
		ar.save(*value);
		return ar.toString(JsonFormatting::None);
	}

	template<class T>
	static Ref<T> deserialize(const StringRef& jsonText)
	{
		Ref<T> value = makeObject<T>();
		JsonTextInputArchive ar(jsonText);
		ar.load(*value);
		return value;
	}
};

//template<
//	typename TValue,
//	typename std::enable_if<std::is_abstract<TValue>::value_type, std::nullptr_t>::type = nullptr>
//	void serialize(Archive& ar, Ref<TValue>& value)
//{
//	bool isNull = (value == nullptr);
//	ar.makeSmartPtrTag(&isNull);
//
//	ln::String typeName;
//	if (ar.isSaving()) {
//		typeName = TypeInfo::getTypeInfo(value)->name();
//	}
//	ar.makeTypeInfo(&typeName);
//	//TODO: ここで makeTypeInfo すると、その中では setNextName しているため、現在の nextName をうわがいてしまう
//
//	if (ar.isSaving()) {
//		if (!isNull) {
//			ar.process(*value.get());
//		}
//	}
//	else {
//		if (!isNull) {
//			if (!typeName.isEmpty()) {
//				auto obj = TypeInfo::createInstance(typeName);
//				value = dynamic_pointer_cast<TValue>(obj);
//			}
//
//			//// TODO: TypeName が登録されていない場合はベースのを作るか、エラーにするかオプションで決められるようにしたい。
//			//if (!value) {
//			//	value = makeObject<TValue>();
//			//}
//
//			ar.process(*value.get());
//		}
//		else {
//			value = nullptr;
//		}
//	}
//}

namespace detail {
	
template<
	typename T,
	typename std::enable_if<std::is_abstract<T>::value, std::nullptr_t>::type = nullptr>
Ref<T> makeObjectHelper() {
	return nullptr;
}

template<
	typename T,
	typename std::enable_if<!std::is_abstract<T>::value, std::nullptr_t>::type = nullptr>
Ref<T> makeObjectHelper() {
	return makeObject<T>();
}

} // namespace detail

template<
	typename TValue,
	typename std::enable_if<detail::is_lumino_engine_object<TValue>::value, std::nullptr_t>::type = nullptr>
void serialize(Archive& ar, Ref<TValue>& value)
{
	bool isNull = (value == nullptr);
	ar.makeSmartPtrTag(&isNull);

	ln::String typeName;
	if (ar.isSaving()) {
		typeName = TypeInfo::getTypeInfo(value)->name();
	}
	ar.makeTypeInfo(&typeName);
	//TODO: ここで makeTypeInfo すると、その中では setNextName しているため、現在の nextName をうわがいてしまう

	if (ar.isSaving()) {
		if (!isNull) {
			ar.process(*value.get());
		}
	}
	else {
		if (!isNull) {
			if (!typeName.isEmpty()) {
				auto obj = TypeInfo::createInstance(typeName);
				value = dynamic_pointer_cast<TValue>(obj);
			}

			// TODO: TypeName が登録されていない場合はベースのを作るか、エラーにするかオプションで決められるようにしたい。
			// TODO: Abstract 対策のためいったん無効化。多分有効化する必要はない気がするけど…。
			if (!value) {
				value = detail::makeObjectHelper<TValue>();
			}
			if (value) {
				ar.process(*value.get());
			}
		}
		else {
			value = nullptr;
		}
	}
}

} // namespace ln

