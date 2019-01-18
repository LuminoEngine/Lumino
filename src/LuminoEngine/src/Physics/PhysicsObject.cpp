
#include "Internal.hpp"
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

void PhysicsObject::initialize()
{
	Object::initialize();
}

void PhysicsObject::setPhysicsWorld(PhysicsWorld* owner)
{
	m_ownerWorld = owner;
}

PhysicsWorld* PhysicsObject::physicsWorld() const
{
	return m_ownerWorld;
}

void PhysicsObject::onBeforeStepSimulation()
{
}

void PhysicsObject::onAfterStepSimulation()
{
}

} // namespace ln

