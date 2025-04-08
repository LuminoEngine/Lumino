#include "Internal.hpp"
#include <LuminoEngine/Base/Task.hpp>
#include <LuminoEngine/Base/Fetch.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Engine/EngineInstance.hpp>
#include <LuminoEngine/Asset/detail/AssetManager.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/Font/detail/FontManager.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "../Audio/AudioManager.hpp"
#include <LuminoEngine/Audio/GameAudio.hpp>

namespace ln {

void registerModuleTypes_Runtime(RuntimeContext* context);

std::unique_ptr<EngineInstance> EngineInstance::s_instance;

MaybeResult EngineInstance::initialize(const RuntimeModuleSettings& settings, EngineInstance* sharedContext) {
    if (sharedContext) {
        LN_NOTIMPLEMENTED();
        return LN_MAKE_ERROR();
    }

    if (s_instance) return LN_MAKE_SUCCESS();
    s_instance = std::unique_ptr<EngineInstance>(LN_NEW EngineInstance());
    return s_instance->init(settings);
}

void EngineInstance::terminate() {
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

EngineInstance::EngineInstance() {
}

EngineInstance::~EngineInstance() {
}

MaybeResult EngineInstance::init(const RuntimeModuleSettings& settings) {
    m_options = settings;

#ifdef LN_DEBUG
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif

#ifdef LN_EMSCRIPTEN
#else
    TaskScheduler::init();
    m_mainThreadTaskDispatcher = makeRef<Dispatcher>();
#endif

    {
        m_activeDiagnostics = makeObject_deprecated<DiagnosticsManager>();
        ProfilingItem::Graphics_RenderPassCount = makeObject_deprecated<ProfilingItem>(ProfilingItemType::Counter, _TT("RenderPass count"));
        m_activeDiagnostics->registerProfilingItem(ProfilingItem::Graphics_RenderPassCount);
    }

    {
        auto result = initializeRuntimeManager();
        if (!result) return result;
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

    {
        auto result = initializeGraphicsManager();
        if (!result) return result;
    }
    {
        auto result = initializePlatformManager();
        if (!result) return result;
    }


    return LN_MAKE_SUCCESS();
}

void EngineInstance::dispose() {
    if (m_platformManager) {
        m_platformManager->dispose();
        m_platformManager = nullptr;
    }
    detail::RenderingManager::terminate();
    detail::FontManager::terminate();
    if (m_graphicsManager) {
        m_graphicsManager->dispose();
        m_graphicsManager = nullptr;
    }
    if (m_assetManager) {
        m_assetManager->dispose();
        m_assetManager = nullptr;
    }
    if (m_runtimeManager) {
        m_runtimeManager->dispose();
        m_runtimeManager = nullptr;
    }

#ifdef LN_EMSCRIPTEN
#else
    if (m_mainThreadTaskDispatcher) {
        m_mainThreadTaskDispatcher->dispose();
        m_mainThreadTaskDispatcher = nullptr;
    }
    TaskScheduler::finalizeInternal();
#endif
}

MaybeResult EngineInstance::initializeRuntimeManager() {
    if (m_runtimeManager) return LN_MAKE_SUCCESS();
    detail::RuntimeManager::Options options = {};
    URef<detail::RuntimeManager> manager(LN_NEW detail::RuntimeManager());
    auto result = manager->init(options);
    if (!result) return result;
    m_runtimeManager = std::move(manager);
    return LN_MAKE_SUCCESS();
}

MaybeResult EngineInstance::initializeGraphicsManager() {
    if (m_graphicsManager) return LN_MAKE_SUCCESS();

    LNGraphicsBackend graphicsAPI = m_options.graphicsAPI;
    WindowSystem windowSystem = m_options.windowSystem;
    GraphicsManager::selectDefaultSystem(&graphicsAPI, &windowSystem);

    GraphicsManager::Settings options;
    options.graphicsAPI = graphicsAPI;
#ifdef LN_DEBUG
    options.debugMode = true;
#endif
    Ref<GraphicsManager> manager(LN_NEW GraphicsManager(), false);
    auto result = manager->init(options);
    if (!result) {
        return result;
    }
    m_graphicsManager = manager;


    // TODO: 後で移動する
    {
        detail::FontManager::Settings settings;
        settings.assetManager = detail::AssetManager::instance();
        if (!detail::FontManager::initialize(settings)) {
            return LN_MAKE_ERROR();
        }
    }
    {
        detail::RenderingManager::Settings settings;
        settings.graphicsManager = m_graphicsManager;
        settings.fontManager = detail::FontManager::instance();
        if (!detail::RenderingManager::initialize(settings)) {
            return LN_MAKE_ERROR();
        }
    }

    return LN_MAKE_SUCCESS();
}

MaybeResult EngineInstance::initializePlatformManager() {
    if (m_platformManager) return LN_MAKE_SUCCESS();

    detail::PlatformManager::Settings options;
    options.windowSystem = m_options.windowSystem;
    Ref<detail::PlatformManager> manager(
        LN_NEW detail::PlatformManager(m_graphicsManager, detail::RenderingManager::instance()), false);
    auto result = manager->init(options);
    if (!result) {
        return result;
    }

    m_platformManager = manager;
    return LN_MAKE_SUCCESS();
}

void EngineInstance::registerModule(Module* mod) {
    if (LN_REQUIRE(!mod->m_context)) return;
    mod->m_context = this;
    mod->onRegisterTypes(this);
    m_modules.add(mod);
}

void EngineInstance::unregisterModule(Module* mod) {
    m_modules.removeIf([&](const auto& x) { return x == mod; });
}

} // namespace ln
