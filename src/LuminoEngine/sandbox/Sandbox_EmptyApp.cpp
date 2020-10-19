
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
using namespace ln;

class App_Sandbox_EmptyApp : public Application
{
public:
    App_Sandbox_EmptyApp()
    {
        EngineSettings::addAssetArchive("C:/Proj/LN/PrivateProjects/HC4/.ln/Assets.lca", "n7OeL8Hh");
    }

    void onInit() override
    {
        //Engine::renderView()->setGuideGridEnabled(true);
        //Engine::camera()->addComponent(CameraOrbitControlComponent::create());


        auto mesh = ln::StaticMesh::load(u"Map/MainMap1-Start.gltf");
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




