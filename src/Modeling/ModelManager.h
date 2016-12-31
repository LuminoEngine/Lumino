
#pragma once
#include <array>
#include "Common.h"

LN_NAMESPACE_BEGIN
class FileManager;
class MeshResource;
class StaticMeshModel;
class PmxSkinnedMeshResource;
namespace detail { class PhysicsManager; }

namespace detail
{

class ModelManager
	: public RefObject
{
public:

	struct ConfigData
	{
		FileManager*				fileManager = nullptr;
		detail::PhysicsManager*		physicsManager = nullptr;
		GraphicsManager*			graphicsManager = nullptr;
		int							modelCoreCacheSize = 32;
		int							modelCoreCacheMemorySize = 0;
	};

public:
	ModelManager();
	virtual ~ModelManager();

	void Initialize(const ConfigData& configData);
	void Finalize();
	FileManager* GetFileManager() const { return m_fileManager; }
	detail::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Texture2D* GetMMDDefaultToonTexture(int index);

	RefPtr<Texture> CreateTexture(const PathName& parentDir, const StringRef& filePath, ModelCreationFlag flags);
	
	RefPtr<MeshResource> CreateStaticMeshResource(const PathName& filePath);
	RefPtr<PmxSkinnedMeshResource> CreateSkinnedMeshModel(const PathName& filePath);
	//Animation::AnimationClip* CreateMotion(const PathName& filePath);

private:
	FileManager*			m_fileManager;
	PhysicsManager*		m_physicsManager;
	GraphicsManager*	m_graphicsManager;
	//RefPtr<CacheManager>				m_cacheManager;
	std::array<Texture2D*, 10>	m_mmdDefaultToonTexture;
};

} // namespace detail
LN_NAMESPACE_END

