
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "BlitRenderFeature.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// BlitRenderFeature

BlitRenderFeature::BlitRenderFeature()
	: m_manager(nullptr)
{
}

void BlitRenderFeature::initialize(RenderingManager* manager)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;

    Vertex vertices[4] =
    {
        { Vector3(-1,  1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White },
        { Vector3(1,  1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White },
        { Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White },
        { Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White },
    };
    m_vertexBuffer = m_manager->graphicsManager()->deviceContext()->createVertexBuffer(GraphicsResourceUsage::Static, sizeof(vertices), vertices);
    m_vertexDeclaration = manager->standardVertexDeclaration()->resolveRHIObject();
}

void BlitRenderFeature::blit()
{
    auto* _this = this;

    LN_ENQUEUE_RENDER_COMMAND_1(
        BlitRenderFeature_blit, m_manager->graphicsManager(),
        BlitRenderFeature*, _this,
        {
            _this->blitImplOnRenderThread();
        });
}

void BlitRenderFeature::flush()
{
}

void BlitRenderFeature::blitImplOnRenderThread()
{
    IGraphicsDeviceContext* device = m_manager->graphicsManager()->deviceContext();

    device->setVertexDeclaration(m_vertexDeclaration);
    device->setVertexBuffer(0, m_vertexBuffer);
    device->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
}

} // namespace detail
} // namespace ln

