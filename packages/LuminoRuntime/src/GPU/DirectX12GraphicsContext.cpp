#include "../../LuminoRuntime/src/GraphicsRHI/DirectX12/DX12DeviceContext.hpp"
#include <LuminoEngine/GPU/DirectX12GraphicsContext.hpp>

namespace ln {

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
            // ドライバが Vulkan をサポートしていない。継続する。
        }
        else {
            LN_LOG_ERROR("Vulkan driver initialization failed.");
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

} // namespace ln

