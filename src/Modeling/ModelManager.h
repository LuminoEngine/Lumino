
#pragma once
#include <array>
//#include <Lumino/Base/Cache.h>
//#include <Lumino/IO/FileManager.h>
//#include <Lumino/Graphics/GraphicsManager.h>
//#include <Lumino/Graphics/Texture.h>
//#include "../Animation/AnimationClip.h"
//#include <Lumino/Physics/PhysicsManager.h>
#include "Common.h"

LN_NAMESPACE_BEGIN
class FileManager;
namespace Physics { class PhysicsManager; }
namespace detail
{

///
class ModelManager
	: public RefObject
{
public:

	/// 初期化データ
	struct Settings
	{
		FileManager*		FileManager;
		Physics::PhysicsManager*	PhysicsManager;
		GraphicsManager*			GraphicsManager;
		int							ModelCoreCacheSize;
		int							ModelCoreCacheMemorySize;

		Settings()
			: FileManager(NULL)
			, PhysicsManager(NULL)
			, GraphicsManager(NULL)
			, ModelCoreCacheSize(32)
			, ModelCoreCacheMemorySize(0)
		{}
	};

public:
	ModelManager();
	virtual ~ModelManager();

public:
	void Initialize(const Settings& configData);
	void Finalize();
	Physics::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Texture2D* GetMMDDefaultToonTexture(int index);

	Texture* CreateTexture(const PathName& parentDir, const StringRef& filePath, ModelCreationFlag flags);
	
	ModelCore* CreateModelCore(const PathName& filePath);
	//Animation::AnimationClip* CreateMotion(const PathName& filePath);

private:
	FileManager*			m_fileManager;
	Physics::PhysicsManager*		m_physicsManager;
	GraphicsManager*	m_graphicsManager;
	//RefPtr<CacheManager>				m_cacheManager;
	std::array<Texture2D*, 10>	m_mmdDefaultToonTexture;
};

} // namespace detail
LN_NAMESPACE_END

