
#include "Internal.hpp"
#define TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_NO_FS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
//#include "D:\Tech\Graphics\tinygltf\tiny_gltf.h"
#include <LuminoBitmap/Bitmap.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/IndexBuffer.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../../../../RuntimeCore/src/Asset/AssetManager.hpp"
#include "../../../../Graphics/src/GraphicsManager.hpp"
#include "../MeshManager.hpp"
#include "GLTFImporter.hpp"

namespace ln {
namespace detail {
	
/*

[2020/9/9] 座標系の合わせ
----------
右手Z+正面のモデルを変換するには？(.vrmはZ-が正面なのでこれにはあてはまらない)

ボーンの位置を合わせるためには次のどちらかが必要
- Y軸180度回転 -> 元モデルは Z+ を向ているので、この後Z反転が必要
- X反転

メッシュの位置を合わせるには
- X反転
だけでよい。元モデルが Z+ を向ていることを前提としているため、Z反転は不要


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

GLTFImporter::GLTFImporter()
	: m_flipZ(false)
	, m_flipX(false)
	, m_clearBoneRotation(true)
{
}

bool GLTFImporter::openGLTFModel(const AssetPath& assetPath)
{
	m_basedir = assetPath.getParentAssetPath();
	auto stream = m_assetManager->openStreamFromAssetPath(assetPath);
	auto data = stream->readToEnd();

	m_model = std::make_shared<tinygltf::Model>();
	//tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	tinygltf::FsCallbacks fs = {
		&FileExists,
		&ExpandFilePath,
		&ReadWholeFile,
		&WriteWholeFile,
		this,
	};
	loader.SetFsCallbacks(fs);

	std::string err;
	std::string warn;
	bool result;
	if (data[0] == 'g' && data[1] == 'l' && data[2] == 'T' && data[3] == 'F') {
		result = loader.LoadBinaryFromMemory(m_model.get(), &err, &warn, data.data(), data.size(), "");
	}
	else {
		result = loader.LoadASCIIFromString(m_model.get(), &err, &warn, (const char*)data.data(), data.size(), "");
	}
	if (!err.empty()) m_diag->reportError(String::fromStdString(err));
	if (!warn.empty()) m_diag->reportError(String::fromStdString(warn));


	return result;
}

bool GLTFImporter::onImportAsStaticMesh(MeshModel* model, const AssetPath& assetPath)
{
	// TODO: ひとまず HC4 用設定。
	// ほんとはここから Y90 回転させるべきなのだが、今その処理を入れてる時間がない。
	//m_flipZ = false;
	m_flipZ = false;

	//m_flipX = true;
	//m_faceFlip = false;	// 何もせずインポートするときは R-Hand->L-Hand の変換なので面反転が必要だが、flipX １回やっているので不要。
	// TODO: そもそも何かNodeの回転修正もあやしいみたい。10/17提出用に向けてはいったん左右反転したままで行ってみる
	m_flipX = false;	
	// TODO: このあたり PostProcess として修正入れたいが、先に SkinnedMeshModel と MeshModel の統合をやらないと無駄作業が多くなるのでいったん保留

	if (!openGLTFModel(assetPath)) {
		return false;
	}

	m_meshModel = model;

	readCommon(m_meshModel);

	return true;
}

bool GLTFImporter::GLTFImporter::onImportAsSkinnedMesh(SkinnedMeshModel* model, const AssetPath& assetPath)
{
	if (isCharacterModelFormat()) {
		// TODO: ひとまず HC4 用設定。
		// インポート元モデルは [Y-Up,R-Hand] で、キャラクターは Z+ を正面としている。
		// そのため Lumino 標準の Z+ 正面に合わせるには、X を反転するだけでよい。
		m_flipZ = false;
		m_flipX = false;
		m_faceFlip = false;	// TODO: onImportAsStaticMesh の理論ならこれも不要なはずなのだが… 後でちゃんと調べる

	}
	else {
		m_flipZ = false;
		m_flipX = false;
		m_faceFlip = false;
	}





	if (!openGLTFModel(assetPath)) {
		return false;
	}

	m_meshModel = model;

	readCommon(m_meshModel);

	if (isSkeletonImport()) {
		for (const auto& skin : m_model->skins) {
			auto meshSkeleton = readSkin(skin);
			if (!meshSkeleton) {
				return false;
			}
			model->addSkeleton(meshSkeleton);

			// Note: skins->skeleton は将来的に使われなくなるみたいなプロパティ。
			// どうもルートボーンを示すための値らしいのだが、Three.js とかでは無視されているようだ。
			// https://tkaaad97.hatenablog.com/entry/2019/07/28/175737
		}

		for (const auto& animation : m_model->animations) {
			auto clip = readAnimation(animation);
			if (!clip) {
				return false;
			}
			if (clip->name().isEmpty()) {
				clip->setName(String::fromNumber(m_animationClips.size()));
			}
			m_animationClips.add(clip);
		}
	}

	return true;
}

bool GLTFImporter::onReadMetadata()
{
	return true;
}

bool GLTFImporter::readCommon(MeshModel* meshModel)
{
	if (!onReadMetadata()) return false;

	for (auto& material : m_model->materials) {
		meshModel->addMaterial(readMaterial(material));
	}

	for (auto& mesh : m_model->meshes) {
		auto meshContainer = readMesh(mesh);
		if (!meshContainer) {
			return false;
		}
		meshModel->addMeshContainer(meshContainer);
	}

	// 先に MeshNode のインスタンスを作っておく。親子関係を結ぶときに参照したい。
	for (auto& node : m_model->nodes) {
		auto meshNode = makeObject<MeshNode>();
		meshModel->addNode(meshNode);
	}

	for (int i = 0; i < m_model->nodes.size(); i++) {
		if (!readNode(meshModel->m_nodes[i], m_model->nodes[i])) {
			return false;
		}

		//auto coreNode = makeObject<MeshNode>();
		//auto meshNode = readNode(node);
		//if (!meshNode) {
		//	return false;
		//}
		//meshModel->addNode(meshNode);
	}

	// MeshNode のインスタンスを作った後、親子関係を結ぶ
	//for (auto& node : m_model->nodes) {
	//	for (size_t i = 0; i < node.children.size(); i++) {
	//		assert(node.children[i] < m_model->nodes.size());
	//		coreNode->addChildIndex(node.children[i]);
	//	}
	//}

	int scene_to_display = m_model->defaultScene > -1 ? m_model->defaultScene : 0;
	const tinygltf::Scene& scene = m_model->scenes[scene_to_display];
	for (size_t i = 0; i < scene.nodes.size(); i++) {
		meshModel->addRootNode(scene.nodes[i]);
	}

	meshModel->updateNodeTransforms();



	return true;
}

Ref<Material> GLTFImporter::readMaterial(const tinygltf::Material& material)
{
    auto coreMaterial = makeObject<Material>();
	coreMaterial->m_name = String::fromStdString(material.name);

	// glTF default
	// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughness
	coreMaterial->setMetallic(1.0f);    
	coreMaterial->setRoughness(1.0f);

	for (const auto& value : material.values) {
		if (value.first == "baseColorFactor") {
			auto& c = value.second.number_array;
			assert(c.size() == 4);
			coreMaterial->setColor(Color(c[0], c[1], c[2], c[3]));
		}
		else if (value.first == "metallicFactor") {
			assert(value.second.has_number_value);
			coreMaterial->setMetallic(value.second.number_value);
		}
		else if (value.first == "roughnessFactor") {
			assert(value.second.has_number_value);
			coreMaterial->setRoughness(value.second.number_value);
		}
		else if (value.first == "baseColorTexture") {
			auto itr2 = value.second.json_double_value.find("index");
			assert(itr2 != value.second.json_double_value.end());
			int index = static_cast<int>(itr2->second);
			const tinygltf::Texture& texture = m_model->textures[index];
			coreMaterial->setMainTexture(loadTexture(texture));
		}
		else if (value.first == "metallicRoughnessTexture") {
			auto itr2 = value.second.json_double_value.find("index");
			assert(itr2 != value.second.json_double_value.end());
			int index = static_cast<int>(itr2->second);
			const tinygltf::Texture& texture = m_model->textures[index];
			coreMaterial->setMetallicRoughnessTexture(loadTexture(texture));
		}
		else if (value.first == "normalTexture") {
			auto itr2 = value.second.json_double_value.find("index");
			assert(itr2 != value.second.json_double_value.end());
			int index = static_cast<int>(itr2->second);
			const tinygltf::Texture& texture = m_model->textures[index];
			coreMaterial->setNormalMap(loadTexture(texture));

			// Blender でマテリアルの Normal を HeightMap にすると、glTF ではここに流れてくる。
			// ただし、glTF としては HeightMap はサポートされていない。
			// https://github.com/KhronosGroup/glTF/issues/948
			// いまは手動で直してもらうしかない。
		}
		else if (value.first == "occlusionTexture") {
			auto itr2 = value.second.json_double_value.find("index");
			assert(itr2 != value.second.json_double_value.end());
			int index = static_cast<int>(itr2->second);
			const tinygltf::Texture& texture = m_model->textures[index];
			coreMaterial->setOcclusionTexture(loadTexture(texture));
		}
		else {
			m_diag->reportWarning(String::format(_TT("Material field '{}' is not supported."), String::fromStdString(value.first)));
		}
	}

	// MMD, VRM など、多くの人型モデルは両面表示を前提として作られていることの方が多いので、
	// デフォルトではそのようにしておく。
	//coreMaterial->setCullingMode(CullMode::None);


    return coreMaterial;
}

//Ref<MeshNode> GLTFImporter::readNode(const tinygltf::Node& node)
bool GLTFImporter::readNode(MeshNode* coreNode, const tinygltf::Node& node)
{
	Matrix nodeTransform;
	if (node.matrix.size() == 16) {
		const double* m = node.matrix.data();
        nodeTransform.set(
            m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15]);

		//nodeTransform *= Matrix::makeScaling(1, 1, -1);

		//nodeTransform.set(	// transpose
		//	m[0], m[4], m[8], m[12],
		//	m[1], m[5], m[9], m[13],
		//	m[2], m[6], m[10], m[14],
		//	m[3], m[7], m[11], m[15]);
	}
	else {
		if (node.scale.size() == 3) {
			nodeTransform.scale(node.scale[0], node.scale[1], node.scale[2]);
		}

		if (node.rotation.size() == 4) {
			//if (m_disableBoneRotation) {

			//}
			//else {
				nodeTransform.rotateQuaternion(
					Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]));
			//}
		}

		if (node.translation.size() == 3) {
			nodeTransform.translate(node.translation[0], node.translation[1], node.translation[2]);
		}
	}


	if (m_flipZ) {
		// Z軸反転の 右手⇔左手回転の変換
		// 回転の符号は (-x, -y, z) となる。
		// https://kamino.hatenablog.com/entry/rotation_expressions
		nodeTransform(0, 2) = -nodeTransform(0, 2);
		nodeTransform(1, 2) = -nodeTransform(1, 2);
		nodeTransform(2, 0) = -nodeTransform(2, 0);
		nodeTransform(2, 1) = -nodeTransform(2, 1);

		// Z座標反転
		nodeTransform(3, 2) = -nodeTransform(3, 2);
	}
	if (m_flipX) {
		// X軸反転の 右手⇔左手回転の変換
		// 回転の符号は (x, -y, -z) となる。
		nodeTransform(0, 1) = -nodeTransform(0, 1);
		nodeTransform(0, 2) = -nodeTransform(0, 2);
		nodeTransform(1, 0) = -nodeTransform(1, 0);
		nodeTransform(2, 0) = -nodeTransform(2, 0);

		// X座標反転
		nodeTransform(3, 0) = -nodeTransform(3, 0);

		/*
		↓実際にこれで比べてみると結果がわかりやすい
		Matrix m1;
		m1.rotateX(1);
		m1.rotateY(1);
		m1.rotateZ(1);
		Matrix m2;
		m2.rotateX(1);
		m2.rotateY(-1);
		m2.rotateZ(-1);
		*/
	}

	coreNode->setName(String::fromStdString(node.name));
    coreNode->setInitialLocalTransform(nodeTransform);
	coreNode->skeletonIndex = node.skin;

    if (node.mesh > -1) {
        assert(node.mesh < m_model->meshes.size());
        coreNode->setMeshContainerIndex(node.mesh);
	}

	for (size_t i = 0; i < node.children.size(); i++) {
		assert(node.children[i] < m_model->nodes.size());
		coreNode->addChildIndex(node.children[i]);
	}

	return true;
}

// glTF の Mesh は小単位として Primitive というデータを持っている。
// Primitive は 頂点バッファ、インデックスバッファ、1つのマテリアル から成る。
// 1つのマテリアル を持つので、Lumino の Mesh だと 1つの Section に対応しそうに見えるが、
// 頂点バッファ、インデックスバッファも持っている点に注意。
//
// Lumino の Mesh への変換としては、この Primitive が持っている個々の頂点バッファとインデックスバッファを
// それぞれひとつのバッファに統合する方針で実装している。
Ref<MeshContainer> GLTFImporter::readMesh(const tinygltf::Mesh& mesh)
{
    MeshView meshView;
	meshView.name = String::fromStdString(mesh.name);

    int indexBufferViewIndex = -1;
    int indexComponentType = -1;

	for (size_t i = 0; i < mesh.primitives.size(); i++) {
		const tinygltf::Primitive& primitive = mesh.primitives[i];
		if (primitive.indices < 0) return nullptr;

		MeshPrimitiveView sectionView;
        sectionView.materialIndex = primitive.material;

		// Vertex buffers
		for (auto itr = primitive.attributes.begin(); itr != primitive.attributes.end(); ++itr) {
			assert(itr->second >= 0);
			const tinygltf::Accessor& accessor = m_model->accessors[itr->second];
			
			if (accessor.sparse.isSparse) {
				LN_NOTIMPLEMENTED();
				return nullptr;
			}

			VertexBufferView vbView;
			//vbView.byteOffset = accessor.byteOffset;
			vbView.count = accessor.count;

			// Type
			// Usage
			// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#meshes
			vbView.usage = VertexElementUsage::Unknown;
			vbView.usageIndex = 0;
			if (itr->first.compare("POSITION") == 0) {
				vbView.usage = VertexElementUsage::Position;
				vbView.usageIndex = 0;
				if (accessor.type == TINYGLTF_TYPE_VEC3 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float3;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else if (itr->first.compare("NORMAL") == 0) {
				vbView.usage = VertexElementUsage::Normal;
				vbView.usageIndex = 0;
				if (accessor.type == TINYGLTF_TYPE_VEC3 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float3;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else if (itr->first.compare("TANGENT") == 0) {
				vbView.usage = VertexElementUsage::Tangent;
				vbView.usageIndex = 0;
				if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float4;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else if (itr->first.compare(0, 8, "TEXCOORD") == 0) {
				if (itr->first.compare("TEXCOORD_0") == 0) {
					vbView.usageIndex = 0;
				}
				else if (itr->first.compare("TEXCOORD_1") == 0) {
					vbView.usageIndex = 1;
				}
				else if (itr->first.compare("TEXCOORD_2") == 0) {
					vbView.usageIndex = 2;
				}
				else {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				vbView.usage = VertexElementUsage::TexCoord;
				if (accessor.type == TINYGLTF_TYPE_VEC2 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float2;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC2 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC2 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else if (itr->first.compare(0, 6, "COLOR_") == 0) {
				vbView.usage = VertexElementUsage::Color;
				
				if (itr->first.compare("COLOR_0") == 0) {
					vbView.usageIndex = 0;
				}
				else if (itr->first.compare("COLOR_1") == 0) {
					vbView.usageIndex = 1;
				}
				else {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}

				if (accessor.type == TINYGLTF_TYPE_VEC3 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float3;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC3 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC3 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float4;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else if (itr->first.compare("JOINTS_0") == 0) {
				vbView.usage = VertexElementUsage::BlendIndices;
				vbView.usageIndex = 0;
				if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					vbView.type = VertexElementType::Short4;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else if (itr->first.compare("WEIGHTS_0") == 0) {
				vbView.usage = VertexElementUsage::BlendWeight;
				vbView.usageIndex = 0;
				if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
					vbView.type = VertexElementType::Float4;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else if (accessor.type == TINYGLTF_TYPE_VEC4 && accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
					LN_NOTIMPLEMENTED();
					return nullptr;
				}
				else {
					m_diag->reportError(_TT("Invalid vertex data type."));
					return nullptr;
				}
			}
			else {
				LN_NOTIMPLEMENTED();
				return nullptr;
			}

			// Stride
			const tinygltf::BufferView& vertexBufferView = m_model->bufferViews[accessor.bufferView];
			vbView.byteStride = accessor.ByteStride(vertexBufferView);
			assert(vbView.byteStride != -1);

			// Buffer
			const tinygltf::Buffer& buffer = m_model->buffers[vertexBufferView.buffer];
			vbView.data = buffer.data.data() + accessor.byteOffset + vertexBufferView.byteOffset;

			sectionView.vertexBufferViews.push_back(std::move(vbView));
		}

		// Index buffer
		{
			const tinygltf::Accessor& indexAccessor = m_model->accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = m_model->bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& buffer = m_model->buffers[indexBufferView.buffer];


			if (indexAccessor.sparse.isSparse) {
				LN_NOTIMPLEMENTED();
				return nullptr;
			}

            if (indexBufferView.byteStride) {
                // 未対応。というか、エラー？普通インデックスバッファを飛び飛びで使わないと思うが…
                LN_NOTIMPLEMENTED();
                return nullptr;
            }

            //// 最初に見つかったものをインデックスバッファとして採用
            //indexBufferViewIndex = indexAccessor.bufferView;
            //indexComponentType = indexAccessor.componentType;

            // 1byte
            if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE || indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
                sectionView.indexElementSize = 1;
            }
            // 2byte
            else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT || indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
                sectionView.indexElementSize = 2;
            }
            // 4byte
            else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_INT || indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
                sectionView.indexElementSize = 4;
            }
            else {
                LN_NOTIMPLEMENTED();
                return nullptr;
            }


            sectionView.indexData = buffer.data.data() + indexAccessor.byteOffset + indexBufferView.byteOffset;// / sectionView.indexElementSize;
            sectionView.indexCount = indexAccessor.count;/// / sectionView.indexElementSize;
		}

		// Topology
		if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
			sectionView.topology = PrimitiveTopology::TriangleList;
		}
		else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
			sectionView.topology = PrimitiveTopology::TriangleStrip;
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
		else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
			sectionView.topology = PrimitiveTopology::TriangleFan;
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
		else if (primitive.mode == TINYGLTF_MODE_POINTS) {
			sectionView.topology = PrimitiveTopology::PointList;
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
		else if (primitive.mode == TINYGLTF_MODE_LINE) {
			sectionView.topology = PrimitiveTopology::LineList;
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
		else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP) {
			LN_NOTIMPLEMENTED();
			return nullptr;
		}
		else {
			LN_UNREACHABLE();
			return nullptr;
		}

		// Morph Targets
		// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#morph-targets
		{
			for (auto itr = primitive.targets.begin(); itr != primitive.targets.end(); ++itr) {
				int morphTargetIndex = itr - primitive.targets.begin();

				int position = -1;
				int normal = -1;
				int tangent = -1;
				{
					for (auto itr2 = itr->begin(); itr2 != itr->end(); ++itr2) {
						if (itr2->first == "POSITION") {
							position = itr2->second;
						}
						else if (itr2->first == "NORMAL") {
							normal = itr2->second;
						}
						else if (itr2->first == "TANGENT") {
							tangent = itr2->second;
						}
						else {
							// glTF として標準サポートされているのは上記3つの属性のみ
							LN_WARNING("Ignored morph target.");
						}
					}
				}

				VertexMorphTargetView view;
				view.positions = (position >= 0) ? static_cast<const Vector3*>(getRawData(m_model->accessors[position])) : nullptr;
				view.normals = (normal >= 0) ? static_cast<const Vector3*>(getRawData(m_model->accessors[normal])) : nullptr;
				view.tangents = (tangent >= 0) ? static_cast<const Vector3*>(getRawData(m_model->accessors[tangent])) : nullptr;
				sectionView.morphTargetViews.push_back(std::move(view));
			}
		}

		meshView.sectionViews.push_back(std::move(sectionView));
	}


	return generateMesh(meshView);
}

template<class T>
void flipFaceIndex_Triangle(T* indices, int count)
{
	int c = count / 3;
	for (int i = 0; i < c; i++) {
		std::swap(indices[(i * 3) + 1], indices[(i * 3) + 2]);
	}
}

Ref<MeshContainer> GLTFImporter::generateMesh(const MeshView& meshView) const
{
	// 検証
	{
		int vertexCount = 0;
		for (auto& section : meshView.sectionViews) {
			int count = section.vertexBufferViews[0].count;
			for (size_t i = 0; i < section.vertexBufferViews.size(); i++) {
				auto& view = section.vertexBufferViews[i];
				if (count != view.count) {
					// ひとつの section 内ではすべての頂点数が一致していることが前提
					LN_ERROR();
					return nullptr;
				}
			}
			vertexCount += count;
		}
	}


	bool hasTangentAttribute = false;

	auto meshContainer = makeObject<MeshContainer>();

	for (const MeshPrimitiveView& primitiveView : meshView.sectionViews) {
		int vertexCount = primitiveView.vertexBufferViews[0].count;
		int indexCount = primitiveView.indexCount;
		IndexBufferFormat indexForamt = GraphicsHelper::selectIndexBufferFormat(vertexCount);

		auto meshPrimitive = makeObject<MeshPrimitive>(vertexCount, indexCount, indexForamt, GraphicsResourceUsage::Static);

		for (auto& vbView : primitiveView.vertexBufferViews) {
			auto reservedGroup = meshPrimitive->getStandardElement(vbView.usage, vbView.usageIndex);
			auto destType = meshPrimitive->findVertexElementType(vbView.usage, vbView.usageIndex);
			if (destType == VertexElementType::Unknown) {
				LN_WARNING("Detected unsaported vertex attibute.");
				continue;
			}

			auto* rawbuf = static_cast<byte_t*>(meshPrimitive->acquireMappedVertexBuffer(destType, vbView.usage, vbView.usageIndex));
			auto* src = static_cast<const byte_t*>(vbView.data);// +vbView.byteOffset;

			int offset = 0;
			int stride = 0;

			if (reservedGroup == InterleavedVertexGroup::Main) {
				stride = sizeof(Vertex);
				if (vbView.usage == VertexElementUsage::Position) offset = LN_MEMBER_OFFSETOF(Vertex, position);
				else if (vbView.usage == VertexElementUsage::Normal) offset = LN_MEMBER_OFFSETOF(Vertex, normal);
				else if (vbView.usage == VertexElementUsage::TexCoord) offset = LN_MEMBER_OFFSETOF(Vertex, uv);
				else if (vbView.usage == VertexElementUsage::Color) offset = LN_MEMBER_OFFSETOF(Vertex, color);
				else if (vbView.usage == VertexElementUsage::Tangent) offset = LN_MEMBER_OFFSETOF(Vertex, tangent);
				else {
					LN_ERROR();
				}
			}
			else if (reservedGroup == InterleavedVertexGroup::Skinning) {
				stride = sizeof(VertexBlendWeight);
				if (vbView.usage == VertexElementUsage::BlendIndices) offset = LN_MEMBER_OFFSETOF(VertexBlendWeight, indices);
				else if (vbView.usage == VertexElementUsage::BlendWeight) offset = LN_MEMBER_OFFSETOF(VertexBlendWeight, weights);
				else {
					LN_ERROR();
				}
			}
			else {
				stride = GraphicsHelper::getVertexElementTypeSize(vbView.type);
				//int size = GraphicsHelper::getVertexElementTypeSize(vbView.type);
				//for (int i = 0; i < vertexCountInSection; i++) {
				//	memcpy(&buf[(vertexOffset + i) * size], src + (vbView.byteStride * i), size);
				//	//memcpy(&buf[(vertexOffset + i) * size], src + (vbView.byteStride * i), size);
				//}
			}

			if (destType != vbView.type) {
				// Float4 <- Short4
				if (destType == VertexElementType::Float4 && vbView.type == VertexElementType::Short4) {
					for (int i = 0; i < vertexCount; i++) {
						float* d = (float*)(&rawbuf[(i * stride) + offset]);
						short* s = (short*)(src + (vbView.byteStride * i));
						d[0] = (float)s[0];
						d[1] = (float)s[1];
						d[2] = (float)s[2];
						d[3] = (float)s[3];
					}
				}
				// Float4 <- Float2. UV 座標など。
				else if (destType == VertexElementType::Float4 && vbView.type == VertexElementType::Float2) {
					for (int i = 0; i < vertexCount; i++) {
						float* d = (float*)(&rawbuf[(i * stride) + offset]);
						float* s = (float*)(src + (vbView.byteStride * i));
						d[0] = s[0];
						d[1] = s[1];
						d[2] = 0.0f;
						d[3] = 0.0f;
					}
				}
				else {
					LN_NOTIMPLEMENTED();
				}
			}
			else {
				// Note:  Blender で接線を Export できた時は Float4 to Float4 でここに来る。Tangent.w は 1.0 になっている。
				int size = GraphicsHelper::getVertexElementTypeSize(vbView.type);
				for (int i = 0; i < vertexCount; i++) {
					memcpy(&rawbuf[(i * stride) + offset], src + (vbView.byteStride * i), size);
				}
			}

			if (vbView.usage == VertexElementUsage::Tangent) {
				hasTangentAttribute = true;
			}
		}



		// Flip Z (RH to LH)
		if (m_flipZ) {
			auto* vertices = static_cast<Vertex*>(meshPrimitive->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
			const int count = meshPrimitive->vertexCount();
			for (int i = 0; i < count; i++) {
				vertices[i].position.z *= -1.0f;
				vertices[i].normal.z *= -1.0f;
			}
		}
		if (m_flipX) {
			auto* vertices = static_cast<Vertex*>(meshPrimitive->acquireMappedVertexBuffer(InterleavedVertexGroup::Main));
			const int count = meshPrimitive->vertexCount();
			for (int i = 0; i < count; i++) {
				vertices[i].position.x *= -1.0f;
				vertices[i].normal.x *= -1.0f;
			}
		}


		// Build index buffer.
		//   Lumino の MeshSection は glTF の Primitive と対応させているが、glTF の Primitive は様々な Index buffer を参照することができる。
		//   MeshSection で扱うにはそれらすべてをひとつの Index buffer に統合する必要がある。
		{
			void* buf = meshPrimitive->acquireMappedIndexBuffer();

			int beginVertexIndex = 0;
			int indexOffset = 0;
			//for (auto& section : meshView.sectionViews)
			{
				//int vertexCountInSection = section.vertexBufferViews[0].count;

				if (indexForamt == IndexBufferFormat::UInt16) {
					if (primitiveView.indexElementSize == 1) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint8_t*>(primitiveView.indexData);
					for (int i = 0; i < primitiveView.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
}
					}
					else if (primitiveView.indexElementSize == 2) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint16_t*>(primitiveView.indexData);
					for (int i = 0; i < primitiveView.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					}
					else if (primitiveView.indexElementSize == 4) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint32_t*>(primitiveView.indexData);
					for (int i = 0; i < primitiveView.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					}
					else {
					LN_NOTIMPLEMENTED();
					}
				}
				else if (indexForamt == IndexBufferFormat::UInt32) {
				if (primitiveView.indexElementSize == 4) {
					auto* b = static_cast<uint32_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint32_t*>(primitiveView.indexData);
					for (int i = 0; i < primitiveView.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					//flipFaceIndex_Triangle<uint32_t>(b, section.indexCount);
				}
				else if (primitiveView.indexElementSize == 2) {
					// glTF の Mesh は primitive という小単位のMeshに分かれることがある。
					// Mesh 全体としては UInt32 だが、primitive ごとにみると UInt16 となるようなこともある。
					// Blender から大きなモデルをエクスポートするとこのような形になった。
					auto* b = static_cast<uint32_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint16_t*>(primitiveView.indexData);
					for (int i = 0; i < primitiveView.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					//flipFaceIndex_Triangle<uint16_t>(b, section.indexCount);
				}
				else {
					LN_NOTIMPLEMENTED();
				}
				}
				else {
				LN_NOTIMPLEMENTED();
				}

				assert(primitiveView.topology == PrimitiveTopology::TriangleList);

				if (m_faceFlip) {
					switch (indexForamt) {
					case ln::IndexBufferFormat::UInt16: {
						auto* b = static_cast<uint16_t*>(buf) + indexOffset;
						const int count = primitiveView.indexCount / 3;
						for (int i = 0; i < count; i++) {
							std::swap(b[i * 3 + 1], b[i * 3 + 2]);
						}
						break;
					}
					case ln::IndexBufferFormat::UInt32: {
						auto* b = static_cast<uint32_t*>(buf) + indexOffset;
						const int count = primitiveView.indexCount / 3;
						for (int i = 0; i < count; i++) {
							std::swap(b[i * 3 + 1], b[i * 3 + 2]);
						}
						break;
					}
					default:
						LN_UNREACHABLE();
						break;
					}

				}


				// make mesh section
				// TODO: tri only
				meshPrimitive->addSection(
					indexOffset, primitiveView.indexCount / 3,
					primitiveView.materialIndex, primitiveView.topology);

				// next
				indexOffset += primitiveView.indexCount;
				//beginVertexIndex += vertexCountInSection;
			}

			// TODO: unmap 無いとめんどい以前に怖い

		}

		// Morph Targets
#if 1
		for (int i = 0; i < primitiveView.morphTargetViews.size(); i++) {
			const VertexMorphTargetView& morphTargetView = primitiveView.morphTargetViews[i];
			Vertex* vertices = static_cast<Vertex*>(meshPrimitive->acquireMappedMorphVertexBuffer(i));
			for (int iVertex = 0; iVertex < vertexCount; iVertex++) {
				if (morphTargetView.positions) vertices[iVertex].setPosition(morphTargetView.positions[iVertex]);
				if (morphTargetView.normals) vertices[iVertex].setNormal(morphTargetView.normals[iVertex]);
				if (morphTargetView.tangents) vertices[iVertex].tangent = Vector4(morphTargetView.tangents[iVertex], 1.0f);
			}
		}
#else
		for (int i = 0; i < primitiveView.morphTargetViews.size(); i++) {
			const VertexMorphTargetView& morphTargetView = primitiveView.morphTargetViews[i];
			if (morphTargetView.positions) {
				Vector3* vertices = static_cast<Vector3*>(meshPrimitive->acquireMappedMorphVertexBuffer(i, VertexElementUsage::Position));
				for (int iVertex = 0; iVertex < vertexCount; iVertex++) vertices[iVertex] = morphTargetView.positions[iVertex];
			}
			if (morphTargetView.normals) {
				Vector3* vertices = static_cast<Vector3*>(meshPrimitive->acquireMappedMorphVertexBuffer(i, VertexElementUsage::Normal));
				for (int iVertex = 0; iVertex < vertexCount; iVertex++) vertices[iVertex] = morphTargetView.normals[iVertex];
			}
			if (morphTargetView.tangents) {
				Vector3* vertices = static_cast<Vector3*>(meshPrimitive->acquireMappedMorphVertexBuffer(i, VertexElementUsage::Tangent));
				for (int iVertex = 0; iVertex < vertexCount; iVertex++) vertices[iVertex] = morphTargetView.tangents[iVertex];
			}
		}
#endif

		bool hasNormalMap = meshPrimitive->sections().containsIf([this](const MeshSection2& x) { return (x.materialIndex >= 0) && m_meshModel->materials()[x.materialIndex]->normalMap() != nullptr; });
		if (hasNormalMap) {
			if (!hasTangentAttribute) {
				meshPrimitive->calculateTangents();
			}
		}

		meshContainer->addMeshPrimitive(meshPrimitive);
	}

	return meshContainer;
}

Ref<MeshSkeleton> GLTFImporter::readSkin(const tinygltf::Skin& skin)
{
	// inverseBindMatrice はボーンの初期姿勢を打ち消して、原点に戻す行列。
	// 最終的に BoneTexture などへ書き出すときにこれを乗算する。（Matrix::Identity なら変形が行われないようにする）
	// src/LuminoEngine/sandbox/Assets/SkinnedMesh1.glb だと、2つめのボーンの
	// - node の 座標 (初期姿勢) は Y=1
	// - inverseBindMatrice は Y=-1
	// となっている。

	const tinygltf::Accessor& accessor = m_model->accessors[skin.inverseBindMatrices];
	LN_CHECK(accessor.count == skin.joints.size());
	LN_CHECK(accessor.type == TINYGLTF_TYPE_MAT4);
	LN_CHECK(accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);

	const tinygltf::BufferView& bufferView = m_model->bufferViews[accessor.bufferView];
	const tinygltf::Buffer& buffer = m_model->buffers[bufferView.buffer];

	const Matrix* inverseBindMatrices = (const Matrix*)(buffer.data.data() + accessor.byteOffset + bufferView.byteOffset);
	auto armature = makeObject<MeshSkeleton>(static_cast<SkinnedMeshModel*>(m_meshModel));
	for (int i = 0; i < skin.joints.size(); i++) {

		if (m_clearBoneRotation) {
			const auto& mat = m_meshModel->m_nodes[skin.joints[i]]->initialLocalTransform();
			armature->addBone(skin.joints[i], Matrix::makeInverse(mat));

			std::cout << mat.toString() << std::endl;
			std::cout << inverseBindMatrices[i].toString() << std::endl;

		}
		else
		{
			if (m_flipX) {
				// TODO: ちょっと処理重いか…
				Matrix t = Matrix::makeInverse(inverseBindMatrices[i]);
				t(3, 0) *= -1.0f;
				armature->addBone(skin.joints[i], Matrix::makeInverse(t));
			}
			else {
				armature->addBone(skin.joints[i], inverseBindMatrices[i]);
			}
		}

	}

	return armature;
}

Ref<Texture> GLTFImporter::loadTexture(const tinygltf::Texture& texture)
{
	// TODO: image->name で名前が取れるので、それでキャッシュ組んだ方がいいかも？
	// でも glb の場合は glb ファイル作った時点のテクスチャの内容が glb 内部に埋め込まれるから
	// キャッシュ組むとしたらどちらを優先するか指定できた方がいいかも。

	const tinygltf::Image& image = m_model->images[texture.source];
	if (image.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE &&  // GL_UNSIGNED_BYTE
		image.component == 4 &&     // RGBA
		image.bits == 8) {
	}
	else {
		LN_NOTIMPLEMENTED();
		return nullptr;
	}

	// 検索キー
	String textureName;
	if (!image.uri.empty()) {
		textureName = String::fromStdString(image.uri);
	}
	else {
		if (LN_REQUIRE(!m_meshModel->m_name.isEmpty())) return nullptr;
		textureName = String::concat(m_meshModel->m_name, _TT(":"), String::fromNumber(texture.source));
	}

	Ref<Texture2D> tex;

	if (1) {
		tex = m_meshManager->graphicsManager()->loadTexture2DFromOnMemoryData(&m_basedir, textureName, [&](const AssetRequiredPathSet* x) {
			Ref<Bitmap2D> bitmap = makeObject<Bitmap2D>(image.width, image.height, PixelFormat::RGBA8, image.image.data());
			return makeObject<Texture2D>(bitmap, GraphicsHelper::translateToTextureFormat(bitmap->format()));
		});
	}
	else {
		Ref<Bitmap2D> bitmap;
		bitmap = makeObject<Bitmap2D>(image.width, image.height, PixelFormat::RGBA8, image.image.data());

		return makeObject<Texture2D>(bitmap, GraphicsHelper::translateToTextureFormat(bitmap->format()));
	}

	int imageIndex = texture.source;
	if (imageIndex >= m_loadedTextures.size()) {
		m_loadedTextures.resize(imageIndex + 1);
	}
	m_loadedTextures[imageIndex] = tex;

	return tex;
}

Ref<AnimationClip> GLTFImporter::readAnimation(const tinygltf::Animation& animation) const
{
	auto clip = makeObject<AnimationClip>();
	clip->setName(String::fromStdString(animation.name));

	/*
	Note:
		input は time.
		output はキーの値。

		weights の場合、time 1 つに対して output 2 つ以上出てくることがあるが、これは channel.target_node が参照しているノードの mesh の weights に対応する。
		https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#morph-targets
		※プロ生ちゃんモデルだと、mesh.weights は 0 個だけど mesh.primitives.targets は 30 個とかあった。

		Lumino としてモーフィングアニメをサポートするときは、ScalarAnimationTrack を複数作って、
		<メッシュコンテナ名>.<表情名 or 表情Index(outputのIndex=mesh.weightsのIndex)>
		みたいな感じになるかな。メッシュコンテナ名は省略可でもいいかも。
	*/

	struct TransformTrackData
	{
		int translationFrames = 0;
		const float* translationTimes;
		const Vector3* translationValues;
		TransformAnimationTrack::Interpolation translationInterpolation;

		int rotationFrames = 0;
		const float* rotationTimes;
		const Quaternion* rotationValues;

		int scaleFrames = 0;
		const float* scaleTimes;
		const Vector3* scaleValues;
		TransformAnimationTrack::Interpolation scaleInterpolation;

		static TransformAnimationTrack::Interpolation getInterpolation(const std::string& value)
		{
			if (value == "STEP") return TransformAnimationTrack::Interpolation::Step;
			if (value == "LINEAR") return TransformAnimationTrack::Interpolation::Linear;
			if (value == "CUBICSPLINE") return TransformAnimationTrack::Interpolation::CubicSpline;
			LN_NOTIMPLEMENTED();
			return TransformAnimationTrack::Interpolation::Step;
		}
	};

	struct WeightAnimationTrackData
	{
		int frames = 0;
		const float* times;
		int weightCount = 0;
		const float* weights;
	};

	// key: node number
	std::unordered_map<int, TransformTrackData> transformTrackMap;

	// channel 対象要素ごとに存在する。一般的には、
	// - 表情アニメであれば "weights" だけ。
	// - ボーンアニメであれば "translation", "rotation", "scale" の 3 channel.
	for (const auto& channel : animation.channels) {
		const auto& sampler = animation.samplers[channel.sampler];
		const auto& node = m_model->nodes[channel.target_node];

		const auto& inputAccessor = m_model->accessors[sampler.input];
		const auto& inputBufferView = m_model->bufferViews[inputAccessor.bufferView];
		const auto& inputBuffer = m_model->buffers[inputBufferView.buffer];
		const auto* inputData = reinterpret_cast<const float*>(inputBuffer.data.data() + inputAccessor.byteOffset + inputBufferView.byteOffset);
		if (LN_REQUIRE(inputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && inputAccessor.type == TINYGLTF_TYPE_SCALAR)) return nullptr;

		const auto& outputAccessor = m_model->accessors[sampler.output];
		const auto& outputBufferView = m_model->bufferViews[outputAccessor.bufferView];
		const auto& outputBuffer = m_model->buffers[outputBufferView.buffer];
		const auto* outputData = reinterpret_cast<const float*>(outputBuffer.data.data() + outputAccessor.byteOffset + outputBufferView.byteOffset);

		if (inputAccessor.sparse.isSparse || outputAccessor.sparse.isSparse) {
			LN_NOTIMPLEMENTED();
			return nullptr;
		}

		if (channel.target_path == "translation") {
			if (LN_REQUIRE(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && outputAccessor.type == TINYGLTF_TYPE_VEC3)) return nullptr;
			auto& data = transformTrackMap[channel.target_node];
			data.translationFrames = inputAccessor.count;
			data.translationTimes = inputData;
			data.translationValues = reinterpret_cast<const Vector3*>(outputData);
			data.translationInterpolation = TransformTrackData::getInterpolation(sampler.interpolation);

			//for (int i = 0; i < inputAccessor.count; i++) {
			//	std::cout << i << ": " << inputData[i] << std::endl;
			//}
			//for (int i = 0; i < outputAccessor.count; i++) {
			//	std::cout << i << ": " << outputData[i] << std::endl;
			//}

		}
		else if (channel.target_path == "rotation") {
			if (LN_REQUIRE(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && outputAccessor.type == TINYGLTF_TYPE_VEC4)) return nullptr;
			auto& data = transformTrackMap[channel.target_node];
			data.rotationFrames = inputAccessor.count;
			data.rotationTimes = inputData;
			data.rotationValues = reinterpret_cast<const Quaternion*>(outputData);
		}
		else if (channel.target_path == "scale") {
			if (LN_REQUIRE(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && outputAccessor.type == TINYGLTF_TYPE_VEC3)) return nullptr;
			auto& data = transformTrackMap[channel.target_node];
			data.scaleFrames = inputAccessor.count;
			data.scaleTimes = inputData;
			data.scaleValues = reinterpret_cast<const Vector3*>(outputData);
			data.scaleInterpolation = TransformTrackData::getInterpolation(sampler.interpolation);
		}
		else if (channel.target_path == "weights") {
			if (LN_REQUIRE(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && outputAccessor.type == TINYGLTF_TYPE_SCALAR)) return nullptr;

			if (LN_REQUIRE(channel.target_node >= 0)) return nullptr;
			const auto& node = m_model->nodes[channel.target_node];

			if (LN_REQUIRE(node.mesh >= 0)) return nullptr;
			const auto& mesh = m_model->meshes[node.mesh];		// "weights" は MeshContainer Node を target としているはず

			// モーフターゲットの数 (=表情数)
			int weightCount = mesh.weights.size();

			// TODO: いまのところ float だけ対応している。
			// 他にも正規化済み BYTE とかに対応する必要がある。
			// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#animations
			if (outputAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT) {
				LN_NOTIMPLEMENTED();
				return nullptr;
			}

			// outputAccessor は、weightCount の倍数となっていなければならない。
			if (LN_REQUIRE((outputAccessor.count / inputAccessor.count) == weightCount)) return nullptr;

			// Create Track
			{
				for (int iWeight = 0; iWeight < weightCount; iWeight++) {
					
				}

				size_t stride = weightCount;
				for (int i = 0; i < outputAccessor.count; i++) {

				}

				//auto track = makeObject<WeightsAnimationTrack>();
				//track->setTargetName(String::fromStdString(node.name));
			}

			//sectionView.indexCount = indexAccessor.count;

			for (int i = 0; i < inputAccessor.count; i++) {
				std::cout << i << ": " << inputData[i] << std::endl;
			}
			for (int i = 0; i < outputAccessor.count; i++) {
				std::cout << i << ": " << outputData[i] << std::endl;
			}

			//LN_NOTIMPLEMENTED();
		}
		else {
			// glTF 2.0 では、上記以外に有効な target_path は存在しない
			// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#animations
			LN_ERROR("Invalid channel.target_path name.");
			return nullptr;
		}
	}

	for (const auto& pair : transformTrackMap) {
		const auto& node = m_model->nodes[pair.first];
		auto track = makeObject<TransformAnimationTrack>();
		track->setTargetName(String::fromStdString(node.name));


		const auto& data = pair.second;
		if (data.translationFrames > 0) track->setupTranslations(data.translationFrames, data.translationTimes, data.translationValues, data.translationInterpolation);
		if (data.rotationFrames > 0) track->setupRotations(data.rotationFrames, data.rotationTimes, data.rotationValues);
		if (data.scaleFrames > 0) track->setupScales(data.scaleFrames, data.scaleTimes, data.scaleValues, data.scaleInterpolation);

		// glTF の animation の姿勢には Node の初期姿勢が含まれている。
		// Lumino としては初期姿勢からの相対変化量だけほしいので、初期姿勢の分を打ち消す。
		{
			if (!node.translation.empty()) {
				const auto nodeInversePos = Vector3(-node.translation[0], -node.translation[1], -node.translation[2]);
				for (auto& k : track->m_translationKeys) {
					k.value += nodeInversePos;
				}
			}

			if (node.rotation.size() == 4) {
				const auto nodeInverseRot = Quaternion::makeInverse(Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]));
				for (auto& k : track->m_rotationKeys) {
					k.value *= nodeInverseRot;
				}
			}
			else {
				// 回転を持たないが、rotationFrames を持つことはある
			}
		}

		if (m_flipX) {
			for (auto& k : track->m_translationKeys) {
				k.value.x = -k.value.x;
			}
			for (auto& k : track->m_rotationKeys) {
				k.value.y = -k.value.y;
				k.value.z = -k.value.z;
			}
		}

		clip->addTrack(track);
	}

	return clip;
}

const void* GLTFImporter::getRawData(const tinygltf::Accessor& accessor) const
{
	const tinygltf::BufferView& bufferView = m_model->bufferViews[accessor.bufferView];
	const tinygltf::Buffer& buffer = m_model->buffers[bufferView.buffer];
	return buffer.data.data() + accessor.byteOffset + bufferView.byteOffset;
}

bool GLTFImporter::FileExists(const std::string &abs_filename, void *user_data)
{
	auto self = reinterpret_cast<GLTFImporter*>(user_data);
	auto assetPath = String::fromStdString(abs_filename);
	return self->m_assetManager->existsAsset(AssetPath::combineAssetPath(self->m_basedir, assetPath));
}

std::string GLTFImporter::ExpandFilePath(const std::string &filepath, void *user_data)
{
    return filepath;
	//auto self = reinterpret_cast<GLTFImporter*>(user_data);
	//auto assetPath = String::concat(self->m_basedir, _TT("/", String::fromStdString(filepath));
	//return assetPath.toStdString();	// TODO: UTF-8
}

bool GLTFImporter::ReadWholeFile(std::vector<unsigned char> *out, std::string *err, const std::string &filepath, void *user_data)
{
	auto self = reinterpret_cast<GLTFImporter*>(user_data);
	auto assetPath = String::fromStdString(filepath);
	auto stream = self->m_assetManager->openStreamFromAssetPath(AssetPath::combineAssetPath(self->m_basedir, assetPath));

	if (!stream) {
		if (err) {
			(*err) += "File open error : " + filepath + "\n";
		}
		return false;
	}

	int sz = stream->length();
	if (int(sz) < 0) {
		if (err) {
			(*err) += "Invalid file size : " + filepath + " (does the path point to a directory?)";
		}
		return false;
	}
	else if (sz == 0) {
		if (err) {
			(*err) += "File is empty : " + filepath + "\n";
		}
		return false;
	}

	out->resize(sz);
	stream->read(out->data(), sz);
	return true;
}

bool GLTFImporter::WriteWholeFile(std::string *err, const std::string &filepath, const std::vector<unsigned char> &contents, void *user_data)
{
	std::ofstream f(filepath.c_str(), std::ofstream::binary);
	if (!f) {
		if (err) {
			(*err) += "File open error for writing : " + filepath + "\n";
		}
		return false;
	}

	f.write(reinterpret_cast<const char *>(&contents.at(0)), static_cast<std::streamsize>(contents.size()));
	if (!f) {
		if (err) {
			(*err) += "File write error: " + filepath + "\n";
		}
		return false;
	}

	f.close();
	return true;
}


} // namespace detail
} // namespace ln
