#include <LuminoCore/Graphics/Mesh.hpp>
#include <cstring>

namespace ln {

Result<Ref<Mesh>> Mesh::create(
    rhi::Device* device,
    const std::vector<Vertex>& vertices,
    const std::vector<uint32_t>& indices,
    const std::vector<SubMesh>& submeshes,
    rhi::PrimitiveTopology topology) {

    auto mesh = Ref<Mesh>::adopt(new Mesh());
    mesh->m_device = device;
    mesh->m_topology = topology;

    // 頂点バッファを作成する。
    rhi::BufferDesc vbDesc;
    vbDesc.size = vertices.size() * sizeof(Vertex);
    vbDesc.usage = rhi::BufferUsage::Vertex;
    vbDesc.initialData = vertices.data();
    auto vbResult = device->createBuffer(vbDesc);
    if (!vbResult) return LN_FORWARD_ERROR(vbResult);
    mesh->m_vertexBuffer = std::move(*vbResult);

    // インデックスバッファを作成する。
    rhi::BufferDesc ibDesc;
    ibDesc.size = indices.size() * sizeof(uint32_t);
    ibDesc.usage = rhi::BufferUsage::Index;
    ibDesc.initialData = indices.data();
    auto ibResult = device->createBuffer(ibDesc);
    if (!ibResult) return LN_FORWARD_ERROR(ibResult);
    mesh->m_indexBuffer = std::move(*ibResult);

    mesh->m_submeshes = submeshes;

    // 必要なマテリアルスロット数を求める。
    uint32_t maxMaterialIndex = 0;
    for (auto& sub : submeshes) {
        if (sub.materialIndex > maxMaterialIndex) maxMaterialIndex = sub.materialIndex;
    }
    mesh->m_materials.resize(maxMaterialIndex + 1);

    return mesh;
}

Result<Ref<Mesh>> Mesh::createDynamic(
    rhi::Device* device,
    uint32_t maxVertexCount,
    uint32_t maxIndexCount,
    rhi::PrimitiveTopology topology) {

    auto mesh = Ref<Mesh>::adopt(new Mesh());
    mesh->m_device = device;
    mesh->m_topology = topology;
    mesh->m_dynamic = true;
    mesh->m_maxVertexCount = maxVertexCount;
    mesh->m_maxIndexCount = maxIndexCount;

    // 動的更新用の頂点バッファを作成する。
    // mappable=true でホストから見えるメモリになり、writeBuffer() はステージング +
    // vkQueueWaitIdle (毎フレーム GPU をストールさせる) ではなく map/memcpy を使う。
    rhi::BufferDesc vbDesc;
    vbDesc.size = static_cast<uint64_t>(maxVertexCount) * sizeof(Vertex);
    vbDesc.usage = rhi::BufferUsage::Vertex;
    vbDesc.mappable = true;
    auto vbResult = device->createBuffer(vbDesc);
    if (!vbResult) return LN_FORWARD_ERROR(vbResult);
    mesh->m_vertexBuffer = std::move(*vbResult);

    // 動的更新用のインデックスバッファを作成する。
    rhi::BufferDesc ibDesc;
    ibDesc.size = static_cast<uint64_t>(maxIndexCount) * sizeof(uint32_t);
    ibDesc.usage = rhi::BufferUsage::Index;
    ibDesc.mappable = true;
    auto ibResult = device->createBuffer(ibDesc);
    if (!ibResult) return LN_FORWARD_ERROR(ibResult);
    mesh->m_indexBuffer = std::move(*ibResult);

    return mesh;
}

Result<void> Mesh::updateVertices(uint32_t firstVertex, const Vertex* vertices, uint32_t count) {
    if (!m_dynamic) return LN_MAKE_ERROR("Mesh is not dynamic");
    if (firstVertex + count > m_maxVertexCount) return LN_MAKE_ERROR("Vertex range out of bounds");

    return m_device->writeBuffer(
        m_vertexBuffer.get(),
        static_cast<uint64_t>(firstVertex) * sizeof(Vertex),
        vertices,
        static_cast<uint64_t>(count) * sizeof(Vertex));
}

Result<void> Mesh::updateIndices(uint32_t firstIndex, const uint32_t* indices, uint32_t count) {
    if (!m_dynamic) return LN_MAKE_ERROR("Mesh is not dynamic");
    if (firstIndex + count > m_maxIndexCount) return LN_MAKE_ERROR("Index range out of bounds");

    return m_device->writeBuffer(
        m_indexBuffer.get(),
        static_cast<uint64_t>(firstIndex) * sizeof(uint32_t),
        indices,
        static_cast<uint64_t>(count) * sizeof(uint32_t));
}

void Mesh::setSubmeshes(const SubMesh* submeshes, uint32_t count) {
    m_submeshes.assign(submeshes, submeshes + count);

    // 新しいサブメッシュに合わせてマテリアルスロットをリサイズする。
    uint32_t maxMaterialIndex = 0;
    for (auto& sub : m_submeshes) {
        if (sub.materialIndex > maxMaterialIndex) maxMaterialIndex = sub.materialIndex;
    }
    if (maxMaterialIndex + 1 > static_cast<uint32_t>(m_materials.size())) {
        m_materials.resize(maxMaterialIndex + 1);
    }
}

} // namespace ln
