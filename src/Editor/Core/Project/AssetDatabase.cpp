
#include "../../../lumino/LuminoEngine/src/Engine/EngineDomain.hpp"
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include "Project.hpp"
#include "AssetDatabase.hpp"

namespace lna {


//==============================================================================
// AssetDatabase

AssetDatabase::AssetDatabase()
{
}

ln::Result AssetDatabase::init(Project* owner)
{
    ln::detail::AssetManager::instance()->buildAssetIndexFromLocalFiles(owner->assetsDir());
    ln::detail::AssetManager::instance()->addAssetDirectory(owner->assetsDir());
    return ln::ok();
}


void AssetDatabase::close()
{
    ln::detail::AssetManager::instance()->removeAllAssetDirectory();
}

Ref<ln::AssetModel> AssetDatabase::openAsset(const ln::Path& filePath)
{
    return ln::detail::AssetManager::instance()->loadAssetModelFromLocalFile(filePath);

    //auto assetFile = filePath.str();
    //if (!filePath.hasExtension(ln::AssetModel::AssetFileExtension)) {
    //    assetFile += ln::AssetModel::AssetFileExtension;
    //}
    //
    //if (LN_REQUIRE(ln::FileSystem::existsFile(assetFile))) return nullptr;

    //auto asset = ln::makeObject<ln::AssetModel>();
    //if (!asset->loadInternal(assetFile)) {
    //    LN_NOTIMPLEMENTED();    // TODO: error
    //    return nullptr;
    //}

    //return asset;
}

ln::Result AssetDatabase::importAsset(const ln::Path& sourceFilePath, const ln::Path& destinationFilePath)
{
    LN_NOTIMPLEMENTED();
    //if (sourceFilePath.hasExtension("png")) {
    //    auto texture = ln::makeObject<ln::Texture2D>(sourceFilePath);
    //    //texture->setAssetId(ln::Uuid::generate());
    //    texture->setAssetSource(destinationFilePath);

    //    auto asset = ln::makeObject<ln::AssetModel>(texture);
    //    asset->saveInternal(destinationFilePath.str() + ln::AssetModel::AssetFileExtension);

    //    if (ln::Path::compare(sourceFilePath, destinationFilePath) != 0)
    //        ln::FileSystem::copyFile(sourceFilePath, destinationFilePath, ln::FileCopyOption::Overwrite);

    //    return true;
    //}
    return ln::err();
}

ln::Result AssetDatabase::createAsset(ln::Object* asset, const ln::Path& filePath)
{
    auto t = ln::makeObject<ln::AssetModel>(asset);//ln::AssetModel::create(asset);
    ln::detail::AssetManager::instance()->saveAssetModelToLocalFile(t, filePath);
    //asset->setAssetId(ln::Uuid::generate());
    //ln::String json = ln::JsonSerializer::serialize(*t, filePath.parent(), ln::JsonFormatting::Indented);
    //ln::FileSystem::writeAllText(filePath, json);
    return ln::ok();
}

ln::Result AssetDatabase::saveAsset(ln::AssetModel* asset)
{
    //if (LN_REQUIRE(asset)) return false;
    //if (LN_REQUIRE(!asset->assetFilePath().isEmpty())) return false;
    //return asset->saveInternal(asset->assetFilePath());
    ln::detail::AssetManager::instance()->saveAssetModelToLocalFile(asset);
    return ln::ok();
}

bool AssetDatabase::isAssetFile(const ln::Path& file)
{
    return file.hasExtension(ln::AssetModel::AssetFileExtension);
}

bool AssetDatabase::isImportedAssetFile(const ln::Path& file)
{
    return ln::FileSystem::existsFile(file.str() + ln::AssetModel::AssetFileExtension);
}

} // namespace lna
