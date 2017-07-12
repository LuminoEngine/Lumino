
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
	LN_OBJECT();
public:

	//void AddRigidBody(RigidBody* rigidBody);
	//void RemoveRigidBody(RigidBody* rigidBody);

	void setGravity(const Vector3& gravity);


LN_CONSTRUCT_ACCESS:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void initialize();

LN_INTERNAL_ACCESS:
	class IDebugRenderer
	{
	public:

		virtual void drawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor) = 0;
	};

	btDiscreteDynamicsWorld* getBtWorld() { return m_btWorld; }
	void stepSimulation(float elapsedTime);
	void drawDebugShapes(IDebugRenderer* renderer);

	void addPhysicsObject(PhysicsObject* physicsObject);
	void addJoint(Joint* joint);
	void removePhysicsObject(PhysicsObject* physicsObject);
	void gcPhysicsObjects();

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
