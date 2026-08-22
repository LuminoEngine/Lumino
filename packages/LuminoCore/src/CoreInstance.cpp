#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>

#if defined(LN_ENABLE_VULKAN)
    // GLFW より先に volk を取り込む (VulkanLoader.hpp のコメントを参照)。
    #include "Graphics/rhi/vulkan/VulkanLoader.hpp"
#endif

#if !defined(__EMSCRIPTEN__) && !defined(LN_NX)
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#endif

namespace ln {

std::unique_ptr<CoreInstance> CoreInstance::s_instance;

VoidResult CoreInstance::initialize(const Settings& settings) {
    if (s_instance) return LN_MAKE_SUCCESS();
    s_instance = std::unique_ptr<CoreInstance>(new CoreInstance());
    return s_instance->init(settings);
}

void CoreInstance::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

VoidResult CoreInstance::init(const Settings& settings) {
    LN_LOG_TRACE("CoreInstance::init: begin");
    m_settings = settings;
    m_objectRegistry = std::make_unique<ObjectRegistry>();

#if !defined(__EMSCRIPTEN__) && !defined(LN_NX)
    #if defined(LN_ENABLE_VULKAN)
    // GLFW は既定で自前に Vulkan ローダーを dlopen するが、Lumino が volk で
    // ロードしたものを使わせることでローダーの実体を 1 つに統一する。
    // glfwInit() より前に呼ぶ必要がある。ローダーが無い環境では何もしない
    // (GLFW は Vulkan を使わないウィンドウ生成であれば問題なく動作する)。
    if (rhi::vulkan::loadVulkanLoader()) {
        glfwInitVulkanLoader(vkGetInstanceProcAddr);
    }
    #endif
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

    LN_LOG_TRACE("CoreInstance::init: end");
    return LN_MAKE_SUCCESS();
}

void CoreInstance::dispose() {
    LN_LOG_TRACE("CoreInstance::dispose: begin");
    if (m_graphicsModule) {
        m_graphicsModule->dispose();
        m_graphicsModule.reset();
    }
    m_objectRegistry.reset();
#if !defined(__EMSCRIPTEN__) && !defined(LN_NX)
    glfwTerminate();
#endif
    LN_LOG_TRACE("CoreInstance::dispose: end");
}

rhi::Device* CoreInstance::rhiDevice() const {
    return m_graphicsModule ? m_graphicsModule->device() : nullptr;
}

} // namespace ln
