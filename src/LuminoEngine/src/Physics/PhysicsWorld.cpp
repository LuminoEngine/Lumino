
#include "Internal.hpp"
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

#include <LuminoEngine/Physics/PhysicsWorld.hpp>

namespace ln {

//==============================================================================
// PhysicsDebugRenderer

class PhysicsDebugRenderer
    : public btIDebugDraw
{
protected:
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        drawLine(from, to, color, color);
    }

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override
    {
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
    }

    virtual int getDebugMode() const override
    {
        return 0;
    }

private:
};

//==============================================================================
// PhysicsWorld

PhysicsWorld::PhysicsWorld()
{
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::initialize()
{
    Object::initialize();


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

    m_debugRenderer = std::make_unique<PhysicsDebugRenderer>();
    m_btWorld->setDebugDrawer(m_debugRenderer.get());
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

void PhysicsWorld::dispose()
{
    Object::dispose();

    LN_SAFE_DELETE(m_softBodyWorldInfo);
    LN_SAFE_DELETE(m_btGhostPairCallback);
    LN_SAFE_DELETE(m_btWorld);
    LN_SAFE_DELETE(m_btSolver);
    LN_SAFE_DELETE(m_btBroadphase);
    LN_SAFE_DELETE(m_btCollisionDispatcher);
    LN_SAFE_DELETE(m_btCollisionConfig);
}

} // namespace ln

