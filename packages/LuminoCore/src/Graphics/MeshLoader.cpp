// stb_image implementation is in TextureLoader.cpp - avoid duplicate.
// Include stb_image.h ourselves (via angle brackets for vcpkg), then tell
// tinygltf not to include it again (it uses quotes which may fail).
#include <stb_image.h>
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#include <tiny_gltf.h>

#include <LuminoCore/Graphics/MeshLoader.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Graphics/TextureLoader.hpp>
#include <LuminoBase/math/Math.hpp>
#include <cstring>
#include <algorithm>

namespace ln {

/** Helper: read accessor data as float array. */
static std::vector<float> readAccessorAsFloat(
    const tinygltf::Model& model,
    int accessorIndex) {

    const auto& accessor = model.accessors[accessorIndex];
    const auto& bufferView = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bufferView.buffer];

    int components = 1;
    if (accessor.type == TINYGLTF_TYPE_VEC2) components = 2;
    else if (accessor.type == TINYGLTF_TYPE_VEC3) components = 3;
    else if (accessor.type == TINYGLTF_TYPE_VEC4) components = 4;

    std::vector<float> result(accessor.count * components);

    const size_t stride = bufferView.byteStride > 0
        ? bufferView.byteStride
        : components * sizeof(float);

    const unsigned char* base =
        buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    for (size_t i = 0; i < accessor.count; ++i) {
        const unsigned char* element = base + i * stride;
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
            std::memcpy(&result[i * components], element, components * sizeof(float));
        } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
            for (int c = 0; c < components; ++c)
                result[i * components + c] = element[c] / 255.0f;
        } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            for (int c = 0; c < components; ++c) {
                uint16_t v;
                std::memcpy(&v, element + c * 2, 2);
                result[i * components + c] = v / 65535.0f;
            }
        }
    }
    return result;
}

/** Helper: read index accessor as uint32_t array. */
static std::vector<uint32_t> readIndices(
    const tinygltf::Model& model,
    int accessorIndex) {

    const auto& accessor = model.accessors[accessorIndex];
    const auto& bufferView = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bufferView.buffer];

    const unsigned char* base =
        buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;

    std::vector<uint32_t> result(accessor.count);
    const size_t stride = bufferView.byteStride > 0
        ? bufferView.byteStride
        : 0;  // 0 means tightly packed

    for (size_t i = 0; i < accessor.count; ++i) {
        const unsigned char* element = base;
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            size_t s = stride > 0 ? stride : 4;
            element = base + i * s;
            std::memcpy(&result[i], element, 4);
        } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            size_t s = stride > 0 ? stride : 2;
            element = base + i * s;
            uint16_t v;
            std::memcpy(&v, element, 2);
            result[i] = v;
        } else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
            size_t s = stride > 0 ? stride : 1;
            element = base + i * s;
            result[i] = *element;
        }
    }
    return result;
}

/** Helper: extract Transform from a glTF node. */
static Transform nodeTransform(const tinygltf::Node& node) {
    Transform t;
    if (node.matrix.size() == 16) {
        // Decompose matrix into TRS (simplified: just extract translation)
        t.position = Vector3{
            static_cast<float>(node.matrix[12]),
            static_cast<float>(node.matrix[13]),
            static_cast<float>(node.matrix[14])};
    }
    if (node.translation.size() == 3) {
        t.position = Vector3{
            static_cast<float>(node.translation[0]),
            static_cast<float>(node.translation[1]),
            static_cast<float>(node.translation[2])};
    }
    if (node.rotation.size() == 4) {
        t.rotation = Quaternion{
            static_cast<float>(node.rotation[0]),
            static_cast<float>(node.rotation[1]),
            static_cast<float>(node.rotation[2]),
            static_cast<float>(node.rotation[3])};
    }
    if (node.scale.size() == 3) {
        t.scale = Vector3{
            static_cast<float>(node.scale[0]),
            static_cast<float>(node.scale[1]),
            static_cast<float>(node.scale[2])};
    }
    return t;
}

/** Load textures from glTF images. */
static std::vector<Ref<rhi::Texture>> loadGltfTextures(
    rhi::Device* device,
    const tinygltf::Model& model) {

    std::vector<Ref<rhi::Texture>> textures;
    textures.reserve(model.images.size());
    for (auto& img : model.images) {
        if (img.image.empty() || img.width <= 0 || img.height <= 0) {
            textures.push_back(nullptr);
            continue;
        }
        // tinygltf decodes to component*bits/8 bytes per pixel.
        // We need RGBA8, so convert if needed.
        std::vector<unsigned char> rgba;
        const unsigned char* pixelData = img.image.data();
        size_t pixelSize = img.image.size();
        if (img.component == 3) {
            // RGB → RGBA
            rgba.resize(img.width * img.height * 4);
            for (int p = 0; p < img.width * img.height; ++p) {
                rgba[p * 4 + 0] = img.image[p * 3 + 0];
                rgba[p * 4 + 1] = img.image[p * 3 + 1];
                rgba[p * 4 + 2] = img.image[p * 3 + 2];
                rgba[p * 4 + 3] = 255;
            }
            pixelData = rgba.data();
            pixelSize = rgba.size();
        }

        rhi::TextureDesc texDesc;
        texDesc.width = static_cast<uint32_t>(img.width);
        texDesc.height = static_cast<uint32_t>(img.height);
        texDesc.format = rhi::TextureFormat::RGBA8Unorm;
        texDesc.usage = rhi::TextureUsage::Sampled | rhi::TextureUsage::CopyDst;
        texDesc.initialData = pixelData;
        auto texResult = device->createTexture(texDesc);
        if (!texResult) {
            textures.push_back(nullptr);
        } else {
            textures.push_back(std::move(*texResult));
        }
    }
    return textures;
}

Result<LoadedModel> MeshLoader::loadGltf(
    GraphicsModule* module,
    const std::string& path) {
    auto* device = module->device();

    // Load glTF file.
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string errStr, warnStr;

    bool ok = false;
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".glb") {
        ok = loader.LoadBinaryFromFile(&model, &errStr, &warnStr, path);
    } else {
        ok = loader.LoadASCIIFromFile(&model, &errStr, &warnStr, path);
    }
    if (!ok) {
        return LN_MAKE_ERROR("%s", errStr.c_str());
    }

    // Load textures from embedded images.
    auto gpuTextures = loadGltfTextures(device, model);

    // Create materials.
    LoadedModel result;
    result.materials.reserve(model.materials.size());
    for (auto& gltfMat : model.materials) {
        auto& pbr = gltfMat.pbrMetallicRoughness;
        bool hasTexture = pbr.baseColorTexture.index >= 0;
        bool isLit = gltfMat.alphaMode != "BLEND"; // Simple heuristic

        Ref<Material> mat;
        if (isLit) {
            auto r = MaterialFactory::createBasicLit(module);
            if (!r) return LN_FORWARD_ERROR(r);
            mat = std::move(*r);
        } else {
            auto r = MaterialFactory::createUnlit(module);
            if (!r) return LN_FORWARD_ERROR(r);
            mat = std::move(*r);
        }

        // Set base color.
        mat->setColor(Color{
            static_cast<float>(pbr.baseColorFactor[0]),
            static_cast<float>(pbr.baseColorFactor[1]),
            static_cast<float>(pbr.baseColorFactor[2]),
            static_cast<float>(pbr.baseColorFactor[3])});

        // Set texture.
        if (hasTexture) {
            int texIndex = pbr.baseColorTexture.index;
            if (texIndex >= 0 && texIndex < static_cast<int>(model.textures.size())) {
                int imgIndex = model.textures[texIndex].source;
                if (imgIndex >= 0 && imgIndex < static_cast<int>(gpuTextures.size()) && gpuTextures[imgIndex]) {
                    mat->setTexture(gpuTextures[imgIndex].get());
                }
            }
        }

        // Double-sided.
        if (gltfMat.doubleSided) {
            mat->setCullMode(rhi::CullMode::None);
        }

        // Alpha blend.
        if (gltfMat.alphaMode == "BLEND") {
            mat->setBlendEnabled(true);
        }

        result.materials.push_back(std::move(mat));
    }

    // Create a default material if no materials exist.
    if (result.materials.empty()) {
        auto r = MaterialFactory::createUnlit(module);
        if (!r) return LN_FORWARD_ERROR(r);
        result.materials.push_back(std::move(*r));
    }

    // Process nodes to find meshes.
    // Iterate over all nodes in the default scene (or all nodes if no scene).
    std::vector<int> nodeIndices;
    if (!model.scenes.empty()) {
        int sceneIndex = model.defaultScene >= 0 ? model.defaultScene : 0;
        // Flatten scene graph (no hierarchy for now, just top-level nodes + children).
        std::vector<int> stack = model.scenes[sceneIndex].nodes;
        while (!stack.empty()) {
            int ni = stack.back();
            stack.pop_back();
            nodeIndices.push_back(ni);
            auto& node = model.nodes[ni];
            for (int child : node.children) {
                stack.push_back(child);
            }
        }
    } else {
        for (int i = 0; i < static_cast<int>(model.nodes.size()); ++i) {
            nodeIndices.push_back(i);
        }
    }

    for (int ni : nodeIndices) {
        auto& node = model.nodes[ni];
        if (node.mesh < 0) continue;

        auto& gltfMesh = model.meshes[node.mesh];

        // Merge all primitives of this mesh into one Mesh object.
        std::vector<Vertex> allVertices;
        std::vector<uint32_t> allIndices;
        std::vector<SubMesh> submeshes;

        for (auto& prim : gltfMesh.primitives) {
            if (prim.mode != -1 && prim.mode != TINYGLTF_MODE_TRIANGLES) {
                continue;  // Skip non-triangles.
            }

            uint32_t vertexOffset = static_cast<uint32_t>(allVertices.size());
            uint32_t indexOffset = static_cast<uint32_t>(allIndices.size());

            // Read positions (required).
            auto posIt = prim.attributes.find("POSITION");
            if (posIt == prim.attributes.end()) continue;
            auto positions = readAccessorAsFloat(model, posIt->second);
            size_t vertexCount = positions.size() / 3;

            // Read normals.
            std::vector<float> normals;
            auto normIt = prim.attributes.find("NORMAL");
            if (normIt != prim.attributes.end()) {
                normals = readAccessorAsFloat(model, normIt->second);
            }

            // Read UVs.
            std::vector<float> uvs;
            auto uvIt = prim.attributes.find("TEXCOORD_0");
            if (uvIt != prim.attributes.end()) {
                uvs = readAccessorAsFloat(model, uvIt->second);
            }

            // Read colors.
            std::vector<float> colors;
            auto colIt = prim.attributes.find("COLOR_0");
            if (colIt != prim.attributes.end()) {
                colors = readAccessorAsFloat(model, colIt->second);
            }

            // Read tangents.
            std::vector<float> tangents;
            auto tanIt = prim.attributes.find("TANGENT");
            if (tanIt != prim.attributes.end()) {
                tangents = readAccessorAsFloat(model, tanIt->second);
            }

            // Build vertices.
            for (size_t v = 0; v < vertexCount; ++v) {
                Vertex vtx{};
                vtx.position = Vector3{positions[v * 3], positions[v * 3 + 1], positions[v * 3 + 2]};

                if (normals.size() >= (v + 1) * 3) {
                    vtx.normal = Vector3{normals[v * 3], normals[v * 3 + 1], normals[v * 3 + 2]};
                }
                if (uvs.size() >= (v + 1) * 2) {
                    vtx.uv = Vector2{uvs[v * 2], uvs[v * 2 + 1]};
                }
                if (colors.size() >= (v + 1) * 4) {
                    vtx.color = Color{colors[v * 4], colors[v * 4 + 1], colors[v * 4 + 2], colors[v * 4 + 3]};
                } else {
                    vtx.color = Color::white();
                }
                if (tangents.size() >= (v + 1) * 4) {
                    vtx.tangent = Vector4{tangents[v * 4], tangents[v * 4 + 1], tangents[v * 4 + 2], tangents[v * 4 + 3]};
                }
                allVertices.push_back(vtx);
            }

            // Read indices.
            if (prim.indices >= 0) {
                auto indices = readIndices(model, prim.indices);
                for (auto idx : indices) {
                    allIndices.push_back(idx + vertexOffset);
                }

                SubMesh sub;
                sub.indexOffset = indexOffset;
                sub.indexCount = static_cast<uint32_t>(indices.size());
                sub.materialIndex = static_cast<uint32_t>(
                    prim.material >= 0 ? prim.material : 0);
                submeshes.push_back(sub);
            } else {
                // No indices - generate sequential indices.
                for (uint32_t i = 0; i < static_cast<uint32_t>(vertexCount); ++i) {
                    allIndices.push_back(vertexOffset + i);
                }
                SubMesh sub;
                sub.indexOffset = indexOffset;
                sub.indexCount = static_cast<uint32_t>(vertexCount);
                sub.materialIndex = static_cast<uint32_t>(
                    prim.material >= 0 ? prim.material : 0);
                submeshes.push_back(sub);
            }
        }

        if (allVertices.empty()) continue;

        auto meshResult = Mesh::create(device, allVertices, allIndices, submeshes);
        if (!meshResult) return LN_FORWARD_ERROR(meshResult);

        auto& mesh = *meshResult;
        // Assign materials to mesh slots.
        for (auto& sub : submeshes) {
            uint32_t idx = sub.materialIndex;
            if (idx < result.materials.size() && idx < mesh->materials().size()) {
                mesh->materials()[idx] = result.materials[idx];
            }
        }

        RenderObject obj;
        obj.mesh = std::move(mesh);
        obj.transform = nodeTransform(node);
        result.objects.push_back(std::move(obj));
    }

    return result;
}

} // namespace ln
