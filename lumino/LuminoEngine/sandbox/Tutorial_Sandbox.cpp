
#include <Lumino.hpp>
//#include "C:/Proj/LN/Lumino/_build/tools/Optick_1.3.1/include/optick.h"
//#pragma comment(lib, "C:/Proj/LN/Lumino/_build/tools/Optick_1.3.1/lib/x64/debug/OptickCore.lib")

class App : public Application
{
public:
    App()
    {
        EngineSettings::setMainWindowResizable(true);
        //EngineSettings::setDevelopmentToolsEnabled(true);
    }

    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        //Engine::renderView()->setGizmoEnabled(true);
        //Engine::renderView()->setBackgroundColor(Color::Gray);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        //Scene::setAntialiasEnabled(false);

        //auto texture = Texture2D::load(u"picture1.jpg");
        ////auto sprite = Sprite::With(texture).buildInto();
        ////sprite->setCullMode(CullMode::None);
        //auto sprite = UISprite::create(texture);
        //sprite->addInto();

        //auto light = Engine::mainLight();
        //auto box = BoxMesh::With().buildInto();
        //box->setMaterial(Material::With()
        //    .color(Color::White)
        //    .roughness(0.0f)
        //    .metallic(1.0f)
        //    .build());


        //light->setPosition(0, 0, 10);
        //light->lookAt(0, 0, 0);
        //light->setAmbientColor(Color::Red);

        auto text1 = UIText::With(u"Hello ------ Aa!").buildInto();

        //auto sh = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/MToon.hlsl");
    }


    void onUpdate() override
    {
        //OPTICK_FRAME("MainThread");

    }
};


//--------------------------------------------------------------------------------

void Tutorial_Sandbox()
{
    App app;
    detail::ApplicationHelper::run(&app);
}




