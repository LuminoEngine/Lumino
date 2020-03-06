
#include "Internal.hpp"
#define TINYGLTF_IMPLEMENTATION
//#define TINYGLTF_NO_FS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../build/BuildCache/tinygltf/tiny_gltf.h"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Asset/Assets.hpp>
#include "../Asset/AssetManager.hpp"
#include "GLTFImporter.hpp"

namespace ln {
namespace detail {

Ref<StaticMeshModel> GLTFImporter::import(AssetManager* assetManager, const AssetPath& assetPath, DiagnosticsManager* diag)
{
	m_assetManager = assetManager;
    m_basedir = assetPath.getParentAssetPath();//ln::Path(assetPath).parent();
	auto stream = assetManager->openStreamFromAssetPath(assetPath);
	auto data = stream->readToEnd();

	tinygltf::Model model;
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
        result = loader.LoadBinaryFromMemory(&model, &err, &warn, data.data(), data.size(), "");
    }
    else {
        result = loader.LoadASCIIFromString(&model, &err, &warn, (const char*)data.data(), data.size(), "");
    }
	if (!err.empty()) diag->reportError(String::fromStdString(err));
	if (!warn.empty()) diag->reportError(String::fromStdString(warn));
	if (!result) {
		return nullptr;
	}
	else {
		m_model = &model;
        m_meshModel = makeObject<StaticMeshModel>();

        for (auto& material : m_model->materials) {
            m_meshModel->addMaterial(readMaterial(material));
        }

        for (auto& mesh : m_model->meshes) {
            auto meshContainer = readMesh(mesh);
            if (!meshContainer) {
                return nullptr;
            }
            m_meshModel->addMeshContainer(meshContainer);
        }

        for (auto& node : m_model->nodes) {
            auto meshNode = readNode(node);
            if (!meshNode) {
                return nullptr;
            }
            m_meshModel->addNode(meshNode);
        }

        int scene_to_display = model.defaultScene > -1 ? model.defaultScene : 0;
        const tinygltf::Scene &scene = model.scenes[scene_to_display];
        for (size_t i = 0; i < scene.nodes.size(); i++) {
            m_meshModel->addRootNode(scene.nodes[i]);
        }

        m_meshModel->updateNodeTransforms();
		return m_meshModel;
	}
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
            coreMaterial->setMetallic(itr->second.number_value);
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

            auto coreTexture = makeObject<Texture2D>(bitmap, GraphicsHelper::translateToTextureFormat(bitmap->format()));
            coreMaterial->setMainTexture(coreTexture);
        }
    }

    {
        auto itr = material.values.find("metallicRoughnessTexture");
        if (itr != material.values.end()) {
            auto itr2 = itr->second.json_double_value.find("index");
            assert(itr2 != itr->second.json_double_value.end());
            int index = static_cast<int>(itr2->second);
            // TODO:
        }
    }

    {
        auto itr = material.additionalValues.find("normalTexture");
        if (itr != material.additionalValues.end()) {
            auto itr2 = itr->second.json_double_value.find("index");
            assert(itr2 != itr->second.json_double_value.end());
            int index = static_cast<int>(itr2->second);
            // TODO:
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

Ref<MeshNode> GLTFImporter::readNode(const tinygltf::Node& node)
{
	Matrix nodeTransform;
	if (node.matrix.size() == 16) {
		const double* m = node.matrix.data();
        nodeTransform.set(
            m[0], m[1], m[2], m[3],
            m[4], m[5], m[6], m[7],
            m[8], m[9], m[10], m[11],
            m[12], m[13], m[14], m[15]);

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

    auto coreNode = makeObject<MeshNode>();
    coreNode->setLocalTransform(nodeTransform);

    if (node.mesh > -1) {
        assert(node.mesh < m_model->meshes.size());
        coreNode->setMeshContainerIndex(node.mesh);
	}

	for (size_t i = 0; i < node.children.size(); i++) {
		assert(node.children[i] < m_model->nodes.size());
        coreNode->addChildIndex(node.children[i]);
	}

	return coreNode;
}

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
			int elementSize = 1;
			if (accessor.type == TINYGLTF_TYPE_SCALAR) {
				vbView.type = VertexElementType::Float1;
				elementSize = 1;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC2) {
				vbView.type = VertexElementType::Float2;
				elementSize = 2;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC3) {
				vbView.type = VertexElementType::Float3;
				elementSize = 3;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC4) {
				vbView.type = VertexElementType::Float4;
				elementSize = 4;
			}
			else {
				LN_UNREACHABLE();
				return nullptr;
			}

			// Usage
			vbView.usage = VertexElementUsage::Unknown;
			vbView.usageIndex = 0;
			if (itr->first.compare("POSITION") == 0) {
				vbView.usage = VertexElementUsage::Position;
				vbView.usageIndex = 0;
			}
			else if (itr->first.compare("NORMAL") == 0) {
				vbView.usage = VertexElementUsage::Normal;
				vbView.usageIndex = 0;
			}
			else if (itr->first.compare("TEXCOORD_0") == 0) {
				vbView.usage = VertexElementUsage::TexCoord;
				vbView.usageIndex = 0;
			}
            else if (itr->first.compare("TANGENT") == 0) {
                vbView.usage = VertexElementUsage::Tangent;
                vbView.usageIndex = 0;
            }
			else if (itr->first.compare("COLOR_0") == 0) {
				vbView.usage = VertexElementUsage::Color;
				vbView.usageIndex = 0;
			}
			else {
				LN_UNREACHABLE();
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
		}
		else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
			sectionView.topology = PrimitiveTopology::TriangleFan;
		}
		else if (primitive.mode == TINYGLTF_MODE_POINTS) {
			sectionView.topology = PrimitiveTopology::PointList;
		}
		else if (primitive.mode == TINYGLTF_MODE_LINE) {
			sectionView.topology = PrimitiveTopology::LineList;
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
	int indexElementSize = 0;
	for (auto& section : meshView.sectionViews) {
		indexCount += section.indexCount;
		indexElementSize = std::max(indexElementSize, section.indexElementSize);
	}

	// select index format.
	IndexBufferFormat indexForamt;
	if (indexElementSize == 1 || indexElementSize == 2) {
		indexForamt = IndexBufferFormat::UInt16;
		indexElementSize = 2;
	}
	else if (indexElementSize == 4) {
		indexForamt = IndexBufferFormat::UInt32;
	}
	else {
		LN_NOTIMPLEMENTED();
		return nullptr;
	}

	auto coreMesh = makeObject<Mesh>(vertexCount, indexCount, indexForamt);




	// Build vertex buffer.
	int vertexOffset = 0;
    for (auto& section : meshView.sectionViews) {
        int vertexCountInSection = section.vertexBufferViews[0].count;

        for (auto& vbView : section.vertexBufferViews) {
			auto reservedGroup = coreMesh->getStandardElement(vbView.usage, vbView.usageIndex);
            auto* rawbuf = static_cast<byte_t*>(coreMesh->acquireMappedVertexBuffer(vbView.type, vbView.usage, vbView.usageIndex));
            auto* src = static_cast<const byte_t*>(vbView.data);// +vbView.byteOffset;

			int offset = 0;
			int stride = 0;

			if (reservedGroup == InterleavedVertexGroup::Main) {
				stride = sizeof(Vertex);
				if (vbView.usage == VertexElementUsage::Position) offset = LN_MEMBER_OFFSETOF(Vertex, position);
				else if (vbView.usage == VertexElementUsage::Normal) offset = LN_MEMBER_OFFSETOF(Vertex, normal);
				else if (vbView.usage == VertexElementUsage::TexCoord) offset = LN_MEMBER_OFFSETOF(Vertex, uv);
				else if (vbView.usage == VertexElementUsage::Color) offset = LN_MEMBER_OFFSETOF(Vertex, color);
				else {
					LN_ERROR();
				}
			}
			else if (reservedGroup == InterleavedVertexGroup::Tangents) {
				stride = sizeof(VertexTangents);
				if (vbView.usage == VertexElementUsage::Tangent) offset = LN_MEMBER_OFFSETOF(VertexTangents, tangent);
				else if (vbView.usage == VertexElementUsage::Binormal) offset = LN_MEMBER_OFFSETOF(VertexTangents, binormal);
				else {
					LN_ERROR();
				}
			}
			else if (reservedGroup == InterleavedVertexGroup::Skinning) {
				stride = sizeof(VertexBlendWeight);
				if (vbView.usage == VertexElementUsage::Tangent) offset = LN_MEMBER_OFFSETOF(VertexBlendWeight, weights);
				else if (vbView.usage == VertexElementUsage::Binormal) offset = LN_MEMBER_OFFSETOF(VertexBlendWeight, indices);
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

			int size = GraphicsHelper::getVertexElementTypeSize(vbView.type);
			for (int i = 0; i < vertexCountInSection; i++) {
				memcpy(&rawbuf[((vertexOffset + i) * stride) + offset], src + (vbView.byteStride * i), size);
			}




			// Flip Z (RH to LH)
			{
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
				else if (reservedGroup == InterleavedVertexGroup::Main && vbView.usage == VertexElementUsage::Normal) {
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

			if (indexElementSize == 2) {
				if (section.indexElementSize == 1) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint8_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					flipFaceIndex_Triangle<uint16_t>(b, section.indexCount);
				}
				else if (section.indexElementSize == 2) {
					auto* b = static_cast<uint16_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint16_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					flipFaceIndex_Triangle<uint16_t>(b, section.indexCount);
				}
				else if (section.indexElementSize == 4) {
				}
				else {
					LN_NOTIMPLEMENTED();
				}
			}
			else if (indexElementSize == 4) {
				if (section.indexElementSize == 4) {
					auto* b = static_cast<uint32_t*>(buf) + indexOffset;
					auto* s = static_cast<const uint32_t*>(section.indexData);
					for (int i = 0; i < section.indexCount; i++) {
						b[i] = beginVertexIndex + s[i];
						assert(b[i] < vertexCount);
					}
					flipFaceIndex_Triangle<uint32_t>(b, section.indexCount);
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


	return coreMesh;
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
