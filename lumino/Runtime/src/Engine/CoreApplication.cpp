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

bool CoreApplication::updateEngine() {
    return true;
}

void CoreApplication::renderEngine() {
}

void CoreApplication::terminateEngine() {
}

Result CoreApplication::initializeInternal() {
    return ok();
}

bool CoreApplication::updateInertnal() {
    return updateEngine();
}

void CoreApplication::renderInertnal() {
    renderEngine();
}

void CoreApplication::terminateInternal() {
    terminateEngine();
}

//==============================================================================
// AppIntegration

Result AppIntegration::initialize(CoreApplication* app) {
    return app->initializeInternal();
}

bool AppIntegration::update(CoreApplication* app) {
    return app->updateInertnal();
}

void AppIntegration::render(CoreApplication* app) {
    app->renderInertnal();
}

void AppIntegration::terminate(CoreApplication* app) {
    app->terminateInternal();
}

void AppIntegration::run(CoreApplication* app) {
    initialize(app);
    while (update(app)) {
        render(app);
    }
    terminate(app);
}

} // namespace ln
