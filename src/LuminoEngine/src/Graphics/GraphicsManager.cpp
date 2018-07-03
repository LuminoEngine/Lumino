
#include "Internal.hpp"
#include <Lumino/Graphics/GraphicsContext.hpp>
#include "GraphicsManager.hpp"
#include "OpenGLDeviceContext.hpp"
#include "LinearAllocator.hpp"
#include "RenderingCommandList.hpp"

namespace ln {

//==============================================================================
// GraphicsHelper

PixelFormat GraphicsHelper::translateToPixelFormat(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::Unknown:
		return PixelFormat::Unknown;
	case TextureFormat::RGBA32:
		return PixelFormat::RGBA32;
	case TextureFormat::RGBX32:
		return PixelFormat::Unknown;
	case TextureFormat::R16G16B16A16Float:
		return PixelFormat::Unknown;
	case TextureFormat::R32G32B32A32Float:
		return PixelFormat::R32G32B32A32Float;
	case TextureFormat::R16Float:
		return PixelFormat::Unknown;
	case TextureFormat::R32Float:
		return PixelFormat::Unknown;
	case TextureFormat::R32UInt:
		return PixelFormat::Unknown;
	default:
		return PixelFormat::Unknown;
	}
}

TextureFormat GraphicsHelper::translateToTextureFormat(PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::Unknown:
		return TextureFormat::Unknown;
	case PixelFormat::A1:
		return TextureFormat::Unknown;
	case PixelFormat::A8:
		return TextureFormat::Unknown;
	case PixelFormat::RGBA32:
		return TextureFormat::RGBA32;
	case PixelFormat::R32G32B32A32Float:
		return TextureFormat::R32G32B32A32Float;
	default:
		return TextureFormat::Unknown;
	}
}



namespace detail {

//==============================================================================
// GraphicsManager

GraphicsManager::GraphicsManager()
	: m_linearAllocatorPageManager()
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

	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();

	m_primaryRenderingCommandList = makeRef<RenderingCommandList>(this);
}

void GraphicsManager::dispose()
{
	m_graphicsContext->dispose();
	m_deviceContext->dispose();
}

} // namespace detail
} // namespace ln

