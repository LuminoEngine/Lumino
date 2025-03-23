#include <stdio.h>
#include <vector>
#include <lumino.h>

int main() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    if (LNRuntime_Initialize() != LN_OK) {
        return 1;
    }


    




    //while (!glfwWindowShouldClose(window)) {
    //}

    LNRuntime_Terminate();
    return 0;
}
