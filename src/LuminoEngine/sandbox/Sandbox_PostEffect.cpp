
#include <LuminoEngine.hpp>
#include <LuminoEngine/ImageEffect/TonemapImageEffect.hpp>
using namespace ln;

class App_Sandbox_PostEffect : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        auto mesh = StaticMesh::create(u"D:/Tech/Tools/cornellBox/BJS-2.79-Cycles-gltf/assets/cornellBox-2.79-Cycles-gltf.bin");


        auto effect1 = makeObject<TonemapImageEffect>();
        Engine::renderView()->addImageEffect(effect1);

    }

    void onUpdate() override
    {
    }
};

void Sandbox_PostEffect()
{
    App_Sandbox_PostEffect app;
	detail::ApplicationHelper::run(&app);
}




