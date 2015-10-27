
#pragma once

LN_NAMESPACE_BEGIN
namespace Physics
{
class PhysicsManager;
class BodyBase;
class RigidBody;
class CollisionBody;
class Joint;
class IDebugRenderer;

enum SimulationType
{
	SimulationType_Sync = 0,
	SimulationType_ASync,
};

/// Body の種類
enum BodyType
{
	BodyType_Unknown = 0,
	BodyType_RigidBody,
	BodyType_SoftBody,
	BodyType_CollisionBody,
	BodyType_CharacterObject,
	BodyType_Joint,

	BodyType_Max,
};

/// 非同期シミュレーションの実行状態
enum ASyncSimulationState
{
	ASyncSimulationState_Idling = 0,	///< 待機状態
	ASyncSimulationState_Runnning,		///< 実行中
};

} // namespace Physics
LN_NAMESPACE_END


class btDynamicsWorld;
class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btAxisSweep3;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btTriangleIndexVertexArray;
struct btSoftBodyWorldInfo;

class btCollisionObject;
class btManifoldPoint;
struct btCollisionObjectWrapper;
class btGeneric6DofSpringConstraint;
