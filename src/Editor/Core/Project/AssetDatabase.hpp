#pragma once

namespace lna {
class Project;


class AssetDatabase
	: public ln::Object
{
public:

    // 拡張子不要
    Ref<ln::AssetModel> openAsset(const ln::Path& filePath);

    // 拡張子から種類を自動判別してインポート。.lnasset を作る。
    // パスが同じ場合はコピーしない。
    ln::Result<> importAsset(const ln::Path& sourceFilePath, const ln::Path& destinationFilePath);

    // 拡張子不要
    ln::Result<> createAsset(ln::Object* asset, const ln::Path& filePath);

    // 上書き保存
    ln::Result<> saveAsset(ln::AssetModel* asset);

    static bool isAssetFile(const ln::Path& file);
    static bool isImportedAssetFile(const ln::Path& file);  // TODO: obsolete
    static bool isExternalFile(const ln::Path& file) { return !isImportedAssetFile(file); }

public:// TODO:
   // LN_INTERNAL_NEW_OBJECT;
    AssetDatabase();
    ln::Result<> init(Project* owner);
    void close();

    Project* m_ownerProject;
};

} // namespace lna
