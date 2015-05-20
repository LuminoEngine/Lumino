
#pragma once

#include "Common.h"
#include "IDebugRenderer.h"

namespace Lumino
{
namespace Physics
{


/// PhysicsManager の実装クラス
class PhysicsManager
	: public RefObject
{
public:
	PhysicsManager(SimulationType type);
	virtual ~PhysicsManager();

public:

	/// 非同期シミュレーションの実行状態の取得
	ASyncSimulationState GetASyncSimulationState() const { return m_asyncSimulationState; }

	/// StepSimulation() の前処理。メインスレッドで呼び出すこと。物理更新スレッドが実行中であれば待機するまで待つ。
	void SyncBeforeStepSimulation();

	/// 
	void StepSimulation(float elapsedTime);

	void DrawDebugShapes(IDebugRenderer* renderer);

public:	// internal

	/// メインスレッド、または物理演算スレッドから呼ばれる。
	void StepSimulationInternal();

	btDiscreteDynamicsWorld* GetBtWorld() { return m_btWorld; }
	void AddRigidBody(RigidBody* body);
	void AddJoint(Joint* joint);


private:

	// それぞれワールドへの追加メソッドが違うので分けておく
	//typedef MultiThreadingRefObjectList<CollisionBody*>	CollisionBodyList;
	typedef MultiThreadingRefObjectList<RigidBody*>		RigidBodyList;		// ソフトボディとはワールドへの追加方法も違うので分けておく
	typedef MultiThreadingRefObjectList<Joint*>			JointList;

	SimulationType	m_simulationType;

	btDefaultCollisionConfiguration*		m_btCollisionConfig;
	btCollisionDispatcher*					m_btCollisionDispatcher;
	btDbvtBroadphase*						m_btBroadphase;
	//btAxisSweep3*							m_btBroadphase;
	btSequentialImpulseConstraintSolver*	m_btSolver;
	btDiscreteDynamicsWorld*				m_btWorld;
	//btSoftRigidDynamicsWorld*				m_btWorld;
	btSoftBodyWorldInfo*					m_softBodyWorldInfo;

	//btSoftRigidDynamicsWorld*			            m_btWorld;
	//btSoftBodyWorldInfo							m_btSoftBodyWorldInfo;
	//btSoftBodyRigidBodyCollisionConfiguration*	m_btCollisionConfiguration;

	//RigidBodyArray			mRigidBodyArrayList;
	//CollisionBodyArray		mCollisionBodyArray;
	//JointArray				mJointArray;

#ifdef LN_USE_PARALLEL
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
#endif

	ASyncSimulationState		m_asyncSimulationState;
	//CollisionBodyList			m_collisionBodyList;
	RigidBodyList				m_rigidBodyList;
	JointList					m_jointList;

	float						m_elapsedTime;

	class DebugDrawer;
	DebugDrawer*	m_debugDrawer;
};

} // namespace Physics
} // namespace Lumino
