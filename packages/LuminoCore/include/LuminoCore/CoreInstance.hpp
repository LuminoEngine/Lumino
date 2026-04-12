#pragma once
#include <memory>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

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
#if !defined(__EMSCRIPTEN__)
    GraphicsModule* graphicsModule() const { return m_graphicsModule.get(); }
#else
    // Web ビルドでは GraphicsModule は使わず、RHI Device を直接保持する。
    GraphicsModule* graphicsModule() const { return nullptr; }
#endif

    /** RHI デバイスへのアクセサ。Web では CoreInstance が直接保持する。Desktop では GraphicsModule 経由。 */
    rhi::Device* rhiDevice() const;

private:
    VoidResult init(const Settings& settings);
    void dispose();
    static std::unique_ptr<CoreInstance> s_instance;
    Settings m_settings;
    std::unique_ptr<ObjectRegistry> m_objectRegistry;
#if !defined(__EMSCRIPTEN__)
    Ref<GraphicsModule> m_graphicsModule;
#else
    Ref<rhi::Device> m_rhiDevice;
#endif
};

} // namespace ln
