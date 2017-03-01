
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
