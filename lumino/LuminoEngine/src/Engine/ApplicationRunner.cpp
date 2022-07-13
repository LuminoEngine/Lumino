
#include "Internal.hpp"
#include <LuminoPlatform/ExternalProxyPlatformWindow.hpp>
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/Engine/ApplicationRunner.hpp>
#include "../UI/UIManager.hpp"
#include "EngineManager.hpp"

namespace ln {

//==============================================================================
// ApplicationRunnerBase

ApplicationRunnerBase::ApplicationRunnerBase() {
}

ApplicationRunnerBase::~ApplicationRunnerBase() {
}

Result ApplicationRunnerBase::initEngine() {
    EngineContext::current()->initializeEngineManager();
    EngineContext::current()->engineManager()->initializeAllManagers();
    return ok();
}

bool ApplicationRunnerBase::updateEngine() {
    detail::EngineDomain::engineManager()->updateFrame();
    return !detail::EngineDomain::engineManager()->isExitRequested();
}

void ApplicationRunnerBase::renderEngine(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget) {
    detail::EngineDomain::engineManager()->presentFrame(commandList, renderTarget);
}

void ApplicationRunnerBase::terminateEngine() {
    detail::EngineDomain::release();
}

//==============================================================================
// StandaloneApplicationRunner

StandaloneApplicationRunner::StandaloneApplicationRunner() {
}

Result StandaloneApplicationRunner::run(ConfigureAppFunc configureApp, CreateAppInstanceFunc createAppInstance) {
    configureApp();

    detail::EngineManager::s_settings.defaultObjectsCreation = false;
    LN_TRY(initEngine());

    m_app = Ref<Application>(createAppInstance(), false);

    auto settings = makeObject<ApplicationSetupSettings>();
    m_app->onSetup(settings);
    if (settings->m_mainWindow) {
        m_app->setupMainWindow(settings->m_mainWindow, true);
        settings->m_mainWindow->setImGuiLayerEnabled(true);
    }
    else {
        m_app->setupMainWindow(makeObject<UIMainWindow>(detail::EngineManager::s_settings.useExternalSwapChain), true);
    }

    detail::EngineDomain::uiManager()->resetApp(m_app);

    while (updateEngine()) {
        renderEngine(nullptr, nullptr);
    }

    terminateEngine();
    return ok();
}

//==============================================================================
// ExternalWindowApplicationRunner

ExternalWindowApplicationRunner::ExternalWindowApplicationRunner() {
}

Result ExternalWindowApplicationRunner::init(ConfigureAppFunc configureApp, CreateAppInstanceFunc createAppInstance) {
    configureApp();
    LN_TRY(initEngine());
    m_app = Ref<Application>(createAppInstance(), false);
    return ok();
}

bool ExternalWindowApplicationRunner::updateFrame(int width, int height) {

    auto* platformWindow = static_cast<ExternalProxyPlatformWindow*>(detail::EngineDomain::engineManager()->mainWindow()->platformWindow().get());
    platformWindow->injectSizeChanged(width, height);

    return updateEngine();
}

void ExternalWindowApplicationRunner::renderFrame(GraphicsCommandList* commandList, RenderTargetTexture* renderTarget) {

    renderEngine(commandList, renderTarget);
}

void ExternalWindowApplicationRunner::terminate() {
    terminateEngine();
    m_app = nullptr;
}

} // namespace ln
