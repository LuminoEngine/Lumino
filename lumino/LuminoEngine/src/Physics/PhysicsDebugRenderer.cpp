
#include "Internal.hpp"
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoGraphics/RHI/VertexLayout.hpp>
#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include "PhysicsDebugRenderer.hpp"

namespace ln {
namespace detail {

//==============================================================================
// PhysicsDebugRenderer

void PhysicsDebugRenderer::init()
{
	m_linesBuffer = makeObject<VertexBuffer>(sizeof(Vertex) * MaxVertexCount, GraphicsResourceUsage::Dynamic);
	m_trianglesBuffer = makeObject<VertexBuffer>(sizeof(Vertex) * MaxVertexCount, GraphicsResourceUsage::Dynamic);
	m_linesVertexCount = 0;
	m_trianglesVertexCount = 0;
}

void PhysicsDebugRenderer::render(RenderingContext* context)
{
	context->pushState();
	context->setBlendMode(BlendMode::Alpha);
	context->setShadingModel(ShadingModel::Unlit);

	context->drawPrimitive(
		detail::RenderingManager::instance()->standardVertexDeclaration(),
		m_trianglesBuffer,
		PrimitiveTopology::TriangleList,
		0, m_trianglesVertexCount / 3);

	context->drawPrimitive(
		detail::RenderingManager::instance()->standardVertexDeclaration(),
		m_linesBuffer,
		PrimitiveTopology::LineList,
		0, m_linesVertexCount / 2);

	context->popState();    // TODO: scoped

	m_linesVertexCount = 0;
	m_trianglesVertexCount = 0;
}

void PhysicsDebugRenderer::addLineVertex(const Vector3& v, const Color& c)
{
	if (m_linesVertexCount < MaxVertexCount)
	{
		Vertex* buf = (Vertex*)m_linesBuffer->writableData();
		buf[m_linesVertexCount].set(v, -Vector3::UnitZ, Vector2::Zero, c);
		m_linesVertexCount++;
	}
}

void PhysicsDebugRenderer::addTriangleVertex(const Vector3& v, const Color& c)
{
	if (m_trianglesVertexCount < MaxVertexCount)
	{
		Vertex* buf = (Vertex*)m_trianglesBuffer->writableData();
		buf[m_trianglesVertexCount].set(v, -Vector3::UnitZ, Vector2::Zero, c);
		m_trianglesVertexCount++;
	}
}

void PhysicsDebugRenderer::drawLine(const Vector3& p1, const Vector3& p2, const Color& color1, const Color& color2)
{
	addLineVertex(p1, color1);
	addLineVertex(p2, color2);
}

} // namespace detail
} // namespace ln

