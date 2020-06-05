
#pragma once
#include "MeshImporter.hpp"
namespace tinygltf {
class Model;
class Node;
class Mesh;
class Material;
class Texture;
class Skin;
}

namespace ln {
class SkinnedMeshModel;

namespace detail {

class GLTFImporter
    : public MeshImporter
{
public:
    Ref<StaticMeshModel> import(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);
	Ref<SkinnedMeshModel> importSkinnedMesh(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);

private:
	// ファイルからのデータ読み込み用。以下、ほとんど glTF 用なので importer 側にもっていってもいいかも。必要なデータを前もって集めておいて、バッファをまとめて確保するのに使う。
	struct VertexBufferView
	{
		VertexElementType type;
		VertexElementUsage usage;
		int usageIndex;
		const void* data;
		//size_t byteOffset;
		size_t count;	// vertex count. not byte size. (byte size = count * size(type))
		size_t byteStride;
	};

	// ファイルからのデータ読み込み用
	struct SectionView
	{
		std::vector<VertexBufferView> vertexBufferViews;
		//int indexOffset;    // (unit: index number. not byte size)
		//int indexCount;     // (unit: index number. not byte size)

		const void* indexData;  // このセクション内で 0 から始まるインデックス
		int indexElementSize;	// byte size. (1, 2, 4)
		size_t indexCount;

		int materialIndex;

		PrimitiveTopology topology;
	};

	struct MeshView
	{
		std::vector<SectionView> sectionViews;
	};

	bool openGLTFModel(const AssetPath& assetPath);
	bool readCommon(StaticMeshModel* meshModel);
    Ref<Material> readMaterial(const tinygltf::Material& material);
	Ref<MeshNode> readNode(const tinygltf::Node& node);
	Ref<MeshContainer> readMesh(const tinygltf::Mesh& mesh);
	Ref<Mesh> generateMesh(const MeshView& meshView) const;
	Ref<MeshArmature> readSkin(const tinygltf::Skin& skin);
	Ref<Texture> loadTexture(const tinygltf::Texture& texture);

	static bool FileExists(const std::string &abs_filename, void *user_data);
	static std::string ExpandFilePath(const std::string &filepath, void *user_data);
	static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data);
	static bool WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data);

	AssetManager* m_assetManager;
    AssetPath m_basedir;
	DiagnosticsManager* m_diag;
	std::shared_ptr<tinygltf::Model> m_model;
	StaticMeshModel* m_meshModel;
};

} // namespace detail
} // namespace ln
