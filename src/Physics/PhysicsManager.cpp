/*
	[2015/5/13] Bullet の btCollisionObject::get/setWorldTransform と btMotionState::get/setWorldTransform について

		btCollisionObject の m_worldTransform は、初期姿勢または setCenterOfMassTransform() で set されるだけ。
		Bullet の演算結果が格納されるものではない。
		さらに、初期姿勢は btMotionState のポインタが指定されていれば btMotionState の姿勢が使われる。

		とりあえず簡単にまとめると、btMotionState のほうが偉い。



	[2015/5/1]
		
		Bullet 自身にもマルチスレッド機能や GPGPU 機能がある。
		http://nullorempry.jimdo.com/2012/03/10/bullet-physics%E3%81%AE%E3%81%8A%E5%8B%89%E5%BC%B7/
		ただ、2.72 では実装されてなさそう。もし MMD に近づけるなら自前で何か作る必要があるかもしれない。

		・bullet のマルチスレッド機能に頼る
		・シングルスレッド用の物理演算を、自前でマルチスレッドで包む
		


		MMD モデルについて・・・

			①剛体をボーン位置に移動
			②物理演算
			③ボーンを剛体位置に移動
			④描画

			描画を正しく行うためには、物理演算と描画が1フレーム内で直列に実行されなければならない。
			並列にしてしまうと1フレーム前の物理演算結でボーン更新を行うことになるので、モデルが高速移動したときに伸びてしまう。
			LightNote ではやむなく、物理演算とScene&GUI の更新を並列にして、描画とは直列にしていた。

			どうしても物理演算を並列化するなら、③ にて物理演算が完了していなければ、
			親ボーンの前回フレームからのオフセットを考慮して自分で移動する必要がある。
			→ 前回フレームというより、最後の①から。①でも、完了していなければ剛体位置を移動しないようにする必要がある。



		Model クラスのインターフェイスは？

			Model::GetBones();
			Model::GetMaterials();

			MotionController::AdvanceTime();

			UserUpdate();

			Model::UpdateBonesBeforePhysics();

			PhysicsManager::StepSimulation();

			Model::UpdateBonesAfterPhysics();

			Model::GetSubsetCount();
			Model::DrawSubset();


		まずは同期モードオンリーでいいと思う。前と同じ。
		bullet のマルチスレッド機能で改善できるならそれでいいし。
		まず一発作ってみてベンチマークして、それで改善の余地があれば検討する、で。

		ちなみにライブラリ側でマルチスレッド対応するとなったら、
		プロパティはすべて保持モード。StepSimu 直前でコミット&bulletオブジェクト作成とかがひつよう。
		ソフトボディはノード単位でも保持する必要がある。


		MMD モデルのマルチスレッド物理更新について・・・
			
			要求した StepSim が完了していなければ親ボーンの姿勢を継承する、いつもの階層更新を行う。
			ポイントは、どうやって完了しているのをチェックするか。

			・単純に、ボーン更新時に物理スレッドが待機中か調べ、待機中の時だけ更新する
				→	NG。API をシングルスレッドモードと同じにしたい場合、ボーン更新タイミングは StepSim をリクエストした直後である。
				StepSim も同様に「待機中であれば実行」の処理が行われる訳で、その直後にあるボーン更新は高確率で処理が行われない異なる。

			・要求した StepSim が完了しているか調べる (ボーン更新時、後に要求された StepSim は動いているかもしれない)
				→ できたとしても、ボーン更新に使われる剛体姿勢は１フレーム前のものである。

*/

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
#include <Lumino/Physics/PhysicsManager.h>

namespace Lumino
{
namespace Physics
{

//=============================================================================
// DebugDrawer
//=============================================================================
class PhysicsManager::DebugDrawer
	: public btIDebugDraw
{
public:

	DebugDrawer()
		: mIDebugRenderer(NULL)
		, mDebugMode(0)
	{}
	virtual ~DebugDrawer() {}

public:

	void setDebugRenderer(IDebugRenderer* r) { mIDebugRenderer = r; }

public:

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
	{
		mIDebugRenderer->DrawLine(
			Vector3(from.getX(), from.getY(), from.getZ()),
			Vector3(to.getX(), to.getY(), to.getZ()),
			Vector3(fromColor.getX(), fromColor.getY(), fromColor.getZ()),
			Vector3(toColor.getX(), toColor.getY(), toColor.getZ()));
	}

	virtual void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color, color);
	}

	virtual void	drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
	{
	}

	virtual void	drawBox(const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
	{
	}

	virtual void	drawTriangle(const btVector3& a, const btVector3& b, const btVector3& c, const btVector3& color, btScalar alpha)
	{
	}

	virtual void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
	}

	virtual void	reportErrorWarning(const char* warningString)
	{
		printf("Physics: %s\n", warningString);
	}

	virtual void	draw3dText(const btVector3& location, const char* textString)
	{
	}

	virtual void	setDebugMode(int debugMode) { mDebugMode = debugMode; }

	virtual int		getDebugMode() const { return mDebugMode; }

protected:

	IDebugRenderer* mIDebugRenderer;
	int mDebugMode;
};

//=============================================================================
// PhysicsManager
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

PhysicsManager::PhysicsManager(SimulationType type)
	: m_simulationType(type)
	, m_asyncSimulationState(ASyncSimulationState_Idling)
	, m_elapsedTime(0.0f)
	, m_debugDrawer(NULL)
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
		btDbvtBroadphase
			AABB木による高速で動的な階層空間を使います。
		btAxisSweep3 bt32BitAxisSweep3
			逐次3Dスイープ&プルーン(incremental 3D sweep and prune)を実装しています。
		btCudaBroadphase
			GPUを使用した高速な一様グリッドを実装しています。
			http://bulletjpn.web.fc2.com/05_BulletCollisionDetection.html
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

	m_debugDrawer = LN_NEW DebugDrawer();
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
	m_btWorld->setGravity(btVector3(0.0f, -9.8f * 2.5f, 0.0f));
	//SetGravity(Vector3(0.0f, -9.8f * 2.5f, 0.0f));
	//SetGravity( LVector3( 0.0f, /*-9.8f*/-9.81f * 10.0f/* * 2.0f*/, 0.0f ) );

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

PhysicsManager::~PhysicsManager()
{
	LN_SAFE_DELETE(m_btCollisionConfig);
	LN_SAFE_DELETE(m_btCollisionDispatcher);
	LN_SAFE_DELETE(m_btBroadphase);
	LN_SAFE_DELETE(m_btSolver);
	LN_SAFE_DELETE(m_btWorld);
	LN_SAFE_DELETE(m_softBodyWorldInfo);

#ifdef LN_USE_PARALLEL
	if (m_threadSupportSolver)
	{
		delete m_threadSupportSolver;
	}
	if (m_threadSupportCollision)
	{
		delete m_threadSupportCollision;
	}
#endif

	LN_SAFE_DELETE(m_debugDrawer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::SyncBeforeStepSimulation()
{
	if (m_simulationType == SimulationType_ASync)
	{
		// 終わるまで待つ
	}

	//---------------------------------------------------------
	// Add～ で追加された新規オブジェクトをワールドに登録

	//// CollisionBody
	//CollisionBodyArray::ObjectArray& collisionArray = mCollisionBodyArray.getRegistOrderObjectArray();
	//ln_foreach(CollisionBody* b, collisionArray)
	//{
	//	m_btWorld->addCollisionObject(b->getBtCollisionObject(), b->getGroup(), b->getGroupMask());
	//}
	// RigidBody
	LN_FOREACH(RigidBody* obj, m_rigidBodyList.GetRegistOrderObjectArray())
	{
		m_btWorld->addRigidBody(obj->GetBtRigidBody(), obj->GetGroup(), obj->GetGroupMask());
	}
	// Joint
	LN_FOREACH(Joint* obj, m_jointList.GetRegistOrderObjectArray())
	{
		m_btWorld->addConstraint(obj->GetBtConstraint());
	}


	//m_collisionBodyList.Commit();
	m_rigidBodyList.Commit();
	m_jointList.Commit();

	//---------------------------------------------------------
	// 剛体の姿勢を同期

	LN_FOREACH(RigidBody* b, m_rigidBodyList.GetObjectArray())
	{
		b->SyncBeforeStepSimulation();
	}

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::StepSimulation(float elapsedTime)
{
	SyncBeforeStepSimulation();

	if (m_simulationType == SimulationType_Sync)
	{
		m_elapsedTime = elapsedTime;
		StepSimulationInternal();
	}
	else
	{
		LN_THROW(m_asyncSimulationState == ASyncSimulationState_Idling, InvalidOperationException);	// 待機状態になっていなければならない

		m_elapsedTime = elapsedTime;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::DrawDebugShapes(IDebugRenderer* renderer)
{
	m_debugDrawer->setDebugRenderer(renderer);
	m_btWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_btWorld->debugDrawWorld();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::StepSimulationInternal()
{
	//---------------------------------------------------------
	// StepSimulation

	const float internalUnit = 1.0f / 60.0f;

	// http://d.hatena.ne.jp/ousttrue/20100425/1272165711
	// m_elapsedTime が 1.0(1秒) より小さい間は 16ms を最大 60 回繰り返して追いつこうとする設定。
	// m_elapsedTime が 1.0 を超えている場合は追いつけずに、物体の移動が遅くなる。
	m_btWorld->stepSimulation(m_elapsedTime, 60, internalUnit);

	// m_elapsedTime が 16ms より大きい場合は、1回 16ms 分のシミュレーションを可能な限り繰り返して m_elapsedTime に追いついていく設定。
	// 遅れるほど計算回数が増えるので、最終的に破綻するかもしれない。
	//m_btWorld->stepSimulation(m_elapsedTime, 1 + (int)(m_elapsedTime / internalUnit), internalUnit);


	//---------------------------------------------------------
	// 剛体の姿勢を同期 (演算結果 → ユーザー)
	LN_FOREACH(RigidBody* b, m_rigidBodyList.GetObjectArray())
	{
		b->SyncAfterStepSimulation();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::AddRigidBody(RigidBody* body)
{
	m_rigidBodyList.AddObject(body);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PhysicsManager::AddJoint(Joint* joint)
{
	m_jointList.AddObject(joint);
}

} // namespace Physics
} // namespace Lumino
