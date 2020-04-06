
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "RenderingManager.hpp"
#include "MeshRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshRenderFeature

MeshRenderFeature::MeshRenderFeature()
{
}

void MeshRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
}

RequestBatchResult MeshRenderFeature::drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, MeshResource* mesh, int sectionIndex)
{
	if (LN_REQUIRE(mesh != nullptr)) return RequestBatchResult::Staging;

	auto result = attemptSubmitBatch(context, batchList, true);

	MeshSection section;
	VertexLayout* layout;
	VertexBuffer* vb[MaxVertexStreams] = {};
	int vbCount;
	IndexBuffer* ib;
	mesh->commitRenderData(sectionIndex, &section, &layout, vb, &vbCount, &ib);

	DrawMeshData data;
	data.vertexLayout = layout;
	for (int i = 0; i < vbCount; ++i) {
		data.vertexBuffers[i] = vb[i];
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = ib;
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = PrimitiveTopology::TriangleList;
	data.instanceCount = 0;

	if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
	if (data.primitiveCount <= 0) return RequestBatchResult::Staging;

	m_meshes.push_back(std::move(data));
	m_batchData.count++;

	return result;
}

RequestBatchResult MeshRenderFeature::drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, Mesh* mesh, int sectionIndex)
{
    if (LN_REQUIRE(mesh != nullptr)) return RequestBatchResult::Staging;

	auto result = attemptSubmitBatch(context, batchList, true);

    MeshSection2 section;
    VertexLayout* layout;
    std::array<VertexBuffer*, MaxVertexStreams> vb = {};
    int vbCount;
    IndexBuffer* ib;
    mesh->commitRenderData(sectionIndex, &section, &layout, &vb, &vbCount, &ib);

    DrawMeshData data;
    data.vertexLayout = layout;
    for (int i = 0; i < vbCount; ++i) {
        data.vertexBuffers[i] = vb[i];
    }
    data.vertexBuffersCount = vbCount;
    data.indexBuffer = ib;
    data.startIndex = section.startIndex;
    data.primitiveCount = section.primitiveCount;
    data.primitiveType = section.topology;
	data.instanceCount = 0;

    if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
    if (data.primitiveCount <= 0) return RequestBatchResult::Staging;

    m_meshes.push_back(std::move(data));
    m_batchData.count++;

	return result;
}

RequestBatchResult MeshRenderFeature::drawMeshInstanced(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, InstancedMeshList* list)
{
	if (LN_REQUIRE(list != nullptr)) return RequestBatchResult::Staging;


	auto result = attemptSubmitBatch(context, batchList, true);

	MeshSection2 section;
	VertexLayout* layout;
	std::array<VertexBuffer*, MaxVertexStreams> vb = {};
	int vbCount;
	IndexBuffer* ib;
	list->commitRenderData(&section, &layout, &vb, &vbCount, &ib);

	DrawMeshData data;
	data.vertexLayout = layout;
	for (int i = 0; i < vbCount; ++i) {
		data.vertexBuffers[i] = vb[i];
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = ib;
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = section.topology;
	data.instanceCount = list->instanceCount();

	if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
	if (data.primitiveCount <= 0) return RequestBatchResult::Staging;

	m_meshes.push_back(std::move(data));
	m_batchData.count++;
	m_batchData.instanced = true;

	return result;
}

RequestBatchResult MeshRenderFeature::attemptSubmitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList, bool instanced)
{
	if (m_batchData.count == 0) {
		return RequestBatchResult::Staging;	// 初回なので Submit する必要なし
	}
	//else if (batchList->lastBatch()->instancing != instanced) {
	else if (m_batchData.instanced != instanced) {
		submitBatch(context, batchList);
		return RequestBatchResult::Submitted;
	}
	else {
		return RequestBatchResult::Staging;
	}
}

void MeshRenderFeature::beginRendering()
{
	m_meshes.clear();
	m_batchData.offset = 0;
	m_batchData.count = 0;
	m_batchData.instanced = false;
}

void MeshRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;
	batch->instancing = m_batchData.instanced;

	m_batchData.offset = m_batchData.offset + m_batchData.count;
	m_batchData.count = 0;
	m_batchData.instanced = false;
}

void MeshRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);

	for (int i = 0; i < localBatch->data.count; i++) {
		auto& data = m_meshes[localBatch->data.offset + i];

		context->setVertexLayout(data.vertexLayout);
		for (int i = 0; i < data.vertexBuffersCount; ++i) {
			context->setVertexBuffer(i, data.vertexBuffers[i]);
		}
		context->setPrimitiveTopology(data.primitiveType);  // TODO: これは RenderingContext がわでやるようになっている
		if (data.indexBuffer) {
			context->setIndexBuffer(data.indexBuffer);
			context->drawPrimitiveIndexed(data.startIndex, data.primitiveCount, data.instanceCount);
		}
		else {
			context->drawPrimitive(data.startIndex, data.primitiveCount);
		}
	}
}

} // namespace detail
} // namespace ln

