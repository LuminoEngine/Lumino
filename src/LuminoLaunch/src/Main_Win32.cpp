
#ifdef _WIN32

#include <windows.h>

static ln::Application* g_app = nullptr;

extern "C" ::ln::Application* LuminoCreateApplicationInstance();

//int APIENTRY wWinMain(
//	_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPWSTR    lpCmdLine,
//	_In_ int       nCmdShow)

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)

{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_app = ::LuminoCreateApplicationInstance();

	ln::detail::ApplicationHelper::initialize(g_app);
	ln::detail::ApplicationHelper::run(g_app);
	ln::detail::ApplicationHelper::finalize(g_app);
	ln::RefObjectHelper::release(g_app);
	g_app = nullptr;

	return 0;
}


#endif
