#include <Lumino.hpp>

class App_Tutorial_Graphics_1 : public Application
{
    void onUpdate() override
    {
        Debug::print(0, String::format(u"X: {0}", Mouse::position().x));
        Debug::print(0, String::format(u"Y: {0}", Mouse::position().y));
    }
};

LUMINO_APP(App_Tutorial_Graphics_1);
