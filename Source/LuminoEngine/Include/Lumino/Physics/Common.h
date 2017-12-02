
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
class PhysicsWorld2;

namespace detail {

class BtShapeManager
{
public:
	BtShapeManager();
	~BtShapeManager();
	void addShape(CollisionShape* shape);
	bool isEmpty() const;
	btCollisionShape* getBtCollisionShape();

private:
	void refresh();

	Ref<CollisionShape>		m_collisionShape;
	btCompoundShape*			m_btCompoundShape;
	btCollisionShape*			m_activeShape;
	bool						m_dirty;
};

} // namespace detail


enum class PhysicsResourceType
{
	RigidBody,
	CollisionBody,
	Joint,
};


/** */
class PhysicsResource2
	: public Object
{
	LN_OBJECT;

public:
	PhysicsWorld2* getWorld() const;

protected:
	virtual void onBeforeStepSimulation();
	virtual void onAfterStepSimulation();

LN_CONSTRUCT_ACCESS:
	PhysicsResource2(PhysicsResourceType type);
	virtual ~PhysicsResource2();
	void initialize();
	void setWorld(PhysicsWorld2* owner);
	PhysicsResourceType getPhysicsResourceType() const { return m_resourceType; }
	bool isRemovingFromWorld() const { return m_removingFromWorld; }
	void setRemovingFromWorld(bool value) { m_removingFromWorld = value; }

private:
	PhysicsResourceType	m_resourceType;
	PhysicsWorld2*		m_ownerWorld;
	bool				m_removingFromWorld;

	friend class PhysicsWorld2;
};

LN_NAMESPACE_END

