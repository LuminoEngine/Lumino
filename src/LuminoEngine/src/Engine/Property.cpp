
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>

namespace ln {

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

void PropertyInfo::notifyPropertyChanged(Object* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source)
{
    if (prop->m_staticPropertyChangedCallback) {
        prop->m_staticPropertyChangedCallback(ownerObject);
    }
}

} // namespace ln

