/*
	[2015/12/7] LN_TR_PROPERTY
		metadata はメンバに持っておいたほうが後々都合がよさそう。
		Property はこれのポインタを参照する。実体は持たない。
		いずれ metadata の継承とかオーバーライドとかやるときは、ポインタのほうが融通効くかも。

*/
#include "../Internal.h"
#include <Lumino/Reflection/Notify.h>
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/ReflectionArrayObject.h>
#include <Lumino/Reflection/ReflectionEventArgs.h>
#include <Lumino/Reflection/Property.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// ReflectionHelper
//==============================================================================
//------------------------------------------------------------------------------
void ReflectionHelper::addGCObject(ReflectionObject* obj, ReflectionObject* child)
{
	if (obj == nullptr || child == nullptr) return;
	obj->m_gcList.add(child);
}

//------------------------------------------------------------------------------
void ReflectionHelper::removeGCObject(ReflectionObject* obj, ReflectionObject* child)
{
	if (obj == nullptr || child == nullptr) return;
	obj->m_gcList.remove(child);
}

//------------------------------------------------------------------------------
void ReflectionHelper::gcObjects(ReflectionObject* obj)
{
	if (obj == nullptr) return;
	obj->m_gcList.removeAll([](Ref<ReflectionObject>& obj) { return isGCReady(obj); });
}

//------------------------------------------------------------------------------
bool ReflectionHelper::isGCReady(ReflectionObject* obj)
{
	if (obj == nullptr) return false;
	return obj->m_autoGC && obj->getReferenceCount() == 1;
}

//==============================================================================
// ReflectionObject
//==============================================================================
ln::tr::TypeInfo				ReflectionObject::lnref_typeInfo(_T("ReflectionObject"), nullptr, nullptr, nullptr, nullptr, {});
ln::tr::TypeInfo*				ReflectionObject::lnref_GetThisTypeInfo() const { return &lnref_typeInfo; }

//------------------------------------------------------------------------------
ReflectionObject::ReflectionObject()
	: m_userData(nullptr)
	, m_weakRefInfo(nullptr)
	, m_autoGC(true)
{
}

//------------------------------------------------------------------------------
ReflectionObject::~ReflectionObject()
{
	MutexScopedLock lock(m_weakRefInfoMutex);
	if (m_weakRefInfo != nullptr)
	{
		m_weakRefInfo->owner = nullptr;
		m_weakRefInfo->release();
		m_weakRefInfo = nullptr;
	}
}

//------------------------------------------------------------------------------
void ReflectionObject::raiseReflectionEvent(const ReflectionEventBase& ev, ReflectionEventArgs* args)
{
	ev.raise(args);
}

//------------------------------------------------------------------------------
void ReflectionObject::onPropertyChanged(PropertyChangedEventArgs* e)
{
	// e->Property を持つクラスのコールバックを呼び出す
	//e->changedProperty->NotifyPropertyChange(this, e);
}

//------------------------------------------------------------------------------
#ifdef LN_LEGACY_VARIANT_ENABLED
void ReflectionObject::setPropertyValueInternal(const PropertyInfo* prop, const Variant& value, bool reset, PropertySetSource source)
{
	//if (prop->isStored())
	//{
	//	// 必要になったので作る
	//	if (m_propertyDataStore == NULL) { m_propertyDataStore = LN_NEW PropertyDataStore(); }
	//	m_propertyDataStore->setValue(prop, value);
	//}
	//else {
		prop->setValue(this, value, source);
	//}

	//PropertyInstanceData* data = prop->GetPropertyInstanceData(this);
	//if (data != NULL)
	//{
	//	if (reset) {
	//		data->IsDefault = true;
	//	}
	//	else
	//	{
	//		if (data->IsDefault == true)
	//		{
	//			// 新しく設定される瞬間、これまで継承元として参照していたプロパティと this に対して
	//			// プロパティ参照更新値を1つ進める。子は Get しようとしたとき、継承元を再検索する。
	//			// TODO: SetTypedPropertyValue にも同じようなのがある。共通化したい。あとテスト
	//			if (data->InheritanceParent != NULL) {
	//				data->InheritanceTarget->GetPropertyInstanceData(data->InheritanceParent)->PathRevisionCount++;
	//			}
	//			data->PathRevisionCount++;
	//		}
	//		data->IsDefault = false;
	//		data->RevisionCount++;
	//	}
	//}

	//setPropertyValue(prop->getName(), value);	// TODO: getName じゃなくて、型情報も考慮するように。あるいは生ポインタ
}
#endif

//------------------------------------------------------------------------------
detail::WeakRefInfo* ReflectionObject::requestWeakRefInfo()
{
	MutexScopedLock lock(m_weakRefInfoMutex);
	if (m_weakRefInfo == nullptr)
	{
		m_weakRefInfo = LN_NEW detail::WeakRefInfo();
		m_weakRefInfo->owner = this;
	}
	return m_weakRefInfo;
}


//==============================================================================
// ReflectionArrayObject
//==============================================================================

//------------------------------------------------------------------------------
ReflectionArrayObject::ReflectionArrayObject()
{
}

//------------------------------------------------------------------------------
ReflectionArrayObject::~ReflectionArrayObject()
{
}

} // namespace tr
LN_NAMESPACE_END

