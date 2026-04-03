#pragma once
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Transform.hpp>
#include <string>

namespace ln {

class GraphicsModule;

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
        GraphicsModule* module,
        const std::string& path);
};

} // namespace ln
