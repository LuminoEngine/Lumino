#pragma once
#include <lumino.h>
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
    LNGraphicsBackend graphicsAPI = LN_GRAPHICS_BACKEND_OPENGL;
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
    static MaybeResult_deprecated initialize(const RuntimeModuleSettings& settings, EngineInstance* sharedContext = nullptr);

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
    const Ref<GraphicsManager>& graphicsManager() const { return m_graphicsManager; }
    const Ref<detail::PlatformManager>& platformManager() const { return m_platformManager; }

    ~EngineInstance();

private:
    EngineInstance();
    MaybeResult_deprecated init(const RuntimeModuleSettings& settings);
    void dispose();
    MaybeResult_deprecated initializeGraphicsManager();
    MaybeResult_deprecated initializePlatformManager();

    static std::unique_ptr<EngineInstance> s_instance;

    RuntimeModuleSettings m_options;
    List<Ref<Module>> m_modules;
    Ref<Dispatcher> m_mainThreadTaskDispatcher;
    Ref<DiagnosticsManager> m_activeDiagnostics;
    URef<detail::AssetManager> m_assetManager;
    Ref<GraphicsManager> m_graphicsManager;
    Ref<detail::PlatformManager> m_platformManager;
};

} // namespace ln
