#pragma once
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Graphics/GraphicsModule.hpp>
#include <LuminoCore/Runtime/ObjectRegistry.hpp>

#ifndef LN_NX
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
    m_settings = settings;
    m_objectRegistry = std::make_unique<ObjectRegistry>();

    glfwInit();

    {
        GraphicsModule::Settings settings;
        settings.enableValidation = m_settings.enableValidation;
        settings.preferredBackend = m_settings.preferredBackend;
        auto result = GraphicsModule::create(settings);
        if (!result) {
            return LN_FORWARD_ERROR(result);
        }
        m_graphicsModule = std::move(*result);
    }

    return LN_MAKE_SUCCESS();
}

void CoreInstance::dispose() {
    if (m_graphicsModule) {
        m_graphicsModule->dispose();
        m_graphicsModule.reset();
    }
    m_objectRegistry.reset();
    glfwTerminate();
}

} // namespace ln
