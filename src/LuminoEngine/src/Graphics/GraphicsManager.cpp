﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsResource.hpp>
#include <LuminoEngine/Graphics/CommandQueue.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Graphics/GraphicsExtension.hpp>
#include "GraphicsManager.hpp"
#include "RenderTargetTextureCache.hpp"
#include "OpenGL/OpenGLDeviceContext.hpp"
#ifdef LN_USE_VULKAN
#include "Vulkan/VulkanDeviceContext.hpp"
#endif
#include "../Engine/LinearAllocator.hpp"
#include "../Asset/AssetManager.hpp"
#include "SingleFrameAllocator.hpp"

namespace ln {
namespace detail {

void ConstantBufferView::setData(const void* data, size_t size)
{
	//byte_t* d = static_cast<byte_t*>(buffer->map()) + offset;	// TODO: map しないほうが効率いい？SingleFrameのデータなので、フレーム締めるときに unmap でよい
	//memcpy(d, data, size);
	//buffer->unmap();
	byte_t* d = static_cast<byte_t*>(buffer->writableData()) + offset;
	memcpy(d, data, size);
}

void* ConstantBufferView::writableData()
{
	return static_cast<byte_t*>(buffer->writableData()) + offset;
}

} // namespace detail

//==============================================================================
// GraphicsHelper

const Char* GraphicsHelper::CandidateExts_Texture2D[5] = { u".png", u".jpg", u".tga", u".bmp", u".gif" };

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
	case TextureFormat::R32S:
		return PixelFormat::R32S;
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
    case TextureFormat::R32S:
        return 4;
    default:
        LN_UNREACHABLE();
        return 0;
    }
}

size_t GraphicsHelper::getPixelSize(PixelFormat format)
{
	switch (format)
	{
	case PixelFormat::Unknown:
		return 0;
	case PixelFormat::A8:
		return 1;
	case PixelFormat::RGBA8:
		return 4;
	case PixelFormat::RGB8:
		return 3;
	case PixelFormat::RGBA32F:
		return 16;
	case PixelFormat::R32S:
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
	: m_assetManager(nullptr)
	, m_platformManager(nullptr)
    , m_linearAllocatorPageManager()
{
}

void GraphicsManager::init(const Settings& settings)
{
    LN_LOG_DEBUG << "GraphicsManager Initialization started.";

	if (LN_REQUIRE(settings.graphicsAPI != GraphicsAPI::Default)) return;

    m_assetManager = settings.assetManager;
	m_platformManager = settings.platformManager;

	m_texture2DCache.init(64);

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

	//m_graphicsContext = makeObject<GraphicsContext>(renderingType());

	//if (auto queue = m_deviceContext->getGraphicsCommandQueue()) {
	//	m_graphicsQueue = Ref<CommandQueue>(LN_NEW CommandQueue(), false);
	//	m_graphicsQueue->init(queue);
	//}
	//if (auto queue = m_deviceContext->getComputeCommandQueue()) {
	//	m_computeQueue = Ref<CommandQueue>(LN_NEW CommandQueue(), false);
	//	m_computeQueue->init(queue);
	//}


	m_renderingQueue = makeRef<RenderingQueue>();

	m_renderTargetTextureCacheManager = makeRef<RenderTargetTextureCacheManager>();
	m_depthBufferCacheManager = makeRef<DepthBufferCacheManager>();
	m_frameBufferCache = makeRef<detail::FrameBufferCache>(m_renderTargetTextureCacheManager, m_depthBufferCacheManager);
	m_singleFrameUniformBufferAllocatorPageManager = makeRef<SingleFrameUniformBufferAllocatorPageManager>(0x200000);// 2MB

	m_extensions.add(nullptr);	// [0] is dummy

	// default objects
	{
        m_blackTexture = makeObject<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_blackTexture->clear(Color::Black);

        m_whiteTexture = makeObject<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_whiteTexture->clear(Color::White);

		m_defaultNormalMap = makeObject<Texture2D>(32, 32, TextureFormat::RGBA8);
		m_defaultNormalMap->clear(Color(0.0f, 0.0f, 1.0f, 1.0f));

		m_defaultSamplerState = makeObject<SamplerState>();
		m_defaultSamplerState->setFrozen(true);

		m_linearSamplerState = makeObject<SamplerState>(TextureFilterMode::Linear);
		m_linearSamplerState->setFrozen(true);

		m_pointClampSamplerState = makeObject<SamplerState>(TextureFilterMode::Point, TextureAddressMode::Clamp);
		m_pointClampSamplerState->setFrozen(true);

		m_linearClampSamplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);
		m_pointClampSamplerState->setFrozen(true);
	}


    EngineDomain::registerType<Texture2D>();

    LN_LOG_DEBUG << "GraphicsManager Initialization ended.";
}

void GraphicsManager::dispose()
{
	if (m_renderingQueue) {
		m_renderingQueue->dispose();
		m_renderingQueue = nullptr;
	}

	m_texture2DCache.dispose();

	// default objects
	{
		m_linearSamplerState = nullptr;
		m_defaultSamplerState = nullptr;
		m_whiteTexture = nullptr;
		m_blackTexture = nullptr;
	}

	List<IGraphicsResource*> removeList = m_graphicsResources;
	m_graphicsResources.clear();
	for (IGraphicsResource* resource : removeList) {
		resource->onManagerFinalizing();
	}

	if (m_singleFrameUniformBufferAllocatorPageManager) {
		m_singleFrameUniformBufferAllocatorPageManager->clear();
		m_singleFrameUniformBufferAllocatorPageManager = nullptr;
	}

	m_frameBufferCache = nullptr;
	m_depthBufferCacheManager = nullptr;
	m_renderTargetTextureCacheManager = nullptr;

	m_deviceContext->dispose();
}

//void GraphicsManager::enterRendering()
//{
//    if (m_deviceContext) {
//        if (renderingType() == RenderingType::Threaded) {
//            LN_NOTIMPLEMENTED();
//        }
//        else {
//            m_deviceContext->enterRenderState();
//        }
//    }
//}
//
//void GraphicsManager::leaveRendering()
//{
//    if (m_deviceContext) {
//        if (renderingType() == RenderingType::Threaded) {
//            LN_NOTIMPLEMENTED();
//        }
//        else {
//            m_deviceContext->leaveRenderState();
//        }
//    }
//}

void GraphicsManager::addGraphicsResource(IGraphicsResource* resource)
{
	m_graphicsResources.add(resource);
}

void GraphicsManager::removeGraphicsResource(IGraphicsResource* resource)
{
	m_graphicsResources.remove(resource);
}

int GraphicsManager::registerExtension(INativeGraphicsExtension* extension)
{
	if (LN_REQUIRE(extension)) return 0;
	m_extensions.add(extension);
	extension->onLoaded(m_deviceContext->getNativeInterface());
	return m_extensions.size() - 1;
}

void GraphicsManager::unregisterExtension(INativeGraphicsExtension* extension)
{
	if (LN_REQUIRE(extension)) return;
	m_extensions.remove(extension);
	extension->onUnloaded(m_deviceContext->getNativeInterface());
}

Ref<Texture> GraphicsManager::requestTexture(const AssetPath& assetPath)
{
	// TODO: cache
	auto stream = m_assetManager->openStreamFromAssetPath(assetPath);
	if (stream) {
		return makeObject<Texture2D>(stream, TextureFormat::RGBA8);	// TODO: format
	}
	else {
		return nullptr;
	}
}

Ref<Texture2D> GraphicsManager::loadTexture2D(const StringRef& filePath)
{
	m_texture2DCache.collectUnreferenceObjects(false);

	static const std::vector<const Char*> exts = { u".png", u".jpg", u".tga", u".bmp", u".gif" };

#if 1
	return AssetManager::loadObjectWithCacheHelper<Texture2D>(&m_texture2DCache, nullptr, exts, filePath, nullptr);
#else
	auto pathSet = std::make_unique<AssetRequiredPathSet>();
	if (!AssetObject::_resolveAssetRequiredPathSet(filePath, exts, pathSet.get())) {
		return nullptr;
	}

	// finalResourceAssetFilePath から拡張子を除いたものを CacheKey とする
	// > CacheKey はどの Archive に入っているファイルであるかまで区別できるものでなければダメ。
	// > Archive 名と、それを基準とした相対パス(または絶対パス) で表す必要がある。
	// > 拡張子は無くてもOK。.yml でも .png でも、出来上がる Texture2D は同じもの。
	const auto cacheKey = Path(pathSet->finalResourceAssetFilePath.toString()).replaceExtension(u"");

	if (auto obj = m_texture2DCache.findObject(cacheKey)) {
		return obj;
	}

	auto obj = makeObject<Texture2D>();
	obj->m_data = std::move(pathSet);
	obj->reload();

	m_texture2DCache.registerObject(cacheKey, obj);

	return obj;
#endif
}

Ref<Texture2D> GraphicsManager::loadTexture2DFromOnMemoryData(const detail::AssetPath* baseDir, const StringRef& filePath, std::function<Ref<Texture2D>(const AssetRequiredPathSet*)> factory)
{
	static const std::vector<const Char*> exts = { u".png", u".jpg", u".tga", u".bmp", u".gif" };
	return AssetManager::loadObjectWithCacheHelper<Texture2D>(texture2DCache(), baseDir, exts, filePath, factory);
}

bool GraphicsManager::checkVulkanSupported()
{
#ifdef LN_USE_VULKAN
	return VulkanHelper::checkVulkanSupported();
#else
	return false;
#endif
}

void GraphicsManager::createOpenGLContext(const Settings& settings)
{
	OpenGLDevice::Settings openglSettings;
	openglSettings.platformManager = m_platformManager;
	openglSettings.mainWindow = settings.mainWindow;
	auto ctx = makeRef<OpenGLDevice>();
	ctx->init(openglSettings);
	m_deviceContext = ctx;
}

void GraphicsManager::createVulkanContext(const Settings& settings)
{
#ifdef LN_USE_VULKAN

	VulkanDevice::Settings dcSettings;
    dcSettings.mainWindow = settings.mainWindow;
	dcSettings.debugMode = settings.debugMode;
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
}

//Ref<RenderingCommandList> GraphicsManager::submitCommandList(RenderingCommandList* commandList)
//{
//	if (LN_REQUIRE(commandList)) return nullptr;
//    commandList->execute(); // TODO: test
//	commandList->clear();
//	return m_inFlightRenderingCommandList;
//}

} // namespace detail
} // namespace ln

