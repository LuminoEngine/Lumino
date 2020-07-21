
#include "Internal.hpp"
#define TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_NO_FS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../build/BuildCache/tinygltf/tiny_gltf.h"
//#include "D:\Tech\Graphics\tinygltf\tiny_gltf.h"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include <LuminoEngine/Animation/AnimationCurve.hpp>
#include <LuminoEngine/Animation/AnimationTrack.hpp>
#include <LuminoEngine/Animation/AnimationClip.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include "../Asset/AssetManager.hpp"
#include "GLTFImporter.hpp"

namespace ln {
namespace detail {

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

bool GLTFImporter::importAsStaticMesh(StaticMeshModel* model, AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
	m_assetManager = assetManager;
	m_diag = diag;

	if (!openGLTFModel(assetPath)) {
		return false;
	}

	m_meshModel = model;

	readCommon(m_meshModel);

	return true;
}

bool GLTFImporter::GLTFImporter::importAsSkinnedMesh(SkinnedMeshModel* model, AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
	m_assetManager = assetManager;
	m_diag = diag;

	if (!openGLTFModel(assetPath)) {
		return false;
	}

	m_meshModel = model;

	readCommon(m_meshModel);

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

	return true;
}

bool GLTFImporter::readCommon(StaticMeshModel* meshModel)
{
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

    {
        auto itr = material.values.find("baseColorFactor");
        if (itr != material.values.end()) {
            auto& c = itr->second.number_array;
            assert(c.size() == 4);
            coreMaterial->setColor(Color(c[0], c[1], c[2], c[3]));
        }
    }

    {
        auto itr = material.values.find("metallicFactor");
        if (itr != material.values.end()) {
            assert(itr->second.has_number_value);
            coreMaterial->setMetallic(itr->second.number_value);
        }
        else {
            coreMaterial->setMetallic(1.0f);    // glTF default
        }
    }

    {
        auto itr = material.values.find("roughnessFactor");
        if (itr != material.values.end()) {
            assert(itr->second.has_number_value);
            coreMaterial->setRoughness(itr->second.number_value);
        }
        else {
            coreMaterial->setRoughness(1.0f);    // glTF default
        }
    }

    {
        auto itr = material.values.find("baseColorTexture");
        if (itr != material.values.end()) {
            auto itr2 = itr->second.json_double_value.find("index");
            assert(itr2 != itr->second.json_double_value.end());
            int index = static_cast<int>(itr2->second);
            const tinygltf::Texture& texture = m_model->textures[index];
			coreMaterial->setMainTexture(loadTexture(texture));
        }
    }

    {
        auto itr = material.values.find("metallicRoughnessTexture");
        if (itr != material.values.end()) {
            auto itr2 = itr->second.json_double_value.find("index");
            assert(itr2 != itr->second.json_double_value.end());
            int index = static_cast<int>(itr2->second);
            // TODO:
			m_diag->reportWarning(u"metallicRoughnessTexture not supported.");
        }
    }

    {
        auto itr = material.additionalValues.find("normalTexture");
        if (itr != material.additionalValues.end()) {
            auto itr2 = itr->second.json_double_value.find("index");
            assert(itr2 != itr->second.json_double_value.end());
            int index = static_cast<int>(itr2->second);
			const tinygltf::Texture& texture = m_model->textures[index];
			coreMaterial->setNormalMap(loadTexture(texture));

			// Blender でマテリアルの Normal を HeightMap にすると、glTF ではここに流れてくる。
			// ただし、glTF としては HeightMap はサポートされていない。
			// https://github.com/KhronosGroup/glTF/issues/948
			// いまは手動で直してもらうしかない。
        }
    }

    {
        auto itr = material.additionalValues.find("occlusionTexture");
        if (itr != material.additionalValues.end()) {
            auto itr2 = itr->second.json_double_value.find("index");
            assert(itr2 != itr->second.json_double_value.end());
            int index = static_cast<int>(itr2->second);
            // TODO:
        }
    }

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
			nodeTransform.rotateQuaternion(
				Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]));
		}

		if (node.translation.size() == 3) {
			nodeTransform.translate(node.translation[0], node.translation[1], node.translation[2]);
		}
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

    int indexBufferViewIndex = -1;
    int indexComponentType = -1;

	for (size_t i = 0; i < mesh.primitives.size(); i++) {
		const tinygltf::Primitive& primitive = mesh.primitives[i];
		if (primitive.indices < 0) return nullptr;

		SectionView sectionView;
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
					m_diag->reportError(u"Invalid vertex data type.");
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
					m_diag->reportError(u"Invalid vertex data type.");
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
					m_diag->reportError(u"Invalid vertex data type.");
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
					m_diag->reportError(u"Invalid vertex data type.");
					return nullptr;
				}
			}
			else if (itr->first.compare("COLOR_0") == 0) {
				vbView.usage = VertexElementUsage::Color;
				vbView.usageIndex = 0;
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
					m_diag->reportError(u"Invalid vertex data type.");
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
					m_diag->reportError(u"Invalid vertex data type.");
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
					m_diag->reportError(u"Invalid vertex data type.");
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

		meshView.sectionViews.push_back(std::move(sectionView));
	}

    auto meshContainer = makeObject<MeshContainer>();
    meshContainer->setMesh(generateMesh(meshView));

	return meshContainer;
}

template<class T>
void flipFaceIndex_Triangle(T* indices, int count)
{
	int c = count / 3;
	for (int i = 0; i < c; i++) {
		std::swap(indices[(i * 3) + 1], indices[(i * 3) + 2]);
	}
}

Ref<Mesh> GLTFImporter::generateMesh(const MeshView& meshView) const
{
	ElapsedTimer timer;


	bool hasTangentAttribute = false;

	// Validation and count vertices.
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

	// count indices and measure element size.
	int indexCount = 0;
	//int indexElementSize = 0;
	for (auto& section : meshView.sectionViews) {
		indexCount += section.indexCount;
	//	indexElementSize = std::max(indexElementSize, section.indexElementSize);
	}
	//int indexElementSize = detail::GraphicsResourceInternal::selectIndexBufferFormat(vertexCount);

	//// select index format.
	//IndexBufferFormat indexForamt;
	//if (indexElementSize == 1 || indexElementSize == 2) {
	//	indexForamt = IndexBufferFormat::UInt16;
	//	indexElementSize = 2;
	//}
	//else if (indexElementSize == 4) {
	//	indexForamt = IndexBufferFormat::UInt32;
	//}
	//else {
	//	LN_NOTIMPLEMENTED();
	//	return nullptr;
	//}
	IndexBufferFormat indexForamt = GraphicsHelper::selectIndexBufferFormat(vertexCount);

	auto coreMesh = makeObject<Mesh>(vertexCount, indexCount, indexForamt, GraphicsResourceUsage::Static);




	// Build vertex buffer.
	int vertexOffset = 0;
    for (auto& section : meshView.sectionViews) {
        int vertexCountInSection = section.vertexBufferViews[0].count;

        for (auto& vbView : section.vertexBufferViews) {
			auto reservedGroup = coreMesh->getStandardElement(vbView.usage, vbView.usageIndex);
			auto destType = coreMesh->findVertexElementType(vbView.usage, vbView.usageIndex);
            auto* rawbuf = static_cast<byte_t*>(coreMesh->acquireMappedVertexBuffer(destType, vbView.usage, vbView.usageIndex));
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
					for (int i = 0; i < vertexCountInSection; i++) {
						float* d = (float*)(&rawbuf[((vertexOffset + i) * stride) + offset]);
						short* s = (short*)(src + (vbView.byteStride * i));
						d[0] = (float)s[0];
						d[1] = (float)s[1];
						d[2] = (float)s[2];
						d[3] = (float)s[3];
					}
				}
				// Float4 <- Float2. UV 座標など。
				else if (destType == VertexElementType::Float4 && vbView.type == VertexElementType::Float2) {
					for (int i = 0; i < vertexCountInSection; i++) {
						float* d = (float*)(&rawbuf[((vertexOffset + i) * stride) + offset]);
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
				for (int i = 0; i < vertexCountInSection; i++) {
					memcpy(&rawbuf[((vertexOffset + i) * stride) + offset], src + (vbView.byteStride * i), size);
				}
			}

			if (vbView.usage == VertexElementUsage::Tangent) {
				hasTangentAttribute = true;
			}



			// Flip Z (RH to LH)
			{
#if 0	// 単に z 反転しただけだと、Node 行列との齟齬がでるのか、正しい位置に描画されなくなる
				if (reservedGroup == InterleavedVertexGroup::Main && vbView.usage == VertexElementUsage::Position) {
					if (vbView.type == VertexElementType::Float3) {
						auto* p = reinterpret_cast<Vertex*>(rawbuf);
						for (int i = 0; i < vertexCountInSection; i++) {
							p[i].position.z *= -1.0f;
						}
					}
					else {
						LN_NOTIMPLEMENTED();
						return nullptr;
					}
				}
				else
				if (reservedGroup == InterleavedVertexGroup::Main && vbView.usage == VertexElementUsage::Normal) {
					if (vbView.type == VertexElementType::Float3) {
						auto* p = reinterpret_cast<Vertex*>(rawbuf);
						for (int i = 0; i < vertexCountInSection; i++) {
							p[i].normal.z *= -1.0f;
						}
					}
					else {
						LN_NOTIMPLEMENTED();
						return nullptr;
					}
				}
#endif
			}
            // TODO: unmap 無いとめんどい以前に怖い
        }

        vertexOffset += vertexCountInSection;
    }



	// Build index buffer.
	//   Lumino の MeshSection は glTF の Primitive と対応させているが、glTF の Primitive は様々な Index buffer を参照することができる。
	//   MeshSection で扱うにはそれらすべてをひとつの Index buffer に統合する必要がある。
	{
		void* buf = coreMesh->acquireMappedIndexBuffer();

		int beginVertexIndex = 0;
		int indexOffset = 0;
		for (auto& section : meshView.sectionViews) {
			int vertexCountInSection = section.vertexBufferViews[0].count;

			if (indexForamt == IndexBufferFormat::UInt16) {
				if (section.indexElementSize == 1) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint8_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
				}
				else if (section.indexElementSize == 2) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint16_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
				}
				else if (section.indexElementSize == 4) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint32_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
				}
				else {
					LN_NOTIMPLEMENTED();
				}
			}
			else if (indexForamt == IndexBufferFormat::UInt32) {
				if (section.indexElementSize == 4) {
					auto* b = static_cast<uint32_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint32_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					//flipFaceIndex_Triangle<uint32_t>(b, section.indexCount);
				}
				else if (section.indexElementSize == 2) {
					// glTF の Mesh は primitive という小単位のMeshに分かれることがある。
					// Mesh 全体としては UInt32 だが、primitive ごとにみると UInt16 となるようなこともある。
					// Blender から大きなモデルをエクスポートするとこのような形になった。
					auto* b = static_cast<uint32_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint16_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
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

			assert(section.topology == PrimitiveTopology::TriangleList);

			// make mesh section
			// TODO: tri only
			coreMesh->addSection(indexOffset, section.indexCount / 3, section.materialIndex, section.topology);

			// next
			indexOffset += section.indexCount;
			beginVertexIndex += vertexCountInSection;
		}

		// TODO: unmap 無いとめんどい以前に怖い

	}


	bool hasNormalMap = coreMesh->sections().containsIf([this](const MeshSection2& x) { return (x.materialIndex >= 0) && m_meshModel->materials()[x.materialIndex]->normalMap() != nullptr; });
	if (hasNormalMap) {
		if (!hasTangentAttribute) {
			coreMesh->calculateTangents();
		}
	}



	//for (int vi = 0; vi < coreMesh->vertexCount(); vi++) {
	//	auto v = coreMesh->vertex(vi);
	//	//v.position.y = 0;
	//	//coreMesh->setVertex(vi, v);
	//	printf("%f, %f, %f\n", v.position.x, v.position.y, v.position.z);
	//}
	//for (int vi = 0; vi < coreMesh->indexCount(); vi++) {
	//	auto v = coreMesh->index(vi);
	//	std::cout << v << std::endl;
	//}

	//coreMesh->setIndex(0, 0);
	//coreMesh->setIndex(1, 1);
	//coreMesh->setIndex(2, 2);

	//coreMesh->setSection(0, 0, 100, 0, PrimitiveTopology::TriangleList);

	// TODO: set to mesh
	auto aabb = MeshHelper::makeAABB(
		static_cast<const Vertex*>(coreMesh->acquireMappedVertexBuffer(InterleavedVertexGroup::Main)),
		vertexCount);

	std::cout << "GenerateMesh: " << timer.elapsedMilliseconds() << "[ms]" << std::endl;

	return coreMesh;
}

Ref<MeshArmature> GLTFImporter::readSkin(const tinygltf::Skin& skin)
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
	auto armature = makeObject<MeshArmature>(static_cast<SkinnedMeshModel*>(m_meshModel));
	for (int i = 0; i < skin.joints.size(); i++) {
		armature->addBone(skin.joints[i], inverseBindMatrices[i]);
	}

	return armature;
}

Ref<Texture> GLTFImporter::loadTexture(const tinygltf::Texture& texture)
{
	// TODO: image->name で名前が取れるので、それでキャッシュ組んだ方がいいかも？
	// でも glb の場合は glb ファイル作った時点のテクスチャの内容が glb 内部に埋め込まれるから
	// キャッシュ組むとしたらどちらを優先するか指定できた方がいいかも。

	const tinygltf::Image& image = m_model->images[texture.source];

	Ref<Bitmap2D> bitmap;
	if (image.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE &&  // GL_UNSIGNED_BYTE
		image.component == 4 &&     // RGBA
		image.bits == 8) {
		bitmap = makeObject<Bitmap2D>(image.width, image.height, PixelFormat::RGBA8, image.image.data());
	}
	else {
		LN_NOTIMPLEMENTED();
		return nullptr;
	}

	return makeObject<Texture2D>(bitmap, GraphicsHelper::translateToTextureFormat(bitmap->format()));
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

	// key: node number
	std::unordered_map<int, TransformTrackData> transformTrackMap;

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

		if (channel.target_path == "weights") {
			if (LN_REQUIRE(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && outputAccessor.type == TINYGLTF_TYPE_SCALAR)) return nullptr;

			//sectionView.indexCount = indexAccessor.count;

			for (int i = 0; i < inputAccessor.count; i++) {
				std::cout << i << ": " << inputData[i] << std::endl;
			}
			for (int i = 0; i < outputAccessor.count; i++) {
				std::cout << i << ": " << outputData[i] << std::endl;
			}

			LN_NOTIMPLEMENTED();
		}
		else if (channel.target_path == "translation") {
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
			if (LN_REQUIRE(outputAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && outputAccessor.type == TINYGLTF_TYPE_VEC4)) return nullptr;
			auto& data = transformTrackMap[channel.target_node];
			data.scaleFrames = inputAccessor.count;
			data.scaleTimes = inputData;
			data.scaleValues = reinterpret_cast<const Vector3*>(outputData);
			data.scaleInterpolation = TransformTrackData::getInterpolation(sampler.interpolation);
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

		clip->addTrack(track);
	}

	return clip;
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
	//auto assetPath = String::concat(self->m_basedir, u"/", String::fromStdString(filepath));
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
