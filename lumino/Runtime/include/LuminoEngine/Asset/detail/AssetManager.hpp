
#pragma once
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Asset/Common.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoEngine/Base/detail/RefObjectCache.hpp>

namespace ln {
// class Texture2D;
class Shader;
class AssetModel;
namespace detail {
class AssetArchive;
class FileSystemReader;
struct AssetRequiredPathSet;

class AssetManager : public URefObject {
public:
    static const String AssetPathPrefix;
    static const String LocalhostPrefix;

    struct Settings {
        AssetStorageAccessPriority assetStorageAccessPriority = AssetStorageAccessPriority::DirectoryFirst;
    };
    // static AssetManager* initialize(const Settings& settings);
    // static void terminate();
     static inline AssetManager* instance() { return EngineContext2::instance()->assetManager(); }

    AssetManager();
    virtual ~AssetManager();
    bool init(const Settings& settings);
    void dispose();

    void addAssetDirectory(const StringView& path);
    void mountAssetArchive(const StringView& filePath, const StringView& password);
    void removeAllAssetDirectory();
    // void setAssetStorageAccessPriority(AssetStorageAccessPriority value);

    // ファイルパスを入力として、そのファイルが存在する場合は内部表現のパス（セパレータ /）を返す。
    // この値は各種キャッシュのキーとして使用できる。また、findAssetPath() で使用する。
    // "asset://local/C:/dir/file.txt" => Windows 形式の絶対パス。 ファイルシステム上のファイルを指す。
    // "asset://local/dir/file.txt"    => Unix 形式の絶対パス。 ファイルシステム上のファイルを指す。
    // "asset:///dir/file.txt"         => ローカルファイルパス。登録されているいずれかの AssetArchive 内のファイルを指す。
    // TODO: "asset://ArchiveName/" とかで AssetArchive を明示できるようにしてもいい気がする
    Optional_deprecated<AssetPath> findAssetPath(const StringView& filePath, const Char* const* exts, int extsCount) const;
    Optional_deprecated<AssetPath> findAssetPath(const StringView& filePath) const;
    bool existsAsset(const AssetPath& assetPath) const;
    Ref<Stream> openStreamFromAssetPath(const AssetPath& assetPath) const;
    AssetPath resolveAssetPath(const AssetPath& assetPath, const Char** exts, int extsCount) const;

    Ref<AssetModel> loadAssetModelFromLocalFile(const String& filePath) const;
    Ref<AssetModel> loadAssetModelFromAssetPath(const AssetPath& assetPath) const;
    bool loadAssetModelFromAssetPathToInstance(Object* obj, const AssetPath& assetPath) const;
    void saveAssetModelToLocalFile(AssetModel* asset, const String& filePath = String::Empty) const; // 別名で保存するときは filePath を指定する

    String assetPathToLocalFullPath(const AssetPath& assetPath) const;
    // String localFullPathToAssetPath(const String& localFullPath) const;
    // static String getParentAssetPath(const String& assetPath);  // 親フォルダ
    // static String combineAssetPath(const String& assetFullBasePath, const String& localAssetPath);  // localAssetPath が asset:// から始まる場合はそれを採用。相対パスの場合は結合する。
    // static String makeRelativeAssetPath(const String& assetFullBasePath, const String& assetFullPath);
    static String canonicalizeAssetPath(const String& assetPath);

    [[deprecated]] bool existsFile(const StringView& filePath) const;
    [[deprecated]] Ref<Stream> openFileStream(const StringView& filePath);
    [[deprecated]] Ref<ByteBuffer> readAllBytes(const StringView& filePath);

    // TODO: for develop & debug
    void buildAssetIndexFromLocalFiles(const ln::Path& assetDir);

    // TODO: deprecated. use loadObjectWithCacheHelperAsync
    template<class TObject, class TCache>
    static Ref<TObject> loadObjectWithCacheHelper(
        TCache* cache,
        const detail::AssetPath* baseDir, // モデルファイルからのテクスチャロード等で使用する。不要なら nullptr
        const std::vector<const Char*>& exts,
        const StringView& filePath,
        std::function<Ref<TObject>(const AssetRequiredPathSet*)> factory) {
        auto pathSet = std::make_unique<AssetRequiredPathSet>();
        if (!AssetObject::_resolveAssetRequiredPathSet(baseDir, filePath, exts, pathSet.get())) {
            return nullptr;
        }

        // finalResourceAssetFilePath から拡張子を除いたものを CacheKey とする
        // > CacheKey はどの Archive に入っているファイルであるかまで区別できるものでなければダメ。
        // > Archive 名と、それを基準とした相対パス(または絶対パス) で表す必要がある。
        // > 拡張子は無くてもOK。.yml でも .png でも、出来上がる Texture2D は同じもの。
        const auto cacheKey = Path(pathSet->finalResourceAssetFilePath.toString()).replaceExtension(_TT(""));

        // キャッシュに見つかったらそれを返す
        if (auto obj = cache->findObject(cacheKey)) {
            return obj;
        }

        Ref<TObject> obj;
        if (factory) {
            obj = factory(pathSet.get());
            obj->m_data = std::move(pathSet);
        }
        else {
            obj = makeObject_deprecated<TObject>();
            obj->m_data = std::move(pathSet);
            obj->reload();
        };

        cache->registerObject(cacheKey, obj);

        return obj;
    }

    template<class TObject, class TCache>
    static Ref<GenericTask<Ref<TObject>>> loadObjectWithCacheHelperAsync(
        TCache* cache,
        const detail::AssetPath* baseDir, // モデルファイルからのテクスチャロード等で使用する。不要なら nullptr
        const std::vector<const Char*>& exts,
        const StringView& filePath) {
        auto pathSet = std::make_unique<AssetRequiredPathSet>();
        if (!AssetObject::_resolveAssetRequiredPathSet(baseDir, filePath, exts, pathSet.get())) {
            return nullptr;
        }

        // finalResourceAssetFilePath から拡張子を除いたものを CacheKey とする
        // > CacheKey はどの Archive に入っているファイルであるかまで区別できるものでなければダメ。
        // > Archive 名と、それを基準とした相対パス(または絶対パス) で表す必要がある。
        // > 拡張子は無くてもOK。.yml でも .png でも、出来上がる Texture2D は同じもの。
        const auto cacheKey = Path(pathSet->finalResourceAssetFilePath.toString()).replaceExtension(_TT(""));

        if (auto obj = cache->findObject(cacheKey)) {
            auto task = GenericTask<Ref<TObject>>::create([obj]() {
                return obj;
            });
            task->start();
            return task;
        }

        auto obj = makeObject_deprecated<TObject>();

        // reload 前に先にインスタンスを登録して、
        // 同時に同じアセットがロードされようとしたときに別のインスタンスを作ってしまうケースに対応する。
        cache->registerObject(cacheKey, obj);
        obj->m_data = std::move(pathSet);

        auto task = GenericTask<Ref<TObject>>::create([obj]() {
            obj->reload();
            return obj;
        });
        task->start();
        return task;
    }

private:
    void refreshActualArchives();
    bool existsFileInternal(const StringView& filePath, const Char** exts, int extsCount) const;
    Ref<Stream> openFileStreamInternal(const StringView& filePath, const Char** exts, int extsCount, Path* outPath);
    void makeFindPaths(const StringView& filePath, const Char* const* exts, int extsCount, List<Path>* paths) const;
    static bool tryParseAssetPath(const String& assetPath, String* outArchiveName, Path* outLocalPath);
    FileSystemReader* primaryAssetDirectoryArchive() const { return m_fileSystemArchives[0]; }

    List<Ref<AssetArchive>> m_requestedArchives;
    List<AssetArchive*> m_actualArchives;
    AssetStorageAccessPriority m_storageAccessPriority;
    std::unordered_map<ln::Uuid, ln::Path> m_assetIndex;
    // Path m_primaryLocalAssetDirectory;

    // file スキーマでアクセスできる Archive
    Ref<FileSystemReader> m_localFileSystemArchive;

    // asset スキーマでアクセスできる Archive。ファイルを保存できる。
    List<FileSystemReader*> m_fileSystemArchives;
};

} // namespace detail
} // namespace ln
