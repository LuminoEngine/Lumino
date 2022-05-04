#if 0
#include "Internal.hpp"
#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/RHI/IndexBuffer.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include "PrimitiveRenderFeature.hpp"

namespace ln {
namespace detail {

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

RequestBatchResult MeshGeneraterRenderFeature::drawMeshGenerater(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	const MeshGenerater* generator)
{
	// TODO: Batching
	Batch* batch = batchList->addNewBatch<Batch>(this, batchState);
	batch->data.indexOffset = m_indexCount;
	batch->data.indexCount = 0;


	// Verify
	if (batch->data.indexCount == 0) {
		// TODO: これ RLIBatchState が toporogy 持ってるから、そっち使うか検討した方がいいかも
		batch->data.topology = generator->primitiveType();
	}
	else {
		// TODO: RequestBatchResult::submitted
		if (LN_REQUIRE(batch->data.topology == generator->primitiveType())) return RequestBatchResult::Staging;
	}

	MeshGenerater* gen = generator->clone(m_linearAllocator);
	m_generators.add(gen);


	batch->data.indexCount = gen->indexCount();
	m_indexCount += batch->data.indexCount;

	return RequestBatchResult::Staging;
}

void MeshGeneraterRenderFeature::beginRendering()
{
    resetBatchData();
}

void MeshGeneraterRenderFeature::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList)
{
#ifdef LN_RLI_BATCH
	LN_UNREACHABLE();
#else
	// add Batch
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	// next Batch
	m_batchData.topology = m_batchData.topology;
	m_batchData.indexOffset = m_batchData.indexOffset + m_batchData.indexCount;
	m_batchData.indexCount = 0;
#endif
}

void MeshGeneraterRenderFeature::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
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
        Vertex* vertexBuffer = (Vertex*)m_vertexBuffer->writableData(0, vertexCount * sizeof(Vertex));
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
	m_indexCount = 0;
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
}

RequestBatchResult PrimitiveRenderFeature::drawPrimitive(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	VertexLayout* vertexDeclaration,
	VertexBuffer* vertexBuffer,
	int startVertex,
	int primitiveCount)
{
	// TODO: Batching
	Batch* batch = batchList->addNewBatch<Batch>(this, batchState);
	batch->data.offset = 0;
	batch->data.count = 0;

	m_primitives.add({ vertexDeclaration, vertexBuffer, startVertex, primitiveCount });
	batch->data.count++;
	return RequestBatchResult::Staging;
}

void PrimitiveRenderFeature::beginRendering()
{
	m_primitives.clear();
}

void PrimitiveRenderFeature::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList)
{
#ifdef LN_RLI_BATCH
	LN_UNREACHABLE();
#else
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.offset = m_batchData.offset + m_batchData.count;
	m_batchData.count = 0;
#endif
}

void PrimitiveRenderFeature::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
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

#endif
