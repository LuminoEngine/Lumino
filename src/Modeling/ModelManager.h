
#pragma once
#include <array>
#include "Common.h"

LN_NAMESPACE_BEGIN
class FileManager;
namespace Physics { class PhysicsManager; }
class MeshResource;
class StaticMeshModel;

namespace detail
{

class ModelManager
	: public RefObject
{
public:

	struct ConfigData
	{
		FileManager*				fileManager = nullptr;
		Physics::PhysicsManager*	physicsManager = nullptr;
		GraphicsManager*			graphicsManager = nullptr;
		int							modelCoreCacheSize = 32;
		int							modelCoreCacheMemorySize = 0;
	};

public:
	ModelManager();
	virtual ~ModelManager();

	void Initialize(const ConfigData& configData);
	void Finalize();
	Physics::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Texture2D* GetMMDDefaultToonTexture(int index);

	RefPtr<Texture> CreateTexture(const PathName& parentDir, const StringRef& filePath, ModelCreationFlag flags);
	
	RefPtr<MeshResource> CreateModelCore(const PathName& filePath);
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

