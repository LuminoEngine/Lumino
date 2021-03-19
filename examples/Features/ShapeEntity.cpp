
#include <LuminoExample.hpp>

class App_ShapeEntity : public Application
{
public:

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setGizmoEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

        //auto box = BoxMesh::With()
        //    .size(1, 1, 1)
        //    .buildInto();

        //auto cone = ConeMesh::With()
        //    .position(1, 0, 0)
        //    .buildInto();

        auto cylinder = CylinderMesh::With()
            .position(2, 0, 0)
            .buildInto();
    }

    void onUpdate() override
    {

    }
};

LUMINO_APP(App_ShapeEntity);



