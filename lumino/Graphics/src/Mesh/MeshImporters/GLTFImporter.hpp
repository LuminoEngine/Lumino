
#pragma once
#include <LuminoGraphics/Animation/AnimationCurve.hpp>
#include <LuminoGraphics/Animation/AnimationTrack.hpp>
#include <LuminoGraphics/Animation/AnimationClip.hpp>
#include "MeshImporter.hpp"

namespace tinygltf {
class Model;
class Node;
class Mesh;
class Material;
class Texture;
class Skin;
class Animation;
class Accessor;
class Value;
}

namespace ln {
class AnimationClip;
class SkinnedMeshModel;

namespace detail {

class GLTFImporter
    : public MeshImporter
{
public:
	bool m_flipZ;
	bool m_flipX;
	bool m_faceFlip = false;
	bool m_clearBoneRotation;

	GLTFImporter();
    bool onImportAsStaticMesh(MeshModel* model, const AssetPath& assetPath) override;
	bool onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath) override;

	const List<Ref<AnimationClip>> animationClips() const { return m_animationClips; }

protected:
	tinygltf::Model* gltfModel() const { return m_model.get(); }
	const Ref<Texture2D>& loadedTextures(int imageIndex) const { return m_loadedTextures[imageIndex]; }
	virtual bool onReadMetadata();

	Ref<Texture> loadTexture(const tinygltf::Texture& texture);	// TODO: tmp
private:
	// ファイルからのデータ読み込み用。以下、ほとんど glTF 用なので importer 側にもっていってもいいかも。必要なデータを前もって集めておいて、バッファをまとめて確保するのに使う。
	struct VertexBufferView
	{
		VertexElementType type;
		VertexElementUsage usage;
		int usageIndex;
		const void* data;
		size_t count;	// vertex count. not byte size. (byte size = count * size(type))
		size_t byteStride;
	};

	struct VertexMorphTargetView
	{
		const Vector3* positions;
		const Vector3* normals;
		const Vector3* tangents;
		//size_t vertexCount;
	};

	// ファイルからのデータ読み込み用
	struct MeshPrimitiveView
	{
		std::vector<VertexBufferView> vertexBufferViews;

		const void* indexData;  // このセクション内で 0 から始まるインデックス
		int indexElementSize;	// byte size. (1, 2, 4)
		size_t indexCount;

		int materialIndex;

		PrimitiveTopology topology;

		std::vector<VertexMorphTargetView> morphTargetViews;
	};

	struct MeshView
	{
		String name;
		std::vector<MeshPrimitiveView> sectionViews;
	};

	bool openGLTFModel(const AssetPath& assetPath);
	bool readCommon(MeshModel* meshModel);
    Ref<Material> readMaterial(const tinygltf::Material& material);
	bool readNode(MeshNode* coreNode, const tinygltf::Node& node);
	Ref<MeshContainer> readMesh(const tinygltf::Mesh& mesh);
	Ref<MeshContainer> generateMesh(const MeshView& meshView) const;
	Ref<MeshSkeleton> readSkin(const tinygltf::Skin& skin);
	Ref<AnimationClip> readAnimation(const tinygltf::Animation& animation) const;
	const void* getRawData(const tinygltf::Accessor& accessor) const;

	static bool FileExists(const std::string &abs_filename, void *user_data);
	static std::string ExpandFilePath(const std::string &filepath, void *user_data);
	static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data);
	static bool WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data);

    AssetPath m_basedir;
	std::shared_ptr<tinygltf::Model> m_model;
	MeshModel* m_meshModel;
	std::vector<Ref<Texture2D>> m_loadedTextures;	// gltfModel の image の index に対応する。

	List<Ref<AnimationClip>> m_animationClips;
};

} // namespace detail
} // namespace ln
