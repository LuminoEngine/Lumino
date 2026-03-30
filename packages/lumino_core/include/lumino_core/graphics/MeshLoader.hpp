#pragma once
#include <vector>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefObject.hpp>
#include <lumino_core/graphics/Mesh.hpp>
#include <lumino_core/graphics/Material.hpp>
#include <lumino_core/graphics/Transform.hpp>
#include <string>

namespace ln {

/** A renderable object loaded from a model file. */
struct RenderObject {
    Ref<Mesh> mesh;
    Transform transform;
};

/** Result of loading a model file. */
struct LoadedModel {
    std::vector<RenderObject> objects;
    std::vector<Ref<Material>> materials;
};

/** Loads 3D models (glTF) into Mesh/Material objects. */
class MeshLoader {
public:
    /** Load a glTF or glb file. */
    static Result<LoadedModel> loadGltf(
        rhi::Device* device,
        const std::string& path);
};

} // namespace ln
