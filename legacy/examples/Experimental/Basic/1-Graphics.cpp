#include <Lumino.hpp>

class GraphicsBasicExample : public Application
{
    Ref<BoxMesh> box;

    void onInit() override
    {
        box = BoxMesh::With().buildInto();
    }

    void onUpdate() override
    {
        box->setRotation(0, Time::totalTime() / 5.0, 0);
    }
};

LUMINO_APP(GraphicsBasicExample);
