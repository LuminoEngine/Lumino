
#include <LuminoEngine.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
#include <LuminoEngine/PostEffect/SSAOPostEffect.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
using namespace ln;

class App_Sandbox_Physics : public Application
{
    Ref<RigidBody> m_rigidBody;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        auto plane = makeObject<RigidBody>();
        plane->addCollisionShape(makeObject<PlaneCollisionShape>());


        m_rigidBody = makeObject<RigidBody>();
        m_rigidBody->addCollisionShape(makeObject<CapsuleCollisionShape>(0.5f, 2.0f));
        m_rigidBody->setMass(50.0f);
        m_rigidBody->setTransform(Matrix::makeTranslation(0, 3, 0));
    }

    void onUpdate() override
    {
        Vector3 dir;
        dir.x = -Input::getAxisValue(u"left") + Input::getAxisValue(u"right");
        dir.z = -Input::getAxisValue(u"down") + Input::getAxisValue(u"up");

        m_rigidBody->setVelocity(dir);
    }
};

void Sandbox_Physics()
{
    App_Sandbox_Physics app;
	detail::ApplicationHelper::run(&app);
}




