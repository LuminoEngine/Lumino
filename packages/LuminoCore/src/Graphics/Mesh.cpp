#include <LuminoCore/Graphics/Mesh.hpp>
#include <cstring>

namespace ln {

Result<Ref<Mesh>> Mesh::create(
    rhi::Device* device,
    const std::vector<Vertex>& vertices,
    const std::vector<u32>& indices,
    const std::vector<SubMesh>& submeshes,
    rhi::PrimitiveTopology topology) {

    auto mesh = Ref<Mesh>::adopt(new Mesh());
    mesh->m_device = device;
    mesh->m_topology = topology;

    // Create vertex buffer.
    rhi::BufferDesc vbDesc;
    vbDesc.size = vertices.size() * sizeof(Vertex);
    vbDesc.usage = rhi::BufferUsage::Vertex;
    vbDesc.initialData = vertices.data();
    auto vbResult = device->createBuffer(vbDesc);
    if (!vbResult) return tl::make_unexpected(vbResult.error());
    mesh->m_vertexBuffer = std::move(*vbResult);

    // Create index buffer.
    rhi::BufferDesc ibDesc;
    ibDesc.size = indices.size() * sizeof(u32);
    ibDesc.usage = rhi::BufferUsage::Index;
    ibDesc.initialData = indices.data();
    auto ibResult = device->createBuffer(ibDesc);
    if (!ibResult) return tl::make_unexpected(ibResult.error());
    mesh->m_indexBuffer = std::move(*ibResult);

    mesh->m_submeshes = submeshes;

    // Determine how many material slots are needed.
    u32 maxMaterialIndex = 0;
    for (auto& sub : submeshes) {
        if (sub.materialIndex > maxMaterialIndex) maxMaterialIndex = sub.materialIndex;
    }
    mesh->m_materials.resize(maxMaterialIndex + 1);

    return mesh;
}

Result<Ref<Mesh>> Mesh::createDynamic(
    rhi::Device* device,
    u32 maxVertexCount,
    u32 maxIndexCount,
    rhi::PrimitiveTopology topology) {

    auto mesh = Ref<Mesh>::adopt(new Mesh());
    mesh->m_device = device;
    mesh->m_topology = topology;
    mesh->m_dynamic = true;
    mesh->m_maxVertexCount = maxVertexCount;
    mesh->m_maxIndexCount = maxIndexCount;

    // Create vertex buffer for dynamic updates.
    rhi::BufferDesc vbDesc;
    vbDesc.size = static_cast<u64>(maxVertexCount) * sizeof(Vertex);
    vbDesc.usage = rhi::BufferUsage::Vertex;
    auto vbResult = device->createBuffer(vbDesc);
    if (!vbResult) return tl::make_unexpected(vbResult.error());
    mesh->m_vertexBuffer = std::move(*vbResult);

    // Create index buffer for dynamic updates.
    rhi::BufferDesc ibDesc;
    ibDesc.size = static_cast<u64>(maxIndexCount) * sizeof(u32);
    ibDesc.usage = rhi::BufferUsage::Index;
    auto ibResult = device->createBuffer(ibDesc);
    if (!ibResult) return tl::make_unexpected(ibResult.error());
    mesh->m_indexBuffer = std::move(*ibResult);

    return mesh;
}

Result<void> Mesh::updateVertices(u32 firstVertex, const Vertex* vertices, u32 count) {
    if (!m_dynamic) return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Mesh is not dynamic"});
    if (firstVertex + count > m_maxVertexCount) return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Vertex range out of bounds"});

    return m_device->writeBuffer(
        m_vertexBuffer.get(),
        static_cast<u64>(firstVertex) * sizeof(Vertex),
        vertices,
        static_cast<u64>(count) * sizeof(Vertex));
}

Result<void> Mesh::updateIndices(u32 firstIndex, const u32* indices, u32 count) {
    if (!m_dynamic) return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Mesh is not dynamic"});
    if (firstIndex + count > m_maxIndexCount) return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Index range out of bounds"});

    return m_device->writeBuffer(
        m_indexBuffer.get(),
        static_cast<u64>(firstIndex) * sizeof(u32),
        indices,
        static_cast<u64>(count) * sizeof(u32));
}

void Mesh::setSubmeshes(const std::vector<SubMesh>& submeshes) {
    m_submeshes = submeshes;

    // Resize material slots to accommodate new submeshes.
    u32 maxMaterialIndex = 0;
    for (auto& sub : submeshes) {
        if (sub.materialIndex > maxMaterialIndex) maxMaterialIndex = sub.materialIndex;
    }
    if (maxMaterialIndex + 1 > static_cast<u32>(m_materials.size())) {
        m_materials.resize(maxMaterialIndex + 1);
    }
}

} // namespace ln
