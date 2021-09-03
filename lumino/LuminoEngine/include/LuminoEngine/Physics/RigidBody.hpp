#pragma once
#include "Common.hpp"
#include "PhysicsObject.hpp"
#include "CollisionShape.hpp"

namespace ln {
namespace detail {
class SynchronizeMotionState;
}

enum class RigidBodyLimitFlags
{
    /** 制限なし */
    None = 0x0000,

    /** X 軸の移動をさせない */
    LockedPositionX = 0x0001,

    /** Y 軸の移動をさせない */
    LockedPositionY = 0x0002,

    /** Z 軸の移動をさせない */
    LockedPositionZ = 0x0004,

    /** X 軸の回転をさせない */
    LockedRotationX = 0x0010,

    /** Y 軸の回転をさせない */
    LockedRotationY = 0x0020,

    /** Z 軸の回転をさせない */
    LockedRotationZ = 0x0040,

    /** 移動させない */
    LockedPosition = LockedPositionX | LockedPositionY | LockedPositionZ,

    /** 回転させない */
    LockedRotation = LockedRotationX | LockedRotationY | LockedRotationZ,

    /** 移動と回転をさせない */
    LockedAll = LockedPosition | LockedRotation,
};
LN_FLAGS_OPERATORS(RigidBodyLimitFlags);


// Note: onCollisionEnter() などの 衝突コールバックは RigidBody vs RigidBody でも使用できるが、
// 移動中は物理エンジンによって通常、接触⇔押し出し が繰り返されるため、毎フレーム Enter, Leave, Stay が 呼ばれる点に注意。
class RigidBody
	: public PhysicsObject
{
public:
    /** 指定した形状で、質量が 0 である静的な RigidBody を作成します。 */
    static Ref<RigidBody> create(CollisionShape* shape);

    RigidBody();
    ~RigidBody();
    void init();
    void init(CollisionShape* shape);



    ///** 位置を設定します。(default: 0, 0) */
    //void setPosition(float x, float y, float z) { return setPosition(Vector3(x, y, z)); }
    //void setPosition(const Vector3& value);

    //const Vector3& position() const { return m_position; }





    /** 質量を設定します。0 を設定すると静的なボディとなります。 */
    void setMass(float mass);

    /** 速度を設定します。単位は m/s です。 */
    void setVelocity(const Vector3& velocity);

    //const Vector3& velocity() const;
    Vector3 velocity() const;

    /** 角速度を設定します。 */
    void setAngularVelocity(const Vector3& velocity);

    /** 物理演算による各軸の移動の制約を設定します。*/
    void setLinearLimits(Flags<RigidBodyLimitFlags> flags);

    /** 物理演算による各軸の回転の制約を設定します。*/
    void setAngularLimits(Flags<RigidBodyLimitFlags> flags);

    /** 速度減衰係数を設定します。 */
    void setLinearDamping(float damping);

    /** 角速度減衰係数を設定します。 */
    void setAngularDamping(float damping);

    /** 摩擦係数を設定します。 */
    void setFriction(float friction);

    /** 反発係数を設定します。 */
    void setRestitution(float restitution);


    /** キネマティックモードを設定します。 キネマティックモードでは、剛体に力はかかりません。 */
    void setKinematic(bool enabled);

    bool isDynamic() const { return !isStatic() && !isKinematic(); }
    bool isStatic() const { return m_mass == 0.0f; }
    bool isKinematic() const { return m_kinematicObject; }

    /** 衝突グループを設定します。デフォルトは 0x00000001 で、0番のグループに属することを示します。 */
    void setCollisionGroup(uint32_t group);

    /** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroupMask(uint32_t groupMask);

    void setTransform(const Matrix& transform);
    const Matrix& transform() const { return m_transform; }

    /** 重心に力を加えます。 */
    void applyForce(const Vector3& force);

    /** 指定したローカル位置に力を加えます。 */
    void applyForce(const Vector3& force, const Vector3& localPosition);
    
    /** 重心に衝撃を与えます。 */
    void applyImpulse(const Vector3& impulse);
    
    /** 指定したローカル位置に衝撃を与えます。 */
    void applyImpulse(const Vector3& impulse, const Vector3& localPosition);

    /** トルクをかけます。 */
    void applyTorque(const Vector3& torque);

    /** トルク衝撃を与えます。 */
    void applyTorqueImpulse(const Vector3& torque);

    /** 累積力をリセットします。 */
    void clearForces();


    void addCollisionShape(CollisionShape* shape);


    // TODO:
    void setLinearRestThreshold(float threshold);
    void setAngularRestThreshold(float threshold);
    void setRollingFriction(float friction);
    void setContactProcessingThreshold(float threshold);
    void setCcdRadius(float radius);
    void setCcdMotionThreshold(float threshold);
    void setTrigger(bool enable);

	// TODO: internal
    btRigidBody* body() const { return m_btRigidBody; }

protected:
    void onPrepareStepSimulation() override;
    void onAfterStepSimulation() override;

private:
    void removeFromBtWorld() override;
    void attemptAddToActiveWorld();
    void activate();
    void createBtRigidBody();
    void setTransformFromMotionState(const btTransform& transform);
    void addToWorld();
    void readdToWorld();
    Vector3 getLinearFactor() const;
    Vector3 getAngularFactor() const;

    enum ModifiedFlags
    {
        Modified_None = 0x0000,
        Modified_Activate = 0x0001,
        Modified_WorldTransform = 0x0002,
        Modified_ClearForces = 0x0004,
        Modified_Mass = 0x0008,
        Modified_LimittFlags = 0x0040,
        Modified_Colliders = 0x0080,
        Modified_LinearVelocity = 0x1000,
        Modified_AngularVelocity = 0x2000,
        Modified_UniformParams = 0x4000,
        Modified_InitialUpdate = 0x8000,

        Modified_ReaddToWorld = 0x10000,

        Modified_ApplyCenterForce = 0x0010,
        Modified_ApplyCenterImpulse = 0x0020,
        Modified_ApplyTorque = 0x20000,
        Modified_ApplyTorqueImpulse = 0x40000,
        Modified_All = 0xFFFFFFFF,
    };

    btRigidBody* m_btRigidBody;
    detail::BtShapeManager m_btShapeManager;

    Matrix m_transform;

    // ReaddToWorld
    float m_mass;				///< 質量
    float m_scale;
    uint32_t m_group;				///< 衝突グループ
    uint32_t m_groupMask;			///< 非衝突グループ
    bool m_kinematicObject;	///< Kinematicオブジェクトとする (質量が 0.0 として扱われ、MotionState の getWorldTransform() が呼ばれるようになる)
    bool m_additionalDamping;	///< 減衰の有効

    Flags<RigidBodyLimitFlags> m_linearLimits;
    Flags<RigidBodyLimitFlags> m_angularLimits;

    // UniformParams
    float m_linearDamping;
    float m_angularDamping;
    float m_friction;
    float m_restitution;

    Vector3 m_linearVelocity;
    Vector3 m_angularVelocity;
    Vector3 m_appliedCenterForce;
    Vector3 m_appliedCenterImpulse;
    Vector3 m_appliedTorque;
    Vector3 m_appliedTorqueImpulse;

    uint32_t m_modifiedFlags;

    friend class detail::SynchronizeMotionState;
    friend class PhysicsWorld;
};

} // namespace ln

