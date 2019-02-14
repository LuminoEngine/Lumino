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
class RigidBody2D;
namespace detail {

class IPhysicsObjectEventListener
{
protected:
	virtual void onBeforeStepSimulation() = 0;
	virtual void onAfterStepSimulation() = 0;
	// TODO: その他、接触など

	friend class RigidBody2D;
};

} // namespace detail
} // namespace ln

