#include "Internal.hpp"
#include <LuminoEngine/Reflection/TypeInfo.hpp>
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Engine/CoreApplication.hpp>

namespace ln {

//==============================================================================
// CoreApplication
    
LN_OBJECT_IMPLEMENT(CoreApplication, Object) {}

CoreApplication* CoreApplication::s_instance = nullptr;

CoreApplication::CoreApplication() {
    if (LN_ASSERT(!s_instance)) return;

    s_instance = this;
}

CoreApplication::~CoreApplication() {
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

void CoreApplication::configure() {
}

//bool CoreApplication::updateEngine() {
//    return true;
//}

//void CoreApplication::renderEngine() {
//}
//
//void CoreApplication::terminateEngine() {
//}
//
//Result CoreApplication::initializeInternal() {
//    return ok();
//}
//
//bool CoreApplication::updateInertnal() {
//    return updateEngine();
//}
//
//void CoreApplication::renderInertnal() {
//    renderEngine();
//}
//
//void CoreApplication::terminateInternal() {
//    terminateEngine();
//}

//==============================================================================
// AppIntegration

//Ref<CoreApplication> AppIntegration::s_app;

//Result AppIntegration::initialize(CoreApplication* app) {
//    return app->initializeInternal();
//}
//
//bool AppIntegration::update(CoreApplication* app) {
//    return app->updateInertnal();
//}
//
//void AppIntegration::render(CoreApplication* app) {
//    app->renderInertnal();
//}
//
//void AppIntegration::terminate(CoreApplication* app) {
//    app->terminateInternal();
//}

//void AppIntegration::run(ConfigureApp configureApp, InitializeEngine initializeEngine, CreateAppInstance createAppInstance) {
//    configureApp();
//
//    if (!initializeEngine()) {
//        return;
//    }
//
//    s_app = Ref<CoreApplication>(createAppInstance(), false);
//
//    initialize(s_app);
//    while (update(s_app)) {
//        render(s_app);
//    }
//    terminate(s_app);
//
//    s_app = nullptr;
//}
//
} // namespace ln
