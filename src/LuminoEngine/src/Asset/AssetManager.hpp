#pragma once

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
	void initialize(const Settings& settings);
	void dispose();

    void addAssetArchive(const StringRef& filePath, const StringRef& password);
	void addAssetDirectory(const StringRef& path);

	bool existsFile(const StringRef& filePath) const;
    Ref<Stream> openFileStream(const StringRef& filePath);
	Ref<ByteBuffer> readAllBytes(const StringRef& filePath);
	Ref<Texture2D> loadTexture(const StringRef& filePath);
    Ref<Shader> loadShader(const StringRef& filePath);


private:
	bool existsFileInternal(const StringRef& filePath, const Char** exts, int extsCount) const;
	Ref<Stream> openFileStreamInternal(const StringRef& filePath, const Char** exts, int extsCount);
	void makeFindPaths(const StringRef& filePath, const Char** exts, int extsCount, List<Path>* paths) const;

    List<Ref<AssetArchive>> m_archives;
};

} // namespace detail
} // namespace ln

