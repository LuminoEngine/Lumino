#pragma once
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <LuminoCore/Graphics/Material.hpp>

namespace ln {

/** マテリアルに対応付けられた、メッシュのインデックスバッファの部分範囲。 */
struct SubMesh {
    uint32_t indexOffset = 0;
    uint32_t indexCount = 0;
    uint32_t materialIndex = 0;
};

/** 頂点/インデックスの GPU バッファとサブメッシュの範囲を持つメッシュ。 */
class Mesh : public Object {
public:
    /** CPU 側のデータからメッシュを作成し、GPU へアップロードします。 */
    static Result<Ref<Mesh>> create(
        rhi::Device* device,
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& indices,
        const std::vector<SubMesh>& submeshes,
        rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList);

    /** フレームごとの CPU 更新用に、ホストから見えるバッファを持つ動的メッシュを作成します。 */
    static Result<Ref<Mesh>> createDynamic(
        rhi::Device* device,
        uint32_t maxVertexCount,
        uint32_t maxIndexCount,
        rhi::PrimitiveTopology topology = rhi::PrimitiveTopology::TriangleList);

    /** 頂点データを更新します (動的メッシュでのみ有効)。 */
    Result<void> updateVertices(uint32_t firstVertex, const Vertex* vertices, uint32_t count);

    /** インデックスデータを更新します (動的メッシュでのみ有効)。 */
    Result<void> updateIndices(uint32_t firstIndex, const uint32_t* indices, uint32_t count);

    /** サブメッシュ配列を置き換え、それに合わせてマテリアルスロットをリサイズします。 */
    void setSubmeshes(const SubMesh* submeshes, uint32_t count);

    rhi::Buffer* vertexBuffer() const { return m_vertexBuffer.get(); }
    rhi::Buffer* indexBuffer() const { return m_indexBuffer.get(); }
    const std::vector<SubMesh>& submeshes() const { return m_submeshes; }
    rhi::PrimitiveTopology topology() const { return m_topology; }

    /** このメッシュのマテリアルスロット。サブメッシュで使われる materialIndex ごとに 1 つ。 */
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
