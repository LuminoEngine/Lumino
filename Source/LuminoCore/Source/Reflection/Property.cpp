
#include "../Internal.h"
#include <Lumino/Reflection/TypeInfo.h>
#include <Lumino/Reflection/Property.h>
#include <Lumino/Reflection/EventArgsPool.h>

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// PropertyInfo
//==============================================================================
static EventArgsPool g_eventArgsPool;

//------------------------------------------------------------------------------
PropertyInfo::PropertyInfo(TypeInfo* ownerClassType, PropertyMetadata* metadata, bool stored)
	: m_ownerClassType(ownerClassType)
	, m_metadata()
	, m_memberOffset(0)
	, m_stored(stored)
	, m_registerd(false)
{
	m_ownerClassType->registerProperty(this);
}

//------------------------------------------------------------------------------
PropertyInfo::~PropertyInfo()
{
}

//------------------------------------------------------------------------------
void PropertyInfo::notifyPropertyChanged(Object* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source)
{
	Ref<PropertyChangedEventArgs> e(g_eventArgsPool.create<PropertyChangedEventArgs>(prop, source), false);
	//target->onPropertyChanged(e);
	//target->CallListener(e);
	//prop->m_metadata->CallPropertyChangedCallback(target, e);
	if (ownerObject != nullptr && prop != nullptr)	// initializeProperties されていないとこれらは設定されない
	{
		ownerObject->onPropertyChanged(e);

		if (!e->handled)
		{
			prop->getMetadata().callStaticPropertyChanged(ownerObject);
		}
	}
}

#ifdef LN_LEGACY_VARIANT_ENABLED
//------------------------------------------------------------------------------
void PropertyInfo::setPropertyValue(Object* obj, const PropertyInfo* prop, const Variant& value, PropertySetSource source)
{
	obj->setPropertyValueInternal(prop, value, false, source);
}

//------------------------------------------------------------------------------
Variant PropertyInfo::getPropertyValue(Object* obj, const PropertyInfo* prop)
{
	//if (prop->isStored())
	//{
	//	if (m_propertyDataStore == NULL) {
	//		return prop->getMetadata()->getDefaultValue();
	//	}
	//	//LN_THROW(m_propertyDataStore != NULL, KeyNotFoundException);
	//	Variant v;
	//	if (m_propertyDataStore->TryGetValue(prop, &v)) {
	//		return v;
	//	}
	//	return prop->getMetadata()->getDefaultValue();
	//}
	//else
	//{
	//	// この const_cast は、外に公開する Getter はとにかく const 関数にしたかったためのもの。
	//	UpdateInheritanceProperty(const_cast<CoreObject*>(this), prop);
		return prop->getValue(obj);
	//}
}
#endif

//==============================================================================
// PropertyBase
//==============================================================================
//------------------------------------------------------------------------------
//void PropertyBase::CallListener(PropertyChangedEventArgs* e) const
//{
//	if (m_staticListenerOwner != nullptr && m_staticListener != nullptr)
//	{
//		 m_staticListener(m_staticListenerOwner, e);
//	}
//
//	for (IPropertyChangedListener* listener : m_listeners) listener->onPropertyChanged(e);
//}

} // namespace tr
LN_NAMESPACE_END
