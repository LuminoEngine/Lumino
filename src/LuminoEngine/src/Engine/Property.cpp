
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include "EngineManager.hpp"

namespace ln {

//==============================================================================
// PropertyRef

void PropertyRef::clearValue()
{
    auto ptr = m_propOwner.resolve();
    if (ptr != nullptr) {
        return m_prop->clearValue();
    }
}

Ref<Object> PropertyRef::owenr()
{
    return m_propOwner.resolve();
}

//==============================================================================
// TypeInfo

void TypeInfo::registerProperty(PropertyInfo* prop)
{
    if (LN_REQUIRE(!prop->m_registerd)) return;
    m_properties.add(prop);
    prop->m_registerd = true;
}

void TypeInfo::initializeObjectProperties(Object* obj)
{
    // TODO: ベースクラス
    TypeInfo* info = getTypeInfo(obj);
    if (info)
    {
        for (auto& propInfo : info->m_properties)
        {
            if (PropertyBase* prop = propInfo->m_getPropertyCallback(obj)) {
                prop->m_owner = obj;
                prop->m_propertyInfo = propInfo;
            }
        }
    }
}

//==============================================================================
// PropertyInfo

PropertyRef PropertyInfo::getPropertyRef(Object* obj, PropertyInfo* propertyInfo)
{
    return PropertyRef(obj, propertyInfo->m_getPropertyCallback(obj));
}

void PropertyInfo::notifyPropertyChanged(Object* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source)
{
    if (prop->m_staticPropertyChangedCallback) {
        prop->m_staticPropertyChangedCallback(ownerObject);
    }
}

//==============================================================================
// EngineContext

EngineContext* EngineContext::current()
{
	return detail::EngineDomain::engineManager()->engineContext();
}

} // namespace ln

