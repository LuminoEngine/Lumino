
#include "LuminoExample.hpp"

class App_Tutorial_Sandbox : public Application
{
public:
    App_Tutorial_Sandbox()
    {
        EngineSettings::setMainWindowResizable(true);
        //EngineSettings::setDevelopmentToolsEnabled(true);
    }

    virtual void onInit() override
    {
        //Engine::renderView()->setGuideGridEnabled(true);
        //Engine::renderView()->setGizmoEnabled(true);
        Engine::renderView()->setBackgroundColor(Color::Gray);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        //Scene::setAntialiasEnabled(false);

        //auto texture = Texture2D::load(u"picture1.jpg");
        ////auto sprite = Sprite::With(texture).buildInto();
        ////sprite->setCullMode(CullMode::None);
        //auto sprite = UISprite::create(texture);
        //sprite->addInto();

        auto box = BoxMesh::With().buildInto();
        //box->setMaterial(Material::With()
        //    .color(Color::White)
        //    .roughness(0.0f)
        //    .metallic(1.0f)
        //    .build());

        auto light = Engine::mainLight();

        light->setPosition(0, 0, 10);
        light->lookAt(0, 0, 0);
        light->setAmbientColor(Color::Red);

        //auto text1 = UIText::With(u"Hello ------ Aa!").buildInto();

        //auto sh = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/MToon.hlsl");
    }


    void onUpdate() override
    {

    }
};

LUMINO_APP(App_Tutorial_Sandbox);



