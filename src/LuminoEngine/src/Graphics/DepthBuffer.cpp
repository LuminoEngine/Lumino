
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/DepthBuffer.hpp>

namespace ln {

//==============================================================================
// DepthBuffer

DepthBuffer::DepthBuffer()
{
}

DepthBuffer::~DepthBuffer()
{
}

void DepthBuffer::init(int width, int height)
{
	GraphicsResource::init();
	m_rhiObject = deviceContext()->createDepthBuffer(width, height);
	m_size.width = width;
	m_size.height = height;
}

void DepthBuffer::onDispose(bool explicitDisposing)
{
	m_rhiObject.reset();
	GraphicsResource::onDispose(explicitDisposing);
}

void DepthBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
	if (!device) {
		m_rhiObject.reset();
	}
}

detail::IDepthBuffer* DepthBuffer::resolveRHIObject()
{
	return m_rhiObject;
}

} // namespace ln
