#include <Lumino.hpp>

class EntityBasicExample : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->setPosition(5, 5, -5);
        Engine::mainCamera()->lookAt(0, 0, 0);

        auto box1 = BoxMesh::With().buildInto();
        box1->setPosition(1, 0, 0);

        auto box2 = BoxMesh::With().buildInto();
        box2->setPosition(0, 2, 0);

        auto box3 = BoxMesh::With().buildInto();
        box3->setPosition(0, 0, 3);
    }
};

LUMINO_APP(EntityBasicExample);
