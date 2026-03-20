#pragma once
#include <memory>
#include "Common.hpp"

#include "PhysicsObject.hpp"    // TODO: for joint

namespace ln {
class RenderingContext;
class PhysicsObject;
class RigidBody;
class PhysicsJoint;
namespace detail {
class PhysicsDebugRenderer3D;
}


struct PhysicsRaycastResult
{
    /** レイがヒットした PhysicsObject */
    Ref<PhysicsObject> physicsObject;

    /** レイがヒットしたワールド上の位置 */
    Vector3 point;

    /** レイがヒットしたワールド上のサーフェスの法線 */
    Vector3 normal;

    /** レイの原点から衝突点までの距離 */
    float distance;
};


class PhysicsWorld
	: public Object
{
public:
    void addPhysicsObject(PhysicsObject* physicsObject);
    void addJoint(Joint* joint);

    // TODO: 衝突コールバック内から呼ばれるとNG。
    // destory() でマーク付けて後で GC する仕組みにしておく。
    // ※前は addingList, removingList 作ってたけど複雑すぎた。
    void removePhysicsObject(PhysicsObject* physicsObject);
    void removeJoint(Joint* joint);

    bool raycast(const Vector3& origin, const Vector3& direction, float maxDistance/* = Math::Inf*/, uint32_t layerMask /*= 0xFFFFFFFF*/, bool queryTrigger /*= false*/, PhysicsRaycastResult* outResult = nullptr);
    bool raycast(const Vector3& origin, const Vector3& direction, float maxDistance, uint32_t layerMask, PhysicsRaycastResult* outResult = nullptr) { return raycast(origin, direction, maxDistance, layerMask, false, outResult); }


public: // TODO: internal
    //btDiscreteDynamicsWorld* getBtWorld() { return m_btWorld; }
    btSoftRigidDynamicsWorld* getBtWorld() { return m_btWorld; }
    btSoftBodyWorldInfo* softBodyWorldInfo() const { return m_softBodyWorldInfo; }
    void stepSimulation(float elapsedSeconds);
    void renderDebug(RenderingContext* context);

    void postBeginContact(PhysicsObject* self, PhysicsObject* other);
    void postEndContact(PhysicsObject* self, PhysicsObject* other);
    void processContactCommands();

LN_CONSTRUCT_ACCESS:
    PhysicsWorld();
	virtual ~PhysicsWorld();
	void init();
    virtual void onDispose(bool explicitDisposing) override;

private:
    void updateObjectList();
    void addObjectInternal(PhysicsObject* obj);
    //void removeObjectInternal(PhysicsObject* obj);

    enum class ContactCommandType
    {
        Begin,
        End,
    };

    struct ContactCommand
    {
        ContactCommandType type;
        Ref<PhysicsObject> self;
        Ref<PhysicsObject> other;
    };

    btDefaultCollisionConfiguration*		m_btCollisionConfig;
    btCollisionDispatcher*					m_btCollisionDispatcher;
    btDbvtBroadphase*						m_btBroadphase;
    //btAxisSweep3*							m_btBroadphase;
    btSequentialImpulseConstraintSolver*	m_btSolver;
    //btDiscreteDynamicsWorld*				m_btWorld;
    btSoftRigidDynamicsWorld*				m_btWorld;
    btGhostPairCallback*					m_btGhostPairCallback;
    btSoftBodyWorldInfo*					m_softBodyWorldInfo;
    std::unique_ptr<detail::PhysicsDebugRenderer3D> m_debugRenderer;


    List<Ref<PhysicsObject>> m_delayAddBodies;
    List<Ref<Joint>> m_delayAddJoints;
    List<Ref<PhysicsObject>> m_physicsObjectList;
    List<Ref<Joint>> m_jointList;

    std::vector<ContactCommand> m_contactCommands;
};


class SpringJoint
    : public PhysicsObject  // TODO: Joint は　PhysicsObject　じゃない気がする
{
public:
    static Ref<SpringJoint> create();

    void setBodyA(RigidBody* body, const Matrix& localJunctionPoint);
    void setBodyB(RigidBody* body, const Matrix& localJunctionPoint);

    /** 移動範囲の下限を設定します。 */
    void setLinearLowerLimit(const Vector3& linearLower);

    /** 移動範囲の上限を設定します。 */
    void setLinearUpperLimit(const Vector3& linearUpper);

    /** 回転の下限を設定します。 */
    void setAngularLowerLimit(const Vector3& angularLower);

    /** 回転の下限を設定します。 */
    void setAngularUpperLimit(const Vector3& angularUpper);

    /** バネの剛性 (元に戻ろうとする強さ) を設定します。 */
    void setLinearStiffness(const Vector3& value);

    void setAngularStiffness(const Vector3& value);

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onPrepareStepSimulation() override;
    virtual void onAfterStepSimulation() override;

LN_CONSTRUCT_ACCESS:
    SpringJoint();
   // ~SpringJoint();
    void init();

private:
    void removeFromBtWorld() override;

    btGeneric6DofSpringConstraint* m_btDofSpringConstraint;
    Ref<RigidBody> m_bodyA;
    Ref<RigidBody> m_bodyB;
    Matrix m_localJunctionPointA;
    Matrix m_localJunctionPointB;


    //Vector3			vec3PosLimitL;	// 移動制限1
    //Vector3			vec3PosLimitU;	// 移動制限2

    //Vector3			vec3RotLimitL;	// 回転制限1
    //Vector3			vec3RotLimitU;	// 回転制限2

    Vector3 m_linearLowerLimit;
    Vector3 m_linearUpperLimit;
    Vector3 m_angularLowerLimit;
    Vector3 m_angularUpperLimit;
    Vector3 m_linearStiffness;
    Vector3 m_angularStiffness;
};

} // namespace ln

