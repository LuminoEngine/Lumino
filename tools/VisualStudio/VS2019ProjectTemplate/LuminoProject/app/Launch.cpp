
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void Main();

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Main();

    ln::Engine::finalize();

    return 0;
}

int main(int argc, const char** argv)
{
    return wWinMain(0, 0, 0, 0);
}
