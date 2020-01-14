#pragma once
#include <unordered_map>

namespace ln {
class DiagnosticsManager;
class VertexLayout;
class Texture;
class Texture2D;
class StaticMeshModel;
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

    Ref<StaticMeshModel> createStaticMeshModel(const Path& filePath, float scale);
    Ref<SkinnedMeshModel> createSkinnedMeshModel(const Path& filePath, float scale);
	Ref<Texture> createTexture(const Path& parentDir, const StringRef& filePath, DiagnosticsManager* diag);

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }
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
};

} // namespace detail
} // namespace ln

