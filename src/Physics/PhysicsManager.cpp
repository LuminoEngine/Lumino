

#include "../Internal.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>

#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

// Win32
//#include <BulletMultiThreaded/Win32ThreadSupport.h>
//#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>

//#include <BulletMultiThreaded/btParallelConstraintSolver.h>
//#include <BulletMultiThreaded/SequentialThreadSupport.h>

#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/Joint.h>
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail
{

PhysicsManager* GetPhysicsManager(PhysicsManager* priority)
{
	if (priority != NULL) return priority;
	return PhysicsManager::Instance;
}

//==============================================================================
// PhysicsDebugDrawer
//==============================================================================
class PhysicsDebugDrawer
	: public btIDebugDraw
{
public:
	PhysicsDebugDrawer()
		: m_debugRenderer(nullptr)
		, m_debugMode(0)
	{}

	virtual ~PhysicsDebugDrawer() {}

public:
	void SetDebugRenderer(IDebugRenderer* r) { m_debugRenderer = r; }

protected:
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color);
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override
	{
		m_debugRenderer->DrawLine(
			Vector3(from.getX(), from.getY(), from.getZ()),
			Vector3(to.getX(), to.getY(), to.getZ()),
			Vector3(fromColor.getX(), fromColor.getY(), fromColor.getZ()),
			Vector3(toColor.getX(), toColor.getY(), toColor.getZ()));
	}

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
	}

	virtual void reportErrorWarning(const char* warningString) override
	{
		printf("Physics: %s\n", warningString);	// TODO
	}

	virtual void draw3dText(const btVector3& location, const char* textString) override
	{
	}

	virtual void setDebugMode(int debugMode) override
	{
		m_debugMode = debugMode;
	}

	virtual int getDebugMode() const override
	{
		return m_debugMode;
	}

private:
	IDebugRenderer* m_debugRenderer;
	int				m_debugMode;
};

//==============================================================================
// PhysicsWorld
//==============================================================================

//------------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld()
{
}

//------------------------------------------------------------------------------
PhysicsWorld::~PhysicsWorld()
{
	GCPhysicsObjects();
	LN_SAFE_DELETE(m_btCollisionConfig);
	LN_SAFE_DELETE(m_btCollisionDispatcher);
	LN_SAFE_DELETE(m_btBroadphase);
	LN_SAFE_DELETE(m_btSolver);
	LN_SAFE_DELETE(m_btWorld);
	LN_SAFE_DELETE(m_softBodyWorldInfo);
	LN_SAFE_DELETE(m_debugDrawer);
}

//------------------------------------------------------------------------------
void PhysicsWorld::Initialize(PhysicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;

	// ↓以下で出てくる単語とか
	//		http://nullorempry.jimdo.com/2012/03/10/bullet-physics%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7/

	int maxNumOutstandingTasks = 4;

#ifdef LN_USE_PARALLEL
	m_threadSupportCollision = new Win32ThreadSupport(Win32ThreadSupport::Win32ThreadConstructionInfo(
		"collision",
		processCollisionTask,
		createCollisionLocalStoreMemory,
		maxNumOutstandingTasks));
#endif

	// カスタムコリジョンコールバック
	//gContactAddedCallback = CustomMaterialCombinerCallback;

	// コリジョンコンフィグ
	btDefaultCollisionConstructionInfo defaultCollisionConstructionInfo;
	//defaultCollisionConstructionInfo.m_defaultMaxPersistentManifoldPoolSize = 32768;
	m_btCollisionConfig = new btDefaultCollisionConfiguration(defaultCollisionConstructionInfo);
	// ソフトボディ使うとき
	//m_btCollisionConfig = LN_NEW btSoftBodyRigidBodyCollisionConfiguration();// 

	// コリジョンディスパッチャ
#ifdef LN_USE_PARALLEL
	m_btCollisionDispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision, maxNumOutstandingTasks, m_btCollisionConfig);
#else
	m_btCollisionDispatcher = LN_NEW btCollisionDispatcher(m_btCollisionConfig);
#endif

	// ブロードフェーズアルゴリズム (衝突検出のためのグループ分けアルゴリズム)
	/*
		btDbvtBroadphase				AABB木による高速で動的な階層空間を使います。
		btAxisSweep3 bt32BitAxisSweep3	逐次3Dスイープ&プルーン(incremental 3D sweep and prune)を実装しています。
		btCudaBroadphase				GPUを使用した高速な一様グリッドを実装しています。http://bulletjpn.web.fc2.com/05_BulletCollisionDetection.html
	*/
	//   btVector3	btv3WorldAabbMin( -3000.0f, -3000.0f, -3000.0f );
	//btVector3	btv3WorldAabbMax(  3000.0f,  3000.0f,  3000.0f );
	//int			iMaxProxies = 32766;//5 * 5 * 5 + 1024;//
	//m_btBroadphase = new btAxisSweep3( btv3WorldAabbMin, btv3WorldAabbMax, iMaxProxies );
	m_btBroadphase = LN_NEW btDbvtBroadphase();

	// ソルバ
#ifdef LN_USE_PARALLEL
	m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
	m_btSolver = new btParallelConstraintSolver(m_threadSupportSolver);
	//this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
	m_btCollisionDispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);
#else
	m_btSolver = new btSequentialImpulseConstraintSolver();
#endif

	// ワールドの作成
	//m_btWorld = LN_NEW btSoftRigidDynamicsWorld( m_btCollisionDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfig, NULL );
	m_btWorld = new btDiscreteDynamicsWorld(m_btCollisionDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfig);

	m_debugDrawer = LN_NEW PhysicsDebugDrawer();
	m_btWorld->setDebugDrawer(m_debugDrawer);
	//m_btWorld->setInternalTickCallback(pickingPreTickCallback,this,true);
	//m_btWorld->getDispatchInfo().m_enableSPU = true;

	//btOverlapFilterCallback * filterCallback = new FilterCallback();
	//m_btWorld->getPairCache()->setOverlapFilterCallback( &gFilterCallback );

#if 0	// MMM setting (デフォルトは 10)
	m_btWorld->getSolverInfo().m_numIterations = 4;
#endif
#ifdef LN_USE_PARALLEL
	m_btWorld->getSimulationIslandManager()->setSplitIslands(false);
	m_btWorld->getSolverInfo().m_numIterations = 4;
	m_btWorld->getSolverInfo().m_solverMode = SOLVER_SIMD + SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

	m_btWorld->getDispatchInfo().m_enableSPU = true;
#endif
	//m_btWorld->setDebugDrawer(&gDebugDrawer);

	// 重力		TODO: 初期値
	m_btWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));

	m_softBodyWorldInfo = new btSoftBodyWorldInfo();
	m_softBodyWorldInfo->air_density = 1.2f;
	m_softBodyWorldInfo->water_density = 0;
	m_softBodyWorldInfo->water_offset = 0;
	m_softBodyWorldInfo->water_normal = btVector3(0.0f, 0.0f, 0.0f);
	m_softBodyWorldInfo->m_gravity = m_btWorld->getGravity();
	m_softBodyWorldInfo->m_broadphase = m_btBroadphase;
	m_softBodyWorldInfo->m_dispatcher = m_btCollisionDispatcher;
	m_softBodyWorldInfo->m_sparsesdf.Initialize();
	m_softBodyWorldInfo->m_sparsesdf.Reset();

	/*
	const btScalar	s=4;
	const btScalar	h=6;
	const int		r=20;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0,h-2,0));



	int fixed=0;//4+8;
	btSoftBody*		psb=btSoftBodyHelpers::CreatePatch(*m_softBodyWorldInfo,btVector3(-s,h,-s),
	btVector3(+s,h,-s),
	btVector3(-s,h,+s),
	btVector3(+s,h,+s),r,r,fixed,true);
	m_btWorld->addSoftBody(psb);
	psb->setTotalMass(0.1);

	psb->m_cfg.piterations = 10;
	psb->m_cfg.citerations = 10;
	psb->m_cfg.diterations = 10;
	//	psb->m_cfg.viterations = 10;

	*/

}

//------------------------------------------------------------------------------
void PhysicsWorld::SetGravity(const Vector3& gravity)
{
	m_btWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

//------------------------------------------------------------------------------
void PhysicsWorld::StepSimulation(float elapsedTime)
{
	for (RigidBody* body : m_rigidBodyList)
	{
		body->SyncBeforeStepSimulation(this);
	}

	const float internalUnit = 1.0f / 60.0f;

	// http://d.hatena.ne.jp/ousttrue/20100425/1272165711
	// m_elapsedTime が 1.0(1秒) より小さい間は 16ms を最大 60 回繰り返して追いつこうとする設定。
	// m_elapsedTime が 1.0 を超えている場合は追いつけずに、物体の移動が遅くなる。
	m_btWorld->stepSimulation(elapsedTime, 60, internalUnit);

	// m_elapsedTime が 16ms より大きい場合は、1回 16ms 分のシミュレーションを可能な限り繰り返して m_elapsedTime に追いついていく設定。
	// 遅れるほど計算回数が増えるので、最終的に破綻するかもしれない。
	//m_btWorld->stepSimulation(m_elapsedTime, 1 + (int)(m_elapsedTime / internalUnit), internalUnit);

	//---------------------------------------------------------
	// 剛体の姿勢を同期 (演算結果 → ユーザー)
	//LN_FOREACH(RigidBody* b, m_rigidBodyList.GetObjectArray())
	//{
	//	b->SyncAfterStepSimulation();
	//}

	for (RigidBody* body : m_rigidBodyList)
	{
		body->SyncAfterStepSimulation();
	}

	GCPhysicsObjects();
}

//------------------------------------------------------------------------------
void PhysicsWorld::DrawDebugShapes(IDebugRenderer* renderer)
{
	m_debugDrawer->SetDebugRenderer(renderer);
	m_btWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btWorld->debugDrawWorld();
}

//------------------------------------------------------------------------------
void PhysicsWorld::AddRigidBody(RigidBody* rigidBody)
{
	LN_ASSERT(rigidBody != nullptr);
	m_rigidBodyList.Add(rigidBody);
	m_btWorld->addRigidBody(rigidBody->GetBtRigidBody(), rigidBody->GetGroup(), rigidBody->GetGroupMask());
}

//------------------------------------------------------------------------------
void PhysicsWorld::AddJoint(Joint* joint)
{
	LN_ASSERT(joint != nullptr);
	m_jointList.Add(joint);
	m_btWorld->addConstraint(joint->GetBtConstraint());
}

//------------------------------------------------------------------------------
void PhysicsWorld::GCPhysicsObjects()
{
	// rigidBody
	for (auto itr = m_rigidBodyList.begin(); itr != m_rigidBodyList.end();)
	{
		if ((*itr)->GetReferenceCount() == 1)
		{
			m_btWorld->removeRigidBody((*itr)->GetBtRigidBody());
			itr = m_rigidBodyList.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	// constraint
	for (auto itr = m_jointList.begin(); itr != m_jointList.end();)
	{
		if ((*itr)->GetReferenceCount() == 1)
		{
			m_btWorld->removeConstraint((*itr)->GetBtConstraint());
			itr = m_jointList.erase(itr);
		}
		else
		{
			++itr;
		}
	}
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
void PhysicsManager::Initialize()
{
	if (Instance == nullptr) {
		Instance = this;
	}
}

//------------------------------------------------------------------------------
void PhysicsManager::Finalize()
{
}

//------------------------------------------------------------------------------
void PhysicsManager::DrawDebugShapesAllWorld(IDebugRenderer* renderer)
{
	for (PhysicsWorld* world : m_worldList)
	{
		world->DrawDebugShapes(renderer);
	}
}

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
//	// RigidBody
//	LN_FOREACH(RigidBody* obj, m_rigidBodyList.GetRegistOrderObjectArray())
//	{
//		m_btWorld->addRigidBody(obj->GetBtRigidBody(), obj->GetGroup(), obj->GetGroupMask());
//	}
//	// Joint
//	LN_FOREACH(Joint* obj, m_jointList.GetRegistOrderObjectArray())
//	{
//		m_btWorld->addConstraint(obj->GetBtConstraint());
//	}
//
//
//	//m_collisionBodyList.Commit();
//	m_rigidBodyList.Commit();
//	m_jointList.Commit();
//
//	//---------------------------------------------------------
//	// 剛体の姿勢を同期
//
//	LN_FOREACH(RigidBody* b, m_rigidBodyList.GetObjectArray())
//	{
//		b->SyncBeforeStepSimulation();
//	}
//
//}
//
////------------------------------------------------------------------------------
//void PhysicsManager::StepSimulation(float elapsedTime)
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
//void PhysicsManager::AddRigidBody(RigidBody* body)
//{
//	m_rigidBodyList.AddObject(body);
//}
//
////------------------------------------------------------------------------------
//void PhysicsManager::AddJoint(Joint* joint)
//{
//	m_jointList.AddObject(joint);
//}

} // namespace detail
LN_NAMESPACE_END
