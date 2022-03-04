
#pragma once
#include "Common.hpp"

namespace ln {
class Shader;
class Texture2D;
class AssetModel;
class AssetObject;

LN_CLASS(Static)
class Assets
{
public:

    //static void setAssetPath(AssetObject* obj, const String& filePath);
    //static const Path& getAssetPath(AssetObject* obj);
    static void save(AssetObject* obj);
    static void reload(AssetObject* obj);


    /** Internal */
    LN_METHOD()
    static void saveAssetToLocalFile(AssetModel* asset, const String& filePath);

    /** Internal */
    LN_METHOD()
    static Ref<AssetModel> loadAssetFromLocalFile(const String& filePath);



    /**
     * 指定したアセットファイルを読み込み、オブジェクト生成します。
     *
     * ファイルの拡張子は .lnasset です。ただし、filePath に指定する値は拡張子を省略可能です。
     */
    LN_METHOD()
    static Ref<Object> loadAsset(const StringView& filePath);
    
    /**
     * 指定したアセットファイルを読み込み、作成済みのオブジェクトへ適用します。
     *
     * このメソッドは Lumino の型システムを使用しないオブジェクトの読み込みに使用します。
     */
    LN_METHOD()
    static void reloadAsset(const StringView& filePath, Object* obj);

    /**
     * 指定したアセットファイルへオブジェクトを保存します。
     *
     * 拡張子が無い場合は追加されます。
     * Lumino 標準の拡張子以外を明示的に指定した場合、アセットを読み込むときも同じように明示する必要があります。
     */
    static void saveAsset(Object* obj, const StringView& filePath);

    /**
     * 既存のオブジェクトに対して、指定したアセットファイルでデシリアライズを行います。
     *
     * このメソッドは Lumino の型システムを使用しないオブジェクトの読み込みに使用します。
     */
    // obsolete: use reloadAsset
    static void deserializeInstance(Object* obj, const StringView& filePath);




    static bool existsFile(const StringView& filePath);

	static Ref<ByteBuffer> readAllBytes(const StringView& filePath);

    /** readAllText */
    LN_METHOD()
    static String readAllText(const StringView& filePath, EncodingType encoding = EncodingType::Unknown);

    static Ref<Stream> openFileStream(const StringView& filePath);



    //static String combineAssetPath(const String& assetFullBasePath, const String& localAssetPath);  // localAssetPath が asset:// から始まる場合はそれを採用。相対パスの場合は結合する。
    //static String makeRelativeAssetPath(const String& assetFullBasePath, const String& assetFullPath);

    //static void serializeAssetObjectInternal(Archive& ar, const StringView& name, Ref<Object>& value);

    // TODO: internal
    // assetPath() をキャッシュキーのソースとして、扱うような AssetObject を serialize する。
    // 
    //template<typename TValue>
    //static void serializeAssetObject(Serializer2& ar, const StringView& name, Ref<TValue>& value)
    //{
    //    if (ar.isSaving()) {
    //        Ref<Object> local = value;
    //        serializeAsAssetPathInternal(ar, name, local);
    //    }
    //    else {
    //        Ref<Object> local;
    //        serializeAsAssetPathInternal(ar, name, local);
    //        value = dynamic_pointer_cast<TValue>(local);
    //    }
    //}

    static const String& engineAssetsDirectory();
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
