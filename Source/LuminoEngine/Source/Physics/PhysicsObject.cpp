
#include "../Internal.h"
#include <Lumino/Physics/PhysicsObject.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// PhysicsObjectComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsObjectComponent, Component);

//------------------------------------------------------------------------------
PhysicsObjectComponent::PhysicsObjectComponent()
	: Component()
	, m_ownerWorld(nullptr)
	, m_collisionFilterGroup(0xFFFF)
	, m_collisionFilterMask(0xFFFF)
{
}

//------------------------------------------------------------------------------
PhysicsObjectComponent::~PhysicsObjectComponent()
{
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::initialize()
{
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::setOwnerWorld(PhysicsWorld* owner)
{
	m_ownerWorld = owner;
}

//------------------------------------------------------------------------------
PhysicsWorld* PhysicsObjectComponent::getOwnerWorld() const
{
	return m_ownerWorld;
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::setCollisionFilterGroup(uint16_t flags)
{
	m_collisionFilterGroup = flags;
}

//------------------------------------------------------------------------------
uint16_t PhysicsObjectComponent::getCollisionFilterGroup() const
{
	return m_collisionFilterGroup;
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::setCollisionFilterMask(uint16_t flags)
{
	m_collisionFilterMask = flags;
}

//------------------------------------------------------------------------------
uint16_t PhysicsObjectComponent::getCollisionFilterMask() const
{
	return m_collisionFilterMask;
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::onBeforeStepSimulation()
{
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::onAfterStepSimulation()
{
}

//------------------------------------------------------------------------------
void PhysicsObjectComponent::onRemovedFromWorld()
{
}


//==============================================================================
// PhysicsObjectComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsObject2, Component);

PhysicsObject2::PhysicsObject2(PhysicsObjectType type)
	: Object()
	, m_objectType(type)
	, m_ownerWorld(nullptr)
	, m_collisionFilterGroup(0xFFFF)
	, m_collisionFilterMask(0xFFFF)
	, m_collisionFilterChanged(false)
	, m_removingFromWorld(false)
{
}

PhysicsObject2::~PhysicsObject2()
{
}

void PhysicsObject2::initialize()
{
	Object::initialize();
}

void PhysicsObject2::setWorld(PhysicsWorld2* owner)
{
	m_ownerWorld = owner;
}

PhysicsWorld2* PhysicsObject2::getWorld() const
{
	return m_ownerWorld;
}

void PhysicsObject2::setCollisionFilterGroup(uint16_t flags)
{
	if (m_collisionFilterGroup != flags)
	{
		m_collisionFilterGroup = flags;
		setCollisionFilterChanged(true);
	}
}

uint16_t PhysicsObject2::getCollisionFilterGroup() const
{
	return m_collisionFilterGroup;
}

void PhysicsObject2::setCollisionFilterMask(uint16_t flags)
{
	if (m_collisionFilterMask != flags)
	{
		m_collisionFilterMask = flags;
		setCollisionFilterChanged(true);
	}
}

uint16_t PhysicsObject2::getCollisionFilterMask() const
{
	return m_collisionFilterMask;
}

void PhysicsObject2::onBeforeStepSimulation()
{
}

void PhysicsObject2::onAfterStepSimulation()
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
