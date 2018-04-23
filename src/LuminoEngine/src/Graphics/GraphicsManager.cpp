
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {

GraphicsManager::GraphicsManager()
{
}

void GraphicsManager::initialize()
{
	m_deviceContext = makeRef<OpenGLDeviceContext>();
	m_deviceContext->initialize();
}

void GraphicsManager::dispose()
{
	m_deviceContext->dispose();
}

} // namespace detail
} // namespace ln

