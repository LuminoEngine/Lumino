#pragma once

class btTransform;
class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
struct btDbvtBroadphase;
class btAxisSweep3;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionShape;
class btCompoundShape;
class btRigidBody;
class btSoftBody;
class btGhostObject;
class btGhostPairCallback;
class btTypedConstraint;
class btTriangleIndexVertexArray;
struct btSoftBodyWorldInfo;

class btCollisionObject;
class btManifoldPoint;
struct btCollisionObjectWrapper;
class btGeneric6DofSpringConstraint;

namespace ln {
class ContactPoint2D;
class PhysicsObject2D;
class RigidBody2D;
namespace detail {

class IPhysicsObjectEventListener
{
protected:
	virtual void onBeforeStepSimulation() = 0;
	virtual void onAfterStepSimulation() = 0;
    virtual void onCollisionEnter(PhysicsObject2D* otherObject, ContactPoint2D* contact) = 0;
    virtual void onCollisionLeave(PhysicsObject2D* otherObject, ContactPoint2D* contact) = 0;
    virtual void onCollisionStay(PhysicsObject2D* otherObject, ContactPoint2D* contact) = 0;

    friend class PhysicsObject2D;
	//friend class RigidBody2D;

    // Event<T> よりも Listener interface 登録のほうがメモリ効率が良い。
    // ただ、Listener interface は外部に公開するには使うのが面倒なインターフェイスなので、内部的な用途にする。
};

} // namespace detail
} // namespace ln

