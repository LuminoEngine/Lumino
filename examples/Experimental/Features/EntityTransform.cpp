
#include <Lumino.hpp>

class Features_EntityTransform : public Application
{
    Ref<BoxMesh> box;
    Ref<TeapotMesh> teapot;

    void onInit() override
    {
        box = BoxMesh::With().buildInto();
        teapot = TeapotMesh::With().buildInto();

        Engine::mainCamera()->setPosition(10, 10, 10);
        Engine::mainCamera()->lookAt(0, 0, 0);
    }

    void onUpdate() override
    {
        float x = sin(Time::totalTime()) * 5.0;
        float z = cos(Time::totalTime()) * 5.0;
        box->setPosition(x, 0.0, z);


        teapot->lookAt(x, 0.0, z);

        //box->setRotation(0, Time::totalTime() / 2.0, 0);
    }
};

LUMINO_APP(Features_EntityTransform);

