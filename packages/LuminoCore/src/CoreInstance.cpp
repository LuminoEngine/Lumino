#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>

#if !defined(__EMSCRIPTEN__) && !defined(LN_NX)
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#endif

namespace ln {

std::unique_ptr<CoreInstance> CoreInstance::s_instance;

VoidResult CoreInstance::initialize(const Settings& settings) {
    if (s_instance) return LN_MAKE_SUCCESS();
    s_instance = std::unique_ptr<CoreInstance>(LN_NEW CoreInstance());
    return s_instance->init(settings);
}

void CoreInstance::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

VoidResult CoreInstance::init(const Settings& settings) {
    LN_LOG_INFO("CoreInstance::init: begin");
    m_settings = settings;
    m_objectRegistry = std::make_unique<ObjectRegistry>();

#if !defined(__EMSCRIPTEN__) && !defined(LN_NX)
    glfwInit();
#endif

    {
        GraphicsModule::Settings gfxSettings;
        gfxSettings.enableValidation = m_settings.enableValidation;
        gfxSettings.preferredBackend = m_settings.preferredBackend;
        auto result = GraphicsModule::create(gfxSettings);
        if (!result) {
            return LN_FORWARD_ERROR(result);
        }
        m_graphicsModule = std::move(*result);
    }

    LN_LOG_INFO("CoreInstance::init: end");
    return LN_MAKE_SUCCESS();
}

void CoreInstance::dispose() {
    LN_LOG_INFO("CoreInstance::dispose: begin");
    if (m_graphicsModule) {
        m_graphicsModule->dispose();
        m_graphicsModule.reset();
    }
    m_objectRegistry.reset();
#if !defined(__EMSCRIPTEN__) && !defined(LN_NX)
    glfwTerminate();
#endif
    LN_LOG_INFO("CoreInstance::dispose: end");
}

rhi::Device* CoreInstance::rhiDevice() const {
    return m_graphicsModule ? m_graphicsModule->device() : nullptr;
}

} // namespace ln
