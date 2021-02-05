
#include <Lumino.hpp>

class App : public Application
{
    virtual void onUpdate() override
    {
        if (Input::isPressed(u"left")) Debug::print(0, u"left");
        if (Input::isPressed(u"right")) Debug::print(0, u"right");
        if (Input::isPressed(u"up")) Debug::print(0, u"up");
        if (Input::isPressed(u"down")) Debug::print(0, u"down");
        if (Input::isPressed(u"submit")) Debug::print(0, u"submit");
        if (Input::isPressed(u"cancel")) Debug::print(0, u"cancel");
        if (Input::isPressed(u"menu")) Debug::print(0, u"menu");
        if (Input::isPressed(u"shift")) Debug::print(0, u"shift");
        if (Input::isPressed(u"pageup")) Debug::print(0, u"pageup");
        if (Input::isPressed(u"pagedown")) Debug::print(0, u"pagedown");
        if (Input::isPressed(u"any")) Debug::print(0, u"any");
    }
};


//--------------------------------------------------------------------------------

void Tutorial_Sandbox()
{
    App app;
    detail::ApplicationHelper::run(&app);
}




