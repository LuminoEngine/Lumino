
#pragma once
#include <LuminoEngine/Asset/Common.hpp>

namespace ln {
//class Texture2D;
class Shader;
namespace detail {
class AssetArchive;

class AssetManager
	: public RefObject
{
public:
    static const String AssetPathPrefix;
    static const String LocalhostPrefix;

	struct Settings
	{
	};

	AssetManager();
	virtual ~AssetManager();
	void init(const Settings& settings);
	void dispose();

	void addAssetDirectory(const StringRef& path);
    void addAssetArchive(const StringRef& filePath, const StringRef& password);
	void setAssetStorageAccessPriority(AssetStorageAccessPriority value);

    // ファイルパスを入力として、そのファイルが存在する場合は内部表現のパス（セパレータ /）を返す。
    // この値は各種キャッシュのキーとして使用できる。また、findAssetPath() で使用する。
    // "asset://local/C:/dir/file.txt" => Windows 形式の絶対パス。 ファイルシステム上のファイルを指す。
    // "asset://local/dir/file.txt"    => Unix 形式の絶対パス。 ファイルシステム上のファイルを指す。
    // "asset:///dir/file.txt"         => ローカルファイルパス。登録されているいずれかの AssetArchive 内のファイルを指す。
    // TODO: "asset://ArchiveName/" とかで AssetArchive を明示できるようにしてもいい気がする
    Optional<String> findAssetPath(const StringRef& filePath, const Char** exts, int extsCount) const;
    Ref<Stream> openStreamFromAssetPath(const String& assetPath) const;


	bool existsFile(const StringRef& filePath) const;
    Ref<Stream> openFileStream(const StringRef& filePath);
	Ref<ByteBuffer> readAllBytes(const StringRef& filePath);
	//Ref<Texture2D> loadTexture(const StringRef& filePath);
    Ref<Shader> loadShader(const StringRef& filePath);
    Ref<Object> loadAsset(const StringRef& filePath);

    // TODO: for develop & debug
    void buildAssetIndexFromLocalFiles(const ln::Path& assetDir);

private:
	void refreshActualArchives();
	bool existsFileInternal(const StringRef& filePath, const Char** exts, int extsCount) const;
	const Path& findFilePathFromIndex(const StringRef& id) const;
    const Path& findFilePathInternal(const StringRef& filePath, const Char** exts, int extsCount);
    Ref<Stream> openFileStreamInternal(const StringRef& filePath, const Char** exts, int extsCount, Path* outPath);
	void makeFindPaths(const StringRef& filePath, const Char** exts, int extsCount, List<Path>* paths) const;
    bool tryParseAssetPath(const String& assetPath, String* outArchiveName, Path* outPath) const;

    List<Ref<AssetArchive>> m_requestedArchives;
	List<AssetArchive*> m_actualArchives;
	AssetStorageAccessPriority m_storageAccessPriority;
    std::unordered_map<ln::Uuid, ln::Path> m_assetIndex;
};

} // namespace detail
} // namespace ln

