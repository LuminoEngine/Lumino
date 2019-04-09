
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsResource.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include "GraphicsManager.hpp"
#include "OpenGLDeviceContext.hpp"
#ifdef LN_USE_VULKAN
#include "VulkanDeviceContext.hpp"
#endif
#include "../Engine/LinearAllocator.hpp"

namespace ln {

//==============================================================================
// GraphicsHelper

size_t GraphicsHelper::getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex)
{
    int size = 0;
    for (int i = 0; i < elementsCount; ++i)
    {
        if (vertexElements[i].StreamIndex == streamIndex) {
            size += getVertexElementTypeSize(vertexElements[i].Type);
        }
    }
    return size;
}

size_t GraphicsHelper::getVertexElementTypeSize(VertexElementType type)
{
    switch (type)
    {
    case VertexElementType::Float1:	return sizeof(float);
    case VertexElementType::Float2:	return sizeof(float) * 2;
    case VertexElementType::Float3:	return sizeof(float) * 3;
    case VertexElementType::Float4:	return sizeof(float) * 4;
    case VertexElementType::Ubyte4:	return sizeof(unsigned char) * 4;
    case VertexElementType::Color4:	return sizeof(unsigned char) * 4;
    case VertexElementType::Short2:	return sizeof(short) * 2;
    case VertexElementType::Short4:	return sizeof(short) * 4;
    default:
        LN_UNREACHABLE();
        break;
    }
    return 0;
}

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

size_t GraphicsHelper::getPixelSize(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::Unknown:
        return 0;
    case TextureFormat::RGBA32:
        return 4;
    case TextureFormat::RGB24:
        return 3;
    case TextureFormat::R16G16B16A16Float:
        return 8;
    case TextureFormat::R32G32B32A32Float:
        return 16;
    case TextureFormat::R16Float:
        return 2;
    case TextureFormat::R32Float:
        return 4;
    case TextureFormat::R32UInt:
        return 4;
    default:
        LN_UNREACHABLE();
        return 0;
    }
}


namespace detail {

//==============================================================================
// GraphicsManager

GraphicsManager::GraphicsManager()
	: m_linearAllocatorPageManager()
{
}

void GraphicsManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "GraphicsManager Initialization started.";

	// Create device context
	{
		if (settings.graphicsAPI == GraphicsAPI::Vulkan) {
			createVulkanContext(settings);
		}

		if (!m_deviceContext) {
			createOpenGLContext(settings);
		}

		// Default
		if (!m_deviceContext) {
			createOpenGLContext(settings);
		}
	}

    m_deviceContext->refreshCaps();

    {
        auto& triple = m_deviceContext->caps().requestedShaderTriple;
        LN_LOG_INFO << "requestedShaderTriple:" << triple.target << "-" << triple.version << "-" << triple.option;
    }


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

    LN_LOG_DEBUG << "GraphicsManager Initialization ended.";
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

void GraphicsManager::createOpenGLContext(const Settings& settings)
{
	OpenGLDeviceContext::Settings openglSettings;
	openglSettings.mainWindow = settings.mainWindow;
	auto ctx = makeRef<OpenGLDeviceContext>();
	ctx->init(openglSettings);
	m_deviceContext = ctx;
}

void GraphicsManager::createVulkanContext(const Settings& settings)
{
#ifdef LN_USE_VULKAN
#if 0
	VulkanSampleDeviceContext::Settings dcSettings;
	dcSettings.mainWindow = settings.mainWindow;
	auto ctx = makeRef<VulkanSampleDeviceContext>();
	ctx->init(dcSettings);
	m_deviceContext = ctx;
#else
	VulkanDeviceContext::Settings dcSettings;
    dcSettings.mainWindow = settings.mainWindow;
	//dcSettings.debugEnabled = true;
	auto ctx = makeRef<VulkanDeviceContext>();
	bool driverSupported = false;
	if (!ctx->init(dcSettings, &driverSupported)) {
		if (!driverSupported) {
			// ドライバが Vulkan をサポートしていない。継続する。
		}
		else {
			LN_ERROR("Vulkan driver initialization failed.");
			return;
		}
	}
	else {
		m_deviceContext = ctx;
	}
#endif
#endif
}

} // namespace detail
} // namespace ln

