
#include "../Internal.h"
#include <unordered_map>
#include <map>
#include <Lumino/Reflection/Notify.h>
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/ReflectionEventArgs.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/Property.h>

LN_NAMESPACE_BEGIN
namespace tr {


namespace detail
{
	// get instance factory
	template<class TObject, class TIsAbstract> struct ObjectFactorySelector
	{};
	template<class TObject> struct ObjectFactorySelector<TObject, std::false_type>
	{
		static ObjectFactory getFactory()
		{
			return []() { return Ref<ReflectionObject>::staticCast(::ln::newObject<TObject>()); };
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

//==============================================================================
// TypeInfoManager
//==============================================================================
class TypeInfoManager
{
public:
	// VS2017 で map や unordered_map を static 領域に置くと落ちてしまうようなので、インスタンスはヒープに置く
	static TypeInfoManager* getInstance()
	{
		//if (m_instance == nullptr)
		//	m_instance = std::make_shared<TypeInfoManager>();
		static TypeInfoManager m_instance;
		return &m_instance;
		//return m_instance.get();
	}

	void RegisterTypeInfo(TypeInfo* typeInfo)
	{
		//mp = std::make_shared<std::unordered_map<std::string, int>>();
		//(*mp)["abc"] = 123;
		//m_typeInfo.insert(std::pair<String, TypeInfo*>(typeInfo->getName(), typeInfo));
		m_typeInfo[typeInfo->getName()] = typeInfo;
		//mp["abc"] = 123;
	}

	TypeInfo* findTypeInfo(const StringRef& typeName)
	{
		auto itr = m_typeInfo.find(typeName);
		return (itr != m_typeInfo.end()) ? itr->second : nullptr;
	}

	~TypeInfoManager()
	{

	}

private:
	//static std::shared_ptr<TypeInfoManager>	m_instance;

		//std::unordered_map<std::string, int>
	 //mp;    //  文字列 → 整数 のハッシュ連想配列
	std::unordered_map<String, TypeInfo*>	m_typeInfo;
	//std::map<std::string, TypeInfo*>	m_typeInfo;
};

//std::shared_ptr<TypeInfoManager> TypeInfoManager::m_instance;

//static TypeInfoManager g_typeInfoManager;

//==============================================================================
// ReflectionHelper
//==============================================================================
//------------------------------------------------------------------------------
//void ReflectionHelper::raiseReflectionEvent(ReflectionObject* obj, ReflectionEventBase* ev, ReflectionEventArgs* e)
//{
//	ev->Raise(e);
//}

//==============================================================================
// TypeInfo
//==============================================================================

/*
	HasLocalValueFlagsGetter はクラスごとに必要となる。
	ReflectionObject に仮想関数を持たせて取得するわけには行かない。
	例えば、
		ReflectionObject
			UIElement
				Control
					Button
	という継承関係があった場合、UIElement の LocalFlags は派生元・先関係なく
	純粋に UIElement に定義されたプロパティの LocalFlags がほしい。
*/

//------------------------------------------------------------------------------
TypeInfo* TypeInfo::getTypeInfo(const ReflectionObject* obj)
{
	return ReflectionHelper::getTypeInfo(obj);
}

//------------------------------------------------------------------------------
TypeInfo::TypeInfo(
	const Char* className,
	TypeInfo* baseClass,
	HasLocalValueFlagsGetter getter,
	BindingTypeInfoSetter bindingTypeInfoSetter,
	BindingTypeInfoGetter bindingTypeInfoGetter,
	//detail::ObjectFactory factory,
	std::initializer_list<ConstructArgHolder> args)
	: m_name(className)
	, m_baseClass(baseClass)
	, m_bindingTypeInfoSetter(bindingTypeInfoSetter)
	, m_bindingTypeInfoGetter(bindingTypeInfoGetter)
	, m_factory(nullptr)
	, m_serializeClassVersion(0)
	, m_internalGroup(0)
{
	TypeInfoManager::getInstance()->RegisterTypeInfo(this);
	for (auto& arg : args) arg.m_arg.doSet(this);
}

//------------------------------------------------------------------------------
TypeInfo* TypeInfo::findTypeInfo(const StringRef& name)
{
	return TypeInfoManager::getInstance()->findTypeInfo(name);
}

//------------------------------------------------------------------------------
const String& TypeInfo::getName() const
{
	return m_name;
}

//------------------------------------------------------------------------------
void TypeInfo::registerProperty(PropertyInfo* prop)
{
	if (LN_REQUIRE(!prop->m_registerd)) return;
	m_propertyList.add(prop);
	prop->m_registerd = true;
}

//------------------------------------------------------------------------------
PropertyInfo* TypeInfo::findProperty(size_t memberOffset) const
{
	for (PropertyInfo* prop : m_propertyList)
	{
		if (prop->m_memberOffset == memberOffset)
			return prop;
	}

	LN_UNREACHABLE();
	return nullptr;
}

//------------------------------------------------------------------------------
void TypeInfo::registerReflectionEvent(ReflectionEventInfo* ev)
{
	if (LN_REQUIRE(!ev->m_registerd)) return;
	m_routedEventList.add(ev);
	ev->m_registerd = true;
}

//------------------------------------------------------------------------------
bool TypeInfo::invokeReflectionEvent(ReflectionObject* target, const ReflectionEventInfo* ev, ReflectionEventArgs* e)
{
	for (ReflectionEventInfo* dynamicEvent : m_routedEventList)
	{
		if (dynamicEvent == ev)
		{
			// owner に addHandler されているイベントハンドラを呼び出す。
			dynamicEvent->callEvent(target, e);
			return e->handled;	// ev と同じイベントは1つしかリスト内に無いはずなのですぐ return
		}
	}

	// ベースクラスがあれば、さらにベースクラスを見に行く
	if (m_baseClass != nullptr)
	{
		return m_baseClass->invokeReflectionEvent(target, ev, e);
	}
	return false;
}

//------------------------------------------------------------------------------
void TypeInfo::setBindingTypeInfo(void* data)
{
	m_bindingTypeInfoSetter(data);
}

//------------------------------------------------------------------------------
void* TypeInfo::getBindingTypeInfo(const ReflectionObject* obj)
{
	TypeInfo* type = getTypeInfo(obj);
	return type->m_bindingTypeInfoGetter();
}

//------------------------------------------------------------------------------
void TypeInfo::initializeProperties(ReflectionObject* obj)
{
	for (PropertyInfo* info : m_propertyList)
	{
		PropertyBase* prop = info->getPropertyBase(obj);
		prop->m_owner = obj;
		prop->m_propId = info;
	}
}

//------------------------------------------------------------------------------
Ref<Object> TypeInfo::createInstance()
{
	if (LN_REQUIRE(m_factory != nullptr)) return nullptr;
	return m_factory();
}

namespace detail
{

//==============================================================================
// WeakRefInfo
//==============================================================================

//------------------------------------------------------------------------------
WeakRefInfo::WeakRefInfo()
	: owner(nullptr)
	, weakRefCount(1)
{}

//------------------------------------------------------------------------------
void WeakRefInfo::addRef()
{
	weakRefCount.fetch_add(1, std::memory_order_relaxed);
	//weakRefCount++;
}

//------------------------------------------------------------------------------
void WeakRefInfo::release()
{
	int before = weakRefCount.fetch_sub(1, std::memory_order_relaxed);
	//int before = weakRefCount;
	//weakRefCount--;
	if (before <= 1)
	{
		delete this;
	}
}

} // namespace detail

} // namespace tr
LN_NAMESPACE_END

