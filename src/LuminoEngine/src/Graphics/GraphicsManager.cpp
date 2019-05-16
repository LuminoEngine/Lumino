
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsResource.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include "GraphicsManager.hpp"
#include "RenderTargetTextureCache.hpp"
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
	case TextureFormat::RGBA8:
		return PixelFormat::RGBA8;
    case TextureFormat::RGB8:
		return PixelFormat::RGB8;
	case TextureFormat::RGBA16F:
		return PixelFormat::Unknown;
	case TextureFormat::RGBA32F:
		return PixelFormat::RGBA32F;
	case TextureFormat::R16F:
		return PixelFormat::Unknown;
	case TextureFormat::R32F:
		return PixelFormat::Unknown;
	case TextureFormat::R32U:
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
	case PixelFormat::A8:
		return TextureFormat::Unknown;
	case PixelFormat::RGBA8:
		return TextureFormat::RGBA8;
    case PixelFormat::RGB8:
        return TextureFormat::RGB8;
	case PixelFormat::RGBA32F:
		return TextureFormat::RGBA32F;
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
    case TextureFormat::RGBA8:
        return 4;
    case TextureFormat::RGB8:
        return 3;
    case TextureFormat::RGBA16F:
        return 8;
    case TextureFormat::RGBA32F:
        return 16;
    case TextureFormat::R16F:
        return 2;
    case TextureFormat::R32F:
        return 4;
    case TextureFormat::R32U:
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

	m_linearAllocatorPageManager = makeRef<LinearAllocatorPageManager>();

	m_graphicsContext = makeObject<GraphicsContext>(m_deviceContext->getGraphicsContext());


	m_inFlightRenderingCommandList = makeRef<RenderingCommandList>(linearAllocatorPageManager());

	m_renderTargetTextureCacheManager = makeRef<RenderTargetTextureCacheManager>();
	m_depthBufferCacheManager = makeRef<DepthBufferCacheManager>();
	m_frameBufferCache = makeRef<detail::FrameBufferCache>(m_renderTargetTextureCacheManager, m_depthBufferCacheManager);

	// default objects
	{
        m_blackTexture = makeObject<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_blackTexture->clear(Color::Black);

        m_whiteTexture = makeObject<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_whiteTexture->clear(Color::White);

		m_defaultSamplerState = makeObject<SamplerState>();
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

	m_frameBufferCache = nullptr;
	m_depthBufferCacheManager = nullptr;
	m_renderTargetTextureCacheManager = nullptr;

	m_graphicsContext->dispose();
	m_deviceContext->dispose();
}

void GraphicsManager::enterRendering()
{
    if (m_deviceContext) {
        if (renderingType() == RenderingType::Threaded) {
            LN_NOTIMPLEMENTED();
        }
        else {
            m_deviceContext->enterMainThread();
            m_deviceContext->enterRenderState();
        }
    }
}

void GraphicsManager::leaveRendering()
{
    if (m_deviceContext) {
        if (renderingType() == RenderingType::Threaded) {
            LN_NOTIMPLEMENTED();
        }
        else {
            m_deviceContext->leaveRenderState();
            m_deviceContext->leaveMainThread();
        }
    }
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
	OpenGLDevice::Settings openglSettings;
	openglSettings.mainWindow = settings.mainWindow;
	auto ctx = makeRef<OpenGLDevice>();
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
	VulkanDevice::Settings dcSettings;
    dcSettings.mainWindow = settings.mainWindow;
	//dcSettings.debugEnabled = true;
	auto ctx = makeRef<VulkanDevice>();
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

Ref<RenderingCommandList> GraphicsManager::submitCommandList(RenderingCommandList* commandList)
{
	if (LN_REQUIRE(commandList)) return nullptr;
	commandList->clear();
	return m_inFlightRenderingCommandList;
}

} // namespace detail
} // namespace ln

