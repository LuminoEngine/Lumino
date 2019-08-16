#pragma once

namespace lna {
class Project;


class AssetDatabase
	: public ln::Object
{
public:
    // 拡張子不要
    Ref<ln::AssetModel> openAsset(const ln::Path& filePath);

    static bool isAssetFile(const ln::Path& file);
    static bool isImportedAssetFile(const ln::Path& file);  // TODO: obsolete
    static bool isExternalFile(const ln::Path& file) { return !isImportedAssetFile(file); }

public:// TODO:
   // LN_INTERNAL_NEW_OBJECT;
    AssetDatabase();
    ln::Result init(Project* owner);

    Project* m_ownerProject;
};

} // namespace lna
