
#include "Internal.h"
#include <Lumino/Variant.h>
#include <Lumino/Property.h>

namespace Lumino
{

//=============================================================================
// PropertyManager
//=============================================================================

PropertyManager::PropertyMap	PropertyManager::m_propertyMap;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
/*AttachedProperty* */ void PropertyManager::RegisterAttachedProperty(TypeInfo* ownerClass, const String& propertyName, const Variant& defaultValue)
{
#if 0
	TypedNameKey key(ownerClass, propertyName);
	PropertyMap::iterator itr = m_propertyMap.find(key);
	if (itr == m_propertyMap.end())
	{
		std::shared_ptr<AttachedProperty> prop(LN_NEW AttachedProperty(propertyName, defaultValue));
		m_propertyMap[key] = prop;
		return prop.get();
	}
	return itr->second.get();
#endif;
}

} // namespace Lumino
