
#include "Internal.hpp"
#pragma warning(disable: 5033)	// disable warning in bullet headers
#include <bullet/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <bullet/BulletCollision/CollisionDispatch/btManifoldResult.h>
#include <bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <bullet/BulletCollision/CollisionDispatch/btSimulationIslandManager.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <bullet/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <bullet/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>


#include <bullet/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h>

// test
#include <bullet/BulletCollision/CollisionShapes/btBoxShape.h>
#include <bullet/LinearMath/btDefaultMotionState.h>

#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoFramework/Physics/PhysicsObject.hpp>
#include <LuminoFramework/Physics/RigidBody.hpp>
#include <LuminoFramework/Physics/Joint.hpp>
#include <LuminoFramework/Physics/PhysicsWorld.hpp>
#include "PhysicsDebugRenderer.hpp"
#include "BulletUtils.hpp"

//extern ContactProcessedCallback gContactProcessedCallback;
extern ContactStartedCallback gContactStartedCallback;
extern ContactEndedCallback gContactEndedCallback;

//static bool HandleContactProcess(btManifoldPoint& p, void* a, void* b) {
//    btCollisionObject* pBody0 = (btCollisionObject*)a;
//    btCollisionObject* pBody1 = (btCollisionObject*)b;
//
//    //TestData* pUserData0 = (TestData*)pBody0->getUserPointer();
//    //TestData* pUserData1 = (TestData*)pBody1->getUserPointer();
//
//    //// カウント
//    //if (pUserData0) pUserData0->count++;
//    //if (pUserData1) pUserData1->count++;
//    printf("");
//    return true;
//}

namespace ln {

// TiggerBody (CF_NO_CONTACT_RESPONSE) 同士の衝突はこのメソッドでは検出されない。
// もともとこれは RigidBody 同士の検出をしたいために使いだしたのもなので、それだけにしておく。
// TiggerBody vs TiggerBody や TiggerBody vs RigidBody は LocalGhostObject で行う。
static void ContactStartedCallback(btPersistentManifold* const& manifold)
{
    const auto* bodyA = static_cast<const btCollisionObject*>(manifold->getBody0());
    const auto* bodyB = static_cast<const btCollisionObject*>(manifold->getBody1());
    auto* ownerA = static_cast<PhysicsObject*>(bodyA->getUserPointer());
    auto* ownerB = static_cast<PhysicsObject*>(bodyB->getUserPointer());

    // RigidBody vs TiggerBody が来ることもあるので、制限しておく
    if (ownerA && ownerB && 
        ownerA->physicsObjectType() == PhysicsObjectType::RigidBody &&
        ownerB->physicsObjectType() == PhysicsObjectType::RigidBody) {
        auto* world = ownerA->physicsWorld();
        world->postBeginContact(ownerA, ownerB);
    }
}

static void ContactEndedCallback(btPersistentManifold* const& manifold)
{
    const auto* bodyA = static_cast<const btCollisionObject*>(manifold->getBody0());
    const auto* bodyB = static_cast<const btCollisionObject*>(manifold->getBody1());
    auto* ownerA = static_cast<PhysicsObject*>(bodyA->getUserPointer());
    auto* ownerB = static_cast<PhysicsObject*>(bodyB->getUserPointer());
    if (ownerA && ownerB &&
        ownerA->physicsObjectType() == PhysicsObjectType::RigidBody &&
        ownerB->physicsObjectType() == PhysicsObjectType::RigidBody) {
        auto* world = ownerA->physicsWorld();
        world->postEndContact(ownerA, ownerB);
    }
}

//==============================================================================
// PhysicsDebugRenderer
namespace detail {

class PhysicsDebugRenderer3D
    : public btIDebugDraw
{
public:
    void init()
    {
        m_renderer.init();
    }

    void render(RenderingContext* context)
    {
        m_renderer.render(context);
    }

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
    {
        drawLine(from, to, color, color);
    }

    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor) override
    {
        const float r = 0.7;    // 少し色を濃くする
        m_renderer.drawLine(
            Vector3(from.getX(), from.getY(), from.getZ()),
            Vector3(to.getX(), to.getY(), to.getZ()),
            Color(fromColor.getX() * r, fromColor.getY() * r, fromColor.getZ() * r, 1.0f),
            Color(toColor.getX() * r, toColor.getY() * r, toColor.getZ() * r, 1.0f));
    }

    virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override
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
    int m_debugMode = 0;
    detail::PhysicsDebugRenderer m_renderer;
};

} // namespace detail

//==============================================================================
// PhysicsWorld

PhysicsWorld::PhysicsWorld()
{
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::init()
{
    Object::init();


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
    //m_btCollisionConfig = new btDefaultCollisionConfiguration(defaultCollisionConstructionInfo);
    // ソフトボディ使うとき
    m_btCollisionConfig = LN_NEW btSoftBodyRigidBodyCollisionConfiguration();// 

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
    m_btWorld = LN_NEW btSoftRigidDynamicsWorld( m_btCollisionDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfig, NULL );
    //m_btWorld = new btDiscreteDynamicsWorld(m_btCollisionDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfig);

    m_debugRenderer = std::make_unique<detail::PhysicsDebugRenderer3D>();
    m_debugRenderer->init();
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
    //m_softBodyWorldInfo->m_sparsesdf.Reset();



    //gContactProcessedCallback = HandleContactProcess;
    gContactStartedCallback = ContactStartedCallback;
    gContactEndedCallback = ContactEndedCallback;


    //// 地面作成、ワールドに追加
    //{
    //    // コリジョン形状　箱
    //    btBoxShape* ground_shape = new btBoxShape(btVector3(btScalar(80.0f), btScalar(80.0f), btScalar(80.0f)));
    //    //aCollisionShapes.push_back(ground_shape);

    //    btTransform ground_pos;
    //    ground_pos.setIdentity();
    //    ground_pos.setOrigin(btVector3(0, -85, 0));

    //    // 動かないので質量0　慣性0
    //    btScalar mass(0.0f);
    //    btVector3 inertia(0, 0, 0);

    //    btDefaultMotionState* motion_state = new btDefaultMotionState(ground_pos);
    //    btRigidBody::btRigidBodyConstructionInfo rb_cinfo(mass, motion_state, ground_shape, inertia);
    //    btRigidBody* body = new btRigidBody(rb_cinfo);
    //    m_btWorld->addRigidBody(body);
    //}
}

void PhysicsWorld::onDispose(bool explicitDisposing)
{
    for (auto& obj : m_delayAddBodies) obj->m_removing = true;
    for (auto& obj : m_delayAddJoints) obj->m_removing = true;
    for (auto& obj : m_physicsObjectList) obj->m_removing = true;
    for (auto& obj : m_jointList) obj->m_removing = true;
    updateObjectList();

    LN_SAFE_DELETE(m_softBodyWorldInfo);
    LN_SAFE_DELETE(m_btGhostPairCallback);
    LN_SAFE_DELETE(m_btWorld);
    LN_SAFE_DELETE(m_btSolver);
    LN_SAFE_DELETE(m_btBroadphase);
    LN_SAFE_DELETE(m_btCollisionDispatcher);
    LN_SAFE_DELETE(m_btCollisionConfig);

	Object::onDispose(explicitDisposing);
}

void PhysicsWorld::addPhysicsObject(PhysicsObject* physicsObject)
{
    if (LN_REQUIRE(physicsObject)) return;
    if (LN_REQUIRE(!physicsObject->physicsWorld())) return;
    m_delayAddBodies.add(physicsObject);
    physicsObject->setPhysicsWorld(this);
}

void PhysicsWorld::addJoint(Joint* joint)
{
    if (LN_REQUIRE(joint)) return;
    if (LN_REQUIRE(!joint->physicsWorld())) return;
    m_delayAddJoints.add(joint);
    joint->m_world = this;
}

void PhysicsWorld::removePhysicsObject(PhysicsObject* physicsObject)
{
    if (LN_REQUIRE(physicsObject)) return;
    if (LN_REQUIRE(physicsObject->physicsWorld() == this)) return;
    physicsObject->m_removing = true;
    //m_physicsObjectList.remove(physicsObject);
    //physicsObject->onRemoveFromPhysicsWorld();
    //physicsObject->setPhysicsWorld(nullptr);
}

void PhysicsWorld::removeJoint(Joint* joint)
{
    if (LN_REQUIRE(joint)) return;
    if (LN_REQUIRE(joint->physicsWorld() == this)) return;
    joint->m_removing = true;
}

bool PhysicsWorld::raycast(const Vector3& origin, const Vector3& direction, float maxDistance, uint32_t layerMask, bool queryTrigger, PhysicsRaycastResult* outResult)
{
    btCollisionWorld::ClosestRayResultCallback callback(
        detail::BulletUtil::LNVector3ToBtVector3(origin),
        detail::BulletUtil::LNVector3ToBtVector3(origin + direction * maxDistance));
    m_btWorld->rayTest(callback.m_rayFromWorld, callback.m_rayToWorld, callback);

    if (outResult && callback.hasHit()) {
        outResult->physicsObject = static_cast<PhysicsObject*>(callback.m_collisionObject->getUserPointer());
        outResult->point = detail::BulletUtil::btVector3ToLNVector3(callback.m_hitPointWorld);
        outResult->normal = detail::BulletUtil::btVector3ToLNVector3(callback.m_hitNormalWorld);
        outResult->distance = maxDistance * callback.m_closestHitFraction;
    }

    return callback.hasHit();
}

void PhysicsWorld::stepSimulation(float elapsedSeconds)
{
    updateObjectList();

    //ElapsedTimer t;
    for (auto& obj : m_physicsObjectList) {
    	obj->onPrepareStepSimulation();
    }

    // TODO: FPS を Engine からもらう
    const float internalTimeUnit = 1.0f / 60.0f;



    // http://d.hatena.ne.jp/ousttrue/20100425/1272165711
    // m_elapsedTime が 1.0(1秒) より小さい間は 16ms を最大 60 回繰り返して追いつこうとする設定。
    // m_elapsedTime が 1.0 を超えている場合は追いつけずに、物体の移動が遅くなる。
    // FIXME: MMD
    //m_btWorld->stepSimulation(elapsedTime, 120, 0.008333334f);

    // NOTE:
    //   tepSimulation(btScalar timeStep, int maxSubSteps, btScalar fixedTimeStep);
    //     timeStep は Game 側の経過時間。
    //     fixedTimeStep は Bullet の内部の1イテレーションの時間。
    //     fixedTimeStep が大きい場合、Bullet は (timeStep/fixedTimeStep)+1 回イテレーションを繰り返して、Game 側の時間に追い付こうとする。
    //     maxSubSteps はこの回数の最大数。
    //     
    //     例えば、fixedTimeStep が Game 側のフレームレートと一致している場合、
    //     Game 側が遅延していない場合は常にイテレーションは1回となる。
    //
    // [2020/9/15] RigidBody とカメラの位置を同期させながら速度指定で移動すると、RigidBody をデバッグ描画したときに、定期的に遅延することがあった。
    // 固定フレームレートでも発生するので、Bullet 内部で elapsedSeconds を加算していく誤差で一瞬更新されないタイミングが出たのか？という感じ。
    // イテレーションを基本2回にしてみたところ安定したので、これで様子を見てみる。
    const float iteration = 2.0f;   // int
    m_btWorld->stepSimulation(elapsedSeconds, 2, internalTimeUnit / iteration);


    // m_elapsedTime が 16ms より大きい場合は、1回 16ms 分のシミュレーションを可能な限り繰り返して m_elapsedTime に追いついていく設定。
    // 遅れるほど計算回数が増えるので、最終的に破綻するかもしれない。
    //m_btWorld->stepSimulation(m_elapsedTime, 1 + (int)(m_elapsedTime / internalUnit), internalUnit);

    processContactCommands();

    for (auto& obj : m_physicsObjectList) {
        obj->onAfterStepSimulation();
    }
    //std::cout << "tt:" << t.elapsedMilliseconds() << std::endl;
}

void PhysicsWorld::renderDebug(RenderingContext* context)
{
    //context->pushState();
    m_btWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    m_btWorld->debugDrawWorld();
    m_debugRenderer->render(context);
    //context->popState();
}

void PhysicsWorld::updateObjectList()
{
    // Delayed Add
    for (auto& obj : m_delayAddBodies) {
        m_physicsObjectList.add(obj);
    }
    for (auto& obj : m_delayAddJoints) {
        m_jointList.add(obj);
    }
    m_delayAddBodies.clear();
    m_delayAddJoints.clear();

    // Remove
    for (int i = m_physicsObjectList.size() - 1; i >= 0; i--) {
        const auto& obj = m_physicsObjectList[i];
        if (obj->m_removing) {
            obj->removeFromBtWorld();
            obj->m_removing = false;
            m_physicsObjectList.removeAt(i);
        }
    }
    for (int i = m_jointList.size() - 1; i >= 0; i--) {
        const auto& obj = m_jointList[i];
        if (obj->m_removing) {
            obj->removeFromBtWorld();
            obj->m_removing = false;
            m_jointList.removeAt(i);
        }
    }
}

void PhysicsWorld::addObjectInternal(PhysicsObject* obj)
{
    switch (obj->physicsObjectType())
    {
    case PhysicsObjectType::RigidBody:
        m_btWorld->addRigidBody(static_cast<RigidBody*>(obj)->body());
        break;
    default:
        LN_UNREACHABLE();
        break;
    }
}

//void PhysicsWorld::removeObjectInternal(PhysicsObject* obj)
//{
//    switch (obj->physicsObjectType())
//    {
//    case PhysicsObjectType::RigidBody:
//        m_btWorld->addRigidBody(static_cast<RigidBody*>(obj)->body());
//        break;
//    default:
//        LN_UNREACHABLE();
//        break;
//    }
//}
//
void PhysicsWorld::postBeginContact(PhysicsObject* self, PhysicsObject* other)
{
    if (LN_REQUIRE(self)) return;
    if (LN_REQUIRE(other)) return;
    m_contactCommands.push_back({ ContactCommandType::Begin, self, other });
}

void PhysicsWorld::postEndContact(PhysicsObject* self, PhysicsObject* other)
{
    if (LN_REQUIRE(self)) return;
    if (LN_REQUIRE(other)) return;
    m_contactCommands.push_back({ ContactCommandType::End, self, other });
}

void PhysicsWorld::processContactCommands()
{
    if (!m_contactCommands.empty()) {
        for (const auto& command : m_contactCommands) {
            switch (command.type)
            {
            case ContactCommandType::Begin:
                command.self->beginContact(command.other);
                break;
            case ContactCommandType::End:
                command.self->endContact(command.other);
                break;
            default:
                LN_UNREACHABLE();
                break;
            }
        }
        m_contactCommands.clear();
    }

    for (auto& obj : m_physicsObjectList) {
        for (auto& other : obj->m_contactBodies) {
            obj->onCollisionStay(other, nullptr);
        }
    }
}

//==============================================================================
// SpringJoint
/*
    Note:
        Damping は減衰率。0 は全く元に戻ろうとしなくなる。デフォルトは 1.0

        接合点やLimitなどは、bodyA のローカル座標系の値を扱う。
        ↓のように設定した状態で bodyA を回転させると bodyB は惑星の公転のように、bodyA の周りを振り回される。
        一方 bodyB を回転させると、bodyB だけ自転する。（そして、バネにより初期姿勢に戻るように回転しようとする）

        サイズ1x1x1のボックスを上下に安定してつなげる場合、
        m_joint1->setBodyA(m_body1, Matrix::makeTranslation(0, -0.5, 0));
        m_joint1->setBodyB(m_body2, Matrix::makeTranslation(0, 0.5, 0));
        のように、シェイプの下と上に接合点をつける。

        TODO:
        LinearLimit は Bullet のインターフェイスだとちょっとイメージしづらい気がする。
        ↑のようなbox2つを、下方向に相対-1まで伸ばせるようにしたければ
        setLinearLowerLimit(Vector3(0, -1, 0));
        とする。
        もし上方向もやりたければ setLinearUpperLimit も設定しなければならない。
        稼働「範囲」ではなく「位置」で指定するのはちょっとイメージしづらいかも。

*/

Ref<SpringJoint> SpringJoint::create()
{
    return makeObject_deprecated<SpringJoint>();
}

SpringJoint::SpringJoint()
    : PhysicsObject(PhysicsObjectType::Joint)
    , m_btDofSpringConstraint(nullptr)
{
}

void SpringJoint::init()
{
    PhysicsObject::init();
}

void SpringJoint::onDispose(bool explicitDisposing)
{
    LN_SAFE_DELETE(m_btDofSpringConstraint);
    PhysicsObject::onDispose(explicitDisposing);
}

void SpringJoint::setBodyA(RigidBody* body, const Matrix& localJunctionPoint)
{
    m_bodyA = body;
    m_localJunctionPointA = localJunctionPoint;
}

void SpringJoint::setBodyB(RigidBody* body, const Matrix& localJunctionPoint)
{
    m_bodyB = body;
    m_localJunctionPointB = localJunctionPoint;
}

void SpringJoint::setLinearLowerLimit(const Vector3& value)
{
    m_linearLowerLimit = value;
}

void SpringJoint::setLinearUpperLimit(const Vector3& value)
{
    m_linearUpperLimit = value;
}

void SpringJoint::setAngularLowerLimit(const Vector3& value)
{
    m_angularLowerLimit = value;
}

void SpringJoint::setAngularUpperLimit(const Vector3& value)
{
    m_angularUpperLimit = value;
}

void SpringJoint::setLinearStiffness(const Vector3& value)
{
    m_linearStiffness = value;
}

void SpringJoint::setAngularStiffness(const Vector3& value)
{
    m_angularStiffness = value;
}

void SpringJoint::onPrepareStepSimulation()
{
    if (LN_REQUIRE(m_bodyA)) return;
    if (LN_REQUIRE(m_bodyB)) return;

    if (!m_btDofSpringConstraint)
    {
        m_btDofSpringConstraint = new btGeneric6DofSpringConstraint(
            *m_bodyA->body(), *m_bodyB->body(),
            detail::BulletUtil::LNMatrixToBtTransform(m_localJunctionPointA),
            detail::BulletUtil::LNMatrixToBtTransform(m_localJunctionPointB),
            true);

        // 移動可能範囲
       
        m_btDofSpringConstraint->setLinearLowerLimit(detail::BulletUtil::LNVector3ToBtVector3(Vector3::min(m_linearLowerLimit, m_linearUpperLimit)));
        m_btDofSpringConstraint->setLinearUpperLimit(detail::BulletUtil::LNVector3ToBtVector3(Vector3::max(m_linearLowerLimit, m_linearUpperLimit)));

        // 回転可能範囲
        m_btDofSpringConstraint->setAngularLowerLimit(detail::BulletUtil::LNVector3ToBtVector3(Vector3::min(m_angularLowerLimit, m_angularUpperLimit)));
        m_btDofSpringConstraint->setAngularUpperLimit(detail::BulletUtil::LNVector3ToBtVector3(Vector3::max(m_angularLowerLimit, m_angularUpperLimit)));

        // 0 : translation X
        if (m_linearStiffness.x != 0.0f)
        {
            m_btDofSpringConstraint->enableSpring(0, true);
            m_btDofSpringConstraint->setStiffness(0, m_linearStiffness.x);
        }

        // 1 : translation Y
        if (m_linearStiffness.y != 0.0f)
        {
            m_btDofSpringConstraint->enableSpring(1, true);
            m_btDofSpringConstraint->setStiffness(1, m_linearStiffness.y);
        }

        // 2 : translation Z
        if (m_linearStiffness.z != 0.0f)
        {
            m_btDofSpringConstraint->enableSpring(2, true);
            m_btDofSpringConstraint->setStiffness(2, m_linearStiffness.z);
        }

        // 3 : rotation X (3rd Euler rotational around new position of X axis, range [-PI+epsilon, PI-epsilon] )
        // 4 : rotation Y (2nd Euler rotational around new position of Y axis, range [-PI/2+epsilon, PI/2-epsilon] )
        // 5 : rotation Z (1st Euler rotational around Z axis, range [-PI+epsilon, PI-epsilon] )
        m_btDofSpringConstraint->enableSpring(3, true);	m_btDofSpringConstraint->setStiffness(3, m_angularStiffness.x);
        m_btDofSpringConstraint->enableSpring(4, true);	m_btDofSpringConstraint->setStiffness(4, m_angularStiffness.y);
        m_btDofSpringConstraint->enableSpring(5, true);	m_btDofSpringConstraint->setStiffness(5, m_angularStiffness.z);

        //m_btDofSpringConstraint->setDamping(0, 10);
        //m_btDofSpringConstraint->setDamping(1, 10);
        //m_btDofSpringConstraint->setDamping(2, 10);

        m_btDofSpringConstraint->setEquilibriumPoint();

        if (m_btDofSpringConstraint) {
            physicsWorld()->getBtWorld()->addConstraint(m_btDofSpringConstraint);
        }
    }

}

void SpringJoint::onAfterStepSimulation()
{
}

void SpringJoint::removeFromBtWorld()
{
    LN_NOTIMPLEMENTED();
}

} // namespace ln

