
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());


        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->setPosition(5, 5, -5);
        Engine::camera()->lookAt(0, 0, 0);

        auto box1 = BoxMesh::create();
        box1->setPosition(1, 0, 0);

        auto box2 = BoxMesh::create();
        box2->setPosition(0, 2, 0);

        auto box3 = BoxMesh::create();
        box3->setPosition(0, 0, 3);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_EmptyApp()
{
    App_Sandbox_EmptyApp app;
	detail::ApplicationHelper::run(&app);
}




