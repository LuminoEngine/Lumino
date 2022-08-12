
#include "../Core/App/Application.hpp"
#include "../Core/App/MainWindow.hpp"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	ln::Engine::setupMainWindow(ln::makeObject_deprecated<MainWindow>());
    auto app = ln::makeObject_deprecated<EditorApplication>();
    app->run();
    app->dispose();
    return 0;
}
