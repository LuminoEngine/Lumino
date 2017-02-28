
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
	m_impl = NewObject<detail::PhysicsWorldCore>(detail::PhysicsManager::Instance);
}

//------------------------------------------------------------------------------
detail::PhysicsWorldCore* PhysicsWorld::GetImpl() const
{
	return m_impl;
}

//------------------------------------------------------------------------------
void PhysicsWorld::StepSimulation(float elapsedTime)	// TODO: deltaTime? https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/UWorld/Tick/index.html
{
	m_impl->StepSimulation(elapsedTime);
}


LN_NAMESPACE_END
