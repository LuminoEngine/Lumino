
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexDeclaration.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "FrameRectRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// InternalFrameRectRenderer

InternalFrameRectRenderer::InternalFrameRectRenderer()
	: m_manager(nullptr)
	, m_vertexDeclaration(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

void InternalFrameRectRenderer::init(RenderingManager* manager)
{
    m_manager = manager;
	m_vertexDeclaration = manager->standardVertexDeclaration()->resolveRHIObject();

    prepareBuffers(512);
}

// TODO: TextRenderer と全く同じなので共通化していいかも
void InternalFrameRectRenderer::prepareBuffers(int spriteCount)
{
	IGraphicsDeviceContext* context = m_manager->graphicsManager()->deviceContext();

	if (m_buffersReservedSpriteCount < spriteCount)
	{
		size_t vertexCount = spriteCount * 4;
		if (LN_ENSURE(vertexCount < 0xFFFF)) {
			return;
		}

		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
		m_vertexBuffer = context->createVertexBuffer(GraphicsResourceUsage::Dynamic, vertexBufferSize, nullptr);

		size_t indexBufferSize = spriteCount * 6;
		std::vector<size_t> indexBuf(sizeof(uint16_t) * indexBufferSize, false);
		uint16_t* ib = (uint16_t*)indexBuf.data();
		int idx = 0;
		int i2 = 0;
		for (int i = 0; i < spriteCount; ++i)
		{
			i2 = i * 6;
			idx = i * 4;
			ib[i2 + 0] = idx;
			ib[i2 + 1] = idx + 1;
			ib[i2 + 2] = idx + 2;
			ib[i2 + 3] = idx + 2;
			ib[i2 + 4] = idx + 1;
			ib[i2 + 5] = idx + 3;
		}
		m_indexBuffer = context->createIndexBuffer(
			GraphicsResourceUsage::Dynamic, IndexBufferFormat::UInt16,
			spriteCount * 6, ib);

		m_buffersReservedSpriteCount = spriteCount;
	}
}

void InternalFrameRectRenderer::flush(IGraphicsDeviceContext* context)
{
	context->setVertexDeclaration(m_vertexDeclaration);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	context->drawPrimitiveIndexed(0, m_spriteCount * 2);

    m_spriteCount = 0;
}

//==============================================================================
// FrameRectRenderFeature

FrameRectRenderFeature::FrameRectRenderFeature()
	: m_internal(nullptr)
{
}

void FrameRectRenderFeature::init(RenderingManager* manager)
{
    RenderFeature::init();
    m_internal = makeRef<InternalFrameRectRenderer>();
    m_internal->init(manager);
}

void FrameRectRenderFeature::flush(GraphicsContext* context)
{
	flushInternal(context);
}

void FrameRectRenderFeature::flushInternal(GraphicsContext* context)
{
}

} // namespace detail
} // namespace ln

