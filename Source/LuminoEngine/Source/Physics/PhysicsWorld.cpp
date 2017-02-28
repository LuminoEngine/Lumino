
#include "../Internal.h"
#include <Lumino/Physics/RigidBody.h>
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
void PhysicsWorld::AddRigidBody(RigidBody* rigidBody)
{
	LN_FAIL_CHECK_ARG(rigidBody != nullptr) return;

	PhysicsWorld* otherOwner = rigidBody->GetOwnerWorld();
	if (otherOwner != nullptr) otherOwner->RemoveRigidBody(rigidBody);
	m_rigidBodyList.AddObject(rigidBody);
	rigidBody->SetOwnerWorld(this);
}

//------------------------------------------------------------------------------
void PhysicsWorld::RemoveRigidBody(RigidBody* rigidBody)
{
	LN_FAIL_CHECK_ARG(rigidBody != nullptr) return;

	if (rigidBody->GetOwnerWorld() == this)
	{
		m_rigidBodyList.RemoveObject(rigidBody);
		rigidBody->SetOwnerWorld(nullptr);
	}
}

//------------------------------------------------------------------------------
detail::PhysicsWorldCore* PhysicsWorld::GetImpl() const
{
	return m_impl;
}

//------------------------------------------------------------------------------
void PhysicsWorld::StepSimulation(float elapsedTime)	// TODO: deltaTime? https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/UWorld/Tick/index.html
{
	m_rigidBodyList.Commit(
		[this](RigidBody* child) { m_impl->AddRigidBodyForMmd(child); },
		[this](RigidBody* child) { m_impl->remo(child); });

	m_impl->StepSimulation(elapsedTime);
}

//------------------------------------------------------------------------------
void PhysicsWorld::AutoAddChild(RigidBody* child)
{
	AddRigidBody(child);
}

LN_NAMESPACE_END
