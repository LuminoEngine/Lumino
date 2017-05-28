
#pragma once
#include <Lumino/Reflection/ReflectionObject.h>

class btTransform;
class btDynamicsWorld;
class  btDefaultCollisionConfiguration;
class  btCollisionDispatcher;
struct btDbvtBroadphase;
class  btAxisSweep3;
class  btSequentialImpulseConstraintSolver;
class  btDiscreteDynamicsWorld;
class btSoftRigidDynamicsWorld;
class btCollisionShape;
class btCompoundShape;
class btRigidBody;
class btGhostObject;
class btGhostPairCallback;
class btTypedConstraint;
class btTriangleIndexVertexArray;
struct btSoftBodyWorldInfo;

class btCollisionObject;
class btManifoldPoint;
struct btCollisionObjectWrapper;
class btGeneric6DofSpringConstraint;


LN_NAMESPACE_BEGIN
class CollisionShape;

namespace detail {

class BtShapeManager
{
public:
	BtShapeManager();
	~BtShapeManager();
	void AddShape(CollisionShape* shape);
	bool IsEmpty() const;
	btCollisionShape* GetBtCollisionShape();

private:
	void Refresh();

	RefPtr<CollisionShape>		m_collisionShape;
	btCompoundShape*			m_btCompoundShape;
	btCollisionShape*			m_activeShape;
	bool						m_dirty;
};

} // namespace detail
LN_NAMESPACE_END

