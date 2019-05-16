
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
	data.vertexDeclaration = GraphicsResourceInternal::resolveRHIObject<IVertexDeclaration>(decls, nullptr);
	for (int i = 0; i < vbCount; ++i)
	{
		data.vertexBuffers[i] = GraphicsResourceInternal::resolveRHIObject<IVertexBuffer>(vb[i], nullptr);
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IIndexBuffer>(ib, nullptr);
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = PrimitiveTopology::TriangleList;

	if (LN_REQUIRE(data.vertexBuffers[0])) return;

	IGraphicsContext* c = GraphicsContextInternal::commitState(context);
	LN_ENQUEUE_RENDER_COMMAND_3(
		MeshRenderFeature_drawMesh, context,
		MeshRenderFeature*, _this,
        IGraphicsContext*, c,
		DrawMeshCommandData, data,
		{
			_this->drawMeshImplOnRenderThread(c, data);
		});
}

void MeshRenderFeature::flush(GraphicsContext* context)
{
}

void MeshRenderFeature::drawMeshImplOnRenderThread(IGraphicsContext* context, const DrawMeshCommandData& data)
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

} // namespace detail
} // namespace ln

