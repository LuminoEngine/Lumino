
#pragma once
#include "MeshImporter.hpp"
namespace tinygltf {
class Model;
class Node;
class Mesh;
class Material;
class Texture;
class Skin;
class Animation;
}

namespace ln {
class AnimationClip;
class SkinnedMeshModel;

namespace detail {

/*

[2020/8/4]
----------
Blender メモ: bone を出力するには glTF エクスポータで "Skinning" を ON にする必要がある。

[2020/7/21-3]
----------
Bone は、Left が X-, Right が X+ だった。ここからもう一回考察しなおしたほうがいいかも。

よく見てみると、モデルが左右反転していた。
AliciaSolid.vrm を見ると、アクセサリの位置が左右逆。
Lumino と、それ以外のツール (Blender, 3Dビューア―, Unity) で。

そうすると、GLTFImporter の中で左手座標系用の反転処理を入れる必要がある。

そうすると、VRM モデルは次のフォーマットとなる。
- 右手座標系
- Z- を正面とする

Lumino へ座標系変換だけしてインポートしたときは、デフォルトでは Z+ 方向を見るのが正しい。
これは MMD モデルとは逆向きとなる。（このため、何も考えずに VMD アニメをアタッチすると、左右が逆になったりした）




[2020/7/21-2]
----------
src/LuminoEngine/sandbox/Assets/Models/Axis.glb でテストした結果、
- Blender は手前を Y- とする右手座標系
- 3D ビューアー は手前を Z+ とする右手座標系 (OpenGL 系) ([グリッド&ビュー] では Z+ を正面と表示する)
- Lumino は手前 Z- をとする左手座標系 (DirectX 系)

また、
- Blender はエクスポートするとき、glTF モデルの座標系を 「手前を Z+ とする右手座標系 (OpenGL 系)」に変換する。
そのため
- Blender と 3D ビューアー で見比べた結果は一致する。
- Lumino と 3D ビューアー で見比べた結果は Z が反転する。

AliciaSolid.vrm は、
- Lumino で見た時は Z- 正面
- 3D ビューアー の [グリッド&ビュー] で見ると、前後反転している。
ということで、
「VRM ファイルは Z- を基本の正面とするファイルフォーマット」と考えてよさそう。
ちなみにこれは MMD モデルと一致する。

[2020/7/21-1]
----------
 * VMR
 * Z+ 方向が基本の正面？AliciaSolid.vrm を Windows の 3D ビューアーの [グリッド&ビュー] で見ると、前後反転している。
 * Blender にインポートすると、モデルは Z- (下) を向いている。（Transform は全部 identity でインポートされた）

   Blender でエクスポートしたものを 3Dビューアー で見ると、
   "Blender の Y- ＝ 3Dビューアーの正面"
   これを Lumino で見ると、
   "Blender の Y- ＝ Lumino の Z+"
   ※Blender から "+Y Up" でエクスポートすると、Z が反転する。→ src/LuminoEngine/sandbox/Assets/Models/Z-Box.glb
   そうすると、
   "3Dビューアーの正面 ＝ Lumino の Z+"


 *
 * ちなみに Unity ちゃんモデルは Z- が正面だった。Prefab 上で Z+ を向くように回転しているみたい。
 *
 * 
 */
class GLTFImporter
    : public MeshImporter
{
public:
	GLTFImporter();
    bool importAsStaticMesh(StaticMeshModel* model, AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);
	bool importAsSkinnedMesh(SkinnedMeshModel* model, AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag);

	const List<Ref<AnimationClip>> animationClips() const { return m_animationClips; }

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
	bool readNode(MeshNode* coreNode, const tinygltf::Node& node);
	Ref<MeshContainer> readMesh(const tinygltf::Mesh& mesh);
	Ref<Mesh> generateMesh(const MeshView& meshView) const;
	Ref<MeshArmature> readSkin(const tinygltf::Skin& skin);
	Ref<Texture> loadTexture(const tinygltf::Texture& texture);
	Ref<AnimationClip> readAnimation(const tinygltf::Animation& animation) const;

	static bool FileExists(const std::string &abs_filename, void *user_data);
	static std::string ExpandFilePath(const std::string &filepath, void *user_data);
	static bool ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data);
	static bool WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data);

	AssetManager* m_assetManager;
    AssetPath m_basedir;
	DiagnosticsManager* m_diag;
	std::shared_ptr<tinygltf::Model> m_model;
	StaticMeshModel* m_meshModel;
	bool m_flipZ;
	bool m_flipX;
	bool m_disableBoneRotation;

	List<Ref<AnimationClip>> m_animationClips;
};

} // namespace detail
} // namespace ln
