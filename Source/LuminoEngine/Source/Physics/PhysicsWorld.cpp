
#include "../Internal.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletMultiThreaded/SpuGatheringCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
// Win32
//#include <BulletMultiThreaded/Win32ThreadSupport.h>
//#include <BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h>
//#include <BulletMultiThreaded/btParallelConstraintSolver.h>
//#include <BulletMultiThreaded/SequentialThreadSupport.h>

#include <Lumino/Physics/RigidBody.h>
#include <Lumino/Physics/Joint.h>
#include <Lumino/Physics/PhysicsWorld.h>
#include "PhysicsManager.h"

LN_NAMESPACE_BEGIN

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
	void SetDebugRenderer(PhysicsWorld::IDebugRenderer* r) { m_debugRenderer = r; }

protected:
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color, color);
	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override
	{
		m_debugRenderer->drawLine(
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
	PhysicsWorld::IDebugRenderer* m_debugRenderer;
	int				m_debugMode;
};

//==============================================================================
// PhysicsWorld
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsWorld, Object);

//------------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld()
{
}

//------------------------------------------------------------------------------
PhysicsWorld::~PhysicsWorld()
{
	gcPhysicsObjects();

	LN_SAFE_DELETE(m_softBodyWorldInfo);
	LN_SAFE_DELETE(m_btGhostPairCallback);
	LN_SAFE_DELETE(m_btWorld);
	LN_SAFE_DELETE(m_debugDrawer);
	LN_SAFE_DELETE(m_btSolver);
	LN_SAFE_DELETE(m_btBroadphase);
	LN_SAFE_DELETE(m_btCollisionDispatcher);
	LN_SAFE_DELETE(m_btCollisionConfig);
}

//------------------------------------------------------------------------------
void PhysicsWorld::initialize()
{
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


	m_btGhostPairCallback = new btGhostPairCallback();
	m_btWorld->getPairCache()->setInternalGhostPairCallback(m_btGhostPairCallback);


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
}

//------------------------------------------------------------------------------
void PhysicsWorld::setGravity(const Vector3& gravity)
{
	m_btWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

//------------------------------------------------------------------------------
void PhysicsWorld::stepSimulation(float elapsedTime)
{
	gcPhysicsObjects();

	for (PhysicsObjectComponent* obj : m_physicsObjectList)
	{
		obj->onBeforeStepSimulation();
	}

	const float internalUnit = 1.0f / 60.0f;

	// http://d.hatena.ne.jp/ousttrue/20100425/1272165711
	// m_elapsedTime が 1.0(1秒) より小さい間は 16ms を最大 60 回繰り返して追いつこうとする設定。
	// m_elapsedTime が 1.0 を超えている場合は追いつけずに、物体の移動が遅くなる。
	m_btWorld->stepSimulation(elapsedTime, 120, 0.008333334f);

	// m_elapsedTime が 16ms より大きい場合は、1回 16ms 分のシミュレーションを可能な限り繰り返して m_elapsedTime に追いついていく設定。
	// 遅れるほど計算回数が増えるので、最終的に破綻するかもしれない。
	//m_btWorld->stepSimulation(m_elapsedTime, 1 + (int)(m_elapsedTime / internalUnit), internalUnit);

	//---------------------------------------------------------
	// 剛体の姿勢を同期 (演算結果 → ユーザー)
	//LN_FOREACH(RigidBodyComponent* b, m_rigidBodyList.GetObjectArray())
	//{
	//	b->SyncAfterStepSimulation();
	//}

	for (PhysicsObjectComponent* obj : m_physicsObjectList)
	{
		obj->onAfterStepSimulation();
	}

	gcPhysicsObjects();
}

//------------------------------------------------------------------------------
void PhysicsWorld::drawDebugShapes(IDebugRenderer* renderer)
{
	m_debugDrawer->SetDebugRenderer(renderer);
	m_btWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btWorld->debugDrawWorld();
}

//------------------------------------------------------------------------------
void PhysicsWorld::addPhysicsObject(PhysicsObjectComponent* physicsObject)
{
	if (LN_REQUIRE(physicsObject != nullptr)) return;

	// 別のワールドに属していれば除外する
	if (physicsObject->getOwnerWorld() != nullptr)
		physicsObject->getOwnerWorld()->removePhysicsObject(physicsObject);

	m_physicsObjectList.add(physicsObject);
	physicsObject->setOwnerWorld(this);
}

//------------------------------------------------------------------------------
void PhysicsWorld::addJoint(Joint* joint)
{
	LN_ASSERT(joint != nullptr);
	m_jointList.add(joint);
	m_btWorld->addConstraint(joint->getBtConstraint());
}

//------------------------------------------------------------------------------
void PhysicsWorld::removePhysicsObject(PhysicsObjectComponent* physicsObject)
{
	if (LN_REQUIRE(physicsObject != nullptr)) return;
	if (physicsObject->getOwnerWorld() != this) return;

	m_physicsObjectList.remove(physicsObject);
	physicsObject->onRemovedFromWorld();
	physicsObject->setOwnerWorld(nullptr);
}

//------------------------------------------------------------------------------
void PhysicsWorld::gcPhysicsObjects()
{
	// rigidBody
	for (auto itr = m_physicsObjectList.begin(); itr != m_physicsObjectList.end();)
	{
		if ((*itr)->getReferenceCount() == 1)
		{
			(*itr)->onRemovedFromWorld();
			itr = m_physicsObjectList.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	// constraint
	for (auto itr = m_jointList.begin(); itr != m_jointList.end();)
	{
		if ((*itr)->getReferenceCount() == 1)
		{
			m_btWorld->removeConstraint((*itr)->getBtConstraint());
			itr = m_jointList.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


////------------------------------------------------------------------------------
//void PhysicsWorld::AddRigidBody(RigidBodyComponent* rigidBody)
//{
//	LN_FAIL_CHECK_ARG(rigidBody != nullptr) return;
//
//	PhysicsWorld* otherOwner = rigidBody->getOwnerWorld();
//	if (otherOwner != nullptr) otherOwner->RemoveRigidBody(rigidBody);
//	m_rigidBodyList.AddObject(rigidBody);
//	rigidBody->setOwnerWorld(this);
//}
//
////------------------------------------------------------------------------------
//void PhysicsWorld::RemoveRigidBody(RigidBodyComponent* rigidBody)
//{
//	LN_FAIL_CHECK_ARG(rigidBody != nullptr) return;
//
//	if (rigidBody->getOwnerWorld() == this)
//	{
//		m_rigidBodyList.RemoveObject(rigidBody);
//		rigidBody->setOwnerWorld(nullptr);
//	}
//}

//------------------------------------------------------------------------------
//detail::PhysicsWorldCore* PhysicsWorld::GetImpl() const
//{
//	return m_impl;
//}
//
//------------------------------------------------------------------------------
//void PhysicsWorld::stepSimulation(float elapsedTime)	// TODO: deltaTime? https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Engine/UWorld/Tick/index.html
//{
//	m_rigidBodyList.commit(
//		[this](RigidBodyComponent* child) { m_impl->AddRigidBodyForMmd(child); },
//		[this](RigidBodyComponent* child) { m_impl->remo(child); });
//
//	m_impl->stepSimulation(elapsedTime);
//}
//
//------------------------------------------------------------------------------
//void PhysicsWorld::AutoAddChild(RigidBodyComponent* child)
//{
//	AddRigidBody(child);
//}


//==============================================================================
// PhysicsWorld2
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PhysicsWorld2, Object);

PhysicsWorld2::PhysicsWorld2()
{
}

PhysicsWorld2::~PhysicsWorld2()
{
}

void PhysicsWorld2::initialize()
{
	Object::initialize();

	// ↓以下で出てくる単語とか
	//		http://nullorempry.jimdo.com/2012/03/10/bullet-physics%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7/

	int maxNumOutstandingTasks = 4;

	// コリジョンコンフィグ
	btDefaultCollisionConstructionInfo defaultCollisionConstructionInfo;
	//defaultCollisionConstructionInfo.m_defaultMaxPersistentManifoldPoolSize = 32768;
	m_btCollisionConfig = new btDefaultCollisionConfiguration(defaultCollisionConstructionInfo);
	// ソフトボディ使うとき
	//m_btCollisionConfig = LN_NEW btSoftBodyRigidBodyCollisionConfiguration();// 

	// コリジョンディスパッチャ
	m_btCollisionDispatcher = LN_NEW btCollisionDispatcher(m_btCollisionConfig);

	// ブロードフェーズアルゴリズム (衝突検出のためのグループ分けアルゴリズム)
	//		btDbvtBroadphase				AABB木による高速で動的な階層空間を使います。
	//		btAxisSweep3 bt32BitAxisSweep3	逐次3Dスイープ&プルーン(incremental 3D sweep and prune)を実装しています。
	//		btCudaBroadphase				GPUを使用した高速な一様グリッドを実装しています。http://bulletjpn.web.fc2.com/05_BulletCollisionDetection.html
	m_btBroadphase = LN_NEW btDbvtBroadphase();

	// ソルバ
	m_btSolver = new btSequentialImpulseConstraintSolver();

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

	// 重力		TODO: 初期値
	m_btWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));

	m_btGhostPairCallback = new btGhostPairCallback();
	m_btWorld->getPairCache()->setInternalGhostPairCallback(m_btGhostPairCallback);

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
}

void PhysicsWorld2::finalize_()
{
	gcPhysicsObjects(true);
}

void PhysicsWorld2::setGravity(const Vector3& gravity)
{
	m_btWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void PhysicsWorld2::add(PhysicsResource2* obj)
{
	if (LN_REQUIRE(obj)) return;
	m_physicsResources.add(obj);
	obj->setRemovingFromWorld(false);
	obj->setWorld(this);
}

void PhysicsWorld2::remove(PhysicsResource2* obj)
{
	if (LN_REQUIRE(obj)) return;
	obj->setRemovingFromWorld(true);
}

void PhysicsWorld2::stepSimulation(float elapsedTime)
{
	// 衝突フィルタ情報が更新されたオブジェクトは再登録
	for (auto& res : m_physicsResources)
	{
		if (res->getPhysicsResourceType() == PhysicsResourceType::CollisionBody ||
			res->getPhysicsResourceType() == PhysicsResourceType::RigidBody)
		{
			auto* obj = static_cast<PhysicsObject2*>(res.get());
			if (obj->isCollisionFilterChanged())
			{
				removeObjectInternal(obj);
				addObjectInternal(obj);
				obj->setCollisionFilterChanged(false);
			}
		}
	}

	//for (PhysicsObjectComponent* obj : m_physicsObjectList)
	//{
	//	obj->onBeforeStepSimulation();
	//}

	const float internalUnit = 1.0f / 60.0f;

	// http://d.hatena.ne.jp/ousttrue/20100425/1272165711
	// m_elapsedTime が 1.0(1秒) より小さい間は 16ms を最大 60 回繰り返して追いつこうとする設定。
	// m_elapsedTime が 1.0 を超えている場合は追いつけずに、物体の移動が遅くなる。
	m_btWorld->stepSimulation(elapsedTime, 120, 0.008333334f);

	// m_elapsedTime が 16ms より大きい場合は、1回 16ms 分のシミュレーションを可能な限り繰り返して m_elapsedTime に追いついていく設定。
	// 遅れるほど計算回数が増えるので、最終的に破綻するかもしれない。
	//m_btWorld->stepSimulation(m_elapsedTime, 1 + (int)(m_elapsedTime / internalUnit), internalUnit);

	//---------------------------------------------------------
	// 剛体の姿勢を同期 (演算結果 → ユーザー)
	//LN_FOREACH(RigidBodyComponent* b, m_rigidBodyList.GetObjectArray())
	//{
	//	b->SyncAfterStepSimulation();
	//}

	//for (PhysicsObjectComponent* obj : m_physicsObjectList)
	//{
	//	obj->onAfterStepSimulation();
	//}

	gcPhysicsObjects(false);
}

// stepSimulation() からの衝突コールバック内でオブジェクトが削除されることに備える
void PhysicsWorld2::gcPhysicsObjects(bool force)
{
	for (auto itr = m_physicsResources.begin(); itr != m_physicsResources.end();)
	{
		if (force || (*itr)->isRemovingFromWorld() || (*itr)->getReferenceCount() == 1)
		{
			itr = m_physicsResources.erase(itr);
			removeObjectInternal((*itr));
		}
		else
		{
			++itr;
		}
	}
}

void PhysicsWorld2::addObjectInternal(PhysicsResource2* obj)
{
	switch (obj->getPhysicsResourceType())
	{
	case PhysicsResourceType::RigidBody:
		m_btWorld->addRigidBody(static_cast<RigidBody2*>(obj)->getBtRigidBody());
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

void PhysicsWorld2::removeObjectInternal(PhysicsResource2* obj)
{
	switch (obj->getPhysicsResourceType())
	{
	case PhysicsResourceType::RigidBody:
		m_btWorld->addRigidBody(static_cast<RigidBody2*>(obj)->getBtRigidBody());
		break;
	default:
		LN_UNREACHABLE();
		break;
	}
}

LN_NAMESPACE_END
