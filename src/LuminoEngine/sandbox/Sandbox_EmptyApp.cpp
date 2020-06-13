
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
#include <LuminoEngine/ImageEffect/SSRImageEffect.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
    void onInit() override
    {
        //Engine::renderView()->setGuideGridEnabled(true);
        //Engine::camera()->addComponent(CameraOrbitControlComponent::create());
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




