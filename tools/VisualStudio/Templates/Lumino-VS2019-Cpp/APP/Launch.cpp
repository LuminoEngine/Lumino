
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

extern "C" ::ln::Application * LuminoCreateApplicationInstance();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(ln::Environment::currentDirectory(), u"assets"));
    
    ln::Application* app = ::LuminoCreateApplicationInstance();
    return ln::Win32PlatformInterface::WinMain(app);
}

int main(int argc, char** argv)
{
    return wWinMain(0, 0, 0, 0);
}
