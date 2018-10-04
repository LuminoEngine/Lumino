
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "MeshRenderFeature.hpp"
#include "RenderingManager.hpp"

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

void MeshRenderFeature::initialize(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
}

//void MeshRenderFeature::drawMesh(MeshResource* mesh, int sectionIndex)
//{
//	mesh->sec
//}

//void MeshRenderFeature::drawMesh(MeshResource* mesh, int startIndex, int primitiveCount, PrimitiveType primitiveType)
void MeshRenderFeature::drawMesh(MeshResource* mesh, int sectionIndex)
{
	if (LN_REQUIRE(mesh != nullptr)) return;
	auto* _this = this;

	MeshSection section;
	VertexDeclaration* decls;
	VertexBuffer* vb[MaxVertexStreams] = {};
	int vbCount;
	IndexBuffer* ib;
	mesh->commitRenderData(sectionIndex, &section, &decls, vb, &vbCount, &ib);

	DrawMeshCommandData data;
	data.vertexDeclaration = GraphicsResourceHelper::resolveRHIObject<IVertexDeclaration>(decls);
	for (int i = 0; i < vbCount; ++i)
	{
		data.vertexBuffers[i] = GraphicsResourceHelper::resolveRHIObject<IVertexBuffer>(vb[i]);
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = ib->resolveRHIObject();
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = PrimitiveType::TriangleList;

	if (LN_REQUIRE(data.vertexBuffers[0])) return;

	LN_ENQUEUE_RENDER_COMMAND_2(
		MeshRenderFeature_drawMesh, m_manager->graphicsManager(),
		MeshRenderFeature*, _this,
		DrawMeshCommandData, data,
		{
			_this->drawMeshImplOnRenderThread(data);
		});
}

void MeshRenderFeature::flush()
{
}

void MeshRenderFeature::drawMeshImplOnRenderThread(const DrawMeshCommandData& data)
{
	IGraphicsDeviceContext* device = m_manager->graphicsManager()->deviceContext();

	device->setVertexDeclaration(data.vertexDeclaration);
	for (int i = 0; i < data.vertexBuffersCount; ++i) {
		device->setVertexBuffer(i, data.vertexBuffers[i]);
	}
	device->setIndexBuffer(data.indexBuffer);
	device->drawPrimitiveIndexed(data.primitiveType, data.startIndex, data.primitiveCount);
}

} // namespace detail
} // namespace ln

