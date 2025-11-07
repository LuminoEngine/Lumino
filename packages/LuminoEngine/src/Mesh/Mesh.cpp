#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Graphics/GPU/VertexLayout.hpp>
#include <LuminoEngine/Graphics/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/IndexBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Mesh/detail/SpriteMeshGenerater.hpp>
#include <LuminoEngine/Mesh/detail/MeshManager.hpp>

namespace ln {

Ref<Mesh> Mesh::create() {
    Ref<Mesh> ref(LN_NEW Mesh(), false);
    return ref;
}

Mesh::Mesh()
    : m_manager(detail::MeshManager::instance()) {
}

void Mesh::addSprite2DSurface(
    Material* material,
    Vector2 size,
    Vector2 anchor,
    Rect uvRect,
    Color color) {
    detail::SpriteMeshGenerater generater;
    generater.sprite.size = size;
    generater.sprite.anchorRatio = anchor;
    generater.sprite.srcUVRect = uvRect;
    generater.sprite.color = color;
    generater.sprite.baseDirection = SpriteBaseDirection::Basic2D;
    generater.sprite.billboardType = BillboardType::None;
    generater.sprite.flipFlags = SpriteFlipFlags::None;

    Ref<VertexLayout> vertexLayout = makeObject_deprecated<VertexLayout>();
    int streamIndex = 0;
    vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Position, 0);
    vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Normal, 0);
    vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::TexCoord, 0);
    vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Color, 0);
    vertexLayout->addElement(streamIndex, VertexElementType::Float4, VertexElementUsage::Tangent, 0);

    int32_t vertexCount = generater.vertexCount();
    int32_t indexCount = generater.indexCount();
    Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(vertexCount * sizeof(Vertex));
    Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indexCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);

	Ref<detail::LinearAllocator> allocator = makeRef<detail::LinearAllocator>(m_manager->linearAllocatorPageManager());
    detail::MeshGeneraterBuffer buffer(allocator);
    void* indexData = indexBuffer->map(MapMode::Write);
    buffer.setBuffer(reinterpret_cast<Vertex*>(vertexBuffer->writableData()), indexData, indexBuffer->format(), 0);
    buffer.generate(&generater);

    MeshSurfaceData surface;
    surface.vertexCount = vertexCount;
    surface.mainVertexBuffer = vertexBuffer;
    surface.indexCount = indexCount;
    surface.indexBuffer = indexBuffer;
    surface.vertexLayout = vertexLayout;
    surface.material = material;
    m_surfaces.push(surface);
}


} // namespace ln
