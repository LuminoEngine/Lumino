#include <stdio.h>
#include <LuminoEngine/RuntimeModule.hpp>
#include <LuminoEngine/RHIModule.hpp>
#include <lumino.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

LUMINO_API LNResult LNRuntime_Initialize() {
    ln::RuntimeModule::initialize();
    ln::GraphicsModule::initialize({ ln::GraphicsAPI::OpenGL });
    return LN_OK;
}

LUMINO_API void LNRuntime_Terminate() {
    ln::GraphicsModule::terminate();
    ln::RuntimeModule::terminate();
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
