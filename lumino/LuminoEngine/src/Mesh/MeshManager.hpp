#pragma once
#include <unordered_map>
#include "../../../Engine/src/Base/RefObjectCache.hpp"
#include <LuminoEngine/Mesh/Common.hpp>

namespace ln {
class DiagnosticsManager;
class VertexLayout;
class Texture;
class Texture2D;
class MeshModel;
class SkinnedMeshModel;
namespace detail {
class AssetManager;

enum PredefinedVertexLayoutFlags	// TODO: name StandardVertexLayoutFlags
{
	PredefinedVertexLayoutFlags_None = 0x0000,
	PredefinedVertexLayoutFlags_Geometry = 0x0001,
	PredefinedVertexLayoutFlags_BlendWeights = 0x0002,
	PredefinedVertexLayoutFlags_AdditionalUV = 0x0004,
	PredefinedVertexLayoutFlags_SdefInfo = 0x0008,
	PredefinedVertexLayoutFlags_MmdExtra = 0x0010,
};

class MeshManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
		AssetManager* assetManager;
	};

	MeshManager();
	void init(const Settings& settings);
	void dispose();
	VertexLayout* getPredefinedVertexLayout(PredefinedVertexLayoutFlags flags);

	Ref<MeshModel> acquireStaticMeshModel(const Path& filePath, float scale);
	Ref<MeshModel> acquireStaticMeshModel(const AssetPath& assetPath, float scale);
    //void loadStaticMeshModel(MeshModel* model, const Path& filePath, float scale);
	void loadStaticMeshModel(MeshModel* model, const AssetPath& assetPath, float scale);
    Ref<SkinnedMeshModel> createSkinnedMeshModel(const Path& filePath, MeshImportSettings* settings);
	Ref<Texture> createTexture(const Path& parentDir, const StringRef& filePath, DiagnosticsManager* diag);

	void collectUnreferenceObjects();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }
	AssetManager* assetManager() const { return m_assetManager; }
    const Ref<Texture2D>& getMMDDefaultToonTexture(int index) const { return  m_mmdDefaultToonTexture[index]; }
	const Ref<LinearAllocatorPageManager>& linearAllocatorPageManager() const { return m_linearAllocatorPageManager; }

	//const Ref<VertexLayout>& predefinedVertexLayout(PredefinedVertexLayout kind) const { return m_predefinedVertexLayouts[kind]; }

private:
	GraphicsManager* m_graphicsManager;
	AssetManager* m_assetManager;
	std::unordered_map<PredefinedVertexLayoutFlags, Ref<VertexLayout>> m_predefinedVertexLayouts;
	//std::array<Ref<VertexLayout>, PredefinedVertexLayout_Count> m_predefinedVertexLayouts;
	//Ref<VertexLayout> m_predefinedVertexLayout_StandardGeometry;
	//Ref<VertexLayout> m_predefinedVertexLayout_StandardBlendWeights;
	//Ref<VertexLayout> m_predefinedVertexLayout_AdditionalUV;
	//Ref<VertexLayout> m_predefinedVertexLayout_SdefInfo;
	//Ref<VertexLayout> m_predefinedVertexLayout_SdefInfo;

    std::array<Ref<Texture2D>, 10> m_mmdDefaultToonTexture;

	Ref<LinearAllocatorPageManager> m_linearAllocatorPageManager;
	ObjectCache<uint64_t, MeshModel> m_meshModelCache;
};

} // namespace detail
} // namespace ln

