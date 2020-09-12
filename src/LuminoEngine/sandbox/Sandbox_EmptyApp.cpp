
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
    void onInit() override
    {
        //Engine::renderView()->setGuideGridEnabled(true);
        //Engine::camera()->addComponent(CameraOrbitControlComponent::create());


        auto mesh = ln::SkinnedMesh::load(u"D:/Documents/Modeling/HC4-10.glb");
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




