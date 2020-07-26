
#include "Internal.hpp"
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/PhysicsObject.hpp>

namespace ln {

//==============================================================================
// PhysicsObject
    
PhysicsObject::PhysicsObject(PhysicsObjectType type)
	: Object()
	, m_resourceType(type)
	, m_ownerWorld(nullptr)
	, m_removingFromWorld(false)
{
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::init()
{
	Object::init();
}

void PhysicsObject::setPhysicsWorld(PhysicsWorld* owner)
{
	m_ownerWorld = owner;
}

PhysicsWorld* PhysicsObject::physicsWorld() const
{
	return m_ownerWorld;
}

void PhysicsObject::removeFromPhysicsWorld()
{
	if (m_ownerWorld) {
		m_ownerWorld->removePhysicsObject(this);
	}
}

void PhysicsObject::onCollisionEnter(PhysicsObject* otherObject, ContactPoint* contact)
{
	m_listener->onCollisionEnter(otherObject, contact);
}

void PhysicsObject::onCollisionLeave(PhysicsObject* otherObject, ContactPoint* contact)
{
	m_listener->onCollisionLeave(otherObject, contact);
}

void PhysicsObject::onCollisionStay(PhysicsObject* otherObject, ContactPoint* contact)
{
	m_listener->onCollisionStay(otherObject, contact);
}

void PhysicsObject::onBeforeStepSimulation()
{
}

void PhysicsObject::onAfterStepSimulation()
{
}

} // namespace ln

