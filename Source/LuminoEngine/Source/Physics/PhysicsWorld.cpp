
#include "../Internal.h"
#include <Lumino/Physics/PhysicsWorld.h>
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// PhysicsWorld
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsWorld, Object);

//------------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld()
	: m_impl(nullptr)
{
}

//------------------------------------------------------------------------------
PhysicsWorld::~PhysicsWorld()
{
}

//------------------------------------------------------------------------------
void PhysicsWorld::Initialize()
{
	m_impl = NewObject<detail::PhysicsWorld>(detail::PhysicsManager::Instance);
}

LN_NAMESPACE_END
