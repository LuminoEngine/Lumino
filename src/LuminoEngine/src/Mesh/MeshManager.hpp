#pragma once
#include <unordered_map>

namespace ln {
class DiagnosticsManager;
class VertexDeclaration;
class Texture;
class StaticMeshModel;
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
	void initialize(const Settings& settings);
	void dispose();
	VertexDeclaration* getPredefinedVertexLayout(PredefinedVertexLayoutFlags flags);

    Ref<StaticMeshModel> createStaticMeshModel(const Path& filePath);
	Ref<Texture> createTexture(const Path& parentDir, const StringRef& filePath, DiagnosticsManager* diag);

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

	//const Ref<VertexDeclaration>& predefinedVertexLayout(PredefinedVertexLayout kind) const { return m_predefinedVertexLayouts[kind]; }

private:
	GraphicsManager* m_graphicsManager;
	AssetManager* m_assetManager;
	std::unordered_map<PredefinedVertexLayoutFlags, Ref<VertexDeclaration>> m_predefinedVertexLayouts;
	//std::array<Ref<VertexDeclaration>, PredefinedVertexLayout_Count> m_predefinedVertexLayouts;
	//Ref<VertexDeclaration> m_predefinedVertexLayout_StandardGeometry;
	//Ref<VertexDeclaration> m_predefinedVertexLayout_StandardBlendWeights;
	//Ref<VertexDeclaration> m_predefinedVertexLayout_AdditionalUV;
	//Ref<VertexDeclaration> m_predefinedVertexLayout_SdefInfo;
	//Ref<VertexDeclaration> m_predefinedVertexLayout_SdefInfo;
};

} // namespace detail
} // namespace ln

