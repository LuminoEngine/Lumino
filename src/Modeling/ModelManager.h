
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/IO/FileManager.h>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Texture.h>
#include "../Animation/AnimationClip.h"
#include <Lumino/Physics/PhysicsManager.h>
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace Modeling
{

///
class ModelManager
	: public RefObject
{
public:

	/// 初期化データ
	struct ConfigData
	{
		FileManager*		FileManager;
		Physics::PhysicsManager*	PhysicsManager;
		GraphicsManager*			GraphicsManager;
		int							ModelCoreCacheSize;
		int							ModelCoreCacheMemorySize;

		ConfigData()
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
	void Initialize(const ConfigData& configData);
	void Finalize();
	Physics::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Texture* GetMMDDefaultToonTexture(int index);
	
	ModelCore* CreateModelCore(const PathName& filePath);
	Animation::AnimationClip* CreateMotion(const PathName& filePath);

private:
	RefPtr<FileManager>			m_fileManager;
	RefPtr<Physics::PhysicsManager>		m_physicsManager;
	RefPtr<GraphicsManager>	m_graphicsManager;
	RefPtr<CacheManager>				m_cacheManager;
	RefPtr<Texture>			m_mmdDefaultToonTexture[10];
};

} // namespace Modeling
LN_NAMESPACE_END

