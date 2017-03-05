
#include "../Internal.h"
#include <Lumino/Physics/BodyBase.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// PhysicsObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsObject, Object);

//------------------------------------------------------------------------------
PhysicsObject::PhysicsObject()
	: m_ownerWorld(nullptr)
	, m_collisionFilterGroup(0xFFFF)
	, m_collisionFilterMask(0xFFFF)
{
}

//------------------------------------------------------------------------------
PhysicsObject::~PhysicsObject()
{
}

//------------------------------------------------------------------------------
void PhysicsObject::Initialize()
{
}

//------------------------------------------------------------------------------
void PhysicsObject::SetOwnerWorld(PhysicsWorld* owner)
{
	m_ownerWorld = owner;
}

//------------------------------------------------------------------------------
PhysicsWorld* PhysicsObject::GetOwnerWorld() const
{
	return m_ownerWorld;
}

//------------------------------------------------------------------------------
void PhysicsObject::SetCollisionFilterGroup(uint16_t flags)
{
	m_collisionFilterGroup = flags;
}

//------------------------------------------------------------------------------
uint16_t PhysicsObject::GetCollisionFilterGroup() const
{
	return m_collisionFilterGroup;
}

//------------------------------------------------------------------------------
void PhysicsObject::SetCollisionFilterMask(uint16_t flags)
{
	m_collisionFilterMask = flags;
}

//------------------------------------------------------------------------------
uint16_t PhysicsObject::GetCollisionFilterMask() const
{
	return m_collisionFilterMask;
}

//------------------------------------------------------------------------------
void PhysicsObject::OnBeforeStepSimulation()
{
}

//------------------------------------------------------------------------------
void PhysicsObject::OnAfterStepSimulation()
{
}

//------------------------------------------------------------------------------
void PhysicsObject::OnRemovedFromWorld()
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
void BodyBase::Initialize(btCollisionObject* obj)
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
