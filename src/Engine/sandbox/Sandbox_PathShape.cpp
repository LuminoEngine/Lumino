﻿
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

        shape->beginCanvas();
        shape->beginPath();
        shape->setFillColor(Color::Red);
        shape->setStrokeColor(Color::White);
        shape->setStrokeWidth(0.05);
        shape->moveTo(0, 0);
        shape->lineTo(1, 1);
        shape->lineTo(0, 3);
        shape->lineTo(-1, 1);
        shape->closePath();
        shape->fill();
        shape->stroke();
        shape->endCanvas(PathPlacementSpace::XZ);
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




