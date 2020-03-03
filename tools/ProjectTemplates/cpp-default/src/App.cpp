#include <Lumino.hpp>

class HelloApp : public Application
{
    void onStart() override
    {
        auto text = UITextBlock::create(u"Hello, Lumino!");
    }

    void onUpdate() override
    {
    }
};

LUMINO_APP(HelloApp);

