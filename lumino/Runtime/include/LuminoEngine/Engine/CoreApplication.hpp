#pragma once
#include "Common2.hpp"
#include <LuminoCore/Runtime/Object.hpp>

extern "C" void LuminoConfigureApplication();

namespace ln {
class AppIntegration;
    
/** CoreApplication */
LN_CLASS()
class CoreApplication : public Object {
    LN_OBJECT;
public:
    static CoreApplication* instance() { return s_instance; }

    CoreApplication();
    virtual ~CoreApplication();

protected:
    static void configure();
    //virtual bool updateEngine();
    //virtual void renderEngine();
    //virtual void terminateEngine();

private:
    //Result initializeInternal();
    //bool updateInertnal();
    //void renderInertnal();
    //void terminateInternal();

    static CoreApplication* s_instance;

    friend void ::LuminoConfigureApplication();
    friend class AppIntegration;
};

//class AppIntegration {
//public:
//    typedef void (*ConfigureApp)();
//    typedef bool (*InitializeEngine)();
//    typedef CoreApplication* (*CreateAppInstance)();
//
//    // for external main loop (emscripten, android)
//    //static Result initialize(CoreApplication* app);
//    //static bool update(CoreApplication* app);
//    //static void render(CoreApplication* app);
//    //static void terminate(CoreApplication* app);
//
//    // for internal main loop (win32, macOS...)
//    //static void run(ConfigureApp configureApp, InitializeEngine initializeEngine, CreateAppInstance createAppInstance);
//
//private:
//    static Ref<CoreApplication> s_app;
//};

} // namespace ln

#define LN_CORE_APP ::ln::CoreApplication::instance()
