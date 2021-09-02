
#include <Windows.h>
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
#ifdef LN_DEBUG
    auto projectRoot = ln::Engine::findProjectLocalRoot();
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u"Assets"));
    ln::EngineSettings::addAssetDirectory(ln::Path::combine(projectRoot, u".ln", u"Assets"));
#endif

    auto archive = u"Assets.lca";
    if (ln::FileSystem::existsFile(archive)) {
        ln::EngineSettings::addAssetArchive(archive, u"");
    }

    ln::Application* app = ::LuminoCreateApplicationInstance();
    return ln::Win32PlatformInterface::WinMain(app);
}

int main(int argc, char** argv)
{
    return wWinMain(0, 0, 0, 0);
}
