#pragma once
#include <vector>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefCounted.hpp>
#include <lumino_core/graphics/Vertex.hpp>
#include <lumino_core/graphics/Material.hpp>

namespace lumino {

/// A sub-range of a mesh's index buffer, associated with a material.
struct SubMesh {
    u32 indexOffset = 0;
    u32 indexCount = 0;
    u32 materialIndex = 0;
};

/// A mesh containing vertex/index GPU buffers and submesh ranges.
class Mesh : public RefCounted {
public:
    /// Create a mesh from CPU data and upload to GPU.
    static Result<Ref<Mesh>> create(
        rhi::Device* device,
        const std::vector<Vertex>& vertices,
        const std::vector<u32>& indices,
        const std::vector<SubMesh>& submeshes);

    rhi::Buffer* vertexBuffer() const { return vertexBuffer_.get(); }
    rhi::Buffer* indexBuffer() const { return indexBuffer_.get(); }
    const std::vector<SubMesh>& submeshes() const { return submeshes_; }

    /// Material slots for this mesh. One per unique materialIndex used in submeshes.
    std::vector<Ref<Material>>& materials() { return materials_; }
    const std::vector<Ref<Material>>& materials() const { return materials_; }

private:
    Ref<rhi::Buffer> vertexBuffer_;
    Ref<rhi::Buffer> indexBuffer_;
    std::vector<SubMesh> submeshes_;
    std::vector<Ref<Material>> materials_;
};

} // namespace lumino
