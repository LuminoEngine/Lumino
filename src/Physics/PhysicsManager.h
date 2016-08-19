
#pragma once
#include <Lumino/Physics/Common.h>

LN_NAMESPACE_BEGIN
class RigidBody;
class Joint;

namespace detail
{
class PhysicsManager;
class PhysicsDebugDrawer;

class IDebugRenderer
{
public:
	virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor) = 0;
};

class PhysicsWorld
	: public RefObject
{
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void Initialize(PhysicsManager* manager);

	btDiscreteDynamicsWorld* GetBtWorld() { return m_btWorld; }

	void StepSimulation(float elapsedTime);
	void DrawDebugShapes(IDebugRenderer* renderer);

	void AddRigidBody(RigidBody* rigidBody);
	void AddJoint(Joint* joint);
	void GCPhysicsObjects();

private:
	RefPtr<PhysicsManager>					m_manager;
	btDefaultCollisionConfiguration*		m_btCollisionConfig;
	btCollisionDispatcher*					m_btCollisionDispatcher;
	btDbvtBroadphase*						m_btBroadphase;
	//btAxisSweep3*							m_btBroadphase;
	btSequentialImpulseConstraintSolver*	m_btSolver;
	btDiscreteDynamicsWorld*				m_btWorld;
	//btSoftRigidDynamicsWorld*				m_btWorld;
	btSoftBodyWorldInfo*					m_softBodyWorldInfo;
	PhysicsDebugDrawer*						m_debugDrawer;

#ifdef LN_USE_PARALLEL
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
#endif

	Array<RefPtr<RigidBody>>				m_rigidBodyList;
	Array<RefPtr<Joint>>					m_jointList;
};

class PhysicsManager
	: public RefObject
{
public:
	static PhysicsManager* Instance;

public:
	PhysicsManager();
	virtual ~PhysicsManager();

	void Initialize();
	void Finalize();

	void AddPhysicsWorld(PhysicsWorld* world) { m_worldList.Add(world); }
	void RemovePhysicsWorld(PhysicsWorld* world) { m_worldList.Remove(world); }
	void DrawDebugShapesAllWorld(IDebugRenderer* renderer);

private:
	Array<PhysicsWorld*>	m_worldList;

//
//
//	/// StepSimulation() の前処理。メインスレッドで呼び出すこと。物理更新スレッドが実行中であれば待機するまで待つ。
//	void SyncBeforeStepSimulation();
//
//	/// 
//	void StepSimulation(float elapsedTime);
//
//	void DrawDebugShapes(IDebugRenderer* renderer);
//
//public:	// internal
//
//	/// メインスレッド、または物理演算スレッドから呼ばれる。
//	void StepSimulationInternal();
//
//	void AddRigidBody(RigidBody* body);
//	void AddJoint(Joint* joint);


//private:
//
//	// それぞれワールドへの追加メソッドが違うので分けておく
//	//typedef MultiThreadingRefObjectList<CollisionBody*>	CollisionBodyList;
//	//typedef MultiThreadingRefObjectList<RigidBody*>		RigidBodyList;		// ソフトボディとはワールドへの追加方法も違うので分けておく
//	//typedef MultiThreadingRefObjectList<Joint*>			JointList;
//
//	SimulationType	m_simulationType;
//
//
//	//btSoftRigidDynamicsWorld*			            m_btWorld;
//	//btSoftBodyWorldInfo							m_btSoftBodyWorldInfo;
//	//btSoftBodyRigidBodyCollisionConfiguration*	m_btCollisionConfiguration;
//
//	//RigidBodyArray			mRigidBodyArrayList;
//	//CollisionBodyArray		mCollisionBodyArray;
//	//JointArray				mJointArray;
//
//
//	ASyncSimulationState		m_asyncSimulationState;
//	//CollisionBodyList			m_collisionBodyList;
//	RigidBodyList				m_rigidBodyList;
//	JointList					m_jointList;
//
//	float						m_elapsedTime;
};

} // namespace detail
LN_NAMESPACE_END
