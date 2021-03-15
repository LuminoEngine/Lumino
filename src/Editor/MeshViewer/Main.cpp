
#include "Application.hpp"

int main(int argc, char *argv[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    auto app = ln::makeObject<Application>();
    ln::detail::ApplicationHelper::run(app);
    app->dispose();
    return 0;
}
