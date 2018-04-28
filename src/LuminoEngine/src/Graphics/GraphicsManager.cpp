
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsContext.hpp>
#include "GraphicsManager.hpp"
#include "OpenGLDeviceContext.hpp"

namespace ln {
namespace detail {

GraphicsManager::GraphicsManager()
{
}

void GraphicsManager::initialize(const Settings& settings)
{
	OpenGLDeviceContext::Settings openglSettings;
	openglSettings.mainWindow = settings.mainWindow;
	auto ctx = makeRef<OpenGLDeviceContext>();
	ctx->initialize(openglSettings);
	m_deviceContext = ctx;

	m_graphicsContext = newObject<GraphicsContext>(m_deviceContext);
}

void GraphicsManager::dispose()
{
	m_graphicsContext->dispose();
	m_deviceContext->dispose();
}

} // namespace detail
} // namespace ln

