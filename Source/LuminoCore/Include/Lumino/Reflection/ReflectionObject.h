
#pragma once
#include <atomic>
#include <list>
#include "../Threading/Mutex.h"
#include "../Base/Common.h"
#include "../Base/RefObject.h"
#include "../Base/Collection.h"
#include "TypeInfo.h"
#include "Notify.h"

LN_NAMESPACE_BEGIN
namespace tr
{

#define LN_TR_REFLECTION_TYPEINFO_DECLARE_COMMON(typeInfo) \
	private: \
		template<typename T> friend class ln::Ref; \
		friend class ln::tr::ReflectionHelper; \
		friend class ln::tr::ReflectionObject; \
		static typeInfo							lnref_typeInfo; \
		ln::tr::LocalValueHavingFlags			lnref_localValueHavingFlags; \
		virtual typeInfo*						lnref_GetThisTypeInfo() const override; \
		static void*							lnref_bindingTypeInfo; \
		inline void								initializeProperties() { lnref_typeInfo.initializeProperties(this); }

#define LN_TR_REFLECTION_TYPEINFO_IMPLEMENT_COMMON(typeInfo, classType, baseClassType, ...) \
	typeInfo						classType::lnref_typeInfo(_TT(#classType), ln::tr::ReflectionHelper::getClassTypeInfo<baseClassType>(), &ln::tr::ReflectionHelper::GetLocalValueHavingFlags<classType>, &ln::tr::ReflectionHelper::setBindingTypeInfo<classType>, &ln::tr::ReflectionHelper::getBindingTypeInfo<classType>, {__VA_ARGS__}); \
	typeInfo*						classType::lnref_GetThisTypeInfo() const { return &lnref_typeInfo; } \
	void*							classType::lnref_bindingTypeInfo = nullptr;

#define LN_TR_REFLECTION_TYPEINFO_DECLARE() \
	LN_TR_REFLECTION_TYPEINFO_DECLARE_COMMON(ln::tr::TypeInfo)

#define LN_OBJECT \
	LN_TR_REFLECTION_TYPEINFO_DECLARE_COMMON(ln::tr::TypeInfo)

#define LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(classType, baseClassType, ...) \
	LN_TR_REFLECTION_TYPEINFO_IMPLEMENT_COMMON(ln::tr::TypeInfo, classType, baseClassType, __VA_ARGS__)

namespace detail { class WeakRefInfo; }
typedef uint32_t LocalValueHavingFlags;
class PropertyInfo;



/**
	@brief	Lumino が提供するオブジェクトのベースクラスです。
*/
class ReflectionObject
	: public RefObject
{
private:
	// declare base TypeInfo
    template<typename T> friend class ln::Ref;
    friend class ln::tr::ReflectionHelper;
    static TypeInfo					lnref_typeInfo;
    LocalValueHavingFlags			lnref_localValueHavingFlags;
    virtual TypeInfo*				lnref_GetThisTypeInfo() const;
    static void*					lnref_bindingTypeInfo;
    
public:
	ReflectionObject();
	virtual ~ReflectionObject();
	void initialize() {}

	void setUserData(void* data) { m_userData = data; }
	void* getUserData() const { return m_userData; }


protected:
	friend class PropertyInfo;
	void raiseReflectionEvent(const ReflectionEventBase& ev, ReflectionEventArgs* args);
	virtual void onPropertyChanged(PropertyChangedEventArgs* e);

	template<typename... TArgs>
	void raiseDelegateEvent(DelegateEvent<TArgs...>& ev, TArgs... args) { ev.raise(args...); }

private:
#ifdef LN_LEGACY_VARIANT_ENABLED
	void setPropertyValueInternal(const PropertyInfo* prop, const Variant& value, bool reset, PropertySetSource source);
#endif

	void*	m_userData;

	friend class ReflectionHelper;
	detail::WeakRefInfo* requestWeakRefInfo();

	detail::WeakRefInfo*	m_weakRefInfo;
	Mutex					m_weakRefInfoMutex;

	List<Ref<ReflectionObject>>	m_gcList;
	bool							m_autoGC;

LN_INTERNAL_ACCESS:

	template<class T, typename... TArgs>
	static Ref<T> makeRef(TArgs... args)
	{
		auto ptr = Ref<T>(LN_NEW T(), false);
		ptr->initialize(args...);
		return ptr;
	}
};
	

class ReflectionHelper
{
public:
	template<class T>
	static ln::tr::LocalValueHavingFlags* GetLocalValueHavingFlags(ReflectionObject* thisObj)
	{
		return &static_cast<T*>(thisObj)->lnref_localValueHavingFlags;
	}
	template<class T>
	static TypeInfo* getClassTypeInfo()
	{
		return &T::lnref_typeInfo;
	}
	template<class T>
	static void setBindingTypeInfo(void* data)
	{
		T::lnref_bindingTypeInfo = data;
	}
	template<class T>
	static void* getBindingTypeInfo()
	{
		return T::lnref_bindingTypeInfo;
	}
	template<class T>
	static TypeInfo* getTypeInfo(const T* obj)
	{
		return obj->lnref_GetThisTypeInfo();
	}
	template<class T>
	inline static TypeInfo* getTypeInfo()
	{
		return &T::lnref_typeInfo;
	}
	
	template<class T>
	inline static detail::WeakRefInfo* requestWeakRefInfo(T* obj)
	{
		return obj->requestWeakRefInfo();
	}
	template<class T, class TData>
	inline static TData* requestAnimationData(T* obj)
	{
		if (obj->m_animationData == nullptr)
		{
			obj->m_animationData = LN_NEW TData();
		}
		return static_cast<TData*>(obj->m_animationData);
	}
	
	static void addGCObject(ReflectionObject* obj, ReflectionObject* child);
	static void removeGCObject(ReflectionObject* obj, ReflectionObject* child);
	static void gcObjects(ReflectionObject* obj);
	static bool isGCReady(ReflectionObject* obj);
	
	template<class TList>
	inline static void gcObjectList(TList* list)
	{
		list->removeAll([](typename TList::value_type& obj) { return isGCReady(obj); });
	}
	
	
	
	static int32_t getInternalReferenceCount(RefObject* obj) { return obj->m_internalReferenceCount; }
	static void addRefInternal(RefObject* obj) { obj->m_internalReferenceCount++; }
	static void releaseInternal(RefObject* obj) { obj->releaseInternal(); }
};

namespace detail {
	
	
	// 1つの ReflectionObject に対して1つ作られる
	class WeakRefInfo final
	{
	public:
		
		RefObject*			owner;
		std::atomic<int>	weakRefCount;// = 1;	// GCC で使えなかった
		//int	weakRefCount;
		
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
		set(ReflectionHelper::requestWeakRefInfo(obj));
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
		set(ReflectionHelper::requestWeakRefInfo(obj));
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

/**
	@brief		
*/
template<typename T>
class ReflectionObjectList
	: public RefObject
	, public Collection<T>
{
public:
	typedef typename Collection<T>::value_type	value_type;

public:
	ReflectionObjectList()
	{}

	virtual ~ReflectionObjectList()
	{
		Collection<T>::clear();
	}

protected:
	virtual void insertItem(int index, const value_type& item) override
	{
		Collection<T>::insertItem(index, item);
		LN_SAFE_ADDREF(item);
	}
	virtual void clearItems() override
	{
		for (auto* item : *this) {
			LN_SAFE_RELEASE(item);
		}
		Collection<T>::clearItems();
	}
	virtual void removeItem(int index) override
	{
		if (Collection<T>::getAt(index) != nullptr) {
			Collection<T>::getAt(index)->release();
		}
		Collection<T>::removeItem(index);
	}
	virtual void setItem(int index, const value_type& item) override
	{
		LN_SAFE_ADDREF(item);
		if (Collection<T>::getAt(index) != nullptr) {
			Collection<T>::getAt(index)->release();
		}
		Collection<T>::setItem(index, item);
	}

private:
};


//------------------------------------------------------------------------------
template<class T>
inline TypeInfo* TypeInfo::getTypeInfo()
{
	return ReflectionHelper::getTypeInfo<T>();
}

} // namespace tr

typedef tr::ReflectionObject Object;


template<class T, typename... TArgs>
Ref<T> newObject(TArgs&&... args)
{
	auto ptr = Ref<T>(new T(), false);
	ptr->initialize(std::forward<TArgs>(args)...);
	return ptr;
}

template<class T, typename... TArgs>
void placementNewObject(void* ptr, TArgs&&... args)
{
	new (ptr)T();
	static_cast<T*>(ptr)->initialize(std::forward<TArgs>(args)...);
}

LN_NAMESPACE_END

