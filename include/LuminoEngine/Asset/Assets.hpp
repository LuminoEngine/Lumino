
#pragma once
#include "Common.hpp"

namespace ln {
class Shader;
class Texture2D;
class AssetModel;

class Assets
{
public:

    /** Internal */
    static void saveAssetToLocalFile(AssetModel* asset, const String& filePath);











    static bool existsFile(const StringRef& filePath);

	/**
	 * 指定したアセットを読み込み、テクスチャを作成します。
	 *
	 * @param[in]   filePath   : アセットのパス
	 * @return		作成された Texture2D
	 * 
	 * 拡張子は不要です。
	 * EngineSettings::addAsset~ で指定されたパスを検索します。
	 */
    //static Ref<Texture2D> loadTexture(const StringRef& filePath);

    static Ref<Shader> loadShader(const StringRef& filePath);
	static Ref<ByteBuffer> readAllBytes(const StringRef& filePath);

    static Ref<Object> loadAsset(const StringRef& filePath);

    static Ref<Stream> openFileStream(const StringRef& filePath);



    //static String combineAssetPath(const String& assetFullBasePath, const String& localAssetPath);  // localAssetPath が asset:// から始まる場合はそれを採用。相対パスの場合は結合する。
    //static String makeRelativeAssetPath(const String& assetFullBasePath, const String& assetFullPath);

    static void serializeAsAssetPathInternal(Archive& ar, const StringRef& name, Ref<Object>& value);

    // TODO: internal
    template<typename TValue>
    static void serializeAsAssetPath(Archive& ar, const StringRef& name, Ref<TValue>& value)
    {
        if (ar.isSaving()) {
            Ref<Object> local = value;
            serializeAsAssetPathInternal(ar, name, local);
        }
        else {
            Ref<Object> local;
            serializeAsAssetPathInternal(ar, name, local);
            value = dynamic_pointer_cast<TValue>(local);
        }
    }

};

class AssetImporter
    : public Object
{
public:
    bool testSupportedExtensions(const ln::Path& sourceFilePath);
    Ref<AssetModel> import(const ln::Path& sourceFilePath);

protected:
    AssetImporter();
    virtual ~AssetImporter();
    virtual void onGetSupportedExtensions(List<String>* outExtensions) = 0;
    virtual Ref<AssetModel> onImport(const ln::Path& sourceFilePath) = 0;
};
//
//class TextureImporter
//    : public AssetImporter
//{
//public:
//    TextureImporter();
//
//    // TODO: 必要に応じてプロパティ持たせる
//
//protected:
//    virtual void onGetSupportedExtensions(List<String>* outExtensions) override;
//    virtual Ref<AssetModel> onImport(const ln::Path& sourceFilePath) override;
//};


} // namespace ln
