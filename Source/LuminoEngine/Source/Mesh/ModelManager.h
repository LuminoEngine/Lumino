
#pragma once
#include <array>
#include "Common.h"

LN_NAMESPACE_BEGIN
class FileManager;
class MeshResource;
class StaticMeshModel;
class PmxSkinnedMeshResource;
class SkinnedMeshModel;
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
	static ModelManager* GetInstance(ModelManager* priority = nullptr);

	ModelManager();
	virtual ~ModelManager();

	void Initialize(const ConfigData& configData);
	void Finalize();
	detail::PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	GraphicsManager* GetGraphicsManager() { return m_graphicsManager; }
	Texture2D* GetMMDDefaultToonTexture(int index);

	RefPtr<Texture> CreateTexture(const PathName& parentDir, const StringRef& filePath, ModelCreationFlag flags);
	

	RefPtr<StaticMeshModel> CreateStaticMeshModel(const PathName& filePath);
	RefPtr<SkinnedMeshModel> CreateSkinnedMeshModel(const PathName& filePath);

	Material* GetDefaultMaterial() const;
	MeshResource* GetUnitBoxMeshResource(bool reverseFaces) const;
	MeshResource* GetUnitSphereMeshResource(bool reverseFaces) const;

private:
	RefPtr<PmxSkinnedMeshResource> CreateSkinnedMeshResource(const PathName& filePath);
	//Animation::AnimationClip* CreateMotion(const PathName& filePath);

	FileManager*			m_fileManager;
	PhysicsManager*		m_physicsManager;
	GraphicsManager*	m_graphicsManager;
	//RefPtr<CacheManager>				m_cacheManager;
	std::array<Texture2D*, 10>	m_mmdDefaultToonTexture;

	RefPtr<Material>		m_defaultMaterial;
	RefPtr<MeshResource>	m_unitBoxMeshResource;
	RefPtr<MeshResource>	m_unitBoxMeshResourceReverseFaces;
	RefPtr<MeshResource>	m_unitSphereMeshResource;
	RefPtr<MeshResource>	m_unitSphereMeshResourceReverseFaces;
};

} // namespace detail
LN_NAMESPACE_END

