

#include "../Internal.h"


#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/Joint.h>
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

PhysicsManager* getPhysicsManager(PhysicsManager* priority)
{
	if (priority != NULL) return priority;
	return PhysicsManager::Instance;
}



//==============================================================================
// PhysicsManager
//==============================================================================

PhysicsManager* PhysicsManager::Instance = nullptr;

//------------------------------------------------------------------------------
PhysicsManager::PhysicsManager()
{
}

//------------------------------------------------------------------------------
PhysicsManager::~PhysicsManager()
{
	if (Instance == this) {
		Instance = nullptr;
	}
}

//------------------------------------------------------------------------------
void PhysicsManager::initialize()
{
	if (Instance == nullptr) {
		Instance = this;
	}
}

//------------------------------------------------------------------------------
void PhysicsManager::dispose()
{
}

//------------------------------------------------------------------------------
//void PhysicsManager::DrawDebugShapesAllWorld(IDebugRenderer* renderer)
//{
//	for (PhysicsWorldCore* world : m_worldList)
//	{
//		world->drawDebugShapes(renderer);
//	}
//}

////------------------------------------------------------------------------------
//void PhysicsManager::SyncBeforeStepSimulation()
//{
//	if (m_simulationType == SimulationType_ASync)
//	{
//		// 終わるまで待つ
//	}
//
//	//---------------------------------------------------------
//	// Add～ で追加された新規オブジェクトをワールドに登録
//
//	//// CollisionBody
//	//CollisionBodyArray::ObjectArray& collisionArray = mCollisionBodyArray.getRegistOrderObjectArray();
//	//ln_foreach(CollisionBody* b, collisionArray)
//	//{
//	//	m_btWorld->addCollisionObject(b->getBtCollisionObject(), b->getGroup(), b->getGroupMask());
//	//}
//	// RigidBodyComponent
//	LN_FOREACH(RigidBodyComponent* obj, m_rigidBodyList.GetRegistOrderObjectArray())
//	{
//		m_btWorld->addRigidBody(obj->GetBtRigidBody(), obj->GetGroup(), obj->GetGroupMask());
//	}
//	// Joint
//	LN_FOREACH(Joint* obj, m_jointList.GetRegistOrderObjectArray())
//	{
//		m_btWorld->addConstraint(obj->getBtConstraint());
//	}
//
//
//	//m_collisionBodyList.commit();
//	m_rigidBodyList.commit();
//	m_jointList.commit();
//
//	//---------------------------------------------------------
//	// 剛体の姿勢を同期
//
//	LN_FOREACH(RigidBodyComponent* b, m_rigidBodyList.GetObjectArray())
//	{
//		b->SyncBeforeStepSimulation();
//	}
//
//}
//
////------------------------------------------------------------------------------
//void PhysicsManager::stepSimulation(float elapsedTime)
//{
//	SyncBeforeStepSimulation();
//
//	if (m_simulationType == SimulationType_Sync)
//	{
//		m_elapsedTime = elapsedTime;
//		StepSimulationInternal();
//	}
//	else
//	{
//		LN_THROW(m_asyncSimulationState == ASyncSimulationState_Idling, InvalidOperationException);	// 待機状態になっていなければならない
//
//		m_elapsedTime = elapsedTime;
//	}
//}
//
////------------------------------------------------------------------------------
//void PhysicsManager::AddRigidBody(RigidBodyComponent* body)
//{
//	m_rigidBodyList.AddObject(body);
//}
//
////------------------------------------------------------------------------------
//void PhysicsManager::addJoint(Joint* joint)
//{
//	m_jointList.AddObject(joint);
//}

} // namespace detail
LN_NAMESPACE_END
