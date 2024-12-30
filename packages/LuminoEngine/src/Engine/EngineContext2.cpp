
#include "Internal.hpp"
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Base/Fetch.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>

namespace ln {

void registerModuleTypes_Runtime(RuntimeContext* context);

std::unique_ptr<EngineContext2> EngineContext2::s_instance;

MaybeResult EngineContext2::initialize(const RuntimeModuleSettings& settings, EngineContext2* sharedContext) {
    if (sharedContext) {
        LN_NOTIMPLEMENTED();
        return LN_MAKE_ERROR();
    }

    if (s_instance) return LN_MAKE_SUCCESS();
    s_instance = std::unique_ptr<EngineContext2>(LN_NEW EngineContext2());
    return s_instance->init(settings);
}

void EngineContext2::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

EngineContext2::EngineContext2() {
}

EngineContext2::~EngineContext2() {
}

MaybeResult EngineContext2::init(const RuntimeModuleSettings& settings) {

#ifdef LN_EMSCRIPTEN
#else
    TaskScheduler::init();
    m_mainThreadTaskDispatcher = makeRef<Dispatcher>();
#endif
    //detail::FetchManager::initialize();

    {
        m_activeDiagnostics = makeObject_deprecated<DiagnosticsManager>();
        ProfilingItem::Graphics_RenderPassCount = makeObject_deprecated<ProfilingItem>(ProfilingItemType::Counter, _TT("RenderPass count"));
        m_activeDiagnostics->registerProfilingItem(ProfilingItem::Graphics_RenderPassCount);
    }

    {
        detail::AssetManager::Settings settings2;
        settings2.assetStorageAccessPriority = settings.assetStorageAccessPriority;
        m_assetManager = makeURef<detail::AssetManager>();
        if (!m_assetManager->init(settings2)) {
            return LN_MAKE_ERROR();
        }
    }



    // Register types
    registerModuleTypes_Runtime(RuntimeContext::current());

    
    m_runtimeManager = makeURef<detail::RuntimeManager>();
    m_runtimeManager->init(detail::RuntimeManager::Settings());

    return LN_MAKE_SUCCESS();
}

void EngineContext2::dispose() {
    if (m_runtimeManager) {
        m_runtimeManager->dispose();
        m_runtimeManager = nullptr;
    }

    if (m_assetManager) {
        m_assetManager->dispose();
        m_assetManager = nullptr;
    }

    //detail::FetchManager::terminate();

#ifdef LN_EMSCRIPTEN
#else
    if (m_mainThreadTaskDispatcher) {
        m_mainThreadTaskDispatcher->dispose();
        m_mainThreadTaskDispatcher = nullptr;
    }
    TaskScheduler::finalizeInternal();
#endif
}

void EngineContext2::registerModule(Module* mod) {
    if (LN_REQUIRE(!mod->m_context)) return;
    mod->m_context = this;
    mod->onRegisterTypes(this);
    m_modules.add(mod);
}

void EngineContext2::unregisterModule(Module* mod) {
    m_modules.removeIf([&](const auto& x) { return x == mod; });
}

} // namespace ln
