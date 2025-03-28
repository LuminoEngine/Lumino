#ifdef LN_USE_DIRECTX12
#include "../../LuminoEngine/src/Graphics/GraphicsRHI/DirectX12/DX12DeviceContext.hpp"
#endif // LN_USE_DIRECTX12

#ifdef LN_USE_VULKAN
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanTextures.hpp>
#endif // LN_USE_VULKAN

#ifdef LN_USE_OPENGL
#include "../../LuminoEngine/src/Graphics/GraphicsRHI/OpenGL/OpenGLDeviceContext.hpp"
#include "../../LuminoEngine/src/Graphics/GraphicsRHI/OpenGL/GLTextures.hpp"
#include "../../LuminoEngine/src/Graphics/GraphicsRHI/OpenGL/GLRenderPass.hpp"
#endif // LN_USE_VULKAN

#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformWindowManager.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Graphics/GPU/RHIIntegrations.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/RenderPass.hpp>

namespace ln {

#ifdef LN_USE_DIRECTX12
Ref<DirectX12GraphicsContext> DirectX12GraphicsContext::create(const Settings& settings) {
    auto ptr = Ref<DirectX12GraphicsContext>(LN_NEW DirectX12GraphicsContext(), false);
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

DirectX12GraphicsContext::DirectX12GraphicsContext() {
}

DirectX12GraphicsContext::~DirectX12GraphicsContext() {
}

bool DirectX12GraphicsContext::init(const Settings& settings) {

    detail::DX12Device::Settings dcSettings;
    dcSettings.mainWindow = settings.mainWindow;
    dcSettings.debugMode = settings.debugMode;
    dcSettings.priorityAdapterName = settings.priorityAdapterName.toStdWString();
    auto device = makeRef<detail::DX12Device>();
    bool driverSupported = false;
    if (!device->init(dcSettings, &driverSupported)) {
        if (!driverSupported) {
            // ドライバが DirectX12 をサポートしていない。継続する。
        }
        else {
            LN_LOG_ERROR("DirectX12 driver initialization failed.");
            return false;
        }
    }
    else {
        m_device = device;
    }
    m_device->refreshCaps();

    return GraphicsContext::init(settings.mainWindow);
}

void DirectX12GraphicsContext::onDispose(bool explicitDisposing) {
    GraphicsContext::onDispose(explicitDisposing);
    if (m_device) {
        m_device->dispose();
        m_device = nullptr;
    }
}

detail::IGraphicsDevice* DirectX12GraphicsContext::rhiDevice() const {
    return m_device;
}

#endif // LN_USE_DIRECTX12

#ifdef LN_USE_VULKAN

#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
Ref<VulkanGraphicsContext> VulkanGraphicsContext::create(const Settings& settings) {
    auto ptr = Ref<VulkanGraphicsContext>(LN_NEW VulkanGraphicsContext(), false);
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

VulkanGraphicsContext::VulkanGraphicsContext() {
}

VulkanGraphicsContext::~VulkanGraphicsContext() {
}

bool VulkanGraphicsContext::init(const Settings& settings) {

    detail::VulkanDevice::Settings dcSettings;
    dcSettings.mainWindow = settings.mainWindow;
    dcSettings.debugMode = settings.debugMode;

    bool driverSupported = false;
    auto device = detail::VulkanDevice::create(dcSettings, &driverSupported);
    if (!device) {
        if (!driverSupported) {
            // ドライバが Vulkan をサポートしていない。継続する。
        }
        else {
            LN_LOG_ERROR("Vulkan driver initialization failed.");
            return false;
        }
    }
    else {
        m_device = *device;
    }
    m_device->refreshCaps();

    return GraphicsContext::init(settings.mainWindow);
}

void VulkanGraphicsContext::onDispose(bool explicitDisposing) {
    GraphicsContext::onDispose(explicitDisposing);
    if (m_device) {
        m_device->dispose();
        m_device = nullptr;
    }
}

detail::IGraphicsDevice* VulkanGraphicsContext::rhiDevice() const {
    return m_device;
}

void VulkanIntegration::getImageInfo(
    GraphicsCommandList* graphicsContext,
    RenderTargetTexture* texture,
    VkImage* outImage,
    VkImageView* outImageView,
    VkFormat* outFormat,
    int* outWidth,
    int* outHeight) {
    auto vulkanTexture = static_cast<detail::VulkanRenderTarget*>(
        detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, texture, nullptr));
    auto image = vulkanTexture->image();
    *outImage = image->vulkanImage();
    *outImageView = image->vulkanImageView();
    *outFormat = image->vulkanFormat();
    *outWidth = vulkanTexture->extentSize().width;
    *outHeight = vulkanTexture->extentSize().height;
}

void VulkanIntegration::getImageInfo(
    GraphicsCommandList* graphicsContext,
    DepthBuffer* texture,
    VkImage* outImage,
    VkImageView* outImageView,
    VkFormat* outFormat,
    int* outWidth,
    int* outHeight) {
    auto vulkanTexture = static_cast<detail::VulkanDepthBuffer*>(
        detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, texture, nullptr));
    auto image = vulkanTexture->image();
    *outImage = image->vulkanImage();
    *outImageView = image->vulkanImageView();
    *outFormat = image->vulkanFormat();
    *outWidth = vulkanTexture->extentSize().width;
    *outHeight = vulkanTexture->extentSize().height;
}

#endif // LN_USE_VULKAN

#ifdef LN_USE_OPENGL

Ref<OpenGLGraphicsContext> OpenGLGraphicsContext::create(const Settings& settings) {
    auto ptr = Ref<OpenGLGraphicsContext>(LN_NEW OpenGLGraphicsContext(), false);
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

OpenGLGraphicsContext::OpenGLGraphicsContext()
    : m_device(nullptr)
    , m_window(nullptr)
    , m_wrapedRenderTarget(nullptr)
    , m_externalDefaultFBO(0)
    , m_externalWidth(0)
    , m_externalHeight(0) {
}

OpenGLGraphicsContext::~OpenGLGraphicsContext() {
}

bool OpenGLGraphicsContext::init(const Settings& settings) {

    detail::PlatformManager* platformManager = nullptr;
    if (settings.window) {
        platformManager = settings.window->windowManager()->manager();
        // TODO: このあたり、古い仕組みを合わせこむための暫定対応。
        // GLContext をライブラリで管理する場合、Platfrom ではなく Graphics 側で場合分けしていいと思う。 Vulkan とかそんな感じだし。
        // auto mainGLContext = platformManager->openGLContext();
        // if (mainGLContext) {
        //    mainGLContext->makeCurrentMain();
        //}
        m_window = settings.window;
    }
    m_externalDefaultFBO = settings.defaultFramebuffer;
    m_externalWidth = settings.width;
    m_externalHeight = settings.height;

    detail::OpenGLDevice::Settings dcSettings;
    dcSettings.platformManager = platformManager;
    dcSettings.mainWindow = settings.window;
    dcSettings.defaultFramebuffer = 0;
    auto device = makeRef<detail::OpenGLDevice>();
    bool driverSupported = false;
    if (!device->init(dcSettings)) {
        LN_ERROR("OpenGL driver initialization failed.");
        return false;
    }
    else {
        m_device = device;
    }

    m_device->refreshCaps();

    return GraphicsContext::init(settings.window);
}

void OpenGLGraphicsContext::onDispose(bool explicitDisposing) {
    GraphicsContext::onDispose(explicitDisposing);
    if (m_device) {
        m_device->dispose();
        m_device = nullptr;
    }
}

void OpenGLGraphicsContext::onCreateRHIObjects() {
    if (isUseExternalGLContext()) {
        const int count = 1;
        m_backbuffers.resize(count);
        m_depthBuffers.resize(count);
        m_renderPasses.resize(count);

        // Dummy RenderTarget representing the back buffer.
        //m_wrapedRHIRenderTarget = wrapRef(LN_NEW detail::GLRenderTargetTexture());
        //if (!m_wrapedRHIRenderTarget->init(0, m_externalWidth, m_externalHeight)) {
        //    LN_ERROR("GLRenderTargetTexture failed.");
        //    return;
        //}
        m_wrapedRenderTarget = wrapRef(LN_NEW RenderTargetTexture());
        if (!m_wrapedRenderTarget->init(this)) {
            LN_ERROR("RenderTargetTexture failed.");
            return;
        }
        m_wrapedRenderTarget->resetNativeObject(0);
        m_wrapedRenderTarget->resetSize(m_externalWidth, m_externalHeight);
        m_backbuffers[0] = m_wrapedRenderTarget;

        // Dummy RenderPass representing the back buffer.
        //m_wrapedRHIRenderPass = wrapRef(LN_NEW detail::GLRenderPass());
        //if (!m_wrapedRHIRenderPass->initFromNativeFBO(m_externalDefaultFBO, m_wrapedRHIRenderTarget)) {
        //    LN_ERROR("GLRenderPass failed.");
        //    return;
        //}
        m_wrapedRenderPass = wrapRef(LN_NEW RenderPass());
        if (!m_wrapedRenderPass->init(m_wrapedRenderTarget, nullptr)) {
            LN_ERROR("RenderPass failed.");
            return;
        }
        m_renderPasses[0] = m_wrapedRenderPass;
    }
    else {
        GraphicsContext::onCreateRHIObjects();
    }
}

detail::IGraphicsDevice* OpenGLGraphicsContext::rhiDevice() const {
    return m_device;
}

bool OpenGLGraphicsContext::isUseExternalGLContext() const {
    return m_window == nullptr;
}

#endif // LN_USE_OPENGL

} // namespace ln

