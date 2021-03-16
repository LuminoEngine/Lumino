
#include <Lumino.hpp>
#include <LuminoEngine/Scene/Shapes/PathShape.hpp>

class App_Sandbox_PathShape : public Application
{
public:
    App_Sandbox_PathShape()
    {
        EngineSettings::setMainWindowResizable(true);
        //EngineSettings::setDevelopmentToolsEnabled(true);
    }

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        //Engine::renderView()->setGizmoEnabled(true);
        Engine::renderView()->setBackgroundColor(Color::Gray);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        //Scene::setAntialiasEnabled(false);

        auto shape = makeObject<PathShape>();
        shape->addInto();
    }


    void onUpdate() override
    {

    }
};


//--------------------------------------------------------------------------------

void Sandbox_PathShape()
{
    App_Sandbox_PathShape app;
    detail::ApplicationHelper::run(&app);
}




