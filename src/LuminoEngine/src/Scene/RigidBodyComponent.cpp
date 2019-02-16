
#include "Internal.hpp"
#include <LuminoEngine/Scene/RigidBodyComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "Engine/EngineManager.hpp"

namespace ln {

//=============================================================================
// RigidBody2DComponent

RigidBody2DComponent::RigidBody2DComponent()
{
}

void RigidBody2DComponent::init()
{
	Component::init();
	m_body = newObject<RigidBody2D>();
    m_body->setEventListener(this);
    detail::EngineDomain::engineManager()->mainPhysicsWorld2D()->addPhysicsObject(m_body);
}

void RigidBody2DComponent::setMass(float value)
{
    m_body->setMass(value);
}

void RigidBody2DComponent::addCollisionShape(CollisionShape2D* shape)
{
    m_body->addCollisionShape(shape);
}

void RigidBody2DComponent::onAttachedWorld(World* newOwner)
{
}

void RigidBody2DComponent::onDetachedWorld(World* oldOwner)
{
}

void RigidBody2DComponent::onBeforeStepSimulation()
{
    m_body->setPosition(worldObject()->position());
}

void RigidBody2DComponent::onAfterStepSimulation()
{
    if (m_body->isDynamic()) {
        float z = worldObject()->position().z;
        worldObject()->setPosition(Vector3(m_body->position(), z));
    }
}

} // namespace ln

