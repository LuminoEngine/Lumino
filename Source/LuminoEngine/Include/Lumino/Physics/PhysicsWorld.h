
#pragma once
#include "../MultiThreadingObjectList.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsObject;
class Joint;
class PhysicsDebugDrawer;

/**
	@brief	
*/
class PhysicsWorld
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	//void AddRigidBody(RigidBody* rigidBody);
	//void RemoveRigidBody(RigidBody* rigidBody);

	void SetGravity(const Vector3& gravity);


LN_CONSTRUCT_ACCESS:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void initialize();

LN_INTERNAL_ACCESS:
	class IDebugRenderer
	{
	public:

		virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor) = 0;
	};

	btDiscreteDynamicsWorld* GetBtWorld() { return m_btWorld; }
	void StepSimulation(float elapsedTime);
	void DrawDebugShapes(IDebugRenderer* renderer);

	void AddPhysicsObject(PhysicsObject* physicsObject);
	void AddJoint(Joint* joint);
	void RemovePhysicsObject(PhysicsObject* physicsObject);
	void GCPhysicsObjects();

	// AutoAdd interface
	//void AutoAddChild(RigidBody* child);

private:
	//RefPtr<detail::PhysicsWorldCore>		m_impl;
	//MultiThreadingInFrameGCList<RigidBody>	m_rigidBodyList;

	btDefaultCollisionConfiguration*		m_btCollisionConfig;
	btCollisionDispatcher*					m_btCollisionDispatcher;
	btDbvtBroadphase*						m_btBroadphase;
	//btAxisSweep3*							m_btBroadphase;
	btSequentialImpulseConstraintSolver*	m_btSolver;
	btDiscreteDynamicsWorld*				m_btWorld;
	//btSoftRigidDynamicsWorld*				m_btWorld;
	btGhostPairCallback*					m_btGhostPairCallback;
	btSoftBodyWorldInfo*					m_softBodyWorldInfo;
	PhysicsDebugDrawer*						m_debugDrawer;

	List<RefPtr<PhysicsObject>>				m_physicsObjectList;
	List<RefPtr<Joint>>						m_jointList;
};

LN_NAMESPACE_END
