
#include "Internal.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include <LuminoEngine/Base/Serializer.hpp>

namespace ln {

//=============================================================================
// AssetModel

LN_OBJECT_IMPLEMENT(AssetModel, Object) {}

// 現在はエディタサポートが十分ではない。Asset は手書きも頻発する。
// 独自拡張子だとテキストエディタのサポートが受けづらいので今は使わない。
const String AssetModel::AssetFileExtension = _TT(".yml");//_TT(".lnasset";

//Ref<AssetModel> AssetModel::create(Object* target)
//{
//	return makeObject<AssetModel>(target);
//}

const String AssetModel::EngineAssetsDirectory = _TT("Engine");

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

const ln::TypeInfo* AssetModel::assetType() const
{
    return TypeInfo::getTypeInfo(m_target);
}

//ln::Result AssetModel::loadInternal(const ln::Path& filePath)
//{
//    String json = FileSystem::readAllText(filePath);
//    JsonSerializer::deserialize(json, filePath.parent(), *this);
//    m_assetFilePath = filePath;
//    //m_target->onSetAssetFilePath(m_assetFilePath);
//    return true;
//}
//
//ln::Result AssetModel::saveInternal(const ln::Path& filePath)
//{
//    String json = JsonSerializer::serialize(*this, JsonFormatting::Indented);
//    FileSystem::writeAllText(filePath, json);
//    return true;
//}

//void AssetModel::serialize(Archive& ar)
//{
//    //String assetId;
//    //if (ar.isSaving()) {
//    //    assetId = m_target->assetPath();
//    //}
//    //ar & makeNVP(_TT("AssetId", assetId);
//
//    ar & makeNVP(_TT("AssetType", m_assetType);
//    ar & makeNVP(_TT("Object", m_target);
//    //ar & makeNVP(_TT("Children", m_children);
//
//    //if (ar.isLoading()) {
//    //    m_target->setAssetPath(assetId);
//    //}
//}
//
void AssetModel::onSerialize(Serializer2* sr)
{
    if (sr->isSaving()) {
        sr->writeName(_TT("object"));
        sr->writeObject(m_target);
    }
    else {
        if (m_externalObjectDeserialization) {
            if (LN_REQUIRE(m_target)) return;
            if (sr->readName(_TT("object"))) sr->readObject(m_target);
        }
        else {
            if (sr->readName(_TT("object"))) m_target = sr->readObject();
        }
    }
}

ln::Uuid AssetModel::readAssetId(const ln::Path& filePath)
{
    struct Dummy
    {
        ln::Uuid id;
        void serialize(Archive& ar)
        {
            ar & makeNVP(_TT("AssetId"), id);
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

