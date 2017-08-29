
#include "../Internal.h"
#include <Lumino/Physics/PhysicsObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// PhysicsObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsObject, Component);

//------------------------------------------------------------------------------
PhysicsObject::PhysicsObject()
	: Component()
	, m_ownerWorld(nullptr)
	, m_collisionFilterGroup(0xFFFF)
	, m_collisionFilterMask(0xFFFF)
{
}

//------------------------------------------------------------------------------
PhysicsObject::~PhysicsObject()
{
}

//------------------------------------------------------------------------------
bool PhysicsObject::initialize()
{
	return Component::initialize();
}

//------------------------------------------------------------------------------
void PhysicsObject::setOwnerWorld(PhysicsWorld* owner)
{
	m_ownerWorld = owner;
}

//------------------------------------------------------------------------------
PhysicsWorld* PhysicsObject::getOwnerWorld() const
{
	return m_ownerWorld;
}

//------------------------------------------------------------------------------
void PhysicsObject::setCollisionFilterGroup(uint16_t flags)
{
	m_collisionFilterGroup = flags;
}

//------------------------------------------------------------------------------
uint16_t PhysicsObject::getCollisionFilterGroup() const
{
	return m_collisionFilterGroup;
}

//------------------------------------------------------------------------------
void PhysicsObject::setCollisionFilterMask(uint16_t flags)
{
	m_collisionFilterMask = flags;
}

//------------------------------------------------------------------------------
uint16_t PhysicsObject::getCollisionFilterMask() const
{
	return m_collisionFilterMask;
}

//------------------------------------------------------------------------------
void PhysicsObject::onBeforeStepSimulation()
{
}

//------------------------------------------------------------------------------
void PhysicsObject::onAfterStepSimulation()
{
}

//------------------------------------------------------------------------------
void PhysicsObject::onRemovedFromWorld()
{
}

LN_NAMESPACE_END


#if 0
#include "../Internal.h"
#include <btBulletDynamicsCommon.h>
#include <Lumino/Physics/BodyBase.h>
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// BodyBase
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(BodyBase, Object);

//------------------------------------------------------------------------------
BodyBase::BodyBase()
	: m_ownerWorld(nullptr)
	, m_userData(nullptr)
	, m_contactList()
{
}

//------------------------------------------------------------------------------
BodyBase::~BodyBase()
{
}

//------------------------------------------------------------------------------
void BodyBase::initialize(btCollisionObject* obj)
{
	obj->setUserPointer(this);
}

//------------------------------------------------------------------------------
void BodyBase::SetOwnerWorld(PhysicsWorld* owner)
{
	m_ownerWorld = owner;
}

//------------------------------------------------------------------------------
PhysicsWorld* BodyBase::GetOwnerWorld() const
{
	return m_ownerWorld;
}

LN_NAMESPACE_END
#endif
