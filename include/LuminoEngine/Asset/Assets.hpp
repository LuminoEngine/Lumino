
#pragma once
#include "Common.hpp"

namespace ln {
class Shader;
class Texture2D;
class AssetModel;

class Assets
{
public:
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
    static Ref<Texture2D> loadTexture(const StringRef& filePath);

    static Ref<Shader> loadShader(const StringRef& filePath);
	static Ref<ByteBuffer> readAllBytes(const StringRef& filePath);

    static Ref<Object> loadAsset(const StringRef& filePath);

    static Ref<Stream> openFileStream(const StringRef& filePath);
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

class TextureImporter
    : public AssetImporter
{
public:
    TextureImporter();

    // TODO: 必要に応じてプロパティ持たせる

protected:
    virtual void onGetSupportedExtensions(List<String>* outExtensions) override;
    virtual Ref<AssetModel> onImport(const ln::Path& sourceFilePath) override;
};

} // namespace ln
