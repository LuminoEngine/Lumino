
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include "EngineManager.hpp"

namespace ln {

//==============================================================================
// PropertyRef

void PropertyRef_old::clearValue()
{
    auto ptr = m_propOwner.resolve();
    if (ptr != nullptr) {
        return m_prop->clearValue();
    }
}

Ref<Object> PropertyRef_old::owenr()
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
    //// TODO: ベースクラス
    //TypeInfo* info = getTypeInfo(obj);
    //if (info)
    //{
    //    for (auto& propInfo : info->m_properties)
    //    {
    //        if (PropertyBase* prop = propInfo->m_getPropertyCallback(obj)) {
    //            prop->m_owner = obj;
    //            prop->m_propertyInfo = propInfo;
    //        }
    //    }
    //}
}

//==============================================================================
// PropertyInfo

PropertyRef_old PropertyInfo::getPropertyRef_old(Object* obj, PropertyInfo* propertyInfo)
{
    return PropertyRef_old(obj, propertyInfo->m_getPropertyCallback(obj));
}

PropertyRef PropertyInfo::getPropertyRef(Object* obj, PropertyInfo* propertyInfo)
{
	return PropertyRef(obj, propertyInfo->m_accessor);
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


//==============================================================================
// PropertyPath

PropertyRef PropertyPath::findProperty(Object* root, const PropertyPath* path)
{
	class LocalVisitor : public ReflectionObjectVisitor
	{
	public:
		const PropertyPath* path;
		PropertyRef ref;
		virtual bool visitProperty(Object* obj, PropertyInfo* prop)
		{
			if (path->m_propertyName == prop->name()) {
				ref = PropertyRef(obj, prop->accessor());
				return true;
			}
			return true;
		}
	};

	LocalVisitor visitor;
	visitor.path = path;
	root->traverseRefrection(&visitor);
	return visitor.ref;
}

//==============================================================================
// EngineContext

bool ReflectionObjectVisitor::visitProperty(Object* obj, PropertyInfo* prop)
{
	return true;
}

} // namespace ln

