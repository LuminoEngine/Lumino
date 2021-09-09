
#define LUMINO_BUILD_DLL 1
#include <LuminoEngine.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>
#include <Windows.h>

// FIXME: 暫定対策
LN_FLAT_API void LNLog_PrintA(LNLogLevel level, const char* tag, const char* format, ...);

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}
#endif
