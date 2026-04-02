#pragma once
#include <memory>
#include <LuminoCore/graphics/rhi/Rhi.hpp>

namespace ln {
class GraphicsModule;
class ObjectRegistry;

// Library root manager クラス。シングルトンで、ライブラリ全体の初期化や終了処理を管理する。
class CoreInstance final {
public:
    struct Settings {
        rhi::Backend preferredBackend = rhi::Backend::Vulkan;
        bool enableValidation = false;
    };
    static CoreInstance* instance() { return s_instance.get(); };
    static VoidResult initialize(const Settings& settings);
    static void terminate();

    ObjectRegistry* objectRegistry() { return m_objectRegistry.get(); }
    GraphicsModule* graphicsModule() const { return m_graphicsModule.get(); }

private:
    VoidResult init(const Settings& settings);
    void dispose();
    static std::unique_ptr<CoreInstance> s_instance;
    Settings m_settings;
    std::unique_ptr<ObjectRegistry> m_objectRegistry;
    Ref<GraphicsModule> m_graphicsModule;
};

} // namespace ln
