
#include <LuminoExample.hpp>

class App_ShapeEntity : public Application
{
public:

    void onInit() override
    {
        auto box = BoxMesh::With()
            .size(1, 1, 1)
            .buildInto();

        auto cone = ConeMesh::With()
            .position(1, 0, 0)
            .buildInto();
    }

    void onUpdate() override
    {

    }
};

LUMINO_APP(App_ShapeEntity);



