
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//=============================================================================
// AssetModel

LN_OBJECT_IMPLEMENT(AssetModel, Object) {}

Ref<AssetModel> AssetModel::create(Object* target)
{
	return makeObject<AssetModel>(target);
}

AssetModel::AssetModel()
    : m_target(nullptr)
	, m_parent(nullptr)
    , m_children()
{
}

void AssetModel::init()
{
    Object::init();
}

void AssetModel::init(Object* target)
{
	init();
	m_target = target;
}

Object* AssetModel::target() const
{
    return m_target;
}

Ref<AssetProperty> AssetModel::findProperty(const String& path)
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


//=============================================================================
// AssetSerializer
//
//ln::String AssetSerializer::serialize(AssetModel* asset)
//{
//    Serializer::serialize<AssetModel>(Ref<AssetModel>(asset));
//}
//
//Ref<AssetModel> AssetSerializer::deserialize(const String& str)
//{
//    return Serializer::deserialize<AssetModel>(str);
//}
//
//void AssetSerializer::processAsset(AssetModel* asset)
//{
//
//}
//
//void AssetSerializer::Ploxy::serialize(Archive& ar)
//{
//    const List<Ref<PropertyInfo>>& props = TypeInfo::getTypeInfo(asset)->properties();
//    for (auto& prop : props) {
//        prop->accessor()->serializeMember(asset, ar, prop->name);
//    }
//}

} // namespace ln

