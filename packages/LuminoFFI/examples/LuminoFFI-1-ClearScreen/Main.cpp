#include <stdio.h>
#include <vector>
#include <lumino.h>

int main() {
    if (LNRuntime_Initialize() != LN_OK) {
        return 1;
    }

    LNHandle window = LN_NULL_HANDLE;
    if (LNWindow_Create(640, 480, "Example", &window) != LN_OK) {
        return 1;
    }

    while (true) {
        LNBool quit = LN_FALSE;
        LNInstance_ShouldQuit(&quit);
        if (quit) {
            break;
        }
        LNInstance_ProcessEvents();
    };

    LNObject_Release(window);
    LNRuntime_Terminate();
    return 0;
}
