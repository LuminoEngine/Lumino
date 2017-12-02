
#pragma once
#include "../MultiThreadingObjectList.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
class PhysicsObjectComponent;
class Joint;
class PhysicsDebugDrawer;
class PhysicsObject2;

/**
	@brief	
*/
class PhysicsWorld
	: public Object
{
	LN_OBJECT;
public:

	//void AddRigidBody(RigidBodyComponent* rigidBody);
	//void RemoveRigidBody(RigidBodyComponent* rigidBody);

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

	void addPhysicsObject(PhysicsObjectComponent* physicsObject);
	void addJoint(Joint* joint);
	void removePhysicsObject(PhysicsObjectComponent* physicsObject);
	void gcPhysicsObjects();

	// AutoAdd interface
	//void AutoAddChild(RigidBodyComponent* child);

private:
	//Ref<detail::PhysicsWorldCore>		m_impl;
	//MultiThreadingInFrameGCList<RigidBodyComponent>	m_rigidBodyList;

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

	List<Ref<PhysicsObjectComponent>>				m_physicsObjectList;
	List<Ref<Joint>>						m_jointList;
};

class PhysicsWorld2
	: public Object
{
	LN_OBJECT;
public:
	void setGravity(const Vector3& gravity);

	void add(PhysicsResource2* obj);
	void remove(PhysicsResource2* obj);

LN_CONSTRUCT_ACCESS:
	PhysicsWorld2();
	virtual ~PhysicsWorld2();
	void initialize();
	virtual void finalize_();

LN_INTERNAL_ACCESS:
	btDiscreteDynamicsWorld* getBtWorld() { return m_btWorld; }
	void stepSimulation(float elapsedTime);
	void gcPhysicsObjects(bool force);
	void addObjectInternal(PhysicsResource2* obj);
	void removeObjectInternal(PhysicsResource2* obj);

private:
	btDefaultCollisionConfiguration*		m_btCollisionConfig;
	btCollisionDispatcher*					m_btCollisionDispatcher;
	btDbvtBroadphase*						m_btBroadphase;
	btSequentialImpulseConstraintSolver*	m_btSolver;
	btDiscreteDynamicsWorld*				m_btWorld;
	btGhostPairCallback*					m_btGhostPairCallback;
	btSoftBodyWorldInfo*					m_softBodyWorldInfo;
	PhysicsDebugDrawer*						m_debugDrawer;
	List<Ref<PhysicsResource2>>				m_physicsResources;
};

LN_NAMESPACE_END
