/*
 * NamingNote: RigidBodyLimitFlags vs RigidBodyConstraintFlags
 *  UE4 参考。
 *  https://docs.unrealengine.com/ja/Engine/Physics/Constraints/ConstraintsReference/index.html
 */
#include "Internal.hpp"
#pragma warning(disable: 5033)	// disable warning in bullet headers
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <LinearMath/btMotionState.h>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/RigidBody.hpp>
#include "PhysicsManager.hpp"
#include "BulletUtils.hpp"

namespace ln {

//==============================================================================
// SynchronizeMotionState
namespace detail {

class SynchronizeMotionState
    : public btDefaultMotionState
{
public:
    RigidBody* m_owner;

    SynchronizeMotionState(RigidBody* owner, const btTransform& startTrans = btTransform::getIdentity(), const btTransform& centerOfMassOffset = btTransform::getIdentity())
        : btDefaultMotionState(startTrans, centerOfMassOffset)
        , m_owner(owner)
    {
    }

    virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override
    {
        btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
        //m_owner->setTransformFromMotionState(centerOfMassWorldTrans);
    }
};
}

//==============================================================================
// RigidBody
//   内部的なとしては、btRigidBody への各パラメータ設定はすべて遅延設定となっている。
//   これはユーザープログラム側で、シミュレーション中(衝突コールバックの中など)で
//   別のオブジェクトに対して力を与えたりするような実装に対応するため。
//   ※Urho3D とかはシミュレーション中は一切 set できないように封印されているが、
//     ユーザーにそれを気を付けてねとは言いたくない・・・。

Ref<RigidBody> RigidBody::create(CollisionShape* shape)
{
    return makeObject<RigidBody>(shape);
}

RigidBody::RigidBody()
    : PhysicsObject(PhysicsObjectType::RigidBody)
    , m_btRigidBody(nullptr)
    , m_btShapeManager()
    , m_transform()
    , m_mass(0.0f)
    , m_scale(1.0f)
    , m_group(0x00000001)
    , m_groupMask(0x0000FFFF)   // Spring とかでダミー用の衝突しない Body が必要になったときのために、後半を衝突しないものとして開けておく。（後で気づいてそれまで作ったオブジェクト全部直すのが面倒だった）
    , m_kinematicObject(false)
    , m_additionalDamping(false)
    , m_linearLimits(RigidBodyLimitFlags::None)
    , m_angularLimits(RigidBodyLimitFlags::None)
    , m_linearDamping(0.0f)
    , m_angularDamping(0.0f)
    , m_friction(0.5)
    , m_restitution(0.0f)
    , m_linearVelocity()
    , m_angularVelocity()
    , m_appliedCenterForce()
    , m_appliedCenterImpulse()
    , m_appliedTorque()
    , m_appliedTorqueImpulse()
    , m_modifiedFlags(Modified_All)
{
}

RigidBody::~RigidBody()
{
    if (m_btRigidBody != nullptr)
    {
        btMotionState* state = m_btRigidBody->getMotionState();
        LN_SAFE_DELETE(state);
        LN_SAFE_DELETE(m_btRigidBody);
    }
}

void RigidBody::init()
{
    Object::init();
    attemptAddToActiveWorld();
}

void RigidBody::init(CollisionShape* shape)
{
    init();
    addCollisionShape(shape);
}

void RigidBody::setMass(float mass)
{
    m_mass = mass;
    m_modifiedFlags |= Modified_Mass;
}

void RigidBody::setVelocity(const Vector3& velocity)
{
    m_linearVelocity = velocity;
    m_modifiedFlags |= Modified_LinearVelocity;
}

//const Vector3& RigidBody::velocity() const
Vector3 RigidBody::velocity() const
{
    // TODO: 取り方よく考える
    if (!m_btRigidBody) {
        return m_linearVelocity;
    }
    else {
        return detail::BulletUtil::btVector3ToLNVector3(m_btRigidBody->getLinearVelocity());
    }
}

void RigidBody::setAngularVelocity(const Vector3& velocity)
{
    m_angularVelocity = velocity;
    m_modifiedFlags |= Modified_AngularVelocity;
}

void RigidBody::setLinearLimits(Flags<RigidBodyLimitFlags> flags)
{
    m_linearLimits = flags;
    m_modifiedFlags |= Modified_LimittFlags;
}

void RigidBody::setAngularLimits(Flags<RigidBodyLimitFlags> flags)
{
    m_angularLimits = flags;
    m_modifiedFlags |= Modified_LimittFlags;
}

void RigidBody::setLinearDamping(float damping)
{
    m_linearDamping = damping;
    m_modifiedFlags |= Modified_UniformParams;
}

void RigidBody::setAngularDamping(float damping)
{
    m_angularDamping = damping;
    m_modifiedFlags |= Modified_UniformParams;
}

void RigidBody::setFriction(float friction)
{
    m_friction = friction;
    m_modifiedFlags |= Modified_UniformParams;
}

void RigidBody::setRestitution(float restitution)
{
    m_restitution = restitution;
    m_modifiedFlags |= Modified_UniformParams;
}

void RigidBody::setKinematic(bool enabled)
{
    m_kinematicObject = enabled;
    m_modifiedFlags |= Modified_ReaddToWorld;
}

void RigidBody::setCollisionGroup(uint32_t group)
{
    m_group = group;
    m_modifiedFlags |= Modified_ReaddToWorld;
}

void RigidBody::setCollisionGroupMask(uint32_t groupMask)
{
    m_groupMask = groupMask;
    m_modifiedFlags |= Modified_ReaddToWorld;
}

void RigidBody::setTransform(const Matrix& transform)
{
    m_transform = transform;
    m_modifiedFlags |= Modified_WorldTransform;
}

void RigidBody::applyForce(const Vector3& force)
{
    m_appliedCenterForce += force;
    m_modifiedFlags |= Modified_ApplyCenterForce;
    activate();
}

void RigidBody::applyForce(const Vector3& force, const Vector3& localPosition)
{
    // see btRigidBody::applyForce
    applyForce(force);
    applyTorque(Vector3::cross(localPosition, force * getLinearFactor()));   // TODO: force * m_linearFactor
}

void RigidBody::applyImpulse(const Vector3& impulse)
{
    m_appliedCenterImpulse += impulse;
    m_modifiedFlags |= Modified_ApplyCenterImpulse;
    activate();
}

void RigidBody::applyImpulse(const Vector3& impulse, const Vector3& localPosition)
{
    // see btRigidBody::applyImpulse
    applyImpulse(impulse);
    applyTorqueImpulse(Vector3::cross(localPosition, impulse * getLinearFactor()));
}

void RigidBody::applyTorque(const Vector3& torque)
{
    m_appliedTorque += torque;
    m_modifiedFlags |= Modified_ApplyCenterForce;
}

void RigidBody::applyTorqueImpulse(const Vector3& torque)
{
    m_appliedTorqueImpulse += torque;
    m_modifiedFlags |= Modified_ApplyTorqueImpulse;
}

void RigidBody::clearForces()
{
    m_modifiedFlags |= Modified_ClearForces;
    m_modifiedFlags &= ~Modified_ApplyCenterForce;
    m_modifiedFlags &= ~Modified_ApplyCenterImpulse;
    m_modifiedFlags &= ~Modified_ApplyTorque;
    m_modifiedFlags &= ~Modified_ApplyTorqueImpulse;
    m_appliedCenterForce = Vector3::Zero;
    m_appliedCenterImpulse = Vector3::Zero;
    m_appliedTorque = Vector3::Zero;
    m_appliedTorqueImpulse = Vector3::Zero;
}

void RigidBody::addCollisionShape(CollisionShape* shape)
{
    m_btShapeManager.addShape(shape);
}

void RigidBody::attemptAddToActiveWorld()
{
    if (auto& activeWorld = detail::EngineDomain::physicsManager()->activePhysicsWorld()) {
        activeWorld->addPhysicsObject(this);
    }
}

void RigidBody::activate()
{
    m_modifiedFlags |= Modified_Activate;
}

void RigidBody::onPrepareStepSimulation()
{
    // RigidBodyComponent::onBeforeStepSimulation() で WorldObject の姿勢を this に同期した後
    // 以降の処理を行いたいので、先に実行しておく。
    PhysicsObject::onPrepareStepSimulation();

    // TODO: KinematicObject が Component と関連づいている場合、ここで transform を設定
    //auto* transform = getTransform();
    //if (transform != nullptr)
    //{
    //    if (m_data.KinematicObject)
    //    {
    //        m_data.InitialTransform = transform->getWorldMatrix();
    //    }
    //}

    if ((m_modifiedFlags & Modified_InitialUpdate) != 0)
    {
        createBtRigidBody();
        m_modifiedFlags &= ~Modified_ReaddToWorld;
    }

    // setWorldTransform 要求
    if ((m_modifiedFlags & Modified_WorldTransform) != 0 | m_kinematicObject)
    {
        /*
            stepSimulation() の中でこれらが関係する処理は以下のとおり。
                Kinematic(質量が0)な物体の姿勢を MotionState から btCollisionObject::m_worldTransform に取得 getWorldTransform()
                    btCollisionObject::m_worldTransform は setWorldTransform からも設定できるので、MotionState を使う意味はあまり無い。
                    ただ、m_updateRevision が変わるのでどう影響するか見切れないところ。黙って MotionState 経由にしておくのが無難かも。
                ↓
                シミュレーション実行
                    btCollisionObject::m_worldTransform は更新される。
                    btCollisionObject::m_interpolationWorldTransform も更新される。setCenterOfMassTransform() 経由で。
                    setCenterOfMassTransform() は stepSimulation() の下で呼ばれている。
                ↓
                Kinematicではない物体の姿勢を MotionState に set。
                    このとき、何かの時間を使って m_interpolationWorldTransform を補完し、その結果を MotionState に set している。
                    なので、btCollisionObject::m_worldTransform とも btCollisionObject::m_interpolationWorldTransform とも違う姿勢が渡されてくることになり、
                    これがシミュレーションの最終結果となる。

            なお、ソースを見た限りだと btMotionState::getWorldTransform() は Kinematic なオブジェクトでしか呼ばれない。
            普通のオブジェクトは btCollisionObject::setWorldTransform() にて姿勢を設定する必要がある。

            念のため…ということで、両方の setWorldTransform() を行い、様子を見てみる。
        */
        btTransform transform;
        transform.setFromOpenGLMatrix((btScalar*)&m_transform);
        m_btRigidBody->setWorldTransform(transform);

        if (m_btRigidBody != nullptr)
        {
            m_btRigidBody->getMotionState()->setWorldTransform(transform);
        }
    }

    if (m_btRigidBody != nullptr)
    {
        // RigidbodyConstraints
        if ((m_modifiedFlags & Modified_LimittFlags) != 0)
        {
            m_btRigidBody->setLinearFactor(detail::BulletUtil::LNVector3ToBtVector3(getLinearFactor()));
            m_btRigidBody->setAngularFactor(detail::BulletUtil::LNVector3ToBtVector3(getAngularFactor()));
        }

        // Mass
        if ((m_modifiedFlags & Modified_Mass) != 0)
        {
            /* Bullet は StaticObject はシミュレーションから完全に除外している。
            * StaticObject でないものは World の m_nonStaticRigidBodies に追加されてシミュレーションの
            * 対象となるのだが、このリストに追加されるのは addRigidBody() された時のみ。
            * 後から StaticObject であるフラグが変更されても、再度 addRigidBody() しなければ
            * シミュレーションの対象とはならない。
            * ここでは、setMassProps() の前後でフラグ isStaticObject() が変わった場合、World に追加しなおしている。
            */
            bool isStatic = m_btRigidBody->isStaticObject();
            if (m_mass != 0.0f)
            {
                btVector3 inertia;
                m_btRigidBody->getCollisionShape()->calculateLocalInertia(m_mass, inertia);
                m_btRigidBody->setMassProps(m_mass, inertia);
            }


            if (isStatic != m_btRigidBody->isStaticObject())
            {
                readdToWorld();
            }
        }

        if ((m_modifiedFlags & Modified_LinearVelocity) != 0)
        {
            m_btRigidBody->setLinearVelocity(detail::BulletUtil::LNVector3ToBtVector3(m_linearVelocity));
        }
        if ((m_modifiedFlags & Modified_AngularVelocity) != 0)
        {
            m_btRigidBody->setAngularVelocity(detail::BulletUtil::LNVector3ToBtVector3(m_angularVelocity));
        }
        if ((m_modifiedFlags & Modified_UniformParams) != 0)
        {
            m_btRigidBody->setDamping(m_linearDamping, m_angularDamping);
            m_btRigidBody->setFriction(m_friction);
            m_btRigidBody->setRestitution(m_restitution);
        }

        if ((m_modifiedFlags & Modified_ReaddToWorld) != 0)
        {
            readdToWorld();
        }

        // clearForces 要求
        if ((m_modifiedFlags & Modified_ClearForces) != 0)
        {
            m_btRigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            m_btRigidBody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
            //m_btRigidBody->setInterpolationLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            //m_btRigidBody->setInterpolationAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
            //m_btRigidBody->setInterpolationWorldTransform(m_btRigidBody->getCenterOfMassTransform());
            m_btRigidBody->clearForces();
        }

        if ((m_modifiedFlags & Modified_ApplyCenterForce) != 0)
        {
            m_btRigidBody->applyCentralForce(detail::BulletUtil::LNVector3ToBtVector3(m_appliedCenterForce));
            m_appliedCenterForce = Vector3::Zero;
        }
        if ((m_modifiedFlags & Modified_ApplyCenterImpulse) != 0)
        {
            m_btRigidBody->applyCentralImpulse(detail::BulletUtil::LNVector3ToBtVector3(m_appliedCenterImpulse));
            m_appliedCenterImpulse = Vector3::Zero;
        }
        if ((m_modifiedFlags & Modified_ApplyCenterImpulse) != 0)
        {
            m_btRigidBody->applyTorque(detail::BulletUtil::LNVector3ToBtVector3(m_appliedTorque));
            m_appliedTorque = Vector3::Zero;
        }
        if ((m_modifiedFlags & Modified_ApplyTorqueImpulse) != 0)
        {
            m_btRigidBody->applyTorqueImpulse(detail::BulletUtil::LNVector3ToBtVector3(m_appliedTorqueImpulse));
            m_appliedTorqueImpulse = Vector3::Zero;
        }
    }

    // activate 要求
    if ((m_modifiedFlags & Modified_Activate) != 0)
    {
        m_btRigidBody->activate();
    }

    m_modifiedFlags = Modified_None;
}

void RigidBody::onAfterStepSimulation()
{
    if (!m_kinematicObject)
    {
        btTransform transform;
        m_btRigidBody->getMotionState()->getWorldTransform(transform);
        transform.getOpenGLMatrix((btScalar*)&m_transform);
    }

    PhysicsObject::onAfterStepSimulation();
}

void RigidBody::removeFromBtWorld()
{
    physicsWorld()->getBtWorld()->removeRigidBody(m_btRigidBody);
}

void RigidBody::createBtRigidBody()
{
    btCollisionShape* shape = m_btShapeManager.getBtCollisionShape();


    // 各初期プロパティ
    float num = m_mass * m_scale;
    float friction;
    float hitFraction;
    float linearDamping;
    float angularDamping;
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if (m_kinematicObject)
    {
        num = 0.0f;
        friction = m_friction;
        hitFraction = m_restitution;
        linearDamping = m_linearDamping;
        angularDamping = m_angularDamping;
    }
    else
    {
        if (num != 0.0f)
        {
            shape->calculateLocalInertia(num, localInertia);
        }
        friction = m_friction;
        hitFraction = m_restitution;
        linearDamping = m_linearDamping;
        angularDamping = m_angularDamping;
    }

    // 初期姿勢と MotionState
    btTransform initialTransform;
    {
        initialTransform.setFromOpenGLMatrix((const btScalar*)&m_transform);
        initialTransform.getOrigin().setX(initialTransform.getOrigin().x() * m_scale);
        initialTransform.getOrigin().setY(initialTransform.getOrigin().y() * m_scale);
        initialTransform.getOrigin().setZ(initialTransform.getOrigin().z() * m_scale);
    }

    btMotionState* motionState;
    if (m_kinematicObject)
    {
        motionState = new detail::SynchronizeMotionState(this, initialTransform);
    }
    else
    {
        motionState = new detail::SynchronizeMotionState(this, initialTransform);
    }

    // RigidBodyComponent 作成
    btRigidBody::btRigidBodyConstructionInfo bodyInfo(num, motionState, shape, localInertia);
    bodyInfo.m_linearDamping = linearDamping;
    bodyInfo.m_angularDamping = angularDamping;
    bodyInfo.m_restitution = m_restitution;
    bodyInfo.m_friction = friction;
    bodyInfo.m_additionalDamping = m_additionalDamping;
    m_btRigidBody = new btRigidBody(bodyInfo);

    if (m_kinematicObject)
    {
        // CF_KINEMATIC_OBJECT と DISABLE_DEACTIVATION はセット。決まり事。
        // http://bulletjpn.web.fc2.com/07_RigidBodyDynamics.html
        m_btRigidBody->setCollisionFlags( /*m_btRigidBody->getCollisionFlags() | */btCollisionObject::CF_KINEMATIC_OBJECT);
        // TODO: フラグ、よくない気がする
        m_btRigidBody->setActivationState( /*m_btRigidBody->getActivationState() | */DISABLE_DEACTIVATION);
    }
    else
    {
        // TODO: フラグ、よくない気がする
        //m_btRigidBody->setActivationState( /*m_btRigidBody->getActivationState() | */DISABLE_DEACTIVATION);
    }
    // TODO: よくない気がする
    m_btRigidBody->setSleepingThresholds(0.0f, 0.0f);

    m_modifiedFlags |= Modified_Activate;

    //BodyBase::init(m_btRigidBody);
    m_btRigidBody->setUserPointer(this);

    addToWorld();

}

void RigidBody::setTransformFromMotionState(const btTransform& transform)
{
    //if (m_childCollisionBody != nullptr)
    //{
    //    // トリガーオブジェクトの姿勢を transform(剛体) に同期する
    //    m_childCollisionBody->setTransformFromMotionState(transform);
    //}
}

void RigidBody::addToWorld()
{
    if (m_btRigidBody) {
        physicsWorld()->getBtWorld()->addRigidBody(m_btRigidBody, m_group, m_groupMask);
    }
}

void RigidBody::readdToWorld()
{
    if (m_btRigidBody) {
        physicsWorld()->getBtWorld()->removeRigidBody(m_btRigidBody);
        physicsWorld()->getBtWorld()->addRigidBody(m_btRigidBody, m_group, m_groupMask);
    }
}

Vector3 RigidBody::getLinearFactor() const
{
    return Vector3(
        m_linearLimits.hasFlag(RigidBodyLimitFlags::LockedPositionX) ? 0.0f : 1.0f,
        m_linearLimits.hasFlag(RigidBodyLimitFlags::LockedPositionY) ? 0.0f : 1.0f,
        m_linearLimits.hasFlag(RigidBodyLimitFlags::LockedPositionZ) ? 0.0f : 1.0f);
}

Vector3 RigidBody::getAngularFactor() const
{
    return Vector3(
        m_angularLimits.hasFlag(RigidBodyLimitFlags::LockedRotationX) ? 0.0f : 1.0f,
        m_angularLimits.hasFlag(RigidBodyLimitFlags::LockedRotationY) ? 0.0f : 1.0f,
        m_angularLimits.hasFlag(RigidBodyLimitFlags::LockedRotationZ) ? 0.0f : 1.0f);
}

} // namespace ln

