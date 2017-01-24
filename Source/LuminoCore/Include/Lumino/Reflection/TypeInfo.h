
#pragma once
#include "Common.h"
#include "../Base/RefObject.h"
#include "../Base/String.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class TypeInfo;
class ReflectionObject;
class PropertyInfo;
typedef uint32_t LocalValueHavingFlags;

namespace detail
{
	// 1つの ReflectionObject に対して1つ作られる
	class WeakRefInfo final
	{
	public:

		RefObject*			owner;
		std::atomic<int>	weakRefCount;// = 1;	// GCC で使えなかった
		//int	weakRefCount;

		WeakRefInfo();
		void AddRef();
		void Release();
	};
}

class ReflectionHelper
{
public:
	template<class T>
	static ln::tr::LocalValueHavingFlags* GetLocalValueHavingFlags(ReflectionObject* thisObj)
	{
		return &static_cast<T*>(thisObj)->lnref_localValueHavingFlags;
	}
	template<class T>
	static TypeInfo* GetClassTypeInfo()
	{
		return &T::lnref_typeInfo;
	}
	template<class T>
	static void SetBindingTypeInfo(void* data)
	{
		T::lnref_bindingTypeInfo = data;
	}
	template<class T>
	static void* GetBindingTypeInfo()
	{
		return T::lnref_bindingTypeInfo;
	}
	template<class T>
	static TypeInfo* GetTypeInfo(const T* obj)
	{
		return obj->lnref_GetThisTypeInfo();
	}
	template<class T>
	inline static TypeInfo* GetTypeInfo()
	{
		return &T::lnref_typeInfo;
	}

	template<class T>
	inline static detail::WeakRefInfo* RequestWeakRefInfo(T* obj)
	{
		return obj->RequestWeakRefInfo();
	}
	template<class T, class TData>
	inline static TData* RequestAnimationData(T* obj)
	{
		if (obj->m_animationData == nullptr)
		{
			obj->m_animationData = LN_NEW TData();
		}
		return static_cast<TData*>(obj->m_animationData);
	}

	static void AddGCObject(ReflectionObject* obj, ReflectionObject* child);
	static void RemoveGCObject(ReflectionObject* obj, ReflectionObject* child);
	static void GCObjects(ReflectionObject* obj);
	static bool IsGCReady(ReflectionObject* obj);

	template<class TList>
	inline static void GCObjectList(TList* list)
	{
		list->RemoveAll([](typename TList::value_type& obj) { return IsGCReady(obj); });
	}
};

/**
	@brief		
*/
class TypeInfo
{
public:
	typedef LocalValueHavingFlags* (*HasLocalValueFlagsGetter)(ReflectionObject* _this);
	typedef void (*BindingTypeInfoSetter)(void* data);
	typedef void* (*BindingTypeInfoGetter)();
	typedef uint8_t RevisionCount;
	static const int MaxProperties = sizeof(LocalValueHavingFlags) * 8;

public:
	
	/**
		@brief	指定したオブジェクトの型情報を取得します。
	*/
	static TypeInfo* GetTypeInfo(const ReflectionObject* obj);

	/**
		@brief	型引数に指定したクラス型の型情報を取得します。
	*/
	template<class T>
	static TypeInfo* GetTypeInfo();

public:
	TypeInfo(
		const TCHAR* className,
		TypeInfo* baseClass,
		HasLocalValueFlagsGetter getter,
		BindingTypeInfoSetter bindingTypeInfoSetter,
		BindingTypeInfoGetter bindingTypeInfoGetter);

	virtual ~TypeInfo() = default;
	
	/**
		@brief	クラス名を取得します。
	*/
	const String& GetName() const;

	void RegisterProperty(PropertyInfo* prop);
	PropertyInfo* FindProperty(const String& name) const;
	PropertyInfo* FindProperty(size_t memberOffset) const;

	void RegisterReflectionEvent(ReflectionEventInfo* ev);
	bool InvokeReflectionEvent(ReflectionObject* target, const ReflectionEventInfo* ev, ReflectionEventArgs* e);

	//// childObjProp が継承できるプロパティをこの TypeInfo から探す。見つからなければ NULL を返す。
	//// childObj : childObjProp を持つオブジェクト
	//Property* FindInheritanceProperty(const Property* childObjProp, CoreObject* childObj) const;

	//void RegisterRoutedEvent(RoutedEvent* ev);
	//RoutedEvent* FindRoutedEvent(const String& name) const;	// TODO: いらないかも
	//void InvokeRoutedEvent(CoreObject* owner, const RoutedEvent* ev, RoutedEventArgs* e);

	///// RoutedEventHandler は、ユーザーが動的に追加できるハンドラよりも前に呼び出される。
	///// WPF では「静的ハンドラ」と呼ばれている。動的イベントに登録するのに比べ、メモリを使用しない。
	//void RegisterRoutedEventHandler(const RoutedEvent* ev, RoutedEventHandler* handler);
	//RoutedEventHandler* FindRoutedEventHandler(const RoutedEvent* ev) const;

	TypeInfo* GetBaseClass() const { return m_baseClass; }

	///// ベースクラスも含めた全てのプロパティを列挙する
	//static void ForEachAllProperty(const TypeInfo* typeInfo, const std::function<void(Property*)>& callback);


	void SetBindingTypeInfo(void* data);
	static void* GetBindingTypeInfo(const ReflectionObject* obj);

	bool operator == (const TypeInfo& info) const { return m_name == info.m_name; }
	bool operator < (const TypeInfo& info) const { return m_name < info.m_name; }

	intptr_t GetInternalGroup() const { return m_internalGroup; }

	void InitializeProperties(ReflectionObject* obj);

protected:
	void SetInternalGroup(intptr_t group) { m_internalGroup = group; }

private:
	//typedef SortedArray<const RoutedEvent*, RoutedEventHandler*>	RoutedEventHandlerList;

	String						m_name;						// クラス名
	TypeInfo*					m_baseClass;				// 継承元クラスを示す TypeInfo
	List<PropertyInfo*>			m_propertyList;				// この型のクラスがもつプロパティのリスト
	List<ReflectionEventInfo*>	m_routedEventList;			// この型のクラスがもつReflectionEventのリスト
	BindingTypeInfoSetter		m_bindingTypeInfoSetter;
	BindingTypeInfoGetter		m_bindingTypeInfoGetter;

	intptr_t					m_internalGroup;
};

//------------------------------------------------------------------------------
template<class T>
inline TypeInfo* TypeInfo::GetTypeInfo()
{
	return ReflectionHelper::GetTypeInfo<T>();
}

} // namespace tr
LN_NAMESPACE_END
