#include <LuminoCore/graphics/Mesh.hpp>

namespace ln {

Result<Ref<Mesh>> Mesh::create(
    rhi::Device* device,
    const std::vector<Vertex>& vertices,
    const std::vector<u32>& indices,
    const std::vector<SubMesh>& submeshes,
    rhi::PrimitiveTopology topology) {

    auto mesh = Ref<Mesh>::adopt(new Mesh());
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

} // namespace ln
