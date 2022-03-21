#pragma once
#include "Common2.hpp"
#include "../Reflection/Object.hpp"

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
    virtual bool updateEngine();
    virtual void terminateEngine();

private:
    Result initializeInternal();
    bool updateInertnal();
    void terminateInternal();

    static CoreApplication* s_instance;

    friend void ::LuminoConfigureApplication();
    friend class AppIntegration;
};

class AppIntegration {
public:
    // for external main loop (emscripten, android)
    static Result initialize(CoreApplication* app);
    static bool processTick(CoreApplication* app);
    static void terminate(CoreApplication* app);

    // for internal main loop (win32, macOS...)
    static void run(CoreApplication* app);
};

} // namespace ln

#define LN_CORE_APP ::ln::CoreApplication::instance()
