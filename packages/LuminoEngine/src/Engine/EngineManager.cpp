
#include "Internal.hpp"
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Base/Fetch.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Engine/EngineManager.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include "../Audio/AudioManager.hpp"
#include <LuminoEngine/Audio/GameAudio.hpp>

namespace ln {

void registerModuleTypes_Runtime(RuntimeContext* context);

std::unique_ptr<EngineManager> EngineManager::s_instance;

MaybeResult EngineManager::initialize(const RuntimeModuleSettings& settings, EngineManager* sharedContext) {
    if (sharedContext) {
        LN_NOTIMPLEMENTED();
        return LN_MAKE_ERROR();
    }

    if (s_instance) return LN_MAKE_SUCCESS();
    s_instance = std::unique_ptr<EngineManager>(LN_NEW EngineManager());
    return s_instance->init(settings);
}

void EngineManager::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

EngineManager::EngineManager() {
}

EngineManager::~EngineManager() {
}

MaybeResult EngineManager::init(const RuntimeModuleSettings& settings) {

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
        detail::RuntimeManager::Settings opt;
        auto result = detail::RuntimeManager::initialize(opt);
        if (!result) {
            return result;
        }
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


#if 0 // test
    { 
        detail::AudioManager::Settings opt;
        opt.assetManager = m_assetManager.get();
        auto result = detail::AudioManager::initialize(opt);
        if (!result) {
            return result;
        }

        GameAudio::playBGM(U"D:\\Music\\momentum\\02 - momentum.wav");
        // detail::AudioManager::instance()->gameAudio2()->playBGM(U"Assets/SE/SE_001.wav");
    }
#endif


    return LN_MAKE_SUCCESS();
}

void EngineManager::dispose() {
    if (m_assetManager) {
        m_assetManager->dispose();
        m_assetManager = nullptr;
    }

    detail::RuntimeManager::terminate();

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

void EngineManager::registerModule(Module* mod) {
    if (LN_REQUIRE(!mod->m_context)) return;
    mod->m_context = this;
    mod->onRegisterTypes(this);
    m_modules.add(mod);
}

void EngineManager::unregisterModule(Module* mod) {
    m_modules.removeIf([&](const auto& x) { return x == mod; });
}

} // namespace ln
