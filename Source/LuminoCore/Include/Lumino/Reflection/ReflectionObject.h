
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
		template<typename T> friend class ln::RefPtr; \
		friend class ln::tr::ReflectionHelper; \
		friend class ln::tr::ReflectionObject; \
		static typeInfo							lnref_typeInfo; \
		ln::tr::LocalValueHavingFlags			lnref_localValueHavingFlags; \
		virtual typeInfo*						lnref_GetThisTypeInfo() const override; \
		static void*							lnref_bindingTypeInfo; \
		inline void								InitializeProperties() { lnref_typeInfo.InitializeProperties(this); }

#define LN_TR_REFLECTION_TYPEINFO_IMPLEMENT_COMMON(typeInfo, classType, baseClassType, ...) \
	typeInfo						classType::lnref_typeInfo(_T(#classType), ln::tr::ReflectionHelper::GetClassTypeInfo<baseClassType>(), &ln::tr::ReflectionHelper::GetLocalValueHavingFlags<classType>, &ln::tr::ReflectionHelper::SetBindingTypeInfo<classType>, &ln::tr::ReflectionHelper::GetBindingTypeInfo<classType>, {__VA_ARGS__}); \
	typeInfo*						classType::lnref_GetThisTypeInfo() const { return &lnref_typeInfo; } \
	void*							classType::lnref_bindingTypeInfo = nullptr;

#define LN_TR_REFLECTION_TYPEINFO_DECLARE() \
	LN_TR_REFLECTION_TYPEINFO_DECLARE_COMMON(ln::tr::TypeInfo)

#define LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(classType, baseClassType, ...) \
	LN_TR_REFLECTION_TYPEINFO_IMPLEMENT_COMMON(ln::tr::TypeInfo, classType, baseClassType, __VA_ARGS__)





/**
	@brief		
*/
class ReflectionObject
	: public RefObject
{
private:
	// declare base TypeInfo
    template<typename T> friend class ln::RefPtr;
    friend class ln::tr::ReflectionHelper;
    static TypeInfo					lnref_typeInfo;
    LocalValueHavingFlags			lnref_localValueHavingFlags;
    virtual TypeInfo*				lnref_GetThisTypeInfo() const;
    static void*					lnref_bindingTypeInfo;
    
public:
	ReflectionObject();
	virtual ~ReflectionObject();
	void initialize() {}

	void SetUserData(void* data) { m_userData = data; }
	void* GetUserData() const { return m_userData; }


protected:
	friend class PropertyInfo;
	void RaiseReflectionEvent(const ReflectionEventBase& ev, ReflectionEventArgs* args);
	virtual void OnPropertyChanged(PropertyChangedEventArgs* e);

	template<typename... TArgs>
	void RaiseDelegateEvent(DelegateEvent<TArgs...>& ev, TArgs... args) { ev.Raise(args...); }

private:
	void SetPropertyValueInternal(const PropertyInfo* prop, const Variant& value, bool reset, PropertySetSource source);

	void*	m_userData;

	friend class ReflectionHelper;
	detail::WeakRefInfo* RequestWeakRefInfo();

	detail::WeakRefInfo*	m_weakRefInfo;
	Mutex					m_weakRefInfoMutex;

	List<RefPtr<ReflectionObject>>	m_gcList;
	bool							m_autoGC;

LN_INTERNAL_ACCESS:

	template<class T, typename... TArgs>
	static RefPtr<T> MakeRef(TArgs... args)
	{
		auto ptr = RefPtr<T>(LN_NEW T(), false);
		ptr->initialize(args...);
		return ptr;
	}
};

/**
	@brief
	@details
		監視しているオブジェクトにアクセスする場合は IsAlive() と Resolve() を併用しないでください。
		マルチスレッドプログラムで不正アクセスの危険があります。
		次のコードは間違いです。
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		WeakRefPtr<MyClass> weak(obj);
		if (weak.IsAlive())
			weak->Resolve()->Func();
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		正しいコードは次の通りです。
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		WeakRefPtr<MyClass> weak(obj);
		RefPtr<MyClass> ptr = weak.Resolve();
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
		Set(ReflectionHelper::RequestWeakRefInfo(obj));
	}

	/** コピーコンストラクタ */
	WeakRefPtr(const WeakRefPtr<T>& obj)
		: m_weakRefInfo(nullptr)
	{
		Set(obj.m_weakRefInfo);
	}

	/** デストラクタ */
	virtual ~WeakRefPtr()
	{
		release();
	}

	/** 監視しているオブジェクトが削除されておらず、使用できるかを確認します。*/
	bool IsAlive() const
	{
		return (m_weakRefInfo != nullptr && m_weakRefInfo->owner != nullptr);
	}

	/** 監視しているオブジェクトへの RefPtr を取得します。*/
	RefPtr<T> Resolve() const
	{
		if (!IsAlive())
		{
			return RefPtr<T>();
		}
		return RefPtr<T>(static_cast<T*>(m_weakRefInfo->owner));
	}

	/** */
	WeakRefPtr<T>& operator =(const WeakRefPtr<T>& obj)
	{
		Set(obj.m_weakRefInfo);
		return *this;
	}

	/** */
	WeakRefPtr<T>& operator =(T* obj)
	{
		Set(ReflectionHelper::RequestWeakRefInfo(obj));
		return *this;
	}

private:
	
	void Set(detail::WeakRefInfo* info)
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
	virtual void InsertItem(int index, const value_type& item) override
	{
		Collection<T>::InsertItem(index, item);
		LN_SAFE_ADDREF(item);
	}
	virtual void ClearItems() override
	{
		for (auto* item : *this) {
			LN_SAFE_RELEASE(item);
		}
		Collection<T>::ClearItems();
	}
	virtual void RemoveItem(int index) override
	{
		if (Collection<T>::GetAt(index) != nullptr) {
			Collection<T>::GetAt(index)->release();
		}
		Collection<T>::RemoveItem(index);
	}
	virtual void SetItem(int index, const value_type& item) override
	{
		LN_SAFE_ADDREF(item);
		if (Collection<T>::GetAt(index) != nullptr) {
			Collection<T>::GetAt(index)->release();
		}
		Collection<T>::SetItem(index, item);
	}

private:
};

} // namespace tr

typedef tr::ReflectionObject Object;


template<class T, typename... TArgs>
RefPtr<T> NewObject(TArgs&&... args)
{
	auto ptr = RefPtr<T>(new T(), false);
	ptr->initialize(std::forward<TArgs>(args)...);
	return ptr;
}

template<class T, typename... TArgs>
void PlacementNewObject(void* ptr, TArgs&&... args)
{
	new (ptr)T();
	static_cast<T*>(ptr)->initialize(std::forward<TArgs>(args)...);
}

LN_NAMESPACE_END

