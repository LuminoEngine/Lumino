
#include <LuminoEngine.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
using namespace ln;


class App_Experiment_MeshCollider : public Application
{
    Ref<RigidBodyComponent> m_boxBody;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        //Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

        auto staticMesh = StaticMesh::load(u"C:/Proj/LN/PrivateProjects/HC4/assets/Map/Map-1.glb");
        auto model = staticMesh->staticMeshComponent()->model();

        if (auto node = model->findNode(u"Lumino.MeshCollider")) {
            int index = node->meshContainerIndex();
            if (index >= 0) {
                auto meshContainer = model->meshContainers()[index];
                auto mesh = meshContainer->meshPrimitives()[0];
                auto shape = MeshCollisionShape::create(mesh);
                auto body = RigidBodyComponent::create();
                body->addCollisionShape(shape);
                staticMesh->addComponent(body);
            }
        }

        auto shape = CapsuleCollisionShape::create(0.5, 2);
        auto box = BoxMesh::With().build();
        m_boxBody = RigidBodyComponent::create();
        m_boxBody->setMass(10);
        m_boxBody->addCollisionShape(shape);
        m_boxBody->rigidBody()->setAngularLimits(RigidBodyLimitFlags::LockedRotation);
        box->addComponent(m_boxBody);
        box->setPosition(0, 5, 0);
	}

    void onUpdate() override
    {
        auto v = m_boxBody->velocity();
        v.x = -Input::getAxisValue(u"left") + Input::getAxisValue(u"right");
        v.z = -Input::getAxisValue(u"down") + Input::getAxisValue(u"up");
        m_boxBody->setVelocity(v);
    }
};

void Experiment_MeshCollider()
{
    App_Experiment_MeshCollider app;
	detail::ApplicationHelper::run(&app);
}




