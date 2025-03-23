#pragma once
#include <LuminoCore/Runtime/TypeInfo.hpp>
#include <LuminoCore/Runtime/Property.hpp>
#include "Common2.hpp"
#include "../Asset/Common.hpp"

namespace ln {
struct RuntimeModuleSettings {
    AssetStorageAccessPriority assetStorageAccessPriority = AssetStorageAccessPriority::DirectoryFirst;
};
namespace detail {
class RuntimeManager;
class AssetManager;
} // namespace detail

class EngineManager {
public:
    static EngineManager* instance() { return s_instance.get(); };

    /** Initialize context. */
    static MaybeResult initialize(const RuntimeModuleSettings& settings, EngineManager* sharedContext = nullptr);

    /** Terminate context. */
    static void terminate();

    /** Register module to this context. */
    void registerModule(Module* mod);

    /** Unregister module from this context. */
    void unregisterModule(Module* mod);

    const Ref<Dispatcher>& mainThreadTaskDispatcher() const { return m_mainThreadTaskDispatcher; }

    const Ref<DiagnosticsManager>& activeDiagnostics() const { return m_activeDiagnostics; }

    // TODO:
    RefObject* platformManager = nullptr;
    RefObject* shaderManager = nullptr;
    RefObject* graphicsManager = nullptr;

    const URef<detail::AssetManager>& assetManager() const { return m_assetManager; }

    ~EngineManager();

private:
    EngineManager();
    MaybeResult init(const RuntimeModuleSettings& settings);
    void dispose();

    static std::unique_ptr<EngineManager> s_instance;

    List<Ref<Module>> m_modules;
    Ref<Dispatcher> m_mainThreadTaskDispatcher;
    Ref<DiagnosticsManager> m_activeDiagnostics;
    URef<detail::AssetManager> m_assetManager;
};

} // namespace ln
