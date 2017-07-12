
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
	using ObjectFactory = RefPtr<ReflectionObject>(*)();

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

	// get instance factory
	template<class TObject, class TIsAbstract> struct ObjectFactorySelector
	{};
	template<class TObject> struct ObjectFactorySelector<TObject, std::false_type>
	{
		static ObjectFactory getFactory()
		{
			return []() { return RefPtr<ReflectionObject>::staticCast(newObject<TObject>()); };
		}
	};
	template<class TObject> struct ObjectFactorySelector<TObject, std::true_type>
	{
		static ObjectFactory getFactory()
		{
			return nullptr;
		}
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

	struct ConstructArg
	{
		virtual void doSet(TypeInfo* typeInfo) = 0;
	};
	template<class T>
	struct ClassVersion : public ConstructArg
	{
		ClassVersion(int version)
			: m_version(version)
		{}

		virtual void doSet(TypeInfo* typeInfo) override
		{
			typeInfo->m_serializeClassVersion = m_version;
			typeInfo->m_factory = ::tr::detail::ObjectFactorySelector<T, std::is_abstract<T>::type>::getFactory();
		}

		int m_version;
	};
	struct ConstructArgHolder
	{
		ConstructArgHolder(ConstructArg& arg)
			: m_arg(arg)
		{}
		ConstructArg& m_arg;
	};

public:
	
	/**
		@brief	指定したオブジェクトの型情報を取得します。
	*/
	static TypeInfo* getTypeInfo(const ReflectionObject* obj);

	/**
		@brief	型引数に指定したクラス型の型情報を取得します。
	*/
	template<class T>
	static TypeInfo* getTypeInfo();

	static TypeInfo* findTypeInfo(const StringRef& name);

public:
	TypeInfo(
		const TCHAR* className,
		TypeInfo* baseClass,
		HasLocalValueFlagsGetter getter,
		BindingTypeInfoSetter bindingTypeInfoSetter,
		BindingTypeInfoGetter bindingTypeInfoGetter,
		//detail::ObjectFactory factory,
		std::initializer_list<ConstructArgHolder> args);

	virtual ~TypeInfo() = default;
	
	/**
		@brief	クラス名を取得します。
	*/
	const String& getName() const;

	void registerProperty(PropertyInfo* prop);
	PropertyInfo* findProperty(const String& name) const;
	PropertyInfo* findProperty(size_t memberOffset) const;

	void registerReflectionEvent(ReflectionEventInfo* ev);
	bool invokeReflectionEvent(ReflectionObject* target, const ReflectionEventInfo* ev, ReflectionEventArgs* e);

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

	TypeInfo* getBaseClass() const { return m_baseClass; }

	///// ベースクラスも含めた全てのプロパティを列挙する
	//static void ForEachAllProperty(const TypeInfo* typeInfo, const std::function<void(Property*)>& callback);


	void setBindingTypeInfo(void* data);
	static void* getBindingTypeInfo(const ReflectionObject* obj);

	bool operator == (const TypeInfo& info) const { return m_name == info.m_name; }
	bool operator < (const TypeInfo& info) const { return m_name < info.m_name; }

	intptr_t getInternalGroup() const { return m_internalGroup; }

	void initializeProperties(ReflectionObject* obj);


	RefPtr<ReflectionObject> createInstance();
	int getSerializeClassVersion() const { return m_serializeClassVersion; }

protected:
	void setInternalGroup(intptr_t group) { m_internalGroup = group; }

private:
	//typedef SortedArray<const RoutedEvent*, RoutedEventHandler*>	RoutedEventHandlerList;

	String						m_name;						// クラス名
	TypeInfo*					m_baseClass;				// 継承元クラスを示す TypeInfo
	List<PropertyInfo*>			m_propertyList;				// この型のクラスがもつプロパティのリスト
	List<ReflectionEventInfo*>	m_routedEventList;			// この型のクラスがもつReflectionEventのリスト
	BindingTypeInfoSetter		m_bindingTypeInfoSetter;
	BindingTypeInfoGetter		m_bindingTypeInfoGetter;
	detail::ObjectFactory		m_factory;

	int							m_serializeClassVersion;

	intptr_t					m_internalGroup;
};

//------------------------------------------------------------------------------
template<class T>
inline TypeInfo* TypeInfo::getTypeInfo()
{
	return ReflectionHelper::getTypeInfo<T>();
}

} // namespace tr
LN_NAMESPACE_END
