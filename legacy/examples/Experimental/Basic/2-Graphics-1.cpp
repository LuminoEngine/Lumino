#include <Lumino.hpp>

class App_Tutorial_Graphics_1 : public Application
{
    void onUpdate() override
    {
        Debug::print(0, String::format(_TT("X: {0}"), Mouse::position().x));
        Debug::print(0, String::format(_TT("Y: {0}"), Mouse::position().y));
    }
};

LUMINO_APP(App_Tutorial_Graphics_1);
