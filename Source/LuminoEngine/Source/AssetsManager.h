
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Graphics/Texture.h>

LN_NAMESPACE_BEGIN
namespace detail { class ArchiveManager; }
class EngineManager;
class StaticMeshModel;

/**
	@brief
*/
class AssetsManager
	: public RefObject
{
public:
	static AssetsManager* getInstance(AssetsManager* priority = nullptr);

public:
	AssetsManager();
	virtual ~AssetsManager();
	void initialize(EngineManager* manager);
	void dispose();

	void addAssetsDirectory(const StringRef& directoryPath);

	Texture2DPtr loadTexture(const StringRef& filePath);
	Ref<Shader> loadShader(const StringRef& filePath, ShaderCodeType codeType = ShaderCodeType::Normal);

	Ref<StaticMeshModel> loadMeshModel(const StringRef& filePath);

	String loadText(const StringRef& filePath);

	Ref<Stream> openFile(const StringRef& filePath);

	//RawFontPtr LoadFont(const StringRef& name, int size, bool isBold, bool isItalic, bool isAntiAlias);

private:
	//struct AssetsDirectory
	//{
	//	PathName	path;
	//	PathName	searchPath;
	//};

	//void makeSearchPath(const StringRef& path);
	//const PathName* findLocalFilePath();
	//Stream* createFileStream(const StringRef& filePath, bool isDeferring = false);

	EngineManager*			m_engineManager;
	Ref<detail::ArchiveManager>	m_archiveManager;
	//List<AssetsDirectory>	m_assetsDirectories;
	Ref<CacheManager>	m_textureCache;
	//Ref<CacheManager>	m_shaderCache;
	Ref<CacheManager>	m_fontCache;
};

LN_NAMESPACE_END
