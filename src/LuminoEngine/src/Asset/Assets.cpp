
#include "Internal.hpp"
#include "AssetManager.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoEngine/Asset/Assets.hpp>

// TODO: for importer
#include <LuminoEngine/Graphics/Texture.hpp>

namespace ln {

//=============================================================================
// Asset

bool Assets::existsFile(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->existsFile(filePath);
}

//Ref<Texture2D> Assets::loadTexture(const StringRef& filePath)
//{
//    return detail::EngineDomain::assetManager()->loadTexture(filePath);
//}

Ref<Shader> Assets::loadShader(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->loadShader(filePath);
}

Ref<ByteBuffer> Assets::readAllBytes(const StringRef& filePath)
{
	return detail::EngineDomain::assetManager()->readAllBytes(filePath);
}

Ref<Object> Assets::loadAsset(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->loadAsset(filePath);
}

Ref<Stream> Assets::openFileStream(const StringRef& filePath)
{
    return detail::EngineDomain::assetManager()->openFileStream(filePath);
}

void Assets::serializeAsAssetPathInternal(Archive& ar, const StringRef& name, Ref<Object>& value)
{
    String localPath;
    if (ar.isSaving()) {
        // TODO: 毎回 parseAssetPath するのはアレなので、ar.basePath() の型を AssetPath にしたいところ。
        localPath = detail::AssetPath::makeRelativePath(detail::AssetPath::parseAssetPath(ar.basePath()), value->assetPath());
    }

    ar & makeNVP(name, localPath);

    if (ar.isLoading()) {
        auto assetPath = detail::AssetPath::combineAssetPath(detail::AssetPath::parseAssetPath(ar.basePath()), localPath);
        //value = dynamic_pointer_cast<Tileset>(Assets::loadAsset(assetPath));
        auto assetModel = detail::EngineDomain::assetManager()->loadAssetModelFromAssetPath(assetPath);
        value = assetModel->target();
        if (value) {
            value->setAssetPath(assetPath);
        }
    }
}

//=============================================================================
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

////=============================================================================
//// TextureImporter
//
//TextureImporter::TextureImporter()
//{
//}
//
//void TextureImporter::onGetSupportedExtensions(List<String>* outExtensions)
//{
//    *outExtensions = { u"png" };
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

AssetPath AssetPath::makeFromLocalFilePath(const Path& filePath)
{
    AssetPath assetPath = makeEmpty();
    assetPath.m_components->scheme = u"file";
    assetPath.m_components->host = u"";
    assetPath.m_components->path = filePath.canonicalize().unify();
    return assetPath;
}

bool AssetPath::tryParseAssetPath(const String& str, AssetPath* outPath)
{
    int separate0 = str.indexOf(u"://");
    if (separate0 < 0) return false;
    //int schemeEnd = separate0 + 3;

    int separate1 = str.indexOf('/', separate0 + 3);
    if (separate1 < 0) return false;

    *outPath = makeEmpty();
    outPath->m_components->scheme = str.substr(0, separate0);
    outPath->m_components->host = str.substr(separate0 + 3, separate1 - (separate0 + 3));
    outPath->m_components->path = Path(str.substr(separate1 + 1)).canonicalize().unify();
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
        result.m_components->path = Path(basePath.path(), localAssetPath).canonicalize().unify();
        return result;
    }
}

String AssetPath::makeRelativePath(const AssetPath& basePath, const AssetPath& assetPath)
{
    if (LN_REQUIRE(!basePath.isNull())) return String::Empty;
    if (LN_REQUIRE(String::compare(basePath.scheme(), assetPath.scheme(), CaseSensitivity::CaseInsensitive) == 0)) return String::Empty;
    if (LN_REQUIRE(String::compare(basePath.host(), assetPath.host(), CaseSensitivity::CaseInsensitive) == 0)) return String::Empty;
    return basePath.path().makeRelative(assetPath.path());
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

    const Char* pathPrefix = u"";
    if (path().length() > 0 && path().str()[0] != u'/')
        pathPrefix = u"/";

    auto result = String::concat(scheme(), u"://", host());
    return  String::concat(result, pathPrefix, path().str());
}

AssetPath::AssetPath()
{
}

AssetPath AssetPath::makeEmpty()
{
    AssetPath assetPath;
    assetPath.m_components = std::make_shared<Components>();
    return assetPath;
}

} // namespace detail
} // namespace ln

