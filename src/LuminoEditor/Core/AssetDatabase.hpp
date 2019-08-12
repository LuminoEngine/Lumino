#pragma once

namespace lna {
class Project;


class AssetDatabase
	: public ln::Object
{
public:
    // 拡張子不要
    Ref<ln::AssetModel> openAsset(const ln::Path& filePath);

    bool isImportedAssetFile(const ln::Path& file) const;
    bool isExternalFile(const ln::Path& file) const { return !isImportedAssetFile(file); }

public:// TODO:
   // LN_INTERNAL_NEW_OBJECT;
    AssetDatabase();
    ln::Result init(Project* owner);

    Project* m_ownerProject;
};

} // namespace lna
