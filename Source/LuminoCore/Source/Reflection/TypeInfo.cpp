
#include "../Internal.h"
#include <Lumino/Reflection/Notify.h>
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/ReflectionEventArgs.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/Property.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// ReflectionHelper
//==============================================================================
//------------------------------------------------------------------------------
//void ReflectionHelper::RaiseReflectionEvent(ReflectionObject* obj, ReflectionEventBase* ev, ReflectionEventArgs* e)
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
TypeInfo* TypeInfo::GetTypeInfo(const ReflectionObject* obj)
{
	return ReflectionHelper::GetTypeInfo(obj);
}

//------------------------------------------------------------------------------
TypeInfo::TypeInfo(
	const TCHAR* className,
	TypeInfo* baseClass,
	HasLocalValueFlagsGetter getter,
	BindingTypeInfoSetter bindingTypeInfoSetter,
	BindingTypeInfoGetter bindingTypeInfoGetter)
	: m_name(className)
	, m_baseClass(baseClass)
	, m_bindingTypeInfoSetter(bindingTypeInfoSetter)
	, m_bindingTypeInfoGetter(bindingTypeInfoGetter)
	, m_internalGroup(0)
{
}

//------------------------------------------------------------------------------
const String& TypeInfo::GetName() const
{
	return m_name;
}

//------------------------------------------------------------------------------
void TypeInfo::RegisterProperty(PropertyInfo* prop)
{
	LN_CHECK_ARG(!prop->m_registerd);
	m_propertyList.Add(prop);
	prop->m_registerd = true;
}

//------------------------------------------------------------------------------
PropertyInfo* TypeInfo::FindProperty(size_t memberOffset) const
{
	for (PropertyInfo* prop : m_propertyList)
	{
		if (prop->m_memberOffset == memberOffset)
			return prop;
	}
	LN_CHECK_STATE(0);
	return nullptr;
}

//------------------------------------------------------------------------------
void TypeInfo::RegisterReflectionEvent(ReflectionEventInfo* ev)
{
	LN_CHECK_ARG(!ev->m_registerd);
	m_routedEventList.Add(ev);
	ev->m_registerd = true;
}

//------------------------------------------------------------------------------
bool TypeInfo::InvokeReflectionEvent(ReflectionObject* target, const ReflectionEventInfo* ev, ReflectionEventArgs* e)
{
	for (ReflectionEventInfo* dynamicEvent : m_routedEventList)
	{
		if (dynamicEvent == ev)
		{
			// owner に AddHandler されているイベントハンドラを呼び出す。
			dynamicEvent->CallEvent(target, e);
			return e->handled;	// ev と同じイベントは1つしかリスト内に無いはずなのですぐ return
		}
	}

	// ベースクラスがあれば、さらにベースクラスを見に行く
	if (m_baseClass != nullptr)
	{
		return m_baseClass->InvokeReflectionEvent(target, ev, e);
	}
	return false;
}

//------------------------------------------------------------------------------
void TypeInfo::SetBindingTypeInfo(void* data)
{
	m_bindingTypeInfoSetter(data);
}

//------------------------------------------------------------------------------
void* TypeInfo::GetBindingTypeInfo(const ReflectionObject* obj)
{
	TypeInfo* type = GetTypeInfo(obj);
	return type->m_bindingTypeInfoGetter();
}

//------------------------------------------------------------------------------
void TypeInfo::InitializeProperties(ReflectionObject* obj)
{
	for (PropertyInfo* info : m_propertyList)
	{
		PropertyBase* prop = info->GetPropertyBase(obj);
		prop->m_owner = obj;
		prop->m_propId = info;
	}
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
void WeakRefInfo::AddRef()
{
	weakRefCount.fetch_add(1, std::memory_order_relaxed);
	//weakRefCount++;
}

//------------------------------------------------------------------------------
void WeakRefInfo::Release()
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

