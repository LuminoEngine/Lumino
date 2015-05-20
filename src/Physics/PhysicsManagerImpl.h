
#pragma once

#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

#include <Lumino/Physics/PhysicsManager.h>
#include <Lumino/Graphics/Color.h>

namespace Lumino
{
namespace Physics
{



/// PhysicsManager ‚ÌŽÀ‘•ƒNƒ‰ƒX
class PhysicsManagerImpl
	: public PhysicsManager
{

public:
	PhysicsManagerImpl(SimulationType type);
	virtual ~PhysicsManagerImpl();

public:

	/// 
	virtual void StepSimulation(float elapsedTime);

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

	//#define LN_USE_PARALLEL

#ifdef LN_USE_PARALLEL
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;
#endif

};

} // namespace Physics
} // namespace Lumino
