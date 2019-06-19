
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//=============================================================================
// AssetObject

LN_OBJECT_IMPLEMENT(AssetObject, Object) {}

Ref<AssetObject> AssetObject::create(Object* target)
{
	return makeObject<AssetObject>(target);
}

AssetObject::AssetObject()
    : m_target(nullptr)
	, m_parent(nullptr)
    , m_children()
{
}

void AssetObject::init()
{
    Object::init();
}

void AssetObject::init(Object* target)
{
	init();
	m_target = target;
}

Ref<AssetProperty> AssetObject::findProperty(const String& path)
{
	PropertyPath pp;
	pp.m_propertyName = path;	// TODO:
	auto ref = PropertyPath::findProperty(m_target, &pp);
	if (ref.isNull()) {
		return nullptr;
	}
	else {
		return makeObject<AssetProperty>(ref);
	}
}


//=============================================================================
// AssetProperty

AssetProperty::AssetProperty()
{
}

void AssetProperty::init(const PropertyRef& prop)
{
	Object::init();
	m_property = prop;
}

} // namespace ln

