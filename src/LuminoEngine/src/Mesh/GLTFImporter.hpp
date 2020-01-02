
#pragma once
#include "MeshImporter.hpp"
namespace tinygltf { 
class Model;
class Node;
class Mesh;
class Material;
}

namespace ln {
namespace detail {

class GLTFImporter
    : public MeshImporter
{
public:
    Ref<StaticMeshModel> import(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);

private:
    Ref<Material> readMaterial(const tinygltf::Material& material);
	Ref<MeshNode> readNode(const tinygltf::Node& node);
	Ref<MeshContainer> readMesh(const tinygltf::Mesh& mesh);

	static bool FileExists(const std::string &abs_filename, void *user_data);
	static std::string ExpandFilePath(const std::string &filepath, void *user_data);
	static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data);
	static bool WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data);

	AssetManager* m_assetManager;
    AssetPath m_basedir;
	tinygltf::Model* m_model;
    Ref<StaticMeshModel> m_meshModel;
};

} // namespace detail
} // namespace ln
