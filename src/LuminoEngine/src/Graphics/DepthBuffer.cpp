
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

void DepthBuffer::initialize(int width, int height)
{
	GraphicsResource::initialize();
	m_rhiObject = deviceContext()->createDepthBuffer(width, height);
}

void DepthBuffer::dispose()
{
	m_rhiObject.reset();
	GraphicsResource::dispose();
}

void DepthBuffer::onChangeDevice(detail::IGraphicsDeviceContext* device)
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
