
#pragma once
#include <Lumino/MultiThreadingObjectList.h>
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

class PhysicsWorldCore
	: public RefObject
{
public:
	PhysicsWorldCore();
	virtual ~PhysicsWorldCore();
	void Initialize(PhysicsManager* manager);

	void SetGravity(const Vector3& gravity);

	btDiscreteDynamicsWorld* GetBtWorld() { return m_btWorld; }

	void StepSimulation(float elapsedTime);
	void DrawDebugShapes(IDebugRenderer* renderer);

	void AddRigidBodyForMmd(RigidBody* rigidBody);
	void AddJointForMmd(Joint* joint);
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

	List<RefPtr<RigidBody>>					m_rigidBodyListForMmd;
	List<RefPtr<Joint>>						m_jointListForMmd;

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

	void AddPhysicsWorld(PhysicsWorldCore* world) { m_worldList.Add(world); }
	void RemovePhysicsWorld(PhysicsWorldCore* world) { m_worldList.Remove(world); }
	void DrawDebugShapesAllWorld(IDebugRenderer* renderer);

private:
	List<PhysicsWorldCore*>	m_worldList;

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
