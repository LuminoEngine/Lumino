
#include "Internal.hpp"
#include "../../../../lumino/LuminoCore/src/IO/PathHelper.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Base/MixHash.hpp>

namespace ln {

//==============================================================================
// Assets

//void Assets::setAssetPath(AssetObject* obj, const String& filePath)
//{
//    obj->setAssetPath(filePath);
//}
//
//const Path& Assets::getAssetPath(AssetObject* obj)
//{
//    return obj->assetPath();
//}
//
void Assets::save(AssetObject* obj)
{
    LN_NOTIMPLEMENTED();
    //Assets::saveAsset(obj, obj->m_assetFilePath);
}

void Assets::reload(AssetObject* obj)
{
    obj->reload();
}

void Assets::saveAssetToLocalFile(AssetModel* asset, const String& filePath)
{
    detail::AssetManager::instance()->saveAssetModelToLocalFile(asset, filePath);
}

Ref<AssetModel> Assets::loadAssetFromLocalFile(const String& filePath)
{
    return detail::AssetManager::instance()->loadAssetModelFromLocalFile(filePath);
}

Ref<Object> Assets::loadAsset(const StringView& filePath)
{
    auto assetModel = detail::AssetManager::instance()->loadAssetModelFromLocalFile(filePath);
    if (assetModel)
        return assetModel->target();
    else
        return nullptr;
}

void Assets::reloadAsset(const StringView& filePath, Object* obj)
{
    auto assetPath = detail::AssetManager::instance()->findAssetPath(filePath);
    if (assetPath) {
        detail::AssetManager::instance()->loadAssetModelFromAssetPathToInstance(obj, *assetPath);
    }
    else {
        LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
    }
}

void Assets::saveAsset(Object* obj, const StringView& filePath)
{
    auto model = makeObject<AssetModel>(obj);
    detail::AssetManager::instance()->saveAssetModelToLocalFile(model, filePath);
}

void Assets::deserializeInstance(Object* obj, const StringView& filePath)
{
    auto assetPath = detail::AssetManager::instance()->findAssetPath(filePath);
    if (assetPath) {
        detail::AssetManager::instance()->loadAssetModelFromAssetPathToInstance(obj, *assetPath);
    }
    else {
        LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
    }
}

bool Assets::existsFile(const StringView& filePath)
{
    return detail::AssetManager::instance()->existsFile(filePath);
}

Ref<ByteBuffer> Assets::readAllBytes(const StringView& filePath)
{
	return detail::AssetManager::instance()->readAllBytes(filePath);
}

String Assets::readAllText(const StringView& filePath, EncodingType encoding)
{
    auto stream = detail::AssetManager::instance()->openFileStream(filePath);
    if (!stream) {
        LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
        return String::Empty;
    }

    TextEncoding* e = (encoding == EncodingType::Unknown) ? nullptr : TextEncoding::getEncoding(encoding);
    return FileSystem::readAllText(stream, e).unwrap();
}

Ref<Stream> Assets::openFileStream(const StringView& filePath)
{
    return detail::AssetManager::instance()->openFileStream(filePath);
}
//
//void Assets::serializeAssetObjectInternal(Archive& ar, const StringView& name, Ref<Object>& value)
//{
//    String localPath;
//    if (ar.isSaving()) {
//        // TODO: 毎回 parseAssetPath するのはアレなので、ar.basePath() の型を AssetPath にしたいところ。
//        localPath = detail::AssetPath::makeRelativePath(detail::AssetPath::parseAssetPath(ar.basePath()), value->assetPath());
//    }
//
//    ar & makeNVP(name, localPath);
//
//    if (ar.isLoading()) {
//        auto assetPath = detail::AssetPath::combineAssetPath(detail::AssetPath::parseAssetPath(ar.basePath()), localPath);
//        //value = dynamic_pointer_cast<Tileset>(Assets::loadAsset(assetPath));
//        auto assetModel = detail::AssetManager::instance()->loadAssetModelFromAssetPath(assetPath);
//        value = assetModel->target();
//        if (value) {
//            value->setAssetPath(assetPath);
//        }
//    }
//}


const String& Assets::engineAssetsDirectory()
{
    return AssetModel::EngineAssetsDirectory;
}

//==============================================================================
// AssetImporter

AssetImporter::AssetImporter()
{
}

AssetImporter::~AssetImporter()
{
}

bool AssetImporter::testSupportedExtensions(const ln::Path& sourceFilePath)
{
    List<String> exts;
    onGetSupportedExtensions(&exts);
    return exts.containsIf([&](auto& x) { return sourceFilePath.hasExtension(x); });
}

Ref<AssetModel> AssetImporter::import(const ln::Path& sourceFilePath)
{
    return onImport(sourceFilePath);
}

//bool AssetImporter::onImport(const ln::Path& sourceFilePath)
//{
//
//}

////==============================================================================
//// TextureImporter
//
//TextureImporter::TextureImporter()
//{
//}
//
//void TextureImporter::onGetSupportedExtensions(List<String>* outExtensions)
//{
//    *outExtensions = { _TT("png") };
//}
//
//Ref<AssetModel> TextureImporter::onImport(const ln::Path& sourceFilePath)
//{
//    auto texture = makeObject<Texture2D>(sourceFilePath);
//    auto asset = makeObject<AssetModel>(texture);
//    asset->saveInternal(sourceFilePath.str() + AssetModel::AssetFileExtension);
//    return asset;
//}

namespace detail {

const String AssetPath::FileSchemeName = _TT("file");
const String AssetPath::AssetSchemeName = _TT("asset");
const AssetPath AssetPath::Null;

AssetPath AssetPath::makeFromLocalFilePath(const Path& filePath)
{
    AssetPath assetPath = makeEmpty();
    assetPath.m_components->scheme = _TT("file");
    assetPath.m_components->host = _TT("");
    assetPath.m_components->path = filePath.canonicalize().unify();
    return assetPath;
}

bool AssetPath::tryParseAssetPath(const String& str, AssetPath* outPath)
{
    int separate0 = str.indexOf(_TT("://"));
    if (separate0 < 0) return false;
    //int schemeEnd = separate0 + 3;

    int separate1 = str.indexOf('/', separate0 + 3);
    if (separate1 < 0) return false;

    *outPath = makeEmpty();
    outPath->m_components->scheme = str.substr(0, separate0);
    outPath->m_components->host = str.substr(separate0 + 3, separate1 - (separate0 + 3));
    if (String::compare(outPath->m_components->scheme, AssetSchemeName, CaseSensitivity::CaseInsensitive) == 0) {
        outPath->m_components->path = Path(str.substr(separate1 + 1));
    }
    else {
        outPath->m_components->path = Path(str.substr(separate1 + 1)).canonicalize().unify();
    }
    return true;
}

AssetPath AssetPath::parseAssetPath(const String& str)
{
    AssetPath assetPath;
    if (tryParseAssetPath(str, &assetPath)) {
        return assetPath;
    }
    else {
        LN_ERROR("Invalid asset path.");
        return AssetPath();
    }
}

AssetPath AssetPath::combineAssetPath(const AssetPath& basePath, const String& localAssetPath)
{
    AssetPath result;
    if (tryParseAssetPath(localAssetPath, &result)) {
        return result;
    }
    else {
        if (LN_REQUIRE(!basePath.isNull())) return AssetPath();
        result = makeEmpty();
        result.m_components->scheme = basePath.scheme();
        result.m_components->host = basePath.host();
        if (String::compare(result.m_components->scheme, AssetSchemeName, CaseSensitivity::CaseInsensitive) == 0) {
            Path path = Path(basePath.path(), localAssetPath);
            std::vector<Char> tmpPath(path.length() + 1);
            int len = detail::PathTraits::canonicalizePath(path.c_str(), path.length(), tmpPath.data(), tmpPath.size());
            result.m_components->path = Path(StringView(tmpPath.data(), len)).unify();
        }
        else {
            result.m_components->path = Path(basePath.path(), localAssetPath).canonicalize().unify();
        }
        return result;
    }
}

Path AssetPath::makeRelativePath(const AssetPath& basePath, const AssetPath& assetPath)
{
    if (LN_REQUIRE(!basePath.isNull())) return String::Empty;
    if (LN_REQUIRE(String::compare(basePath.scheme(), assetPath.scheme(), CaseSensitivity::CaseInsensitive) == 0)) return String::Empty;
    if (LN_REQUIRE(String::compare(basePath.host(), assetPath.host(), CaseSensitivity::CaseInsensitive) == 0)) return String::Empty;
    return basePath.path().makeRelative(assetPath.path()).unify();
}

AssetPath AssetPath::resolveAssetPath(const Path& filePath, const Char** exts, size_t extsCount)
{
    auto path = detail::AssetManager::instance()->findAssetPath(filePath, exts, extsCount);
    if (path) {
        return *path;
    }
    else {
        LN_WARNING(_TT("Asset not found: ") + String(filePath));    // TODO: operator
        return AssetPath::makeEmpty();
    }
}

AssetPath AssetPath::resolveAssetPath(const AssetPath& assetPath, const Char** exts, size_t extsCount)
{
    return detail::AssetManager::instance()->resolveAssetPath(assetPath, exts, extsCount);
}

bool AssetPath::isAssetFilePath() const
{
    if (!m_components) return false;
    return m_components->path.hasExtension(AssetModel::AssetFileExtension);
}

bool AssetPath::isAssetFilePath(const Path& path)
{
    return path.hasExtension(AssetModel::AssetFileExtension);
}

AssetPath AssetPath::getParentAssetPath() const
{
    AssetPath result = makeEmpty();
    result.m_components->scheme = scheme();
    result.m_components->host = host();
    if (path().isRoot())
        result.m_components->path = path();
    else
        result.m_components->path = path().parent();
    return result;
}

String AssetPath::toString() const
{
    if (LN_REQUIRE(!isNull())) return String::Empty;

    const Char* pathPrefix = _TT("");
    if (path().length() > 0 && path().str()[0] != u'/')
        pathPrefix = _TT("/");

    auto result = String::concat(scheme(), _TT("://"), host());
    return  String::concat(result, pathPrefix, path().str());
}

uint64_t AssetPath::calculateHash() const
{
    uint64_t hash = std::hash<String>()(m_components->scheme);
    hash = detail::hash_combine(hash, std::hash<String>()(m_components->host));
    return detail::hash_combine(hash, std::hash<String>()(m_components->path.str()));
}


AssetPath::AssetPath(const String& scheme, const String& host, const Path& path)
    : m_components(std::make_shared<Components>())
{
    m_components->scheme = scheme;
    m_components->host = host;
    m_components->path = path;

    //if (m_components->path.str()[0] != u'/') m_components->path = Path(_TT("/" + path.str());
}

void AssetPath::clear()
{
    if (m_components) {
        m_components->scheme.clear();
        m_components->host.clear();
        m_components->path.clear();
    }
}

AssetPath AssetPath::makeEmpty()
{
    AssetPath assetPath;
    assetPath.m_components = std::make_shared<Components>();
    return assetPath;
}

} // namespace detail
} // namespace ln

