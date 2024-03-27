
#include "Internal.hpp"
#include <LuminoFramework/Physics/PhysicsWorld.hpp>
#include <LuminoFramework/Physics/PhysicsObject.hpp>

namespace ln {

//==============================================================================
// PhysicsObject
    
PhysicsObject::PhysicsObject(PhysicsObjectType type)
	: Object()
	, m_resourceType(type)
	, m_ownerWorld(nullptr)
	, m_removing(false)
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
	if (m_listener) {
		m_listener->onCollisionEnter(otherObject, contact);
	}
}

void PhysicsObject::onCollisionLeave(PhysicsObject* otherObject, ContactPoint* contact)
{
	if (m_listener) {
		m_listener->onCollisionLeave(otherObject, contact);
	}
}

void PhysicsObject::onCollisionStay(PhysicsObject* otherObject, ContactPoint* contact)
{
	if (m_listener) {
		m_listener->onCollisionStay(otherObject, contact);
	}
}

void PhysicsObject::onPrepareStepSimulation()
{
	if (m_listener) {
		m_listener->onBeforeStepSimulation_Deprecated();
	}
}

void PhysicsObject::onAfterStepSimulation()
{
	if (m_listener) {
		m_listener->onAfterStepSimulation();
	}
}

void PhysicsObject::beginContact(PhysicsObject* otherObject)
{
	m_contactBodies.add(otherObject);
	onCollisionEnter(otherObject, nullptr);
}

void PhysicsObject::endContact(PhysicsObject* otherObject)
{
	if (LN_REQUIRE(m_contactBodies.remove(otherObject))) return;
	onCollisionLeave(otherObject, nullptr);
}

} // namespace ln

