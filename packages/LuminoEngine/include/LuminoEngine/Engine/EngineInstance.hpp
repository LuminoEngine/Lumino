#pragma once
#include <LuminoCore/Runtime/TypeInfo.hpp>
#include <LuminoCore/Runtime/Property.hpp>
#include "Common2.hpp"
#include "../Asset/Common.hpp"
#include "../Platform/Common.hpp"
#include "../Graphics/Common.hpp"

namespace ln {
struct RuntimeModuleSettings {
    WindowSystem windowSystem = WindowSystem::Native;
    AssetStorageAccessPriority assetStorageAccessPriority = AssetStorageAccessPriority::DirectoryFirst;
    GraphicsAPI graphicsAPI = GraphicsAPI::OpenGL;
};
namespace detail {
class RuntimeManager;
class PlatformManager;
class AssetManager;
} // namespace detail

class EngineInstance {
public:
    static EngineInstance* instance() { return s_instance.get(); };

    /** Initialize context. */
    static MaybeResult initialize(const RuntimeModuleSettings& settings, EngineInstance* sharedContext = nullptr);

    /** Terminate context. */
    static void terminate();

    /** Register module to this context. */
    void registerModule(Module* mod);

    /** Unregister module from this context. */
    void unregisterModule(Module* mod);

    const Ref<Dispatcher>& mainThreadTaskDispatcher() const { return m_mainThreadTaskDispatcher; }

    const Ref<DiagnosticsManager>& activeDiagnostics() const { return m_activeDiagnostics; }

    // TODO:
    RefObject* shaderManager = nullptr;

    const URef<detail::AssetManager>& assetManager() const { return m_assetManager; }
    const Ref<detail::PlatformManager>& platformManager() const { return m_platformManager; }
    const Ref<GraphicsManager>& graphicsManager() const { return m_graphicsManager; }

    ~EngineInstance();

private:
    EngineInstance();
    MaybeResult init(const RuntimeModuleSettings& settings);
    void dispose();
    MaybeResult initializeGraphicsManager();
    MaybeResult initializePlatformManager();

    static std::unique_ptr<EngineInstance> s_instance;

    RuntimeModuleSettings m_options;
    List<Ref<Module>> m_modules;
    Ref<Dispatcher> m_mainThreadTaskDispatcher;
    Ref<DiagnosticsManager> m_activeDiagnostics;
    URef<detail::AssetManager> m_assetManager;
    Ref<detail::PlatformManager> m_platformManager;
    Ref<GraphicsManager> m_graphicsManager;
};

} // namespace ln
