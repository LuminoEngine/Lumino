#pragma once
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/graphics/Vertex.hpp>
#include <LuminoCore/graphics/Material.hpp>

namespace ln {

/** A sub-range of a mesh's index buffer, associated with a material. */
struct SubMesh {
    u32 indexOffset = 0;
    u32 indexCount = 0;
    u32 materialIndex = 0;
};

/** A mesh containing vertex/index GPU buffers and submesh ranges. */
class Mesh : public Object {
public:
    /** Create a mesh from CPU data and upload to GPU. */
    static Result<Ref<Mesh>> create(
        rhi::Device* device,
        const std::vector<Vertex>& vertices,
        const std::vector<u32>& indices,
        const std::vector<SubMesh>& submeshes,
        rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList);

    rhi::Buffer* vertexBuffer() const { return m_vertexBuffer.get(); }
    rhi::Buffer* indexBuffer() const { return m_indexBuffer.get(); }
    const std::vector<SubMesh>& submeshes() const { return m_submeshes; }
    rhi::PrimitiveTopology topology() const { return m_topology; }

    /** Material slots for this mesh. One per unique materialIndex used in submeshes. */
    std::vector<Ref<Material>>& materials() { return m_materials; }
    const std::vector<Ref<Material>>& materials() const { return m_materials; }

private:
    Ref<rhi::Buffer> m_vertexBuffer;
    Ref<rhi::Buffer> m_indexBuffer;
    std::vector<SubMesh> m_submeshes;
    std::vector<Ref<Material>> m_materials;
    rhi::PrimitiveTopology m_topology = rhi::PrimitiveTopology::TriangleList;
};

} // namespace ln
