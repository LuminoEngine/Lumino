
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	auto archive = u"Assets.lca";
	if (ln::FileSystem::existsFile(archive)) {
		ln::Win32PlatformInterface::addAssetArchive(archive);
	}

    ln::Win32PlatformInterface::initialize();
	return ln::Win32PlatformInterface::WinMain();
}

int main(int argc, char** argv)
{
	return wWinMain(0, 0, 0, 0);
}
