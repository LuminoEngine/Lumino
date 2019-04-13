
#include "Internal.hpp"
#include <LuminoEngine/Scene/RigidBodyComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "Engine/EngineManager.hpp"

namespace ln {

//=============================================================================
// Collision

Collision::Collision()
    : m_worldObject(nullptr)
{
}

void Collision::init(WorldObject* worldObject/*, RigidBody2DComponent* component*/)
{
    Object::init();
    m_worldObject = worldObject;
}

//=============================================================================
// RigidBody2DComponent

Ref<RigidBody2DComponent> RigidBody2DComponent::create()
{
	return newObject<RigidBody2DComponent>();
}

RigidBody2DComponent::RigidBody2DComponent()
{
}

void RigidBody2DComponent::init()
{
	Component::init();
	m_body = newObject<RigidBody2D>();
    m_body->setEventListener(this);
    m_body->setOwnerData(this);
    detail::EngineDomain::engineManager()->mainPhysicsWorld2D()->addPhysicsObject(m_body);
}

void RigidBody2DComponent::onDispose(bool explicitDisposing)
{
	if (m_body) {
		m_body->setEventListener(nullptr);
		m_body->removeFromPhysicsWorld();
		m_body = nullptr;
	}

	Component::onDispose(explicitDisposing);
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

EventConnection RigidBody2DComponent::connectOnCollisionEnter(CollisionEventHandler handler)
{
    return m_onCollisionEnter.connect(handler);
}

EventConnection RigidBody2DComponent::connectOnCollisionLeave(CollisionEventHandler handler)
{
    return m_onCollisionLeave.connect(handler);
}

EventConnection RigidBody2DComponent::connectOnCollisionStay(CollisionEventHandler handler)
{
    return m_onCollisionStay.connect(handler);
}

void RigidBody2DComponent::onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    auto* ownerComponent = reinterpret_cast<RigidBody2DComponent*>(otherObject->ownerData());
	if (ownerComponent) {
		// TODO: Cache
		auto c = newObject<Collision>(ownerComponent->worldObject());
		m_onCollisionEnter.raise(c);
	}
}

void RigidBody2DComponent::onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    auto* ownerComponent = reinterpret_cast<RigidBody2DComponent*>(otherObject->ownerData());
	if (ownerComponent) {
		// TODO: Cache
		auto c = newObject<Collision>(ownerComponent->worldObject());
		m_onCollisionLeave.raise(c);
	}
}

void RigidBody2DComponent::onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    auto* ownerComponent = reinterpret_cast<RigidBody2DComponent*>(otherObject->ownerData());
	if (ownerComponent) {
		// TODO: Cache
		auto c = newObject<Collision>(ownerComponent->worldObject());
		m_onCollisionStay.raise(c);
	}
}




//=============================================================================
// TriggerBody2DComponent

Ref<TriggerBody2DComponent> TriggerBody2DComponent::create()
{
    return newObject<TriggerBody2DComponent>();
}

TriggerBody2DComponent::TriggerBody2DComponent()
{
}

void TriggerBody2DComponent::init()
{
    Component::init();
    m_body = newObject<TriggerBody2D>();
    m_body->setEventListener(this);
    m_body->setOwnerData(this);
    detail::EngineDomain::engineManager()->mainPhysicsWorld2D()->addPhysicsObject(m_body);
}

void TriggerBody2DComponent::onDispose(bool explicitDisposing)
{
    if (m_body) {
        m_body->setEventListener(nullptr);
        m_body->removeFromPhysicsWorld();
        m_body = nullptr;
    }

    Component::onDispose(explicitDisposing);
}

void TriggerBody2DComponent::addCollisionShape(CollisionShape2D* shape)
{
    m_body->addCollisionShape(shape);
}

void TriggerBody2DComponent::onAttachedWorld(World* newOwner)
{
}

void TriggerBody2DComponent::onDetachedWorld(World* oldOwner)
{
}

void TriggerBody2DComponent::onBeforeStepSimulation()
{
    m_body->setPosition(worldObject()->position());
}

void TriggerBody2DComponent::onAfterStepSimulation()
{
}

EventConnection TriggerBody2DComponent::connectOnCollisionEnter(CollisionEventHandler handler)
{
    return m_onCollisionEnter.connect(handler);
}

EventConnection TriggerBody2DComponent::connectOnCollisionLeave(CollisionEventHandler handler)
{
    return m_onCollisionLeave.connect(handler);
}

EventConnection TriggerBody2DComponent::connectOnCollisionStay(CollisionEventHandler handler)
{
    return m_onCollisionStay.connect(handler);
}

void TriggerBody2DComponent::onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    auto* ownerComponent = reinterpret_cast<TriggerBody2DComponent*>(otherObject->ownerData());
    if (ownerComponent) {
        // TODO: Cache
        auto c = newObject<Collision>(ownerComponent->worldObject());
        m_onCollisionEnter.raise(c);
    }
}

void TriggerBody2DComponent::onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    auto* ownerComponent = reinterpret_cast<TriggerBody2DComponent*>(otherObject->ownerData());
    if (ownerComponent) {
        // TODO: Cache
        auto c = newObject<Collision>(ownerComponent->worldObject());
        m_onCollisionLeave.raise(c);
    }
}

void TriggerBody2DComponent::onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact)
{
    auto* ownerComponent = reinterpret_cast<TriggerBody2DComponent*>(otherObject->ownerData());
    if (ownerComponent) {
        // TODO: Cache
        auto c = newObject<Collision>(ownerComponent->worldObject());
        m_onCollisionStay.raise(c);
    }
}

} // namespace ln

