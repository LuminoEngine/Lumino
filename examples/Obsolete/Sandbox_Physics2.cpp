﻿
#include <LuminoEngine.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoEngine/PostEffect/SSRPostEffect.hpp>
#include <LuminoEngine/PostEffect/SSAOPostEffect.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldPostEffect.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
using namespace ln;

class App_Sandbox_Physics2 : public Application
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


        //m_rigidBody = makeObject<RigidBody>();
        //m_rigidBody->addCollisionShape(makeObject<CapsuleCollisionShape>(0.5f, 2.0f));
        //m_rigidBody->setMass(50.0f);
        //m_rigidBody->setTransform(Matrix::makeTranslation(0, 3, 0));

        m_box = BoxMesh::create();
        m_box->setPosition(0, 5, 0);
        m_box->setScale(1, 1, 0.1);

        auto body = RigidBodyComponent::create();
        body->addCollisionShape(makeObject<BoxCollisionShape>(1, 1, 0.1));
        body->setMass(10.0);
        m_box->addComponent(body);
    }

    void onUpdate() override
    {


    }
};

void Sandbox_Physics2()
{
    App_Sandbox_Physics2 app;
	detail::ApplicationHelper::run(&app);
}




