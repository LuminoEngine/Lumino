
#include <Lumino.hpp>

class App : public Application
{
    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setGizmoEnabled(true);
        Engine::renderView()->setBackgroundColor(Color::Gray);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Scene::setAntialiasEnabled(false);

        auto texture = Texture2D::load(u"picture1.jpg");
        auto sprite = Sprite::With(texture).buildInto();
        sprite->setCullMode(CullMode::None);
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




