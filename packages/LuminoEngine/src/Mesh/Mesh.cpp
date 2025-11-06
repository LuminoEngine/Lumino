#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/detail\SpriteMeshGenerater.hpp>

namespace ln {

Ref<Mesh> Mesh::create() {
    Ref<Mesh> ref(LN_NEW Mesh(), false);
    return ref;
}

void Mesh::addSprite2DSurface(
    Material* material,
    Vector2 size,
    Vector2 anchor,
    Rect uvRect,
    Color color) {
    detail::SpriteMeshGenerater generater;
    generater.data.size = size;
    generater.data.anchorRatio = anchor;
    generater.data.srcUVRect = uvRect;
    generater.data.color = color;
    generater.data.baseDirection = SpriteBaseDirection::Basic2D;
    generater.data.billboardType = BillboardType::None;
    generater.data.flipFlags = SpriteFlipFlags::None;

    
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

	Ref<LinearAllocator> allocator = makeRef<LinearAllocator>(m_manager->graphicsManager()->linearAllocatorPageManager());
    detail::MeshGeneraterBuffer buffer(allocator);
    buffer.setBuffer(vertexBuffer.writableData(), indexBuffer.writableData(), indexBuffer->format(), 0);
    buffer.generate(generater);

    MeshSurfaceData surface;
    surface.vertexCount = vertexCount;
    surface.mainVertexBuffer = vertexBuffer;
    surface.indexCount = indexCount;
    surface.indexBuffer = indexBuffer;
    surface.vertexLayout = vertexLayout;
    surface.material = material;
}


} // namespace ln
