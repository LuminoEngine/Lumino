
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsResource.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include "GraphicsManager.hpp"
#include "OpenGLDeviceContext.hpp"
#include "../Engine/LinearAllocator.hpp"

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
    case TextureFormat::RGB24:
		return PixelFormat::RGB24;
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
    case PixelFormat::RGB24:
        return TextureFormat::RGB24;
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
	ctx->refreshCaps();
	m_deviceContext = ctx;

	m_graphicsContext = newObject<GraphicsContext>(m_deviceContext);

	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();

	m_primaryRenderingCommandList = makeRef<RenderingCommandList>(linearAllocatorPageManager());

	if (renderingType() == RenderingType::Threaded) {
		LN_NOTIMPLEMENTED();
	}
	else {
		m_deviceContext->enterMainThread();
		m_deviceContext->enterRenderState();
	}

	// default objects
	{
        m_blackTexture = newObject<Texture2D>(32, 32, TextureFormat::RGBA32, false, GraphicsResourceUsage::Static);
        m_blackTexture->clear(Color::Black);

        m_whiteTexture = newObject<Texture2D>(32, 32, TextureFormat::RGBA32, false, GraphicsResourceUsage::Static);
        m_whiteTexture->clear(Color::White);

		m_defaultSamplerState = newObject<SamplerState>();
		m_defaultSamplerState->setFrozen(true);
	}
}

void GraphicsManager::dispose()
{
	// default objects
	{
		m_defaultSamplerState.reset();
	}

	List<GraphicsResource*> removeList = m_graphicsResources;
	m_graphicsResources.clear();
	for (GraphicsResource* resource : removeList) {
		resource->dispose();
	}

	m_deviceContext->leaveRenderState();
	m_deviceContext->leaveMainThread();

	m_graphicsContext->dispose();
	m_deviceContext->dispose();
}

void GraphicsManager::addGraphicsResource(GraphicsResource* resource)
{
	m_graphicsResources.add(resource);
}

void GraphicsManager::removeGraphicsResource(GraphicsResource* resource)
{
	m_graphicsResources.remove(resource);
}

} // namespace detail
} // namespace ln

