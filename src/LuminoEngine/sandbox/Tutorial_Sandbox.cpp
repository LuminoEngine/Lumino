#include <Lumino.hpp>

class App : public Application
{
    void onInit() override
    {
        auto box = BoxMesh::With()
            .size(1, 1, 1)
            .buildInto();

        auto camera = Engine::camera();
        camera->setPosition(5, 5, -5);
        camera->lookAt(0, 0, 0);
    }
};

//--------------------------------------------------------------------------------

void Tutorial_Sandbox()
{
    App app;
    detail::ApplicationHelper::run(&app);
}




