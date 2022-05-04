
#include <LuminoEngine.hpp>
#include <LuminoGraphics/Rendering/InstancedMeshesModel.hpp>
#include <LuminoEngine/Scene/Mesh/InstancedMeshes.hpp>
#include <LuminoEngine/Scene/Sprite/InstancedSprites.hpp>
using namespace ln;

class App_Sandbox_InstancedMesh : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());


        auto sprites = makeObject<InstancedSprites>();
        //sprites->addInto();
        sprites->clearSprites();
        sprites->addSprite(Vector3(0, 0, 0));
        sprites->addSprite(Vector3(1, 1, 0));


        //auto mp = makeObject<MeshPrimitive>(4, 6);
        //mp->setVertex(0, Vertex(Vector3(-1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White));
        //mp->setVertex(1, Vertex(Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White));
        //mp->setVertex(2, Vertex(Vector3(1, 1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White));
        //mp->setVertex(3, Vertex(Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White));
        //mp->setIndex(0, 0);
        //mp->setIndex(1, 1);
        //mp->setIndex(2, 2);
        //mp->setIndex(3, 2);
        //mp->setIndex(4, 1);
        //mp->setIndex(5, 3);
        //mp->addSection(0, 2, 0, PrimitiveTopology::TriangleList);

        //auto mesh = makeObject<InstancedMeshes>();
        //auto c = mesh->instancedMeshesComponent();
        //mesh->addInto();

        //auto model = makeObject<InstancedMeshList>(mp, 0);
        //model->drawMesh();
        //model->setTransform(Matrix::makeTranslation(2, 1, 0));
        //model->drawMesh();
        //c->setModel(model);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_InstancedMesh()
{
    App_Sandbox_InstancedMesh app;
	detail::ApplicationHelper::run(&app);
}




