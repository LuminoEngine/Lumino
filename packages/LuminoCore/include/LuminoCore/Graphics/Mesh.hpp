#pragma once
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <LuminoCore/Graphics/Material.hpp>

namespace ln {

/** A sub-range of a mesh's index buffer, associated with a material. */
struct SubMesh {
    uint32_t indexOffset = 0;
    uint32_t indexCount = 0;
    uint32_t materialIndex = 0;
};

/** A mesh containing vertex/index GPU buffers and submesh ranges. */
class Mesh : public Object {
public:
    /** Create a mesh from CPU data and upload to GPU. */
    static Result<Ref<Mesh>> create(
        rhi::Device* device,
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        const std::vector<SubMesh>& submeshes,
        rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList);

    /** Create a dynamic mesh with host-visible buffers for per-frame CPU updates. */
    static Result<Ref<Mesh>> createDynamic(
        rhi::Device* device,
        uint32_t maxVertexCount,
        uint32_t maxIndexCount,
        rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList);

    /** Update vertex data (only valid for dynamic meshes). */
    Result<void> updateVertices(uint32_t firstVertex, const Vertex* vertices, uint32_t count);

    /** Update index data (only valid for dynamic meshes). */
    Result<void> updateIndices(uint32_t firstIndex, const uint32_t* indices, uint32_t count);

    /** Replace the submesh array and resize material slots accordingly. */
    void setSubmeshes(const std::vector<SubMesh>& submeshes);

    rhi::Buffer* vertexBuffer() const { return m_vertexBuffer.get(); }
    rhi::Buffer* indexBuffer() const { return m_indexBuffer.get(); }
    const std::vector<SubMesh>& submeshes() const { return m_submeshes; }
    rhi::PrimitiveTopology topology() const { return m_topology; }

    /** Material slots for this mesh. One per unique materialIndex used in submeshes. */
    std::vector<Ref<Material>>& materials() { return m_materials; }
    const std::vector<Ref<Material>>& materials() const { return m_materials; }

private:
    rhi::Device* m_device = nullptr;
    Ref<rhi::Buffer> m_vertexBuffer;
    Ref<rhi::Buffer> m_indexBuffer;
    std::vector<SubMesh> m_submeshes;
    std::vector<Ref<Material>> m_materials;
    rhi::PrimitiveTopology m_topology = rhi::PrimitiveTopology::TriangleList;
    bool m_dynamic = false;
    uint32_t m_maxVertexCount = 0;
    uint32_t m_maxIndexCount = 0;
};

} // namespace ln
