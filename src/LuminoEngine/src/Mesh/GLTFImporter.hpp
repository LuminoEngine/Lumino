
#pragma once
#include "MeshImporter.hpp"
namespace tinygltf { 
class Model;
class Node;
class Mesh;
}

namespace ln {
namespace detail {

class GLTFImporter
    : public MeshImporter
{
public:
    Ref<StaticMeshModel> import(AssetManager* assetManager, const String& assetPath, DiagnosticsManager* diag);

private:
	bool readNode(const tinygltf::Node& node, const Matrix& parentTransform);
	bool readMesh(const tinygltf::Mesh& mesh, const Matrix& transform);

	static bool FileExists(const std::string &abs_filename, void *user_data);
	static std::string ExpandFilePath(const std::string &filepath, void *user_data);
	static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data);
	static bool WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data);

	AssetManager* m_assetManager;
	String m_basedir;
	tinygltf::Model* m_model;
};

} // namespace detail
} // namespace ln
