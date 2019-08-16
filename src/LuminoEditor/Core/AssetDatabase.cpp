
#include "Project.hpp"
#include "AssetDatabase.hpp"

namespace lna {


//==============================================================================
// AssetDatabase

AssetDatabase::AssetDatabase()
{
}

Ref<ln::AssetModel> AssetDatabase::openAsset(const ln::Path& filePath)
{
    auto assetFile = filePath.str();
    if (!filePath.hasExtension(ln::AssetModel::AssetFileExtension)) {
        assetFile += ln::AssetModel::AssetFileExtension;
    }
    
    if (LN_REQUIRE(ln::FileSystem::existsFile(assetFile))) return nullptr;

    auto asset = ln::makeObject<ln::AssetModel>();
    if (!asset->loadInternal(assetFile)) {
        LN_NOTIMPLEMENTED();    // TODO: error
        return nullptr;
    }

    return asset;
}

bool AssetDatabase::isAssetFile(const ln::Path& file)
{
    return file.hasExtension(ln::AssetModel::AssetFileExtension);
}

bool AssetDatabase::isImportedAssetFile(const ln::Path& file)
{
    return ln::FileSystem::existsFile(file.str() + ln::AssetModel::AssetFileExtension);
}

ln::Result AssetDatabase::init(Project* owner)
{
    return true;
}

} // namespace lna
