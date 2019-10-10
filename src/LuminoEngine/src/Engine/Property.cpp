
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include "EngineManager.hpp"

namespace ln {

//==============================================================================
// TypeInfo

void TypeInfo::registerProperty(PropertyInfo* prop)
{
    if (LN_REQUIRE(!prop->m_registerd)) return;
    m_properties.add(prop);
    prop->m_registerd = true;
}

Ref<Object> TypeInfo::createInstance() const
{
	return m_factory();
}

Ref<Object> TypeInfo::createInstance(const String& typeName)
{
	if (TypeInfo* info = EngineContext::current()->findTypeInfo(typeName)) {
		return info->createInstance();
	}
	else {
        // TODO: Objcet.hpp の serialize の TODO にもあるが、切り替えできるようにしたい
        LN_LOG_WARNING << " Not found type.　(" << typeName << ")";
		return nullptr;
	}
}

void TypeInfo::initializeObjectProperties(Object* obj)
{
    // TODO: ベースクラス
    TypeInfo* info = getTypeInfo(obj);
    if (info)
    {
        for (auto& propInfo : info->m_properties)
        {
			if (propInfo->m_getPropertyCallback)
			{
				if (PropertyBase* prop = propInfo->m_getPropertyCallback(obj)) {
					prop->m_owner = obj;
					prop->m_propertyInfo = propInfo;
				}
			}
        }
    }
}

//==============================================================================
// PropertyInfo

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
			return false;
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

