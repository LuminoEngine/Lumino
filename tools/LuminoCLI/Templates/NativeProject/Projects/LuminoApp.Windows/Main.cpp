
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
    //auto archive = ln::Path(ln::Path(ln::Environment::executablePath()).parent(), u"Assets.lca");
    auto archive = ln::Path::combine(ln::Path(ln::Environment::executablePath()).parent().parent().parent().parent().parent(), u".ln", u"Assets.lca");
    ln::Win32PlatformInterface::addAssetArchive(archive, u"j7OeF8Hh");
    ln::Win32PlatformInterface::initialize();
	return ln::Win32PlatformInterface::WinMain();
}

int main(int argc, char** argv)
{
	return wWinMain(0, 0, 0, 0);
}
