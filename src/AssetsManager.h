
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Graphics/Texture.h>

LN_NAMESPACE_BEGIN
class EngineManager;

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

	Texture2DPtr LoadTexture(const StringRef& filePath);

private:
	EngineManager*			m_engineManager;
	RefPtr<CacheManager>	m_textureCache;
};

LN_NAMESPACE_END
