
#pragma once
#include <LuminoEngine/Asset/Common.hpp>

namespace ln {
class Texture2D;
class Shader;
namespace detail {
class AssetArchive;

class AssetManager
	: public RefObject
{
public:
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

	bool existsFile(const StringRef& filePath) const;
    Ref<Stream> openFileStream(const StringRef& filePath);
	Ref<ByteBuffer> readAllBytes(const StringRef& filePath);
	Ref<Texture2D> loadTexture(const StringRef& filePath);
    Ref<Shader> loadShader(const StringRef& filePath);

    // TODO: for develop & debug
    void buildAssetIndexFromLocalFiles(const ln::Path& assetDir);

private:
	void refreshActualArchives();
	bool existsFileInternal(const StringRef& filePath, const Char** exts, int extsCount) const;
	const Path& findFilePathFromIndex(const StringRef& id) const;
    Ref<Stream> openFileStreamInternal(const StringRef& filePath, const Char** exts, int extsCount);
	void makeFindPaths(const StringRef& filePath, const Char** exts, int extsCount, List<Path>* paths) const;

    List<Ref<AssetArchive>> m_requestedArchives;
	List<AssetArchive*> m_actualArchives;
	AssetStorageAccessPriority m_storageAccessPriority;
    std::unordered_map<ln::Uuid, ln::Path> m_assetIndex;
};

} // namespace detail
} // namespace ln

