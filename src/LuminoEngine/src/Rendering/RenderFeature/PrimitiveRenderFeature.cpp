
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../../Graphics/RHIs/GraphicsDeviceContext.hpp"
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
    m_vertexDeclaration = m_manager->standardVertexDeclarationRHI();
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

void InternalPrimitiveRenderer::flush(ICommandList* context)
{
    if (m_generators.isEmpty()) return;

    // Prepare buffers
    int vertexCount = 0;
    int indexCount = 0;
    for (MeshGenerater* gen : m_generators) {
        vertexCount += gen->vertexCount();
        indexCount += gen->indexCount();
    }
    prepareBuffers(m_manager->graphicsManager()->deviceContext(), vertexCount, indexCount);

    // Create Vertex and Index buffers
    Vertex* vertexBuffer = (Vertex*)context->map(m_vertexBuffer, 0, m_vertexBuffer->memorySize());
    uint16_t* indexBuffer = (uint16_t*)context->map(m_indexBuffer, 0, m_indexBuffer->getBytesSize());
    MeshGeneraterBuffer buffer(nullptr);
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    for (MeshGenerater* gen : m_generators) {
        buffer.setBuffer(vertexBuffer + vertexOffset, indexBuffer + indexOffset, IndexBufferFormat::UInt16, vertexOffset);
        buffer.generate(gen);
        //gen->generate(&buffer);
        vertexOffset += gen->vertexCount();
        indexOffset += gen->indexCount();
    }
    context->unmap(m_vertexBuffer);
    context->unmap(m_indexBuffer);

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
    context->drawPrimitiveIndexed(0, primitiveCount, 0, 0);

    for (MeshGenerater* gen : m_generators) {
        gen->~MeshGenerater();
    }

    m_linearAllocator->cleanup();
    m_generators.clear();
}

void InternalPrimitiveRenderer::prepareBuffers(IGraphicsDevice* context, int vertexCount, int indexCount)
{
    size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
    if (!m_vertexBuffer || m_vertexBuffer->memorySize() < vertexBufferSize)
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
// MeshGeneraterRenderFeature

MeshGeneraterRenderFeature::MeshGeneraterRenderFeature()
	: m_manager(nullptr)
{
}

MeshGeneraterRenderFeature::~MeshGeneraterRenderFeature()
{
}

void MeshGeneraterRenderFeature::init(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
	m_linearAllocator = makeRef<LinearAllocator>(m_manager->graphicsManager()->linearAllocatorPageManager());
	prepareBuffers(512, 512 * 3);
	m_vertexLayout = m_manager->standardVertexDeclaration();
	resetBatchData();
}

RequestBatchResult MeshGeneraterRenderFeature::drawMeshGenerater(const MeshGenerater* generator)
{
	// Verify
	if (m_batchData.indexCount == 0) {
		m_batchData.topology = generator->primitiveType();
	}
	else {
		// TODO: RequestBatchResult::submitted
		if (LN_REQUIRE(m_batchData.topology == generator->primitiveType())) return RequestBatchResult::Staging;
	}

	MeshGenerater* gen = generator->clone(m_linearAllocator);
	m_generators.add(gen);

	m_batchData.indexCount += gen->indexCount();

	return RequestBatchResult::Staging;
}

void MeshGeneraterRenderFeature::beginRendering()
{
    resetBatchData();
}

void MeshGeneraterRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	// add Batch
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	// next Batch
	m_batchData.topology = m_batchData.topology;
	m_batchData.indexOffset = m_batchData.indexOffset + m_batchData.indexCount;
	m_batchData.indexCount = 0;
}

void MeshGeneraterRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	if (m_generators.isEmpty()) return;
    auto localBatch = static_cast<Batch*>(batch);

    // TODO: ↓ SceneRenderer から、一連の Batch 描画の開始タイミングを教えてもらってそこでやるほうがいいかも
    if (localBatch->data.indexOffset == 0) {
        // Prepare buffers
        int vertexCount = 0;
        int indexCount = 0;
        for (MeshGenerater* gen : m_generators) {
            vertexCount += gen->vertexCount();
            indexCount += gen->indexCount();
        }
        prepareBuffers(vertexCount, indexCount);

        // Create Vertex and Index buffers
        Vertex* vertexBuffer = (Vertex*)m_vertexBuffer->map(MapMode::Write);
        uint16_t* indexBuffer = (uint16_t*)m_indexBuffer->map(MapMode::Write);
        MeshGeneraterBuffer buffer(m_linearAllocator);
        size_t vertexOffset = 0;
        size_t indexOffset = 0;
        for (MeshGenerater* gen : m_generators) {
            buffer.setBuffer(vertexBuffer + vertexOffset, indexBuffer + indexOffset, IndexBufferFormat::UInt16, vertexOffset);
            buffer.generate(gen);
            vertexOffset += gen->vertexCount();
            indexOffset += gen->indexCount();
        }
        //context->unmap(m_vertexBuffer);
        //context->unmap(m_indexBuffer);


    }

    int primitiveCount = 0;
    switch (localBatch->data.topology)
    {
    case ln::PrimitiveTopology::TriangleList:
        primitiveCount = localBatch->data.indexCount / 3;
        break;
    case ln::PrimitiveTopology::TriangleStrip:
        LN_NOTIMPLEMENTED();
        break;
    case ln::PrimitiveTopology::TriangleFan:
        primitiveCount = localBatch->data.indexCount - 2;
        break;
    case ln::PrimitiveTopology::LineList:
        primitiveCount = localBatch->data.indexCount / 2;
        break;
    case ln::PrimitiveTopology::LineStrip:
        primitiveCount = localBatch->data.indexCount - 1;
        break;
    case ln::PrimitiveTopology::PointList:
        LN_NOTIMPLEMENTED();
        break;
    default:
        LN_UNREACHABLE();
        break;
    }

	// Render
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.indexOffset, primitiveCount);
}

void MeshGeneraterRenderFeature::resetBatchData()
{
	m_generators.clear();
	m_linearAllocator->cleanup();
	m_batchData.indexOffset = 0;
	m_batchData.indexCount = 0;
}

void MeshGeneraterRenderFeature::prepareBuffers(int vertexCount, int indexCount)
{
	if (vertexCount > UINT16_MAX) {
		LN_NOTIMPLEMENTED();
		return;
	}

	size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
	if (!m_vertexBuffer || m_vertexBuffer->size() < vertexBufferSize) {
		m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferSize, GraphicsResourceUsage::Dynamic);
	}

	size_t indexBufferSize = sizeof(uint16_t) * indexCount;
	if (!m_indexBuffer || m_indexBuffer->bytesSize() < indexCount)
	{
		m_indexBuffer = makeObject<IndexBuffer>(vertexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
	}
}

//==============================================================================
// PrimitiveRenderFeature

PrimitiveRenderFeature::PrimitiveRenderFeature()
{
}

void PrimitiveRenderFeature::init()
{
	m_primitives.reserve(16);
	m_batchData.offset = 0;
	m_batchData.count = 0;
}

RequestBatchResult PrimitiveRenderFeature::drawPrimitive(detail::RenderFeatureBatchList* batchList, VertexLayout* vertexDeclaration, VertexBuffer* vertexBuffer, int startVertex, int primitiveCount)
{
	m_primitives.add({ vertexDeclaration, vertexBuffer, startVertex, primitiveCount });
	m_batchData.count++;
	return RequestBatchResult::Staging;
}

void PrimitiveRenderFeature::beginRendering()
{
	m_primitives.clear();
	m_batchData.offset = 0;
	m_batchData.count = 0;
}

void PrimitiveRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.offset = m_batchData.offset + m_batchData.count;
	m_batchData.count = 0;
}

void PrimitiveRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);

	for (int i = 0; i < localBatch->data.count; i++) {
		auto& data = m_primitives[localBatch->data.offset + i];
		context->setVertexLayout(data.vertexLayout);
		context->setVertexBuffer(0, data.vertexBuffer);
		context->drawPrimitive(data.startVertex, data.primitiveCount);
	}
}

} // namespace detail
} // namespace ln

