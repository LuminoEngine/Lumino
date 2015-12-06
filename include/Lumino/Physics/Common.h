
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>

LN_NAMESPACE_BEGIN
namespace Physics
{
class PhysicsManager;
class Collider;
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

/** Rigidbodyの操作軸制限 */
LN_ENUM_FLAGS(RigidBodyConstraintFlags)
{
	None				= 0x0000,		/**< 制限なし */
	FreezePositionX		= 0x0001,		/**< X 軸の移動をさせない。*/
	FreezePositionY		= 0x0002,		/**< Y 軸の移動をさせない。*/
	FreezePositionZ		= 0x0004,		/**< Z 軸の移動をさせない。*/
	FreezeRotationX		= 0x0008,		/**< X 軸の回転をさせない。*/
	FreezeRotationY		= 0x0010,		/**< Y 軸の回転をさせない。*/
	FreezeRotationZ		= 0x0020,		/**< Z 軸の回転をさせない。*/
	FreezePosition		= FreezePositionX | FreezePositionY | FreezePositionZ,		/**< 移動させない。*/
	FreezeRotation		= FreezeRotationX | FreezeRotationY | FreezeRotationZ,		/**< 回転させない。*/
	FreezeAll			= FreezePosition | FreezeRotation,		/**< 移動も回転もさせない。*/
};
LN_ENUM_FLAGS_DECLARE(RigidBodyConstraintFlags);

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
