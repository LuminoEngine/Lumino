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

} // namespace ln
