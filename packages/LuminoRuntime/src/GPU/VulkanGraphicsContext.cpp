#include <LuminoEngine/GraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include <LuminoEngine/GPU/VulkanGraphicsContext.hpp>

namespace ln {

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
    return true;
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

} // namespace ln

