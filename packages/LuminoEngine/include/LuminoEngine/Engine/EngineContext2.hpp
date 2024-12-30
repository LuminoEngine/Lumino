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

class EngineContext2 {
public:
    static EngineContext2* instance() { return s_instance.get(); };

    /** Initialize context. */
    static MaybeResult initialize(const RuntimeModuleSettings& settings, EngineContext2* sharedContext = nullptr);

    /** Terminate context. */
    static void terminate();

    /** Register module to this context. */
    void registerModule(Module* mod);

    /** Unregister module from this context. */
    void unregisterModule(Module* mod);

    const Ref<Dispatcher>& mainThreadTaskDispatcher() const { return m_mainThreadTaskDispatcher; }

    const Ref<DiagnosticsManager>& activeDiagnostics() const { return m_activeDiagnostics; }

    detail::RuntimeManager* runtimeManager() const { return m_runtimeManager.get(); }


    // TODO:
    RefObject* platformManager = nullptr;
    RefObject* shaderManager = nullptr;
    RefObject* graphicsManager = nullptr;

    const URef<detail::AssetManager>& assetManager() const { return m_assetManager; }

    ~EngineContext2();

private:
    EngineContext2();
    MaybeResult init(const RuntimeModuleSettings& settings);
    void dispose();

    static std::unique_ptr<EngineContext2> s_instance;

    List<Ref<Module>> m_modules;
    Ref<Dispatcher> m_mainThreadTaskDispatcher;
    Ref<DiagnosticsManager> m_activeDiagnostics;
    URef<detail::RuntimeManager> m_runtimeManager;
    URef<detail::AssetManager> m_assetManager;
};

} // namespace ln
