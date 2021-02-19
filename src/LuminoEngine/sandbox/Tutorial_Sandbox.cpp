
#include <Lumino.hpp>

class App : public Application
{
    virtual void onInit() override
    {
        auto texture = Texture2D::load(u"picture1.jpg");
        auto sprite = Sprite::With(texture).buildInto();
    }
};


//--------------------------------------------------------------------------------

void Tutorial_Sandbox()
{
    App app;
    detail::ApplicationHelper::run(&app);
}




