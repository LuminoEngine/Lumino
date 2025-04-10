#include <LuminoEngine/Graphics/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsResource.hpp>
#include <LuminoEngine/Graphics/GPU/CommandQueue.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/Shader.hpp>
#include <LuminoEngine/Graphics/GPU/SamplerState.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/GraphicsExtension.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include "GPU/RenderTargetTextureCache.hpp"
#ifdef LN_USE_VULKAN
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDevice.hpp>
#endif
#include <LuminoEngine/Graphics/GPU/RHIIntegrations.hpp>
#ifdef LN_USE_WEBGPU
#include <LuminoEngine/Graphics/GraphicsRHI/WebGPU/WebGPUDevice.hpp>
#endif
#include "../../LuminoEngine/src/Graphics/GraphicsRHI/OpenGL/OpenGLDeviceContext.hpp"
#include <LuminoEngine/Graphics/GPU/detail/RenderingCommandList.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include "GPU/SingleFrameAllocator.hpp"
#include "GPU/GraphicsProfiler.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/detail/ShaderManager.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/RHIHelper.hpp>
#include "GPU/StreamingBufferAllocator.hpp"
#include "GPU/RenderPassCache.hpp"
//#include "Rendering/RenderingManager2.hpp"
#include <LuminoEngine/Mesh/detail/MeshManager.hpp>

namespace ln {
namespace detail {

void ConstantBufferView::setData(const void* data, size_t size) const {
    //byte_t* d = static_cast<byte_t*>(buffer->map()) + offset;	// TODO: map しないほうが効率いい？SingleFrameのデータなので、フレーム締めるときに unmap でよい
    //memcpy(d, data, size);
    //buffer->unmap();
    byte_t* d = static_cast<byte_t*>(buffer->writableData()) + offset;
    memcpy(d, data, size);
}

void* ConstantBufferView::writableData() const {
    return static_cast<byte_t*>(buffer->writableData()) + offset;
}

} // namespace detail

//==============================================================================
// GraphicsHelper

const Char* GraphicsHelper::CandidateExts_Texture2D[5] = { _TT(".png"), _TT(".jpg"), _TT(".tga"), _TT(".bmp"), _TT(".gif") };

PixelFormat GraphicsHelper::translateToPixelFormat(TextureFormat format) {
    switch (format) {
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

TextureFormat GraphicsHelper::translateToTextureFormat(PixelFormat format) {
    switch (format) {
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


//==============================================================================
// GraphicsManager

   // TODO: 移行期間の暫定処置
GraphicsManager* s_GraphicsManager = nullptr;
GraphicsManager* GraphicsManager::instance() {
    return s_GraphicsManager;
}

GraphicsManager::GraphicsManager()
    : m_assetManager(nullptr)
    //, m_platformManager(nullptr)
    , m_resourceRegistry(makeURef<detail::GraphicsObjectRegistry>())
    //, m_vertexBufferRegistry(makeURef<GraphicsResourceRegistry>())
    //, m_indexBufferRegistry(makeURef<GraphicsResourceRegistry>())
    //, m_constantBufferRegistry(makeURef<GraphicsResourceRegistry>())
    //, m_shaderPassRegistry(makeURef<GraphicsResourceRegistry>())
    //, m_textureRegistry(makeURef<GraphicsResourceRegistry>())
    //, m_depthBufferRegistry(makeURef<GraphicsResourceRegistry>())
    , m_linearAllocatorPageManager() {
}

MaybeResult GraphicsManager::init(const Settings& settings) {
    LN_LOG_DEBUG("GraphicsManager Initialization started.");
    s_GraphicsManager = this;
    m_settings = settings;

    m_assetManager = detail::AssetManager::instance();
    if (LN_ASSERT(m_assetManager)) return LN_MAKE_ERROR();
    
    //m_platformManager = PlatformManager::instance();
    //if (LN_ASSERT(m_platformManager)) return false;

    m_profiler = std::make_unique<detail::GraphicsProfiler>();

    m_texture2DCache.init(64);
    m_shaderCache.init(64);

//    // Create device context
//    {
//        if (settings.graphicsAPI == GraphicsAPI::OpenGL) {
//            createOpenGLContext(settings);
//        }
//        else if (settings.graphicsAPI == GraphicsAPI::Vulkan) {
//            createVulkanContext(settings);
//        }
//        else if (settings.graphicsAPI == GraphicsAPI::DirectX12) {
//            createDirectX12Context(settings);
//        }
//
//        // Fallback
//        {
//#ifdef _WIN32
//            if (!m_deviceContext) {
//                createDirectX12Context(settings);
//            }
//#else
//            if (!m_deviceContext) {
//                createVulkanContext(settings);
//            }
//#endif
//        }
//    }

    //m_deviceContext->refreshCaps();

    //{
    //    auto& triple = m_deviceContext->caps().requestedShaderTriple;
    //    LN_LOG_INFO("requestedShaderTriple: {}-{}-{}", triple.target, triple.version, triple.option);
    //}

    m_linearAllocatorPageManager = makeRef<detail::LinearAllocatorPageManager>();


    //if (auto queue = m_deviceContext->getGraphicsCommandQueue()) {
    //	m_graphicsQueue = Ref<CommandQueue>(LN_NEW CommandQueue(), false);
    //	m_graphicsQueue->init(queue);
    //}
    //if (auto queue = m_deviceContext->getComputeCommandQueue()) {
    //	m_computeQueue = Ref<CommandQueue>(LN_NEW CommandQueue(), false);
    //	m_computeQueue->init(queue);
    //}

    m_renderingQueue = makeRef<detail::RenderingQueue>();

    m_renderTargetTextureCacheManager = makeRef<detail::RenderTargetTextureCacheManager>();
    m_depthBufferCacheManager = makeRef<detail::DepthBufferCacheManager>();
    m_frameBufferCache = makeRef<detail::FrameBufferCache>(m_renderTargetTextureCacheManager, m_depthBufferCacheManager);
    m_renderPassCache = makeURef<detail::RenderPassCache>();

    m_extensions.add(nullptr); // [0] is dummy

    // default objects
    {
        m_blackTexture = makeObject_deprecated<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_blackTexture->clear(Color::Black);

        m_whiteTexture = makeObject_deprecated<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_whiteTexture->clear(Color::White);

        m_defaultNormalMap = makeObject_deprecated<Texture2D>(32, 32, TextureFormat::RGBA8);
        m_defaultNormalMap->clear(Color(0.0f, 0.0f, 1.0f, 1.0f));

        m_defaultSamplerState = makeObject_deprecated<SamplerState>();
        m_defaultSamplerState->setFrozen(true);

        m_linearSamplerState = makeObject_deprecated<SamplerState>(TextureFilterMode::Linear);
        m_linearSamplerState->setFrozen(true);

        m_pointClampSamplerState = makeObject_deprecated<SamplerState>(TextureFilterMode::Point, TextureAddressMode::Clamp);
        m_pointClampSamplerState->setFrozen(true);

        m_linearClampSamplerState = makeObject_deprecated<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);
        m_pointClampSamplerState->setFrozen(true);
    }

//#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    {
        detail::ShaderManager::Settings shaderManagerSettings;
        m_shaderManager = detail::ShaderManager::initialize(shaderManagerSettings);
    }
//#endif

    {
        detail::MeshManager::Settings settings;
        settings.graphicsManager = this;
        settings.assetManager = m_assetManager;
        m_meshManager = makeURef<detail::MeshManager>();
        if (!m_meshManager->init(settings)) {
            return LN_MAKE_ERROR();
        }
    }

    LN_LOG_DEBUG("GraphicsManager Initialization ended.");
    return LN_MAKE_SUCCESS();
}

void GraphicsManager::dispose() {
    m_vertexBufferStreamingAllocatorManager.clear();
    m_indexBufferStreamingAllocatorManager.clear();

    if (m_openglIntegrationCommandList) {
        m_openglIntegrationCommandList->dispose();
        m_openglIntegrationCommandList = nullptr;
    }

    if (m_renderingQueue) {
        m_renderingQueue->dispose();
        m_renderingQueue = nullptr;
    }

    if (m_meshManager) {
        m_meshManager->dispose();
        m_meshManager = nullptr;
    }

    m_shaderCache.dispose();
    m_texture2DCache.dispose();
    if (m_renderPassCache) {
        m_renderPassCache->clear();
        m_renderPassCache = nullptr;
    }

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

    m_frameBufferCache = nullptr;
    m_depthBufferCacheManager = nullptr;
    m_renderTargetTextureCacheManager = nullptr;

    //m_deviceContext->dispose();

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    if (m_shaderManager) {
        detail::ShaderManager::terminate();
        m_shaderManager = nullptr;
    }
 #endif
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


Result_deprecated<Ref<GraphicsContext>> GraphicsManager::createGraphicsContext(PlatformWindow* window) {
    Ref<GraphicsContext> result;

    // Create device context
    {
        if (m_settings.graphicsAPI == LN_GRAPHICS_BACKEND_OPENGL) {
            OpenGLGraphicsContext::Settings s;
            s.window = window;
            s.defaultFramebuffer = 0;
            result = OpenGLGraphicsContext::create(s);
        }
        else if (m_settings.graphicsAPI == LN_GRAPHICS_BACKEND_VULKAN) {
#ifdef LN_USE_VULKAN
            VulkanGraphicsContext::Settings s;
            s.mainWindow = window;
            s.debugMode = m_settings.debugMode;
            result = VulkanGraphicsContext::create(s);
#endif
        }
        else if (m_settings.graphicsAPI == LN_GRAPHICS_BACKEND_DIRECTX12) {
#ifdef _WIN32
            DirectX12GraphicsContext::Settings s;
            s.mainWindow = window;
            s.debugMode = m_settings.debugMode;
            s.priorityAdapterName = m_settings.priorityGPUName;
            result = DirectX12GraphicsContext::create(s);
#endif
        }
        else if (m_settings.graphicsAPI == LN_GRAPHICS_BACKEND_WEBGPU) {
#ifdef LN_USE_WEBGPU
            WebGPUGraphicsContext::Settings s;
            s.mainWindow = window;
            s.debugMode = m_settings.debugMode;
            result = WebGPUGraphicsContext::create(s);
#endif
        }

        // Fallback
        if (!result) {
#ifdef _WIN32
            DirectX12GraphicsContext::Settings s;
            s.mainWindow = window;
            s.debugMode = m_settings.debugMode;
            s.priorityAdapterName = m_settings.priorityGPUName;
            result = DirectX12GraphicsContext::create(s);
#else
            OpenGLGraphicsContext::Settings s;
            s.window = window;
            s.defaultFramebuffer = 0;
            result = OpenGLGraphicsContext::create(s);
#endif
        }
    }

    if (!result) {
        auto& triple = result->rhiDevice()->caps().requestedShaderTriple;
        LN_LOG_INFO("requestedShaderTriple: {}-{}-{}", triple.target, triple.version, triple.option);
    }
    return result;
}

void GraphicsManager::addGraphicsResource(IGraphicsResource* resource) {
    m_graphicsResources.add(resource);
}

void GraphicsManager::removeGraphicsResource(IGraphicsResource* resource) {
    m_graphicsResources.remove(resource);
}

//int GraphicsManager::registerExtension(INativeGraphicsExtension* extension) {
//    if (LN_REQUIRE(extension)) return 0;
//    m_extensions.add(extension);
//    extension->onLoaded(m_deviceContext->getNativeInterface());
//    return m_extensions.size() - 1;
//}
//
//void GraphicsManager::unregisterExtension(INativeGraphicsExtension* extension) {
//    if (LN_REQUIRE(extension)) return;
//    m_extensions.remove(extension);
//    extension->onUnloaded(m_deviceContext->getNativeInterface());
//}

Ref<Texture> GraphicsManager::requestTexture(const detail::AssetPath& assetPath) {
    // TODO: cache
    auto stream = m_assetManager->openStreamFromAssetPath(assetPath);
    if (stream) {
        return makeObject_deprecated<Texture2D>(stream, TextureFormat::RGBA8); // TODO: format
    }
    else {
        return nullptr;
    }
}

Ref<Texture2D> GraphicsManager::loadTexture2D(const StringView& filePath) {
    m_texture2DCache.collectUnreferenceObjects(false);

    static const std::vector<const Char*> exts = { _TT(".png"), _TT(".jpg"), _TT(".tga"), _TT(".bmp"), _TT(".gif") };

#if 1
    return detail::AssetManager::loadObjectWithCacheHelper<Texture2D>(
        &m_texture2DCache, nullptr, exts, filePath, nullptr);
#else
    auto pathSet = std::make_unique<AssetRequiredPathSet>();
    if (!AssetObject::_resolveAssetRequiredPathSet(filePath, exts, pathSet.get())) {
        return nullptr;
    }

    // finalResourceAssetFilePath から拡張子を除いたものを CacheKey とする
    // > CacheKey はどの Archive に入っているファイルであるかまで区別できるものでなければダメ。
    // > Archive 名と、それを基準とした相対パス(または絶対パス) で表す必要がある。
    // > 拡張子は無くてもOK。.yml でも .png でも、出来上がる Texture2D は同じもの。
        const auto cacheKey = Path(pathSet->finalResourceAssetFilePath.toString()).replaceExtension(_TT("");

	if (auto obj = m_texture2DCache.findObject(cacheKey)) {
        return obj;
	}

	auto obj = makeObject_deprecated<Texture2D>();
	obj->m_data = std::move(pathSet);
	obj->reload();

	m_texture2DCache.registerObject(cacheKey, obj);

	return obj;
#endif
}

Ref<Texture2D> GraphicsManager::loadTexture2DFromOnMemoryData(
    const detail::AssetPath* baseDir,
    const StringView& filePath,
    std::function<Ref<Texture2D>(const detail::AssetRequiredPathSet*)> factory) {
    static const std::vector<const Char*> exts = { _TT(".png"), _TT(".jpg"), _TT(".tga"), _TT(".bmp"), _TT(".gif") };
    return detail::AssetManager::loadObjectWithCacheHelper<Texture2D>(
        texture2DCache(), baseDir, exts, filePath, factory);
}

//GraphicsCommandList* GraphicsManager::getOpenGLIntegrationCommandList() {
//    if (!m_openglIntegrationCommandList) {
//        m_openglIntegrationCommandList = makeObject_deprecated<GraphicsCommandList>(this);
//    }
//    return m_openglIntegrationCommandList;
//}

bool GraphicsManager::checkVulkanSupported() {
#ifdef LN_USE_VULKAN
    return detail::VulkanHelper::checkVulkanSupported();
#else
    return false;
#endif
}

void GraphicsManager::selectDefaultSystem(LNGraphicsBackend* api, WindowSystem* ws) {
    LNGraphicsBackend defaultBackend;
    WindowSystem defaultSystem;
    if (Environment::isRuntimePlatform(RuntimePlatform::Windows)) {
        defaultBackend = LN_GRAPHICS_BACKEND_DIRECTX12;
        defaultSystem = WindowSystem::Native;
    }
    else if (Environment::isRuntimePlatform(RuntimePlatform::Web)) {
        defaultBackend = LN_GRAPHICS_BACKEND_OPENGL;
        defaultSystem = WindowSystem::GLFWWithOpenGL;
    }
    else {
        LN_NOTIMPLEMENTED();
    }

    if (*api == LN_GRAPHICS_BACKEND_DEFAULT) {
        *api = defaultBackend;
    }
    if (*ws == WindowSystem::Native) {
        *ws = defaultSystem;
    }
}

detail::StreamingBufferAllocatorManager* GraphicsManager::obtainVertexBufferStreamingAllocatorManager(
    size_t elementSize) {
    auto r = m_vertexBufferStreamingAllocatorManager.findIf([elementSize](auto& x) { return x->elementSize() == elementSize; });
    if (r) {
        return *r;
    }
    else {
        auto manager = makeURef<detail::StreamingBufferAllocatorManager>(
            detail::StreamingBufferPage::Type::VertexBuffer, elementSize);
        m_vertexBufferStreamingAllocatorManager.push(std::move(manager));
        return m_vertexBufferStreamingAllocatorManager.back();
    }
}

detail::StreamingBufferAllocatorManager* GraphicsManager::obtainIndexBufferStreamingAllocatorManager(
    IndexBufferFormat format) {
    size_t elementSize = RHIHelper::getIndexStride(format);
    auto r = m_indexBufferStreamingAllocatorManager.findIf([elementSize](auto& x) { return x->elementSize() == elementSize; });
    if (r) {
        return *r;
    }
    else {
        auto manager = makeURef<detail::StreamingBufferAllocatorManager>(
            detail::StreamingBufferPage::Type::VertexBuffer, elementSize);
        m_indexBufferStreamingAllocatorManager.push(std::move(manager));
        return m_indexBufferStreamingAllocatorManager.back();
    }
}

Ref<Shader> GraphicsManager::loadShader(const StringView& filePath) {
    m_shaderCache.collectUnreferenceObjects(false);

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    static const std::vector<const Char*> exts = { _TT(".hlsl"), _TT(".fx"), _TT(".lcfx") };
#else
    static const std::vector<const Char*> exts = { _TT(".lcfx") };
#endif
    return detail::AssetManager::loadObjectWithCacheHelper<Shader>(&m_shaderCache, nullptr, exts, filePath, nullptr);
}

//void GraphicsManager::createOpenGLContext(const Settings& settings) {
//    OpenGLDevice::Settings dcSettings;
//    dcSettings.platformManager = m_platformManager;
//    dcSettings.mainWindow = m_platformManager->mainWindow();
//    dcSettings.defaultFramebuffer = 0;
//    auto device = makeRef<OpenGLDevice>();
//    bool driverSupported = false;
//    if (!device->init(dcSettings)) {
//        LN_ERROR("OpenGL driver initialization failed.");
//        return;
//    }
//    else {
//        m_deviceContext = device;
//    }
//}
//
//void GraphicsManager::createVulkanContext(const Settings& settings) {
//#ifdef LN_USE_VULKAN
//    VulkanDevice::Settings dcSettings;
//    dcSettings.mainWindow = m_platformManager->mainWindow();
//    dcSettings.debugMode = settings.debugMode;
//
//    bool driverSupported = false;
//    auto device = detail::VulkanDevice::create(dcSettings, &driverSupported);
//    if (!device) {
//        if (!driverSupported) {
//            // ドライバが Vulkan をサポートしていない。継続する。
//        }
//        else {
//            LN_ERROR("Vulkan driver initialization failed.");
//            return;
//        }
//    }
//    else {
//        m_deviceContext = *device;
//    }
//#endif
//}
//
//void GraphicsManager::createDirectX12Context(const Settings& settings) {
//#if _WIN32
//    DX12Device::Settings dcSettings;
//    dcSettings.mainWindow = m_platformManager->mainWindow();
//    dcSettings.debugMode = settings.debugMode;
//    dcSettings.priorityAdapterName = settings.priorityGPUName.toStdWString();
//    auto ctx = makeRef<DX12Device>();
//    bool driverSupported = false;
//    if (!ctx->init(dcSettings, &driverSupported)) {
//        if (!driverSupported) {
//            // ドライバが Vulkan をサポートしていない。継続する。
//        }
//        else {
//            LN_ERROR("Vulkan driver initialization failed.");
//            return;
//        }
//    }
//    else {
//        m_deviceContext = ctx;
//    }
//#endif
//}

//Ref<RenderingCommandList> GraphicsManager::submitCommandList(RenderingCommandList* commandList)
//{
//	if (LN_REQUIRE(commandList)) return nullptr;
//    commandList->execute(); // TODO: test
//	commandList->clear();
//	return m_inFlightRenderingCommandList;
//}

} // namespace ln
