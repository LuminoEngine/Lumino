
#include <LuminoEngine.hpp>
#include <LuminoGraphics/Mesh/MeshProcessing.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
#include <LuminoEngine/PostEffect/SSAOPostEffect.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
using namespace ln;

class App_Sandbox_Physics : public Application
{
    Ref<BoxMesh> m_box;
    Ref<RigidBody> m_rigidBody;

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        auto plane = makeObject<RigidBody>();
        plane->addCollisionShape(makeObject<PlaneCollisionShape>());


        m_rigidBody = makeObject<RigidBody>();
        m_rigidBody->addCollisionShape(makeObject<CapsuleCollisionShape>(0.5f, 2.0f));
        m_rigidBody->setMass(50.0f);
        m_rigidBody->setTransform(Matrix::makeTranslation(0, 3, 0));

        m_box = BoxMesh::With().build();
    }

    void onUpdate() override
    {

        Vector3 dir;
        dir.x = -Input::getAxisValue(_TT("left")) + Input::getAxisValue(_TT("right"));
        dir.z = -Input::getAxisValue(_TT("down")) + Input::getAxisValue(_TT("up"));

        Camera* camera = Engine::mainCamera();

        if (0) {
            // 遅延しない
            const float velocity = 5.0f /60;
            const auto pos = m_box->position();
            m_box->setPosition(pos + dir * velocity);

            camera->setPosition(m_box->position() + Vector3(0, 0, -10));
        }
        else if (1) {
            // 定期的に一瞬遅延する
            const float velocity = 5.0f / 60;
            const auto pos = m_rigidBody->transform().position();
            m_rigidBody->setTransform(Matrix::makeTranslation(pos + dir * velocity));

            camera->setPosition(m_rigidBody->transform().position() + Vector3(0, 0, -10));
        }
        else {
            // 定期的に遅延する
            const float velocity = 5.0f;
            m_rigidBody->setVelocity(dir * velocity);

            camera->setPosition(m_rigidBody->transform().position() + Vector3(0, 0, -10));
            //camera->lookAt(m_rigidBody->transform().position());

            //printf("%f\n", m_rigidBody->transform().position().x);
        }


    }
};

void Sandbox_Physics()
{
    App_Sandbox_Physics app;
	detail::ApplicationHelper::run(&app);
}




