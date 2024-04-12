#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformWindowManager.hpp>
#include <LuminoEngine/Platform/detail/OpenGLContext.hpp>
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/GPU/OpenGLGraphicsContext.hpp>
#include "../../LuminoRuntime/src/GraphicsRHI/OpenGL/OpenGLDeviceContext.hpp"

namespace ln {

Ref<OpenGLGraphicsContext> OpenGLGraphicsContext::create(const Settings& settings) {
    auto ptr = Ref<OpenGLGraphicsContext>(LN_NEW OpenGLGraphicsContext(), false);
    if (!ptr->init(settings)) {
        return nullptr;
    }
    return ptr;
}

OpenGLGraphicsContext::OpenGLGraphicsContext()
    : m_device(nullptr) {
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
    }

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
    return true;
}

void OpenGLGraphicsContext::onDispose(bool explicitDisposing) {
    GraphicsContext::onDispose(explicitDisposing);
    if (m_device) {
        m_device->dispose();
        m_device = nullptr;
    }
}

detail::IGraphicsDevice* OpenGLGraphicsContext::rhiDevice() const {
    return m_device;
}

} // namespace ln
