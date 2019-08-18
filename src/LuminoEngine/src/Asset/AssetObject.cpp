
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>

namespace ln {

//=============================================================================
// AssetModel

LN_OBJECT_IMPLEMENT(AssetModel, Object) {}

const String AssetModel::AssetFileExtension = u".lnasset";

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
    m_assetType = TypeInfo::getTypeInfo(target)->name();
}

ln::Result AssetModel::loadInternal(const ln::Path& filePath)
{
    String json = FileSystem::readAllText(filePath);
    JsonSerializer::deserialize(json, *this);
    return true;
}

ln::Result AssetModel::saveInternal(const ln::Path& filePath)
{
    String json = JsonSerializer::serialize(*this, JsonFormatting::Indented);
    FileSystem::writeAllText(filePath, json);
    return true;
}

void AssetModel::serialize(Archive& ar)
{
    Uuid assetId;
    if (ar.isSaving()) {
        assetId = m_target->assetId();
    }
    ar & makeNVP(u"AssetId", assetId);

    ar & makeNVP(u"AssetType", m_assetType);
    ar & makeNVP(u"Object", m_target);
    //ar & makeNVP(u"Children", m_children);

    if (ar.isLoading()) {
        m_target->setAssetId(assetId);
    }
}

ln::Uuid AssetModel::readAssetId(const ln::Path& filePath)
{
    struct Dummy
    {
        ln::Uuid id;
        void serialize(Archive& ar)
        {
            ar & makeNVP(u"AssetId", id);
        }
    } d;
    String json = FileSystem::readAllText(filePath);
    JsonSerializer::deserialize(json, d);
    return d.id;
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

void AssetModel::addChild(AssetModel* model)
{
    m_children.add(model);
}

void AssetModel::removeChild(AssetModel* model)
{
    m_children.remove(model);
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

//ln::String AssetSerializer::serialize(AssetModel* asset)
//{
//    Ploxy p{ asset };
//    return JsonSerializer::serialize(p, JsonFormatting::None);
//    //return Serializer::serialize<AssetModel>(Ref<AssetModel>(asset));
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
//        prop->accessor()->serializeMember(asset, ar, prop->name());
//    }
//}

} // namespace ln

