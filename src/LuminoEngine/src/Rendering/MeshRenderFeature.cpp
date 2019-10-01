
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

#ifdef LN_RENDERING_MIGRATION

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
	auto* _this = this;

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

	if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
	if (data.primitiveCount <= 0) return RequestBatchResult::Staging;

	m_meshes.push_back(std::move(data));
	m_batchData.count++;
	return RequestBatchResult::Staging;
}

void MeshRenderFeature::beginRendering()
{
	m_meshes.clear();
	m_batchData.offset = 0;
	m_batchData.count = 0;
}

void MeshRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.offset = m_batchData.offset + m_batchData.count;
	m_batchData.count = 0;
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
		context->setPrimitiveTopology(data.primitiveType);
		if (data.indexBuffer) {
			context->setIndexBuffer(data.indexBuffer);
			context->drawPrimitiveIndexed(data.startIndex, data.primitiveCount);
		}
		else {
			context->drawPrimitive(data.startIndex, data.primitiveCount);
		}
	}
}

#else
//==============================================================================
// MeshRenderFeature

MeshRenderFeature::MeshRenderFeature()
	: m_manager(nullptr)
{
}

MeshRenderFeature::~MeshRenderFeature()
{
}

void MeshRenderFeature::init(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
}

//void MeshRenderFeature::drawMesh(MeshResource* mesh, int sectionIndex)
//{
//	mesh->sec
//}

//void MeshRenderFeature::drawMesh(MeshResource* mesh, int startIndex, int primitiveCount, PrimitiveType primitiveType)
void MeshRenderFeature::drawMesh(GraphicsContext* context, MeshResource* mesh, int sectionIndex)
{
	if (LN_REQUIRE(mesh != nullptr)) return;
	auto* _this = this;

	MeshSection section;
	VertexLayout* decls;
	VertexBuffer* vb[MaxVertexStreams] = {};
	int vbCount;
	IndexBuffer* ib;
	mesh->commitRenderData(sectionIndex, &section, &decls, vb, &vbCount, &ib);

	DrawMeshCommandData data;
	data.vertexDeclaration = GraphicsResourceInternal::resolveRHIObject<IVertexDeclaration>(context, decls, nullptr);
	for (int i = 0; i < vbCount; ++i)
	{
		data.vertexBuffers[i] = GraphicsResourceInternal::resolveRHIObject<IVertexBuffer>(context, vb[i], nullptr);
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IIndexBuffer>(context, ib, nullptr);
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = PrimitiveTopology::TriangleList;

	if (LN_REQUIRE(data.vertexBuffers[0])) return;

	ICommandList* c = GraphicsContextInternal::commitState(context);
	LN_ENQUEUE_RENDER_COMMAND_3(
		MeshRenderFeature_drawMesh, context,
		MeshRenderFeature*, _this,
		ICommandList*, c,
		DrawMeshCommandData, data,
		{
			_this->drawMeshImplOnRenderThread(c, data);
		});
}

void MeshRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	// TODO:
}

void MeshRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	LN_NOTIMPLEMENTED();
}

void MeshRenderFeature::drawMeshImplOnRenderThread(ICommandList* context, const DrawMeshCommandData& data)
{
	context->setVertexDeclaration(data.vertexDeclaration);
	for (int i = 0; i < data.vertexBuffersCount; ++i) {
		context->setVertexBuffer(i, data.vertexBuffers[i]);
	}
	context->setPrimitiveTopology(data.primitiveType);
    if (data.indexBuffer) {
        context->setIndexBuffer(data.indexBuffer);
        context->drawPrimitiveIndexed(data.startIndex, data.primitiveCount);
    }
    else {
        context->drawPrimitive(data.startIndex, data.primitiveCount);
    }
}
#endif

} // namespace detail
} // namespace ln

