
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
	static ModelManager* getInstance(ModelManager* priority = nullptr);

	ModelManager();
	virtual ~ModelManager();

	void initialize(const ConfigData& configData);
	void dispose();
	FileManager* getFileManager() const { return m_fileManager; }
	detail::PhysicsManager* getPhysicsManager() { return m_physicsManager; }
	GraphicsManager* getGraphicsManager() { return m_graphicsManager; }
	Texture2D* getMMDDefaultToonTexture(int index);

	Ref<Texture> createTexture(const PathName& parentDir, const StringRef& filePath, ModelCreationFlag flags);
	

	Ref<StaticMeshModel> createStaticMeshModel(const PathName& filePath);
	Ref<SkinnedMeshModel> createSkinnedMeshModel(const PathName& filePath);

	CommonMaterial* getDefaultMaterial() const;
	MeshResource* getUnitBoxMeshResource(bool reverseFaces) const;
	MeshResource* getUnitSphereMeshResource(bool reverseFaces) const;
	MeshResource* getUnitTeapotMeshResource() const;

private:
	Ref<PmxSkinnedMeshResource> createSkinnedMeshResource(const PathName& filePath);
	//Animation::AnimationClip* CreateMotion(const PathName& filePath);

	FileManager*			m_fileManager;
	PhysicsManager*		m_physicsManager;
	GraphicsManager*	m_graphicsManager;
	//Ref<CacheManager>				m_cacheManager;
	std::array<Ref<Texture2D>, 10>	m_mmdDefaultToonTexture;

	Ref<CommonMaterial>		m_defaultMaterial;
	Ref<MeshResource>	m_unitBoxMeshResource;
	Ref<MeshResource>	m_unitBoxMeshResourceReverseFaces;
	Ref<MeshResource>	m_unitSphereMeshResource;
	Ref<MeshResource>	m_unitSphereMeshResourceReverseFaces;
	Ref<MeshResource>	m_unitTeapotMeshResource;
};

} // namespace detail
LN_NAMESPACE_END

