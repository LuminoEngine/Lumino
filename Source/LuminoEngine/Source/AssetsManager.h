
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Graphics/Texture.h>

LN_NAMESPACE_BEGIN
class EngineManager;
class StaticMeshModel;

/**
	@brief
*/
class AssetsManager
	: public RefObject
{
public:
	static AssetsManager* GetInstance(AssetsManager* priority = nullptr);

public:
	AssetsManager();
	virtual ~AssetsManager();
	void Initialize(EngineManager* manager);
	void Finalize();

	void AddAssetsDirectory(const StringRef& directoryPath);

	Texture2DPtr LoadTexture(const StringRef& filePath);

	RefPtr<StaticMeshModel> LoadMeshModel(const StringRef& filePath);

	String LoadText(const StringRef& filePath);

	RefPtr<Stream> OpenFile(const StringRef& filePath);

	//RawFontPtr LoadFont(const StringRef& name, int size, bool isBold, bool isItalic, bool isAntiAlias);

private:
	struct AssetsDirectory
	{
		PathName	path;
		PathName	searchPath;
	};

	void MakeSearchPath(const StringRef& path);
	const PathName* FindLocalFilePath();
	//Stream* CreateFileStream(const StringRef& filePath, bool isDeferring = false);

	EngineManager*			m_engineManager;
	List<AssetsDirectory>	m_assetsDirectories;
	RefPtr<CacheManager>	m_textureCache;
	RefPtr<CacheManager>	m_fontCache;
};

LN_NAMESPACE_END
