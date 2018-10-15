
#include <LuminoEngine/Platform/Win32PlatformInterface.hpp>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return ln::Win32PlatformInterface::WinMain();
}

int main(int argc, char** argv)
{
	return ln::Win32PlatformInterface::WinMain();
}
