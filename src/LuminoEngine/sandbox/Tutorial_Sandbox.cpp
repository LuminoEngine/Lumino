
#include <Lumino.hpp>

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
        //Engine::renderView()->setGuideGridEnabled(true);
        //Engine::renderView()->setGizmoEnabled(true);
        //Engine::renderView()->setBackgroundColor(Color::Gray);
        //Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        //Scene::setAntialiasEnabled(false);

        //auto texture = Texture2D::load(u"picture1.jpg");
        ////auto sprite = Sprite::With(texture).buildInto();
        ////sprite->setCullMode(CullMode::None);
        //auto sprite = UISprite::create(texture);
        //sprite->addInto();

        //auto box = BoxMesh::With().buildInto();


        auto text1 = UIText::With(u"Hello ------ Aa!").buildInto();
    }


    void onUpdate() override
    {

    }
};


//--------------------------------------------------------------------------------

void Tutorial_Sandbox()
{
    App app;
    detail::ApplicationHelper::run(&app);
}




