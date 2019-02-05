
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "PrimitiveRenderFeature.hpp"

namespace ln {
namespace detail {

InternalPrimitiveRenderer::InternalPrimitiveRenderer()
{
}

InternalPrimitiveRenderer::~InternalPrimitiveRenderer()
{
}

void InternalPrimitiveRenderer::init(RenderingManager* manager)
{
    m_manager = manager;
    m_linearAllocator = makeRef<LinearAllocator>(m_manager->graphicsManager()->linearAllocatorPageManager());
    prepareBuffers(m_manager->graphicsManager()->deviceContext(), 512, 512 * 3);
    m_vertexDeclaration = m_manager->standardVertexDeclaration()->resolveRHIObject();
}

void InternalPrimitiveRenderer::drawMeshGenerater(const MeshGenerater* generator)
{
    // Verify
    if (m_generators.isEmpty()) {
        m_primitiveType = generator->primitiveType();
    }
    else {
        if (LN_REQUIRE(m_primitiveType == generator->primitiveType())) return;
    }

    MeshGenerater* gen = generator->clone(m_linearAllocator);
    m_generators.add(gen);
}

void InternalPrimitiveRenderer::flush(IGraphicsDeviceContext* context)
{
    if (m_generators.isEmpty()) return;

    // Prepare buffers
    int vertexCount = 0;
    int indexCount = 0;
    for (MeshGenerater* gen : m_generators) {
        vertexCount += gen->vertexCount();
        indexCount += gen->indexCount();
    }
    prepareBuffers(context, vertexCount, indexCount);

    // Create Vertex and Index buffers
    Vertex* vertexBuffer = (Vertex*)m_vertexBuffer->map();
    uint16_t* indexBuffer = (uint16_t*)m_indexBuffer->map();
    MeshGeneraterBuffer buffer;
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    for (MeshGenerater* gen : m_generators) {
        buffer.setBuffer(vertexBuffer + vertexOffset, indexBuffer + indexOffset, IndexBufferFormat::UInt16, vertexOffset);
        buffer.generate(gen);
        //gen->generate(&buffer);
        vertexOffset += gen->vertexCount();
        indexOffset += gen->indexCount();
    }
    m_vertexBuffer->unmap();
    m_indexBuffer->unmap();


    int primitiveCount = 0;
    switch (m_primitiveType)
    {
    case ln::PrimitiveTopology::TriangleList:
        primitiveCount = indexCount / 3;
        break;
    case ln::PrimitiveTopology::TriangleStrip:
        LN_NOTIMPLEMENTED();
        break;
    case ln::PrimitiveTopology::TriangleFan:
        LN_NOTIMPLEMENTED();
        break;
    case ln::PrimitiveTopology::LineList:
        primitiveCount = indexCount / 2;
        break;
    case ln::PrimitiveTopology::LineStrip:
        LN_NOTIMPLEMENTED();
        break;
    case ln::PrimitiveTopology::PointList:
        LN_NOTIMPLEMENTED();
        break;
    default:
        break;
    }

    // Render
    context->setVertexDeclaration(m_vertexDeclaration);
    context->setVertexBuffer(0, m_vertexBuffer);
    context->setIndexBuffer(m_indexBuffer);
	context->setPrimitiveTopology(m_primitiveType);
    context->drawPrimitiveIndexed(0, primitiveCount);

    for (MeshGenerater* gen : m_generators) {
        gen->~MeshGenerater();
    }

    m_linearAllocator->cleanup();
    m_generators.clear();
}

void InternalPrimitiveRenderer::prepareBuffers(IGraphicsDeviceContext* context, int vertexCount, int indexCount)
{
    size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
    if (!m_vertexBuffer || m_vertexBuffer->getBytesSize() < vertexBufferSize)
    {
        m_vertexBuffer = context->createVertexBuffer(GraphicsResourceUsage::Dynamic, vertexBufferSize, nullptr);
    }

    size_t indexBufferSize = sizeof(uint16_t) * indexCount;
    if (!m_indexBuffer || m_indexBuffer->getBytesSize() < indexCount)
    {
        m_indexBuffer = context->createIndexBuffer(GraphicsResourceUsage::Dynamic, IndexBufferFormat::UInt16, indexCount);
    }
}

//==============================================================================
// PrimitiveRenderFeature

PrimitiveRenderFeature::PrimitiveRenderFeature()
	: m_manager(nullptr)
{
}

PrimitiveRenderFeature::~PrimitiveRenderFeature()
{
}

void PrimitiveRenderFeature::init(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
    m_internal = makeRef<InternalPrimitiveRenderer>();
    m_internal->init(manager);
}

//void PrimitiveRenderFeature::drawMeshGenerater(const MeshGenerater* generator)
//{
//    if (m_lastPrimitiveType.hasValue() && m_lastPrimitiveType != generator->primitiveType()) {
//        flush(m_manager->graphicsManager()->graphicsContext());
//    }
//        
//    m_lastPrimitiveType = generator->primitiveType();
//
//
//
//    GraphicsManager* manager = m_manager->graphicsManager();
//    LN_ENQUEUE_RENDER_COMMAND_2(
//        PrimitiveRenderFeature_drawMeshGenerater, manager,
//        InternalPrimitiveRenderer*, m_internal,
//        InternalSpriteRenderer::State, m_state,
//        {
//            m_internal->setState(m_state);
//        });
//}

//void PrimitiveRenderFeature::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
//{
//    GraphicsManager* manager = m_manager->graphicsManager();
//    LN_ENQUEUE_RENDER_COMMAND_2(
//        SpriteRenderFeature_setState, manager,
//        InternalSpriteRenderer*, m_internal,
//        InternalSpriteRenderer::State, m_state,
//        {
//            m_internal->setState(m_state);
//        });
//
//    if (m_lastPrimitiveType.hasValue() && m_lastPrimitiveType != PrimitiveType::LineList) {
//        flush(m_manager->graphicsManager()->graphicsContext());
//    }
//
//    m_lastPrimitiveType = PrimitiveType::LineList;
//}

void PrimitiveRenderFeature::flush(GraphicsContext* context)
{
    GraphicsManager* manager = m_manager->graphicsManager();
    IGraphicsDeviceContext* deviceContext = context->commitState();
    LN_ENQUEUE_RENDER_COMMAND_2(
        PrimitiveRenderFeature_flush, manager,
        InternalPrimitiveRenderer*, m_internal,
        IGraphicsDeviceContext*, deviceContext,
        {
            m_internal->flush(deviceContext);
        });

    m_lastPrimitiveType = nullptr;
}

} // namespace detail
} // namespace ln

