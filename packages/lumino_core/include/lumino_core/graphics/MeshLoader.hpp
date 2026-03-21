#pragma once
#include <vector>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefCounted.hpp>
#include <lumino_core/graphics/Mesh.hpp>
#include <lumino_core/graphics/Material.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <string>

namespace ln {

/// A renderable object loaded from a model file.
struct RenderObject {
    Ref<Mesh> mesh;
    Transform transform;
};

/// Result of loading a model file.
struct LoadedModel {
    std::vector<RenderObject> objects;
    std::vector<Ref<Material>> materials;
};

/// Loads 3D models (glTF) into Mesh/Material objects.
class MeshLoader {
public:
    /// Load a glTF or glb file.
    /// @param pipelineLayout Shared PipelineLayout for all materials (3 BindGroupLayouts: view, material, object).
    static Result<LoadedModel> loadGltf(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        const std::string& path,
        rhi::TextureFormat colorFormat = rhi::TextureFormat::BGRA8Unorm,
        rhi::TextureFormat depthFormat = rhi::TextureFormat::Depth32Float);
};

} // namespace ln
