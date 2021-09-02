#include <Lumino.hpp>

class HelloExample : public Application
{
    void onInit() override
    {
        auto text = UIText::With(u"Hello, Lumino!").buildInto();
    }

    void onUpdate() override
    {
    }
};

LUMINO_APP(HelloExample);
